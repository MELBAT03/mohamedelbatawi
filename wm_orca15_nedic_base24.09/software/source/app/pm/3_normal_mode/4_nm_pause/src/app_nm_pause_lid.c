#include "def.h"

#include "app_nm_pause_lid.h"

#include "app_nm.h"
#include "app_lid.h"

#include "app_motor.h"

void app_nm_pause_lid_process(uint32_t period)
{
    if (APP_MOTOR_HALT_OK == app_motor_haltState_get())
    {
        app_lidLock_state_set(APP_LID_LOCK_OFF);
    }
}
