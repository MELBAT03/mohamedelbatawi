#include "def.h"
#include "os.h"
#include "app_nm.h"
#include "app_drain.h"

#include "app_nm_wash.h"
#include "app_buzzer.h"
#include "app_nm.h"
#include "app_memory.h"
#include "app_nm_wash_rinse.h"
#include "app_nm_wash_spin.h"
#include "app_nm_wash_fill.h"

#include "app_nm_wash_motor.h"

#include "app_nm_wash_user_interface.h"

#include "handlers/output/inc/handler_buzzer.h"

static app_nm_state_t normalMode_wash_pauseRequest_handle(app_nm_state_t state);

static void wash_setup_transition_handle(void);
static void wash_soak_transition_handle(void);
static void wash_wash_transition_handle(void);
static void wash_steam_transition_handle(void);
static void wash_rinse_transition_handle(void);
static void wash_spin_transition_handle(void);

static void wash_data_refresh(void);

static app_nm_wash_state_t gx_app_wash_state = APP_NORMAL_MODE_WASH_INIT;

app_nm_state_t app_nm_wash_process(uint32_t period)
{
    app_nm_state_t ret = APP_NORMAL_MODE_WASH;

    if (APP_NORMAL_MODE_WASH == app_nm_state_get())
    {
        switch (gx_app_wash_state)
        {
            case APP_NORMAL_MODE_WASH_INIT:
            {
                gx_app_wash_state = APP_NORMAL_MODE_WASH_SETUP;
            }
            break;

            case APP_NORMAL_MODE_WASH_SETUP:
            {
                app_nm_wash_setup_process(period);
                wash_setup_transition_handle();
            }
            break;

            case APP_NORMAL_MODE_WASH_SOAK:
            {
                app_nm_wash_soak_process(period);
                wash_soak_transition_handle();
            }
            break;

            case APP_NORMAL_MODE_WASH_WASH:
            {
                app_nm_wash_wash_process(period);
                wash_wash_transition_handle();
            }
            break;

            case APP_NORMAL_MODE_WASH_STEAM:
            {
                app_nm_wash_steam_process(period);
                wash_steam_transition_handle();
            }
            break;

            case APP_NORMAL_MODE_WASH_RINSE:
            {
                app_nm_wash_rinse_process(period);
                wash_rinse_transition_handle();
            }
            break;

            case APP_NORMAL_MODE_WASH_SPIN:
            {
                app_nm_wash_spin_process(period);
                wash_spin_transition_handle();
            }
            break;

            case APP_NORMAL_MODE_WASH_FINISH:
            {
                gx_app_wash_state = APP_NORMAL_MODE_WASH_DONE;
            }
            break;

            case APP_NORMAL_MODE_WASH_DONE:
            {
                if (APP_NORMAL_MODE_UI_OPERATION_WASH_ONLY != app_userInterface_data_get()->normalMode.common.operationSelection)
                {
                    ret = APP_NORMAL_MODE_DRY;
                }
                else
                {
                    ret = APP_NORMAL_MODE_FINISH;
                }
            }
            break;

            default:
            {
                // do nothhing
            }
            break;
        }
    }

    ret = normalMode_wash_pauseRequest_handle(ret);

    return ret;
}

app_nm_wash_state_t app_nm_wash_state_get(void)
{
    return gx_app_wash_state;
}

void app_nm_wash_state_set(app_nm_wash_state_t value)
{
    gx_app_wash_state = value;
}

void app_nm_wash_reset(void)
{
    gx_app_wash_state = APP_NORMAL_MODE_WASH_INIT;

    app_nm_wash_setup_reset();
    app_nm_wash_soak_reset();
    app_nm_wash_wash_reset();
    app_nm_wash_rinse_reset();
    app_nm_wash_spin_reset();
}

