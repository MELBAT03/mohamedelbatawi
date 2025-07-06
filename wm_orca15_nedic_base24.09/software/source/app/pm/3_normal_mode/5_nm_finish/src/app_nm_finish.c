#include "def.h"
#include "app_nm_finish.h"
#include "app_nm.h"

#include "app_nm_log.h"
#include "app_lid.h"
#include "app_motor.h"
#include "app_buzzer.h"

#include "app_nm_endurance.h"

#define NORMAL_MODE_FINISH_STATE_TIMEOUT_MSEC (10000UL)

app_nm_state_t app_nm_finish_process(uint32_t period)
{
    app_nm_state_t ret = APP_NORMAL_MODE_FINISH;
    static uint32_t finishStateTimeoutMsec = 0;
    static uint8_t buzzerOneTime_flag = 0;

    if (buzzerOneTime_flag == 0U)
    {
        buzzerOneTime_flag = 1;
        app_buzzer_tone_set(APP_BUZZER_TONE_FINISH);
    }
    app_nm_log_completedCycleCount_set(app_userInterface_data_get()->normalMode.common.courseSelection);

    finishStateTimeoutMsec += period;
    if ((finishStateTimeoutMsec >= NORMAL_MODE_FINISH_STATE_TIMEOUT_MSEC) && (app_lidLock_state_get() == APP_LID_LOCK_OFF) && (APP_MOTOR_HALT_OK == app_motor_haltState_get()))
    {
        finishStateTimeoutMsec = 0;

        if (APP_NORMAL_MODE_UI_CONTINUOUS_CYCLE == app_userInterface_data_get()->normalMode.common.cycleTypeSelection)
        {
            app_nm_endurance_restart();
            ret = APP_NORMAL_MODE_INIT;
        }
        else
        {
            ret = APP_NORMAL_MODE_DONE;
        }
    }

    return ret;
}
