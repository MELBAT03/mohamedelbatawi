#include "def.h"

#include "app_nm.h"

#include "app_nm_user_interface.h"
#include "app_buzzer.h"

#define NORMAL_MODE_PAUSE_TIMEOUT_PERIOD_SEC (3000000U)

static uint8_t normalMode_pauseTimeout_check(uint32_t period);

#ifndef PAUSE_TIMEOUT_DISABLE
static uint32_t gu32_pauseTimeoutCnt = 0;
#endif

app_nm_state_t app_nm_pause_process(uint32_t period)
{
    app_nm_state_t ret = APP_NORMAL_MODE_PAUSE;

    if (APP_NORMAL_MODE_UI_SETTING_OK == app_nm_userInterface_startPause_check())
    {
        if (APP_NORMAL_MODE_UI_SETTING_OK == app_nm_userInterface_setting_check())
        {
#ifndef PAUSE_TIMEOUT_DISABLE
            gu32_pauseTimeoutCnt = 0;
#endif
            ret = app_nm_prevState_get();
            app_buzzer_tone_set(APP_BUZZER_TONE_START);
        }
        else if (APP_NORMAL_MODE_UI_SETTING_NOK == app_nm_userInterface_setting_check())
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else
        {
            // do nothing
        }
    }

    if (0U != normalMode_pauseTimeout_check(period))
    {
#ifndef PAUSE_TIMEOUT_DISABLE
        gu32_pauseTimeoutCnt = 0;
#endif
        ret = APP_NORMAL_MODE_FINISH;
        app_buzzer_tone_set(APP_BUZZER_TONE_SHUTDOWN);
    }

    return ret;
}

static uint8_t normalMode_pauseTimeout_check(uint32_t period)
{
    uint8_t ret = 0;

#ifndef PAUSE_TIMEOUT_DISABLE
    gu32_pauseTimeoutCnt += period;
    if (gu32_pauseTimeoutCnt >= NORMAL_MODE_PAUSE_TIMEOUT_PERIOD_SEC)
    {
        ret = 1;
    }
#endif

    return ret;
}
