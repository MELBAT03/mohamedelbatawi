#include "def.h"
#include "os.h"
#include "board.h"
#include "components/doutput_module/doutput_module.h"
#include "components/ac_motor/ac_motor.h"
#include "components/hw_timer/hw_timer.h"

#define WEIGHT_DETECTION_REPEATION_COUNTER 4

static void acMotor_command_generate(acMotorCom_command_t command);

acMotor_rpmSensorConfig_t gus_acMotor_rpmSensor;
uint8_t gu8_acMotor_rpmSensor_flag = 0;

static acMotor_controlParameters_t gx_acMotor_controlParameters;
static uint32_t weightKG = 0;
static uint16_t num_of_pulses_CW[5] = {0};
static uint16_t num_of_pulses_CCW[5] = {0};
uint32_t gu32_acMotor_weightNumberOfPulses = 0;
static uint16_t average_number_of_pulses_CW = 0;
static uint16_t average_number_of_pulses_CCW = 0;
static uint8_t gu8_inputModule_internalCounter_CW = 0;
static uint8_t gu8_inputModule_internalCounter_CCW = 0;
static uint32_t gu32_acMotor_rpmFeedback = 0;
static uint32_t gu32_acMotor_testModeTotalPulseCnt = 0;
static uint32_t gu32_acMotor_ccwRPMFeedback = 0;
static uint32_t gu32_acMotor_cwRPMFeedback = 0;

static acMotor_motion_t ge_acMotor_motionState = AC_MOTOR_MOTION_IDLE;
static acMotorCom_state_t ge_acMotor_state = AC_MOTOR_SYNC_STATE;
static acMotorCom_command_t ge_acMotor_motorCommand;

static acMotor_motion_t gx_spinDirection;

static doutputModule_handle gx_cwMotorHandle, gx_ccwMotorHandle;

void acMotor_init(acMotor_config_t *acMotorConfig)
{
    doutputModule_t cw_motor, ccw_motor;

    cw_motor.gpio.port = acMotorConfig->cw.gpio.port;
    cw_motor.gpio.pin = acMotorConfig->cw.gpio.pin;
    cw_motor.gpio.state = acMotorConfig->cw.gpio.state;
    gx_cwMotorHandle = doutputModule_init(&cw_motor);

    ccw_motor.gpio.port = acMotorConfig->ccw.gpio.port;
    ccw_motor.gpio.pin = acMotorConfig->ccw.gpio.pin;
    ccw_motor.gpio.state = acMotorConfig->ccw.gpio.state;
    gx_ccwMotorHandle = doutputModule_init(&ccw_motor);

    gu8_acMotor_rpmSensor_flag = 1;
    gx_spinDirection = acMotorConfig->spinDirection;

    gus_acMotor_rpmSensor.timerModule = acMotorConfig->rpmSensor.timerModule;
    gus_acMotor_rpmSensor.gpio.pin = acMotorConfig->rpmSensor.gpio.pin;
    gus_acMotor_rpmSensor.edge = acMotorConfig->rpmSensor.edge;

    hwTimer_init();
    hwTimer_counterMode_set(&gus_acMotor_rpmSensor.timerModule, &gus_acMotor_rpmSensor.gpio, gus_acMotor_rpmSensor.edge);
    hwTimer_channel_enable(&gus_acMotor_rpmSensor.timerModule);
}

void acMotor_command_set(acMotorCom_command_t command, acMotor_controlParameters_t *controlBlock)
{
    if (controlBlock != NULL)
    {
        gx_acMotor_controlParameters.ccwOFFTime = controlBlock->ccwOFFTime;
        gx_acMotor_controlParameters.ccwONTime = controlBlock->ccwONTime;
        gx_acMotor_controlParameters.cwOFFTime = controlBlock->cwOFFTime;
        gx_acMotor_controlParameters.cwONTime = controlBlock->cwONTime;
        gx_acMotor_controlParameters.motorRPM = controlBlock->motorRPM;
    }

    acMotor_command_generate(command);
}

acMotorCom_state_t acMotor_state_get(void)
{
    return ge_acMotor_state;
}

void acMotor_state_set(acMotorCom_state_t value)
{
    ge_acMotor_state = value;
}

ret_t acMotor_rpmFeedback_get(uint32_t *rpmBuff)
{
    ret_t ret = RET_NOK;

    if (gu8_acMotor_rpmSensor_flag && rpmBuff != NULL)
    {
        *rpmBuff = gu32_acMotor_rpmFeedback;
        ret = RET_OK;
    }

    return ret;
}

