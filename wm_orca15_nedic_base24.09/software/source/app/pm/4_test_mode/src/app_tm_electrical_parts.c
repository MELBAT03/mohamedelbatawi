#include "def.h"
#include "os.h"

#include "app_memory.h"
#include "app_test_mode.h"
#include "app_buzzer.h"

#include "app_tm_user_interface.h"

#include "handlers/input/inc/handler_water_level.h"
#include "handlers/output/inc/handler_buzzer.h"
#include "handlers/input/inc/handler_keypad.h"

static app_tm_electricalParts_state_t electricalPartsState = TM_ELECTRICAL_PARTS_VERSION_CHECK;

void app_tm_electricalParts_process(uint32_t period)
{
    switch (app_tm_state_get())
    {
        case APP_TEST_MODE_INIT:
        {
            app_tm_state_set(APP_TEST_MODE_OPERATION);
            electricalPartsState = TM_ELECTRICAL_PARTS_VERSION_CHECK;
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

                if (electricalPartsState < TM_ELECTRICAL_PARTS_ENDURANCECOUNT_CHECK)
                {
                    electricalPartsState++;
                }
                else
                {
                    electricalPartsState = TM_ELECTRICAL_PARTS_VERSION_CHECK;
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

app_tm_electricalParts_state_t app_tm_electricalPartsState_get(void)
{
    return electricalPartsState;
}
