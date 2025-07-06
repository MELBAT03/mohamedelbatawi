#include "def.h"

#include "app_nm_wash.h"
#include "app_motor.h"
#include "app_nm.h"
#include "app_fill.h"
#include "app_drain.h"
#include "app_unbalance.h"

#include "app_nm_wash_spin.h"
#include "app_nm_wash_motor.h"

#define SPIN_FILL_MINIMUM_ALLOWED_VARIANCE (0)
#define SPIN_FILL_MAXIMUM_ALLOWED_VARIANCE (200U)

static void init_handle(uint32_t period);
static void drain_handle(uint32_t period);
static void spin_handle(uint32_t period);
static void superSpin_handle(uint32_t period);
static void spinBrake_handle(uint32_t period);
static void antiWrinkle_handle(uint32_t period);
static void unbalance_handle(uint32_t period);
static void finish_handle(uint32_t period);
static void endTransition_handle(uint32_t period);

static void unbalance_process(uint32_t period);
static void unbalance_transition(void);

static uint8_t spin_cycleEnd_check(void);

static app_nm_wash_spin_state_t gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_INIT;
static app_nm_wash_spin_state_t gx_wash_spin_bufferedState = APP_NORMAL_MODE_WASH_SPIN_INIT;
static app_nm_wash_spin_unbalance_state_t gx_wash_spin_unbalance_state = APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_INIT;

static const app_nm_wash_data_t *data;

void app_nm_wash_spin_process(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        init_handle,
        drain_handle,
        spin_handle,
        superSpin_handle,
        spinBrake_handle,
        antiWrinkle_handle,
        unbalance_handle,
        finish_handle,
        endTransition_handle,
        NULL};

    data = &(app_nm_data_get()->wash);

    if (handlers[gx_wash_spin_state] != NULL)
    {
        handlers[gx_wash_spin_state](period);
    }
}

app_nm_wash_spin_state_t app_nm_wash_spin_state_get(void)
{
    return gx_wash_spin_state;
}

void app_nm_wash_spin_state_set(app_nm_wash_spin_state_t value)
{
    gx_wash_spin_state = value;
}

app_nm_wash_spin_unbalance_state_t app_nm_wash_spin_unbalanceState_get(void)
{
    return gx_wash_spin_unbalance_state;
}

void app_nm_wash_spin_reset(void)
{
    gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_INIT;
    gx_wash_spin_bufferedState = APP_NORMAL_MODE_WASH_SPIN_INIT;
    gx_wash_spin_unbalance_state = APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_INIT;
}

static void init_handle(uint32_t period)
{
    app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_DRAIN;
}
static void drain_handle(uint32_t period)
{
    if (0U != spin_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_FINISH;
    }
    else if (0U == data->spinStateTimeMSec[gx_wash_spin_state])
    {
        gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_SPIN;
        app_unbalance_reset();
    }
    else
    {
        if ((APP_CLUTCH_MODE_OPERATION != app_motor_data_get()->clutchMode) && (APP_DRAIN_TUB_EMPTY == app_drain_tubState_get()))
        {
            app_nm_data_set()->wash.spinStateTimeMSec[gx_wash_spin_state] = APP_DRAIN_SHORT_WATER_ESTIMATED_INTERVAL;
        }
        else if ((APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode) && (APP_DRAIN_TUB_EMPTY == app_drain_tubState_get()))
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.spinStateTimeMSec[gx_wash_spin_state]));
        }
        else
        {
            // do nothing
        }
    }
}
static void spin_handle(uint32_t period)
{
    static uint8_t spinUnbalanceCnt = 0;
    const app_nm_wash_spin_unbalance_stateTime_t *unbalanceData;

    if (0U != spin_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_FINISH;
    }
    else if (0U == data->spinStateTimeMSec[gx_wash_spin_state])
    {
        gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_SUPER_SPIN;
        app_unbalance_reset();
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.spinStateTimeMSec[gx_wash_spin_state]));

            if (APP_UNBALANCE_DETECTED == app_unbalance_state_get())
            {
                spinUnbalanceCnt++;
                if (3U > spinUnbalanceCnt)
                {
                    gx_wash_spin_bufferedState = gx_wash_spin_state;
                    gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_UNBALANCE;
                    app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;

                    unbalanceData = app_nm_wash_spin_unabalanceStateTime_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

                    if (NULL != unbalanceData)
                    {
                        app_nm_data_set()->wash.spinStateTimeMSec[APP_NORMAL_MODE_WASH_SPIN_SPIN] = (60U * 1000U * (uint32_t)app_userInterface_data_get()->normalMode.wash.spinSelection) + app_nm_wash_spin_preSpinTimeMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

                        app_nm_data_set()->wash.spinUnbalanceFillTimeMSec = unbalanceData->fillTimeMSec;
                        app_nm_data_set()->wash.spinUnbalanceStirTimeMSec = unbalanceData->stirTimeMSec;
                        app_nm_data_set()->wash.spinUnbalanceDrainTimeMSec = unbalanceData->drainTimeMSec;
                        app_nm_data_set()->wash.spinUnbalanceSuperSpinStirTimeMSec = 0;
                    }
                }
                else
                {
                    gx_wash_spin_unbalance_state = APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_ERROR;
                }
            }
        }
    }
}
static void superSpin_handle(uint32_t period)
{
    static uint8_t spinUnbalanceCnt = 0;
    const app_nm_wash_spin_unbalance_stateTime_t *unbalanceData;

    if (0U != spin_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_FINISH;
    }
    else if (0U == data->spinStateTimeMSec[gx_wash_spin_state])
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_BRAKE;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.spinStateTimeMSec[gx_wash_spin_state]));

            if (APP_UNBALANCE_DETECTED == app_unbalance_state_get())
            {
                spinUnbalanceCnt++;
                if (1U > spinUnbalanceCnt)
                {
                    gx_wash_spin_bufferedState = gx_wash_spin_state;
                    gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_UNBALANCE;
                    app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;

                    unbalanceData = app_nm_wash_spin_unabalanceStateTime_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

                    if (NULL != unbalanceData)
                    {
                        app_nm_data_set()->wash.spinUnbalanceFillTimeMSec = 0;
                        app_nm_data_set()->wash.spinUnbalanceStirTimeMSec = 0;
                        app_nm_data_set()->wash.spinUnbalanceDrainTimeMSec = 0;
                        app_nm_data_set()->wash.spinUnbalanceSuperSpinStirTimeMSec = unbalanceData->superSpinStirTimeMSec;
                    }
                }
                else
                {
                    gx_wash_spin_unbalance_state = APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_ERROR;
                }
            }
        }
    }
}
static void spinBrake_handle(uint32_t period)
{
    if (0U != spin_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_FINISH;
    }
    else if (0U == data->spinStateTimeMSec[gx_wash_spin_state])
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_ANTI_WRINKLE;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.spinStateTimeMSec[gx_wash_spin_state]));
        }
    }
}
static void antiWrinkle_handle(uint32_t period)
{
    if ((0U != spin_cycleEnd_check()) || ((0U == data->spinStateTimeMSec[gx_wash_spin_state])))
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_FINISH;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.spinStateTimeMSec[gx_wash_spin_state]));
        }
    }
}
static void unbalance_handle(uint32_t period)
{
    if (0U != spin_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_FINISH;
    }
    else
    {
        unbalance_process(period);
        unbalance_transition();
    }
}
static void finish_handle(uint32_t period)
{
    if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_END_TRANSITION;
    }
}
static void endTransition_handle(uint32_t period)
{
    gx_wash_spin_state = APP_NORMAL_MODE_WASH_SPIN_DONE;
}

