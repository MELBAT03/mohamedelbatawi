#ifndef COMPONENT_AC_MOTOR_H
#define COMPONENT_AC_MOTOR_H 1

#include "def.h"
#include "components/hw_timer/hw_timer.h"
#include "components/doutput_module/doutput_module.h"

typedef enum
{
    AC_MOTOR_MOTION_IDLE,
    AC_MOTOR_MOTION_CCW,
    AC_MOTOR_MOTION_CCW_STOP,
    AC_MOTOR_MOTION_CW,
    AC_MOTOR_MOTION_CW_STOP,
    AC_MOTOR_MOTION_BRAKE
} acMotor_motion_t;

typedef struct
{
    hwTimer_t timerModule;
    gpio_t gpio;
    hwTimer_eventEdge_t edge;
} acMotor_rpmSensorConfig_t;

typedef struct
{
    doutputModule_t cw;
    doutputModule_t ccw;
    acMotor_rpmSensorConfig_t rpmSensor;
    acMotor_motion_t spinDirection;
} acMotor_config_t;

typedef struct
{
    uint32_t motorRPM;
    uint32_t cwONTime;
    uint32_t ccwONTime;
    uint32_t cwOFFTime;
    uint32_t ccwOFFTime;
} acMotor_controlParameters_t;

typedef enum
{
    AC_MOTOR_COMMAND_CW,
    AC_MOTOR_COMMAND_CCW,
    AC_MOTOR_COMMAND_SPIN,
    AC_MOTOR_COMMAND_AGITATION,
    AC_MOTOR_COMMAND_STOP,
    AC_MOTOR_COMMAND_BRAKE,
    AC_MOTOR_COMMAND_CLUTCH,
    AC_MOTOR_COMMAND_DECLUTCH,
    AC_MOTOR_COMMAND_WEIGHT_DETECTION
} acMotorCom_command_t;

typedef enum
{
    AC_MOTOR_SYNC_STATE,
    AC_MOTOR_OPERATION_STATE
} acMotorCom_state_t;


void acMotor_init(acMotor_config_t *acMotorConfig);
void acMotor_update(uint32_t period);

acMotorCom_state_t acMotor_state_get(void);
ret_t acMotor_rpmFeedback_get(uint32_t *rpmBuff);
uint32_t acMotor_testModePulseCount_get(void);
acMotor_motion_t acMotor_motionState_get(void);
uint32_t acMotor_weightFeedback_get(void);
uint32_t acMotor_cwRPMFeedback_get(void);
uint32_t acMotor_ccwRPMFeedback_get(void);

void acMotor_command_set(acMotorCom_command_t command, acMotor_controlParameters_t *controlBlock);
void acMotor_state_set(acMotorCom_state_t value);
void acMotor_rpmFeedback_set(uint32_t value);
void acMotor_weightFeedback_set(uint32_t value);

#endif
