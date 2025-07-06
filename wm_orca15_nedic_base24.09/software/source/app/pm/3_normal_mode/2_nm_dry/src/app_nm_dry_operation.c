#include "def.h"

#include "app_lid.h"
#include "app_drain.h"

#include "app_nm_dry.h"
#include "app_nm_dry_motor.h"
#include "app_nm_dry_heater.h"
#include "app_nm_dry_drain.h"

#include "app_nm.h"

#include "app_user_interface.h"

#include "handlers/input/inc/handler_lint_filter_sensor.h"

#define COOLING_EARLY_EXIT_TEMP (55U)

typedef enum app_dry_operation_initState
{
    APP_DRY_OPERATION_INIT_SYNC,
    APP_DRY_OPERATION_INIT_DONE
} app_dry_operation_initState_t;

typedef enum app_nm_dry_lintFilterCheckState
{
    DRY_LINT_FILTER_CHECK_INIT,
    DRY_LINT_FILTER_CHECK_REMOVE,
    DRY_LINT_FILTER_CHECK_RETURN,
    DRY_LINT_FILTER_CHECK_FINISH
} app_nm_dry_lintFilterCheckState_t;

static void init_handle(uint32_t period);
static void drain_handle(uint32_t period);
static void finalSpin_handle(uint32_t period);
static void dryingSpin_handle(uint32_t period);
static void brake_handle(uint32_t period);
static void untangle_handle(uint32_t period);
static void heating_handle(uint32_t period);
static void condense1_handle(uint32_t period);
static void condense2_handle(uint32_t period);
static void extraTime_handle(uint32_t period);
static void coolingWater_handle(uint32_t period);
static void coolingAir_handle(uint32_t period);
static void lintFilterClogged_handle(uint32_t period);
static void lintFilterCloggedReset_handle(uint32_t period);
static void finish_handle(uint32_t period);

static const app_nm_dry_data_t *data;
static app_nm_dry_operation_state_t gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_INIT;

static app_nm_dry_lintFilterCheckState_t gx_dry_lintFilterCheckState = DRY_LINT_FILTER_CHECK_INIT;

void app_nm_dry_operation_process(uint32_t period)
{
    static void (*handlers[])(uint32_t period) = {
        init_handle,
        drain_handle,
        finalSpin_handle,
        dryingSpin_handle,
        brake_handle,
        untangle_handle,
        heating_handle,
        condense1_handle,
        condense2_handle,
        extraTime_handle,
        coolingWater_handle,
        coolingAir_handle,
        lintFilterClogged_handle,
        lintFilterCloggedReset_handle,
        finish_handle,
        NULL};

    if (APP_NORMAL_MODE_DRY_OPERATION == app_nm_dry_state_get())
    {
        data = &(app_nm_data_get()->dry);

        if (handlers[gx_dry_operation_state] != NULL)
        {
            handlers[gx_dry_operation_state](period);
        }
    }
}

app_nm_dry_operation_state_t app_nm_dry_operation_state_get(void)
{
    return gx_dry_operation_state;
}

void app_nm_dry_operation_state_set(app_nm_dry_operation_state_t value)
{
    gx_dry_operation_state = value;
}

void app_nm_dry_operation_reset(void)
{
    gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_INIT;
}

static void init_handle(uint32_t period)
{
    static app_dry_operation_initState_t state = APP_DRY_OPERATION_INIT_SYNC;

    // NOTE: the aim of the state machine is to give the opportunity to other app components to run this state
    switch (state)
    {
        case APP_DRY_OPERATION_INIT_SYNC:
        {
            app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
            state = APP_DRY_OPERATION_INIT_DONE;
        }
        break;

        case APP_DRY_OPERATION_INIT_DONE:
        {
            gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_DRAIN;
            state = APP_DRY_OPERATION_INIT_SYNC;
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}

static void drain_handle(uint32_t period)
{
    if (data->operationTimeMSec[gx_dry_operation_state] == 0)
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_FINAL_SPIN;
    }
    else
    {
        if ((APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode) && (APP_DRAIN_TUB_EMPTY == app_drain_tubState_get()))
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->dry.operationTimeMSec[gx_dry_operation_state]));
        }
    }
}

static void finalSpin_handle(uint32_t period)
{
    if (data->operationTimeMSec[gx_dry_operation_state] == 0)
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_DRYING_SPIN;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->dry.operationTimeMSec[gx_dry_operation_state]));
        }
    }
}

static void dryingSpin_handle(uint32_t period)
{
    if (data->operationTimeMSec[gx_dry_operation_state] == 0U)
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_BRAKE;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->dry.operationTimeMSec[gx_dry_operation_state]));
        }
    }
}

static void brake_handle(uint32_t period)
{
    if (data->operationTimeMSec[gx_dry_operation_state] == 0U)
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_UNTANGLE;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->dry.operationTimeMSec[gx_dry_operation_state]));
        }
    }
}