void acMotor_rpmFeedback_set(uint32_t value)
{
    gu32_acMotor_rpmFeedback = value;
}

uint32_t acMotor_testModePulseCount_get(void)
{
    return gu32_acMotor_testModeTotalPulseCnt;
}

acMotor_motion_t acMotor_motionState_get(void)
{
    return ge_acMotor_motionState;
}

uint32_t acMotor_weightFeedback_get(void)
{
    return gu32_acMotor_weightNumberOfPulses;
}

void acMotor_weightFeedback_set(uint32_t value)
{
    gu32_acMotor_weightNumberOfPulses = value;
}

uint32_t acMotor_cwRPMFeedback_get(void)
{
    return gu32_acMotor_cwRPMFeedback;
}

uint32_t acMotor_ccwRPMFeedback_get(void)
{
    return gu32_acMotor_ccwRPMFeedback;
}

void acMotor_update(uint32_t period)
{
    static uint32_t stateTimeoutMSec = 0;
    static uint8_t weightDetectionRepeatCnt = 0;
    static uint32_t rpmUpdatePeriod = 0;
    static uint32_t operationTimeoutMSec = 0;

    switch (ge_acMotor_state)
    {
        case AC_MOTOR_SYNC_STATE:
            if (gu32_acMotor_rpmFeedback == 0)
            {
                operationTimeoutMSec += period;
                if (operationTimeoutMSec >= 1000)
                {
                    ge_acMotor_state = AC_MOTOR_OPERATION_STATE;
                }
            }
            break;

        case AC_MOTOR_OPERATION_STATE:
            break;

        default:
            // do nothing
            break;
    }

    if (ge_acMotor_motorCommand != AC_MOTOR_COMMAND_WEIGHT_DETECTION)
    {
        hwTimer_state_set(&gus_acMotor_rpmSensor.timerModule, HW_TIMER_START);

        if (rpmUpdatePeriod >= 100)
        {
            rpmUpdatePeriod = 0;
            gu32_acMotor_rpmFeedback = hwTimer_count_get(&gus_acMotor_rpmSensor.timerModule) * 7 * 6;
            hwTimer_count_reset(&gus_acMotor_rpmSensor.timerModule);
        }
        else
        {
            rpmUpdatePeriod += period;
        }
    }

    switch (ge_acMotor_motionState)
    {
        case AC_MOTOR_MOTION_IDLE:
            hwTimer_state_set(&gus_acMotor_rpmSensor.timerModule, HW_TIMER_START);
            break;

        case AC_MOTOR_MOTION_CW:
            gu32_acMotor_ccwRPMFeedback = 0;
            if (ge_acMotor_motorCommand == AC_MOTOR_COMMAND_WEIGHT_DETECTION)
            {
                hwTimer_state_set(&gus_acMotor_rpmSensor.timerModule, HW_TIMER_STOP);
            }

            if (stateTimeoutMSec >= gx_acMotor_controlParameters.cwONTime)
            {
                ge_acMotor_motionState = AC_MOTOR_MOTION_CW_STOP;
                if (ge_acMotor_motorCommand == AC_MOTOR_COMMAND_WEIGHT_DETECTION)
                {
                    num_of_pulses_CW[gu8_inputModule_internalCounter_CW++] = hwTimer_count_get(&gus_acMotor_rpmSensor.timerModule);
                    gu32_acMotor_cwRPMFeedback = hwTimer_count_get(&gus_acMotor_rpmSensor.timerModule) * 7 * 6;
                    gu32_acMotor_testModeTotalPulseCnt += num_of_pulses_CW[gu8_inputModule_internalCounter_CW - 1];
                    average_number_of_pulses_CW += num_of_pulses_CW[gu8_inputModule_internalCounter_CW - 1];

                    hwTimer_count_reset(&gus_acMotor_rpmSensor.timerModule);
                }
                stateTimeoutMSec = 0;
            }
            else
            {
                stateTimeoutMSec += period;
                doutputModule_state_set(gx_cwMotorHandle, 0);
                doutputModule_state_set(gx_ccwMotorHandle, 1);
                gu32_acMotor_cwRPMFeedback = gu32_acMotor_rpmFeedback;
            }
            break;

        case AC_MOTOR_MOTION_CW_STOP:
            if (ge_acMotor_motorCommand == AC_MOTOR_COMMAND_WEIGHT_DETECTION)
            {
                hwTimer_state_set(&gus_acMotor_rpmSensor.timerModule, HW_TIMER_START);
            }

            if (stateTimeoutMSec >= gx_acMotor_controlParameters.cwOFFTime)
            {
                if (ge_acMotor_motorCommand == AC_MOTOR_COMMAND_SPIN || ge_acMotor_motorCommand == AC_MOTOR_COMMAND_CW)
                {
                    ge_acMotor_motionState = AC_MOTOR_MOTION_CW;
                }
                else if (ge_acMotor_motorCommand == AC_MOTOR_COMMAND_AGITATION || ge_acMotor_motorCommand == AC_MOTOR_COMMAND_WEIGHT_DETECTION)
                {
                    ge_acMotor_motionState = AC_MOTOR_MOTION_CCW;
                }

                stateTimeoutMSec = 0;
            }
            else
            {
                stateTimeoutMSec += period;
                doutputModule_state_set(gx_cwMotorHandle, 0);
                doutputModule_state_set(gx_ccwMotorHandle, 0);
            }
            break;

        case AC_MOTOR_MOTION_CCW:
            gu32_acMotor_cwRPMFeedback = 0;
            if (ge_acMotor_motorCommand == AC_MOTOR_COMMAND_WEIGHT_DETECTION)
            {
                hwTimer_state_set(&gus_acMotor_rpmSensor.timerModule, HW_TIMER_STOP);
            }

            if (stateTimeoutMSec >= gx_acMotor_controlParameters.ccwONTime)
            {
                if (ge_acMotor_motorCommand == AC_MOTOR_COMMAND_WEIGHT_DETECTION)
                {
                    num_of_pulses_CCW[gu8_inputModule_internalCounter_CCW++] = hwTimer_count_get(&gus_acMotor_rpmSensor.timerModule);
                    gu32_acMotor_ccwRPMFeedback = hwTimer_count_get(&gus_acMotor_rpmSensor.timerModule) * 7 * 6;
                    average_number_of_pulses_CCW += num_of_pulses_CCW[gu8_inputModule_internalCounter_CCW - 1];
                    gu32_acMotor_testModeTotalPulseCnt += num_of_pulses_CCW[gu8_inputModule_internalCounter_CCW - 1];

                    hwTimer_count_reset(&gus_acMotor_rpmSensor.timerModule);
                }
                ge_acMotor_motionState = AC_MOTOR_MOTION_CCW_STOP;
                stateTimeoutMSec = 0;
            }
            else
            {
                stateTimeoutMSec += period;
                doutputModule_state_set(gx_ccwMotorHandle, 0);
                doutputModule_state_set(gx_cwMotorHandle, 1);
                gu32_acMotor_ccwRPMFeedback = gu32_acMotor_rpmFeedback;
            }
            break;

        case AC_MOTOR_MOTION_CCW_STOP:
            if (ge_acMotor_motorCommand == AC_MOTOR_COMMAND_WEIGHT_DETECTION)
            {
                hwTimer_state_set(&gus_acMotor_rpmSensor.timerModule, HW_TIMER_START);
            }

            if (stateTimeoutMSec >= gx_acMotor_controlParameters.ccwOFFTime)
            {
                if (ge_acMotor_motorCommand == AC_MOTOR_COMMAND_WEIGHT_DETECTION)
                {
                    weightDetectionRepeatCnt++;
                    if (weightDetectionRepeatCnt < WEIGHT_DETECTION_REPEATION_COUNTER)
                    {
                        ge_acMotor_motionState = AC_MOTOR_MOTION_CW;
                    }
                    else
                    {
                        hwTimer_state_set(&gus_acMotor_rpmSensor.timerModule, HW_TIMER_STOP);

                        num_of_pulses_CW[gu8_inputModule_internalCounter_CW++] = hwTimer_count_get(&gus_acMotor_rpmSensor.timerModule);
                        gu32_acMotor_ccwRPMFeedback = hwTimer_count_get(&gus_acMotor_rpmSensor.timerModule) * 7 * 6;
                        average_number_of_pulses_CW += num_of_pulses_CW[gu8_inputModule_internalCounter_CW - 1];
                        gu32_acMotor_testModeTotalPulseCnt += num_of_pulses_CW[gu8_inputModule_internalCounter_CW - 1];
                        hwTimer_count_reset(&gus_acMotor_rpmSensor.timerModule);

                        gu8_inputModule_internalCounter_CCW = 0;
                        gu8_inputModule_internalCounter_CW = 0;
                        weightDetectionRepeatCnt = 0;
                        ge_acMotor_motionState = AC_MOTOR_MOTION_BRAKE;
                        gu32_acMotor_weightNumberOfPulses = (average_number_of_pulses_CW + average_number_of_pulses_CCW);
                    }
                }
                else if (ge_acMotor_motorCommand == AC_MOTOR_COMMAND_AGITATION)
                {
                    ge_acMotor_motionState = AC_MOTOR_MOTION_CW;
                }
                else if (ge_acMotor_motorCommand == AC_MOTOR_COMMAND_SPIN || ge_acMotor_motorCommand == AC_MOTOR_COMMAND_CCW)
                {
                    ge_acMotor_motionState = AC_MOTOR_MOTION_CCW;
                }

                stateTimeoutMSec = 0;
            }
            else
            {
                stateTimeoutMSec += period;
                doutputModule_state_set(gx_ccwMotorHandle, 0);
                doutputModule_state_set(gx_cwMotorHandle, 0);
            }
            break;

        case AC_MOTOR_MOTION_BRAKE:
            doutputModule_state_set(gx_cwMotorHandle, 0);
            doutputModule_state_set(gx_ccwMotorHandle, 0);
            gu8_inputModule_internalCounter_CCW = 0;
            gu8_inputModule_internalCounter_CW = 0;
            weightDetectionRepeatCnt = 0;
            gu32_acMotor_testModeTotalPulseCnt = 0;
            average_number_of_pulses_CCW = 0;
            average_number_of_pulses_CW = 0;
            stateTimeoutMSec = 0;
            ge_acMotor_motionState = AC_MOTOR_MOTION_IDLE;
            break;

        default:
            // do nothing
            break;
    }
}

