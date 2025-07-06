#include "def.h"
#include "os.h"

#include "app_processor.h"
#include "app_motor.h"
#include "app_lid.h"
#include "app_fill.h"
#include "app_load.h"
#include "app_heater.h"
#include "app_user_interface.h"

#include "app_startup.h"
#include "app_setup.h"
#include "app_standby.h"
#include "app_nm.h"
#include "app_test_mode.h"
#include "app_fault.h"
#include "app_finish.h"

#include "app_exit_shutdown.h"

#include "app_nm_init.h"
#include "app_memory.h"
#include "app_nm_log.h"

#include "app_nm_wash.h"
#include "app_nm_wash_fill.h"
#include "app_nm_wash_heater.h"
#include "app_nm_tubclean.h"

#ifdef DRYER_ENABLED
#include "app_nm_dry.h"
#include "app_nm_dry_motor.h"
#include "app_nm_dry_heater.h"
#endif

#include "app_nm_data_display.h"

#include "handlers/output/inc/handler_buzzer.h"
#include "handlers/output/inc/handler_lock_motor.h"
#include "handlers/output/inc/handler_drain_pump.h"
#include "handlers/input/inc/handler_power_detection.h"
#include "handlers/input/inc/handler_water_level.h"
#include "handlers/input/inc/handler_detergent_sensor.h"
#include "handlers/input/inc/handler_softener_sensor.h"
#include "handlers/input/inc/handler_mems.h"
#ifdef LINT_FILTER_ENABLED
#include "handlers/input/inc/handler_lint_filter_sensor.h"
#endif
#include "handlers/input/inc/handler_off_sw.h"
#include "handlers/input/inc/handler_temp_sensor.h"
#include "handlers/input/inc/handler_acin.h"
#include "handlers/input/inc/handler_lid_sensor.h"
#include "handlers/output/inc/handler_drain_valve.h"
#include "handlers/output/inc/handler_heater.h"
#include "handlers/output/inc/handler_detergent_pump.h"
#include "handlers/output/inc/handler_softener_pump.h"
#ifdef BLOWER_ENABLED
#include "handlers/output/inc/handler_blower.h"
#endif

static app_processor_state_t processor_done_handler(uint32_t period);
static void processor_serial_log(void);

static app_processor_state_t gx_processor_state = APP_PROCESSOR_STARTUP_MODE;
static app_processor_state_t gx_processor_previousState = APP_PROCESSOR_STARTUP_MODE;
static app_processor_state_t gx_processor_bufferedState = APP_PROCESSOR_STARTUP_MODE;
static app_memory_shutdownState_t gx_processor_shutdownState = MEMORY_SHUTDOWN_NONE;

static uint8_t appEntry_flag = 0;

// TODO: add tasks contract for the whole system
void app_processor_update(uint32_t period)
{
    static app_processor_state_t (*const handlers[])(uint32_t period) = {
        app_startup_process,
        app_setup_process,
        app_standby_process,
        app_nm_process,
        app_testMode_process,
        app_fault_process,
        app_finish_process,
        processor_done_handler};

    appEntry_flag = 1;

    if (handlers[gx_processor_state] != NULL)
    {
        gx_processor_bufferedState = gx_processor_state;
        gx_processor_state = handlers[gx_processor_state](period);
        if (gx_processor_bufferedState != gx_processor_state)
        {
            gx_processor_previousState = gx_processor_bufferedState;
        }
    }

    processor_serial_log();
}

uint8_t app_processor_entryFlag_get(void)
{
    return appEntry_flag;
}

app_processor_state_t app_processor_state_get(void)
{
    return gx_processor_state;
}
void app_processor_state_set(app_processor_state_t value)
{
    gx_processor_state = value;
}

app_processor_state_t app_processor_previousState_get(void)
{
    return gx_processor_previousState;
}

app_memory_shutdownState_t app_processor_shutdownState_get(void)
{
    return gx_processor_shutdownState;
}

void app_processor_shutdownState_set(app_memory_shutdownState_t state)
{
    gx_processor_shutdownState = state;
}

static app_processor_state_t processor_done_handler(uint32_t period)
{
    app_exit_shutdown_request(NULL);

    return APP_PROCESSOR_DONE_MODE;
}

