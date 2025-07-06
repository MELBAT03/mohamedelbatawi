#include "def.h"

#include "app_fill.h"

#include "app_processor.h"

#include "app_nm_fill.h"
#include "app_nm_init_fill.h"
#include "app_nm_wash_fill.h"
#ifdef DRYER_ENABLED
#include "app_nm_dry_fill.h"
#endif
#include "app_nm_tubclean_fill.h"
#include "app_nm_pause_fill.h"
#include "app_nm_finish_fill.h"

#include "app_nm.h"
#include "app_nm_log.h"

#define FILL_DELAY_MS (3000UL)

void app_nm_fill_process(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        app_nm_init_fill_process,
        app_nm_wash_fill_process,
#ifdef DRYER_ENABLED
        app_nm_dry_fill_process,
#else
        NULL,
#endif
        app_nm_tubclean_fill_process,
        app_nm_pause_fill_process,
        app_nm_finish_fill_process,
        NULL};

    if (handlers[app_nm_state_get()] != NULL)
    {
        handlers[app_nm_state_get()](period);
    }

    if (OVERFLOW_NONE != app_fill_overFlowState_get())
    {
        app_fill_stop();
    }
}
