#include "def.h"

#include "app_processor.h"

#include "app_blower.h"

#include "app_nm_blower.h"
#include "app_nm_init_blower.h"
#include "app_nm_wash_blower.h"
#include "app_nm_dry_blower.h"
#include "app_nm_tubclean_blower.h"
#include "app_nm_pause_blower.h"
#include "app_nm_finish_blower.h"

#include "app_nm.h"

void app_nm_blower_process(uint32_t period)
{
    static void (*handlers[])(uint32_t period) = {
        app_nm_init_blower_process,
        app_nm_wash_blower_process,
        app_nm_dry_blower_process,
        app_nm_tubclean_blower_process,
        app_nm_pause_blower_process,
        app_nm_finish_blower_process,
        NULL};

    if (handlers[app_nm_state_get()] != NULL)
    {
        handlers[app_nm_state_get()](period);
    }
}
