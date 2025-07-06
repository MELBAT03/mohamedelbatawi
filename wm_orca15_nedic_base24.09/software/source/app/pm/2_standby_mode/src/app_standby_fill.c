#include "def.h"
#include "config.h"
#include "app_fill.h"
#include "app_standby_fill.h"
#include "app_memory.h"

#include "handlers/input/inc/handler_detergent_sensor.h"

void app_standby_fill_process(uint32_t period)
{
    app_fill_stop();

#ifdef AUTO_DETERGENT_HEATER_MODEL
    if (APP_FILL_AUTO_DETERGENT_MAYBE_LOW == app_fill_autoDetergentState_get())
    {
        app_memory_autoDetergentFirstTime_set(MEMORY_AUTO_DETERGENT_FIRST_TIME_RECORDED);
    }
    else if (APP_FILL_AUTO_DETERGENT_NORMAL == app_fill_autoDetergentState_get())
    {
        app_memory_autoDetergentFirstTime_set(MEMORY_AUTO_DETERGENT_FIRST_TIME_NOT_RECORDED);
    }
    else
    {
        // do nothing
    }

    if (APP_FILL_AUTO_SOFTENER_MAYBE_LOW == app_fill_autoSoftenerState_get())
    {
        app_memory_autoSoftenerFirstTime_set(MEMORY_AUTO_DETERGENT_FIRST_TIME_RECORDED);
    }
    else if (APP_FILL_AUTO_SOFTENER_NORMAL == app_fill_autoSoftenerState_get())
    {
        app_memory_autoSoftenerFirstTime_set(MEMORY_AUTO_DETERGENT_FIRST_TIME_NOT_RECORDED);
    }
    else
    {
        // do nothing
    }
#endif
}