static void acMotor_command_generate(acMotorCom_command_t command)
{
    switch (command)
    {
        case AC_MOTOR_COMMAND_CW:
            ge_acMotor_motionState = AC_MOTOR_MOTION_CW;
            ge_acMotor_motorCommand = command;
            break;

        case AC_MOTOR_COMMAND_SPIN:
            if (ge_acMotor_motionState == AC_MOTOR_MOTION_IDLE)
            {
                ge_acMotor_motionState = gx_spinDirection;
                ge_acMotor_motorCommand = command;
            }
            break;

        case AC_MOTOR_COMMAND_CCW:
            ge_acMotor_motionState = AC_MOTOR_MOTION_CCW;
            ge_acMotor_motorCommand = command;
            break;

        case AC_MOTOR_COMMAND_AGITATION:
            if (ge_acMotor_motionState == AC_MOTOR_MOTION_IDLE)
            {
                ge_acMotor_motionState = AC_MOTOR_MOTION_CW;
                ge_acMotor_motorCommand = command;
            }
            break;

        case AC_MOTOR_COMMAND_STOP:
        case AC_MOTOR_COMMAND_BRAKE:
            gx_acMotor_controlParameters.ccwOFFTime = 0;
            gx_acMotor_controlParameters.ccwONTime = 0;
            gx_acMotor_controlParameters.cwOFFTime = 0;
            gx_acMotor_controlParameters.cwONTime = 0;
            gx_acMotor_controlParameters.motorRPM = 0;

            ge_acMotor_motionState = AC_MOTOR_MOTION_BRAKE;
            ge_acMotor_motorCommand = command;
            break;

        case AC_MOTOR_COMMAND_CLUTCH:
        case AC_MOTOR_COMMAND_DECLUTCH:
            gx_acMotor_controlParameters.ccwOFFTime = 0;
            gx_acMotor_controlParameters.ccwONTime = 0;
            gx_acMotor_controlParameters.cwOFFTime = 0;
            gx_acMotor_controlParameters.cwONTime = 0;
            gx_acMotor_controlParameters.motorRPM = 0;

            if (ge_acMotor_motionState == AC_MOTOR_MOTION_IDLE)
            {
                ge_acMotor_motionState = AC_MOTOR_MOTION_BRAKE;
                ge_acMotor_motorCommand = command;
            }
            break;

        case AC_MOTOR_COMMAND_WEIGHT_DETECTION:
            // :TODO: move the profile
            if (ge_acMotor_motionState == AC_MOTOR_MOTION_IDLE)
            {
                gx_acMotor_controlParameters.ccwOFFTime = 600;
                gx_acMotor_controlParameters.ccwONTime = 300;
                gx_acMotor_controlParameters.cwOFFTime = 600;
                gx_acMotor_controlParameters.cwONTime = 300;

                ge_acMotor_motionState = AC_MOTOR_MOTION_CW;
                ge_acMotor_motorCommand = command;
            }
            break;

        default:
            // do nothing
            break;
    }
}
