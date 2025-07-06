#include "def.h"
#include "os.h"

#include "app_memory.h"
#include "app_test_mode.h"
#include "app_buzzer.h"

#include "app_tm_user_interface.h"

#include "handlers/input/inc/handler_water_level.h"
#include "handlers/output/inc/handler_buzzer.h"
#include "handlers/input/inc/handler_keypad.h"

static app_tm_waterLevelFreq_state_t waterLevelFreqState = TM_WATER_LEVEL_FREQ_FILLING;

void app_tm_waterLevelFreq_process(uint32_t period)
{
    switch (app_tm_state_get())
    {
        case APP_TEST_MODE_INIT:
        {
            app_tm_state_set(APP_TEST_MODE_OPERATION);
            waterLevelFreqState = TM_WATER_LEVEL_FREQ_FILLING;
            for (uint8_t i = 0; i < app_tm_userInterface_keyConfig_get().fnNum; i++)
            {
                fnSW[i] = app_tm_userInterface_keyConfig_get().fnSW[i];
            }
        }
        break;

        case APP_TEST_MODE_OPERATION:
        {
            if (fnSW[0] != NULL && *(fnSW[0]) == HANDLER_KEY_PRESSED)
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);

                if (waterLevelFreqState < TM_WATER_LEVEL_FREQ_DRAIN)
                {
                    waterLevelFreqState++;
                }
                else
                {
                    waterLevelFreqState = TM_WATER_LEVEL_FREQ_FILLING;
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

app_tm_waterLevelFreq_state_t app_tm_waterLevelFreqState_get(void)
{
    return waterLevelFreqState;
}