void app_nm_wash_data_update(void)
{
    if ((app_nm_savedUIData_get()->wash.waterLevelSelection != app_userInterface_data_get()->normalMode.wash.waterLevelSelection) ||
        (app_nm_savedUIData_get()->common.courseSelection != app_userInterface_data_get()->normalMode.common.courseSelection) ||
        (app_nm_savedUIData_get()->wash.soakSelection != app_userInterface_data_get()->normalMode.wash.soakSelection) ||
        (app_nm_savedUIData_get()->wash.washSelection != app_userInterface_data_get()->normalMode.wash.washSelection) ||
        (app_nm_savedUIData_get()->wash.rinseSelection != app_userInterface_data_get()->normalMode.wash.rinseSelection) ||
        (app_nm_savedUIData_get()->wash.spinSelection != app_userInterface_data_get()->normalMode.wash.spinSelection))
    {
        wash_data_refresh();
        app_nm_wash_currentUI_save();
    }

    app_nm_wash_soakTotalTime_update();
    app_nm_wash_washTotalTime_update();
    app_nm_wash_steamTotalTime_update();
    app_nm_wash_rinseTotalTime_update();
    app_nm_wash_spinTotalTime_update();
    app_nm_wash_tubcleanTotalTime_update();
    app_nm_wash_totalTimeMSec_update();
}

void app_nm_wash_totalTimeMSec_update(void)
{
    if (app_userInterface_data_get()->normalMode.common.tubcleanSelection != APP_NORMAL_MODE_UI_TUBCLEAN_ON)
    {
        app_nm_data_set()->wash.washTotalTimeMSec = app_nm_data_get()->wash.soakTotalTimeMSec + app_nm_data_get()->wash.washWashTotalTimeMSec + app_nm_data_get()->wash.steamTotalTimeMSec + app_nm_data_get()->wash.rinseTotalTimeMSec + app_nm_data_get()->wash.spinTotalTimeMSec;
    }
    else
    {
        app_nm_data_set()->wash.washTotalTimeMSec = app_nm_data_get()->wash.tubcleanTotalTimeMSec;
    }
}

void app_nm_wash_soakTotalTime_update(void)
{
    app_nm_data_set()->wash.soakTotalTimeMSec = 0;

    for (uint8_t i = 0; i < APP_NORMAL_MODE_WASH_STATES_WITH_DATA; i++)
    {
        app_nm_data_set()->wash.soakTotalTimeMSec += app_nm_data_get()->wash.soakStateTimeMSec[i];
    }
    app_nm_data_set()->wash.soakTotalTimeMSec += app_nm_data_get()->wash.soakTimeMSec;
}

void app_nm_wash_washTotalTime_update(void)
{
    app_nm_data_set()->wash.washWashTotalTimeMSec = 0;

    for (uint8_t i = 0; i < APP_NORMAL_MODE_WASH_STATES_WITH_DATA; i++)
    {
        app_nm_data_set()->wash.washWashTotalTimeMSec += app_nm_data_get()->wash.washStateTimeMSec[i];
    }
}

void app_nm_wash_steamTotalTime_update(void)
{
    app_nm_data_set()->wash.steamTotalTimeMSec = 0;

    for (uint8_t i = 0; i < APP_NORMAL_MODE_STEAM_STATES_WITH_DATA; i++)
    {
        app_nm_data_set()->wash.steamTotalTimeMSec += app_nm_data_get()->wash.steamStateTimeMSec[i];
    }
}

void app_nm_wash_rinseTotalTime_update(void)
{
    app_nm_data_set()->wash.rinseTotalTimeMSec = 0;
    for (uint8_t i = 0; i < app_nm_data_get()->wash.rinseNum; i++)
    {
        app_nm_data_set()->wash.rinseTotalTimeMSec += app_nm_data_get()->wash.rinseDrainTimeMSec[i] + app_nm_data_get()->wash.rinseSpinTimeMSec[i] + app_nm_data_get()->wash.rinseBrakeTimeMSec[i] + app_nm_data_get()->wash.rinseFillTimeMSec[i] + app_nm_data_get()->wash.rinseStirTimeMSec[i] + app_nm_data_get()->wash.rinseUntangleTimeMSec[i];
    }

    app_nm_data_set()->wash.rinseTotalTimeMSec += app_nm_data_get()->wash.rinseUnbalanceFillTimeMSec + app_nm_data_get()->wash.rinseUnbalanceStirTimeMSec + app_nm_data_get()->wash.rinseUnbalanceDrainTimeMSec;
}

void app_nm_wash_spinTotalTime_update(void)
{
    app_nm_data_set()->wash.spinTotalTimeMSec = 0;

    for (uint8_t i = 0; i < APP_NORMAL_MODE_SPIN_STATES_WITH_DATA; i++)
    {
        app_nm_data_set()->wash.spinTotalTimeMSec += app_nm_data_set()->wash.spinStateTimeMSec[i];
    }
    app_nm_data_set()->wash.spinTotalTimeMSec += app_nm_data_set()->wash.spinUnbalanceDrainTimeMSec + app_nm_data_set()->wash.spinUnbalanceFillTimeMSec + app_nm_data_set()->wash.spinUnbalanceStirTimeMSec + app_nm_data_set()->wash.spinUnbalanceSuperSpinStirTimeMSec;
}

