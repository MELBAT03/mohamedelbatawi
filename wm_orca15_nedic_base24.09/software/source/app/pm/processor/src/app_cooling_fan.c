#include "def.h"
#include "app_processor.h"

#include "app_cooling_fan.h"

#include "app_startup_cooling_fan.h"
#include "app_setup_cooling_fan.h"
#include "app_standby_cooling_fan.h"
#include "app_nm_cooling_fan.h"
#include "app_tm_cooling_fan.h"
#include "app_fault_cooling_fan.h"
#include "app_finish_cooling_fan.h"

#include "handlers/output/inc/handler_cooling_fan.h"

void app_coolingFan_update(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        app_startup_coolingFan_process,
        app_setup_coolingFan_process,
        app_standby_coolingFan_process,
        app_nm_coolingFan_process,
        app_tm_coolingFan_process,
        app_fault_coolingFan_process,
        app_finish_coolingFan_process,
        NULL};

    if (app_processor_entryFlag_get() == 0)
    {
        return;
    }

    if (handlers[app_processor_state_get()] != NULL)
    {
        handlers[app_processor_state_get()](period);
    }
}

void app_coolingFan_state_set(app_coolingFan_state_t state)
{
    handler_coolingFan_state_set(state);
}

uint32_t app_coolingFan_speed_get(void)
{
    return handler_coolingFan_speed_get();
}
