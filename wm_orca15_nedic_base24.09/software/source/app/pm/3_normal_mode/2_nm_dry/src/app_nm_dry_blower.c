#include "def.h"
#include "app_nm_dry.h"
#include "app_nm.h"
#include "app_nm_dry_blower.h"
#include "app_nm_log.h"

#include "app_blower.h"

static void blower_operation_handle(uint32_t period);

static uint32_t gu32_blower_speed;

void app_nm_dry_blower_process(uint32_t period)
{
    if (APP_NORMAL_MODE_DRY == app_nm_state_get())
    {
        gu32_blower_speed = app_blower_speedFeedback_get();

        switch (app_nm_dry_state_get())
        {
            case APP_NORMAL_MODE_DRY_INIT:
            case APP_NORMAL_MODE_DRY_PRE_SETUP:
            case APP_NORMAL_MODE_DRY_SETUP:
            case APP_NORMAL_MODE_DRY_FINISH:
            {
                app_blower_speedControl_set(APP_BLOWER_OFF);
            }
            break;

            case APP_NORMAL_MODE_DRY_OPERATION:
            {
                blower_operation_handle(period);
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

static void blower_operation_handle(uint32_t period)
{
    switch (app_nm_dry_operation_state_get())
    {
        case APP_NORMAL_MODE_DRY_OPERATION_INIT:
        case APP_NORMAL_MODE_DRY_OPERATION_DRAIN:
        case APP_NORMAL_MODE_DRY_OPERATION_FINAL_SPIN:
        case APP_NORMAL_MODE_DRY_OPERATION_FINISH:
        {
            app_blower_speedControl_set(APP_BLOWER_OFF);
        }
        break;

        case APP_NORMAL_MODE_DRY_OPERATION_DRYING_SPIN:
        case APP_NORMAL_MODE_DRY_OPERATION_BRAKE:
        case APP_NORMAL_MODE_DRY_OPERATION_UNTANGLE:
        case APP_NORMAL_MODE_DRY_OPERATION_HEATING:
        case APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_1:
        case APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_2:
        case APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME:
        case APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED:
        case APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED_RESET:
        case APP_NORMAL_MODE_DRY_OPERATION_COOLING_WATER:
        case APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR:
        {
            app_blower_speedControl_set(BLOWER_TARGET_POWER);
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
