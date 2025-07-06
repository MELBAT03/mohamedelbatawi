#include "def.h"

#include "app_lid.h"

#include "app_setup_lid.h"

void app_setup_lid_process(uint32_t period)
{
    handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_RESET);
}