// NOTE: used for logging
static void processor_serial_log(void)
{
    // log for all system states
#ifdef DRYER_ENABLED
    LOG('P', 'v', "P:%d T:%d WT:%d DT:%d nm[%d int:%d wsh[%d stup:%d sk:%d wsh:%d rin:%d[un:%d] spn:%d[un:%d]] - dry[%d pre:%d stup:%d op:%d]] - cltch[stat:%d mode:%d] - motor[stat:%d] - flt[err:%d cLck:%d] - [dd:%d] - [OFF:%d]", app_processor_state_get(), app_nm_data_get()->totalTimeMSec, app_nm_data_get()->wash.washTotalTimeMSec, app_nm_data_get()->dry.dryTotalTimeMSec, app_nm_state_get(), app_nm_init_state_get(), app_nm_wash_state_get(), app_nm_wash_setup_state_get(), app_nm_wash_soak_state_get(), app_nm_wash_wash_state_get(), app_nm_wash_rinse_state_get(), app_nm_wash_rinse_unbalanceState_get(), app_nm_wash_spin_state_get(), app_nm_wash_spin_unbalanceState_get(), app_nm_dry_state_get(), app_nm_dry_preSetup_state_get(), app_nm_dry_setup_state_get(), app_nm_dry_operation_state_get(), app_motor_data_get()->clutchState, app_motor_data_get()->clutchMode, app_motor_operationState_get(), app_nm_log_error_get(), app_fault_childLockState_get(), app_nm_userInterface_dataDisplayState_get(), handler_offSW_state_get());
#else
    LOG('P', 'v', "P:%d T:%d WT:%d nm[%d int:%d wsh[%d stup:%d sk:%d wsh:%d rin:%d[un:%d] spn:%d[un:%d]] - cltch[stat:%d mode:%d] - motor[stat:%d] - flt[err:%d cLck:%d] - [dd:%d] - [OFF:%d]", app_processor_state_get(), app_nm_data_get()->totalTimeMSec, app_nm_data_get()->wash.washTotalTimeMSec, app_nm_state_get(), app_nm_init_state_get(), app_nm_wash_state_get(), app_nm_wash_setup_state_get(), app_nm_wash_soak_state_get(), app_nm_wash_wash_state_get(), app_nm_wash_rinse_state_get(), app_nm_wash_rinse_unbalanceState_get(), app_nm_wash_spin_state_get(), app_nm_wash_spin_unbalanceState_get(), app_motor_data_get()->clutchState, app_motor_data_get()->clutchMode, app_motor_operationState_get(), app_nm_log_error_get(), app_fault_childLockState_get(), app_nm_userInterface_dataDisplayState_get(), handler_offSW_state_get());
#endif

    LOG('K', 'v', "[SK] P:%d T:%d WT:%d SKT:%d sTime:%d stupT:%d stupWL:%d m0T:%d m0WL:%d mhT:%d mdT:%d m1T:%d m1WL:%d m2T:%d m3T:%d m3WL:%d m4T:%d m4WL:%d HetingT:%d stirT:%d stirWL:%d UT:%d", app_processor_state_get(), app_nm_data_get()->totalTimeMSec, app_nm_data_get()->wash.washTotalTimeMSec, app_nm_data_get()->wash.soakTotalTimeMSec, app_nm_data_get()->wash.soakTimeMSec, app_nm_data_get()->wash.soakStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_SETUP], app_nm_data_get()->wash.soakStateWaterLevelLiter[APP_NORMAL_MODE_WASH_WASH_SETUP], app_nm_data_get()->wash.soakStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_M0], app_nm_data_get()->wash.soakStateWaterLevelLiter[APP_NORMAL_MODE_WASH_WASH_M0], app_nm_data_get()->wash.soakStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_MH], app_nm_data_get()->wash.soakStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_MD], app_nm_data_get()->wash.soakStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_M1], app_nm_data_get()->wash.soakStateWaterLevelLiter[APP_NORMAL_MODE_WASH_WASH_M1], app_nm_data_get()->wash.soakStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_M2], app_nm_data_get()->wash.soakStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_M3], app_nm_data_get()->wash.soakStateWaterLevelLiter[APP_NORMAL_MODE_WASH_WASH_M3], app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_M4], app_nm_data_get()->wash.washStateWaterLevelLiter[APP_NORMAL_MODE_WASH_WASH_M4], app_nm_data_get()->wash.soakStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_HEATING], app_nm_data_get()->wash.soakStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_STIR], app_nm_data_get()->wash.soakStateWaterLevelLiter[APP_NORMAL_MODE_WASH_WASH_STIR], app_nm_data_get()->wash.soakStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_UNTANGLE]);

    LOG('W', 'v', "[W] P:%d T:%d WT:%d WWT:%d stupT:%d stupWL:%d m0T:%d m0WL:%d mhT:%d mdT:%d m1T:%d m1WL:%d m2T:%d m3T:%d m3WL:%d m4T:%d m4WL:%d HetingT:%d stirT:%d stirWL:%d unT:%d", app_processor_state_get(), app_nm_data_get()->totalTimeMSec, app_nm_data_get()->wash.washTotalTimeMSec, app_nm_data_get()->wash.washWashTotalTimeMSec, app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_SETUP], app_nm_data_get()->wash.washStateWaterLevelLiter[APP_NORMAL_MODE_WASH_WASH_SETUP], app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_M0], app_nm_data_get()->wash.washStateWaterLevelLiter[APP_NORMAL_MODE_WASH_WASH_M0], app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_MH], app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_MD], app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_M1], app_nm_data_get()->wash.washStateWaterLevelLiter[APP_NORMAL_MODE_WASH_WASH_M1], app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_M2], app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_M3], app_nm_data_get()->wash.washStateWaterLevelLiter[APP_NORMAL_MODE_WASH_WASH_M3], app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_M4], app_nm_data_get()->wash.washStateWaterLevelLiter[APP_NORMAL_MODE_WASH_WASH_M4], app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_HEATING], app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_STIR], app_nm_data_get()->wash.washStateWaterLevelLiter[APP_NORMAL_MODE_WASH_WASH_STIR], app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_UNTANGLE]);

    LOG('T', 'v', "[T] P:%d[%d] T:%d WT:%d TT:%d dT:%d sT:%d oT:%d", app_processor_state_get(), app_nm_wash_steam_state_get(), app_nm_data_get()->totalTimeMSec, app_nm_data_get()->wash.washTotalTimeMSec, app_nm_data_get()->wash.steamTotalTimeMSec, app_nm_data_get()->wash.steamStateTimeMSec[APP_NORMAL_MODE_WASH_STEAM_DRAIN], app_nm_data_get()->wash.steamStateTimeMSec[APP_NORMAL_MODE_WASH_STEAM_SPIN], app_nm_data_get()->wash.steamStateTimeMSec[APP_NORMAL_MODE_WASH_STEAM_STEAM]);

    LOG('R', 'v', "[R] P:%d T:%d WT:%d RT:%d #:%d lvl:%d dr[0:%d 1:%d 2:%d 3:%d] sp[0:%d 1:%d 2:%d 3:%d] br[0:%d 1:%d 2:%d 3:%d] f[0:%d 1:%d 2:%d 3:%d] st[0:%d 1:%d 2:%d 3:%d] ut[0:%d 1:%d 2:%d 3:%d] UF:%d US:%d UD:%d", app_processor_state_get(), app_nm_data_get()->totalTimeMSec, app_nm_data_get()->wash.washTotalTimeMSec, app_nm_data_get()->wash.rinseTotalTimeMSec, app_nm_data_get()->wash.rinseNum, app_nm_data_get()->wash.rinseFillWaterLevelLiter, app_nm_data_get()->wash.rinseDrainTimeMSec[0], app_nm_data_get()->wash.rinseDrainTimeMSec[1], app_nm_data_get()->wash.rinseDrainTimeMSec[2], app_nm_data_get()->wash.rinseDrainTimeMSec[3], app_nm_data_get()->wash.rinseSpinTimeMSec[0], app_nm_data_get()->wash.rinseSpinTimeMSec[1], app_nm_data_get()->wash.rinseSpinTimeMSec[2], app_nm_data_get()->wash.rinseSpinTimeMSec[3], app_nm_data_get()->wash.rinseBrakeTimeMSec[0], app_nm_data_get()->wash.rinseBrakeTimeMSec[1], app_nm_data_get()->wash.rinseBrakeTimeMSec[2], app_nm_data_get()->wash.rinseBrakeTimeMSec[3], app_nm_data_get()->wash.rinseFillTimeMSec[0], app_nm_data_get()->wash.rinseFillTimeMSec[1], app_nm_data_get()->wash.rinseFillTimeMSec[2], app_nm_data_get()->wash.rinseFillTimeMSec[3], app_nm_data_get()->wash.rinseStirTimeMSec[0], app_nm_data_get()->wash.rinseStirTimeMSec[1], app_nm_data_get()->wash.rinseStirTimeMSec[2], app_nm_data_get()->wash.rinseStirTimeMSec[3], app_nm_data_get()->wash.rinseUntangleTimeMSec[0], app_nm_data_get()->wash.rinseUntangleTimeMSec[1], app_nm_data_get()->wash.rinseUntangleTimeMSec[2], app_nm_data_get()->wash.rinseUntangleTimeMSec[3], app_nm_data_get()->wash.rinseUnbalanceFillTimeMSec, app_nm_data_get()->wash.rinseUnbalanceStirTimeMSec, app_nm_data_get()->wash.rinseUnbalanceDrainTimeMSec);

    LOG('S', 'v', "[S] P:%d T:%d WT:%d ST:%d dr:%d s:%d ss:%d brake:%d aWrinkle:%d", app_processor_state_get(), app_nm_data_get()->totalTimeMSec, app_nm_data_get()->wash.washTotalTimeMSec, app_nm_data_get()->wash.spinTotalTimeMSec, app_nm_data_get()->wash.spinStateTimeMSec[1], app_nm_data_get()->wash.spinStateTimeMSec[2], app_nm_data_get()->wash.spinStateTimeMSec[3], app_nm_data_get()->wash.spinStateTimeMSec[4], app_nm_data_get()->wash.spinStateTimeMSec[5]);

