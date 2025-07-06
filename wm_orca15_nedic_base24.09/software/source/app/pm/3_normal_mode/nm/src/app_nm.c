#include "def.h"
#include "os.h"

#include "app_processor.h"

#include "app_nm.h"
#ifdef BLOWER_ENABLED
#include "app_blower.h"
#include "app_nm_init_blower.h"
#include "app_nm_dry_blower.h"
#endif

#ifdef CONDENSER_ENABLED
#include "app_condenser.h"
#include "app_nm_init_condenser.h"
#include "app_nm_dry_condenser.h"
#endif

#include "app_fill.h"
#include "app_motor.h"
#include "app_drain.h"
#include "app_heater.h"
#include "app_buzzer.h"

#include "app_nm_init.h"
#include "app_nm_init_motor.h"

#include "app_nm_init_drain.h"

#ifdef DRYER_ENABLED
#include "app_nm_dry.h"
#include "app_nm_dry_motor.h"
#include "app_nm_dry_drain.h"
#endif

#include "app_nm_wash.h"

#include "app_nm_tubclean.h"

#include "app_nm_pause.h"
#include "app_nm_pause_motor.h"

#include "app_nm_finish.h"

#include "app_user_interface.h"
#include "app_fault.h"

#include "app_memory.h"
#include "app_nm_log.h"
#include "app_lid.h"
#include "app_buzzer.h"

#include "handlers/input/inc/handler_off_sw.h"
#include "handlers/input/inc/handler_power_detection.h"
#include "handlers/input/inc/handler_lid_sensor.h"
#include "handlers/output/inc/handler_lock_motor.h"
#include "handlers/input/inc/handler_water_level.h"
#include "handlers/output/inc/handler_drain_valve.h"
#include "handlers/output/inc/handler_buzzer.h"
#include "handlers/input/inc/handler_temp_sensor.h"
#include "handlers/input/inc/handler_acin.h"
#include "handlers/input/inc/handler_mems.h"
#ifdef LINT_FILTER_ENABLED
#include "handlers/input/inc/handler_lint_filter_sensor.h"
#endif
#include "handlers/output/inc/handler_display.h"

#define ONE_SECOND                          (1000U)
#define NM_LINT_FILTER_FAILURE_TIMEOUT_MSEC (10000U)

static app_processor_state_t nm_processor_transitions_handle(void);

static void nm_fault_check(uint32_t period);

static void nm_data_update(void);

static app_nm_state_t gx_normalMode_state = APP_NORMAL_MODE_INIT;
static app_nm_state_t gx_normalMode_prevState = APP_NORMAL_MODE_INIT;

static app_nm_data_t gx_nm_data = {0U};
static app_nm_userInterface_data_t gx_nm_savedUIData = {0};

static uint32_t gu32_nm_savedRemainingTimeMin;

app_processor_state_t app_nm_process(uint32_t period)
{
    app_processor_state_t ret = APP_PROCESSOR_NORMAL_MODE;

    static app_nm_state_t (*const handlers[])(uint32_t period) = {
        app_nm_init_process,
        app_nm_wash_process,
#ifdef DRYER_ENABLED
        app_nm_dry_process,
#else
        NULL,
#endif
        app_nm_tubclean_process,
        app_nm_pause_process,
        app_nm_finish_process,
        NULL};

    nm_data_update();

    if (handlers[gx_normalMode_state] != NULL)
    {
        gx_normalMode_state = handlers[gx_normalMode_state](period);
    }

    nm_fault_check(period);

    ret = nm_processor_transitions_handle();

    return ret;
}

app_nm_state_t app_nm_state_get(void)
{
    return gx_normalMode_state;
}

void app_nm_state_set(app_nm_state_t value)
{
    gx_normalMode_state = value;
}

app_nm_state_t app_nm_prevState_get(void)
{
    return gx_normalMode_prevState;
}

void app_nm_prevState_set(app_nm_state_t value)
{
    gx_normalMode_prevState = value;
}

const app_nm_data_t *app_nm_data_get(void)
{
    return &gx_nm_data;
}

app_nm_data_t *app_nm_data_set(void)
{
    return &gx_nm_data;
}

