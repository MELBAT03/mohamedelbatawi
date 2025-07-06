#include "def.h"
#include "app_nm_wash_lid.h"
#include "app_nm.h"
#include "app_lid.h"

#include "app_motor.h"

#include "app_user_interface.h"

#include "app_nm_wash.h"

void app_nm_wash_lid_process(uint32_t period)
{
    switch (app_nm_wash_state_get())
    {
        case APP_NORMAL_MODE_WASH_INIT:
        case APP_NORMAL_MODE_WASH_SETUP:
        {
            app_lidLock_state_set(APP_LID_LOCK_OFF);
        }
        break;

        case APP_NORMAL_MODE_WASH_SOAK:
        case APP_NORMAL_MODE_WASH_WASH:
        case APP_NORMAL_MODE_WASH_STEAM:
        case APP_NORMAL_MODE_WASH_RINSE:
        case APP_NORMAL_MODE_WASH_SPIN:
        case APP_NORMAL_MODE_WASH_FINISH:
        case APP_NORMAL_MODE_WASH_DONE:
        {
            app_lidLock_state_set(APP_LID_LOCK_ON);
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}
