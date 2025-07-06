#ifndef APP_LID_H
#define APP_LID_H

#include "def.h"

#include "handlers/output/inc/handler_lock_motor.h"
#include "handlers/input/inc/handler_lid_sensor.h"

typedef enum app_lid_safetyState
{
    APP_LID_NORMAL = 0,
    APP_LID_FAILURE = 1
} app_lid_safetyState_t;

typedef handler_lidSensor_state_t app_lid_sensorState_t;

#define APP_LID_SENSOR_CLOSED (HANDLER_LID_SENSOR_CLOSED)
#define APP_LID_SENSOR_OPENED (HANDLER_LID_SENSOR_OPENED)

typedef handler_lockMotor_state_t app_lidLock_state_t;

#define APP_LID_LOCK_OFF (HANDLER_LOCK_MOTOR_INACTIVE)
#define APP_LID_LOCK_ON  (HANDLER_LOCK_MOTOR_ACTIVE)

void app_lid_update(uint32_t period);

void app_lidLock_state_set(app_lidLock_state_t lidLockState);

app_lidLock_state_t app_lidLock_state_get(void);

app_lid_sensorState_t app_lidSensor_state_get(void);
app_lid_sensorState_t app_lidSensor_edgeState_get(void);

app_lid_safetyState_t app_lid_faultState_get(void);

void app_lid_fault_clear(void);

#endif
