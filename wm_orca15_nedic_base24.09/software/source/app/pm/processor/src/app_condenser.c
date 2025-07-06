#include "def.h"

#include "app_condenser.h"
#include "app_processor.h"

#include "app_startup_condenser.h"
#include "app_setup_condenser.h"
#include "app_standby_condenser.h"
#include "app_nm_condenser.h"
#include "app_tm_condenser.h"
#include "app_fault_condenser.h"
#include "app_finish_condenser.h"

#include "handlers/output/inc/handler_water_valve.h"

static app_condenser_state_t gx_dry_condenserState;

void app_condenser_update(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        app_startup_condenser_process,
        app_setup_condenser_process,
        app_standby_condenser_process,
        app_nm_condenser_process,
        app_tm_condenser_process,
        app_fault_condenser_process,
        app_finish_condenser_process,
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

void app_condenser_state_set(app_condenser_state_t state)
{
#ifndef TESTING
    handler_condenserWaterValve_state_set(state);
#endif
    gx_dry_condenserState = state;
}

uint32_t app_condenser_state_get(void)
{
    return gx_dry_condenserState;
}
