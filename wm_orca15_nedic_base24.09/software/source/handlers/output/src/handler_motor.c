#include "def.h"
#include "board.h"

#include "handlers/output/inc/handler_motor.h"
#include "components/ddm_motor/ddm_motor.h"

#include "app_memory.h"

#ifdef COMP_MOCKER_ENABLE
static uint32_t gu32_mock_motorSpeed;
static uint8_t gu8_mock_motorError;
static uint8_t gu8_mock_motorClutchDoneFlag;
static uint8_t gu8_mock_motorState;
#endif

void handler_motor_init(void)
{
    DDM_t motor;

    motor.uartx = (uart_t)MOTOR_COM_PORT_ID;
    motor.txIntervalMS = 500;
    motor.receiveTimeoutMS = 1000;
    motor.platform = MODEL_NUMBER;
    motor.rawTubValue = app_memory_weightData_get();

    DDM_init(&motor);
}

uint32_t handler_motor_weightFeedback_get(void)
{
    return DDM_weightGramFeedback_get();
}

uint32_t handler_motor_weightRawFeedback_get(void)
{
    return DDM_weightRawFeedback_get();
}

void handler_motor_weightFeedback_set(uint32_t value)
{
    DDM_weightGramFeedback_set(value);
}

uint32_t handler_motor_testModePulseCount_get(void)
{
    return DDM_weightGramFeedback_get();
}

void handler_motor_command_set(handler_motor_command_t command, const handler_motor_motionPattern_t *pattern)
{
    DDM_command_set(command, pattern);
}

handler_motor_state_t handler_motor_state_get(void)
{
#ifdef COMP_MOCKER_ENABLE
    (void)DDM_state_get();
    return gu8_mock_motorState;
#else
    return DDM_state_get();
#endif
}

void handler_motor_state_set(handler_motor_state_t value)
{
#ifdef COMP_MOCKER_ENABLE
    DDM_state_set(value);
    gu8_mock_motorState = value;
#else
    DDM_state_set(value);
#endif
}

uint16_t handler_motor_version_get(void)
{
    return DDM_swVersion_get();
}

uint8_t handler_motor_clutchDoneFlag_get(void)
{
#ifdef COMP_MOCKER_ENABLE
    (void)DDM_clutchDoneFlag_get();
    return gu8_mock_motorClutchDoneFlag;
#else
    return DDM_clutchDoneFlag_get();
#endif
}

void handler_motor_clutchDoneFlag_set(uint8_t value)
{
#ifdef COMP_MOCKER_ENABLE
    DDM_clutchDoneFlag_set(value);
    gu8_mock_motorClutchDoneFlag = value;
#else
    DDM_clutchDoneFlag_set(value);
#endif
}

handler_motor_error_t handler_motor_error_get(void)
{
#ifdef COMP_MOCKER_ENABLE
    (void)DDM_error_get();
    return gu8_mock_motorError;
#else
    return DDM_error_get();
#endif
}

void handler_motor_error_set(handler_motor_error_t error)
{
#ifdef COMP_MOCKER_ENABLE
    gu8_mock_motorError = error;
#endif
}

uint32_t handler_motor_speed_get(void)
{
#ifdef COMP_MOCKER_ENABLE
    (void)DDM_speed_get();
    return gu32_mock_motorSpeed;
#else
    return DDM_speed_get();
#endif
}

void handler_motor_speed_set(uint32_t value)
{
#ifdef COMP_MOCKER_ENABLE
    DDM_speed_set(value);
    gu32_mock_motorSpeed = value;
#else
    DDM_speed_set(value);
#endif
}
