#include "def.h"

#include "app_setup.h"
#include "app_nm.h"
#include "app_nm_wash.h"
#include "app_nm_wash_rinse.h"
#include "app_nm_wash_spin.h"
#ifdef DRYER_ENABLED
#include "app_nm_dry.h"
#include "app_nm_dry_motor.h"
#endif
#include "app_nm_init.h"
#include "app_nm_wash_steam_table.h"

#include "app_nm_wash_motor.h"
#include "app_buzzer.h"

#include "app_nm_wash_fill.h"

#include "app_nm_tubclean.h"

#include "app_processor.h"

#include "handlers/input/inc/handler_off_sw.h"

#define SETUP_DELAY_MSEC (2400UL)

static void setup_wash_data_setup(void);

#ifdef DRYER_ENABLED
static void setup_dry_data_setup(void);
#endif

static void setup_executionStates_restore(void);

app_processor_state_t app_setup_process(uint32_t period)
{
    app_processor_state_t ret = APP_PROCESSOR_SETUP_MODE;
    static uint32_t gu32_setup_syncCnt = 0U;

    if (MEMORY_SHUTDOWN_ABNORMAL == app_memory_shutdownState_get())
    {
        setup_executionStates_restore();
    }

    setup_nm_data_handle();

    gu32_setup_syncCnt += period;
    if (gu32_setup_syncCnt >= SETUP_DELAY_MSEC)
    {
        gu32_setup_syncCnt = 0;
        ret = APP_PROCESSOR_STANDBY_MODE;
    }

    return ret;
}

static void setup_executionStates_restore(void)
{
    if (app_userInterface_data_set()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF)
    {
        app_nm_state_set((app_nm_state_t)app_memory_data_get(MEMORY_LAYOUT_NM_STATE_LOCATION));
        app_nm_init_state_set((app_nm_init_state_t)app_memory_data_get(MEMORY_LAYOUT_NM_INIT_STATE_LOCATION));
#ifdef LINT_FILTER_ENABLED
        app_nm_init_lintFilterCheckState_set(LINT_FILTER_CHECK_INIT);
#endif
        app_nm_wash_state_set((app_nm_wash_state_t)app_memory_data_get(MEMORY_LAYOUT_NM_WASH_STATE_LOCATION));
        app_nm_wash_setup_state_set((app_nm_wash_setup_state_t)app_memory_data_get(MEMORY_LAYOUT_WASH_SETUP_STATE_LOCATION));
        app_nm_wash_soak_state_set((app_nm_wash_soak_state_t)app_memory_data_get(MEMORY_LAYOUT_WASH_SOAK_STATE_LOCATION));
        app_nm_wash_wash_state_set((app_nm_wash_wash_state_t)app_memory_data_get(MEMORY_LAYOUT_WASH_WASH_STATE_LOCATION));
        app_nm_wash_steam_state_set((app_nm_wash_steam_state_t)app_memory_data_get(MEMORY_LAYOUT_WASH_STEAM_STATE_LOCATION));
        app_nm_wash_rinse_state_set((app_nm_wash_rinse_state_t)app_memory_data_get(MEMORY_LAYOUT_WASH_RINSE_STATE_LOCATION));
        app_nm_wash_spin_state_set((app_nm_wash_spin_state_t)app_memory_data_get(MEMORY_LAYOUT_WASH_SPIN_STATE_LOCATION));
#ifdef DRYER_ENABLED
        app_nm_dry_state_set((app_nm_dry_state_t)app_memory_data_get(MEMORY_LAYOUT_NM_DRY_STATE_LOCATION));
        app_nm_dry_preSetup_state_set((app_nm_dry_preSetup_state_t)app_memory_data_get(MEMORY_LAYOUT_DRY_PRESETUP_STATE_LOCATION));
        app_nm_dry_setup_state_set((app_nm_dry_setup_state_t)app_memory_data_get(MEMORY_LAYOUT_DRY_SETUP_STATE_LOCATION));
        app_nm_dry_operation_state_set((app_nm_dry_operation_state_t)app_memory_data_get(MEMORY_LAYOUT_DRY_OPERATION_STATE_LOCATION));
#endif
    }
    else
    {
        app_nm_state_set((app_nm_state_t)app_memory_data_get(MEMORY_LAYOUT_NM_STATE_LOCATION));
        app_nm_init_state_set((app_nm_init_state_t)app_memory_data_get(MEMORY_LAYOUT_NM_INIT_STATE_LOCATION));
        app_nm_tubclean_state_set((app_nm_tubclean_state_t)app_memory_data_get(MEMORY_LAYOUT_TUBCLEAN_STATE_LOCATION));
    }
}

