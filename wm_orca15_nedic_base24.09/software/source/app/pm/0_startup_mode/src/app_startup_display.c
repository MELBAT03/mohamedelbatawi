#include "def.h"

#include "app_startup_display.h"
#include "app_display.h"
#include "app_user_interface.h"

#include "handlers/output/inc/handler_display.h"
#include "handlers/input/inc/handler_water_level.h"

void app_startup_display_process(uint32_t period)
{
    app_display_fire();

    if (0U == handler_waterLevel_calibrationFlag_get())
    {
        handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, 0);
    }
}
