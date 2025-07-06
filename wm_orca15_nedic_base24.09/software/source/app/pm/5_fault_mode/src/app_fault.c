#include "os.h"

#include "app_processor.h"

#include "app_fault.h"
#include "app_user_interface.h"
#include "app_nm.h"
#include "app_lid.h"
#ifdef BLOWER_ENABLED
#include "app_blower.h"
#endif
#include "app_fill.h"
#include "app_motor.h"
#include "app_drain.h"
#include "app_heater.h"
#include "app_buzzer.h"
#include "app_nm_wash.h"
#include "app_nm_log.h"

#include "handlers/input/inc/handler_off_sw.h"
#include "handlers/input/inc/handler_lid_sensor.h"
#include "handlers/output/inc/handler_lock_motor.h"
#include "handlers/input/inc/handler_keypad.h"

#define FAULT_CHILD_LOCK_ERROR_TIMEOUT (20000U)
#define FAULT_CHILD_LOCK_TIMEOUT_MSEC  (600000U)

#define MAIN_FAULT_INDICATION_INTERVAL_MS (60000U)

static void fault_buzzer_handle(uint32_t period);
static uint8_t fault_processing_userAction_monitor(void);
static void fault_childLockProcedure_check(uint32_t period);

static app_fault_childLockProcedure_t gx_fault_childLockProcedureState = CHILD_LOCK_PROCEDURE_INACTIVE;

