#include "def.h"

#include "app_standby.h"
#include "app_nm.h"
#include "app_nm_wash.h"
#include "app_nm_wash_rinse.h"
#include "app_nm_wash_spin.h"
#ifdef DRYER_ENABLED
#include "app_nm_dry.h"
#include "app_nm_dry_motor.h"
#endif

#include "app_processor.h"

#include "app_nm_wash_motor.h"

#include "app_memory.h"
#include "app_buzzer.h"

#include "app_fault.h"
#include "app_nm_log.h"
#include "app_fill.h"
#include "app_nm_user_interface.h"
#include "app_nm_wash_fill.h"
#include "app_heater.h"
#include "app_lid.h"

#include "app_nm_tubclean.h"

#include "app_test_mode.h"

#include "handlers/input/inc/handler_off_sw.h"
#include "handlers/input/inc/handler_power_detection.h"
#include "handlers/input/inc/handler_lid_sensor.h"
#include "handlers/output/inc/handler_lock_motor.h"
#include "handlers/input/inc/handler_water_level.h"
#include "handlers/output/inc/handler_drain_valve.h"
#include "handlers/output/inc/handler_buzzer.h"
#include "handlers/input/inc/handler_temp_sensor.h"
#include "handlers/input/inc/handler_acin.h"
#include "handlers/input/inc/handler_mems.h"
#ifdef LINT_FILTER_ENABLED
#include "handlers/input/inc/handler_lint_filter_sensor.h"
#endif
#include "handlers/output/inc/handler_display.h"

#define STANDBY_TIMEOUT_PERIOD_SEC            (600000UL)
#define STANDBY_POWER_INTERRUPTION_DELAY_MSEC (1000UL)

static uint8_t standby_timeout_check(uint32_t period);

static void standby_wash_data_setup(void);
#ifdef DRYER_ENABLED
static void standby_dry_data_setup(void);
#endif

static void standby_fault_check(uint32_t period);

app_processor_state_t app_standby_process(uint32_t period)
{
    handler_offSW_state_t offSW = handler_offSW_state_get();
    static uint32_t gu32_standby_syncCnt = 0;
    app_processor_state_t ret = APP_PROCESSOR_STANDBY_MODE;

    if (MEMORY_SHUTDOWN_ABNORMAL == app_memory_shutdownState_get())
    {
        gu32_standby_syncCnt += period;
        if (gu32_standby_syncCnt >= STANDBY_POWER_INTERRUPTION_DELAY_MSEC)
        {
            gu32_standby_syncCnt = 0;
            ret = APP_PROCESSOR_NORMAL_MODE;
            app_buzzer_tone_set(APP_BUZZER_TONE_START);
        }
    }
    else
    {
        standby_nm_data_handle();

        if ((APP_NORMAL_MODE_UI_SETTING_OK == app_nm_userInterface_startPause_check()) && (APP_NORMAL_MODE_UI_SETTING_OK == app_nm_userInterface_setting_check()))
        {
            ret = APP_PROCESSOR_NORMAL_MODE;
            app_buzzer_tone_set(APP_BUZZER_TONE_START);
        }
        else if (APP_TM_NONE != app_tm_type_get())
        {
            ret = APP_PROCESSOR_TEST_MODE;
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
        }
        else if ((APP_NORMAL_MODE_UI_SETTING_OK == app_nm_userInterface_startPause_check()) && (APP_NORMAL_MODE_UI_SETTING_NOK == app_nm_userInterface_setting_check()))
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else
        {
            // do nothing
        }
    }

    if ((APP_NORMAL_MODE_UI_CHILD_LOCK_OFF == app_userInterface_data_get()->normalMode.common.childLockSelection) && (0U != standby_timeout_check(period)))
    {
        ret = APP_PROCESSOR_FINISH_MODE;
        app_processor_shutdownState_set(MEMORY_SHUTDOWN_NORMAL);
        app_buzzer_tone_set(APP_BUZZER_TONE_SHUTDOWN);
    }
    else if (((APP_NORMAL_MODE_UI_CHILD_LOCK_OFF == app_userInterface_data_get()->normalMode.common.childLockSelection) && (offSW == HANDLER_OFF_SW_PRESSED)) || ((APP_NORMAL_MODE_UI_CHILD_LOCK_ON == app_userInterface_data_get()->normalMode.common.childLockSelection) && (offSW == HANDLER_OFF_SW_LONG_PRESSED)))
    {
        app_processor_shutdownState_set(MEMORY_SHUTDOWN_SKIP);
        ret = APP_PROCESSOR_FINISH_MODE;
        handler_buzzer_tone_set(HANDLER_BUZZER_SHUTDOWN_TONE);
    }
    else if (APP_NORMAL_MODE_LOG_ERROR_NO_ERROR != app_nm_log_error_get())
    {
        ret = APP_PROCESSOR_FAULT_MODE;
        app_buzzer_tone_set(APP_BUZZER_TONE_FAULT);
    }
    else
    {
        // do nothing
    }

    standby_fault_check(period);

    return ret;
}

