#include "def.h"

#include "app_condenser.h"

#include "app_processor.h"

#include "app_nm_condenser.h"
#include "app_nm_init_condenser.h"
#include "app_nm_wash_condenser.h"
#include "app_nm_dry_condenser.h"
#include "app_nm_tubclean_condenser.h"
#include "app_nm_pause_condenser.h"
#include "app_nm_finish_condenser.h"

#include "app_nm.h"

void app_nm_condenser_process(uint32_t period)
{
    static void (*handlers[])(uint32_t period) = {
        app_nm_init_condenser_process,
        app_nm_wash_condenser_process,
        app_nm_dry_condenser_process,
        app_nm_tubclean_condenser_process,
        app_nm_pause_condenser_process,
        app_nm_finish_condenser_process,
        NULL};

    if (handlers[app_nm_state_get()] != NULL)
    {
        handlers[app_nm_state_get()](period);
    }
}
