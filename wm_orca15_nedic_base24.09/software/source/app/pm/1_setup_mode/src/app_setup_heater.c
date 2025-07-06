#include "def.h"

#include "app_heater.h"

#include "app_setup_heater.h"

void app_setup_heater_process(uint32_t period)
{
    app_heater_plateState_set(APP_HEATER_OFF);
}