static uint8_t standby_timeout_check(uint32_t period)
{
    uint8_t ret = 0;

#ifndef STANDBY_TIMEOUT_DISABLE
    static uint32_t standbyTimeoutCnt = 0;

    standbyTimeoutCnt += period;
    if (standbyTimeoutCnt >= STANDBY_TIMEOUT_PERIOD_SEC)
    {
        ret = 1;
    }
#endif

    return ret;
}

void standby_nm_data_handle(void)
{
    standby_wash_data_setup();
#ifdef DRYER_ENABLED
    standby_dry_data_setup();
#endif

    app_nm_data_set()->delayStart = 60 * app_userInterface_data_get()->normalMode.common.delayStartHourSelection;

#ifdef DRYER_ENABLED
    app_nm_data_set()->totalTimeMSec = app_nm_data_set()->wash.washTotalTimeMSec + app_nm_data_set()->dry.dryTotalTimeMSec;
#else
    app_nm_data_set()->totalTimeMSec = app_nm_data_set()->wash.washTotalTimeMSec;
#endif
}
static void standby_wash_data_setup(void)
{
    const app_nm_wash_wash_stateTime_t *washData = NULL;
    const app_nm_wash_wash_stateTime_t *soakData = NULL;
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
static void standby_dry_data_setup(void)
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
#endif

static void standby_fault_check(uint32_t period)
{
    if ((APP_LID_FAILURE == app_lid_faultState_get()) && (app_userInterface_data_get()->normalMode.common.childLockSelection == APP_NORMAL_MODE_UI_CHILD_LOCK_ON))
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_LID_OPENING_FAILURE);
    }
    else if (0U != handler_lockMotor_error_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_LID_LOCK_FAILURE);
    }
    else if (handler_display_error_check() != 0U)
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_DISPLAY_DISCONNECTED_FAILURE);
    }
    else if (HANDLER_DANGEROUS_UNDER_VOLTAGE == handler_acin_state_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_LOW_POWER_FAILURE);
    }
    else if (APP_MOTOR_RESET_FAILURE == app_motor_resetState_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_MOTOR_FAILURE);
    }
    else if (OVERFLOW_DANGEROUS == app_fill_overFlowState_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_WASH_LOG_ERROR_ABNORMAL_WATER_SUPPLY_OVERFLOW);
    }
    else if (0U != handler_drainValve_error_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_DRAIN_VALVE_MOTOR_FAILURE);
    }
    else if (0 != handler_waterLevel_error_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_ABNORMAL_WATER_LEVEL_SENSOR_VALUE);
    }
    else if ((0U != handler_ambientTempSensor_error_get()) || (0U != handler_waterTempSensor_error_get()) || (0U != handler_airATempSensor_error_get()) || (0U != handler_airBTempSensor_error_get()))
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_ABNORMAL_THERMISTOR_VALUE);
    }
    else if (0U != handler_mems_error_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_MEMS_SENSOR_FAILURE);
    }
    else if (HANDLER_MOTOR_ERROR_NO_COM == handler_motor_error_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_DRIVER_NO_COMMUNICATION);
    }
    else if (HANDLER_MOTOR_NO_ERROR != handler_motor_error_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_MOTOR_FAILURE);
    }
#ifdef DRYER_ENABLED
    else if (0U != app_heater_overCurrentFault_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_DRY_LOG_ERROR_HEATER_OVERCURRENT_FAILURE);
    }
    else if (0U != app_heater_tempSensorFault_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_DRY_LOG_ERROR_HEATER_TEMP_SENSOR_FAILURE);
    }
#endif
    else if (APP_MOTOR_OPERATION_STATE_FAULT == app_motor_operationState_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_MOTOR_FAILURE);
    }
    else
    {
        // do nothing
    }
}
