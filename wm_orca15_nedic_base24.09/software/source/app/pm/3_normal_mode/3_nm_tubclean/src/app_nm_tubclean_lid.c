#include "def.h"
#include "app_nm_tubclean_lid.h"
#include "app_nm_tubclean.h"
#include "app_lid.h"

void app_nm_tubclean_lid_process(uint32_t period)
{
    switch (app_nm_tubclean_state_get())
    {
        case APP_NORMAL_MODE_TUBCLEAN_INIT:
        case APP_NORMAL_MODE_TUBCLEAN_FILL:
        case APP_NORMAL_MODE_TUBCLEAN_FINISH:
        {
            app_lidLock_state_set(APP_LID_LOCK_OFF);
        }
        break;

        case APP_NORMAL_MODE_TUBCLEAN_MH:
        case APP_NORMAL_MODE_TUBCLEAN_MD:
        case APP_NORMAL_MODE_TUBCLEAN_FILL_STIR:
        case APP_NORMAL_MODE_TUBCLEAN_SOAK:
        case APP_NORMAL_MODE_TUBCLEAN_WASH:
        case APP_NORMAL_MODE_TUBCLEAN_DRAIN:
        case APP_NORMAL_MODE_TUBCLEAN_SPIN:
        case APP_NORMAL_MODE_TUBCLEAN_REGROUP:
        {
            app_lidLock_state_set(APP_LID_LOCK_ON);
        }
        break;

        case APP_NORMAL_MODE_TUBCLEAN_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}
