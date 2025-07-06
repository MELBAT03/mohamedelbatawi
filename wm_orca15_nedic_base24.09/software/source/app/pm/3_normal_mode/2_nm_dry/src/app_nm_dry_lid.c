#include "def.h"

#include "app_nm_dry_lid.h"

#include "app_nm_dry.h"
#include "app_nm.h"
#include "app_lid.h"

void app_nm_dry_lid_process(uint32_t period)
{
    if (APP_NORMAL_MODE_DRY == app_nm_state_get())
    {
        switch (app_nm_dry_state_get())
        {
            case APP_NORMAL_MODE_DRY_INIT:
            case APP_NORMAL_MODE_DRY_PRE_SETUP:
            case APP_NORMAL_MODE_DRY_SETUP:
            {
                app_lidLock_state_set(APP_LID_LOCK_ON);
            }
            break;

            case APP_NORMAL_MODE_DRY_OPERATION:
            {
                if (app_nm_dry_operation_state_get() != APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED_RESET)
                {
                    app_lidLock_state_set(APP_LID_LOCK_ON);
                }
                else
                {
                    app_lidLock_state_set(APP_LID_LOCK_OFF);
                }
            }
            break;

            case APP_NORMAL_MODE_DRY_FINISH:
            {
                app_lidLock_state_set(APP_LID_LOCK_OFF);
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
