#include "def.h"

#include "app_nm_wash.h"
#include "app_motor.h"
#include "app_nm.h"
#include "app_fill.h"
#include "app_drain.h"
#include "app_unbalance.h"

#include "app_nm_wash_rinse.h"
#include "app_nm_wash_motor.h"

static void init_handle(uint32_t period);
static void drain_handle(uint32_t period);
static void spin_handle(uint32_t period);
static void spinBrake_handle(uint32_t period);
static void fill_handle(uint32_t period);
static void stir_handle(uint32_t period);
static void untangle_handle(uint32_t period);
static void unbalance_handle(uint32_t period);
static void regroup_handle(uint32_t period);
static void finish_handle(uint32_t period);
static void spinTransition_handle(uint32_t period);

static void unbalance_process(uint32_t period);
static void unbalance_transition(void);

static uint8_t rinse_cycleEnd_check(void);

static app_nm_wash_rinse_state_t gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_INIT;
static app_nm_wash_rinse_unbalance_state_t gx_wash_rinse_unbalance_state = APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_INIT;

static const app_nm_wash_data_t *data;

void app_nm_wash_rinse_process(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        init_handle,
        drain_handle,
        spin_handle,
        spinBrake_handle,
        fill_handle,
        stir_handle,
        untangle_handle,
        unbalance_handle,
        regroup_handle,
        finish_handle,
        spinTransition_handle,
        NULL};

    data = &(app_nm_data_get()->wash);

    if (handlers[gx_wash_rinse_state] != NULL)
    {
        handlers[gx_wash_rinse_state](period);
    }
}

app_nm_wash_rinse_state_t app_nm_wash_rinse_state_get(void)
{
    return gx_wash_rinse_state;
}

void app_nm_wash_rinse_state_set(app_nm_wash_rinse_state_t value)
{
    gx_wash_rinse_state = value;
}

app_nm_wash_rinse_unbalance_state_t app_nm_wash_rinse_unbalanceState_get(void)
{
    return gx_wash_rinse_unbalance_state;
}

void app_nm_wash_rinse_reset(void)
{
    gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_INIT;
    gx_wash_rinse_unbalance_state = APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_INIT;
}

static void init_handle(uint32_t period)
{
    app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_DRAIN;
}
static void drain_handle(uint32_t period)
{
    if (0U != rinse_cycleEnd_check())
    {
        app_nm_data_set()->wash.rinseNum = 0;

        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_FINISH;
    }
    else if (0U == data->rinseDrainTimeMSec[data->rinseNum - 1])
    {
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_SPIN;
        app_unbalance_reset();
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            if (APP_DRAIN_TUB_EMPTY == app_drain_tubState_get())
            {
                app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.rinseDrainTimeMSec[data->rinseNum - 1]));
            }
        }
    }
}
static void spin_handle(uint32_t period)
{
    static uint8_t rinseUnbalanceCnt = 0;
    const app_nm_wash_rinse_stateTime_t *rinseData;

    if (0U != rinse_cycleEnd_check())
    {
        app_nm_data_set()->wash.rinseNum = 0;

        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_FINISH;
    }
    else if (0U == data->rinseSpinTimeMSec[data->rinseNum - 1])
    {
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_SPIN_BRAKE;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.rinseSpinTimeMSec[data->rinseNum - 1]));

            if (APP_UNBALANCE_DETECTED == app_unbalance_state_get())
            {
                if (0U == rinseUnbalanceCnt)
                {
                    rinseUnbalanceCnt++;
                    gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_UNBALANCE;
                    app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;

                    rinseData = app_nm_wash_rinse_stateTime_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, app_userInterface_data_get()->normalMode.wash.extraRinseSelection);

                    if (NULL != rinseData)
                    {
                        app_nm_data_set()->wash.rinseSpinTimeMSec[data->rinseNum - 1] = rinseData->spinTimeMSec + app_nm_wash_rinse_preSpinTimeMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

                        app_nm_data_set()->wash.rinseUnbalanceFillTimeMSec = rinseData->fillTimeMSec;
                        app_nm_data_set()->wash.rinseUnbalanceStirTimeMSec = rinseData->stirTimeMSec;
                        app_nm_data_set()->wash.rinseUnbalanceDrainTimeMSec = rinseData->drainTimeMSec;
                    }
                }
                else
                {
                    app_nm_data_set()->wash.rinseSpinTimeMSec[data->rinseNum - 1] = 0;
                    rinseUnbalanceCnt = 0;
                }
            }
        }
    }
#if 0
// TODO: required to happen in the first spin only and for courses that the max speed is 700 rpm
#if defined(WM_AC)
    else if ((app_washerRinse_state_get() == WASHER_RINSE_SPIN) && (app_washerRinse_spinProfileSectionNum_get() == SPIN_PROFILE_SECTION_6) && (app_userSelection_data_get()->courseSelection != COURSE_SENSITIVE) && (app_userSelection_data_get()->courseSelection != COURSE_WOOL) && (app_userSelection_data_get()->courseSelection != COURSE_TUBCLEAN))
    {
        foamDetectionDelayMSec += period;
        if (foamDetectionDelayMSec >= WASHER_FOAM_DETECTION_DELAY_MSEC)
        {
            foamDetectionDelayMSec = 0;
            if (motorSpeed < WASHER_FOAM_DETECTION_REQUIRED_MOTOR_RPM)
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_UNBALANCE);
                gx_app_washer_unbalanceBufferedExecState = gx_app_washer_executionState;
                gx_app_washer_executionState = WASHER_EXEC_UNBALANCE;
                app_washerRinse_clutchState_set(CLUTCH_WAITING_MOTOR_STOP);
                app_washerUnbalance_state_set(WASHER_UNBALANCE_INIT);
                app_washerUnbalance_clutchState_set(CLUTCH_WAITING_MOTOR_STOP);
            }
        }
    }
    else
    {
        foamDetectionDelayMSec = 0;
    }