app_nm_userInterface_data_t *app_nm_savedUIData_get(void)
{
    return &gx_nm_savedUIData;
}

void app_nm_remainingTime_process(uint32_t period, uint32_t *current)
{
    static uint32_t elapsedTimeCounter = 0U;

    if (current != NULL)
    {
        elapsedTimeCounter += period;
        if (elapsedTimeCounter >= ONE_SECOND)
        {
            if ((*current > 0) && (*current >= elapsedTimeCounter))
            {
                (*current) -= elapsedTimeCounter;
            }
            else
            {
                *current = 0;
            }
            elapsedTimeCounter = 0;
        }

        gu32_nm_savedRemainingTimeMin = *current / 60000U;
    }
}

uint32_t app_nm_remainingTimeMin_get(void)
{
    return gu32_nm_savedRemainingTimeMin;
}

void app_nm_reset(void)
{
    app_nm_init_reset();
    app_nm_wash_reset();
#ifdef DRYER_ENABLED
    app_nm_dry_reset();
#endif
}

static app_processor_state_t nm_processor_transitions_handle(void)
{
    app_processor_state_t ret = APP_PROCESSOR_NORMAL_MODE;
    handler_offSW_state_t offSW = handler_offSW_state_get();

    if (handler_powerDetection_state_get() == HANDLER_POWER_DETECTION_NOK)
    {
        app_processor_shutdownState_set(MEMORY_SHUTDOWN_ABNORMAL);
        ret = APP_PROCESSOR_FINISH_MODE;
    }
    else if (offSW == HANDLER_OFF_SW_PRESSED)
    {
        app_processor_shutdownState_set(MEMORY_SHUTDOWN_NORMAL);
        ret = APP_PROCESSOR_FINISH_MODE;
        app_buzzer_tone_set(APP_BUZZER_TONE_SHUTDOWN);
    }
    else if (APP_NORMAL_MODE_LOG_ERROR_NO_ERROR != app_nm_log_error_get())
    {
        ret = APP_PROCESSOR_FAULT_MODE;
        app_buzzer_tone_set(APP_BUZZER_TONE_FAULT);
    }
    else if (APP_NORMAL_MODE_DONE == gx_normalMode_state)
    {
        app_processor_shutdownState_set(MEMORY_SHUTDOWN_NORMAL);
        ret = APP_PROCESSOR_FINISH_MODE;
    }
    else
    {
        // do nothing
    }

    return ret;
}

static void nm_fault_check(uint32_t period)
{
#ifdef LINT_FILTER_ENABLED
    static uint32_t lintFilterErrorTimeoutMSec = 0U;
#endif

    if (APP_LID_FAILURE == app_lid_faultState_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_LID_OPENING_FAILURE);
    }
    else if (APP_NORMAL_MODE_WASH_WASH_LEAKAGE == app_nm_wash_wash_leakageState_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_WASH_LOG_ERROR_ABNORMAL_WATER_LEAKAGE);
    }
    else if (0U != app_nm_wash_setup_overloadFlag_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_OVER_LOADING_FAILURE);
    }
    else if (0U != handler_lockMotor_error_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_LID_LOCK_FAILURE);
    }
    else if (0U != handler_display_error_check())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_DISPLAY_DISCONNECTED_FAILURE);
    }
    else if (HANDLER_DANGEROUS_UNDER_VOLTAGE == handler_acin_state_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_LOW_POWER_FAILURE);
    }
    else if (APP_MOTOR_RESET_FAILURE == app_motor_resetState_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_MOTOR_FAILURE);
    }
    else if (OVERFLOW_DANGEROUS == app_fill_overFlowState_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_WASH_LOG_ERROR_ABNORMAL_WATER_SUPPLY_OVERFLOW);
    }
    else if (0U != handler_drainValve_error_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_DRAIN_VALVE_MOTOR_FAILURE);
    }
    else if (0U != handler_waterLevel_error_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_ABNORMAL_WATER_LEVEL_SENSOR_VALUE);
    }
    else if ((0U != handler_ambientTempSensor_error_get()) || (0U != handler_waterTempSensor_error_get()) || (0U != handler_airATempSensor_error_get()) || (0U != handler_airBTempSensor_error_get()))
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_ABNORMAL_THERMISTOR_VALUE);
    }
    else if (WATER_VALVE_WRONG_CONNECTION == app_fill_waterTempFault_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_WASH_LOG_ERROR_WATER_SUPPLY_CONNECTION_FAILURE);
    }
    else if (WATER_VALVE_TEMP_FAILURE == app_fill_waterTempFault_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_WASH_LOG_ERROR_FILL_HIGH_TEMP_FAILURE);
    }
