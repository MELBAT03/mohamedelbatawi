#include "def.h"

#include "app_nm_wash.h"
#include "app_motor.h"

#include "app_nm.h"

static app_nm_wash_soak_state_t gx_wash_soak_state = APP_NORMAL_MODE_WASH_SOAK_INIT;

static const app_nm_wash_data_t *data;

void app_nm_wash_soak_process(uint32_t period)
{
    data = &(app_nm_data_get()->wash);

    switch (gx_wash_soak_state)
    {
        case APP_NORMAL_MODE_WASH_SOAK_INIT:
        {
            gx_wash_soak_state = APP_NORMAL_MODE_WASH_SOAK_OPERATION;
        }
        break;

        case APP_NORMAL_MODE_WASH_SOAK_OPERATION:
        {
            if (0 == data->soakTimeMSec)
            {
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
                gx_wash_soak_state = APP_NORMAL_MODE_WASH_SOAK_FINISH;
            }
            else
            {
                if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
                {
                    app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.soakTimeMSec));
                }
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_SOAK_FINISH:
        {
            if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
            {
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
                gx_wash_soak_state = APP_NORMAL_MODE_WASH_SOAK_WASH_TRANSITION;
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_SOAK_WASH_TRANSITION:
        {
            gx_wash_soak_state = APP_NORMAL_MODE_WASH_SOAK_DONE;
        }
        break;

        case APP_NORMAL_MODE_WASH_SOAK_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

app_nm_wash_soak_state_t app_nm_wash_soak_state_get(void)
{
    return gx_wash_soak_state;
}

void app_nm_wash_soak_state_set(app_nm_wash_soak_state_t value)
{
    gx_wash_soak_state = value;
}

void app_nm_wash_soak_reset(void)
{
    gx_wash_soak_state = APP_NORMAL_MODE_WASH_SOAK_INIT;
}
