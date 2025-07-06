#include "def.h"

#include "app_fault.h"
#include "app_fault_drain.h"

#include "app_drain.h"

#include "handlers/input/inc/handler_water_level.h"
#include "handlers/output/inc/handler_drain_pump.h"

void app_fault_drain_process(uint32_t period)
{
    if (CHILD_LOCK_PROCEDURE_ACTIVE == app_fault_childLockState_get())
    {
        app_drain_switch(APP_DRAIN_OPEN);

        if (handler_waterLevel_resetLevel_check(WATER_LEVEL_RESET_NORMAL_CHECK) == 0U)
        {
            handler_drainPump_state_set(HANDLER_DRAIN_PUMP_ACTIVE);
        }
        else
        {
            handler_drainPump_state_set(HANDLER_DRAIN_PUMP_INACTIVE);
        }
    }
    else
    {
        app_drain_switch(APP_DRAIN_CLOSE);
    }
}
