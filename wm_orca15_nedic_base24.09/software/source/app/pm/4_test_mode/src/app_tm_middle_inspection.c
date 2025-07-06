#include "def.h"
#include "os.h"

#include "app_memory.h"
#include "app_test_mode.h"
#include "app_buzzer.h"
#include "app_lid.h"
#include "app_motor.h"

#include "app_tm_user_interface.h"

#include "handlers/input/inc/handler_water_level.h"
#include "handlers/input/inc/handler_load_sense.h"
#include "handlers/output/inc/handler_buzzer.h"
#include "handlers/input/inc/handler_keypad.h"

#define MIDDLE_INSPECTION_TIMEOUT_MSEC (20 * 60 * 1000)

static uint8_t middleInspection_timeout_check(uint32_t period);

static app_tm_middleInspection_state_t middleInspectionState = TM_MIDDLE_INSPECTION_STANDBY;
static app_tm_middleInspectionError_t middleInspectionError = TM_MIDDLE_INSPECTION_ERROR_NONE;
static app_tm_middleInspection_valveCheckState_t middleInspectionValveState = TM_MIDDLE_INSPECTION_NONE_VALVE_CHECK;

static uint32_t u32_current_waterLevel_freq = 0;
static uint32_t waterLevelFreqUpperLimit = 0;
static uint32_t waterLevelFreqLowerLimit = 0;