void setup_nm_data_handle(void)
{
    setup_wash_data_setup();
#ifdef DRYER_ENABLED
    setup_dry_data_setup();
#endif

    app_nm_data_set()->delayStart = 60 * app_userInterface_data_get()->normalMode.common.delayStartHourSelection;

#ifdef DRYER_ENABLED
    app_nm_data_set()->totalTimeMSec = app_nm_data_set()->wash.washTotalTimeMSec + app_nm_data_set()->dry.dryTotalTimeMSec;
#else
    app_nm_data_set()->totalTimeMSec = app_nm_data_set()->wash.washTotalTimeMSec;
#endif
}

// CAREFUL: this shouldn't be called in the middle of the operation randomly as it resets the data to the start
static void setup_wash_data_setup(void)
{
    const app_nm_wash_wash_stateTime_t *soakData = NULL;
    const app_nm_wash_wash_stateTime_t *washData = NULL;
    const app_nm_wash_steam_stateTime_t *steamData = NULL;
    const app_nm_wash_rinse_stateTime_t *rinseData = NULL;
    const app_nm_wash_rinse_waterLevel_t *rinseWaterLevel = NULL;
    const app_nm_wash_spin_stateTime_t *spinData = NULL;
    const uint32_t *tubcleanData = NULL;

    soakData = app_nm_wash_wash_soak_stateData_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    washData = app_nm_wash_wash_stateData_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, app_userInterface_data_get()->normalMode.wash.washSelection, app_userInterface_data_get()->normalMode.wash.soilLevelSelection);

    steamData = app_nm_wash_steam_stateData_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, app_userInterface_data_get()->normalMode.wash.steamSelection, app_nm_wash_steam_waterTemp_map(app_nm_wash_fill_waterTempDuringWashFilling_get()));

    rinseData = app_nm_wash_rinse_stateTime_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, app_userInterface_data_get()->normalMode.wash.extraRinseSelection);
    rinseWaterLevel = app_nm_wash_rinse_stateWaterLevel_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    spinData = app_nm_wash_spin_stateTime_get(app_userInterface_data_get()->normalMode.common.courseSelection);

    tubcleanData = app_nm_tubclean_timeTable_get();

    if ((NULL != soakData) && (APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR != app_userInterface_data_get()->normalMode.wash.soakSelection))
    {
        for (uint8_t i = (uint8_t)app_nm_wash_soak_state_get(); i < APP_NORMAL_MODE_SOAK_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->wash.soakTimeMSec = 3600000U * (uint32_t)app_userInterface_data_get()->normalMode.wash.soakSelection;
        }

        for (uint8_t i = (uint8_t)app_nm_wash_wash_refillExecludedState_get(); i < APP_NORMAL_MODE_WASH_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->wash.soakStateTimeMSec[i] = soakData->state[i].timeMSec;
            app_nm_data_set()->wash.soakStateWaterLevelLiter[i] = soakData->state[i].waterLevelLiter;
        }

        if (MEMORY_SHUTDOWN_ABNORMAL == app_memory_shutdownState_get() && (app_nm_wash_soak_state_get() < APP_NORMAL_MODE_SOAK_STATES_WITH_DATA))
        {
            app_nm_data_set()->wash.soakTimeMSec = ((((uint32_t)app_memory_data_get(MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_HIGH_LOCATION)) << 8) | ((uint32_t)app_memory_data_get(MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_LOW_LOCATION))) * 60000U;
        }
    }
    else
    {
        app_nm_data_set()->wash.soakTimeMSec = 0;
        for (uint8_t i = (uint8_t)app_nm_wash_wash_refillExecludedState_get(); i < APP_NORMAL_MODE_WASH_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->wash.soakStateTimeMSec[i] = 0;
            app_nm_data_set()->wash.soakStateWaterLevelLiter[i] = 0;
        }
    }

    if ((NULL != washData) && (APP_NORMAL_MODE_UI_WASH_TIME_0_MIN != app_userInterface_data_get()->normalMode.wash.washSelection))
    {
        for (uint8_t i = (uint8_t)app_nm_wash_wash_refillExecludedState_get(); i < APP_NORMAL_MODE_WASH_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->wash.washStateTimeMSec[i] = washData->state[i].timeMSec;
            app_nm_data_set()->wash.washStateWaterLevelLiter[i] = washData->state[i].waterLevelLiter;
        }

        if (MEMORY_SHUTDOWN_ABNORMAL == app_memory_shutdownState_get() && (app_nm_wash_wash_refillExecludedState_get() < APP_NORMAL_MODE_WASH_STATES_WITH_DATA))
        {
            app_nm_data_set()->wash.washStateTimeMSec[app_nm_wash_wash_refillExecludedState_get()] = ((((uint32_t)app_memory_data_get(MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_HIGH_LOCATION)) << 8) | ((uint32_t)app_memory_data_get(MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_LOW_LOCATION))) * 60000U;
        }
    }
    else
    {
        for (uint8_t i = (uint8_t)app_nm_wash_wash_refillExecludedState_get(); i < APP_NORMAL_MODE_WASH_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->wash.washStateTimeMSec[i] = 0;
            app_nm_data_set()->wash.washStateWaterLevelLiter[i] = 0;
        }
    }

    if ((NULL != steamData) && (APP_NORMAL_MODE_UI_WASH_TIME_0_MIN != app_userInterface_data_get()->normalMode.wash.washSelection))
    {
        for (uint8_t i = (uint8_t)app_nm_wash_steam_state_get(); i < APP_NORMAL_MODE_STEAM_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->wash.steamStateTimeMSec[i] = steamData->state[i];

            if (APP_NORMAL_MODE_WASH_STEAM_SPIN == i)
            {
                app_nm_data_set()->wash.steamStateTimeMSec[APP_NORMAL_MODE_WASH_STEAM_SPIN] += app_nm_wash_steam_preSpinTimeMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            }
        }

        if (MEMORY_SHUTDOWN_ABNORMAL == app_memory_shutdownState_get() && (app_nm_wash_steam_state_get() < APP_NORMAL_MODE_STEAM_STATES_WITH_DATA))
        {
            app_nm_data_set()->wash.steamStateTimeMSec[app_nm_wash_steam_state_get()] = ((((uint32_t)app_memory_data_get(MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_HIGH_LOCATION)) << 8) | ((uint32_t)app_memory_data_get(MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_LOW_LOCATION))) * 60000U;
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

        for (uint8_t i = 0U; i < (uint8_t)APP_NORMAL_MODE_UI_RINSE_SELECTION_NUM; i++)
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
        for (uint8_t i = (uint8_t)app_nm_wash_spin_state_get(); i < APP_NORMAL_MODE_SPIN_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->wash.spinStateTimeMSec[i] = spinData->state[i];

            if ((uint8_t)APP_NORMAL_MODE_WASH_SPIN_SPIN == i)
            {
                app_nm_data_set()->wash.spinStateTimeMSec[APP_NORMAL_MODE_WASH_SPIN_SPIN] += app_nm_wash_spin_preSpinTimeMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            }
        }

        if (MEMORY_SHUTDOWN_ABNORMAL == app_memory_shutdownState_get() && (app_nm_wash_spin_state_get() < APP_NORMAL_MODE_SPIN_STATES_WITH_DATA))
        {
            app_nm_data_set()->wash.spinStateTimeMSec[app_nm_wash_spin_state_get()] = ((((uint32_t)app_memory_data_get(MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_HIGH_LOCATION)) << 8) | ((uint32_t)app_memory_data_get(MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_LOW_LOCATION))) * 60000U;
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

    if ((NULL != tubcleanData) && (app_userInterface_data_get()->normalMode.common.tubcleanSelection != APP_NORMAL_MODE_UI_TUBCLEAN_OFF))
    {
        if (MEMORY_SHUTDOWN_ABNORMAL == app_memory_shutdownState_get())
        {
            app_nm_data_set()->wash.tubcleanNum = app_memory_data_get(MEMORY_LAYOUT_TUBCLEAN_NUM_LOCATION);
        }
        else
        {
            app_nm_data_set()->wash.tubcleanNum = APP_NM_TUBCLEAN_REPETITION_COUNT;
        }

        for (uint8_t i = 0; i < app_nm_data_get()->wash.tubcleanNum; i++)
        {
            for (uint8_t j = app_nm_tubclean_state_get(); j < APP_NORMAL_MODE_TUBCLEAN_STATES_WITH_DATA; j++)
            {
                app_nm_data_set()->wash.tubcleanStateTimeMSec[j][i] = tubcleanData[j];
            }
        }

        app_nm_data_set()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_FILL][0] = 0;
        app_nm_data_set()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_MH][0] = 0;
        app_nm_data_set()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_MD][0] = 0;
        app_nm_data_set()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_SOAK][0] = 0;
    }
    else
    {
        app_nm_data_set()->wash.tubcleanNum = 0;
        for (uint8_t i = 0; i < APP_NM_TUBCLEAN_REPETITION_COUNT; i++)
        {
            for (uint8_t j = app_nm_tubclean_state_get(); j < APP_NORMAL_MODE_TUBCLEAN_STATES_WITH_DATA; j++)
            {
                app_nm_data_set()->wash.tubcleanStateTimeMSec[j][i] = 0;
            }
        }
    }

    app_nm_wash_soakTotalTime_update();
    app_nm_wash_washTotalTime_update();
    app_nm_wash_steamTotalTime_update();
    app_nm_wash_rinseTotalTime_update();
    app_nm_wash_spinTotalTime_update();
    app_nm_wash_tubcleanTotalTime_update();

    app_nm_wash_totalTimeMSec_update();

    app_nm_wash_currentUI_save();
}

