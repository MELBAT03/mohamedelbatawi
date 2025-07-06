#include "def.h"
#include "os.h"

#include "app_processor.h"
#include "app_display.h"
#include "app_user_interface.h"

#include "app_startup_display.h"
#include "app_setup_display.h"
#include "app_standby_display.h"
#include "app_nm_display.h"
#include "app_tm_display.h"
#include "app_fault_display.h"
#include "app_finish_display.h"

#include "handlers/output/inc/handler_display.h"

static void done_display_process(uint32_t period);

void app_display_update(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        app_startup_display_process,
        app_setup_display_process,
        app_standby_display_process,
        app_nm_display_process,
        app_tm_display_process,
        app_fault_display_process,
        app_finish_display_process,
        done_display_process};

    if (app_processor_entryFlag_get() == 0)
    {
        return;
    }

    if (handlers[app_processor_state_get()] != NULL)
    {
        handlers[app_processor_state_get()](period);
    }
}

void app_display_dashSymbols_put(void)
{
    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_CENTER_DASH);
    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_CENTER_DASH);
    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_CENTER_DASH);
    handler_display_ssdDP_set(SSD_DP_OFF);
}

void app_display_ssdSymbols_put(ssd_symbolEnum_t ssd1, ssd_symbolEnum_t ssd2, ssd_symbolEnum_t ssd3, ssd_symbolEnum_t ssd4)
{
    handler_display_ssd_clear();
    handler_display_clear();

    handler_display_ssd_set(SSD_DIGIT_0_ID, ssd4);
    handler_display_ssd_set(SSD_DIGIT_1_ID, ssd3);
    handler_display_ssd_set(SSD_DIGIT_2_ID, ssd2);
    handler_display_ssd_set(SSD_DIGIT_3_ID, ssd1);
}

void app_display_remainingTime_put(uint32_t period, uint32_t reminingTime, uint8_t dpState)
{
    static uint8_t Internal_Number_Of_Hours;
    static uint8_t Internal_Number_Of_Minutes;
    static uint32_t dpCnt = 0;

    Internal_Number_Of_Hours = (uint8_t)((uint32_t)reminingTime / 60U);
    Internal_Number_Of_Minutes = (uint8_t)((uint32_t)reminingTime % 60U);
    if (Internal_Number_Of_Minutes == 59)
    {
        Internal_Number_Of_Minutes = 0;
        Internal_Number_Of_Hours += 1;
    }
    else
    {
        Internal_Number_Of_Minutes += 1;
    }

    // Display Hours
    if (Internal_Number_Of_Hours)
    {
        if (ONE_OR_TWO_DIGIT(Internal_Number_Of_Hours))
        {
            handler_display_ssd_set(SSD_DIGIT_2_ID, Internal_Number_Of_Hours % 10);
            handler_display_ssd_set(SSD_DIGIT_3_ID, ONE_OR_TWO_DIGIT(Internal_Number_Of_Hours));
        }
        else
        {
            handler_display_ssd_set(SSD_DIGIT_2_ID, Internal_Number_Of_Hours);
            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
        }

        if (dpState != 0)
        {
            dpCnt += period;
            if (dpCnt > 1000)
            {
                dpCnt = 0;
                if (handler_display_ssdDP_get() == 0)
                {
                    handler_display_ssdDP_set(SSD_DP_ON);
                }
                else
                {
                    handler_display_ssdDP_set(SSD_DP_OFF);
                }
            }
        }
    }
    else
    {
        handler_display_ssdDP_set(SSD_DP_OFF);
        handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
        handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
    }

    // Display Minutes
    if (Internal_Number_Of_Minutes)
    {
        if (ONE_OR_TWO_DIGIT(Internal_Number_Of_Minutes))
        {
            handler_display_ssd_set(SSD_DIGIT_0_ID, Internal_Number_Of_Minutes % 10);
            handler_display_ssd_set(SSD_DIGIT_1_ID, ONE_OR_TWO_DIGIT(Internal_Number_Of_Minutes));
        }
        else
        {
            if (Internal_Number_Of_Hours > 0)
            {
                handler_display_ssd_set(SSD_DIGIT_0_ID, Internal_Number_Of_Minutes);
                handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_0_SYMBOL);
            }
            else
            {
                handler_display_ssd_set(SSD_DIGIT_0_ID, Internal_Number_Of_Minutes);
                handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
            }
        }
    }
    else
    {
        if (Internal_Number_Of_Hours > 0)
        {
            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_0_SYMBOL);
            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_0_SYMBOL);
        }
        else
        {
            // added 29-1-22
            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_1_SYMBOL);
            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
            handler_display_ssdDP_set(SSD_DP_OFF);
        }
    }
}