static void unbalance_process(uint32_t period)
{
    switch (gx_wash_spin_unbalance_state)
    {
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_INIT:
        {
            gx_wash_spin_unbalance_state = APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_SETUP;
            app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_SETUP:
        {
            if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
            {
                if (APP_NORMAL_MODE_WASH_SPIN_SPIN == gx_wash_spin_bufferedState)
                {
                    gx_wash_spin_unbalance_state = APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_FILL;
                }
                else if (APP_NORMAL_MODE_WASH_SPIN_SUPER_SPIN == gx_wash_spin_bufferedState)
                {
                    gx_wash_spin_unbalance_state = APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_SUPER_SPIN_STIR;
                }
                else
                {
                    // do nothing
                }
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_FILL:
        {
            if (APP_FILL_WATER_LEVEL_OK == app_fill_waterLevelState_get(app_nm_wash_spin_unbalanceStateWaterLevel_get(app_userInterface_data_get()->normalMode.wash.waterLevelSelection), SPIN_FILL_MAXIMUM_ALLOWED_VARIANCE))
            {
                app_nm_data_set()->wash.spinUnbalanceFillTimeMSec = 0;
                gx_wash_spin_unbalance_state = APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_STIR;
            }
            else
            {
                if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
                {
                    app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.spinUnbalanceFillTimeMSec));
                }
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_STIR:
        {
            if (0U == data->spinUnbalanceStirTimeMSec)
            {
                gx_wash_spin_unbalance_state = APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_DRAIN;
            }
            else
            {
                if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
                {
                    app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.spinUnbalanceStirTimeMSec));
                }
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_DRAIN:
        {
            if (0U == data->spinUnbalanceDrainTimeMSec)
            {
                gx_wash_spin_unbalance_state = APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_FINISH;
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
            }
            else
            {
                if ((APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode) && (APP_DRAIN_TUB_EMPTY == app_drain_tubState_get()))
                {
                    app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.spinUnbalanceDrainTimeMSec));
                }
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_SUPER_SPIN_STIR:
        {
            if (0U == data->spinUnbalanceSuperSpinStirTimeMSec)
            {
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
                gx_wash_spin_unbalance_state = APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_FINISH;
            }
            else
            {
                if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
                {
                    app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.spinUnbalanceSuperSpinStirTimeMSec));
                }
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_FINISH:
        {
            if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
            {
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
                gx_wash_spin_unbalance_state = APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_DONE;
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

static void unbalance_transition(void)
{
    if (APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_DONE == gx_wash_spin_unbalance_state)
    {
        gx_wash_spin_state = gx_wash_spin_bufferedState;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
}

static uint8_t spin_cycleEnd_check(void)
{
    uint8_t ret = 0;

    if ((APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.spinSelection) && (APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.superSpinSelection) && (APP_NORMAL_MODE_UI_ANTI_WRINKLE_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.antiWrinkleSelection))
    {
        ret = 1;
    }

    return ret;
}
