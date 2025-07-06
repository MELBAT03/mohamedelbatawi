#include "def.h"
#include "app_nm_dry_drain.h"
#include "app_nm_dry.h"
#include "app_nm.h"
#include "app_drain.h"

static void drain_operation_handle(uint32_t period);

void app_nm_dry_drain_process(uint32_t period)
{
    if (APP_NORMAL_MODE_DRY == app_nm_state_get())
    {
        switch (app_nm_dry_state_get())
        {
            case APP_NORMAL_MODE_DRY_INIT:
            case APP_NORMAL_MODE_DRY_SETUP:
            case APP_NORMAL_MODE_DRY_FINISH:
            {
                app_drain_switch(APP_DRAIN_CLOSE);
            }
            break;

            case APP_NORMAL_MODE_DRY_PRE_SETUP:
            {
                app_drain_switch(APP_DRAIN_OPEN);

                if (APP_NORMAL_MODE_DRY_PRE_SETUP_SPIN == app_nm_dry_preSetup_state_get())
                {
                    app_drain_spinError_check(period);
                }
                else if (APP_NORMAL_MODE_DRY_PRE_SETUP_DRAIN == app_nm_dry_preSetup_state_get())
                {
                    app_drain_error_check(period);
                }
                else
                {
                    // do nothing
                }
            }
            break;

            case APP_NORMAL_MODE_DRY_OPERATION:
            {
                drain_operation_handle(period);
            }

            case APP_NORMAL_MODE_DRY_DONE:
            default:
            {
                // do nothing
            }
            break;
        }
    }
}

static void drain_operation_handle(uint32_t period)
{
    switch (app_nm_dry_operation_state_get())
    {
        case APP_NORMAL_MODE_DRY_OPERATION_INIT:
        case APP_NORMAL_MODE_DRY_OPERATION_HEATING:
        case APP_NORMAL_MODE_DRY_OPERATION_FINISH:
        {
            app_drain_switch(APP_DRAIN_CLOSE);
        }
        break;

        case APP_NORMAL_MODE_DRY_OPERATION_DRAIN:
        case APP_NORMAL_MODE_DRY_OPERATION_FINAL_SPIN:
        case APP_NORMAL_MODE_DRY_OPERATION_DRYING_SPIN:
        case APP_NORMAL_MODE_DRY_OPERATION_BRAKE:
        case APP_NORMAL_MODE_DRY_OPERATION_UNTANGLE:
        case APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_1:
        case APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_2:
        case APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME:
        case APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED:
        case APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED_RESET:
        case APP_NORMAL_MODE_DRY_OPERATION_COOLING_WATER:
        case APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR:
        {
            app_drain_switch(APP_DRAIN_OPEN);
        }
        break;

        case APP_NORMAL_MODE_DRY_OPERATION_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}