void app_display_ssd_charBlink(uint32_t period, uint8_t *buf, uint8_t bufCount, uint8_t blinkingNumber, uint16_t onTimeMS, uint16_t OffTimeMS)
{
    static handler_display_ssd_state_t status = SSD_STATUS_OFF;
    static uint32_t timeInSate = 0;
    static uint32_t counter = 0;

    timeInSate += period;

    switch (status)
    {
        case SSD_STATUS_INIT:
        {
            status = SSD_STATUS_ON;
            counter = 0;
            timeInSate = 0;
        }
        break;

        case SSD_STATUS_ON:
        {
            for (uint8_t i = 0; i < bufCount; i++)
            {
                handler_display_ssd_set(i, buf[i]);
            }
            if (timeInSate >= onTimeMS)
            {
                status = SSD_STATUS_OFF;
                counter++;
                timeInSate = 0;
            }
        }
        break;

        case SSD_STATUS_OFF:
        {
            handler_display_ssd_clear();
            if (timeInSate >= OffTimeMS)
            {
                status = SSD_STATUS_ON;
                counter++;
                timeInSate = 0;
                // blinking mean on off what it pass on only so multiply by 2 for counter on off
                if (counter >= blinkingNumber * 2)
                {
                    status = SSD_STATUS_FINISH;
                }
            }
        }
        break;

        case SSD_STATUS_FINISH:
        {
            status = SSD_STATUS_INIT;
            counter = 0;
            timeInSate = 0;
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}

void app_display_ssd_blink(uint32_t period, uint16_t Disp_number, uint8_t blinkingNumber, uint16_t OnTimeMS, uint16_t OffTimeMS, handler_display_ssdFormat_t FormatSel)
{
    static handler_display_ssd_state_t status = SSD_STATUS_OFF;
    static uint32_t timeInSate = 0;
    static uint32_t counter = 0;

    timeInSate += period;

    switch (status)
    {
        case SSD_STATUS_INIT:
            status = SSD_STATUS_ON;
            counter = 0;
            timeInSate = 0;
            break;
        case SSD_STATUS_ON:
            app_display_ssd_put(Disp_number, FormatSel);
            if (timeInSate >= OnTimeMS)
            {
                status = SSD_STATUS_OFF;
                counter++;
                timeInSate = 0;
            }
            break;
        case SSD_STATUS_OFF:
            handler_display_ssd_clear();
            if (timeInSate >= OffTimeMS)
            {
                status = SSD_STATUS_ON;
                counter++;
                timeInSate = 0;
                // blinking mean on off what it pass on only so multiply by 2 for counter on off
                if (counter >= blinkingNumber * 2)
                {
                    status = SSD_STATUS_FINISH;
                }
            }
            break;

        case SSD_STATUS_FINISH:
            status = SSD_STATUS_INIT;
            counter = 0;
            timeInSate = 0;

            break;

        default:
            break;
    }
}

void app_display_led_blink(app_display_ledBlink_t *led, uint32_t period, uint16_t OnTimeMS, uint16_t OffTimeMS, uint32_t value)
{
    led->timeInState += period;

    switch (led->status)
    {
        case LED_STATUS_ON:
        {
            handler_display_ledGroup_set((handler_display_ledGroup_t)led->ledID, value);
            if (led->timeInState >= OnTimeMS)
            {
                led->status = LED_STATUS_OFF;
                led->timeInState = 0;
            }
        }
        break;

        case LED_STATUS_OFF:
        {
            handler_display_ledGroup_set((handler_display_ledGroup_t)led->ledID, 0);
            if (led->timeInState >= OffTimeMS)
            {
                led->status = LED_STATUS_ON;
                led->timeInState = 0;
            }
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}

void app_display_ssd_put(uint16_t Disp_number, handler_display_ssdFormat_t FormatSel)
{
    static uint8_t buff_of_digits[4], i;

    for (i = 0; i < 4; i++)
    {
        buff_of_digits[i] = (uint8_t)(Disp_number % 10);
        Disp_number /= 10;
    }

    switch (FormatSel)
    {
        case FORMAT_HOURS:
            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_0_SYMBOL);
            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_0_SYMBOL);

            handler_display_ssd_set(SSD_DIGIT_2_ID, buff_of_digits[0]);
            if (buff_of_digits[1])
            {
                handler_display_ssd_set(SSD_DIGIT_3_ID, (ssd_symbolEnum_t)buff_of_digits[1]);
            }
            else
            {
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
            }
            handler_display_ssdDP_set(SSD_DP_ON);

            break;
        case FORMAT_NUMBERS:
            handler_display_ssd_set(SSD_DIGIT_0_ID, buff_of_digits[0]);
            Disp_number /= 10;

            if (buff_of_digits[1])
            {
                // handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_0_SYMBOL);
                handler_display_ssd_set(SSD_DIGIT_1_ID, (ssd_symbolEnum_t)buff_of_digits[1]);
            }
            else
            {
                handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
            }
            if (buff_of_digits[2])
            {
                handler_display_ssd_set(SSD_DIGIT_2_ID, (ssd_symbolEnum_t)buff_of_digits[2]);
            }
            else
            {
                handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
            }
            if (buff_of_digits[3])
            {
                handler_display_ssd_set(SSD_DIGIT_3_ID, (ssd_symbolEnum_t)buff_of_digits[3]);
            }
            else
            {
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
            }
            break;
        case FORMAT_NUMBERS_WITH_DOUBLE_DOT:

            handler_display_ssdDP_set(SSD_DP_ON);
            handler_display_ssd_set(SSD_DIGIT_0_ID, (uint8_t)(Disp_number % 10U));

            // debug = Disp_number;

            if (Disp_number)
            {
                // handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_0_SYMBOL);
                handler_display_ssd_set(SSD_DIGIT_1_ID, (ssd_symbolEnum_t)Disp_number % 10);
            }
            else
            {
                handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
            }
            if (Disp_number)
            {
                handler_display_ssd_set(SSD_DIGIT_2_ID, (ssd_symbolEnum_t)Disp_number % 10);
            }
            else
            {
                handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
            }
            if (Disp_number)
            {
                handler_display_ssd_set(SSD_DIGIT_3_ID, (ssd_symbolEnum_t)Disp_number % 10);
            }
            else
            {
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
            }
            break;
        case FORMAT_NUMBERS_TEST:
            handler_display_ssd_set(SSD_DIGIT_0_ID, buff_of_digits[0]);
            Disp_number /= 10;   // ???

            // handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_0_SYMBOL);
            if (buff_of_digits[1] == 0 && buff_of_digits[2] == 0 && buff_of_digits[3] == 0)
            {
                handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
            }
            else
            {
                handler_display_ssd_set(SSD_DIGIT_1_ID, (ssd_symbolEnum_t)buff_of_digits[1]);
            }
            if (buff_of_digits[2] == 0 && buff_of_digits[3] == 0)
            {
                handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
            }
            else
            {
                handler_display_ssd_set(SSD_DIGIT_2_ID, (ssd_symbolEnum_t)buff_of_digits[2]);
            }

            if (buff_of_digits[3] == 0)
            {
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
            }
            else
            {
                handler_display_ssd_set(SSD_DIGIT_3_ID, (ssd_symbolEnum_t)buff_of_digits[3]);
            }
            break;
        default:
            break;
    }
}

void app_display_weightDetectionPattern_put(uint32_t period)
{
    static uint32_t time_in_state = 0;
    time_in_state += period;
    static uint8_t counter = 0;

    if (counter == 0)
    {
        time_in_state = 0;
    }

    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_CENTER_DASH);
    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_CENTER_DASH);
    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_CENTER_DASH);

    if (time_in_state <= 200)
    {
        handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, 1);
        counter++;
    }
    else if (time_in_state > 200 && time_in_state <= 400)
    {
        handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, 2);
        counter++;
    }
    else if (time_in_state > 400 && time_in_state <= 600)
    {
        handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, 4);
        counter++;
    }
    else if (time_in_state > 600 && time_in_state <= 800)
    {
        handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, 6);
        counter++;
    }
    else if (time_in_state > 800 && time_in_state <= 1000)
    {
        handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, 8);
        counter++;
    }
    else if (time_in_state > 1000 && time_in_state <= 1200)
    {
        handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, 6);
        counter++;
    }
    else if (time_in_state > 1200 && time_in_state <= 1400)
    {
        handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, 4);
        counter++;
    }
    else if (time_in_state > 1400 && time_in_state <= 1600)
    {
        handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, 2);
        counter++;
    }
    else if (time_in_state > 1800)
    {
        time_in_state = 0;
        counter = 0;
    }
}

void app_display_fire(void)
{
    handler_display_fire();
}

void app_display_clear(void)
{
    handler_display_clear();
}

static void done_display_process(uint32_t period)
{
    handler_display_clear();
}
