#include "def.h"

#include "app_test_mode.h"

#include "app_processor.h"
#include "app_user_interface.h"

#include "app_buzzer.h"

#include "handlers/input/inc/handler_off_sw.h"
#include "handlers/input/inc/handler_power_detection.h"

#define USER_INTERFACE_MODE_CHECKING_PERIOD_MSEC (200U)

static app_processor_state_t tm_processor_transitions_handle(void);

static app_tm_state_t gx_tm_state = APP_TEST_MODE_INIT;

static app_tm_type_t gx_tm_type = APP_TM_NONE;

uint8_t *fnSW[APP_TEST_MODE_FUNCTION_TEST_MAX_NUM];

app_processor_state_t app_testMode_process(uint32_t period)
{
    app_processor_state_t ret = APP_PROCESSOR_TEST_MODE;

    static void (*const handlers[])(uint32_t period) = {
        NULL,
        app_tm_waterLevelCalibration_process,
        app_tm_weightDetectionCalibration_process,
        app_tm_lvCalibrationTest_process,
        app_tm_electricalParts_process,
        app_tm_unbalanceSpin_process,
        app_tm_waterLevelFreq_process,
        app_tm_pcb_process,
        app_tm_middleInspection_process,
        app_tm_finalInspection_process,
        app_tm_errorCnt_process,
        app_tm_cycleCnt_process};

    if (handlers[gx_tm_type] != NULL)
    {
        handlers[gx_tm_type](period);
    }

    ret = tm_processor_transitions_handle();

    return ret;
}

uint8_t app_tm_entry_check(uint32_t period)
{
    static uint32_t timeout = 0;
    uint8_t ret = 0;

    if ((app_userInterface_keyState_get()->waterLevelKeyState != HANDLER_KEY_RELEASED) || (app_userInterface_keyState_get()->waterTempKeyState != HANDLER_KEY_RELEASED) || (app_userInterface_keyState_get()->rinseKeyState != HANDLER_KEY_RELEASED) || (app_userInterface_keyState_get()->spinKeyState != HANDLER_KEY_RELEASED))
    {
        if (timeout > USER_INTERFACE_MODE_CHECKING_PERIOD_MSEC)
        {
            if (app_userInterface_PressedKeysCnt_get() >= 2U)
            {
                ret = 1;
            }
        }
        else
        {
            timeout += period;
        }
    }
    else
    {
        timeout = 0;
    }

    return ret;
}

app_tm_type_t app_tm_type_get(void)
{
    return gx_tm_type;
}

void app_tm_type_set(app_tm_type_t type)
{
    gx_tm_type = type;
}

app_tm_state_t app_tm_state_get(void)
{
    return gx_tm_state;
}

void app_tm_state_set(app_tm_state_t testModeState)
{
    gx_tm_state = testModeState;
}

static app_processor_state_t tm_processor_transitions_handle(void)
{
    app_processor_state_t ret = APP_PROCESSOR_TEST_MODE;
    handler_offSW_state_t offSW = handler_offSW_state_get();

    if (handler_powerDetection_state_get() == HANDLER_POWER_DETECTION_NOK)
    {
        app_processor_shutdownState_set(MEMORY_SHUTDOWN_NORMAL);
        ret = APP_PROCESSOR_DONE_MODE;
    }
    else if (offSW == HANDLER_OFF_SW_PRESSED)
    {
        app_processor_shutdownState_set(MEMORY_SHUTDOWN_NORMAL);
        ret = APP_PROCESSOR_FINISH_MODE;
        app_buzzer_tone_set(APP_BUZZER_TONE_SHUTDOWN);
    }
    else if ((gx_tm_state == APP_TEST_MODE_DONE) && ((gx_tm_type == APP_TM_WATER_LEVEL_CALIBRATION) || (gx_tm_type == APP_TM_LV_CALIBRATION) || (gx_tm_type == APP_TM_WEIGHT_CALIBRATION) || (gx_tm_type == APP_TM_MIDDLE_INSPECTION) || (gx_tm_type == APP_TM_FINAL_INSPECTION)))
    {
        // NOTE: below is for automatic shut down
        ret = APP_PROCESSOR_FINISH_MODE;
        app_buzzer_tone_set(APP_BUZZER_TONE_SHUTDOWN);
    }
    else
    {
        // do nothing
    }

    return ret;
}
