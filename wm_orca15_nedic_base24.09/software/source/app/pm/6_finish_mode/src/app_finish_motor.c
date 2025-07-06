#include "def.h"
#include "app_motor.h"

#include "app_finish_motor.h"

void app_finish_motor_process(uint32_t period)
{
    app_motor_brake();
    if (APP_MOTOR_HALT_OK == app_motor_haltState_get())
    {
        app_motor_data_get()->clutchState = app_motor_clutch_switch(APP_MOTOR_CLUTCH_WITHOUT_BASKET);
    }
}