#ifdef DRYER_ENABLED
    LOG('D', 'v', "[D] P:%d[%d %d %d %d] T:%d DT:%d pre[d:%d s:%d ut:%d] - op[d:%d fSp:%d dSp:%d brake:%d ut:%d heating:%d cond1:%d cond2:%d extra:%d cW:%d cA:%d] M[%d %d %d %d %d %d]", app_processor_state_get(), app_nm_dry_state_get(), app_nm_dry_preSetup_state_get(), app_nm_dry_setup_state_get(), app_nm_dry_operation_state_get(), app_nm_data_get()->totalTimeMSec, app_nm_data_get()->dry.dryTotalTimeMSec, app_nm_data_get()->dry.preSetupTimeMSec[APP_NORMAL_MODE_DRY_PRE_SETUP_DRAIN], app_nm_data_get()->dry.preSetupTimeMSec[APP_NORMAL_MODE_DRY_PRE_SETUP_SPIN], app_nm_data_get()->dry.preSetupTimeMSec[APP_NORMAL_MODE_DRY_PRE_SETUP_UNTANGLE], app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_DRAIN], app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_FINAL_SPIN], app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_DRYING_SPIN], app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_BRAKE], app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_UNTANGLE], app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_HEATING], app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_1], app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_2], app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME], app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_COOLING_WATER], app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR], app_nm_dry_motor_data_get()->operation.heatingState, app_nm_dry_motor_data_get()->operation.condense1State, app_nm_dry_motor_data_get()->operation.condense2State, app_nm_dry_motor_data_get()->operation.extraTimeState, app_nm_dry_motor_data_get()->operation.coolingWaterState, app_nm_dry_motor_data_get()->operation.coolingAirState);