void app_nm_wash_tubcleanTotalTime_update(void)
{
    app_nm_data_set()->wash.tubcleanTotalTimeMSec = 0;

    for (uint8_t i = 0; i < app_nm_data_get()->wash.tubcleanNum; i++)
    {
        for (uint8_t j = 0; j < APP_NORMAL_MODE_TUBCLEAN_STATES_WITH_DATA; j++)
        {
            app_nm_data_set()->wash.tubcleanTotalTimeMSec += app_nm_data_set()->wash.tubcleanStateTimeMSec[j][i];
        }
    }
}

void app_nm_wash_currentUI_save(void)
{
    app_nm_savedUIData_get()->common.courseSelection = app_userInterface_data_get()->normalMode.common.courseSelection;
    app_nm_savedUIData_get()->wash.waterLevelSelection = app_userInterface_data_get()->normalMode.wash.waterLevelSelection;
    app_nm_savedUIData_get()->wash.soakSelection = app_userInterface_data_get()->normalMode.wash.soakSelection;
    app_nm_savedUIData_get()->wash.washSelection = app_userInterface_data_get()->normalMode.wash.washSelection;
    app_nm_savedUIData_get()->wash.rinseSelection = app_userInterface_data_get()->normalMode.wash.rinseSelection;
    app_nm_savedUIData_get()->wash.spinSelection = app_userInterface_data_get()->normalMode.wash.spinSelection;
}

static app_nm_state_t normalMode_wash_pauseRequest_handle(app_nm_state_t state)
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

static void wash_setup_transition_handle(void)
{
    if (APP_NORMAL_MODE_WASH_SETUP_DONE == app_nm_wash_setup_state_get())
    {
        if (app_userInterface_data_get()->normalMode.wash.washSelection != APP_NORMAL_MODE_UI_WASH_TIME_0_MIN)
        {
            gx_app_wash_state = APP_NORMAL_MODE_WASH_WASH;
        }
        else if ((app_userInterface_data_get()->normalMode.wash.washSelection == APP_NORMAL_MODE_UI_WASH_TIME_0_MIN) && (app_userInterface_data_get()->normalMode.wash.rinseSelection != APP_NORMAL_MODE_UI_RINSE_0_TIMES))
        {
            gx_app_wash_state = APP_NORMAL_MODE_WASH_RINSE;
        }
        else if ((app_userInterface_data_get()->normalMode.wash.washSelection == APP_NORMAL_MODE_UI_WASH_TIME_0_MIN) && (app_userInterface_data_get()->normalMode.wash.rinseSelection == APP_NORMAL_MODE_UI_RINSE_0_TIMES) && ((app_userInterface_data_get()->normalMode.wash.spinSelection != APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN) || (app_userInterface_data_get()->normalMode.wash.superSpinSelection != APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN)))
        {
            gx_app_wash_state = APP_NORMAL_MODE_WASH_SPIN;
        }
        else if ((app_userInterface_data_get()->normalMode.wash.washSelection == APP_NORMAL_MODE_UI_WASH_TIME_0_MIN) && (app_userInterface_data_get()->normalMode.wash.rinseSelection == APP_NORMAL_MODE_UI_RINSE_0_TIMES) && (app_userInterface_data_get()->normalMode.wash.spinSelection == APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN) && (app_userInterface_data_get()->normalMode.wash.superSpinSelection == APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN))
        {
            gx_app_wash_state = APP_NORMAL_MODE_WASH_FINISH;   // :NOTE: this shouldn't happen but added for consistency
        }
        else
        {
            // do nothing
        }
    }
}

static void wash_soak_transition_handle(void)
{
    if (APP_NORMAL_MODE_WASH_SOAK_DONE == app_nm_wash_soak_state_get())
    {
        gx_app_wash_state = APP_NORMAL_MODE_WASH_WASH;
    }
}

