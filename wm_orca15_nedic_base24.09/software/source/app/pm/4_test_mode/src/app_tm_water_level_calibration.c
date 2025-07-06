#include "def.h"
#include "os.h"

#include "app_memory.h"
#include "app_test_mode.h"

#include "handlers/input/inc/handler_water_level.h"
#include "handlers/output/inc/handler_buzzer.h"

#define SAMPLES_COUNTS                         (10U)
#define WATER_LEVEL_SENSOR_STABLE_TIMEOUT_MSEC (10000U)

typedef enum app_tm_waterLevelCalibrationEnum
{
    TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_INIT,
    TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_CALIBRATION,
    TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_SAVING,
    TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_ERROR,
    TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_FINISH,
    TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_DONE,
} app_tm_waterLevelCalibrationEnum_t;

static app_tm_waterLevelCalibrationEnum_t gx_testMode_waterLevelCalibrationState = TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_INIT;

// on start, start operation, at the same time rotate to motor to indicate a work in progress.
// read the reset level value and use the value read as a ratio to the rcorded value then save this ratio in the eeprom, use it in the app water level module.

void app_tm_waterLevelCalibration_process(uint32_t period)
{
    uint32_t testMode_waterLevelFreq = 0;
    float gf_testMode_waterLevel_correctionFactor = 1.0f;
    static uint32_t gau32_testMode_waterLevel_valueBuffer[SAMPLES_COUNTS] = {0};
    float sum = 0.0f;
    float avg = 0.0f;
    uint32_t u32_testMode_waterLevel_correctionFactor = 0;
    static uint8_t u8_index = 0;
    static uint16_t u16_internalTimer = 0;

    switch (app_tm_state_get())
    {
        case APP_TEST_MODE_INIT:
        {
            gx_testMode_waterLevelCalibrationState = TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_INIT;
            app_tm_state_set(APP_TEST_MODE_OPERATION);
        }
        break;

        case APP_TEST_MODE_OPERATION:
        {
            switch (gx_testMode_waterLevelCalibrationState)
            {
                case TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_INIT:
                {
                    u16_internalTimer += period;
                    if (u16_internalTimer >= WATER_LEVEL_SENSOR_STABLE_TIMEOUT_MSEC)
                    {
                        u16_internalTimer = 0;
                        gx_testMode_waterLevelCalibrationState = TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_CALIBRATION;
                    }
                }
                break;

                case TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_CALIBRATION:
                {
                    testMode_waterLevelFreq = handler_waterLevel_currentFreq_get();

                    if (testMode_waterLevelFreq < (HANDLER_WATER_LEVEL_RESET_VALUE - HANDLER_WATER_LEVEL_ZERO_LEVEL_ERROR_ACCEPTED_DIFF))
                    {
                        gx_testMode_waterLevelCalibrationState = TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_ERROR;
                    }
                    else
                    {
                        gau32_testMode_waterLevel_valueBuffer[u8_index] = testMode_waterLevelFreq;
                        u8_index++;
                        if (u8_index >= SAMPLES_COUNTS)
                        {
                            u8_index = 0;
                            gx_testMode_waterLevelCalibrationState = TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_SAVING;
                        }
                    }
                }
                break;

                case TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_ERROR:
                {
                    handler_buzzer_tone_set(HANDLER_BUZZER_FAULT_TONE);
                }
                break;

                case TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_SAVING:
                {
                    for (u8_index = 0; u8_index < SAMPLES_COUNTS; u8_index++)
                    {
                        sum += (float)gau32_testMode_waterLevel_valueBuffer[u8_index];
                    }

                    u8_index = 0;
                    avg = sum / (float)SAMPLES_COUNTS;
                    gf_testMode_waterLevel_correctionFactor = avg;
                    u32_testMode_waterLevel_correctionFactor = (uint32_t)gf_testMode_waterLevel_correctionFactor;

                    app_memory_resetWaterLevel_set(u32_testMode_waterLevel_correctionFactor);

                    gx_testMode_waterLevelCalibrationState = TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_FINISH;
                }
                break;

                case TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_FINISH:
                {
                    gx_testMode_waterLevelCalibrationState = TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_DONE;
                }
                break;

                case TEST_MODE_WATER_LEVEL_CALIBRATION_STATE_DONE:
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

    LOG(0, 'i', "tmType:%d  tmState:%d  WLCalibrationState:%d  correctionFactor:%d  currentWLFreq:%d", app_tm_type_get(), app_tm_state_get(), gx_testMode_waterLevelCalibrationState, u32_testMode_waterLevel_correctionFactor, testMode_waterLevelFreq);
}
