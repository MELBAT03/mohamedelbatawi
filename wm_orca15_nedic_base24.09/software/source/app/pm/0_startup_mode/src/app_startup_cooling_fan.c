#include "def.h"

#include "app_startup_cooling_fan.h"

#include "app_cooling_fan.h"

void app_startup_coolingFan_process(uint32_t period)
{
    app_coolingFan_state_set(APP_COOLING_FAN_OFF);
}