#ifdef DRYER_ENABLED
static void setup_dry_data_setup(void)
{
    const app_dry_operation_timeTable_t *operationData = NULL;
    const app_dry_setup_timeTable_t *preSetupData = NULL;

    preSetupData = app_nm_dry_preSetup_stateTotalTimeMSec_get(app_userInterface_data_get()->normalMode.dry.dryTimeSelection);

    operationData = app_nm_dry_operation_stateTotalMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel, app_userInterface_data_get()->normalMode.dry.dryIntensitySelection);

    if (NULL != preSetupData)
    {
        for (uint8_t i = (uint8_t)app_nm_dry_preSetup_state_get(); i < APP_NORMAL_MODE_DRY_PRE_SETUP_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->dry.preSetupTimeMSec[i] = preSetupData->stateTotalTimeMSec[i];

            if (APP_NORMAL_MODE_DRY_PRE_SETUP_SPIN == i)
            {
                app_nm_data_set()->dry.preSetupTimeMSec[APP_NORMAL_MODE_DRY_PRE_SETUP_SPIN] += app_nm_dry_preSpinStateTotalTimeMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel);
            }
        }

        if (MEMORY_SHUTDOWN_ABNORMAL == app_memory_shutdownState_get() && (app_nm_dry_preSetup_state_get() < APP_NORMAL_MODE_DRY_PRE_SETUP_STATES_WITH_DATA))
        {
            app_nm_data_set()->dry.preSetupTimeMSec[app_nm_dry_preSetup_state_get()] = ((((uint32_t)app_memory_data_get(MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_HIGH_LOCATION)) << 8) | ((uint32_t)app_memory_data_get(MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_LOW_LOCATION))) * 60000U;

            if (APP_NORMAL_MODE_DRY_PRE_SETUP_SPIN == app_nm_dry_preSetup_state_get())
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
        for (uint8_t i = (uint8_t)app_nm_dry_operation_state_get(); i < APP_NORMAL_MODE_DRY_OPERATION_STATES_WITH_DATA; i++)
        {
            app_nm_data_set()->dry.operationTimeMSec[i] = operationData->stateTotalTimeMSec[i];

            if (APP_NORMAL_MODE_DRY_OPERATION_FINAL_SPIN == i)
            {
                app_nm_data_set()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_FINAL_SPIN] += app_nm_dry_preSpinStateTotalTimeMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel);
            }
        }

        if (MEMORY_SHUTDOWN_ABNORMAL == app_memory_shutdownState_get() && (app_nm_dry_operation_state_get() < APP_NORMAL_MODE_DRY_OPERATION_STATES_WITH_DATA))
        {
            app_nm_data_set()->dry.operationTimeMSec[app_nm_dry_operation_state_get()] = ((((uint32_t)app_memory_data_get(MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_HIGH_LOCATION)) << 8) | ((uint32_t)app_memory_data_get(MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_LOW_LOCATION))) * 60000U;

            if (APP_NORMAL_MODE_DRY_OPERATION_FINAL_SPIN == app_nm_dry_operation_state_get())
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
#endif
