#include "def.h"
#include "app_nm_finish_condenser.h"
#include "app_condenser.h"

void app_nm_finish_condenser_process(uint32_t period)
{
    app_condenser_state_set(APP_CONDENSER_OFF);
}
