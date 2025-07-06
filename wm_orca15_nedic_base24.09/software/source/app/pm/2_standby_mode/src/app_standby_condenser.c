#include "def.h"
#include "app_standby_condenser.h"

#include "app_condenser.h"

void app_standby_condenser_process(uint32_t period)
{
    app_condenser_state_set(APP_CONDENSER_OFF);
}
