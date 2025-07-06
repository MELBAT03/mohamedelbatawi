#include "doutput_module/doutput_module.h"
#include "def.h"
#include "board.h"

#include "handler_lock_motor.h"
#include "components/dls35s/dls35s.h"

#define LID_LOCK_MOTOR_ERROR_TIME_MS (5000UL)

#ifdef COMP_MOCKER_ENABLE
static handler_lockMotor_state_t gd_mock_lockMotorState;
static uint8_t gd_mock_lockMotorSensorState;
static uint8_t gd_mock_lockMotorError;
#endif

void handler_lockMotor_init(void)
{
    dls35s_config_t lidLockMotor;

    lidLockMotor.dls35s.gpio.port = LOCK_MOTOR_PORT;
    lidLockMotor.dls35s.gpio.pin = LOCK_MOTOR_PIN;
    lidLockMotor.dls35s.gpio.state = HANDLER_LOCK_MOTOR_INACTIVE;

    lidLockMotor.errorInterval = LID_LOCK_MOTOR_ERROR_TIME_MS;

    lidLockMotor.sensor.type = DINPUT_GPIO_INPUT;
    lidLockMotor.sensor.activeLevel = 0;
    lidLockMotor.sensor.shortPressMultiplier = 1;
    lidLockMotor.sensor.longPressMultiplier = UINT32_MAX;
    lidLockMotor.sensor.gpio.port = LOCK_MOTOR_SENSOR_PORT;
    lidLockMotor.sensor.gpio.pin = LOCK_MOTOR_SENSOR_PIN;

    dls35s_init(&lidLockMotor);
}

handler_lockMotor_state_t handler_lockMotor_state_get(void)
{
#ifdef COMP_MOCKER_ENABLE
    (void)dls35s_currentState_get();
    return gd_mock_lockMotorState;
#else
    return dls35s_currentState_get();
#endif
}

void handler_lockMotor_state_set(handler_lockMotor_state_t state)
{
#ifdef COMP_MOCKER_ENABLE
    dls35s_state_set(state);
    gd_mock_lockMotorSensorState = state;
#else
    dls35s_state_set(state);
#endif
}

uint8_t handler_lockMotor_sensorState_get(void)
{
#ifdef COMP_MOCKER_ENABLE
    (void)dls35s_sensorCurrentState_get();
    return gd_mock_lockMotorSensorState;
#else
    return dls35s_sensorCurrentState_get();
#endif
}

void handler_lockMotor_sensorState_set(uint8_t value)
{
#ifdef COMP_MOCKER_ENABLE
    gd_mock_lockMotorSensorState = value;
#endif
}

uint8_t handler_lockMotor_error_get(void)
{
#ifdef COMP_MOCKER_ENABLE
    (void)dls35s_error_get();
    return gd_mock_lockMotorError;
#else
#ifndef LID_CLOSE_PERMENENT
    return dls35s_error_get();
#else
    return 0;
#endif
#endif
}

void handler_lockMotor_error_set(uint8_t value)
{
#ifdef COMP_MOCKER_ENABLE
    dls35s_error_set(value);
    gd_mock_lockMotorError = value;
#else
    dls35s_error_set(value);
#endif
}
