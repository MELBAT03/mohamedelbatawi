#include "def.h"
#include "app_condenser.h"
#include "app_nm_wash_condenser.h"

void app_nm_wash_condenser_process(uint32_t period)
{
    app_condenser_state_set(APP_CONDENSER_OFF);
}