static void untangle_handle(uint32_t period)
{
    if (data->operationTimeMSec[gx_dry_operation_state] == 0U)
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_HEATING;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->dry.operationTimeMSec[gx_dry_operation_state]));
        }
    }
}

static void heating_handle(uint32_t period)
{
    if (APP_NORMAL_MODE_DRY_HEATER_LINT_FILTER_CLOGGED == app_nm_dry_heater_lintFilterCloggingState_get())
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else if (data->operationTimeMSec[gx_dry_operation_state] == 0U)
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_1;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->dry.operationTimeMSec[gx_dry_operation_state]));
        }
    }
}

static void condense1_handle(uint32_t period)
{
    if (APP_NORMAL_MODE_DRY_HEATER_LINT_FILTER_CLOGGED == app_nm_dry_heater_lintFilterCloggingState_get())
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else if (((app_userInterface_data_get()->normalMode.dry.dryTimeSelection == APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO) && app_nm_dry_heater_autoTerminationState_get() == APP_NORMAL_MODE_DRY_HEATER_AUTO_TERMINATION_OK) || (data->operationTimeMSec[gx_dry_operation_state] == 0U))
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_2;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->dry.operationTimeMSec[gx_dry_operation_state]));
        }
    }
}

static void condense2_handle(uint32_t period)
{
    if (APP_NORMAL_MODE_DRY_HEATER_LINT_FILTER_CLOGGED == app_nm_dry_heater_lintFilterCloggingState_get())
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else if ((app_userInterface_data_get()->normalMode.dry.dryTimeSelection == APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO) && app_nm_dry_heater_autoTerminationState_get() == APP_NORMAL_MODE_DRY_HEATER_AUTO_TERMINATION_OK)
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else if (data->operationTimeMSec[gx_dry_operation_state] == 0U)
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_OPERATION;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->dry.operationTimeMSec[gx_dry_operation_state]));
        }
    }
}

static void extraTime_handle(uint32_t period)
{
    if (APP_NORMAL_MODE_DRY_HEATER_LINT_FILTER_CLOGGED == app_nm_dry_heater_lintFilterCloggingState_get())
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else if (data->operationTimeMSec[gx_dry_operation_state] == 0U)
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_COOLING_WATER;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->dry.operationTimeMSec[gx_dry_operation_state]));
        }
    }
}

static void coolingWater_handle(uint32_t period)
{
    if (data->operationTimeMSec[gx_dry_operation_state] == 0)
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->dry.operationTimeMSec[gx_dry_operation_state]));

            // NOTE: this state keeps running in `MD` state only
        }
    }
}

static void coolingAir_handle(uint32_t period)
{
    if (data->operationTimeMSec[gx_dry_operation_state] == 0U)
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_FINISH;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else if (COOLING_EARLY_EXIT_TEMP >= app_heater_temp_get())
    {
        app_nm_data_set()->dry.operationTimeMSec[gx_dry_operation_state] = 0;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->dry.operationTimeMSec[gx_dry_operation_state]));
        }
    }
}

static void lintFilterClogged_handle(uint32_t period)
{
    if (COOLING_EARLY_EXIT_TEMP >= app_heater_temp_get())
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED_RESET;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_dry_lintFilterCheckState = DRY_LINT_FILTER_CHECK_INIT;
    }
}

static void lintFilterCloggedReset_handle(uint32_t period)
{
    switch (gx_dry_lintFilterCheckState)
    {
        case DRY_LINT_FILTER_CHECK_INIT:
        {
            gx_dry_lintFilterCheckState = DRY_LINT_FILTER_CHECK_REMOVE;
        }
        break;

        case DRY_LINT_FILTER_CHECK_REMOVE:
        {
            if (HANDLER_LINT_FILTER_SENSOR_CLOSED != handler_lintFilterSensor_state_get())
            {
                gx_dry_lintFilterCheckState = DRY_LINT_FILTER_CHECK_RETURN;
            }
        }
        break;

        case DRY_LINT_FILTER_CHECK_RETURN:
        {
            if (HANDLER_LINT_FILTER_SENSOR_CLOSED == handler_lintFilterSensor_state_get())
            {
                gx_dry_lintFilterCheckState = DRY_LINT_FILTER_CHECK_FINISH;
            }
        }
        break;

        case DRY_LINT_FILTER_CHECK_FINISH:
        {
            if (HANDLER_LINT_FILTER_SENSOR_CLOSED == handler_lintFilterSensor_state_get())
            {
                app_nm_dry_heater_lintFilterCloggingState_set(APP_NORMAL_MODE_DRY_HEATER_LINT_FILTER_NOT_CLOGGED);
                gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_INIT;
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
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

static void finish_handle(uint32_t period)
{
    if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
    {
        gx_dry_operation_state = APP_NORMAL_MODE_DRY_OPERATION_DONE;
    }
}
