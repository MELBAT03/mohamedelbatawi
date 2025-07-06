#include "def.h"
#include "os.h"

#include "app_memory.h"
#include "app_test_mode.h"
#include "app_buzzer.h"
#include "app_nm_log.h"

#include "app_tm_user_interface.h"

#include "handlers/input/inc/handler_water_level.h"
#include "handlers/output/inc/handler_buzzer.h"
#include "handlers/input/inc/handler_keypad.h"

static app_tm_errorCount_state_t errorCntState = TM_ERROR_COUNT_STATE_IDLE;
static app_tm_errorCntData_t currentError = {0};
static uint8_t errorCnt_dmemReset_flag = 1;

void app_tm_errorCnt_process(uint32_t period)
{
    static uint32_t keyPressCnt = 0;

    switch (app_tm_state_get())
    {
        case APP_TEST_MODE_INIT:
        {
            app_tm_state_set(APP_TEST_MODE_OPERATION);
            errorCntState = TM_ERROR_COUNT_STATE_IDLE;
            currentError.currentErrorDisplay = APP_NORMAL_MODE_LOG_ERROR_DRAIN_FAILURE;
            for (uint8_t i = 0; i < app_tm_userInterface_keyConfig_get().fnNum; i++)
            {
                fnSW[i] = app_tm_userInterface_keyConfig_get().fnSW[i];
            }
        }
        break;

        case APP_TEST_MODE_OPERATION:
        {
            switch (errorCntState)
            {
                case TM_ERROR_COUNT_STATE_IDLE:
                {
                    if (fnSW[0] != NULL && *(fnSW[0]) == HANDLER_KEY_PRESSED)
                    {
                        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);

                        errorCntState = TM_ERROR_COUNT_STATE_ERROR_TRAVERSE;
                    }
                }
                break;

                case TM_ERROR_COUNT_STATE_ERROR_TRAVERSE:
                {
                    if (fnSW[0] != NULL && *(fnSW[0]) != HANDLER_KEY_RELEASED)
                    {
                        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                        errorCntState = TM_ERROR_COUNT_STATE_ERROR_DISPLAY;
                    }
                    else if (fnSW[3] != NULL && *(fnSW[3]) != HANDLER_KEY_RELEASED)
                    {
                        if (keyPressCnt >= 2000)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
                            app_nm_log_errorCnt_clear();
                            if (errorCnt_dmemReset_flag != 0)
                            {
                                errorCnt_dmemReset_flag = app_memory_reset();
                            }
                        }
                        else
                        {
                            keyPressCnt += period;
                        }
                    }
                    else
                    {
                        if (fnSW[1] != NULL && *(fnSW[1]) == HANDLER_KEY_PRESSED)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                            if (currentError.currentErrorDisplay < APP_NORMAL_MODE_LOG_ERRORS_TOTAL_NUM)
                            {
                                currentError.currentErrorDisplay++;
                            }
                            else
                            {
                                currentError.currentErrorDisplay = APP_NORMAL_MODE_LOG_ERROR_DRAIN_FAILURE;
                            }
                        }
                        else if (fnSW[2] != NULL && *(fnSW[2]) == HANDLER_KEY_PRESSED)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                            if (currentError.currentErrorDisplay > APP_NORMAL_MODE_LOG_ERROR_DRAIN_FAILURE)
                            {
                                currentError.currentErrorDisplay--;
                            }
                            else
                            {
                                currentError.currentErrorDisplay = APP_NORMAL_MODE_LOG_ERRORS_TOTAL_NUM - 1;
                            }
                        }

                        currentError.currentErrorCnt = app_nm_log_errorCount_get((app_nm_log_error_t)currentError.currentErrorDisplay);
                    }
                }
                break;

                case TM_ERROR_COUNT_STATE_ERROR_DISPLAY:
                {
                    if ((fnSW[1] != NULL && *(fnSW[1]) != HANDLER_KEY_RELEASED) || (fnSW[2] != NULL && *(fnSW[2]) != HANDLER_KEY_RELEASED) || (fnSW[0] != NULL && *(fnSW[0]) != HANDLER_KEY_RELEASED))
                    {
                        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                        errorCntState = TM_ERROR_COUNT_STATE_ERROR_TRAVERSE;
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
        break;

        case APP_TEST_MODE_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

app_tm_errorCount_state_t app_tm_errorCntState_get(void)
{
    return errorCntState;
}

app_tm_errorCntData_t app_tm_errorDisplayedData_get(void)
{
    return currentError;
}
