#include "def.h"

#include "app_motor.h"

#include "app_processor.h"
#include "app_fault.h"
#include "app_unbalance.h"
#include "app_nm_log.h"

#include "app_startup_motor.h"
#include "app_setup_motor.h"
#include "app_standby_motor.h"
#include "app_nm_motor.h"
#include "app_tm_motor.h"
#include "app_fault_motor.h"
#include "app_finish_motor.h"

#include "app_user_interface.h"

#include "handlers/output/inc/handler_motor.h"
#include "handlers/output/inc/handler_clutch.h"
#include "handlers/input/inc/handler_mems.h"

#define MOTOR_CLUTCH_ENGAGEMENT_MOTION_TIMEOUT_MSEC (10000U)
#define MOTOR_CLUTCH_ENGAGEMENT_HALT_TIMEOUT_MSEC   (1000U)

#define MOTOR_MIN_BRAKE_TIME_MSEC (30000U)

#define MOTOR_HALT_ASSURANCE_COUNT (30U)

#define MOTOR_ROTATION_ERRO_COUNT (100U)

static app_motor_haltState_t gx_motor_haltState = APP_MOTOR_HALT_NOK;
static app_motor_resetState_t gx_motor_resetState = APP_MOTOR_RESET_NOK;
static app_motor_clutchResetState_t gx_motor_clutchResetState = APP_CLUTCH_RESET_NOK;

static app_motor_data_t gx_motor_data;

void app_motor_update(uint32_t period)
{
    gx_motor_haltState = app_motor_halt_check();

    static void (*const handlers[])(uint32_t period) = {
        app_startup_motor_process,
        app_setup_motor_process,
        app_standby_motor_process,
        app_nm_motor_process,
        app_tm_motor_process,
        app_fault_motor_process,
        app_finish_motor_process,
        NULL};

    if (app_processor_entryFlag_get() == 0)
    {
        return;
    }

    if (handlers[app_processor_state_get()] != NULL)
    {
        handlers[app_processor_state_get()](period);
    }
}

app_motor_data_t *app_motor_data_get(void)
{
    return &gx_motor_data;
}

app_motor_operationState_t app_motor_operationState_get(void)
{
    return handler_motor_state_get();
}

app_motor_haltState_t app_motor_haltState_get(void)
{
    return gx_motor_haltState;
}

void app_motor_haltState_set(app_motor_haltState_t value)
{
    gx_motor_haltState = value;
}

app_motor_resetState_t app_motor_resetState_get(void)
{
    return gx_motor_resetState;
}

void app_motor_resetState_set(app_motor_resetState_t value)
{
    gx_motor_resetState = value;
}

app_motor_clutchResetState_t app_motor_clutchResetState_get(void)
{
    return gx_motor_clutchResetState;
}
void app_motor_clutchResetState_set(app_motor_clutchResetState_t value)
{
    gx_motor_clutchResetState = value;
}

app_motor_clutchState_t app_motor_clutch_switch(app_motor_clutchState_t state)
{
    return handler_clutch_switch(state);
}

