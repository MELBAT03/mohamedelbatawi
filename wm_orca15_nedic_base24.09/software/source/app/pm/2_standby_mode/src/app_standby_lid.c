#include "def.h"

#include "app_lid.h"
#include "app_user_interface.h"

#include "app_standby_lid.h"

void app_standby_lid_process(uint32_t period)
{
    if (app_userInterface_data_get()->normalMode.common.childLockSelection == APP_NORMAL_MODE_UI_CHILD_LOCK_ON)
    {
        app_lidLock_state_set(APP_LID_LOCK_ON);
    }
    else
    {
        app_lidLock_state_set(APP_LID_LOCK_OFF);
    }
}
