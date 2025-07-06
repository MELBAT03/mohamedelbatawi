#include "def.h"

#include "app_test_mode.h"

#include "app_motor.h"

static void tm_weightDetection_handle(uint32_t period);

void app_tm_motor_process(uint32_t period)
{
    static app_motor_clutchModeHandlers_t gx_motor_callback;

    switch (app_tm_type_get())
    {
        case APP_TM_WATER_LEVEL_CALIBRATION:
        {
            app_motor_brake();
        }
        break;

        case APP_TM_WEIGHT_CALIBRATION:
        {
            switch (app_tm_weightCalibrationState_get())
            {
                case TM_WEIGHTDETECTION_CALIBRATION_STATE_CALIBRATION:
                {
                    gx_motor_callback.motorHalt = NULL;
                    gx_motor_callback.clutchSwitching = NULL;
                    gx_motor_callback.operation = tm_weightDetection_handle;
                    app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
                }
                break;

                case TM_WEIGHTDETECTION_CALIBRATION_STATE_INIT:
                case TM_WEIGHTDETECTION_CALIBRATION_STATE_SAVING:
                case TM_WEIGHTDETECTION_CALIBRATION_STATE_ERROR:
                case TM_WEIGHTDETECTION_CALIBRATION_STATE_FINISH:
                case TM_WEIGHTDETECTION_CALIBRATION_STATE_DONE:
                default:
                {
                    // do nothing
                }
                break;
            }
        }
        break;

        case APP_TM_ELECTRICAL_PARTS:
        {
        }
        break;

        case APP_TM_UNBALANCE_SPIN:
        {
        }
        break;

        case APP_TM_WATER_LEVEL_FREQ:
        {
        }
        break;

        case APP_TM_PCB:
        {
        }
        break;

        case APP_TM_MIDDLE_INSPECTION:
        {
        }
        break;

        case APP_TM_FINAL_INSPECTION:
        {
        }
        break;

        case APP_TM_ERROR_COUNT_DISPLAY:
        {
        }
        break;

        case APP_TM_CYCLE_COUNT_DISPLAY:
        {
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}

static void tm_weightDetection_handle(uint32_t period)
{
    app_motor_weightDetection_handle();
}
