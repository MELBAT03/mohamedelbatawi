#include "def.h"
#include "app_nm_wash_drain.h"
#include "app_nm.h"
#include "app_drain.h"

#include "app_nm_wash.h"

static void drain_wash_steam_process(uint32_t period);

static void drain_wash_rinse_process(uint32_t period);
static void drain_wash_rinse_unbalance_handle(uint32_t period);

static void drain_wash_spin_process(uint32_t period);
static void drain_wash_spin_unbalance_handle(uint32_t period);

void app_nm_wash_drain_process(uint32_t period)
{
    switch (app_nm_wash_state_get())
    {
        case APP_NORMAL_MODE_WASH_INIT:
        case APP_NORMAL_MODE_WASH_SETUP:
        case APP_NORMAL_MODE_WASH_SOAK:
        case APP_NORMAL_MODE_WASH_WASH:
        case APP_NORMAL_MODE_WASH_FINISH:
        {
            app_drain_switch(APP_DRAIN_CLOSE);
        }
        break;

        case APP_NORMAL_MODE_WASH_STEAM:
        {
            drain_wash_steam_process(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE:
        {
            drain_wash_rinse_process(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN:
        {
            drain_wash_spin_process(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

static void drain_wash_steam_process(uint32_t period)
{
    switch (app_nm_wash_steam_state_get())
    {
        case APP_NORMAL_MODE_WASH_STEAM_INIT:
        case APP_NORMAL_MODE_WASH_STEAM_STEAM:
        case APP_NORMAL_MODE_WASH_STEAM_FINISH:
        case APP_NORMAL_MODE_WASH_STEAM_RINSE_TRANSITION:
        {
            app_drain_switch(APP_DRAIN_CLOSE);
        }
        break;

        case APP_NORMAL_MODE_WASH_STEAM_DRAIN:
        case APP_NORMAL_MODE_WASH_STEAM_SPIN:
        {
            app_drain_switch(APP_DRAIN_OPEN);
            app_drain_error_check(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_STEAM_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

static void drain_wash_rinse_process(uint32_t period)
{
    switch (app_nm_wash_rinse_state_get())
    {
        case APP_NORMAL_MODE_WASH_RINSE_INIT:
        case APP_NORMAL_MODE_WASH_RINSE_FILL:
        case APP_NORMAL_MODE_WASH_RINSE_STIR:
        case APP_NORMAL_MODE_WASH_RINSE_UNTANGLE:
        case APP_NORMAL_MODE_WASH_RINSE_FINISH:
        {
            app_drain_switch(APP_DRAIN_CLOSE);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_DRAIN:
        {
            app_drain_switch(APP_DRAIN_OPEN);
            app_drain_error_check(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_SPIN:
        case APP_NORMAL_MODE_WASH_RINSE_SPIN_BRAKE:
        {
            app_drain_switch(APP_DRAIN_OPEN);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE:
        {
            drain_wash_rinse_unbalance_handle(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_REGROUP:
        case APP_NORMAL_MODE_WASH_RINSE_SPIN_TRANSITION:
        case APP_NORMAL_MODE_WASH_RINSE_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

static void drain_wash_rinse_unbalance_handle(uint32_t period)
{
    switch (app_nm_wash_rinse_unbalanceState_get())
    {
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_SETUP:
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_FILL:
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_STIR:
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_FINISH:
        {
            app_drain_switch(APP_DRAIN_CLOSE);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_DRAIN:
        {
            app_drain_switch(APP_DRAIN_OPEN);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_INIT:
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

static void drain_wash_spin_process(uint32_t period)
{
    switch (app_nm_wash_spin_state_get())
    {
        case APP_NORMAL_MODE_WASH_SPIN_INIT:
        case APP_NORMAL_MODE_WASH_SPIN_ANTI_WRINKLE:
        case APP_NORMAL_MODE_WASH_SPIN_FINISH:
        {
            app_drain_switch(APP_DRAIN_CLOSE);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_DRAIN:
        {
            app_drain_switch(APP_DRAIN_OPEN);
            app_drain_error_check(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_SPIN:
        case APP_NORMAL_MODE_WASH_SPIN_SUPER_SPIN:
        case APP_NORMAL_MODE_WASH_SPIN_BRAKE:
        {
            app_drain_switch(APP_DRAIN_OPEN);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE:
        {
            drain_wash_spin_unbalance_handle(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_END_TRANSITION:
        case APP_NORMAL_MODE_WASH_SPIN_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

static void drain_wash_spin_unbalance_handle(uint32_t period)
{
    switch (app_nm_wash_spin_unbalanceState_get())
    {
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_SETUP:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_FILL:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_STIR:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_SUPER_SPIN_STIR:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_FINISH:
        {
            app_drain_switch(APP_DRAIN_CLOSE);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_DRAIN:
        {
            app_drain_switch(APP_DRAIN_OPEN);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_INIT:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_DONE:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_ERROR:
        default:
        {
            // do nothing
        }
        break;
    }
}
