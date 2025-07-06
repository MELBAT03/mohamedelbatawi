#include "def.h"

#include "app_lid.h"
#include "app_processor.h"
#include "app_nm_log.h"
#include "app_motor.h"

#include "app_startup_lid.h"
#include "app_setup_lid.h"
#include "app_standby_lid.h"
#include "app_nm_lid.h"
#include "app_tm_lid.h"
#include "app_fault_lid.h"
#include "app_finish_lid.h"

#include "handlers/input/inc/handler_lid_sensor.h"
#include "handlers/output/inc/handler_lock_motor.h"

#define LID_LOCK_UNBALANCE_TIME_MS (10000UL)
#define LID_E2_TIMEOUT_MSEC        (2000UL)

static void lid_error_check(uint32_t period);

static app_lid_safetyState_t gx_app_lid_safetyState = APP_LID_NORMAL;
static app_lidLock_state_t gx_app_lid_state = APP_LID_LOCK_OFF;

void app_lid_update(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        app_startup_lid_process,
        app_setup_lid_process,
        app_standby_lid_process,
        app_nm_lid_process,
        app_tm_lid_process,
        app_fault_lid_process,
        app_finish_lid_process,
        NULL};

    if (app_processor_entryFlag_get() == 0)
    {
        return;
    }

    if (handlers[app_processor_state_get()] != NULL)
    {
        handlers[app_processor_state_get()](period);
    }

    if (APP_LID_LOCK_ON == gx_app_lid_state)
    {
        lid_error_check(period);
    }
}

app_lid_safetyState_t app_lid_faultState_get(void)
{
    return gx_app_lid_safetyState;
}

void app_lidLock_state_set(app_lidLock_state_t lidLockState)
{
    handler_lockMotor_state_set(lidLockState);
    gx_app_lid_state = lidLockState;
}

app_lidLock_state_t app_lidLock_state_get(void)
{
    return handler_lockMotor_state_get();
}

app_lid_sensorState_t app_lidSensor_state_get(void)
{
    return handler_lidSensor_state_get();
}

app_lid_sensorState_t app_lidSensor_edgeState_get(void)
{
    return handler_lidSensor_edgeState_get();
}

void app_lid_fault_clear(void)
{
    handler_lockMotor_error_set(0);
}

static void lid_error_check(uint32_t period)
{
    static uint32_t errorTimeout = 0;

    if (handler_lidSensor_state_get() == HANDLER_LID_SENSOR_OPENED)
    {
        if (errorTimeout >= LID_E2_TIMEOUT_MSEC)
        {
            errorTimeout = 0;
            gx_app_lid_safetyState = APP_LID_FAILURE;
        }
        else
        {
            errorTimeout += period;
        }
    }
    else
    {
        errorTimeout = 0;
        gx_app_lid_safetyState = APP_LID_NORMAL;
    }
}