#endif
#endif
}
static void spinBrake_handle(uint32_t period)
{
    if (0 != rinse_cycleEnd_check())
    {
        app_nm_data_set()->wash.rinseNum = 0;

        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_FINISH;
    }
    else if (0 == data->rinseBrakeTimeMSec[data->rinseNum - 1])
    {
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_FILL;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.rinseBrakeTimeMSec[data->rinseNum - 1]));
        }
    }
}
static void fill_handle(uint32_t period)
{
    if (0 != rinse_cycleEnd_check())
    {
        app_nm_data_set()->wash.rinseNum = 0;

        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_FINISH;
    }
    else if (APP_FILL_WATER_LEVEL_OK == app_fill_waterLevelState_get(data->rinseFillWaterLevelLiter, RINSE_FILL_MAXIMUM_ALLOWED_VARIANCE))
    {
        app_nm_data_set()->wash.rinseFillTimeMSec[data->rinseNum - 1] = 0;
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_STIR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.rinseFillTimeMSec[data->rinseNum - 1]));
        }
    }
}
static void stir_handle(uint32_t period)
{
    if (0U != rinse_cycleEnd_check())
    {
        app_nm_data_set()->wash.rinseNum = 0;

        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_FINISH;
    }
    else if (0U == data->rinseStirTimeMSec[data->rinseNum - 1])
    {
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_UNTANGLE;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.rinseStirTimeMSec[data->rinseNum - 1]));
        }
    }
}
static void untangle_handle(uint32_t period)
{
    if (0 != rinse_cycleEnd_check())
    {
        app_nm_data_set()->wash.rinseNum = 0;

        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_FINISH;
    }
    else if (0 == data->rinseUntangleTimeMSec[data->rinseNum - 1])
    {
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_REGROUP;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.rinseUntangleTimeMSec[data->rinseNum - 1]));
        }
    }
}
static void unbalance_handle(uint32_t period)
{
    if (0 != rinse_cycleEnd_check())
    {
        app_nm_data_set()->wash.rinseNum = 0U;

        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_FINISH;
    }
    else
    {
        unbalance_process(period);
        unbalance_transition();
    }
}
static void regroup_handle(uint32_t period)
{
    app_nm_data_set()->wash.rinseNum = app_userInterface_data_get()->normalMode.wash.rinseSelection;
    if (0U == data->rinseNum)
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_FINISH;
    }
    else
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_DRAIN;
    }
}
static void finish_handle(uint32_t period)
{
    if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_SPIN_TRANSITION;
    }
}
static void spinTransition_handle(uint32_t period)
{
    gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_DONE;
}

static void unbalance_process(uint32_t period)
{
    switch (gx_wash_rinse_unbalance_state)
    {
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_INIT:
        {
            gx_wash_rinse_unbalance_state = APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_SETUP;
            app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_SETUP:
        {
            if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
            {
                gx_wash_rinse_unbalance_state = APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_FILL;
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_FILL:
        {
            if (APP_FILL_WATER_LEVEL_OK == app_fill_waterLevelState_get(data->rinseFillWaterLevelLiter, RINSE_FILL_MAXIMUM_ALLOWED_VARIANCE))
            {
                app_nm_data_set()->wash.rinseUnbalanceFillTimeMSec = 0;
                gx_wash_rinse_unbalance_state = APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_STIR;
            }
            else
            {
                if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
                {
                    app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.rinseUnbalanceFillTimeMSec));
                }
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_STIR:
        {
            if (0 == data->rinseUnbalanceStirTimeMSec)
            {
                gx_wash_rinse_unbalance_state = APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_DRAIN;
            }
            else
            {
                if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
                {
                    app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.rinseUnbalanceStirTimeMSec));
                }
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_DRAIN:
        {
            if (0 == data->rinseUnbalanceDrainTimeMSec)
            {
                gx_wash_rinse_unbalance_state = APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_FINISH;
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
            }
            else
            {
                if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
                {
                    if (APP_DRAIN_TUB_EMPTY == app_drain_tubState_get())
                    {
                        app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.rinseUnbalanceDrainTimeMSec));
                    }
                }
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_FINISH:
        {
            if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
            {
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
                gx_wash_rinse_unbalance_state = APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_DONE;
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

static void unbalance_transition(void)
{
    if (APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_DONE == gx_wash_rinse_unbalance_state)
    {
        gx_wash_rinse_state = APP_NORMAL_MODE_WASH_RINSE_SPIN;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
}

static uint8_t rinse_cycleEnd_check(void)
{
    uint8_t ret = 0;
    if (APP_NORMAL_MODE_UI_RINSE_0_TIMES == app_userInterface_data_get()->normalMode.wash.rinseSelection)
    {
        ret = 1;
    }

    return ret;
}
