#include "def.h"
#include "app_nm_tubclean.h"
#include "app_nm_tubclean_drain.h"
#include "app_drain.h"

void app_nm_tubclean_drain_process(uint32_t period)
{
    switch (app_nm_tubclean_state_get())
    {
        case APP_NORMAL_MODE_TUBCLEAN_INIT:
        case APP_NORMAL_MODE_TUBCLEAN_FILL:
        case APP_NORMAL_MODE_TUBCLEAN_MH:
        case APP_NORMAL_MODE_TUBCLEAN_MD:
        case APP_NORMAL_MODE_TUBCLEAN_FILL_STIR:
        case APP_NORMAL_MODE_TUBCLEAN_SOAK:
        case APP_NORMAL_MODE_TUBCLEAN_WASH:
        {
            app_drain_switch(APP_DRAIN_CLOSE);
        }
        break;

        case APP_NORMAL_MODE_TUBCLEAN_DRAIN:
        {
            app_drain_switch(APP_DRAIN_OPEN);
            app_drain_error_check(period);
        }
        break;

        case APP_NORMAL_MODE_TUBCLEAN_SPIN:
        {
            app_drain_switch(APP_DRAIN_OPEN);
        }
        break;

        case APP_NORMAL_MODE_TUBCLEAN_REGROUP:
        case APP_NORMAL_MODE_TUBCLEAN_FINISH:
        case APP_NORMAL_MODE_TUBCLEAN_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}
