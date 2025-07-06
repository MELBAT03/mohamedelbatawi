#include "def.h"
#include "app_nm_wash_motor.h"
#include "app_nm.h"
#include "app_motor.h"
#include "app_fill.h"
#include "app_unbalance.h"

#include "app_user_interface.h"

#include "app_nm_wash.h"

typedef app_motor_command_t app_wash_motor_command_t;

#define APP_MOTOR_SPIN_BREAK_INTERVAL_MSEC (900000UL)
#define APP_MOTOR_SPIN_BREAK_PERIOD_MSEC   (5000UL)

#define MOTOR_RINSE_FILL_MINIMUM_ALLOWED_VARIANCE (100U)

static void motor_wash_setup_process(uint32_t period);
static void motor_wash_setup_weightDetection_handle(uint32_t period);

static void motor_wash_soak_process(uint32_t period);
static void motor_wash_soak_operation_handle(uint32_t period);
static void motor_wash_soak_finish_handle(uint32_t period);

static void motor_wash_wash_process(uint32_t period);
static void motor_wash_wash_setup_handle(uint32_t period);
static void motor_wash_wash_m0_handle(uint32_t period);
static void motor_wash_wash_mh_handle(uint32_t period);
static void motor_wash_wash_md_handle(uint32_t period);
static void motor_wash_wash_m1_handle(uint32_t period);
static void motor_wash_wash_m2_handle(uint32_t period);
static void motor_wash_wash_m3_handle(uint32_t period);
static void motor_wash_wash_m4_handle(uint32_t period);
static void motor_wash_wash_heating_handle(uint32_t period);
static void motor_wash_wash_stir_handle(uint32_t period);
static void motor_wash_wash_untangle_handle(uint32_t period);
static void motor_wash_wash_refill_handle(uint32_t period);
static void motor_wash_wash_finish_handle(uint32_t period);

static void motor_wash_steam_process(uint32_t period);
static void motor_wash_steam_drain_handle(uint32_t period);
static void motor_wash_steam_spin_handle(uint32_t period);
static void motor_wash_steam_steam_clutchSwitching_handle(uint32_t period);
static void motor_wash_steam_steam_operation_handle(uint32_t period);
static void motor_wash_steam_finish_handle(uint32_t period);

static void motor_wash_rinse_process(uint32_t period);
static void motor_wash_rinse_drain_handle(uint32_t period);
static void motor_wash_rinse_spin_handle(uint32_t period);
static void motor_wash_rinse_spinBrake_handle(uint32_t period);
static void motor_wash_rinse_fill_handle(uint32_t period);
static void motor_wash_rinse_stir_handle(uint32_t period);
static void motor_wash_rinse_untangle_handle(uint32_t period);
static void motor_wash_rinse_unbalance_handle(uint32_t period);
static void motor_wash_rinse_unbalance_setup_handle(uint32_t period);
static void motor_wash_rinse_finish_handle(uint32_t period);

static void motor_wash_spin_process(uint32_t period);
static void motor_wash_spin_drain_handle(uint32_t period);
static void motor_wash_spin_spin_handle(uint32_t period);
static void motor_wash_spin_brake_handle(uint32_t period);
static void motor_wash_spin_antiWrinkle_handle(uint32_t peirod);
static void motor_wash_spin_unbalance_handle(uint32_t period);
static void motor_wash_spin_unbalance_setup_handle(uint32_t period);
static void motor_wash_spin_unbalance_stir_handle(uint32_t period);
static void motor_wash_spin_finish_handle(uint32_t period);

static app_motor_clutchModeHandlers_t gx_motor_callback;
static app_nm_wash_steam_motor_stirState_t gx_motor_steam_data = STEAM_STIR_UNTANGLE;

