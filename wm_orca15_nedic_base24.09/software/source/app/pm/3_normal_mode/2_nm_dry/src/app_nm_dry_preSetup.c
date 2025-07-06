#include "def.h"
#include "app_nm_dry.h"
#include "app_nm_dry_motor.h"
#include "app_nm_dry_drain.h"
#include "app_nm.h"

#include "app_drain.h"

#include "app_user_interface.h"

typedef enum app_dry_preSetup_initState
{
    APP_DRY_PRESETUP_INIT_SYNC,
    APP_DRY_PRESETUP_INIT_DONE
} app_dry_preSetup_initState_t;

static void init_handle(uint32_t period);
static void drain_handle(uint32_t period);
static void spin_handle(uint32_t period);
static void untangle_handle(uint32_t period);
static void finish_handle(uint32_t period);

static const app_nm_dry_data_t *data;
static app_nm_dry_preSetup_state_t gx_dry_preSetup_state = APP_NORMAL_MODE_DRY_PRE_SETUP_INIT;
static uint32_t gu32_app_dry_preSetup_remaningTimeMSec = 0;

void app_nm_dry_preSetup_process(uint32_t period)
{
    static void (*handlers[])(uint32_t period) = {
        init_handle,
        drain_handle,
        spin_handle,
        untangle_handle,
        finish_handle,
        NULL};

    if (APP_NORMAL_MODE_DRY_PRE_SETUP == app_nm_dry_state_get())
    {
        data = &(app_nm_data_get()->dry);

        if (handlers[gx_dry_preSetup_state] != NULL)
        {
            handlers[gx_dry_preSetup_state](period);
        }
    }
}

app_nm_dry_preSetup_state_t app_nm_dry_preSetup_state_get(void)
{
    return gx_dry_preSetup_state;
}

void app_nm_dry_preSetup_state_set(app_nm_dry_preSetup_state_t value)
{
    gx_dry_preSetup_state = value;
}

void app_nm_dry_preSetup_reset(void)
{
    gx_dry_preSetup_state = APP_NORMAL_MODE_DRY_PRE_SETUP_INIT;
}

static void init_handle(uint32_t period)
{
    static app_dry_preSetup_initState_t state = APP_DRY_PRESETUP_INIT_SYNC;

    switch (state)
    {
        case APP_DRY_PRESETUP_INIT_SYNC:
        {
            gu32_app_dry_preSetup_remaningTimeMSec = 0;
            app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
            state = APP_DRY_PRESETUP_INIT_DONE;
        }
        break;

        case APP_DRY_PRESETUP_INIT_DONE:
        {
            gx_dry_preSetup_state = APP_NORMAL_MODE_DRY_PRE_SETUP_DRAIN;
            state = APP_DRY_PRESETUP_INIT_SYNC;
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
    if (data->preSetupTimeMSec[gx_dry_preSetup_state] == 0)
    {
        gx_dry_preSetup_state = APP_NORMAL_MODE_DRY_PRE_SETUP_SPIN;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            if (APP_DRAIN_TUB_EMPTY == app_drain_tubState_get())
            {
                app_nm_remainingTime_process(period, &(app_nm_data_set()->dry.preSetupTimeMSec[gx_dry_preSetup_state]));
            }
        }
    }
}
static void spin_handle(uint32_t period)
{
    if (data->preSetupTimeMSec[gx_dry_preSetup_state] == 0)
    {
        gx_dry_preSetup_state = APP_NORMAL_MODE_DRY_PRE_SETUP_UNTANGLE;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->dry.preSetupTimeMSec[gx_dry_preSetup_state]));
        }
    }
}
static void untangle_handle(uint32_t period)
{
    if (data->preSetupTimeMSec[gx_dry_preSetup_state] == 0)
    {
        gx_dry_preSetup_state = APP_NORMAL_MODE_DRY_PRE_SETUP_FINISH;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->dry.preSetupTimeMSec[gx_dry_preSetup_state]));
        }
    }
}
static void finish_handle(uint32_t period)
{
    gx_dry_preSetup_state = APP_NORMAL_MODE_DRY_PRE_SETUP_DONE;
    app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
}
