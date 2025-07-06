#include "def.h"
#include "app_nm_data_display.h"
#include "app_nm_user_interface.h"

#include "app_nm.h"
#include "app_buzzer.h"

#include "app_display.h"

#include "app_user_interface.h"

#include "handlers/input/inc/handler_mems.h"
#include "handlers/input/inc/handler_water_level.h"
#include "handlers/input/inc/handler_load_sense.h"
#include "handlers/output/inc/handler_motor.h"
#include "handlers/input/inc/handler_acin.h"

#define DATA_DISPLAY_BUTTON_PRESS_TIME_MSEC (5000U)

typedef enum
{
    LOAD_DATA_GRAMS,
    LOAD_DATA_RAW
} app_display_loadData_t;

static app_nm_dataDisplay_state_t gx_app_nm_UIDataDisplay_state = APP_NM_DATA_DISPLAY_STANDBY;

void app_nm_userInterface_dataDisplay_handle(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->washKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);

        if (gx_app_nm_UIDataDisplay_state < APP_NM_DATA_DISPLAY_CALIBRATED_WATER_LEVEL)
        {
            gx_app_nm_UIDataDisplay_state++;
        }
        else
        {
            gx_app_nm_UIDataDisplay_state = APP_NM_DATA_DISPLAY_STANDBY;
        }
    }
    else if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->rinseKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);

        if (gx_app_nm_UIDataDisplay_state > APP_NM_DATA_DISPLAY_STANDBY)
        {
            gx_app_nm_UIDataDisplay_state--;
        }
        else
        {
            gx_app_nm_UIDataDisplay_state = APP_NM_DATA_DISPLAY_CALIBRATED_WATER_LEVEL;
        }
    }
    else if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->spinKeyEdgeState)
    {
        if (gx_app_nm_UIDataDisplay_state == APP_NM_DATA_DISPLAY_MEMS_UNBALANCE_X)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            if (handler_mems_xThresholdValue_get() > 10U)
            {
                handler_mems_xThreshold_set(handler_mems_xThresholdValue_get() - 10U);
            }
        }
        else if (gx_app_nm_UIDataDisplay_state == APP_NM_DATA_DISPLAY_MEMS_UNBALANCE_Y)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            if (handler_mems_yThresholdValue_get() > 10U)
            {
                handler_mems_yThreshold_set(handler_mems_yThresholdValue_get() - 10U);
            }
        }
        else if (gx_app_nm_UIDataDisplay_state == APP_NM_DATA_DISPLAY_MEMS_UNBALANCE_Z)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            if (handler_mems_zThresholdValue_get() > 10U)
            {
                handler_mems_zThreshold_set(handler_mems_zThresholdValue_get() - 10U);
            }
        }
        else if (gx_app_nm_UIDataDisplay_state == APP_NM_DATA_DISPLAY_MEMS_NOTMAL_HITS)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            if (handler_mems_hitCnt_get() > 1U)
            {
                handler_mems_hitCnt_set(handler_mems_hitCnt_get() - 1U);
            }
        }
        else if (gx_app_nm_UIDataDisplay_state == APP_NM_DATA_DISPLAY_MEMS_CRITICAL_HITS)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            if (handler_mems_critHitCnt_get() > 1U)
            {
                handler_mems_critHitCnt_set(handler_mems_critHitCnt_get() - 1U);
            }
        }
        else
        {
            // do nothing
        }
    }
    else if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->waterTempKeyEdgeState)
    {
        if (gx_app_nm_UIDataDisplay_state == APP_NM_DATA_DISPLAY_MEMS_UNBALANCE_X)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            if (handler_mems_xThresholdValue_get() < 9000U)
            {
                handler_mems_xThreshold_set(handler_mems_xThresholdValue_get() + 10U);
            }
        }
        else if (gx_app_nm_UIDataDisplay_state == APP_NM_DATA_DISPLAY_MEMS_UNBALANCE_Y)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            if (handler_mems_yThresholdValue_get() < 9000U)
            {
                handler_mems_yThreshold_set(handler_mems_yThresholdValue_get() + 10U);
            }
        }
        else if (gx_app_nm_UIDataDisplay_state == APP_NM_DATA_DISPLAY_MEMS_UNBALANCE_Z)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            if (handler_mems_zThresholdValue_get() < 9000U)
            {
                handler_mems_zThreshold_set(handler_mems_zThresholdValue_get() + 10U);
            }
        }
        else if (gx_app_nm_UIDataDisplay_state == APP_NM_DATA_DISPLAY_MEMS_NOTMAL_HITS)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            if (handler_mems_hitCnt_get() < 255U)
            {
                handler_mems_hitCnt_set(handler_mems_hitCnt_get() + 1U);
            }
        }
        else if (gx_app_nm_UIDataDisplay_state == APP_NM_DATA_DISPLAY_MEMS_CRITICAL_HITS)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            if (handler_mems_critHitCnt_get() < 255U)
            {
                handler_mems_critHitCnt_set(handler_mems_critHitCnt_get() + 1U);
            }
        }
        else
        {
            // do nothing
        }
    }
    else
    {
        // do nothing
    }
}