void app_nm_wash_motor_process(uint32_t period)
{
    switch (app_nm_wash_state_get())
    {
        case APP_NORMAL_MODE_WASH_INIT:
        case APP_NORMAL_MODE_WASH_FINISH:
        {
            app_motor_brake();
        }
        break;

        case APP_NORMAL_MODE_WASH_SETUP:
        {
            motor_wash_setup_process(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_SOAK:
        {
            motor_wash_soak_process(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH:
        {
            motor_wash_wash_process(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_STEAM:
        {
            motor_wash_steam_process(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE:
        {
            motor_wash_rinse_process(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN:
        {
            motor_wash_spin_process(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

app_nm_wash_steam_motor_stirState_t app_nm_wash_motor_steamData_get(void)
{
    return gx_motor_steam_data;
}

// NOTE: setup
static void motor_wash_setup_process(uint32_t period)
{
    switch (app_nm_wash_setup_state_get())
    {
        case APP_NORMAL_MODE_WASH_SETUP_INIT:
        case APP_NORMAL_MODE_WASH_SETUP_CHECK_WATER_LEVEL:
        case APP_NORMAL_MODE_WASH_SETUP_DETERGENT_ADD:
        case APP_NORMAL_MODE_WASH_SETUP_FINISH:
        {
            app_motor_reset();
        }
        break;

        case APP_NORMAL_MODE_WASH_SETUP_WEIGHT_DETECTION:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_setup_weightDetection_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_SETUP_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}
static void motor_wash_setup_weightDetection_handle(uint32_t period)
{
    app_motor_weightDetection_handle();
}

// NOTE: soak
static void motor_wash_soak_process(uint32_t period)
{
    switch (app_nm_wash_soak_state_get())
    {
        case APP_NORMAL_MODE_WASH_SOAK_INIT:
        {
            app_motor_reset();
        }
        break;

        case APP_NORMAL_MODE_WASH_SOAK_OPERATION:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_soak_operation_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_SOAK_FINISH:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_soak_finish_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_SOAK_WASH_TRANSITION:
        case APP_NORMAL_MODE_WASH_SOAK_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}
static void motor_wash_soak_operation_handle(uint32_t period)
{
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_soak_motorStatePattern_t *pattern = NULL;

    static uint32_t gu32_motor_subStateElapsedTimeMSec = 0;
    static app_nm_wash_motor_data_t gx_motor_data = {.operation.soakState = APP_NORMAL_MODE_WASH_MOTOR_SOAK_PAUSE_1};

    gu32_motor_subStateElapsedTimeMSec += period;
    if (gu32_motor_subStateElapsedTimeMSec >= app_nm_wash_soak_motorStateTable_get(app_userInterface_data_get()->normalMode.wash.soakSelection, gx_motor_data.operation.soakState))
    {
        if (gx_motor_data.operation.soakState < APP_NORMAL_MODE_WASH_MOTOR_SOAK_MOTION_2)
        {
            gx_motor_data.operation.soakState++;
        }
        else
        {
            gx_motor_data.operation.soakState = APP_NORMAL_MODE_WASH_MOTOR_SOAK_PAUSE_1;
        }
        gu32_motor_subStateElapsedTimeMSec = 0;
    }

    if ((APP_NORMAL_MODE_WASH_MOTOR_SOAK_PAUSE_1 == gx_motor_data.operation.soakState) || (APP_NORMAL_MODE_WASH_MOTOR_SOAK_PAUSE_2 == gx_motor_data.operation.soakState))
    {
        app_motor_brake();
    }
    else if (APP_NORMAL_MODE_WASH_MOTOR_SOAK_MOTION_1 == gx_motor_data.operation.soakState)
    {
        pattern = app_nm_wash_motor_soak_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, gx_motor_data.operation.soakState, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
        if (pattern != NULL)
        {
            agiPattern = &(pattern->m1Pattern);
            app_motor_agitationPattern_handle(agiPattern);
        }
    }
    else if (APP_NORMAL_MODE_WASH_MOTOR_SOAK_MOTION_2 == gx_motor_data.operation.soakState)
    {
        pattern = app_nm_wash_motor_soak_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, gx_motor_data.operation.soakState, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
        if (pattern != NULL)
        {
            agiPattern = &(pattern->m2Pattern);
            app_motor_agitationPattern_handle(agiPattern);
        }
    }
    else
    {
        // do nothing
    }
}
static void motor_wash_soak_finish_handle(uint32_t period)
{
    app_motor_reset();
}

// NOTE: wash
static void motor_wash_wash_process(uint32_t period)
{
    switch (app_nm_wash_wash_state_get())
    {
        case APP_NORMAL_MODE_WASH_WASH_INIT:
        {
            app_motor_reset();
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_SETUP:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_wash_setup_handle;
            if (app_nm_wash_pulsatorVsTubState_get() == 1U)
            {
                app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
            }
            else
            {
                app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_M0:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_wash_m0_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_MH:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_wash_mh_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_MD:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_wash_md_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_M1:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_wash_m1_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_M2:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_wash_m2_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_M3:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_wash_m3_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_M4:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_wash_m4_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_HEATING:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_wash_heating_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_STIR:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_wash_stir_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_UNTANGLE:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_wash_untangle_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_REFILL:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_wash_refill_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_FINISH:
        case APP_NORMAL_MODE_WASH_WASH_SOAK_TRANSITION:
        case APP_NORMAL_MODE_WASH_WASH_RINSE_TRANSITION:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_wash_finish_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}
static void motor_wash_wash_setup_handle(uint32_t period)
{
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_motor_wash_statePattern_t *pattern = NULL;

    pattern = app_nm_wash_wash_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    if (pattern != NULL)
    {
        agiPattern = &(pattern->setupPattern);
        app_motor_agitationPattern_handle(agiPattern);
    }
}
static void motor_wash_wash_m0_handle(uint32_t period)
{
    app_motor_brake();
}
static void motor_wash_wash_mh_handle(uint32_t period)
{
    static app_nm_wash_steamMH_motor_stirState_t gx_motor_wash_stirState = STEAM_TECH_MH;
    static uint32_t stirStateCnt = 0;
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_motor_wash_statePattern_t *pattern = NULL;

    stirStateCnt += period;
    if (stirStateCnt >= app_nm_wash_wash_motorMHStateMotion_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, gx_motor_wash_stirState))
    {
        stirStateCnt = 0;
        if (STEAM_TECH_MH_STOP <= gx_motor_wash_stirState)
        {
            gx_motor_wash_stirState = STEAM_TECH_MH;
        }
        else
        {
            gx_motor_wash_stirState++;
        }
    }

    pattern = app_nm_wash_wash_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    if (pattern != NULL)
    {
        switch (gx_motor_wash_stirState)
        {
            case STEAM_TECH_MH:
            {
                agiPattern = &(pattern->mhPattern);
                app_motor_agitationPattern_handle(agiPattern);
            }
            break;

            case STEAM_TECH_MH_STOP:
            {
                app_motor_brake();
            }
            break;

            default:
            {
                // do nothing
            }
            break;
        }
    }
}
static void motor_wash_wash_md_handle(uint32_t period)
{
    static app_nm_wash_steamMD_motor_stirState_t gx_motor_wash_stirState = STEAM_TECH_MD;
    static uint32_t stirStateCnt = 0;
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_motor_wash_statePattern_t *pattern = NULL;

    stirStateCnt += period;
    if (stirStateCnt >= app_nm_wash_wash_motorMDStateMotion_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, gx_motor_wash_stirState))
    {
        stirStateCnt = 0;
        if (STEAM_TECH_MD_STOP <= gx_motor_wash_stirState)
        {
            gx_motor_wash_stirState = STEAM_TECH_MD;
        }
        else
        {
            gx_motor_wash_stirState++;
        }
    }

    pattern = app_nm_wash_wash_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    if (pattern != NULL)
    {
        switch (gx_motor_wash_stirState)
        {
            case STEAM_TECH_MD:
            {
                agiPattern = &(pattern->mdPattern);
                app_motor_agitationPattern_handle(agiPattern);
            }
            break;

            case STEAM_TECH_MD_STOP:
            {
                app_motor_brake();
            }
            break;

            default:
            {
                // do nothing
            }
            break;
        }
    }
}
static void motor_wash_wash_m1_handle(uint32_t period)
{
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_motor_wash_statePattern_t *pattern = NULL;

    pattern = app_nm_wash_wash_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    if (pattern != NULL)
    {
        agiPattern = &(pattern->m1Pattern);
        app_motor_agitationPattern_handle(agiPattern);
    }
}
static void motor_wash_wash_m2_handle(uint32_t period)
{
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_motor_wash_statePattern_t *pattern = NULL;

    pattern = app_nm_wash_wash_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    if (pattern != NULL)
    {
        agiPattern = &(pattern->m2Pattern);
        app_motor_agitationPattern_handle(agiPattern);
    }
}
static void motor_wash_wash_m3_handle(uint32_t period)
{
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_motor_wash_statePattern_t *pattern = NULL;

    pattern = app_nm_wash_wash_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    if (pattern != NULL)
    {
        agiPattern = &(pattern->m3Pattern);
        app_motor_agitationPattern_handle(agiPattern);
    }
}
static void motor_wash_wash_m4_handle(uint32_t period)
{
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_motor_wash_statePattern_t *pattern = NULL;

    pattern = app_nm_wash_wash_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    if (pattern != NULL)
    {
        agiPattern = &(pattern->m4Pattern);
        app_motor_agitationPattern_handle(agiPattern);
    }
}
static void motor_wash_wash_heating_handle(uint32_t period)
{
    static app_nm_wash_steamHeating_motor_stirState_t gx_motor_wash_stirState = STEAM_TECH_HEATING;
    static uint32_t stirStateCnt = 0;
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_motor_wash_statePattern_t *pattern = NULL;

    stirStateCnt += period;
    if (stirStateCnt >= app_nm_wash_wash_motorHeatingStateMotion_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, gx_motor_wash_stirState))
    {
        stirStateCnt = 0;
        if (STEAM_TECH_HEATING_STOP <= gx_motor_wash_stirState)
        {
            gx_motor_wash_stirState = STEAM_TECH_HEATING;
        }
        else
        {
            gx_motor_wash_stirState++;
        }
    }

    pattern = app_nm_wash_wash_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    if (pattern != NULL)
    {
        switch (gx_motor_wash_stirState)
        {
            case STEAM_TECH_HEATING:
            {
                agiPattern = &(pattern->heatingPattern);
                app_motor_agitationPattern_handle(agiPattern);
            }
            break;

            case STEAM_TECH_HEATING_STOP:
            {
                app_motor_brake();
            }
            break;

            default:
            {
                // do nothing
            }
            break;
        }
    }
}
static void motor_wash_wash_stir_handle(uint32_t period)
{
    static app_nm_wash_motor_stirState_t gx_motor_wash_stirState = WASH_STIR_M5;
    static uint32_t stirStateCnt = 0;
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_motor_wash_statePattern_t *pattern = NULL;

    stirStateCnt += period;
    if (stirStateCnt >= app_nm_wash_wash_motorStirStateMotion_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, gx_motor_wash_stirState))
    {
        stirStateCnt = 0;
        if (WASH_STIR_MU <= gx_motor_wash_stirState)
        {
            gx_motor_wash_stirState = WASH_STIR_M5;
        }
        else
        {
            gx_motor_wash_stirState++;
        }
    }

    pattern = app_nm_wash_wash_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
    switch (gx_motor_wash_stirState)
    {
        case WASH_STIR_M5:
        {
            if (pattern != NULL)
            {
                agiPattern = &(pattern->m5Pattern);
                app_motor_agitationPattern_handle(agiPattern);
            }
        }
        break;

        case WASH_STIR_M6:
        {
            if (pattern != NULL)
            {
                agiPattern = &(pattern->m6Pattern);
                app_motor_agitationPattern_handle(agiPattern);
            }
        }
        break;

        case WASH_STIR_M7:
        {
            if (pattern != NULL)
            {
                agiPattern = &(pattern->m7Pattern);
                app_motor_agitationPattern_handle(agiPattern);
            }
        }
        break;

        case WASH_STIR_MU:
        {
            if (pattern != NULL)
            {
                agiPattern = &(pattern->mUPattern);
                app_motor_agitationPattern_handle(agiPattern);
            }
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}
static void motor_wash_wash_untangle_handle(uint32_t period)
{
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_motor_wash_statePattern_t *pattern = NULL;

    pattern = app_nm_wash_wash_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    if (pattern != NULL)
    {
        agiPattern = &(pattern->untanglePattern);
        app_motor_agitationPattern_handle(agiPattern);
    }
}
static void motor_wash_wash_refill_handle(uint32_t period)
{
    app_motor_brake();
}
static void motor_wash_wash_finish_handle(uint32_t period)
{
    app_motor_reset();
}

// NOTE: steam
static void motor_wash_steam_process(uint32_t period)
{
    switch (app_nm_wash_steam_state_get())
    {
        case APP_NORMAL_MODE_WASH_STEAM_INIT:
        {
            app_motor_reset();
        }
        break;

        case APP_NORMAL_MODE_WASH_STEAM_DRAIN:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_steam_drain_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_STEAM_SPIN:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_steam_spin_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_STEAM_STEAM:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = motor_wash_steam_steam_clutchSwitching_handle;
            gx_motor_callback.operation = motor_wash_steam_steam_operation_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_STEAM_FINISH:
        case APP_NORMAL_MODE_WASH_STEAM_RINSE_TRANSITION:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_steam_finish_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_STEAM_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

static void motor_wash_steam_drain_handle(uint32_t period)
{
    // do nothing
}
static void motor_wash_steam_spin_handle(uint32_t period)
{
    static app_motor_spinProfileSection_t gx_motor_spinSectionNum = SPIN_PROFILE_SECTION_0;
    static uint32_t gu32_motor_maxContinuousSpinTimeMSec = 0;
    static uint32_t gu32_motor_spinProfileSectionInterval = 0;
    const app_motor_spinProfileSectionData_t *spinSection = NULL;

    switch (app_motor_data_get()->spinState)
    {
        case SPIN_RUN_STAGE:
        {
            if (gx_motor_spinSectionNum < SPIN_PROFILE_SECTION_6)
            {
                gu32_motor_spinProfileSectionInterval += period;

                if (gu32_motor_spinProfileSectionInterval >= (app_nm_wash_rinse_motor_spinProfileSectionData_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, gx_motor_spinSectionNum)->sectionTimeMSec))
                {
                    gx_motor_spinSectionNum++;
                    gu32_motor_spinProfileSectionInterval = 0;
                }
            }

            gu32_motor_maxContinuousSpinTimeMSec += period;
            if (gu32_motor_maxContinuousSpinTimeMSec >= APP_MOTOR_SPIN_BREAK_INTERVAL_MSEC)
            {
                app_motor_data_get()->spinState = SPIN_BREAK_STAGE;
                gu32_motor_maxContinuousSpinTimeMSec = 0;
            }

            spinSection = app_nm_wash_rinse_motor_spinProfileSectionData_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, gx_motor_spinSectionNum);

            if (spinSection != NULL)
            {
                app_motor_spinPattern_handle(spinSection);
            }
        }
        break;

        case SPIN_BREAK_STAGE:
        {
            gu32_motor_maxContinuousSpinTimeMSec += period;
            if (gu32_motor_maxContinuousSpinTimeMSec >= APP_MOTOR_SPIN_BREAK_PERIOD_MSEC)
            {
                app_motor_data_get()->spinState = SPIN_RUN_STAGE;
                gu32_motor_maxContinuousSpinTimeMSec = 0;
                app_unbalance_reset();
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_SWITCHING;
            }

            app_motor_brake();
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}
static void motor_wash_steam_steam_clutchSwitching_handle(uint32_t period)
{
    if (gx_motor_steam_data == STEAM_STIR_MD)
    {
        app_motor_data_get()->clutchState = app_motor_clutch_switch(APP_MOTOR_CLUTCH_WITH_BASKET);
        if (app_motor_data_get()->clutchState == APP_MOTOR_CLUTCH_WITH_BASKET)
        {
            app_motor_data_get()->clutchEngagementMotionState = app_motor_clutchEngementMotion_process(period, APP_MOTOR_COMMAND_CLUTCH);
        }
    }
    else
    {
        app_motor_data_get()->clutchState = app_motor_clutch_switch(APP_MOTOR_CLUTCH_WITHOUT_BASKET);
        if (app_motor_data_get()->clutchState == APP_MOTOR_CLUTCH_WITHOUT_BASKET)
        {
            app_motor_data_get()->clutchEngagementMotionState = app_motor_clutchEngementMotion_process(period, APP_MOTOR_COMMAND_DECLUTCH);
        }
    }
}
static void motor_wash_steam_steam_operation_handle(uint32_t period)
{
    const app_nm_wash_steam_motor_statePattern_t *motorPattern = NULL;
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    static uint32_t gu32_motor_subStateElapsedTimeMSec = 0;

    gu32_motor_subStateElapsedTimeMSec += period;
    if (gu32_motor_subStateElapsedTimeMSec >= app_nm_wash_steam_motorStirStateMotion_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, gx_motor_steam_data))
    {
        switch (gx_motor_steam_data)
        {
            case STEAM_STIR_UNTANGLE:
            {
                gx_motor_steam_data = STEAM_STIR_MD;
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
            }
            break;

            case STEAM_STIR_MD:
            {
                gx_motor_steam_data = STEAM_STIR_M1;
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
            }
            break;

            case STEAM_STIR_M1:
            case STEAM_STIR_M2:
            {
                gx_motor_steam_data++;
            }
            break;

            case STEAM_STIR_M3:
            {
                gx_motor_steam_data = STEAM_STIR_MD;
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
            }

            default:
            {
                // do nothing
            }
            break;
        }
        gu32_motor_subStateElapsedTimeMSec = 0;
    }

    motorPattern = app_nm_wash_steam_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
    if (motorPattern != NULL)
    {
        agiPattern = &(motorPattern->pattern[gx_motor_steam_data]);
        app_motor_agitationPattern_handle(agiPattern);
    }
}
static void motor_wash_steam_finish_handle(uint32_t period)
{
    app_motor_reset();
}

// NOTE: rinse
static void motor_wash_rinse_process(uint32_t period)
{
    switch (app_nm_wash_rinse_state_get())
    {
        case APP_NORMAL_MODE_WASH_RINSE_INIT:
        {
            app_motor_reset();
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_DRAIN:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_rinse_drain_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_SPIN:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_rinse_spin_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_SPIN_BRAKE:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_rinse_spinBrake_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_FILL:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_rinse_fill_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_STIR:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_rinse_stir_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNTANGLE:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_rinse_untangle_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE:
        {
            motor_wash_rinse_unbalance_handle(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_FINISH:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_rinse_finish_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_REGROUP:
        case APP_NORMAL_MODE_WASH_RINSE_SPIN_TRANSITION:
        case APP_NORMAL_MODE_WASH_RINSE_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}
static void motor_wash_rinse_drain_handle(uint32_t period)
{
    // do nothing
}
static void motor_wash_rinse_spin_handle(uint32_t period)
{
    static app_motor_spinProfileSection_t gx_motor_spinSectionNum = SPIN_PROFILE_SECTION_0;
    static uint32_t gu32_motor_maxContinuousSpinTimeMSec = 0;
    static uint32_t gu32_motor_spinProfileSectionInterval = 0;
    const app_motor_spinProfileSectionData_t *spinSection = NULL;

    switch (app_motor_data_get()->spinState)
    {
        case SPIN_RUN_STAGE:
        {
            if (gx_motor_spinSectionNum < SPIN_PROFILE_SECTION_6)
            {
                gu32_motor_spinProfileSectionInterval += period;

                if (gu32_motor_spinProfileSectionInterval >= (app_nm_wash_rinse_motor_spinProfileSectionData_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, gx_motor_spinSectionNum)->sectionTimeMSec))
                {
                    gx_motor_spinSectionNum++;
                    gu32_motor_spinProfileSectionInterval = 0;
                }
            }

            gu32_motor_maxContinuousSpinTimeMSec += period;
            if (gu32_motor_maxContinuousSpinTimeMSec >= APP_MOTOR_SPIN_BREAK_INTERVAL_MSEC)
            {
                app_motor_data_get()->spinState = SPIN_BREAK_STAGE;
                gu32_motor_maxContinuousSpinTimeMSec = 0;
            }

            spinSection = app_nm_wash_rinse_motor_spinProfileSectionData_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, gx_motor_spinSectionNum);

            if (spinSection != NULL)
            {
                app_motor_spinPattern_handle(spinSection);
            }
        }
        break;

        case SPIN_BREAK_STAGE:
        {
            gu32_motor_maxContinuousSpinTimeMSec += period;
            if (gu32_motor_maxContinuousSpinTimeMSec >= APP_MOTOR_SPIN_BREAK_PERIOD_MSEC)
            {
                app_motor_data_get()->spinState = SPIN_RUN_STAGE;
                gu32_motor_maxContinuousSpinTimeMSec = 0;
                app_unbalance_reset();
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_SWITCHING;
            }

            app_motor_brake();
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}
static void motor_wash_rinse_spinBrake_handle(uint32_t period)
{
    app_unbalance_reset();
    app_motor_brake();
}
static void motor_wash_rinse_fill_handle(uint32_t period)
{
    uint8_t stirWaterLevelLiter = 0;
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_rinse_motorStatePattern_t *pattern = NULL;

    stirWaterLevelLiter = app_nm_wash_rinse_motorStirWaterLevel_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection)->stirWaterLevelLiter;

    if (APP_FILL_WATER_LEVEL_OK == app_fill_waterLevelState_get(stirWaterLevelLiter, MOTOR_RINSE_FILL_MINIMUM_ALLOWED_VARIANCE))
    {
        pattern = app_nm_wash_rinse_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

        if (pattern != NULL)
        {
            agiPattern = &(pattern->fillStirPattern);
            app_motor_agitationPattern_handle(agiPattern);
        }
    }
    else
    {
        app_motor_brake();
    }
}
static void motor_wash_rinse_stir_handle(uint32_t period)
{
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_rinse_motorStatePattern_t *pattern = NULL;

    pattern = app_nm_wash_rinse_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    if (pattern != NULL)
    {
        agiPattern = &(pattern->stirPattern);
        app_motor_agitationPattern_handle(agiPattern);
    }
}
static void motor_wash_rinse_untangle_handle(uint32_t period)
{
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_rinse_motorStatePattern_t *pattern = NULL;

    pattern = app_nm_wash_rinse_motorMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    if (pattern != NULL)
    {
        agiPattern = &(pattern->untanglePattern);
        app_motor_agitationPattern_handle(agiPattern);
    }
}
static void motor_wash_rinse_unbalance_handle(uint32_t period)
{
    switch (app_nm_wash_rinse_unbalanceState_get())
    {
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_SETUP:
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_FILL:
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_DRAIN:
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_FINISH:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_rinse_unbalance_setup_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_STIR:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_rinse_untangle_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_INIT:
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}
static void motor_wash_rinse_unbalance_setup_handle(uint32_t period)
{
    app_motor_brake();
}
static void motor_wash_rinse_finish_handle(uint32_t period)
{
    app_motor_reset();
}

// NOTE: spin
static void motor_wash_spin_process(uint32_t period)
{
    switch (app_nm_wash_spin_state_get())
    {
        case APP_NORMAL_MODE_WASH_SPIN_INIT:
        {
            app_motor_reset();
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_DRAIN:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_spin_drain_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_SPIN:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_spin_spin_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_SUPER_SPIN:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_spin_spin_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_BRAKE:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_spin_brake_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_ANTI_WRINKLE:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_spin_antiWrinkle_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE:
        {
            motor_wash_spin_unbalance_handle(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_FINISH:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_spin_finish_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_END_TRANSITION:
        case APP_NORMAL_MODE_WASH_SPIN_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}
static void motor_wash_spin_drain_handle(uint32_t period)
{
    // do nothing
}
static void motor_wash_spin_spin_handle(uint32_t period)
{
    static app_motor_spinProfileSection_t gx_motor_spinSectionNum = SPIN_PROFILE_SECTION_0;
    static uint32_t gu32_motor_maxContinuousSpinTimeMSec = 0;
    static uint32_t gu32_motor_spinProfileSectionInterval = 0;
    const app_motor_spinProfileSectionData_t *spinSection = NULL;

    switch (app_motor_data_get()->spinState)
    {
        case SPIN_RUN_STAGE:
        {
            if (gx_motor_spinSectionNum < SPIN_PROFILE_SECTION_6)
            {
                gu32_motor_spinProfileSectionInterval += period;

                if (gu32_motor_spinProfileSectionInterval >= (app_nm_wash_spin_motor_spinProfileSectionData_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, gx_motor_spinSectionNum)->sectionTimeMSec))
                {
                    gx_motor_spinSectionNum++;
                    gu32_motor_spinProfileSectionInterval = 0;
                }
            }

            gu32_motor_maxContinuousSpinTimeMSec += period;
            if (gu32_motor_maxContinuousSpinTimeMSec >= APP_MOTOR_SPIN_BREAK_INTERVAL_MSEC)
            {
                app_motor_data_get()->spinState = SPIN_BREAK_STAGE;
                gu32_motor_maxContinuousSpinTimeMSec = 0;
            }

            spinSection = app_nm_wash_spin_motor_spinProfileSectionData_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, gx_motor_spinSectionNum);

            if (spinSection != NULL)
            {
                app_motor_spinPattern_handle(spinSection);
            }
        }
        break;

        case SPIN_BREAK_STAGE:
        {
            gu32_motor_maxContinuousSpinTimeMSec += period;
            if (gu32_motor_maxContinuousSpinTimeMSec >= APP_MOTOR_SPIN_BREAK_PERIOD_MSEC)
            {
                app_motor_data_get()->spinState = SPIN_RUN_STAGE;
                gu32_motor_maxContinuousSpinTimeMSec = 0;
                app_unbalance_reset();
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_SWITCHING;
            }

            app_motor_brake();
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}
static void motor_wash_spin_brake_handle(uint32_t period)
{
    app_unbalance_reset();
    app_motor_brake();
}
static void motor_wash_spin_antiWrinkle_handle(uint32_t peirod)
{
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_spin_motorStatePattern_t *pattern = NULL;

    pattern = app_nm_wash_spin_motorMotionPattern_get(app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    if (pattern != NULL)
    {
        agiPattern = &(pattern->antiWrinklePattern);
        app_motor_agitationPattern_handle(agiPattern);
    }
}
static void motor_wash_spin_unbalance_handle(uint32_t period)
{
    switch (app_nm_wash_spin_unbalanceState_get())
    {
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_SETUP:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_FILL:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_DRAIN:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_FINISH:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_spin_unbalance_setup_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_STIR:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_SUPER_SPIN_STIR:
        {
            gx_motor_callback.motorHalt = NULL;
            gx_motor_callback.clutchSwitching = NULL;
            gx_motor_callback.operation = motor_wash_spin_unbalance_stir_handle;
            app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_INIT:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_DONE:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_ERROR:
        default:
        {
            // do nothing
        }
        break;
    }
}
static void motor_wash_spin_unbalance_setup_handle(uint32_t period)
{
    app_motor_brake();
}
static void motor_wash_spin_unbalance_stir_handle(uint32_t period)
{
    const app_nm_wash_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_wash_spin_motorStatePattern_t *pattern = NULL;

    pattern = app_nm_wash_spin_motorMotionPattern_get(app_userInterface_data_get()->normalMode.wash.waterLevelSelection);

    if (pattern != NULL)
    {
        agiPattern = &(pattern->stirPattern);
        app_motor_agitationPattern_handle(agiPattern);
    }
}
static void motor_wash_spin_finish_handle(uint32_t period)
{
    app_motor_reset();
}
