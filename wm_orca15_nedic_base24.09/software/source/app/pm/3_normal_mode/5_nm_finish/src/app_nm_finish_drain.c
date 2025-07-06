#include "def.h"
#include "app_nm_finish_drain.h"

#include "app_drain.h"
#include "app_motor.h"

void app_nm_finish_drain_process(uint32_t period)
{
    if (APP_MOTOR_HALT_OK == app_motor_haltState_get())
    {
        app_drain_switch(APP_DRAIN_CLOSE);
    }
}