void app_nm_display_dataDisplay_handle(uint32_t period)
{
    static app_display_loadData_t loadDataState = LOAD_DATA_GRAMS;
    static uint32_t loadDataDisplayCnt = 0;
    uint16_t xValue = 0;
    uint16_t yValue = 0;
    uint16_t zValue = 0;

    switch (gx_app_nm_UIDataDisplay_state)
    {
        case APP_NM_DATA_DISPLAY_STANDBY:
        {
            handler_display_ssd_clear();
            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_A_SYMBOL);
            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_t_SYMBOL);
            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_A_SYMBOL);
            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_d_SYMBOL);
        }
        break;

        case APP_NM_DATA_DISPLAY_WATER_LEVEL_FREQ:
        {
            handler_display_ssd_clear();
            app_display_ssd_put(handler_waterLevel_currentFreq_get() / 10, FORMAT_NUMBERS_TEST);
        }
        break;

        case APP_NM_DATA_DISPLAY_LOAD_SENSE_DATA:
        {
            handler_display_ssd_clear();
            loadDataDisplayCnt += period;
            switch (loadDataState)
            {
                case LOAD_DATA_GRAMS:
                {
                    app_display_ssd_put(handler_motor_weightFeedback_get(), FORMAT_NUMBERS_TEST);
                    if (loadDataDisplayCnt >= 1000U)
                    {
                        loadDataDisplayCnt = 0;
                        loadDataState = LOAD_DATA_RAW;
                    }
                }
                break;

                case LOAD_DATA_RAW:
                {
                    app_display_ssd_put(handler_motor_weightRawFeedback_get(), FORMAT_NUMBERS_TEST);
                    if (loadDataDisplayCnt >= 1000U)
                    {
                        loadDataDisplayCnt = 0;
                        loadDataState = LOAD_DATA_GRAMS;
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
        break;

        case APP_NM_DATA_DISPLAY_MOTOR_RPM:
        {
            handler_display_ssd_clear();
            app_display_ssd_put(handler_motor_speed_get(), FORMAT_NUMBERS_TEST);
        }
        break;

        case APP_NM_DATA_DISPLAY_MEMS_X:
        {
            handler_display_ssd_clear();
            if (handler_mems_xValue_get() < 0)
            {
                xValue = -1 * handler_mems_xValue_get();
                app_display_ssd_put((xValue / 100), FORMAT_NUMBERS_TEST);
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_CENTER_DASH);
            }
            else
            {
                xValue = handler_mems_xValue_get();
                app_display_ssd_put((xValue / 100), FORMAT_NUMBERS_TEST);
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
            }
        }
        break;

        case APP_NM_DATA_DISPLAY_MEMS_Y:
        {
            handler_display_ssd_clear();
            if (handler_mems_yValue_get() < 0)
            {
                yValue = -1 * handler_mems_yValue_get();
                app_display_ssd_put((yValue / 100), FORMAT_NUMBERS_TEST);
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_CENTER_DASH);
            }
            else
            {
                yValue = handler_mems_yValue_get();
                app_display_ssd_put((yValue / 100), FORMAT_NUMBERS_TEST);
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
            }
        }
        break;

        case APP_NM_DATA_DISPLAY_MEMS_Z:
        {
            handler_display_ssd_clear();
            if (handler_mems_zValue_get() < 0)
            {
                zValue = -1 * handler_mems_zValue_get();
                app_display_ssd_put((zValue / 100), FORMAT_NUMBERS_TEST);
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_CENTER_DASH);
            }
            else
            {
                zValue = handler_mems_zValue_get();
                app_display_ssd_put((zValue / 100), FORMAT_NUMBERS_TEST);
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
            }
        }
        break;

        case APP_NM_DATA_DISPLAY_MEMS_UNBALANCE_X:
        {
            handler_display_ssd_clear();
            app_display_ssd_put(handler_mems_xThresholdValue_get(), FORMAT_NUMBERS_TEST);
        }
        break;

        case APP_NM_DATA_DISPLAY_MEMS_UNBALANCE_Y:
        {
            handler_display_ssd_clear();
            app_display_ssd_put(handler_mems_yThresholdValue_get(), FORMAT_NUMBERS_TEST);
        }
        break;

        case APP_NM_DATA_DISPLAY_MEMS_UNBALANCE_Z:
        {
            handler_display_ssd_clear();
            app_display_ssd_put(handler_mems_zThresholdValue_get(), FORMAT_NUMBERS_TEST);
        }
        break;

        case APP_NM_DATA_DISPLAY_MEMS_NOTMAL_HITS:
        {
            handler_display_ssd_clear();
            app_display_ssd_put(handler_mems_hitCnt_get(), FORMAT_NUMBERS_TEST);
        }
        break;

        case APP_NM_DATA_DISPLAY_MEMS_CRITICAL_HITS:
        {
            handler_display_ssd_clear();
            app_display_ssd_put(handler_mems_critHitCnt_get(), FORMAT_NUMBERS_TEST);
        }
        break;

        case APP_NM_DATA_DISPLAY_MEMS_CALIBRATED_X:
        {
            handler_display_ssd_clear();
            if (handler_mems_xCalibratedValue_get() < 0)
            {
                xValue = -1 * handler_mems_xCalibratedValue_get();
                app_display_ssd_put((xValue / 100), FORMAT_NUMBERS_TEST);
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_CENTER_DASH);
            }
            else
            {
                xValue = handler_mems_xCalibratedValue_get();
                app_display_ssd_put((xValue / 100), FORMAT_NUMBERS_TEST);
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
            }
        }
        break;

        case APP_NM_DATA_DISPLAY_MEMS_CALIBRATED_Y:
        {
            handler_display_ssd_clear();
            if (handler_mems_yCalibratedValue_get() < 0)
            {
                yValue = -1 * handler_mems_yCalibratedValue_get();
                app_display_ssd_put((yValue / 100), FORMAT_NUMBERS_TEST);
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_CENTER_DASH);
            }
            else
            {
                yValue = handler_mems_yCalibratedValue_get();
                app_display_ssd_put((yValue / 100), FORMAT_NUMBERS_TEST);
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
            }
        }
        break;

        case APP_NM_DATA_DISPLAY_MEMS_CALIBRATED_Z:
        {
            handler_display_ssd_clear();
            if (handler_mems_zCalibratedValue_get() < 0)
            {
                zValue = -1 * handler_mems_zCalibratedValue_get();
                app_display_ssd_put((zValue / 100), FORMAT_NUMBERS_TEST);
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_CENTER_DASH);
            }
            else
            {
                zValue = handler_mems_zCalibratedValue_get();
                app_display_ssd_put((zValue / 100), FORMAT_NUMBERS_TEST);
                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
            }
        }
        break;

        case APP_NM_DATA_DISPLAY_MEMS_FAULT_X:
        {
            handler_display_ssd_clear();
            app_display_ssd_put(handler_mems_faultValue_get(APP_MEMS_FAULT_X) / 10, FORMAT_NUMBERS_TEST);
        }
        break;

        case APP_NM_DATA_DISPLAY_MEMS_FAULT_Y:
        {
            handler_display_ssd_clear();
            app_display_ssd_put(handler_mems_faultValue_get(APP_MEMS_FAULT_Y) / 10, FORMAT_NUMBERS_TEST);
        }
        break;

        case APP_NM_DATA_DISPLAY_MEMS_FAULT_Z:
        {
            handler_display_ssd_clear();
            app_display_ssd_put(handler_mems_faultValue_get(APP_MEMS_FAULT_Z) / 10, FORMAT_NUMBERS_TEST);
        }
        break;

        case APP_NM_DATA_DISPLAY_ACIN:
        {
            handler_display_ssd_clear();
            app_display_ssd_put(handler_acin_value_get(), FORMAT_NUMBERS_TEST);
        }
        break;

        case APP_NM_DATA_DISPLAY_INTERNAL_WATER_LEVEL:
        {
            handler_display_ssd_clear();
            app_display_ssd_put(app_userInterface_data_get()->normalMode.wash.waterLevelSelection, FORMAT_NUMBERS_TEST);
        }
        break;

        case APP_NM_DATA_DISPLAY_CALIBRATED_WATER_LEVEL:
        {
            handler_display_ssd_clear();
            app_display_ssd_put((app_memory_restWaterLevel_get() & 0x00ffffff) / 10, FORMAT_NUMBERS_TEST);
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}

