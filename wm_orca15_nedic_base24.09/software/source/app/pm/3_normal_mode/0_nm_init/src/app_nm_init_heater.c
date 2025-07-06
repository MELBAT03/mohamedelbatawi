#include "def.h"
#include "app_nm_init_heater.h"

#include "app_heater.h"

void app_nm_init_heater_process(uint32_t period)
{
    app_heater_plateState_set(APP_HEATER_OFF);
}
