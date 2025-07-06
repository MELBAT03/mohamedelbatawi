#include "def.h"
#include "app_nm_pause_blower.h"

#include "app_blower.h"

void app_nm_pause_blower_process(uint32_t period)
{
    app_blower_speedControl_set(APP_BLOWER_OFF);
    // TODO: do i need to check the feedback and make sure it is off?
}
