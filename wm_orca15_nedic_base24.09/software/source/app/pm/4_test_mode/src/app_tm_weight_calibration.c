#include "def.h"

#include "os.h"

#include "app_memory.h"
#include "app_test_mode.h"
#include "app_motor.h"
#include "app_load.h"

#include "handlers/input/inc/handler_water_level.h"
#include "handlers/output/inc/handler_buzzer.h"
#include "handlers/output/inc/handler_motor.h"

#define WEIGHT_DETECTION_STABLE_TIMEOUT_MSEC                       (5000U)
#define WEIGHT_DETECTION_WAITING_FOR_WEIGHT_DETECTION_TIMEOUT_MSEC (10000U)
#define WEIGHTDETECTION_CALIBRATION_TIMEOUT_MSEC                   (60000U)

static app_tm_weightCalibrationEnum_t gx_testMode_weightCalibrationState = TM_WEIGHTDETECTION_CALIBRATION_STATE_INIT;

void app_tm_weightDetectionCalibration_process(uint32_t period)
{
    static uint32_t gu32_app_weightCalibration_weightG = 0;
    static uint32_t gu32_app_weightCalibration_weightRaw = 0;
    static uint32_t u32_internalTimer = 0;
    static uint16_t rawTubValue = 0;

    switch (app_tm_state_get())
    {
        case APP_TEST_MODE_INIT:
        {
            gx_testMode_weightCalibrationState = TM_WEIGHTDETECTION_CALIBRATION_STATE_INIT;
            app_tm_state_set(APP_TEST_MODE_OPERATION);
        }
        break;

        case APP_TEST_MODE_OPERATION:
        {
            switch (gx_testMode_weightCalibrationState)
            {
                case TM_WEIGHTDETECTION_CALIBRATION_STATE_INIT:
                {
                    u32_internalTimer += period;
                    if ((u32_internalTimer >= WEIGHT_DETECTION_STABLE_TIMEOUT_MSEC) && (app_motor_operationState_get() == APP_MOTOR_OPERATION_STATE_OPERATION))
                    {
                        u32_internalTimer = 0;
                        gx_testMode_weightCalibrationState = TM_WEIGHTDETECTION_CALIBRATION_STATE_CALIBRATION;
                    }
                }
                break;

                case TM_WEIGHTDETECTION_CALIBRATION_STATE_CALIBRATION:
                {
                    u32_internalTimer += period;
                    if (u32_internalTimer >= WEIGHT_DETECTION_WAITING_FOR_WEIGHT_DETECTION_TIMEOUT_MSEC)
                    {
                        u32_internalTimer = 0;
                        gx_testMode_weightCalibrationState = TM_WEIGHTDETECTION_CALIBRATION_STATE_SAVING;
                    }
                }
                break;

                case TM_WEIGHTDETECTION_CALIBRATION_STATE_SAVING:
                {
                    u32_internalTimer += period;
                    gu32_app_weightCalibration_weightG = app_load_weightG_get();
                    gu32_app_weightCalibration_weightRaw = app_load_weightRaw_get();

                    if (0U != gu32_app_weightCalibration_weightG)
                    {
                        u32_internalTimer = 0;
                        rawTubValue = (uint16_t)gu32_app_weightCalibration_weightRaw;

                        app_memory_resetWeight_set(rawTubValue);

                        gx_testMode_weightCalibrationState = TM_WEIGHTDETECTION_CALIBRATION_STATE_FINISH;
                    }
                    else if (u32_internalTimer >= WEIGHTDETECTION_CALIBRATION_TIMEOUT_MSEC)
                    {
                        u32_internalTimer = 0;
                        gx_testMode_weightCalibrationState = TM_WEIGHTDETECTION_CALIBRATION_STATE_ERROR;
                    }
                    else
                    {
                        // do nothing
                    }
                }
                break;

                case TM_WEIGHTDETECTION_CALIBRATION_STATE_ERROR:
                {
                    handler_buzzer_tone_set(HANDLER_BUZZER_FAULT_TONE);
                }
                break;

                case TM_WEIGHTDETECTION_CALIBRATION_STATE_FINISH:
                {
                    gx_testMode_weightCalibrationState = TM_WEIGHTDETECTION_CALIBRATION_STATE_DONE;
                }
                break;

                case TM_WEIGHTDETECTION_CALIBRATION_STATE_DONE:
                {
                    app_tm_state_set(APP_TEST_MODE_DONE);
                }
                break;

                default:
                {
                    // do nothing
                }
                break;
            }
        }
        break;

        case APP_TEST_MODE_DONE:
        default:
        {
            // do nothing
        }
        break;
    }

    LOG(0, 'i', "tmType:%d  tmState:%d  WeightCalibrationState:%d  calibratedG:%d  calibratedRAW:%d   clutch[Mode:%d  State:%d]", app_tm_type_get(), app_tm_state_get(), gx_testMode_weightCalibrationState, gu32_app_weightCalibration_weightG, gu32_app_weightCalibration_weightRaw, app_motor_data_get()->clutchMode, app_motor_data_get()->clutchState);
}

app_tm_weightCalibrationEnum_t app_tm_weightCalibrationState_get(void)
{
    return gx_testMode_weightCalibrationState;
}
