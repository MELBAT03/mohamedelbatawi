#include "def.h"
#include "os.h"

#include "app_memory.h"
#include "app_test_mode.h"
#include "app_buzzer.h"
#include "app_lid.h"
#include "app_unbalance.h"
#include "app_motor.h"

#include "app_tm_user_interface.h"

#include "handlers/input/inc/handler_water_level.h"
#include "handlers/output/inc/handler_buzzer.h"
#include "handlers/input/inc/handler_keypad.h"
#include "handlers/input/inc/handler_mems.h"
#include "handlers/input/inc/handler_load_sense.h"
#include "handlers/output/inc/handler_drain_valve.h"

#define FINAL_INSPECTION_TIMEOUT_MSEC (20U * 60U * 1000U)
#define SAMPLES_COUNTS                (10U)
#define LED_FLASH_INTERVAL_MSEC       (5000UL)

static uint8_t finalInspection_timeout_check(uint32_t period);

static app_tm_finalInspection_state_t finalInspectionState = TM_FINAL_INSPECTION_STANDBY;
static app_tm_finalInspectionSpinDisplay_t finalInspectionDataTypeState = TM_FINAL_INSPECTION_SPIN_DISPLAY_NORMAL;
static app_tm_finalInspectionError_t finalInspectionError = TM_FINAL_INSPECTION_ERROR_NONE;
static app_tm_finalInspectionMechRankDisplayState_t finalInspectionDisplayState = TM_FINAL_INSPECTION_STATE_DISPLAY;
static app_tm_finalInspectionWeightSensorState_t finalInspectionWeightSensorState = TM_FINAL_INSPECTION_WEIGHT_SENSOR_OK;

static uint32_t gu32_finalInspection_brakingTimeMSec = 0;
static uint32_t gu32_finalInspection_weightData = 0;
static uint32_t gu32_finalInspection_mechRank = 0;

static uint32_t gu32_finalInspection_totalTimeMSec = 0;

