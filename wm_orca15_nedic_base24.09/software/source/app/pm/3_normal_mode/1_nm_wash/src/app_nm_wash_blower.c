#include "def.h"
#include "app_condenser.h"
#include "app_nm_wash_condenser.h"
#include "app_nm_wash.h"

#include "app_blower.h"

void app_nm_wash_blower_process(uint32_t period)
{
    if (APP_NORMAL_MODE_WASH_STEAM != app_nm_wash_state_get())
    {
        app_blower_speedControl_set(APP_BLOWER_OFF);
    }
    else
    {
        app_blower_speedControl_set(BLOWER_TARGET_POWER);
    }
}