#endif

    LOG('M', 'v', "P:%d clutchState %d    clutchMode %d    haltState %d    clutchResetState %d    operationState %d", app_processor_state_get(), app_motor_data_get()->clutchState, app_motor_data_get()->clutchMode, app_motor_haltState_get(), app_motor_clutchResetState_get(), app_motor_operationState_get());

#ifdef BLOWER_ENABLED
    LOG('s', 'm', "P:%d tmp[A:%u B:%u water:%u ambnt:%u] ac[in:%u state:%u] wLevel[c:%u level:%u] lid[snsr:%u lcksnsr:%u lckStat:%d] lntFltr:%u offSW:%u dVSensor:%u motorRPM:%u freq:%u blowerRPM:%u heaterCurr:%u memStat:%d", app_processor_state_get(), handler_heaterTempSensor_temp_get(), handler_airBTempSensor_temp_get(), handler_waterTempSensor_temp_get(), handler_ambientTempSensor_temp_get(), handler_acin_value_get(), handler_acin_state_get(), handler_waterLevel_resetLevel_get(), handler_waterLevel_currentFreq_get(), handler_lidSensor_state_get(), handler_lockMotor_sensorState_get(), app_lidLock_state_get(), handler_lintFilterSensor_state_get(), handler_offSW_state_get(), handler_drainValve_state_get(), app_motor_speed_get(), handler_powerDetection_freq_get(), handler_blower_speed_get(), (uint32_t)handler_heater_feedback_get(), app_memory_storingState_get());