app_nm_userInterface_state_t app_nm_userInterface_dataDisplay_entry_check(uint32_t period)
{
    app_nm_userInterface_state_t ret = APP_NM_USER_INTERFACE_NORMAL;
    static uint32_t dataDisplayCnt = 0;

    if (APP_NORMAL_MODE_PAUSE != app_nm_state_get())
    {
        if (HANDLER_KEY_RELEASED != app_userInterface_keyState_get()->waterLevelKeyState)
        {
            dataDisplayCnt += period;
            if (dataDisplayCnt >= DATA_DISPLAY_BUTTON_PRESS_TIME_MSEC)
            {
                dataDisplayCnt = 0;
                gx_app_nm_UIDataDisplay_state = APP_NM_DATA_DISPLAY_STANDBY;
                ret = APP_NM_USER_INTERFACE_DATA_DISPLAY;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
            }
        }
        else
        {
            dataDisplayCnt = 0;
        }
    }
    else
    {
        dataDisplayCnt = 0;
    }

    return ret;
}

app_nm_userInterface_state_t app_nm_userInterface_dataDisplay_exit_check(void)
{
    app_nm_userInterface_state_t ret = APP_NM_USER_INTERFACE_DATA_DISPLAY;
    if (APP_NORMAL_MODE_PAUSE == app_nm_state_get())
    {
        gx_app_nm_UIDataDisplay_state = APP_NM_DATA_DISPLAY_STANDBY;
        ret = APP_NM_USER_INTERFACE_NORMAL;
    }

    return ret;
}

app_nm_dataDisplay_state_t app_nm_userInterface_dataDisplayState_get(void)
{
    return gx_app_nm_UIDataDisplay_state;
}
