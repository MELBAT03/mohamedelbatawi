#include "def.h"
#include "os.h"

#include "app_memory.h"
#include "app_test_mode.h"
#include "app_buzzer.h"

#include "app_tm_user_interface.h"

#include "handlers/input/inc/handler_water_level.h"
#include "handlers/output/inc/handler_buzzer.h"
#include "handlers/input/inc/handler_keypad.h"

static app_tm_unbalanceSpin_state_t unbalanceSpinState = TM_UNBALANCE_SPIN_STANDBY;

void app_tm_unbalanceSpin_process(uint32_t period)
{
    switch (app_tm_state_get())
    {
        case APP_TEST_MODE_INIT:
        {
            app_tm_state_set(APP_TEST_MODE_OPERATION);
            unbalanceSpinState = TM_UNBALANCE_SPIN_STANDBY;
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

                if (unbalanceSpinState < TM_UNBALANCE_SPIN_BRAKE)
                {
                    unbalanceSpinState++;
                }
                else
                {
                    unbalanceSpinState = TM_UNBALANCE_SPIN_SPIN;
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

app_tm_unbalanceSpin_state_t app_tm_unbalanceSpinState_get(void)
{
    return unbalanceSpinState;
}
