#include "def.h"
#include "app_nm.h"
#include "app_user_interface.h"
#include "app_nm_user_interface.h"
#include "app_motor.h"
#include "app_nm_init.h"
#include "app_buzzer.h"

#ifdef LINT_FILTER_ENABLED
#include "handlers/input/inc/handler_lint_filter_sensor.h"
#endif

#define INIT_ONE_MIN_MSEC (60000UL)

static app_nm_state_t normalMode_init_pauseRequest_handle(app_nm_state_t state);

static void init_init_handle(uint32_t period);
static void init_motorClutchReset_handle(uint32_t period);
#ifdef LINT_FILTER_ENABLED
static void init_lintFilterCheck_handle(uint32_t period);
#endif
static void init_delayStart_handle(uint32_t period);
static void init_finish_handle(uint32_t period);

static app_nm_init_state_t gx_init_state = APP_NORMAL_MODE_INIT_INIT;

#ifdef LINT_FILTER_ENABLED
static app_nm_init_lintFilterCheckState_t gx_init_lintFilterCheckState = LINT_FILTER_CHECK_INIT;
#endif

app_nm_state_t app_nm_init_process(uint32_t period)
{
    app_nm_state_t ret = APP_NORMAL_MODE_INIT;

    switch (gx_init_state)
    {
        case APP_NORMAL_MODE_INIT_INIT:
        {
            init_init_handle(period);
        }
        break;

        case APP_NORMAL_MODE_INIT_MOTOR_CLUTCH_RESET:
        {
            init_motorClutchReset_handle(period);
        }
        break;

#ifdef LINT_FILTER_ENABLED
        case APP_NORMAL_MODE_INIT_LINT_FILTER_CHECK:
        {
            init_lintFilterCheck_handle(period);
        }
        break;
#endif

        case APP_NORMAL_MODE_INIT_DELAY_START:
        {
            init_delayStart_handle(period);
        }
        break;

        case APP_NORMAL_MODE_INIT_FINISH:
        {
            init_finish_handle(period);
        }
        break;

        case APP_NORMAL_MODE_INIT_DONE:
        {
            if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_ON)
            {
                ret = APP_NORMAL_MODE_TUBCLEAN;
            }
            else if ((APP_NORMAL_MODE_UI_OPERATION_WASH_ONLY == app_userInterface_data_get()->normalMode.common.operationSelection) || (APP_NORMAL_MODE_UI_OPERATION_WASH_DRY == app_userInterface_data_get()->normalMode.common.operationSelection))
            {
                ret = APP_NORMAL_MODE_WASH;
            }
            else if (APP_NORMAL_MODE_UI_OPERATION_DRY_ONLY == app_userInterface_data_get()->normalMode.common.operationSelection)
            {
                ret = APP_NORMAL_MODE_DRY;
            }
            else
            {
                // do nothing
            }
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }

    ret = normalMode_init_pauseRequest_handle(ret);

    return ret;
}

static app_nm_state_t normalMode_init_pauseRequest_handle(app_nm_state_t state)
{
    app_nm_state_t ret = state;

    if (APP_NORMAL_MODE_UI_SETTING_OK == app_nm_userInterface_startPause_check())
    {
        if (APP_NORMAL_MODE_UI_SETTING_OK == app_nm_userInterface_setting_check())
        {
            app_nm_prevState_set(ret);
            ret = APP_NORMAL_MODE_PAUSE;
            app_buzzer_tone_set(APP_BUZZER_TONE_PAUSE);
        }
        else if (APP_NORMAL_MODE_UI_SETTING_NOK == app_nm_userInterface_setting_check())
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else
        {
            // do nothing
        }
    }

    return ret;
}

app_nm_init_state_t app_nm_init_state_get(void)
{
    return gx_init_state;
}

void app_nm_init_state_set(app_nm_init_state_t value)
{
    gx_init_state = value;
}

#ifdef LINT_FILTER_ENABLED
app_nm_init_lintFilterCheckState_t app_nm_init_lintFilterCheckState_get(void)
{
    return gx_init_lintFilterCheckState;
}

void app_nm_init_lintFilterCheckState_set(app_nm_init_lintFilterCheckState_t state)
{
    gx_init_lintFilterCheckState = state;
}
#endif

void app_nm_init_reset(void)
{
    gx_init_state = APP_NORMAL_MODE_INIT_INIT;
}

static void init_init_handle(uint32_t period)
{
    if (APP_MOTOR_RESET_OK == app_motor_resetState_get())
    {
        gx_init_state = APP_NORMAL_MODE_INIT_MOTOR_CLUTCH_RESET;
    }
}
static void init_motorClutchReset_handle(uint32_t period)
{
    if ((APP_MOTOR_HALT_OK == app_motor_haltState_get()) && (APP_CLUTCH_RESET_OK == app_motor_clutchResetState_get()))
    {
#ifdef LINT_FILTER_ENABLED
        gx_init_state = APP_NORMAL_MODE_INIT_LINT_FILTER_CHECK;
#else
        gx_init_state = APP_NORMAL_MODE_INIT_DELAY_START;
#endif
    }
}
#ifdef LINT_FILTER_ENABLED
static void init_lintFilterCheck_handle(uint32_t period)
{
#ifdef BYPASS_LINT_FILTER_CHECK
    gx_init_state = APP_NORMAL_MODE_INIT_DELAY_START;
#endif

    switch (gx_init_lintFilterCheckState)
    {
        case LINT_FILTER_CHECK_INIT:
        {
            gx_init_lintFilterCheckState = LINT_FILTER_CHECK_REMOVE;
        }
        break;

        case LINT_FILTER_CHECK_REMOVE:
        {
            if (HANDLER_LINT_FILTER_SENSOR_CLOSED != handler_lintFilterSensor_state_get())
            {
                gx_init_lintFilterCheckState = LINT_FILTER_CHECK_RETURN;
            }
        }
        break;

        case LINT_FILTER_CHECK_RETURN:
        {
            if (HANDLER_LINT_FILTER_SENSOR_CLOSED == handler_lintFilterSensor_state_get())
            {
                gx_init_lintFilterCheckState = LINT_FILTER_CHECK_FINISH;
            }
        }
        break;

        case LINT_FILTER_CHECK_FINISH:
        {
            if (HANDLER_LINT_FILTER_SENSOR_CLOSED == handler_lintFilterSensor_state_get())
            {
                gx_init_state = APP_NORMAL_MODE_INIT_DELAY_START;
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
#endif
static void init_delayStart_handle(uint32_t period)
{
    static uint32_t delayStartProcessedTimeMSec = 0;

    if (app_nm_data_get()->delayStart > 0)
    {
        delayStartProcessedTimeMSec += period;
        if (delayStartProcessedTimeMSec >= INIT_ONE_MIN_MSEC)
        {
            app_nm_data_set()->delayStart -= (delayStartProcessedTimeMSec / INIT_ONE_MIN_MSEC);
            delayStartProcessedTimeMSec = 0;
        }

        if (APP_NORMAL_MODE_UI_SETTING_OK == app_nm_userInterface_startPause_check())
        {
            app_nm_data_set()->delayStart = 0;
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
        }
    }
    else
    {
        delayStartProcessedTimeMSec = 0;
        gx_init_state = APP_NORMAL_MODE_INIT_FINISH;
    }
}
static void init_finish_handle(uint32_t period)
{
    gx_init_state = APP_NORMAL_MODE_INIT_DONE;
}
