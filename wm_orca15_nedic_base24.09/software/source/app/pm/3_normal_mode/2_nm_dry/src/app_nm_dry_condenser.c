#include "def.h"
#include "app_nm_dry_condenser.h"
#include "app_nm_dry.h"
#include "app_nm.h"

#include "app_condenser.h"

static void condenser_operation_handle(uint32_t period);

void app_nm_dry_condenser_process(uint32_t period)
{
    if (APP_NORMAL_MODE_DRY == app_nm_state_get())
    {
        switch (app_nm_dry_state_get())
        {
            case APP_NORMAL_MODE_DRY_INIT:
            case APP_NORMAL_MODE_DRY_PRE_SETUP:
            case APP_NORMAL_MODE_DRY_SETUP:
            case APP_NORMAL_MODE_DRY_FINISH:
            {
                app_condenser_state_set(APP_CONDENSER_OFF);
            }
            break;

            case APP_NORMAL_MODE_DRY_OPERATION:
            {
                condenser_operation_handle(period);
            }
            break;

            case APP_NORMAL_MODE_DRY_DONE:
            default:
            {
                // do nothing
            }
            break;
        }
    }
}

static void condenser_operation_handle(uint32_t period)
{
    switch (app_nm_dry_operation_state_get())
    {
        case APP_NORMAL_MODE_DRY_OPERATION_INIT:
        case APP_NORMAL_MODE_DRY_OPERATION_DRAIN:
        case APP_NORMAL_MODE_DRY_OPERATION_FINAL_SPIN:
        case APP_NORMAL_MODE_DRY_OPERATION_DRYING_SPIN:
        case APP_NORMAL_MODE_DRY_OPERATION_BRAKE:
        case APP_NORMAL_MODE_DRY_OPERATION_UNTANGLE:
        case APP_NORMAL_MODE_DRY_OPERATION_HEATING:
        case APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR:
        case APP_NORMAL_MODE_DRY_OPERATION_FINISH:
        {
            app_condenser_state_set(APP_CONDENSER_OFF);
        }
        break;

        case APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_1:
        case APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_2:
        case APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME:
        case APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED:
        case APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED_RESET:
        case APP_NORMAL_MODE_DRY_OPERATION_COOLING_WATER:
        {
            app_condenser_state_set(APP_CONDENSER_ON);
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