#else
    LOG('s', 'm', "P:%d tmp[A:%u B:%u water:%u ambnt:%u] ac[in:%u state:%u] wLevel[c:%u level:%u] lid[snsr:%u lcksnsr:%u lckStat:%d] offSW:%u dVSensor:%u motorRPM:%u freq:%u heaterFB:%u memStat:%d deterSW:[%d %d] softSW:[%d %d]", app_processor_state_get(), handler_heaterTempSensor_temp_get(), handler_airBTempSensor_temp_get(), handler_waterTempSensor_temp_get(), handler_ambientTempSensor_temp_get(), handler_acin_value_get(), handler_acin_state_get(), handler_waterLevel_resetLevel_get(), handler_waterLevel_currentFreq_get(), handler_lidSensor_state_get(), handler_lockMotor_sensorState_get(), app_lidLock_state_get(), handler_offSW_state_get(), handler_drainValve_state_get(), app_motor_speed_get(), handler_powerDetection_freq_get(), (uint32_t)handler_heater_feedback_get(), app_memory_storingState_get(), handler_detergentSensor_state_get(), app_memory_autoDetergentFirstTime_get(), handler_softenerSensor_state_get(), app_memory_autoSoftenerFirstTime_get());
#endif

#ifdef DRYER_ENABLED
    LOG('U', 'm', "P:%d T:%d WT:%d DT:%d course:%d Oper:%d dStart:%d cLck:%d WWLvl:%d wTmp:%d sk:%d wsh:%d stem:%d rns:%d eRns:%d spn:%d sLvl:%d ss:%d aRnkl:%d sTch:%d gDeter:%d hTime:%d dTime:%d dInten:%d dWLvl:%d cycleTyp:%d tubC:%d", app_processor_state_get(), app_nm_data_get()->totalTimeMSec, app_nm_data_get()->wash.washTotalTimeMSec, app_nm_data_get()->dry.dryTotalTimeMSec, app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.common.operationSelection, app_userInterface_data_get()->normalMode.common.delayStartHourSelection, app_userInterface_data_get()->normalMode.common.childLockSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, app_userInterface_data_get()->normalMode.wash.waterTempSelection, app_userInterface_data_get()->normalMode.wash.soakSelection, app_userInterface_data_get()->normalMode.wash.washSelection, app_userInterface_data_get()->normalMode.wash.steamSelection, app_userInterface_data_get()->normalMode.wash.rinseSelection, app_userInterface_data_get()->normalMode.wash.extraRinseSelection, app_userInterface_data_get()->normalMode.wash.spinSelection, app_userInterface_data_get()->normalMode.wash.soilLevelSelection, app_userInterface_data_get()->normalMode.wash.superSpinSelection, app_userInterface_data_get()->normalMode.wash.antiWrinkleSelection, app_userInterface_data_get()->normalMode.wash.steamTechSelection, app_userInterface_data_get()->normalMode.wash.gelDetergentSelection, app_userInterface_data_get()->normalMode.wash.waterHeatingSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryIntensitySelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel, app_userInterface_data_get()->normalMode.common.cycleTypeSelection, app_userInterface_data_get()->normalMode.common.tubcleanSelection);
#else
    LOG('U', 'm', "P:%d T:%d WT:%d course:%d Oper:%d dStart:%d cLck:%d WWLvl:%d wTmp:%d sk:%d wsh:%d stem:%d rns:%d eRns:%d spn:%d sLvl:%d ss:%d aRnkl:%d sTch:%d gDeter:%d hTime:%d cycleTyp:%d tubC:%d", app_processor_state_get(), app_nm_data_get()->totalTimeMSec, app_nm_data_get()->wash.washTotalTimeMSec, app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.common.operationSelection, app_userInterface_data_get()->normalMode.common.delayStartHourSelection, app_userInterface_data_get()->normalMode.common.childLockSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, app_userInterface_data_get()->normalMode.wash.waterTempSelection, app_userInterface_data_get()->normalMode.wash.soakSelection, app_userInterface_data_get()->normalMode.wash.washSelection, app_userInterface_data_get()->normalMode.wash.steamSelection, app_userInterface_data_get()->normalMode.wash.rinseSelection, app_userInterface_data_get()->normalMode.wash.extraRinseSelection, app_userInterface_data_get()->normalMode.wash.spinSelection, app_userInterface_data_get()->normalMode.wash.soilLevelSelection, app_userInterface_data_get()->normalMode.wash.superSpinSelection, app_userInterface_data_get()->normalMode.wash.antiWrinkleSelection, app_userInterface_data_get()->normalMode.wash.steamTechSelection, app_userInterface_data_get()->normalMode.wash.gelDetergentSelection, app_userInterface_data_get()->normalMode.wash.waterHeatingSelection, app_userInterface_data_get()->normalMode.common.cycleTypeSelection, app_userInterface_data_get()->normalMode.common.tubcleanSelection);
