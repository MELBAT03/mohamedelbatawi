#include "def.h"

#include "app_standby_display.h"
#include "app_display.h"
#include "app_nm.h"
#include "app_fill.h"

#include "app_user_interface.h"

#include "handlers/output/inc/handler_display.h"
#include "handlers/output/inc/handler_lock_motor.h"
#include "handlers/input/inc/handler_softener_sensor.h"
#include "handlers/input/inc/handler_detergent_sensor.h"

static void standby_display_handle(uint32_t period);

void app_standby_display_process(uint32_t period)
{
    if (app_userInterface_data_get()->normalMode.common.childLockSelection == APP_NORMAL_MODE_UI_CHILD_LOCK_ON)
    {
        handler_display_dimState_set(0);
        app_display_clear();
        handler_display_ledGroup_set(HANDLER_DISPLAY_LID_LOCK_LED_GROUP, handler_lockMotor_state_get());
        handler_display_ledGroup_set(HANDLER_DISPLAY_CHILD_LOCK_LED_GROUP, app_userInterface_data_get()->normalMode.common.childLockSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_BUTTON_LED_GROUP, 1);
    }
    else
    {
        handler_display_dimState_set(1);
        standby_display_handle(period);
    }
}

static void standby_display_handle(uint32_t period)
{
    static app_display_ledBlink_t courseBlinking = {.ledID = HANDLER_DISPLAY_COURSE_LED_GROUP, .status = LED_STATUS_ON, .timeInState = 0};
    static app_display_ledBlink_t tubcleanBlinking = {.ledID = HANDLER_DISPLAY_TUBCLEAN_LED_GROUP, .status = LED_STATUS_ON, .timeInState = 0};
    static app_display_ledBlink_t steamTechBlinking = {.ledID = HANDLER_DISPLAY_STEAM_TECH_LED_GROUP, .status = LED_STATUS_ON, .timeInState = 0};
#ifdef AUTO_DETERGENT_HEATER_MODEL
    static app_display_ledBlink_t gelBlinking = {.ledID = HANDLER_DISPLAY_GEL_DETERGENT_LED_GROUP, .status = LED_STATUS_ON, .timeInState = 0};
    static app_display_ledBlink_t softenerBlinking = {.ledID = HANDLER_DISPLAY_SOFTNER_LED_GROUP, .status = LED_STATUS_ON, .timeInState = 0};
    static uint16_t gelTotalBlinkingTimeoutMSec = 0;
    static uint16_t softenerTotalBlinkingTimeoutMSec = 0;
#endif
    static uint32_t timeoutMSec = 0;
    ssd_symbolEnum_t data[3];

    uint32_t totalRemainingTimeForCourse;

    totalRemainingTimeForCourse = app_nm_data_get()->totalTimeMSec / 60000U;

    if ((HANDLER_KEY_RELEASED != app_userInterface_courseKeyState_get()))
    {
        courseBlinking.timeInState = 0;
        courseBlinking.status = LED_STATUS_ON;
        timeoutMSec = 0;
    }

    switch (app_userInterface_state_get())
    {
        case UI_ALTERNATE_NONE:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 0);

            if ((app_userInterface_data_get()->normalMode.wash.waterLevelSelection != APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0) && totalRemainingTimeForCourse != 0U)
            {
                app_display_remainingTime_put(period, totalRemainingTimeForCourse, 0);
            }
            else
            {
                app_display_dashSymbols_put();
            }
        }
        break;

        case UI_ALTERNATE_DELAY_START:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 0);

            app_display_ssd_blink(period, (app_userInterface_data_get()->normalMode.common.delayStartHourSelection), 50, 500, 500, FORMAT_HOURS);
        }
        break;

        case UI_ALTERNATE_SOAK:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 0);

            app_display_ssd_blink(period, app_userInterface_data_get()->normalMode.wash.soakSelection, 50, 500, 500, FORMAT_HOURS);
        }
        break;

        case UI_ALTERNATE_ANTI_WRINKLE:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 0);

            app_display_ssd_blink(period, (app_userInterface_data_get()->normalMode.wash.antiWrinkleSelection), 50, 500, 500, FORMAT_NUMBERS);
        }
        break;

        case UI_ALTERNATE_EXTRA_SPIN:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 0);

            app_display_ssd_blink(period, (app_userInterface_data_get()->normalMode.wash.superSpinSelection), 50, 500, 500, FORMAT_NUMBERS);
        }
        break;

        case UI_ALTERNATE_WASH:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 0);

            app_display_ssd_blink(period, (app_userInterface_data_get()->normalMode.wash.washSelection), 50, 500, 500, FORMAT_NUMBERS);
        }
        break;

        case UI_ALTERNATE_RINSE:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 1);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 0);

            if (app_userInterface_data_get()->normalMode.wash.extraRinseSelection == APP_NORMAL_MODE_UI_RINSE_EXTENDED)
            {
                data[2] = SSD_P_SYMBOL;
                data[1] = SSD_r_SYMBOL;
                data[0] = SSD_o_SYMBOL;
                app_display_ssd_charBlink(period, (uint8_t *)data, 3, 50, 500, 500);
            }
            else
            {
                app_display_ssd_blink(period, (app_userInterface_data_get()->normalMode.wash.rinseSelection), 50, 500, 500, FORMAT_NUMBERS);
            }
        }
        break;

        case UI_ALTERNATE_SPIN:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 1);

            app_display_ssd_blink(period, (app_userInterface_data_get()->normalMode.wash.spinSelection), 50, 500, 500, FORMAT_NUMBERS);
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }

    if (timeoutMSec < 5000U)
    {
        timeoutMSec += period;
        app_display_led_blink(&courseBlinking, period, 1000, 500, app_userInterface_data_get()->normalMode.common.courseSelection);
    }
    else
    {
        handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, app_userInterface_data_get()->normalMode.common.courseSelection);
    }

    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterTempSelection);
    handler_display_ledGroup_set(HANDLER_DISPLAY_LID_LOCK_LED_GROUP, handler_lockMotor_state_get());
    handler_display_ledGroup_set(HANDLER_DISPLAY_CHILD_LOCK_LED_GROUP, app_userInterface_data_get()->normalMode.common.childLockSelection);
    handler_display_ledGroup_set(HANDLER_DISPLAY_DELAY_START_LED_GROUP, app_userInterface_data_get()->normalMode.common.delayStartHourSelection);
    handler_display_ledGroup_set(HANDLER_DISPLAY_HEATER_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterHeatingSelection);

