#include "def.h"
#include "app_motor.h"

#include "app_startup_motor.h"

void app_startup_motor_process(uint32_t period)
{
    app_motor_brake();
}
