#include "def.h"
#include "app_startup_blower.h"

#include "app_blower.h"

void app_startup_blower_process(uint32_t period)
{
    app_blower_speedControl_set(APP_BLOWER_OFF);
}