void app_tm_finalInspection_process(uint32_t period)
{
    static float gf_testMode_waterLevel_correctionFactor = 1.0f;
    static uint8_t gu8_waterLevelCalibrationEnd_flag = 0;
    static uint32_t gau32_testMode_waterLevel_valueBuffer[SAMPLES_COUNTS] = {0};
    uint32_t sum = 0;
    float avg = 0.0f;
    uint32_t u32_testMode_waterLevel_correctionFactor;
    uint32_t motorSpeed;
    static uint32_t tmpCnt = 0;
    uint32_t currentLidState;
    static uint8_t buzzerOneTime_flag = 0;
    static uint8_t u8_index = 0;
    static uint32_t unbalanceOffCnt = 0;
    static uint8_t motorSpeed_flag = 0;
    static uint32_t ledFlashInterval = 0;

    switch (app_tm_state_get())
    {
        case APP_TEST_MODE_INIT:
        {
            app_tm_state_set(APP_TEST_MODE_OPERATION);
            finalInspectionState = TM_FINAL_INSPECTION_STANDBY;
            finalInspectionError = TM_FINAL_INSPECTION_ERROR_NONE;
            handler_mems_xThreshold_set(handler_mems_xThresholdTestModeValue_get());
            handler_mems_yThreshold_set(handler_mems_yThresholdTestModeValue_get());
            handler_mems_zThreshold_set(handler_mems_zThresholdTestModeValue_get());
            handler_mems_hitCnt_set(handler_mems_hitCntTestModeValue_get());
            handler_mems_critHitCnt_set((handler_mems_critHitCntTestModeValue_get()));
            for (uint8_t i = 0; i < app_tm_userInterface_keyConfig_get().fnNum; i++)
            {
                fnSW[i] = app_tm_userInterface_keyConfig_get().fnSW[i];
            }
        }
        break;

        case APP_TEST_MODE_OPERATION:
        {
            if (finalInspection_timeout_check(period))
            {
                finalInspectionState = TM_FINAL_INSPECTION_DONE;
            }

            if (finalInspectionState > TM_FINAL_INSPECTION_STANDBY && finalInspectionState < TM_FINAL_INSPECTION_TOTAL_TIME_CHECK)
            {
                gu32_finalInspection_totalTimeMSec += period;
            }

            switch (finalInspectionState)
            {
                case TM_FINAL_INSPECTION_STANDBY:
                {
                    if ((fnSW[0] != NULL) && *(fnSW[0]) == HANDLER_KEY_PRESSED)
                    {
                        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                        finalInspectionState = TM_FINAL_INSPECTION_DRAIN;
                    }
                    else if ((fnSW[4] != NULL) && *(fnSW[4]) != HANDLER_KEY_RELEASED)
                    {
                        unbalanceOffCnt += period;
                        if (unbalanceOffCnt >= 3000)
                        {
                            unbalanceOffCnt = 0;
                            app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
                            app_unbalance_detection_set(0);
                        }
                    }

                    if ((fnSW[3] != NULL) && *(fnSW[3]) == HANDLER_KEY_PRESSED)
                    {
                        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                        if (finalInspectionDataTypeState < TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_CRITICAL_HIT)
                        {
                            finalInspectionDataTypeState++;
                        }
                        else
                        {
                            finalInspectionDataTypeState = TM_FINAL_INSPECTION_SPIN_DISPLAY_NORMAL;
                        }
                    }

                    if ((fnSW[5] != NULL) && *(fnSW[5]) == HANDLER_KEY_PRESSED)
                    {
                        if (finalInspectionDataTypeState == TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_X)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                            if (handler_mems_xThresholdValue_get() < 9000)
                            {
                                handler_mems_xThreshold_set(handler_mems_xThresholdValue_get() + 10);
                            }
                        }
                        else if (finalInspectionDataTypeState == TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_Y)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                            if (handler_mems_yThresholdValue_get() < 9000)
                            {
                                handler_mems_yThreshold_set(handler_mems_yThresholdValue_get() + 10);
                            }
                        }
                        else if (finalInspectionDataTypeState == TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_Z)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                            if (handler_mems_zThresholdValue_get() < 9000)
                            {
                                handler_mems_zThreshold_set(handler_mems_zThresholdValue_get() + 10);
                            }
                        }
                        else if (finalInspectionDataTypeState == TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_NORMAL_HIT)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                            if (handler_mems_hitCnt_get() < 255)
                            {
                                handler_mems_hitCnt_set(handler_mems_hitCnt_get() + 1);
                            }
                        }
                        else if (finalInspectionDataTypeState == TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_CRITICAL_HIT)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                            if (handler_mems_critHitCnt_get() < 255)
                            {
                                handler_mems_critHitCnt_set(handler_mems_critHitCnt_get() + 1);
                            }
                        }
                    }
                    else if ((fnSW[6] != NULL) && *(fnSW[6]) == HANDLER_KEY_PRESSED)
                    {
                        if (finalInspectionDataTypeState == TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_X)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                            if (handler_mems_xThresholdValue_get() > 10)
                            {
                                handler_mems_xThreshold_set(handler_mems_xThresholdValue_get() - 10);
                            }
                        }
                        else if (finalInspectionDataTypeState == TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_Y)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                            if (handler_mems_yThresholdValue_get() > 10)
                            {
                                handler_mems_yThreshold_set(handler_mems_yThresholdValue_get() - 10);
                            }
                        }
                        else if (finalInspectionDataTypeState == TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_Z)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                            if (handler_mems_zThresholdValue_get() > 10)
                            {
                                handler_mems_zThreshold_set(handler_mems_zThresholdValue_get() - 10);
                            }
                        }
                        else if (finalInspectionDataTypeState == TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_NORMAL_HIT)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                            if (handler_mems_hitCnt_get() > 1)
                            {
                                handler_mems_hitCnt_set(handler_mems_hitCnt_get() - 1);
                            }
                        }
                        else if (finalInspectionDataTypeState == TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_CRITICAL_HIT)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                            if (handler_mems_critHitCnt_get() > 1)
                            {
                                handler_mems_critHitCnt_set(handler_mems_critHitCnt_get() - 1);
                            }
                        }
                    }

                    gu32_finalInspection_totalTimeMSec = 0;
                    handler_loadSense_weightG_set(0);
                    tmpCnt = 0;
                    gu32_finalInspection_brakingTimeMSec = 0;
                    buzzerOneTime_flag = 0;
                    gu8_waterLevelCalibrationEnd_flag = 0;
                    finalInspectionDisplayState = TM_FINAL_INSPECTION_WEIGHT_DISPLAY;
                    finalInspectionWeightSensorState = TM_FINAL_INSPECTION_WEIGHT_SENSOR_OK;
                    finalInspectionError = TM_FINAL_INSPECTION_ERROR_NONE;
                }
                break;

                case TM_FINAL_INSPECTION_DRAIN:
                {
                    if (handler_waterLevel_resetLevel_check(WATER_LEVEL_RESET_TEST_MODE_CHECK))
                    {
                        if (tmpCnt >= 3000)
                        {
                            tmpCnt = 0;
                            finalInspectionState = TM_FINAL_INSPECTION_LID_CLOSE_CHECK;
                        }
                        else
                        {
                            tmpCnt += period;
                        }
                    }
                }
                break;

                case TM_FINAL_INSPECTION_LID_CLOSE_CHECK:
                {
                    currentLidState = app_lidSensor_state_get();
                    if (currentLidState == APP_LID_SENSOR_CLOSED)
                    {
                        if (tmpCnt >= 4000)
                        {
                            tmpCnt = 0;
                            finalInspectionState = TM_FINAL_INSPECTION_SPIN;
                            finalInspectionDataTypeState = TM_FINAL_INSPECTION_SPIN_DISPLAY_NORMAL;
                            handler_mems_unbalanceCnt_reset();
                            app_unbalance_detection_set(0);
                        }
                        else
                        {
                            tmpCnt += period;
                        }
                    }
                }
                break;

                case TM_FINAL_INSPECTION_SPIN:
                {
                    if (handler_drainValve_state_get() == HANDLER_DRAIN_VALVE_OPEN)
                    {
                        currentLidState = app_lidSensor_state_get();
                        motorSpeed = app_motor_speed_get();

                        if (currentLidState == APP_LID_SENSOR_OPENED && motorSpeed < 650)
                        {
                            finalInspectionState = TM_FINAL_INSPECTION_LID_CLOSE_CHECK;
                        }
                        else if (currentLidState == APP_LID_SENSOR_OPENED && motorSpeed >= 650)
                        {
                            finalInspectionState = TM_FINAL_INSPECTION_BRAKING;
                            finalInspectionDataTypeState = TM_FINAL_INSPECTION_SPIN_DISPLAY_NORMAL;
                        }
                        else if (handler_mems_unbalanceDetectionFlag_get() == 1)
                        {
                            finalInspectionError = TM_FINAL_INSPECTION_ERROR_UNBALANCE;
                            finalInspectionState = TM_FINAL_INSPECTION_ERROR;
                            app_buzzer_tone_set(APP_BUZZER_TONE_FAULT);
                            finalInspectionDataTypeState = TM_FINAL_INSPECTION_SPIN_DISPLAY_NORMAL;
                        }
                    }
                }
                break;

                case TM_FINAL_INSPECTION_BRAKING:
                {
                    motorSpeed = app_motor_speed_get();

                    if ((fnSW[3] != NULL) && *(fnSW[3]) == HANDLER_KEY_PRESSED)
                    {
                        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                        if (finalInspectionDataTypeState < TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_CRITICAL_HIT)
                        {
                            finalInspectionDataTypeState++;
                        }
                        else
                        {
                            finalInspectionDataTypeState = TM_FINAL_INSPECTION_SPIN_DISPLAY_NORMAL;
                        }
                    }
                    else
                    {
                        if (motorSpeed > 0 && motorSpeed_flag == 0)
                        {
                            gu32_finalInspection_brakingTimeMSec += period;
                        }
                        else
                        {
                            if (motorSpeed <= 0)
                            {
                                motorSpeed_flag = 1;
                            }

                            if ((fnSW[1] != NULL) && *(fnSW[1]) == HANDLER_KEY_PRESSED)
                            {
                                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                                finalInspectionState = TM_FINAL_INSPECTION_LED_FLASH;
                            }
                        }
                    }

                    if (gu32_finalInspection_brakingTimeMSec > 7200 && buzzerOneTime_flag == 0)
                    {
                        buzzerOneTime_flag = 1;
                        app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
                    }
                }
                break;

                case TM_FINAL_INSPECTION_LED_FLASH:
                {
                    ledFlashInterval += period;
                    if (ledFlashInterval >= LED_FLASH_INTERVAL_MSEC)
                    {
                        ledFlashInterval = 0;
                        finalInspectionState = TM_FINAL_INSPECTION_MECH_RANKING_WATER_LEVEL_CALIBRATION;
                    }
                }
                break;

                case TM_FINAL_INSPECTION_MECH_RANKING_WATER_LEVEL_CALIBRATION:
                {
                    if (gu8_waterLevelCalibrationEnd_flag == 0)
                    {
                        gau32_testMode_waterLevel_valueBuffer[u8_index] = handler_waterLevel_currentFreq_get();
                        u8_index++;
                        if (u8_index >= SAMPLES_COUNTS)
                        {
                            for (u8_index = 0; u8_index < SAMPLES_COUNTS; u8_index++)
                            {
                                sum += gau32_testMode_waterLevel_valueBuffer[u8_index];
                            }

                            u8_index = 0;
                            avg = (float)sum / SAMPLES_COUNTS;
                            gf_testMode_waterLevel_correctionFactor = avg;

                            u32_testMode_waterLevel_correctionFactor = (uint32_t)gf_testMode_waterLevel_correctionFactor;

                            app_memory_resetWaterLevel_set(u32_testMode_waterLevel_correctionFactor);
                            gu8_waterLevelCalibrationEnd_flag = 1;
                        }
                    }

                    gu32_finalInspection_weightData = handler_loadSense_weightG_get();

                    if (gu32_finalInspection_weightData && gu8_waterLevelCalibrationEnd_flag == 1)
                    {
                        if (handler_loadSense_resetLoad_check(gu32_finalInspection_weightData) == RET_NOK)
                        {
                            gu32_finalInspection_mechRank = 0xff;
                            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
                        }

                        if ((fnSW[2] != NULL) && *(fnSW[2]) == HANDLER_KEY_PRESSED)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                            finalInspectionState = TM_FINAL_INSPECTION_WEIGHT_SENSOR_CHECK;
                            handler_loadSense_weightG_set(0);
                        }

                        tmpCnt += period;

                        switch (finalInspectionDisplayState)
                        {
                            case TM_FINAL_INSPECTION_WEIGHT_DISPLAY:
                            {
                                if (tmpCnt > 1200)
                                {
                                    tmpCnt = 0;
                                    finalInspectionDisplayState = TM_FINAL_INSPECTION_MECH_RANK_DISPLAY;
                                }
                            }
                            break;

                            case TM_FINAL_INSPECTION_MECH_RANK_DISPLAY:
                            {
                                if (tmpCnt > 1200)
                                {
                                    tmpCnt = 0;
                                    finalInspectionDisplayState = TM_FINAL_INSPECTION_STATE_DISPLAY;
                                }
                            }
                            break;

                            case TM_FINAL_INSPECTION_STATE_DISPLAY:
                            {
                                if (tmpCnt > 1200U)
                                {
                                    tmpCnt = 0;
                                    finalInspectionDisplayState = TM_FINAL_INSPECTION_WEIGHT_DISPLAY;
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
                    else
                    {
                        finalInspectionDisplayState = TM_FINAL_INSPECTION_STATE_DISPLAY;
                    }
                }
                break;

                case TM_FINAL_INSPECTION_WEIGHT_SENSOR_CHECK:
                {
                    gu32_finalInspection_weightData = handler_loadSense_weightG_get();

                    if (gu32_finalInspection_weightData)
                    {
                        if (handler_loadSense_resetLoad_check(gu32_finalInspection_weightData) == RET_NOK)
                        {
                            finalInspectionWeightSensorState = TM_FINAL_INSPECTION_WEIGHT_SENSOR_NOK;
                        }
                        else
                        {
                            finalInspectionWeightSensorState = TM_FINAL_INSPECTION_WEIGHT_SENSOR_OK;
                        }

                        finalInspectionState = TM_FINAL_INSPECTION_TOTAL_TIME_CHECK;
                    }
                }
                break;

                case TM_FINAL_INSPECTION_TOTAL_TIME_CHECK:
                {
                    if (tmpCnt >= 5000)
                    {
                        finalInspectionState = TM_FINAL_INSPECTION_STANDBY;
                        tmpCnt = 0;
                    }
                    else
                    {
                        tmpCnt += period;
                    }
                }
                break;

                case TM_FINAL_INSPECTION_ERROR:
                {
                    switch (finalInspectionError)
                    {
                        case TM_FINAL_INSPECTION_ERROR_UNBALANCE:
                        {
                            if ((fnSW[3] != NULL) && *(fnSW[3]) == HANDLER_KEY_PRESSED)
                            {
                                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                                if (finalInspectionDataTypeState < TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_CRITICAL_HIT)
                                {
                                    finalInspectionDataTypeState++;
                                }
                                else
                                {
                                    finalInspectionDataTypeState = TM_FINAL_INSPECTION_SPIN_DISPLAY_NORMAL;
                                }
                            }
                        }
                        break;

                        default:
                            // do nothing
                            break;
                    }
                }
                break;

                case TM_FINAL_INSPECTION_DONE:
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
}

app_tm_finalInspection_state_t app_tm_finalInspectionState_get(void)
{
    return finalInspectionState;
}

app_tm_finalInspectionError_t app_tm_finalInspectionError_get(void)
{
    return finalInspectionError;
}

uint32_t app_tm_finalInspectionBrakingTimeMSec_get(void)
{
    return gu32_finalInspection_brakingTimeMSec;
}

uint32_t app_tm_finalInspectionMechRank_get(void)
{
    return gu32_finalInspection_mechRank;
}

app_tm_finalInspectionMechRankDisplayState_t app_tm_finalInspectionDisplayState_get(void)
{
    return finalInspectionDisplayState;
}

app_tm_finalInspectionWeightSensorState_t app_tm_finalInspectionWeightSensorState_get(void)
{
    return finalInspectionWeightSensorState;
}

uint32_t app_tm_finalInspectionTotalTimeMSec_get(void)
{
    return gu32_finalInspection_totalTimeMSec;
}

app_tm_finalInspectionSpinDisplay_t app_tm_finalInspectionDataDisplayType_get(void)
{
    return finalInspectionDataTypeState;
}

static uint8_t finalInspection_timeout_check(uint32_t period)
{
    static uint32_t timeoutIntervalMSec = 0;
    uint8_t ret = 0;

    if (timeoutIntervalMSec > FINAL_INSPECTION_TIMEOUT_MSEC)
    {
        ret = 1;
    }
    else
    {
        timeoutIntervalMSec += period;
        ret = 0;
    }

    return ret;
}
