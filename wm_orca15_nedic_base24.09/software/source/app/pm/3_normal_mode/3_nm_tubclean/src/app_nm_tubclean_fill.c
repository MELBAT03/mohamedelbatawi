#include "def.h"
#include "app_nm_tubclean_fill.h"
#include "app_nm_tubclean.h"
#include "app_fill.h"

void app_nm_tubclean_fill_process(uint32_t period)
{
    switch (app_nm_tubclean_state_get())
    {
        case APP_NORMAL_MODE_TUBCLEAN_INIT:
        case APP_NORMAL_MODE_TUBCLEAN_MH:
        case APP_NORMAL_MODE_TUBCLEAN_MD:
        case APP_NORMAL_MODE_TUBCLEAN_SOAK:
        case APP_NORMAL_MODE_TUBCLEAN_WASH:
        case APP_NORMAL_MODE_TUBCLEAN_DRAIN:
        case APP_NORMAL_MODE_TUBCLEAN_SPIN:
        case APP_NORMAL_MODE_TUBCLEAN_REGROUP:
        case APP_NORMAL_MODE_TUBCLEAN_FINISH:
        {
            app_fill_stop();
        }
        break;

        case APP_NORMAL_MODE_TUBCLEAN_FILL:
        {
            app_fill_handle(period, app_userInterface_data_get()->normalMode.wash.waterTempSelection, APP_FILL_MAIN_WASH);
        }
        break;

        case APP_NORMAL_MODE_TUBCLEAN_FILL_STIR:
        {
            if ((APP_NORMAL_MODE_TUBCLEAN_FILL_BY_LEVEL == app_nm_tubclean_fillStirTable_get(app_nm_tubclean_fillCounter_get())->type) || (APP_NORMAL_MODE_TUBCLEAN_FILL_BY_TIME == app_nm_tubclean_fillStirTable_get(app_nm_tubclean_fillCounter_get())->type))
            {
                app_fill_handle(period, app_userInterface_data_get()->normalMode.wash.waterTempSelection, APP_FILL_MAIN_WASH);
            }
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
