#ifndef APP_MOTOR_H
#define APP_MOTOR_H

#include "def.h"
#include "handlers/output/inc/handler_motor.h"
#include "handlers/output/inc/handler_clutch.h"

#include "app_nm_user_interface.h"

#define APP_MOTOR_SPIN_BRAKE_TIME_MSEC (30000U)

typedef handler_motor_state_t app_motor_operationState_t;
#define APP_MOTOR_OPERATION_STATE_SYNC                (HANDLER_MOTOR_SYNC_STATE)
#define APP_MOTOR_OPERATION_STATE_BRAKE               (HANDLER_MOTOR_BRAKE_STATE)
#define APP_MOTOR_OPERATION_STATE_FORCE_BRAKE_STARTUP (HANDLER_MOTOR_FORCE_BRAKE_STARTUP)
#define APP_MOTOR_OPERATION_STATE_FORCE_BRAKE_CANCEL  (HANDLER_MOTOR_FORCE_BRAKE_CANCEL)
#define APP_MOTOR_OPERATION_STATE_STARTUP_DONE        (HANDLER_MOTOR_FORCE_BRAKE_STARTUP_DONE)
#define APP_MOTOR_OPERATION_STATE_OPERATION           (HANDLER_MOTOR_OPERATION_STATE)
#define APP_MOTOR_OPERATION_STATE_FAULT               (HANDLER_MOTOR_FAULT_STATE)

typedef handler_motor_command_t app_motor_command_t;
#define APP_MOTOR_COMMAND_CLUTCH           HANDLER_MOTOR_COMMAND_CLUTCH
#define APP_MOTOR_COMMAND_DECLUTCH         HANDLER_MOTOR_COMMAND_DECLUTCH
#define APP_MOTOR_COMMAND_BRAKE            HANDLER_MOTOR_COMMAND_BRAKE
#define APP_MOTOR_COMMAND_STOP             HANDLER_MOTOR_COMMAND_STOP
#define APP_MOTOR_COMMAND_AGITATION        HANDLER_MOTOR_COMMAND_AGITATION
#define APP_MOTOR_COMMAND_SPIN             HANDLER_MOTOR_COMMAND_SPIN
#define APP_MOTOR_COMMAND_SPIN_PULSATOR    HANDLER_MOTOR_COMMAND_SPIN_PULSATOR
#define APP_MOTOR_COMMAND_WEIGHT_DETECTION HANDLER_MOTOR_COMMAND_WEIGHT_DETECTION
#define APP_MOTOR_COMMAND_CW               HANDLER_MOTOR_COMMAND_CW
#define APP_MOTOR_COMMAND_CCW              HANDLER_MOTOR_COMMAND_CCW

typedef enum app_motor_haltState
{
    APP_MOTOR_HALT_OK = 1,
    APP_MOTOR_HALT_NOK = 0
} app_motor_haltState_t;

typedef enum app_motor_resetState
{
    APP_MOTOR_RESET_NOK = 0,
    APP_MOTOR_RESET_OK = 1,
    APP_MOTOR_RESET_FAILURE = 2
} app_motor_resetState_t;

typedef enum app_motor_clutchResetState
{
    APP_CLUTCH_RESET_OK = 1,
    APP_CLUTCH_RESET_NOK = 0
} app_motor_clutchResetState_t;

typedef enum app_motor_clutchEngagementState
{
    APP_CLUTCH_ENGAGEMENT_OK = 1,
    APP_CLUTCH_ENGAGEMENT_NOK = 0
} app_motor_clutchEngagementState_t;

typedef enum app_mmotor_clutchMode
{
    APP_CLUTCH_MODE_HALTING_MOTOR,
    APP_CLUTCH_MODE_SWITCHING,
    APP_CLUTCH_MODE_OPERATION
} app_motor_clutchMode_t;

typedef handler_clutch_state_t app_motor_clutchState_t;

#define APP_MOTOR_CLUTCH_WITHOUT_BASKET (HANDLER_CLUTCH_WITHOUT_BASKET)
#define APP_MOTOR_CLUTCH_WITH_BASKET    (HANDLER_CLUTCH_WITH_BASKET)
#define APP_MOTOR_CLUTCH_PROCESSING     (HANDLER_CLUTCH_PROCESSING)

typedef handler_motor_motionPattern_t app_motor_motionPattern_t;

typedef struct app_motor_spinProfileSectionData
{
    uint32_t sectionRPM;
    uint32_t sectionOnTimeMSec;
    uint32_t sectionOffTimeMSec;
    uint32_t sectionTimeMSec;
} app_motor_spinProfileSectionData_t;

typedef enum app_motor_spinProfileSection
{
    SPIN_PROFILE_SECTION_0,
    SPIN_PROFILE_SECTION_1,
    SPIN_PROFILE_SECTION_2,
    SPIN_PROFILE_SECTION_3,
    SPIN_PROFILE_SECTION_4,
    SPIN_PROFILE_SECTION_5,
    SPIN_PROFILE_SECTION_6,
    SPIN_PFORILE_SECTION_NUM
} app_motor_spinProfileSection_t;

typedef struct app_motor_spinProfile
{
    app_motor_spinProfileSectionData_t profile[SPIN_PFORILE_SECTION_NUM];
} app_motor_spinProfile_t;

typedef enum app_motor_spinState
{
    SPIN_RUN_STAGE,
    SPIN_BREAK_STAGE
} app_motor_spinState_t;

typedef struct app_motor_data
{
    app_motor_clutchState_t clutchState;
    app_motor_clutchMode_t clutchMode;
    app_motor_clutchEngagementState_t clutchEngagementMotionState;
    app_motor_spinState_t spinState;
} app_motor_data_t;

typedef struct app_motor_clutchModeHandlers
{
    void (*motorHalt)(uint32_t period);
    void (*clutchSwitching)(uint32_t period);
    void (*operation)(uint32_t period);
} app_motor_clutchModeHandlers_t;

void app_motor_update(uint32_t period);

app_motor_data_t *app_motor_data_get(void);

app_motor_operationState_t app_motor_operationState_get(void);

app_motor_haltState_t app_motor_haltState_get(void);
void app_motor_haltState_set(app_motor_haltState_t value);

app_motor_resetState_t app_motor_resetState_get(void);
void app_motor_resetState_set(app_motor_resetState_t value);

app_motor_clutchResetState_t app_motor_clutchResetState_get(void);
void app_motor_clutchResetState_set(app_motor_clutchResetState_t value);

app_motor_clutchState_t app_motor_clutch_switch(app_motor_clutchState_t state);

void app_motor_process(uint32_t period, handler_clutch_state_t clutch, app_motor_command_t clutchMotion, app_motor_clutchModeHandlers_t *callback);

app_motor_clutchEngagementState_t app_motor_clutchEngementMotion_process(uint32_t period, app_motor_command_t command);

uint32_t app_motor_speed_get(void);

void app_motor_agitationPattern_handle(const app_motor_motionPattern_t *pattern);
void app_motor_spinPattern_handle(const app_motor_spinProfileSectionData_t *section);

void app_motor_weightDetection_handle(void);
void app_motor_brake(void);

uint32_t app_motor_brakeWaitTimeoutMSec_get(void);

void app_motor_motion_set(app_motor_command_t command, const app_motor_motionPattern_t *motion);
app_motor_haltState_t app_motor_halt_check(void);

void app_motor_reset(void);

#endif