void app_tm_middleInspection_process(uint32_t period)
{
    uint32_t motorSpeed;
    static uint32_t tmpCnt = 0;
    static uint32_t rotationStopCounter = 0;
    uint32_t currentLidState;
    uint32_t gu32_middleInspection_weightData = 0;

    switch (app_tm_state_get())
    {
        case APP_TEST_MODE_INIT:
        {
            app_tm_state_set(APP_TEST_MODE_OPERATION);
            middleInspectionState = TM_MIDDLE_INSPECTION_STANDBY;
            middleInspectionError = TM_MIDDLE_INSPECTION_ERROR_NONE;
            middleInspectionValveState = TM_MIDDLE_INSPECTION_NONE_VALVE_CHECK;
            for (uint8_t i = 0; i < app_tm_userInterface_keyConfig_get().fnNum; i++)
            {
                fnSW[i] = app_tm_userInterface_keyConfig_get().fnSW[i];
            }
        }
        break;

        case APP_TEST_MODE_OPERATION:
        {
            u32_current_waterLevel_freq = handler_waterLevel_currentFreq_get();
            waterLevelFreqUpperLimit = handler_waterLevel_freqValue_get(35);
            waterLevelFreqLowerLimit = handler_waterLevel_freqValue_get(10);

            if (middleInspection_timeout_check(period) != 0UL)
            {
                middleInspectionState = TM_MIDDLE_INSPECTION_DONE;
            }

            switch (middleInspectionState)
            {
                case TM_MIDDLE_INSPECTION_STANDBY:
                {
                    if ((fnSW[0] != NULL) && *(fnSW[0]) == HANDLER_KEY_PRESSED)
                    {
                        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                        middleInspectionState = TM_MIDDLE_INSPECTION_WATER_VALVE_CHECK;
                    }
                }
                break;

                case TM_MIDDLE_INSPECTION_WATER_VALVE_CHECK:
                {
                    if ((fnSW[1] != NULL) && *(fnSW[1]) == HANDLER_KEY_PRESSED)
                    {
                        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                        middleInspectionState = TM_MIDDLE_INSPECTION_WATER_LEVEL_CHECK;
                    }

                    if (middleInspectionValveState == TM_MIDDLE_INSPECTION_NONE_VALVE_CHECK)
                    {
                        if ((fnSW[2] != NULL) && *(fnSW[2]) != HANDLER_KEY_RELEASED)
                        {
                            middleInspectionValveState = TM_MIDDLE_INSPECTION_COLD_VALVE_CHECK;
                        }
                        else if ((fnSW[3] != NULL) && *(fnSW[3]) != HANDLER_KEY_RELEASED)
                        {
                            middleInspectionValveState = TM_MIDDLE_INSPECTION_SOFTENER_VALVE_CHECK;
                        }
                        else if ((fnSW[4] != NULL) && *(fnSW[4]) != HANDLER_KEY_RELEASED)
                        {
                            middleInspectionValveState = TM_MIDDLE_INSPECTION_HOT_VALVE_CHECK;
                        }
#if defined(WM_SHOWER_VALVE_ENABLED)
                        else if ((fnSW[5] != NULL) && *(fnSW[5]) != HANDLER_KEY_RELEASED)
                        {
                            middleInspectionValveState = TM_MIDDLE_INSPECTION_SHOWER_VALVE_CHECK;
                        }
#endif
                        else if ((fnSW[6] != NULL) && *(fnSW[6]) != HANDLER_KEY_RELEASED)
                        {
                            middleInspectionValveState = TM_MIDDLE_INSPECTION_LID_LOCK_CHECK;
                        }
                    }

                    if (((fnSW[2] != NULL) && *(fnSW[2]) == HANDLER_KEY_RELEASED && middleInspectionValveState == TM_MIDDLE_INSPECTION_COLD_VALVE_CHECK) || ((fnSW[3] != NULL) && *(fnSW[3]) == HANDLER_KEY_RELEASED && middleInspectionValveState == TM_MIDDLE_INSPECTION_SOFTENER_VALVE_CHECK) || ((fnSW[4] != NULL) && *(fnSW[4]) == HANDLER_KEY_RELEASED && middleInspectionValveState == TM_MIDDLE_INSPECTION_HOT_VALVE_CHECK)
#if defined(WM_SHOWER_VALVE_ENABLED)
                        || ((fnSW[5] != NULL) && *(fnSW[5]) == HANDLER_KEY_RELEASED && middleInspectionValveState == TM_MIDDLE_INSPECTION_SHOWER_VALVE_CHECK)
#endif
                    )
                    {
                        middleInspectionValveState = TM_MIDDLE_INSPECTION_NONE_VALVE_CHECK;
                    }
                    else if (middleInspectionValveState == TM_MIDDLE_INSPECTION_LID_LOCK_CHECK && (((fnSW[2] != NULL) && *(fnSW[2]) != HANDLER_KEY_RELEASED) || ((fnSW[3] != NULL) && *(fnSW[3]) != HANDLER_KEY_RELEASED) || ((fnSW[4] != NULL) && *(fnSW[4]) != HANDLER_KEY_RELEASED)
#if defined(WM_SHOWER_VALVE_ENABLED)
                                                                                                   || ((fnSW[5] != NULL) && *(fnSW[5]) != HANDLER_KEY_RELEASED)
#endif
                                                                                                       ))
                    {
                        middleInspectionValveState = TM_MIDDLE_INSPECTION_NONE_VALVE_CHECK;
                    }
                    else
                    {
                        // do nothing
                    }
                }
                break;

                case TM_MIDDLE_INSPECTION_WATER_LEVEL_CHECK:
                {
                    if (tmpCnt >= 4000)
                    {
                        if (((uint32_t)u32_current_waterLevel_freq > (uint32_t)waterLevelFreqUpperLimit) && ((uint32_t)u32_current_waterLevel_freq < (uint32_t)waterLevelFreqLowerLimit))
                        {
                            tmpCnt = 0;
                            middleInspectionError = TM_MIDDLE_INSPECTION_ERROR_NONE;
                            middleInspectionState = TM_MIDDLE_INSPECTION_LOAD_SENSE;
                        }
                        else
                        {
                            tmpCnt = 0;
                            middleInspectionError = TM_MIDDLE_INSPECTION_ERROR_WATER_LEVEL;
                            middleInspectionState = TM_MIDDLE_INSPECTION_ERROR;
                        }
                    }
                    else
                    {
                        tmpCnt += period;
                    }
                }
                break;

                case TM_MIDDLE_INSPECTION_LOAD_SENSE:
                {
                    gu32_middleInspection_weightData = handler_loadSense_weightG_get();

                    if (gu32_middleInspection_weightData)
                    {
                        middleInspectionState = TM_MIDDLE_INSPECTION_WASHING;
                    }
                }
                break;

                case TM_MIDDLE_INSPECTION_WASHING:
                {
                    if (tmpCnt >= 6000)
                    {
                        tmpCnt = 0;
                        middleInspectionState = TM_MIDDLE_INSPECTION_DRAIN;
                    }
                    else
                    {
                        tmpCnt += period;

                        motorSpeed = app_motor_speed_get();
                        if (!motorSpeed)
                        {
                            rotationStopCounter += period;
                            if (rotationStopCounter >= 2000)
                            {
                                middleInspectionError = TM_MIDDLE_INSPECTION_ERROR_MOTOR_ROTATION;
                                middleInspectionState = TM_MIDDLE_INSPECTION_ERROR;
                            }
                        }
                        else
                        {
                            rotationStopCounter = 0;
                        }
                    }
                }
                break;

                case TM_MIDDLE_INSPECTION_DRAIN:
                {
                    if (tmpCnt >= 10000)
                    {
                        if (handler_waterLevel_resetLevel_check(WATER_LEVEL_RESET_CALIBRATION_CHECK))
                        {
                            tmpCnt = 0;
                            middleInspectionState = TM_MIDDLE_INSPECTION_DRAIN_HOLD;
                        }
                        else
                        {
                            if (tmpCnt >= 2 * 60 * 1000)
                            {
                                tmpCnt = 0;
                                middleInspectionError = TM_MIDDLE_INSPECTION_ERROR_DRAIN;
                                middleInspectionState = TM_MIDDLE_INSPECTION_ERROR;
                            }
                            else
                            {
                                tmpCnt += period;
                            }
                        }
                    }
                    else
                    {
                        tmpCnt += period;
                    }
                }
                break;

                case TM_MIDDLE_INSPECTION_DRAIN_HOLD:
                {
                    if (tmpCnt >= 10000)
                    {
                        currentLidState = app_lidSensor_state_get();
                        if (currentLidState == APP_LID_SENSOR_CLOSED)
                        {
                            tmpCnt = 0;
                            middleInspectionState = TM_MIDDLE_INSPECTION_SPIN;
                        }
                        else
                        {
                            tmpCnt = 0;
                            middleInspectionError = TM_MIDDLE_INSPECTION_ERROR_LID;
                            middleInspectionState = TM_MIDDLE_INSPECTION_ERROR;
                        }
                    }
                    else
                    {
                        tmpCnt += period;
                    }
                }
                break;

                case TM_MIDDLE_INSPECTION_SPIN:
                {
                    // do nothing
                }
                break;

                case TM_MIDDLE_INSPECTION_ERROR:
                {
                    if (middleInspectionError == TM_MIDDLE_INSPECTION_ERROR_LID)
                    {
                        currentLidState = app_lidSensor_state_get();
                        if (currentLidState == APP_LID_SENSOR_CLOSED)
                        {
                            tmpCnt = 0;
                            middleInspectionState = TM_MIDDLE_INSPECTION_SPIN;
                        }
                    }
                }
                break;

                case TM_MIDDLE_INSPECTION_DONE:
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

app_tm_middleInspection_state_t app_tm_middleInspectionState_get(void)
{
    return middleInspectionState;
}

app_tm_middleInspectionError_t app_tm_middleInspectionError_get(void)
{
    return middleInspectionError;
}

app_tm_middleInspection_valveCheckState_t app_tm_middleInspectionWaterValveState_get(void)
{
    return middleInspectionValveState;
}

static uint8_t middleInspection_timeout_check(uint32_t period)
{
    static uint32_t timeoutIntervalMSec = 0;
    uint8_t ret = 0;

    if (timeoutIntervalMSec > MIDDLE_INSPECTION_TIMEOUT_MSEC)
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