static void wash_wash_transition_handle(void)
{
    if (APP_NORMAL_MODE_WASH_WASH_SOAK_TRANSITION == app_nm_wash_wash_state_get())
    {
        gx_app_wash_state = APP_NORMAL_MODE_WASH_SOAK;
    }
    else if (APP_NORMAL_MODE_WASH_WASH_DONE == app_nm_wash_wash_state_get())
    {
        if (app_userInterface_data_get()->normalMode.wash.steamSelection != APP_NORMAL_MODE_UI_STEAM_OFF)
        {
            gx_app_wash_state = APP_NORMAL_MODE_WASH_STEAM;
        }
        else if (app_userInterface_data_get()->normalMode.wash.rinseSelection != APP_NORMAL_MODE_UI_RINSE_0_TIMES)
        {
            gx_app_wash_state = APP_NORMAL_MODE_WASH_RINSE;
        }
        else if ((app_userInterface_data_get()->normalMode.wash.rinseSelection == APP_NORMAL_MODE_UI_RINSE_0_TIMES) && ((app_userInterface_data_get()->normalMode.wash.spinSelection != APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN) || (app_userInterface_data_get()->normalMode.wash.superSpinSelection != APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN)))
        {
            gx_app_wash_state = APP_NORMAL_MODE_WASH_SPIN;
        }
        else if ((app_userInterface_data_get()->normalMode.wash.rinseSelection == APP_NORMAL_MODE_UI_RINSE_0_TIMES) && (app_userInterface_data_get()->normalMode.wash.spinSelection == APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN) && (app_userInterface_data_get()->normalMode.wash.superSpinSelection == APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN))
        {
            gx_app_wash_state = APP_NORMAL_MODE_WASH_FINISH;
        }
        else
        {
            // do nothing
        }
    }
    else
    {
        // do nothing
    }
}

static void wash_steam_transition_handle(void)
{
    if (APP_NORMAL_MODE_WASH_STEAM_DONE == app_nm_wash_steam_state_get())
    {
        if (app_userInterface_data_get()->normalMode.wash.rinseSelection != APP_NORMAL_MODE_UI_RINSE_0_TIMES)
        {
            gx_app_wash_state = APP_NORMAL_MODE_WASH_RINSE;
        }
        else if ((app_userInterface_data_get()->normalMode.wash.rinseSelection == APP_NORMAL_MODE_UI_RINSE_0_TIMES) && ((app_userInterface_data_get()->normalMode.wash.spinSelection != APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN) || (app_userInterface_data_get()->normalMode.wash.superSpinSelection != APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN)))
        {
            gx_app_wash_state = APP_NORMAL_MODE_WASH_SPIN;
        }
        else if ((app_userInterface_data_get()->normalMode.wash.rinseSelection == APP_NORMAL_MODE_UI_RINSE_0_TIMES) && (app_userInterface_data_get()->normalMode.wash.spinSelection == APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN) && (app_userInterface_data_get()->normalMode.wash.superSpinSelection == APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN))
        {
            gx_app_wash_state = APP_NORMAL_MODE_WASH_FINISH;
        }
        else
        {
            // do nothing
        }
    }
}

static void wash_rinse_transition_handle(void)
{
    if (APP_NORMAL_MODE_WASH_RINSE_DONE == app_nm_wash_rinse_state_get())
    {
        if (APP_NORMAL_MODE_UI_OPERATION_WASH_DRY == app_userInterface_data_get()->normalMode.common.operationSelection)
        {
            // NOTE: this is to skip the spin if there is dry selection
            gx_app_wash_state = APP_NORMAL_MODE_WASH_FINISH;
        }
        else if ((app_userInterface_data_get()->normalMode.wash.spinSelection != APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN) || (app_userInterface_data_get()->normalMode.wash.superSpinSelection != APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN))
        {
            gx_app_wash_state = APP_NORMAL_MODE_WASH_SPIN;
        }
        else if ((app_userInterface_data_get()->normalMode.wash.spinSelection == APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN) && (app_userInterface_data_get()->normalMode.wash.superSpinSelection == APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN))
        {
            gx_app_wash_state = APP_NORMAL_MODE_WASH_FINISH;
        }
        else
        {
            // do nothing
        }
    }
}

static void wash_spin_transition_handle(void)
{
    if (APP_NORMAL_MODE_WASH_SPIN_DONE == app_nm_wash_spin_state_get())
    {
        gx_app_wash_state = APP_NORMAL_MODE_WASH_FINISH;
    }
    else
    {
        // do nothing
    }
}

