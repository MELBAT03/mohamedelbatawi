#include "def.h"
#include "app_nm_dry.h"
#include "app_motor.h"
#include "app_load.h"

static app_nm_dry_setup_state_t gx_app_dry_setup_state = APP_NORMAL_MODE_DRY_SETUP_INIT;

static uint32_t weightData;

void app_nm_dry_setup_process(uint32_t period)
{
    if (APP_NORMAL_MODE_DRY_SETUP == app_nm_dry_state_get())
    {
        switch (gx_app_dry_setup_state)
        {
            case APP_NORMAL_MODE_DRY_SETUP_INIT:
            {
                weightData = 0;
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
                if ((APP_MOTOR_HALT_OK == app_motor_haltState_get()) && (APP_CLUTCH_RESET_OK == app_motor_clutchResetState_get()))
                {
                    gx_app_dry_setup_state = APP_NORMAL_MODE_DRY_SETUP_WEIGHT_SENSE;
                }
            }
            break;

            case APP_NORMAL_MODE_DRY_SETUP_WEIGHT_SENSE:
            {
                // TODO: -WEIGHT_DETECTION- fix after weight modifications
                // weightData = app_load_weightG_get();
                weightData = 6000;

                if (0 != weightData)
                {
                    weightData = weightData / 1000;   // get value in kg
                    weightData = (weightData > APP_NORMAL_MODE_DRY_MAX_WATER_LEVEL) ? (APP_NORMAL_MODE_DRY_MAX_WATER_LEVEL) : (weightData);

                    gx_app_dry_setup_state = APP_NORMAL_MODE_DRY_SETUP_FINISH;
                }
            }
            break;

            case APP_NORMAL_MODE_DRY_SETUP_FINISH:
            {
                gx_app_dry_setup_state = APP_NORMAL_MODE_DRY_SETUP_DONE;
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
            }
            break;

            case APP_NORMAL_MODE_DRY_SETUP_DONE:
            {
                // do nothing
            }
            break;

            default:
                // do nothing
                break;
        }
    }
}

app_nm_dry_setup_state_t app_nm_dry_setup_state_get(void)
{
    return gx_app_dry_setup_state;
}

void app_nm_dry_setup_state_set(app_nm_dry_setup_state_t value)
{
    gx_app_dry_setup_state = value;
}

uint32_t app_nm_dry_setup_weightData_get(void)
{
    return weightData;
}

void app_nm_dry_setup_reset(void)
{
    gx_app_dry_setup_state = APP_NORMAL_MODE_DRY_SETUP_INIT;
}
