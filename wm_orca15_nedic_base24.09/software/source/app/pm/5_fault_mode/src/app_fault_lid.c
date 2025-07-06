#include "def.h"

#include "app_lid.h"

#include "app_fault_lid.h"
#include "app_nm_log.h"
#include "app_motor.h"

void app_fault_lid_process(uint32_t period)
{
    if ((APP_MOTOR_HALT_OK == app_motor_haltState_get()) && (app_nm_log_error_get() != APP_NORMAL_MODE_LOG_ERROR_LID_OPENING_FAILURE))
    {
        app_lidLock_state_set(APP_LID_LOCK_OFF);
    }
}
