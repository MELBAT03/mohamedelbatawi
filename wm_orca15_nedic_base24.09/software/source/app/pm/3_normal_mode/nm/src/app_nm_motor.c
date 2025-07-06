#include "def.h"

#include "app_motor.h"

#include "app_processor.h"

#include "app_nm_motor.h"
#include "app_nm_init_motor.h"
#ifdef DRYER_ENABLED
#include "app_nm_dry_motor.h"
#endif
#include "app_nm_wash_motor.h"
#include "app_nm_tubclean_motor.h"
#include "app_nm_pause_motor.h"
#include "app_nm_finish_motor.h"

#include "app_nm.h"

void app_nm_motor_process(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        app_nm_init_motor_process,
        app_nm_wash_motor_process,
#ifdef DRYER_ENABLED
        app_nm_dry_motor_process,
#else
        NULL,
#endif
        app_nm_tubclean_motor_process,
        app_nm_pause_motor_process,
        app_nm_finish_motor_process,
        NULL};

    if (handlers[app_nm_state_get()] != NULL)
    {
        handlers[app_nm_state_get()](period);
    }
}
