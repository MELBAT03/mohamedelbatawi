#include "def.h"

#include "app_nm_pause_motor.h"

#include "app_motor.h"
#include "app_unbalance.h"

void app_nm_pause_motor_process(uint32_t period)
{
    app_motor_brake();
    app_unbalance_reset();
    if (APP_MOTOR_HALT_OK == app_motor_haltState_get())
    {
        app_motor_data_get()->clutchState = app_motor_clutch_switch(APP_MOTOR_CLUTCH_WITHOUT_BASKET);
    }

    app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
}