void app_motor_process(uint32_t period, handler_clutch_state_t clutch, app_motor_command_t clutchMotion, app_motor_clutchModeHandlers_t *callback)
{
    switch (app_motor_data_get()->clutchMode)
    {
        case APP_CLUTCH_MODE_HALTING_MOTOR:
        {
            app_motor_data_get()->clutchEngagementMotionState = APP_CLUTCH_ENGAGEMENT_NOK;

            if (NULL != callback->motorHalt)
            {
                callback->motorHalt(period);
            }

            app_unbalance_reset();
            app_motor_brake();

            if (gx_motor_haltState == APP_MOTOR_HALT_OK)
            {
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_SWITCHING;
            }
        }
        break;

        case APP_CLUTCH_MODE_SWITCHING:
        {
            app_motor_data_get()->clutchState = app_motor_clutch_switch(clutch);
            if (app_motor_data_get()->clutchState == clutch)
            {
                app_motor_data_get()->clutchEngagementMotionState = app_motor_clutchEngementMotion_process(period, clutchMotion);
            }

            if (NULL != callback->clutchSwitching)
            {
                callback->clutchSwitching(period);
            }

            if (app_motor_data_get()->clutchEngagementMotionState == APP_CLUTCH_ENGAGEMENT_OK)
            {
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_OPERATION;
            }
        }
        break;

        case APP_CLUTCH_MODE_OPERATION:
        {
            app_motor_data_get()->clutchEngagementMotionState = APP_CLUTCH_ENGAGEMENT_NOK;

            if (NULL != callback->operation)
            {
                callback->operation(period);
            }
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}

app_motor_clutchEngagementState_t app_motor_clutchEngementMotion_process(uint32_t period, app_motor_command_t command)
{
#ifdef TESTING
    return APP_CLUTCH_ENGAGEMENT_OK;
#else
#if defined(WM_DDM) && defined(CLUTCH_DECLUTCH_ENABLE)
    app_motor_clutchEngagementState_t ret = APP_CLUTCH_ENGAGEMENT_NOK;

    static uint32_t motionTimeout = 0;
    static uint32_t stopTimeout = 0;

    if (motionTimeout < MOTOR_CLUTCH_ENGAGEMENT_MOTION_TIMEOUT_MSEC)
    {
        motionTimeout += period;
        app_motor_motion_set(command, NULL);
    }
    else
    {
        app_motor_motion_set(APP_MOTOR_COMMAND_STOP, NULL);

        stopTimeout += period;
        if (stopTimeout >= MOTOR_CLUTCH_ENGAGEMENT_HALT_TIMEOUT_MSEC)
        {
            stopTimeout = 0;
            motionTimeout = 0;
            ret = APP_CLUTCH_ENGAGEMENT_OK;
        }
    }

    return ret;
#else
    return APP_CLUTCH_ENGAGEMENT_OK;
#endif
#endif
}

uint32_t app_motor_speed_get(void)
{
#ifdef TESTING
    return 0;
#else
    uint32_t motorSpeed;

    motorSpeed = handler_motor_speed_get();

    return motorSpeed;
#endif
}

void app_motor_agitationPattern_handle(const app_motor_motionPattern_t *pattern)
{
    static uint8_t errorCnt = 0;

    if (pattern != NULL)
    {
        if (!pattern->cwOnTimeMSec && !pattern->ccwOnTimeMSec)
        {
            app_motor_motion_set(APP_MOTOR_COMMAND_BRAKE, NULL);
        }
        else if ((!pattern->cwOnTimeMSec && pattern->ccwOnTimeMSec) || (pattern->cwOnTimeMSec && !pattern->ccwOnTimeMSec))
        {
            app_motor_motion_set(APP_MOTOR_COMMAND_SPIN_PULSATOR, pattern);
            if ((pattern->rpm > 0U) && handler_motor_speed_get() == 0U)
            {
                errorCnt++;
                if (errorCnt >= MOTOR_ROTATION_ERRO_COUNT)
                {
                    errorCnt = 0;
                    app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_MOTOR_CW_CCW_ROTATION);
                }
            }
            else
            {
                errorCnt = 0;
            }
        }
        else
        {
            app_motor_motion_set(APP_MOTOR_COMMAND_AGITATION, pattern);
            if ((pattern->rpm > 0U) && handler_motor_speed_get() == 0U)
            {
                errorCnt++;
                if (errorCnt >= MOTOR_ROTATION_ERRO_COUNT)
                {
                    errorCnt = 0;
                    app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_MOTOR_CW_CCW_ROTATION);
                }
            }
            else
            {
                errorCnt = 0;
            }
        }
    }
}

void app_motor_spinPattern_handle(const app_motor_spinProfileSectionData_t *section)
{
    app_motor_motionPattern_t pattern;
    static uint8_t errorCnt = 0;

    if (section != NULL)
    {
        pattern.cwOnTimeMSec = section->sectionOnTimeMSec;
        pattern.cwOffTimeMSec = section->sectionOffTimeMSec;
        pattern.ccwOnTimeMSec = section->sectionOnTimeMSec;
        pattern.ccwOffTimeMSec = section->sectionOffTimeMSec;
        pattern.accTimeMSec = section->sectionTimeMSec;
        pattern.rpm = section->sectionRPM;

        app_motor_motion_set(APP_MOTOR_COMMAND_SPIN, &pattern);

        if ((pattern.rpm > 0U) && handler_motor_speed_get() == 0U)
        {
            errorCnt++;
            if (errorCnt >= MOTOR_ROTATION_ERRO_COUNT)
            {
                errorCnt = 0;
                app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_MOTOR_SPIN_ROTATION);
            }
        }
        else
        {
            errorCnt = 0;
        }
    }
}

void app_motor_weightDetection_handle(void)
{
    app_motor_motion_set(APP_MOTOR_COMMAND_WEIGHT_DETECTION, NULL);
}

void app_motor_brake(void)
{
#if defined(MECH_BRAKE)
    app_motor_data_get()->clutchState = app_motor_clutch_switch(APP_MOTOR_CLUTCH_WITHOUT_BASKET);
#endif
    app_motor_motion_set(APP_MOTOR_COMMAND_BRAKE, NULL);
}

uint32_t app_motor_brakeWaitTimeoutMSec_get(void)
{
    return MOTOR_MIN_BRAKE_TIME_MSEC;
}

app_motor_haltState_t app_motor_halt_check(void)
{
#ifdef TESTING
    return gx_motor_haltState;
#else
    app_motor_haltState_t ret = APP_MOTOR_HALT_NOK;

    static uint32_t tmpStopCounter = 0;

    if (app_motor_speed_get() == 0U)
    {
        if (tmpStopCounter < MOTOR_HALT_ASSURANCE_COUNT)
        {
            tmpStopCounter++;
        }
        else
        {
            ret = APP_MOTOR_HALT_OK;
        }
    }
    else
    {
        tmpStopCounter = 0;
        ret = APP_MOTOR_HALT_NOK;
    }

    return ret;
#endif
}

void app_motor_motion_set(app_motor_command_t command, const app_motor_motionPattern_t *motion)
{
    handler_motor_command_set(command, motion);
}

void app_motor_reset(void)
{
    app_motor_brake();
    app_unbalance_reset();

    app_motor_data_get()->clutchState = APP_MOTOR_CLUTCH_PROCESSING;
    // app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    app_motor_data_get()->clutchEngagementMotionState = APP_CLUTCH_ENGAGEMENT_NOK;
    app_motor_data_get()->spinState = SPIN_RUN_STAGE;
}