// CAREFUL: this shouldn't be called in the middle of the operation randomly as it resets the data to the start
static void wash_data_refresh(void)
{
    const app_nm_wash_wash_stateTime_t *washData = NULL;
    const app_nm_wash_wash_stateTime_t *soakData = NULL;
    const app_nm_wash_steam_stateTime_t *steamData = NULL;
    const app_nm_wash_rinse_stateTime_t *rinseData = NULL;
    const app_nm_wash_rinse_waterLevel_t *rinseWaterLevel = NULL;
    const app_nm_wash_spin_stateTime_t *spinData = NULL;

    soakData = app_nm_wash_wash_soak_stateData_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    washData = app_nm_wash_wash_stateData_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, app_userInterface_data_get()->normalMode.wash.washSelection, app_userInterface_data_get()->normalMode.wash.soilLevelSelection);

    steamData = app_nm_wash_steam_stateData_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, app_userInterface_data_get()->normalMode.wash.steamSelection, app_nm_wash_steam_waterTemp_map(app_nm_wash_fill_waterTempDuringWashFilling_get()));

    rinseData = app_nm_wash_rinse_stateTime_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, app_userInterface_data_get()->normalMode.wash.extraRinseSelection);
    rinseWaterLevel = app_nm_wash_rinse_stateWaterLevel_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    spinData = app_nm_wash_spin_stateTime_get(app_userInterface_data_get()->normalMode.common.courseSelection);

    if ((NULL != soakData) && (APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR != app_userInterface_data_get()->normalMode.wash.soakSelection))
    {
        app_nm_data_set()->wash.soakTimeMSec = 3600000U * (uint32_t)app_userInterface_data_get()->normalMode.wash.soakSelection;
        for (uint8_t i = app_nm_wash_wash_refillExecludedState_get(); i < APP_NORMAL_MODE_WASH_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->wash.soakStateTimeMSec[i] = soakData->state[i].timeMSec;
            app_nm_data_set()->wash.soakStateWaterLevelLiter[i] = soakData->state[i].waterLevelLiter;
        }
    }
    else
    {
        app_nm_data_set()->wash.soakTimeMSec = 0;
        for (uint8_t i = app_nm_wash_wash_refillExecludedState_get(); i < APP_NORMAL_MODE_WASH_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->wash.soakStateTimeMSec[i] = 0;
            app_nm_data_set()->wash.soakStateWaterLevelLiter[i] = 0;
        }
    }

    if ((NULL != washData) && (APP_NORMAL_MODE_UI_WASH_TIME_0_MIN != app_userInterface_data_get()->normalMode.wash.washSelection))
    {
        for (uint8_t i = app_nm_wash_wash_refillExecludedState_get(); i < APP_NORMAL_MODE_WASH_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->wash.washStateTimeMSec[i] = washData->state[i].timeMSec;
            app_nm_data_set()->wash.washStateWaterLevelLiter[i] = washData->state[i].waterLevelLiter;
        }
    }
    else
    {
        for (uint8_t i = app_nm_wash_wash_refillExecludedState_get(); i < APP_NORMAL_MODE_WASH_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->wash.washStateTimeMSec[i] = 0;
            app_nm_data_set()->wash.washStateWaterLevelLiter[i] = 0;
        }
    }

    if ((NULL != steamData) && (APP_NORMAL_MODE_UI_STEAM_OFF != app_userInterface_data_get()->normalMode.wash.steamSelection))
    {
        for (uint8_t i = (uint8_t)app_nm_wash_steam_state_get(); i < APP_NORMAL_MODE_STEAM_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->wash.steamStateTimeMSec[i] = steamData->state[i];

            if (APP_NORMAL_MODE_WASH_STEAM_SPIN == i)
            {
                app_nm_data_set()->wash.steamStateTimeMSec[APP_NORMAL_MODE_WASH_STEAM_SPIN] += app_nm_wash_steam_preSpinTimeMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            }
        }
    }
    else
    {
        for (uint8_t i = (uint8_t)app_nm_wash_steam_state_get(); i < APP_NORMAL_MODE_STEAM_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->wash.steamStateTimeMSec[i] = 0;
        }
    }

    if ((NULL != rinseData) && (APP_NORMAL_MODE_UI_RINSE_0_TIMES != app_userInterface_data_get()->normalMode.wash.rinseSelection))
    {
        app_nm_data_set()->wash.rinseNum = (uint8_t)app_userInterface_data_get()->normalMode.wash.rinseSelection;

        for (uint8_t i = 0; i < app_nm_data_get()->wash.rinseNum; i++)
        {
            app_nm_data_set()->wash.rinseDrainTimeMSec[i] = rinseData->drainTimeMSec;
            app_nm_data_set()->wash.rinseSpinTimeMSec[i] = rinseData->spinTimeMSec + app_nm_wash_rinse_preSpinTimeMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            app_nm_data_set()->wash.rinseBrakeTimeMSec[i] = rinseData->spinBrakeTimeMSec;
            app_nm_data_set()->wash.rinseFillTimeMSec[i] = rinseData->fillTimeMSec;
            app_nm_data_set()->wash.rinseStirTimeMSec[i] = rinseData->stirTimeMSec;
            app_nm_data_set()->wash.rinseUntangleTimeMSec[i] = rinseData->untangleTimeMSec;
        }

        app_nm_data_set()->wash.rinseUnbalanceFillTimeMSec = 0;
        app_nm_data_set()->wash.rinseUnbalanceStirTimeMSec = 0;
        app_nm_data_set()->wash.rinseUnbalanceDrainTimeMSec = 0;

        if (NULL != rinseWaterLevel)
        {
            app_nm_data_set()->wash.rinseFillWaterLevelLiter = rinseWaterLevel->fillWaterLevelLiter;
        }
        else
        {
            app_nm_data_set()->wash.rinseFillWaterLevelLiter = 0;
        }
    }
    else
    {
        app_nm_data_set()->wash.rinseNum = 0;

        for (uint8_t i = 0; i < APP_NORMAL_MODE_UI_RINSE_SELECTION_NUM; i++)
        {
            app_nm_data_set()->wash.rinseDrainTimeMSec[i] = 0;
            app_nm_data_set()->wash.rinseSpinTimeMSec[i] = 0;
            app_nm_data_set()->wash.rinseBrakeTimeMSec[i] = 0;
            app_nm_data_set()->wash.rinseFillTimeMSec[i] = 0;
            app_nm_data_set()->wash.rinseStirTimeMSec[i] = 0;
            app_nm_data_set()->wash.rinseUntangleTimeMSec[i] = 0;
        }

        app_nm_data_set()->wash.rinseUnbalanceFillTimeMSec = 0;
        app_nm_data_set()->wash.rinseUnbalanceStirTimeMSec = 0;
        app_nm_data_set()->wash.rinseUnbalanceDrainTimeMSec = 0;

        app_nm_data_set()->wash.rinseFillWaterLevelLiter = 0;
    }

    if ((NULL != spinData) && (APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN != app_userInterface_data_get()->normalMode.wash.spinSelection))
    {
        for (uint8_t i = app_nm_wash_spin_state_get(); i < APP_NORMAL_MODE_SPIN_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->wash.spinStateTimeMSec[i] = spinData->state[i];

            if (APP_NORMAL_MODE_WASH_SPIN_SPIN == i)
            {
                app_nm_data_set()->wash.spinStateTimeMSec[APP_NORMAL_MODE_WASH_SPIN_SPIN] += app_nm_wash_spin_preSpinTimeMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            }
        }

        app_nm_data_set()->wash.spinUnbalanceDrainTimeMSec = 0;
        app_nm_data_set()->wash.spinUnbalanceFillTimeMSec = 0;
        app_nm_data_set()->wash.spinUnbalanceStirTimeMSec = 0;
        app_nm_data_set()->wash.spinUnbalanceSuperSpinStirTimeMSec = 0;
    }
    else
    {
        for (uint8_t i = 0; i < APP_NORMAL_MODE_SPIN_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->wash.spinStateTimeMSec[i] = 0;
        }
        app_nm_data_set()->wash.spinUnbalanceDrainTimeMSec = 0;
        app_nm_data_set()->wash.spinUnbalanceFillTimeMSec = 0;
        app_nm_data_set()->wash.spinUnbalanceStirTimeMSec = 0;
        app_nm_data_set()->wash.spinUnbalanceSuperSpinStirTimeMSec = 0;
    }

    app_nm_wash_soakTotalTime_update();
    app_nm_wash_washTotalTime_update();
    app_nm_wash_steamTotalTime_update();
    app_nm_wash_rinseTotalTime_update();
    app_nm_wash_spinTotalTime_update();
    app_nm_wash_tubcleanTotalTime_update();

    app_nm_wash_totalTimeMSec_update();
}
