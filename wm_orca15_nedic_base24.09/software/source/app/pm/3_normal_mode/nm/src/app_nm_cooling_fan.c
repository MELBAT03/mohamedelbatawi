#include "def.h"

#include "app_processor.h"

#include "app_cooling_fan.h"

#include "app_nm_cooling_fan.h"
#include "app_nm_init_cooling_fan.h"
#include "app_nm_wash_cooling_fan.h"
#include "app_nm_dry_cooling_fan.h"
#include "app_nm_tubclean_cooling_fan.h"
#include "app_nm_pause_cooling_fan.h"
#include "app_nm_finish_cooling_fan.h"

#include "app_nm.h"

void app_nm_coolingFan_process(uint32_t period)
{
    static void (*handlers[])(uint32_t period) = {
        app_nm_init_coolingFan_process,
        app_nm_wash_coolingFan_process,
        app_nm_dry_coolingFan_process,
        app_nm_tubclean_coolingFan_process,
        app_nm_pause_coolingFan_process,
        app_nm_finish_coolingFan_process,
        NULL};

    if (handlers[app_nm_state_get()] != NULL)
    {
        handlers[app_nm_state_get()](period);
    }
}