app_processor_state_t app_fault_process(uint32_t period)
{
    handler_offSW_state_t offSW = handler_offSW_state_get();
    app_processor_state_t ret = APP_PROCESSOR_FAULT_MODE;

    fault_buzzer_handle(period);
    fault_childLockProcedure_check(period);

    switch (app_nm_log_error_get())
    {
        case APP_NORMAL_MODE_LOG_ERROR_DRAIN_FAILURE:
        case APP_NORMAL_MODE_LOG_ERROR_DRAIN_FILTER_FAILURE:

        case APP_NORMAL_MODE_WASH_LOG_ERROR_WATER_SUPPLY_FAILURE:
        case APP_NORMAL_MODE_WASH_LOG_ERROR_UNBALANCE_FAILURE:
        {
            if (0U != fault_processing_userAction_monitor())
            {
                ret = app_processor_previousState_get();
            }
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_LID_OPENING_FAILURE:
        case APP_NORMAL_MODE_LOG_ERROR_LID_LOCK_FAILURE:
        case APP_NORMAL_MODE_LOG_ERROR_OVER_LOADING_FAILURE:
        {
            if (CHILD_LOCK_PROCEDURE_ACTIVE == app_fault_childLockState_get())
            {
                // do nothing
            }
            else
            {
                if (0U != fault_processing_userAction_monitor())
                {
                    ret = app_processor_previousState_get();
                    app_lid_fault_clear();
                    app_nm_wash_setup_overloadFlag_set(0);
                }
            }
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_MEMS_SENSOR_FAILURE:
        case APP_NORMAL_MODE_LOG_ERROR_DRAIN_VALVE_MOTOR_FAILURE:
        case APP_NORMAL_MODE_LOG_ERROR_ABNORMAL_WATER_LEVEL_SENSOR_VALUE:
        case APP_NORMAL_MODE_LOG_ERROR_MOTOR_CW_ROTATION:
        case APP_NORMAL_MODE_LOG_ERROR_MOTOR_CCW_ROTATION:
        case APP_NORMAL_MODE_LOG_ERROR_MOTOR_SPIN_ROTATION:
        case APP_NORMAL_MODE_LOG_ERROR_MOTOR_CW_CCW_ROTATION:
        case APP_NORMAL_MODE_LOG_ERROR_MOTOR_FAILURE:
        case APP_NORMAL_MODE_LOG_ERROR_DRIVER_IPM_FAILRUE:
        case APP_NORMAL_MODE_LOG_ERROR_MOTOR_VOLTAGE_FAILURE:
        case APP_NORMAL_MODE_LOG_ERROR_DRIVER_NO_COMMUNICATION:
        case APP_NORMAL_MODE_LOG_ERROR_CRC_MISMATCH_FAILURE:
        case APP_NORMAL_MODE_LOG_ERROR_ABNORMAL_WATER_WHEN_DRY:
        case APP_NORMAL_MODE_LOG_ERROR_DISPLAY_DISCONNECTED_FAILURE:
        case APP_NORMAL_MODE_LOG_ERROR_LOW_POWER_FAILURE:
        case APP_NORMAL_MODE_LOG_ERROR_ABNORMAL_THERMISTOR_VALUE:
        case APP_NORMAL_MODE_LOG_ERROR_LINT_FILTER_FAILURE:

        case APP_NORMAL_MODE_WASH_LOG_ERROR_ABNORMAL_WATER_SUPPLY_OVERFLOW:
        case APP_NORMAL_MODE_WASH_LOG_ERROR_WATER_SUPPLY_CONNECTION_FAILURE:
        case APP_NORMAL_MODE_WASH_LOG_ERROR_ABNORMAL_WATER_LEAKAGE:
        case APP_NORMAL_MODE_WASH_LOG_ERROR_FILL_HIGH_TEMP_FAILURE:

        case APP_NORMAL_MODE_DRY_LOG_ERROR_BLOWER_NOT_OPERATING_FAILURE:
        case APP_NORMAL_MODE_DRY_LOG_ERROR_BLOWER_OFF_HEATER_ON_FAILURE:
        case APP_NORMAL_MODE_DRY_LOG_ERROR_HEATER_OVERCURRENT_FAILURE:
        case APP_NORMAL_MODE_DRY_LOG_ERROR_HEATER_NOT_OPERATING_FAILURE:
        case APP_NORMAL_MODE_DRY_LOG_ERROR_HEATER_TEMP_SENSOR_FAILURE:
        case APP_NORMAL_MODE_DRY_LOG_ERROR_LINT_FILTER_NOT_FOUND_FAILURE:
        default:
        {
            // do nothing
        }
        break;
    }

    if ((gx_fault_childLockProcedureState == CHILD_LOCK_PROCEDURE_DONE) || (offSW == HANDLER_OFF_SW_PRESSED))
    {
        app_processor_shutdownState_set(MEMORY_SHUTDOWN_NORMAL);
        ret = APP_PROCESSOR_FINISH_MODE;
        app_buzzer_tone_set(APP_BUZZER_TONE_SHUTDOWN);
    }
    else
    {
        // do nothing
    }

    return ret;
}

app_fault_childLockProcedure_t app_fault_childLockState_get(void)
{
    return gx_fault_childLockProcedureState;
}

static void fault_buzzer_handle(uint32_t period)
{
    static uint32_t faultBuzzerRepeatCnt = 0;

    faultBuzzerRepeatCnt += period;
    if (faultBuzzerRepeatCnt >= MAIN_FAULT_INDICATION_INTERVAL_MS)
    {
        faultBuzzerRepeatCnt = 0;
        app_buzzer_tone_set(APP_BUZZER_TONE_FAULT);
    }
}

static uint8_t fault_processing_userAction_monitor(void)
{
    uint8_t ret = 0;
    static uint8_t moveOn_flag = 0;
    handler_lidSensor_state_t lidState = handler_lidSensor_state_get();

    if (HANDLER_LID_SENSOR_OPENED == lidState)
    {
        moveOn_flag = 1;
    }

    if ((HANDLER_LID_SENSOR_CLOSED == lidState) && (0 != moveOn_flag))
    {
        moveOn_flag = 0;
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_NO_ERROR);
        ret = 1;
    }

    return ret;
}

static void fault_childLockProcedure_check(uint32_t period)
{
    static uint32_t childLockCounter = 0;
    static uint32_t childLockProcedureCounter = 0;

    if ((APP_NORMAL_MODE_LOG_ERROR_LID_OPENING_FAILURE == app_nm_log_error_get()) && (APP_NORMAL_MODE_UI_CHILD_LOCK_ON == app_userInterface_data_get()->normalMode.common.childLockSelection))
    {
        childLockCounter += period;
        if (childLockCounter >= FAULT_CHILD_LOCK_ERROR_TIMEOUT)
        {
            gx_fault_childLockProcedureState = CHILD_LOCK_PROCEDURE_ACTIVE;
        }
    }
    else
    {
        childLockCounter = 0;
        gx_fault_childLockProcedureState = CHILD_LOCK_PROCEDURE_INACTIVE;
    }

    childLockProcedureCounter += period;
    if (childLockProcedureCounter >= FAULT_CHILD_LOCK_TIMEOUT_MSEC)
    {
        childLockProcedureCounter = 0;
        gx_fault_childLockProcedureState = CHILD_LOCK_PROCEDURE_DONE;
    }
}
