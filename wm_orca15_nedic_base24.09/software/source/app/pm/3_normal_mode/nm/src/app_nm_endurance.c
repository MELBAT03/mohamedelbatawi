#include "def.h"
#include "app_nm_data_display.h"
#include "app_nm_user_interface.h"

#include "app_nm.h"
#include "app_buzzer.h"
#include "app_memory.h"
#include "app_display.h"

#include "app_user_interface.h"

#include "app_nm_wash.h"
#include "app_nm_wash_rinse.h"
#include "app_nm_wash_spin.h"
#ifdef DRYER_ENABLED
#include "app_nm_dry.h"
#include "app_nm_dry_motor.h"
#endif

#include "app_nm_wash_motor.h"
#include "app_nm_wash_fill.h"

#define ENDURANCE_BUTTON_PRESS_TIME_MSEC (5000U)

static void endurance_ui_restore(void);
static void endurance_data_handle(void);
static void endurance_wash_data_setup(void);
#ifdef DRYER_ENABLED
static void endurance_dry_data_setup(void);
#endif
static void endurance_ui_save(void);

static app_nm_userInterface_data_t gx_endurance_savedUI;

static uint16_t gu16_endurance_cycleCnt = 0;

void app_nm_endurance_restart(void)
{
    gu16_endurance_cycleCnt = app_memory_enduranceCnt_get();
    gu16_endurance_cycleCnt++;
    app_memory_enduranceCnt_set(gu16_endurance_cycleCnt);

    app_nm_reset();

    endurance_ui_restore();
    endurance_data_handle();
}

void app_nm_endurance_entry_check(uint32_t period)
{
    static uint32_t enduranceCnt = 0;

    if (APP_NORMAL_MODE_PAUSE != app_nm_state_get())
    {
        if (HANDLER_KEY_RELEASED != app_userInterface_keyState_get()->spinKeyState)
        {
            enduranceCnt += period;
            if (enduranceCnt >= ENDURANCE_BUTTON_PRESS_TIME_MSEC)
            {
                enduranceCnt = 0;
                app_userInterface_data_set()->normalMode.common.cycleTypeSelection = APP_NORMAL_MODE_UI_CONTINUOUS_CYCLE;
                endurance_ui_save();
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
            }
        }
    }
}

void app_nm_endurance_display_handle(void)
{
    app_display_ssdSymbols_put(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_d_SYMBOL, SSD_3_SYMBOL);
}

app_nm_userInterface_data_t *app_nm_endurance_ui_get(void)
{
    return &gx_endurance_savedUI;
}

uint16_t app_nm_endurance_cycleCnt_get(void)
{
    return gu16_endurance_cycleCnt;
}

static void endurance_ui_restore(void)
{
    app_userInterface_data_set()->normalMode = gx_endurance_savedUI;
}

static void endurance_data_handle(void)
{
    endurance_wash_data_setup();
#ifdef DRYER_ENABLED
    endurance_dry_data_setup();
#endif

    app_nm_data_set()->delayStart = 60 * gx_endurance_savedUI.common.delayStartHourSelection;

#ifdef DRYER_ENABLED
    app_nm_data_set()->totalTimeMSec = app_nm_data_get()->wash.washTotalTimeMSec + app_nm_data_get()->dry.dryTotalTimeMSec;
#else
    app_nm_data_set()->totalTimeMSec = app_nm_data_get()->wash.washTotalTimeMSec;
#endif
}

static void endurance_wash_data_setup(void)
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

    app_nm_wash_totalTimeMSec_update();

    app_nm_wash_currentUI_save();
}

#ifdef DRYER_ENABLED
static void endurance_dry_data_setup(void)
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
}
#endif

static void endurance_ui_save(void)
{
    gx_endurance_savedUI = app_userInterface_data_get()->normalMode;
}
