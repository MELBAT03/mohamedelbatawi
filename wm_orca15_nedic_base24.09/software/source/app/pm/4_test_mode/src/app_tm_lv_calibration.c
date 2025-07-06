#include "def.h"

#include "os.h"

#include "app_memory.h"
#include "app_test_mode.h"

#include "handlers/input/inc/handler_acin.h"
#include "handlers/output/inc/handler_buzzer.h"

typedef enum
{
    TEST_MODE_LVOLTAGE_CALIBRATION_STATE_INIT,
    TEST_MODE_LVOLTAGE_CALIBRATION_STATE_CALIBRATION,
    TEST_MODE_LVOLTAGE_CALIBRATION_STATE_SAVING,
    TEST_MODE_LVOLTAGE_CALIBRATION_STATE_FINISH,
    TEST_MODE_LVOLTAGE_CALIBRATION_STATE_DONE
} testMode_lVoltageCalibrationEnum_t;

static testMode_lVoltageCalibrationEnum_t gx_testMode_lVoltageCalibrationState = TEST_MODE_LVOLTAGE_CALIBRATION_STATE_INIT;

void app_tm_lvCalibrationTest_process(uint32_t period)
{
    static uint16_t u16_internalTimer = 0;
    static uint32_t calibratedValue = 0;

    switch (app_tm_state_get())
    {
        case APP_TEST_MODE_INIT:
        {
            gx_testMode_lVoltageCalibrationState = TEST_MODE_LVOLTAGE_CALIBRATION_STATE_INIT;
            app_tm_state_set(APP_TEST_MODE_OPERATION);
        }
        break;

        case APP_TEST_MODE_OPERATION:
        {
            switch (gx_testMode_lVoltageCalibrationState)
            {
                case TEST_MODE_LVOLTAGE_CALIBRATION_STATE_INIT:
                {
                    if (u16_internalTimer < 2000)
                    {
                        u16_internalTimer += period;
                        return;
                    }

                    gx_testMode_lVoltageCalibrationState = TEST_MODE_LVOLTAGE_CALIBRATION_STATE_CALIBRATION;
                    u16_internalTimer = 0;
                }
                break;

                case TEST_MODE_LVOLTAGE_CALIBRATION_STATE_CALIBRATION:
                {
                    calibratedValue = handler_acin_value_get() - 20;
                    gx_testMode_lVoltageCalibrationState = TEST_MODE_LVOLTAGE_CALIBRATION_STATE_SAVING;
                }
                break;

                case TEST_MODE_LVOLTAGE_CALIBRATION_STATE_SAVING:
                {
                    app_memory_lVoltage_set(calibratedValue);
                    gx_testMode_lVoltageCalibrationState = TEST_MODE_LVOLTAGE_CALIBRATION_STATE_FINISH;
                }
                break;

                case TEST_MODE_LVOLTAGE_CALIBRATION_STATE_FINISH:
                {
                    u16_internalTimer += period;
                    while (u16_internalTimer < 1000)
                    {
                        return;
                    }
                    gx_testMode_lVoltageCalibrationState = TEST_MODE_LVOLTAGE_CALIBRATION_STATE_DONE;
                }
                break;

                case TEST_MODE_LVOLTAGE_CALIBRATION_STATE_DONE:
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
        {
            // do nothing
        }
        break;
    }

    LOG(0, 'i', "tmType:%d    tmState:%d    WeightCalibrationState:%d    calibratedValue:%d", app_tm_type_get(), app_tm_state_get(), gx_testMode_lVoltageCalibrationState, calibratedValue);
}
