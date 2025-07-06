#include "def.h"

#include "app_lid.h"

#include "app_processor.h"

#include "app_nm_lid.h"
#include "app_nm_init_lid.h"
#include "app_nm_wash_lid.h"
#ifdef DRYER_ENABLED
#include "app_nm_dry_lid.h"
#endif
#include "app_nm_tubclean_lid.h"
#include "app_nm_pause_lid.h"
#include "app_nm_finish_lid.h"

#include "app_nm.h"

void app_nm_lid_process(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        app_nm_init_lid_process,
        app_nm_wash_lid_process,
#ifdef DRYER_ENABLED
        app_nm_dry_lid_process,
#else
        NULL,
#endif
        app_nm_tubclean_lid_process,
        app_nm_pause_lid_process,
        app_nm_finish_lid_process,
        NULL};

    if (handlers[app_nm_state_get()] != NULL)
    {
        handlers[app_nm_state_get()](period);
    }
}
