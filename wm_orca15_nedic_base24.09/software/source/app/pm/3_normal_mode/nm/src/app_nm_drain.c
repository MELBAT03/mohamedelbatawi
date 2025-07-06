#include "def.h"

#include "app_drain.h"
#include "app_fill.h"
#include "app_processor.h"

#include "app_nm_drain.h"
#include "app_nm_init_drain.h"
#ifdef DRYER_ENABLED
#include "app_nm_dry_drain.h"
#endif
#include "app_nm_tubclean_drain.h"
#include "app_nm_pause_drain.h"
#include "app_nm_finish_drain.h"

#include "app_nm_wash_drain.h"

#include "app_nm.h"

#include "app_fault.h"

#include "handlers/output/inc/handler_drain_pump.h"

#define OVERFLOW_PROTECTION_PUMP_OPERATION_TIMEOUT_MSEC (60000U)

void app_nm_drain_process(uint32_t period)
{
    static uint32_t overflowProtectionPumpOperationTimeoutMSec = 0;
    static void (*const handlers[])(uint32_t period) = {
        app_nm_init_drain_process,
        app_nm_wash_drain_process,
#ifdef DRYER_ENABLED
        app_nm_dry_drain_process,
#else
        NULL,
#endif
        app_nm_tubclean_drain_process,
        app_nm_pause_drain_process,
        app_nm_finish_drain_process,
        NULL};

    if (handlers[app_nm_state_get()] != NULL)
    {
        handlers[app_nm_state_get()](period);
    }

    if (OVERFLOW_NONE != app_fill_overFlowState_get())
    {
        overflowProtectionPumpOperationTimeoutMSec += period;
        if (overflowProtectionPumpOperationTimeoutMSec >= OVERFLOW_PROTECTION_PUMP_OPERATION_TIMEOUT_MSEC)
        {
            overflowProtectionPumpOperationTimeoutMSec = 0;
            handler_drainPump_state_set(HANDLER_DRAIN_PUMP_ACTIVE);
        }
    }
    else
    {
        overflowProtectionPumpOperationTimeoutMSec = 0;
    }

    app_drain_pumpSafteyOff_handle(period);
}