#ifdef AUTO_DETERGENT_HEATER_MODEL
    if (app_fill_autoSoftenerState_get() == APP_FILL_AUTO_SOFTENER_LOW && (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF))
    {
        if (softenerTotalBlinkingTimeoutMSec > 1500 * 4)
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_SOFTNER_LED_GROUP, 0);
        }
        else
        {
            softenerTotalBlinkingTimeoutMSec += period;
            app_display_led_blink(&softenerBlinking, period, 1000, 500, 1);
        }
    }
    else if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_ON)
    {
        handler_display_ledGroup_set(HANDLER_DISPLAY_SOFTNER_LED_GROUP, 0);
    }
    else
    {
        handler_display_ledGroup_set(HANDLER_DISPLAY_SOFTNER_LED_GROUP, 1);
    }

    if (app_fill_autoDetergentState_get() == APP_FILL_AUTO_DETERGENT_LOW && (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF))
    {
        if (gelTotalBlinkingTimeoutMSec > 1500 * 4)
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_LED_GROUP, app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_POWDER_DETERGENT_LED_GROUP, !app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
        }
        else
        {
            gelTotalBlinkingTimeoutMSec += period;
            app_display_led_blink(&gelBlinking, period, 1000, 500, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_POWDER_DETERGENT_LED_GROUP, 1);
        }
    }
    else
    {
        handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_LED_GROUP, app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_POWDER_DETERGENT_LED_GROUP, !app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
    }
#endif

    app_display_led_blink(&tubcleanBlinking, period, 1000, 1000, app_userInterface_data_get()->normalMode.common.tubcleanSelection);

    if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_ON)
    {
        app_display_led_blink(&steamTechBlinking, period, 2000, 2000, app_userInterface_data_get()->normalMode.wash.steamTechSelection);
    }
    else
    {
        handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_LED_GROUP, app_userInterface_data_get()->normalMode.wash.steamTechSelection);
    }

    handler_display_ledGroup_set(HANDLER_DISPLAY_STAIN_LEVEL_LED_GROUP, app_userInterface_data_get()->normalMode.wash.soilLevelSelection);

    handler_display_ledGroup_set(HANDLER_DISPLAY_EXTRA_SPIN_LED_GROUP, app_userInterface_data_get()->normalMode.wash.superSpinSelection);
    handler_display_ledGroup_set(HANDLER_DISPLAY_SOAK_LED_GROUP, app_userInterface_data_get()->normalMode.wash.soakSelection);

    handler_display_ledGroup_set(HANDLER_DISPLAY_ON_OFF_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_START_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_STAIN_LEVEL_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_TUBCLEAN_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_DELAY_START_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_BUTTON_LED_GROUP, 1);
}