#endif

    LOG('C', 'v', "[TC] P:%d T:%d TCT:%d WT:%d  course:%d tcSel:%d #:%d (s:%d f:%d) fil:[%d %d] mh:[%d %d] md:[%d %d] stfl:[%d %d] sk:[%d %d] wsh:[%d %d] dr:[%d %d] sp:[%d %d]  {%d}", app_processor_state_get(), app_nm_data_get()->totalTimeMSec, app_nm_data_get()->wash.tubcleanTotalTimeMSec, app_nm_data_get()->wash.washTotalTimeMSec, app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.common.tubcleanSelection, app_nm_data_get()->wash.tubcleanNum, app_nm_tubclean_state_get(), app_nm_tubclean_fillCounter_get(), app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_FILL][0], app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_FILL][1], app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_MH][0], app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_MH][1], app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_MD][0], app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_MD][1], app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_FILL_STIR][0], app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_FILL_STIR][1], app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_SOAK][0], app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_SOAK][1], app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_WASH][0], app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_WASH][1], app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_DRAIN][0], app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_DRAIN][1], app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_SPIN][0], app_nm_data_get()->wash.tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_SPIN][1], app_motor_data_get()->clutchMode);

    // NOTE: this is a special command for the GUI monitor
    LOG('V', 'v', "AA,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n", app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.common.tubcleanSelection, (app_nm_data_get()->totalTimeMSec / 60000), app_userInterface_data_get()->normalMode.wash.washSelection, app_userInterface_data_get()->normalMode.wash.rinseSelection, app_userInterface_data_get()->normalMode.wash.extraRinseSelection, app_userInterface_data_get()->normalMode.wash.spinSelection, app_userInterface_data_get()->normalMode.wash.waterTempSelection, app_userInterface_data_get()->normalMode.wash.waterLevelDisplaySelection, app_userInterface_data_get()->normalMode.wash.soilLevelSelection, app_userInterface_data_get()->normalMode.wash.steamTechSelection, app_fill_autoDetergentState_get(), app_fill_autoSoftenerState_get(), app_userInterface_data_get()->normalMode.wash.gelDetergentSelection, 0, app_load_weightG_get() / 1000, app_nm_wash_wash_detergent_time_get(app_userInterface_data_get()->normalMode.wash.waterLevelSelection), app_nm_wash_fill_autoSoftenerTimeout_get(), app_userInterface_data_get()->normalMode.wash.waterLevelSelection, app_nm_wash_heater_steamTechTargetONTemp_get(app_userInterface_data_get()->normalMode.common.courseSelection), app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_HEATING] + app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_MH] + app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_MD], app_processor_state_get(), app_nm_state_get(), app_nm_wash_state_get(), app_nm_wash_wash_state_get(), app_nm_wash_rinse_state_get(), app_nm_wash_spin_state_get(), handler_waterLevel_currentFreq_get(), handler_waterTempSensor_temp_get(), handler_heaterTempSensor_temp_get(), handler_mems_yValue_get(), handler_mems_yValue_get(), handler_mems_zValue_get(), handler_mems_currentHitCnt_get(), handler_mems_currentCritHitCnt_get(), app_motor_speed_get(), handler_softenerSensor_state_get(), handler_detergentSensor_state_get(), handler_lidSensor_state_get(), handler_lockMotor_sensorState_get(), (app_heater_plateState_get() & 0x01), handler_drainPump_state_get(), handler_coldWaterValve_state_get(), handler_softenerWaterValve_state_get(), handler_hotWaterValve_state_get(), handler_drainValve_state_get(), app_lidLock_state_get(), handler_detergentPump_state_get(), handler_softenerPump_state_get());
}
