#include "def.h"
#include "os.h"

#include "app_drain.h"
#include "app_buzzer.h"

#include "app_nm_dry.h"
#include "app_nm.h"
#include "app_memory.h"
#include "app_nm_dry_drain.h"
#include "app_nm_dry_motor.h"

static app_nm_state_t normalMode_dry_pauseRequest_handle(app_nm_state_t state);

static void app_nm_dry_data_refresh(void);

static app_nm_dry_state_t gx_app_dry_state = APP_NORMAL_MODE_DRY_INIT;

app_nm_state_t app_nm_dry_process(uint32_t period)
{
    app_nm_state_t ret = APP_NORMAL_MODE_DRY;

    switch (gx_app_dry_state)
    {
        case APP_NORMAL_MODE_DRY_INIT:
        {
            gx_app_dry_state = APP_NORMAL_MODE_DRY_PRE_SETUP;
        }
        break;

        case APP_NORMAL_MODE_DRY_PRE_SETUP:
        {
            app_nm_dry_preSetup_process(period);

            if (APP_NORMAL_MODE_DRY_PRE_SETUP_DONE == app_nm_dry_preSetup_state_get())
            {
                gx_app_dry_state = APP_NORMAL_MODE_DRY_SETUP;
            }
        }
        break;

        case APP_NORMAL_MODE_DRY_SETUP:
        {
            app_nm_dry_setup_process(period);

            if (APP_NORMAL_MODE_DRY_SETUP_DONE == app_nm_dry_setup_state_get())
            {
                gx_app_dry_state = APP_NORMAL_MODE_DRY_OPERATION;
            }
        }
        break;

        case APP_NORMAL_MODE_DRY_OPERATION:
        {
            app_nm_dry_operation_process(period);

            if (APP_NORMAL_MODE_DRY_OPERATION_DONE == app_nm_dry_operation_state_get())
            {
                gx_app_dry_state = APP_NORMAL_MODE_DRY_FINISH;
            }
        }
        break;

        case APP_NORMAL_MODE_DRY_FINISH:
        {
            gx_app_dry_state = APP_NORMAL_MODE_DRY_DONE;
        }
        break;

        case APP_NORMAL_MODE_DRY_DONE:
        {
            ret = APP_NORMAL_MODE_FINISH;
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }

    ret = normalMode_dry_pauseRequest_handle(ret);

    return ret;
}

app_nm_dry_state_t app_nm_dry_state_get(void)
{
    return gx_app_dry_state;
}

void app_nm_dry_state_set(app_nm_dry_state_t value)
{
    gx_app_dry_state = value;
}

void app_nm_dry_data_update(void)
{
    if ((app_nm_savedUIData_get()->dry.dryWaterLevel != app_userInterface_data_get()->normalMode.dry.dryWaterLevel) ||
        (app_nm_savedUIData_get()->dry.dryIntensitySelection != app_userInterface_data_get()->normalMode.dry.dryIntensitySelection) ||
        (app_nm_savedUIData_get()->dry.dryTimeSelection != app_userInterface_data_get()->normalMode.dry.dryTimeSelection))
    {
        app_nm_dry_data_refresh();
    }

    app_nm_dry_totalTime_update();
}

void app_nm_dry_totalTime_update(void)
{
    app_nm_data_set()->dry.dryTotalTimeMSec = 0;

    for (uint8_t i = 0; i < APP_NORMAL_MODE_DRY_PRE_SETUP_STATES_WITH_DATA; i++)
    {
        app_nm_data_set()->dry.dryTotalTimeMSec += app_nm_data_get()->dry.preSetupTimeMSec[i];
    }

    for (uint8_t i = 0; i < APP_NORMAL_MODE_DRY_OPERATION_STATES_WITH_DATA; i++)
    {
        app_nm_data_set()->dry.dryTotalTimeMSec += app_nm_data_get()->dry.operationTimeMSec[i];
    }
}

void app_nm_dry_currentUI_save(void)
{
    app_nm_savedUIData_get()->dry.dryTimeSelection = app_userInterface_data_get()->normalMode.dry.dryTimeSelection;
    app_nm_savedUIData_get()->dry.dryIntensitySelection = app_userInterface_data_get()->normalMode.dry.dryIntensitySelection;
    app_nm_savedUIData_get()->dry.dryWaterLevel = app_userInterface_data_get()->normalMode.dry.dryWaterLevel;
}

void app_nm_dry_reset(void)
{
    gx_app_dry_state = APP_NORMAL_MODE_DRY_INIT;

    app_nm_dry_preSetup_reset();
    app_nm_dry_setup_reset();
    app_nm_dry_operation_reset();
}

static app_nm_state_t normalMode_dry_pauseRequest_handle(app_nm_state_t state)
{
    app_nm_state_t ret = state;

    if (APP_NORMAL_MODE_UI_SETTING_OK == app_nm_userInterface_startPause_check())
    {
        if (APP_NORMAL_MODE_UI_SETTING_OK == app_nm_userInterface_setting_check())
        {
            app_nm_prevState_set(ret);
            ret = APP_NORMAL_MODE_PAUSE;
            app_buzzer_tone_set(APP_BUZZER_TONE_PAUSE);
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

    return ret;
}

static void app_nm_dry_data_refresh(void)
{
    const app_dry_operation_timeTable_t *operationData = NULL;
    const app_dry_setup_timeTable_t *preSetupData = NULL;

    preSetupData = app_nm_dry_preSetup_stateTotalTimeMSec_get(app_userInterface_data_get()->normalMode.dry.dryTimeSelection);

    operationData = app_nm_dry_operation_stateTotalMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel, app_userInterface_data_get()->normalMode.dry.dryIntensitySelection);

    if (NULL != preSetupData)
    {
        for (uint8_t i = app_nm_dry_preSetup_state_get(); i < APP_NORMAL_MODE_DRY_PRE_SETUP_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->dry.preSetupTimeMSec[i] = preSetupData->stateTotalTimeMSec[i];

            if (APP_NORMAL_MODE_DRY_PRE_SETUP_SPIN == i)
            {
                app_nm_data_set()->dry.preSetupTimeMSec[APP_NORMAL_MODE_DRY_PRE_SETUP_SPIN] += app_nm_dry_preSpinStateTotalTimeMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel);
            }
        }
    }
    else
    {
        for (uint8_t i = 0; i < APP_NORMAL_MODE_DRY_PRE_SETUP_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->dry.preSetupTimeMSec[i] = 0;
        }
    }

    if (NULL != operationData)
    {
        for (uint8_t i = app_nm_dry_operation_state_get(); i < APP_NORMAL_MODE_DRY_OPERATION_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->dry.operationTimeMSec[i] = operationData->stateTotalTimeMSec[i];

            if (APP_NORMAL_MODE_DRY_OPERATION_FINAL_SPIN == i)
            {
                app_nm_data_set()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_FINAL_SPIN] += app_nm_dry_preSpinStateTotalTimeMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel);
            }
        }
    }
    else
    {
        for (uint8_t i = 0; i < APP_NORMAL_MODE_DRY_OPERATION_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->dry.operationTimeMSec[i] = 0;
        }
    }

    app_nm_dry_totalTime_update();

    app_nm_dry_currentUI_save();
}