#ifdef BLOWER_ENABLED
    else if (0U != app_blower_error_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_DRY_LOG_ERROR_BLOWER_NOT_OPERATING_FAILURE);
    }
#endif
    else if (0U != app_fill_error_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_WASH_LOG_ERROR_WATER_SUPPLY_FAILURE);
    }
    else if (APP_DRAIN_FAILURE_BLOCKED == app_drain_failureState_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_DRAIN_FAILURE);
    }
    else if (APP_DRAIN_FAILURE_LOW_RATE == app_drain_failureState_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_DRAIN_FILTER_FAILURE);
    }
    else if (APP_DRAIN_FAILURE_WATER_DURING_SPIN_DANGEROUS == app_drain_failureState_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_ABNORMAL_WATER_WHEN_DRY);
    }
    else if (0U != handler_mems_error_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_MEMS_SENSOR_FAILURE);
    }
    else if (HANDLER_MOTOR_ERROR_NO_COM == handler_motor_error_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_DRIVER_NO_COMMUNICATION);
    }
    else if (HANDLER_MOTOR_NO_ERROR != handler_motor_error_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_MOTOR_FAILURE);
    }
#ifdef BLOWER_ENABLED
    else if (0U != app_heater_heaterOnWhileBlowerOffFault_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_DRY_LOG_ERROR_BLOWER_OFF_HEATER_ON_FAILURE);
    }
#endif
#ifdef DRYER_ENABLED
    else if (0U != app_heater_overCurrentFault_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_DRY_LOG_ERROR_HEATER_OVERCURRENT_FAILURE);
    }
    else if (0U != app_heater_notOperatingFault_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_DRY_LOG_ERROR_HEATER_NOT_OPERATING_FAILURE);
    }
    else if (0U != app_heater_shouldntOperatingFault_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_DRY_LOG_ERROR_HEATER_SHOULDNOT_OPERATING_FAILURE);
    }
    else if (0U != app_heater_tempSensorFault_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_DRY_LOG_ERROR_HEATER_TEMP_SENSOR_FAILURE);
    }
#ifdef LINT_FILTER_ENABLED
    else if ((HANDLER_LINT_FILTER_SENSOR_OPENED == handler_lintFilterSensor_state_get()) && (APP_NORMAL_MODE_DRY == app_nm_state_get()))
    {
        app_nm_log_error_set(APP_NORMAL_MODE_DRY_LOG_ERROR_LINT_FILTER_NOT_FOUND_FAILURE);
    }
#endif
#endif
    else if (APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_ERROR == app_nm_wash_spin_unbalanceState_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_WASH_LOG_ERROR_UNBALANCE_FAILURE);
    }
#ifdef LINT_FILTER_ENABLED
    else if (HANDLER_LINT_FILTER_SENSOR_CLOSED != handler_lintFilterSensor_state_get())
    {
        lintFilterErrorTimeoutMSec += period;
        if (lintFilterErrorTimeoutMSec >= NM_LINT_FILTER_FAILURE_TIMEOUT_MSEC)
        {
            lintFilterErrorTimeoutMSec = 0;
            app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_LINT_FILTER_FAILURE);
        }
        else
        {
            lintFilterErrorTimeoutMSec = 0;
        }
    }
#endif
    else
    {
        // do nothing
    }
}

static void nm_data_update(void)
{
    app_nm_wash_data_update();
#ifdef DRYER_ENABLED
    app_nm_dry_data_update();
#endif

#ifdef DRYER_ENABLED
    gx_nm_data.totalTimeMSec = gx_nm_data.wash.washTotalTimeMSec + gx_nm_data.dry.dryTotalTimeMSec;
#else
    gx_nm_data.totalTimeMSec = gx_nm_data.wash.washTotalTimeMSec;
#endif
}
