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

static app_tm_cycleCount_state_t cycleCntState = TM_CYCLE_COUNT_STATE_IDLE;
static app_tm_cycleCntData_t currentCycle = {0};
static uint8_t cycleCnt_dmemReset_flag = 1;

void app_tm_cycleCnt_process(uint32_t period)
{
    static uint32_t keyPressCnt = 0;
    
    switch (app_tm_state_get())
    {
        case APP_TEST_MODE_INIT:
        {
            app_tm_state_set(APP_TEST_MODE_OPERATION);
            cycleCntState = TM_CYCLE_COUNT_STATE_IDLE;
            currentCycle.currentCycleDisplay = APP_NORMAL_MODE_LOG_COURSE_CYCLE_NUM_START;
            for (uint8_t i = 0; i < app_tm_userInterface_keyConfig_get().fnNum; i++)
            {
                fnSW[i] = app_tm_userInterface_keyConfig_get().fnSW[i];
            }
        }
        break;

        case APP_TEST_MODE_OPERATION:
        {
            switch (cycleCntState)
            {
                case TM_CYCLE_COUNT_STATE_IDLE:
                {
                    if (fnSW[0] != NULL && *(fnSW[0]) == HANDLER_KEY_PRESSED)
                    {
                        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);

                        cycleCntState = TM_CYCLE_COUNT_STATE_CYCLE_TRAVERSE;
                    }
                }
                break;

                case TM_CYCLE_COUNT_STATE_CYCLE_TRAVERSE:
                {
                    if (fnSW[0] != NULL && *(fnSW[0]) != HANDLER_KEY_RELEASED)
                    {
                        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                        cycleCntState = TM_CYCLE_COUNT_STATE_CYCLE_DISPLAY;
                    }
                    else if (fnSW[3] != NULL && *(fnSW[3]) != HANDLER_KEY_RELEASED)
                    {
                        if (keyPressCnt >= 2000)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
                            app_nm_log_courseCnt_clear();
                            if (cycleCnt_dmemReset_flag != 0)
                            {
                                cycleCnt_dmemReset_flag = app_memory_reset();
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
                            if (currentCycle.currentCycleDisplay < APP_NORMAL_MODE_LOG_COURSE_CYCLE_NUM_END)
                            {
                                currentCycle.currentCycleDisplay++;
                            }
                            else
                            {
                                currentCycle.currentCycleDisplay = APP_NORMAL_MODE_LOG_COURSE_CYCLE_NUM_START;
                            }
                        }
                        else if (fnSW[2] != NULL && *(fnSW[2]) == HANDLER_KEY_PRESSED)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                            if (currentCycle.currentCycleDisplay > APP_NORMAL_MODE_LOG_COURSE_CYCLE_NUM_START)
                            {
                                currentCycle.currentCycleDisplay--;
                            }
                            else
                            {
                                currentCycle.currentCycleDisplay = APP_NORMAL_MODE_LOG_COURSE_CYCLE_NUM_END;
                            }
                        }

                        currentCycle.currentCycleCnt = app_nm_log_completedCycleCount_get((app_nm_userInterface_courseSelection_t)(currentCycle.currentCycleDisplay - APP_NORMAL_MODE_LOG_COURSE_CYCLE_NUM_START));
                    }
                }
                break;

                case TM_CYCLE_COUNT_STATE_CYCLE_DISPLAY:
                {
                    if ((fnSW[1] != NULL && *(fnSW[1])) != HANDLER_KEY_RELEASED || (fnSW[2] != NULL && *(fnSW[2]) != HANDLER_KEY_RELEASED) || (fnSW[0] != NULL && *(fnSW[0]) != HANDLER_KEY_RELEASED))
                    {
                        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                        cycleCntState = TM_CYCLE_COUNT_STATE_CYCLE_TRAVERSE;
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

app_tm_cycleCount_state_t app_tm_cycleCntState_get(void)
{
    return cycleCntState;
}

app_tm_cycleCntData_t app_tm_cycleDisplayedData_get(void)
{
    return currentCycle;
}
