#include "def.h"

#include "app_heater.h"

#include "app_processor.h"

#include "app_nm_heater.h"
#include "app_nm_init_heater.h"
#include "app_nm_wash_heater.h"
#ifdef DRYER_ENABLED
#include "app_nm_dry_heater.h"
#endif
#include "app_nm_tubclean_heater.h"
#include "app_nm_pause_heater.h"
#include "app_nm_finish_heater.h"

#include "app_nm.h"

void app_nm_heater_process(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        app_nm_init_heater_process,
        app_nm_wash_heater_process,
#ifdef DRYER_ENABLED
        app_nm_dry_heater_process,
#else
        NULL,
#endif
        app_nm_tubclean_heater_process,
        app_nm_pause_heater_process,
        app_nm_finish_heater_process,
        NULL};

    if (handlers[app_nm_state_get()] != NULL)
    {
        handlers[app_nm_state_get()](period);
    }
}
