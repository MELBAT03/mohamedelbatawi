#include "def.h"
#include "app_motor.h"
#include "app_fault.h"

#include "app_fault_motor.h"

void app_fault_motor_process(uint32_t period)
{
    app_motor_brake();
    if ((APP_MOTOR_HALT_OK == app_motor_haltState_get()) && (CHILD_LOCK_PROCEDURE_INACTIVE == app_fault_childLockState_get()))
    {
        app_motor_data_get()->clutchState = app_motor_clutch_switch(APP_MOTOR_CLUTCH_WITHOUT_BASKET);
    }
    else if ((APP_MOTOR_HALT_OK == app_motor_haltState_get()) && (CHILD_LOCK_PROCEDURE_ACTIVE == app_fault_childLockState_get()))
    {
        app_motor_data_get()->clutchState = app_motor_clutch_switch(APP_MOTOR_CLUTCH_WITH_BASKET);
    }
    else
    {
        // do nothing
    }
}
