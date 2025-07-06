#include "def.h"
#include "app_nm_init.h"
#include "app_motor.h"

#include "app_nm_init_motor.h"

#define MOTOR_INIT_MOTOR_STUCK_SYNC_STATE_TIMEOUT_MSEC (60000UL)

void app_nm_init_motor_process(uint32_t period)
{
    static uint32_t gu32_main_motor_timePassCnt = 0;

    switch (app_nm_init_state_get())
    {
        case APP_NORMAL_MODE_INIT_INIT:
        {
            if (APP_MOTOR_OPERATION_STATE_OPERATION != app_motor_operationState_get())
            {
                gu32_main_motor_timePassCnt += period;
                if (gu32_main_motor_timePassCnt >= MOTOR_INIT_MOTOR_STUCK_SYNC_STATE_TIMEOUT_MSEC)
                {
                    app_motor_resetState_set(APP_MOTOR_RESET_FAILURE);
                }
                else
                {
                    app_motor_resetState_set(APP_MOTOR_RESET_NOK);
                }
            }
            else
            {
                app_motor_resetState_set(APP_MOTOR_RESET_OK);
            }
        }
        break;

        case APP_NORMAL_MODE_INIT_MOTOR_CLUTCH_RESET:
        {
            app_motor_brake();

            if (APP_MOTOR_HALT_OK == app_motor_haltState_get())
            {
                app_motor_data_get()->clutchState = app_motor_clutch_switch(APP_MOTOR_CLUTCH_WITHOUT_BASKET);
                if (APP_MOTOR_CLUTCH_WITHOUT_BASKET == app_motor_data_get()->clutchState)
                {
                    if (APP_CLUTCH_ENGAGEMENT_OK == app_motor_clutchEngementMotion_process(period, APP_MOTOR_COMMAND_DECLUTCH))
                    {
                        app_motor_clutchResetState_set(APP_CLUTCH_RESET_OK);
                    }
                    else
                    {
                        app_motor_clutchResetState_set(APP_CLUTCH_RESET_NOK);
                    }
                }
            }
        }
        break;

        case APP_NORMAL_MODE_INIT_DELAY_START:
        case APP_NORMAL_MODE_INIT_FINISH:
        {
            app_motor_brake();
            if (APP_MOTOR_HALT_OK == app_motor_haltState_get())
            {
                app_motor_data_get()->clutchState = app_motor_clutch_switch(APP_MOTOR_CLUTCH_WITHOUT_BASKET);
            }
        }
        break;

        case APP_NORMAL_MODE_INIT_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}
