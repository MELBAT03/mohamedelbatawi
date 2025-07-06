#include "def.h"
#include "app_nm_wash.h"
#include "app_nm_wash_motor.h"
#include "app_nm_wash_drain.h"
#include "app_nm.h"

#include "app_drain.h"

#include "app_user_interface.h"

static void init_handle(uint32_t period);
static void drain_handle(uint32_t period);
static void spin_handle(uint32_t period);
static void steam_handle(uint32_t period);
static void finish_handle(uint32_t period);
static void rinseTransition_handle(uint32_t period);

static app_nm_wash_steam_state_t gx_wash_steam_state = APP_NORMAL_MODE_WASH_STEAM_INIT;

static const app_nm_wash_data_t *data;

void app_nm_wash_steam_process(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        init_handle,
        drain_handle,
        spin_handle,
        steam_handle,
        finish_handle,
        rinseTransition_handle,
        NULL};

    if (APP_NORMAL_MODE_WASH_STEAM == app_nm_wash_state_get())
    {
        data = &(app_nm_data_get()->wash);

        if (handlers[gx_wash_steam_state] != NULL)
        {
            handlers[gx_wash_steam_state](period);
        }
    }
}

app_nm_wash_steam_state_t app_nm_wash_steam_state_get(void)
{
    return gx_wash_steam_state;
}

void app_nm_wash_steam_state_set(app_nm_wash_steam_state_t value)
{
    gx_wash_steam_state = value;
}

void app_nm_wash_steam_reset(void)
{
    gx_wash_steam_state = APP_NORMAL_MODE_WASH_STEAM_INIT;
}

static void init_handle(uint32_t period)
{
    app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    gx_wash_steam_state = APP_NORMAL_MODE_WASH_STEAM_DRAIN;
}
static void drain_handle(uint32_t period)
{
    if (data->steamStateTimeMSec[gx_wash_steam_state] == 0U)
    {
        gx_wash_steam_state = APP_NORMAL_MODE_WASH_STEAM_SPIN;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            if (APP_DRAIN_TUB_EMPTY == app_drain_tubState_get())
            {
                app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.steamStateTimeMSec[gx_wash_steam_state]));
            }
        }
    }
}
static void spin_handle(uint32_t period)
{
    if (data->steamStateTimeMSec[gx_wash_steam_state] == 0U)
    {
        gx_wash_steam_state = APP_NORMAL_MODE_WASH_STEAM_STEAM;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.steamStateTimeMSec[gx_wash_steam_state]));
        }
    }
}
static void steam_handle(uint32_t period)
{
    if (data->steamStateTimeMSec[gx_wash_steam_state] == 0U)
    {
        gx_wash_steam_state = APP_NORMAL_MODE_WASH_STEAM_FINISH;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.steamStateTimeMSec[gx_wash_steam_state]));
        }
    }
}
static void finish_handle(uint32_t period)
{
    gx_wash_steam_state = APP_NORMAL_MODE_WASH_STEAM_RINSE_TRANSITION;
    app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
}
static void rinseTransition_handle(uint32_t period)
{
    gx_wash_steam_state = APP_NORMAL_MODE_WASH_STEAM_DONE;
    app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
}
