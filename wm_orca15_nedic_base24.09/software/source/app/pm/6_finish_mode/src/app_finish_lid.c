#include "def.h"

#include "app_lid.h"
#include "app_motor.h"

#include "app_finish_lid.h"

void app_finish_lid_process(uint32_t period)
{
    app_lidLock_state_set(APP_LID_LOCK_OFF);
}
