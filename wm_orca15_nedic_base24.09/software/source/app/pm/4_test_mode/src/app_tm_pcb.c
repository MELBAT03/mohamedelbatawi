#include "def.h"
#include "os.h"

#include "app_memory.h"
#include "app_test_mode.h"
#include "app_buzzer.h"
#include "app_lid.h"

#include "app_tm_user_interface.h"

#include "handlers/input/inc/handler_water_level.h"
#include "handlers/output/inc/handler_buzzer.h"
#include "handlers/input/inc/handler_keypad.h"

#define PCB_TEST_MODE_STATE_EXIT_INTERVAL_MSEC (300U)

static app_tm_pcb_state_t pcbState = TM_PCB_VERSION_CHECK;

static uint8_t pcb_dmemReset_flag = 1U;

void app_tm_pcb_process(uint32_t period)
{
    static uint32_t stateExitTimeout = 0;
    static app_tm_pcb_state_t prevState = TM_PCB_VERSION_CHECK;

    switch (app_tm_state_get())
    {
        case APP_TEST_MODE_INIT:
        {
            app_tm_state_set(APP_TEST_MODE_OPERATION);
            pcbState = TM_PCB_VERSION_CHECK;
            for (uint8_t i = 0; i < app_tm_userInterface_keyConfig_get().fnNum; i++)
            {
                fnSW[i] = app_tm_userInterface_keyConfig_get().fnSW[i];
            }
        }
        break;

        case APP_TEST_MODE_OPERATION:
        {
            if (stateExitTimeout == 0UL)
            {
                if ((fnSW[0] != NULL) && *(fnSW[0]) == HANDLER_KEY_PRESSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_DRAIN_PUMP_CHECK;
                }
                else if ((fnSW[1] != NULL) && *(fnSW[1]) == HANDLER_KEY_PRESSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_HOT_VALVE_CHECK;
                }
                else if ((fnSW[2] != NULL) && *(fnSW[2]) == HANDLER_KEY_PRESSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_SOFTENER_VALVE_CHECK;
                }
                else if ((fnSW[3] != NULL) && *(fnSW[3]) == HANDLER_KEY_PRESSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_DRAIN_VALVE_CHECK;
                }
                else if ((fnSW[4] != NULL) && *(fnSW[4]) == HANDLER_KEY_PRESSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_COLD_VALVE_CHECK;
                }
#if defined(WM_SHOWER_VALVE_ENABLED)
                else if ((fnSW[5] != NULL) && *(fnSW[5]) == HANDLER_KEY_PRESSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_SHOWER_VALVE_CHECK;
                }
#endif
                else if ((fnSW[6] != NULL) && *(fnSW[6]) == HANDLER_KEY_PRESSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_MOTOR_CCW_CHECK;
                }
                else if ((fnSW[7] != NULL) && *(fnSW[7]) == HANDLER_KEY_PRESSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_MOTOR_CCW_STOP_CHECK;
                }
                else if ((fnSW[8] != NULL) && *(fnSW[8]) == HANDLER_KEY_PRESSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_MOTOR_CW_CHECK;
                }
                else if ((fnSW[9] != NULL) && *(fnSW[9]) == HANDLER_KEY_PRESSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_MOTOR_CW_STOP_CHECK;
                }
                else if ((fnSW[10] != NULL) && *(fnSW[10]) == HANDLER_KEY_PRESSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_DISPLAY_CHECK;
                    if (pcb_dmemReset_flag != 0)
                    {
                        pcb_dmemReset_flag = app_memory_reset();
                    }
                }
                else if ((fnSW[11] != NULL) && *(fnSW[11]) == HANDLER_KEY_PRESSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_WATER_LEVEL_SENSOR_CHECK;
                }
                else if ((fnSW[12] != NULL) && *(fnSW[12]) == HANDLER_KEY_PRESSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_MEMS_CHECK;
                }
                else if ((fnSW[13] != NULL) && *(fnSW[13]) == HANDLER_KEY_PRESSED)
                {
                    // NOTE: not used
                }
                else if ((fnSW[14] != NULL) && *(fnSW[14]) == HANDLER_KEY_PRESSED)
                {
                    // NOTE: not used
                }
                else if ((fnSW[15] != NULL) && *(fnSW[15]) == HANDLER_KEY_PRESSED)
                {
                    // NOTE: not used
                }
                else if ((fnSW[16] != NULL) && *(fnSW[16]) == HANDLER_KEY_PRESSED)
                {
                    // NOTE: not used
                }
                else if ((fnSW[17] != NULL) && *(fnSW[17]) == HANDLER_KEY_PRESSED)
                {
                    // NOTE: not used
                }
                else if ((fnSW[18] != NULL) && *(fnSW[18]) == HANDLER_KEY_PRESSED)
                {
                    // NOTE: not used
                }
                else if ((fnSW[19] != NULL) && *(fnSW[19]) == HANDLER_KEY_PRESSED)
                {
                    // NOTE: not used
                }
                else if ((fnSW[20] != NULL) && *(fnSW[20]) == HANDLER_KEY_PRESSED)
                {
                    // NOTE: not used
                }
                else if ((fnSW[21] != NULL) && *(fnSW[21]) == HANDLER_KEY_PRESSED)
                {
                    // NOTE: not used
                }
                else if ((fnSW[22] != NULL) && *(fnSW[22]) == HANDLER_KEY_PRESSED)
                {
                    // NOTE: not used
                }
                else if ((fnSW[23] != NULL) && *(fnSW[23]) == HANDLER_KEY_PRESSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_LID_LOCK_ON_CHECK;
                }
                else if ((fnSW[24] != NULL) && *(fnSW[24]) == HANDLER_KEY_PRESSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_LID_LOCK_OFF_CHECK;
                }
                // NOTE: make sure this function only gets called here
                else if (app_lidSensor_edgeState_get() == APP_LID_SENSOR_CLOSED)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    pcbState = TM_PCB_LID_SENSOR_CHECK;
                }
                else
                {
                    // do nothing
                }
            }

            if (prevState != pcbState)
            {
                stateExitTimeout += period;
                if (stateExitTimeout >= 300)
                {
                    stateExitTimeout = 0;
                    prevState = pcbState;
                }
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

app_tm_pcb_state_t app_tm_pcbState_get(void)
{
    return pcbState;
}
