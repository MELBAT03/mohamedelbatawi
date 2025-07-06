#include "def.h"

#include "app_nm.h"
#include "app_nm_endurance.h"

#include "app_nm_display.h"
#include "app_display.h"
#include "app_nm_data_display.h"

#include "app_nm_init_display.h"
#include "app_nm_wash_display.h"
#ifdef DRYER_ENABLED
#include "app_nm_dry_display.h"
#endif
#include "app_nm_tubclean_display.h"
#include "app_nm_pause_display.h"
#include "app_nm_finish_display.h"

void app_nm_display_process(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) =
        {
            app_nm_init_display_process,
            app_nm_wash_display_process,
#ifdef DRYER_ENABLED
            app_nm_dry_display_process,
#else
            NULL,
#endif
            app_nm_tubclean_display_process,
            app_nm_pause_display_process,
            app_nm_finish_display_process,
            NULL};

    switch (app_nm_userInterface_state_get())
    {
        case APP_NM_USER_INTERFACE_NORMAL:
        {
            if (app_userInterface_data_set()->normalMode.common.cycleTypeSelection == APP_NORMAL_MODE_UI_CONTINUOUS_CYCLE)
            {
                app_nm_endurance_display_handle();
            }
            else
            {
                if (handlers[app_nm_state_get()] != NULL)
                {
                    handlers[app_nm_state_get()](period);
                }
            }
        }
        break;

        case APP_NM_USER_INTERFACE_DATA_DISPLAY:
        {
            app_nm_display_dataDisplay_handle(period);
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}
