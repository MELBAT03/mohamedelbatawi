#include "def.h"
#include "app_nm_dry.h"
#include "app_nm_dry_motor.h"
#include "app_nm.h"
#include "app_motor.h"
#include "app_unbalance.h"

#include "app_user_interface.h"

typedef app_motor_command_t app_dry_motor_command_t;

#define APP_MOTOR_SPIN_BREAK_INTERVAL_MSEC (900000UL)
#define APP_MOTOR_SPIN_BREAK_PERIOD_MSEC   (5000UL)

static void motor_preSetup_init_handle(uint32_t period);
static void motor_preSetup_drain_handle(uint32_t period);
static void motor_preSetup_spin_handle(uint32_t period);
static void motor_preSetup_untangle_handle(uint32_t period);
static void motor_preSetup_finish_handle(uint32_t period);
static void motor_preSetup_done_handle(uint32_t period);

static void motor_setup_init_handle(uint32_t period);
static void motor_setup_weightSense_handle(uint32_t period);
static void motor_setup_finish_handle(uint32_t period);
static void motor_setup_done_handle(uint32_t period);

static void motor_operation_init_handle(uint32_t period);
static void motor_operation_drain_handle(uint32_t period);
static void motor_operation_finalSpin_handle(uint32_t period);
static void motor_operation_dryingSpin_handle(uint32_t period);
static void motor_operation_brake_handle(uint32_t period);
static void motor_operation_untangle_handle(uint32_t period);
static void motor_operation_heating_handle(uint32_t period);
static void motor_operation_condense1_handle(uint32_t period);
static void motor_operation_condense2_handle(uint32_t period);
static void motor_operation_extraTime_handle(uint32_t period);
static void motor_operation_lintFilterClogging_handle(uint32_t period);
static void motor_operation_lintFilterCloggingReset_handle(uint32_t period);
static void motor_operation_coolingWater_handle(uint32_t period);
static void motor_operation_coolingAir_handle(uint32_t period);
static void motor_operation_finish_handle(uint32_t period);
static void motor_operation_done_handle(uint32_t period);

static void presetup_spin_haltMotor_handle(uint32_t period);
static void presetup_spin_operation_handle(uint32_t period);
static void presetup_untangle_operation_handle(uint32_t period);
static void setup_loadSense_operation_handle(uint32_t period);
static void operation_finalSpin_motorHalt_handle(uint32_t period);
static void operation_finalSpin_operation_handle(uint32_t period);
static void operation_dryingSpin_haltMotor_handle(uint32_t period);
static void operation_dryingSpin_operation_handle(uint32_t period);
static void operation_untangle_operation_handle(uint32_t period);
static void operation_heating_clutchSwitching_handle(uint32_t period);
static void operation_heating_operation_handle(uint32_t period);
static void operation_condense1_clutchSwitching_handle(uint32_t period);
static void operation_condense1_operation_handle(uint32_t period);
static void operation_condense2_operation_handle(uint32_t period);
static void operation_extraTime_operation_handle(uint32_t period);
static void operation_coolingWater_operation_handle(uint32_t period);
static void operation_coolingAir_operation_handle(uint32_t period);

static app_motor_haltState_t gx_app_motor_haltState = APP_MOTOR_HALT_NOK;
static app_motor_spinProfileSection_t gx_motor_spinSectionNum = SPIN_PROFILE_SECTION_0;

static uint32_t gu32_motor_maxContinuousSpinTimeMSec = 0;
static uint32_t gu32_motor_spinProfileSectionInterval = 0;
static uint32_t gu32_motor_subStateElapsedTimeMSec = 0;
static uint32_t gu32_motor_extraTimeBreakTimeoutMSec = 0;

static app_nm_dry_motor_data_t gx_motor_data;

static app_motor_clutchModeHandlers_t gx_motor_callback;

void app_nm_dry_motor_process(uint32_t period)
{
    static void (*preSetup_handlers[])(uint32_t period) = {
        motor_preSetup_init_handle,
        motor_preSetup_drain_handle,
        motor_preSetup_spin_handle,
        motor_preSetup_untangle_handle,
        motor_preSetup_finish_handle,
        motor_preSetup_done_handle};

    static void (*setup_handlers[])(uint32_t period) = {
        motor_setup_init_handle,
        motor_setup_weightSense_handle,
        motor_setup_finish_handle,
        motor_setup_done_handle};

    static void (*operation_handlers[])(uint32_t period) = {
        motor_operation_init_handle,
        motor_operation_drain_handle,
        motor_operation_finalSpin_handle,
        motor_operation_dryingSpin_handle,
        motor_operation_brake_handle,
        motor_operation_untangle_handle,
        motor_operation_heating_handle,
        motor_operation_condense1_handle,
        motor_operation_condense2_handle,
        motor_operation_extraTime_handle,
        motor_operation_coolingWater_handle,
        motor_operation_coolingAir_handle,
        motor_operation_lintFilterClogging_handle,
        motor_operation_lintFilterCloggingReset_handle,
        motor_operation_finish_handle,
        motor_operation_done_handle};

    gx_app_motor_haltState = app_motor_haltState_get();

    switch (app_nm_dry_state_get())
    {
        case APP_NORMAL_MODE_DRY_INIT:
        case APP_NORMAL_MODE_DRY_FINISH:
        {
            app_motor_brake();
        }
        break;

        case APP_NORMAL_MODE_DRY_PRE_SETUP:
        {
            if (preSetup_handlers[app_nm_dry_preSetup_state_get()] != NULL)
            {
                preSetup_handlers[app_nm_dry_preSetup_state_get()](period);
            }
        }
        break;

        case APP_NORMAL_MODE_DRY_SETUP:
        {
            if (setup_handlers[app_nm_dry_setup_state_get()] != NULL)
            {
                setup_handlers[app_nm_dry_setup_state_get()](period);
            }
        }
        break;

        case APP_NORMAL_MODE_DRY_OPERATION:
        {
            if (operation_handlers[app_nm_dry_operation_state_get()] != NULL)
            {
                operation_handlers[app_nm_dry_operation_state_get()](period);
            }
        }

        case APP_NORMAL_MODE_DRY_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

app_nm_dry_motor_data_t *app_nm_dry_motor_data_get(void)
{
    return &gx_motor_data;
}

app_motor_spinProfileSection_t app_nm_dry_motor_spinProfileSection_get(void)
{
    return gx_motor_spinSectionNum;
}
uint32_t app_nm_dry_motor_maxContinuousSpinCounter_get(void)
{
    return gu32_motor_maxContinuousSpinTimeMSec;
}

static void motor_preSetup_init_handle(uint32_t period)
{
    app_unbalance_reset();
    app_motor_brake();

    app_motor_data_get()->clutchEngagementMotionState = APP_CLUTCH_ENGAGEMENT_NOK;
    app_motor_data_get()->spinState = SPIN_RUN_STAGE;
}
static void motor_preSetup_drain_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = NULL;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
}
static void motor_preSetup_spin_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = presetup_spin_haltMotor_handle;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = presetup_spin_operation_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
}
static void motor_preSetup_untangle_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = presetup_untangle_operation_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
}
static void motor_preSetup_finish_handle(uint32_t period)
{
    app_motor_data_get()->clutchEngagementMotionState = APP_CLUTCH_ENGAGEMENT_NOK;
    app_unbalance_reset();

    app_motor_brake();
}
static void motor_preSetup_done_handle(uint32_t period)
{
    // do nothing
}

static void motor_setup_init_handle(uint32_t period)
{
    app_motor_brake();

    app_motor_data_get()->clutchEngagementMotionState = APP_CLUTCH_ENGAGEMENT_NOK;
}
static void motor_setup_weightSense_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = setup_loadSense_operation_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
}
static void motor_setup_finish_handle(uint32_t period)
{
    app_motor_data_get()->clutchEngagementMotionState = APP_CLUTCH_ENGAGEMENT_NOK;
    app_unbalance_reset();

    app_motor_brake();
}
static void motor_setup_done_handle(uint32_t period)
{
    // do nothing
}

static void motor_operation_init_handle(uint32_t period)
{
    app_unbalance_reset();
    app_motor_brake();

    app_motor_data_get()->clutchEngagementMotionState = APP_CLUTCH_ENGAGEMENT_NOK;

    app_motor_data_get()->spinState = SPIN_RUN_STAGE;

    gx_motor_data.operation.heatingState = APP_NORMAL_MODE_DRY_OPERATION_HEATING_MD;
    gx_motor_data.operation.condense1State = APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_MD;
    gx_motor_data.operation.condense2State = APP_NORMAL_MODE_DRY_OPERATION_CONDENSE2_M4;
    gx_motor_data.operation.coolingWaterState = APP_NORMAL_MODE_DRY_OPERATION_COOLING_WATER_MD;
    gx_motor_data.operation.coolingAirState = APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR_M1;

    gu32_motor_maxContinuousSpinTimeMSec = 0;
    gx_motor_spinSectionNum = SPIN_PROFILE_SECTION_0;
}
static void motor_operation_drain_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = NULL;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
}
static void motor_operation_finalSpin_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = operation_finalSpin_motorHalt_handle;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = operation_finalSpin_operation_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
}
static void motor_operation_dryingSpin_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = operation_dryingSpin_haltMotor_handle;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = operation_dryingSpin_operation_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
}
static void motor_operation_brake_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = NULL;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
}
static void motor_operation_untangle_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = operation_untangle_operation_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
}
static void motor_operation_heating_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = operation_heating_clutchSwitching_handle;
    gx_motor_callback.operation = operation_heating_operation_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
}
static void motor_operation_condense1_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = operation_condense1_clutchSwitching_handle;
    gx_motor_callback.operation = operation_condense1_operation_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
}
static void motor_operation_condense2_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = operation_condense2_operation_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
}
static void motor_operation_extraTime_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = operation_extraTime_operation_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
}
static void motor_operation_coolingWater_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = operation_coolingWater_operation_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
}
static void motor_operation_coolingAir_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = operation_coolingAir_operation_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
}
static void motor_operation_lintFilterClogging_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = NULL;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
}
static void motor_operation_lintFilterCloggingReset_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = NULL;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
}
static void motor_operation_finish_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = NULL;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
}
static void motor_operation_done_handle(uint32_t period)
{
    // do nothing
}

static void presetup_spin_haltMotor_handle(uint32_t period)
{
    app_motor_data_get()->spinState = SPIN_RUN_STAGE;
    gu32_motor_spinProfileSectionInterval = 0;
    gu32_motor_maxContinuousSpinTimeMSec = 0;
    gx_motor_spinSectionNum = SPIN_PROFILE_SECTION_0;
}
static void presetup_spin_operation_handle(uint32_t period)
{
    const app_motor_spinProfileSectionData_t *spinSection = NULL;

    switch (app_motor_data_get()->spinState)
    {
        case SPIN_RUN_STAGE:
        {
            if (gx_motor_spinSectionNum < SPIN_PROFILE_SECTION_6)
            {
                gu32_motor_spinProfileSectionInterval += period;

                if (gu32_motor_spinProfileSectionInterval >= (app_nm_dry_motor_preSetupSpinProfileSectionData_get(gx_motor_spinSectionNum)->sectionTimeMSec))
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

            spinSection = app_nm_dry_motor_preSetupSpinProfileSectionData_get(gx_motor_spinSectionNum);

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

static void presetup_untangle_operation_handle(uint32_t period)
{
    const app_nm_dry_motor_statePattern_t *motorPattern = NULL;
    const app_nm_dry_motor_motionPattern_t *agiPattern = NULL;

    motorPattern = app_nm_dry_motor_preSetupMotionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection);
    if (motorPattern != NULL)
    {
        agiPattern = &(motorPattern->untangleState.pattern);
        app_motor_agitationPattern_handle(agiPattern);
    }
}

static void setup_loadSense_operation_handle(uint32_t period)
{
    app_unbalance_reset();
    app_motor_weightDetection_handle();
}

static void operation_finalSpin_motorHalt_handle(uint32_t period)
{
    app_motor_data_get()->spinState = SPIN_RUN_STAGE;
    gu32_motor_spinProfileSectionInterval = 0;
    gu32_motor_maxContinuousSpinTimeMSec = 0;
}
static void operation_finalSpin_operation_handle(uint32_t period)
{
    const app_motor_spinProfileSectionData_t *spinSection = NULL;

    switch (app_motor_data_get()->spinState)
    {
        case SPIN_RUN_STAGE:
        {
            if (gx_motor_spinSectionNum < SPIN_PROFILE_SECTION_6)
            {
                gu32_motor_spinProfileSectionInterval += period;

                if (gu32_motor_spinProfileSectionInterval >= (app_nm_dry_motor_spinProfileSectionData_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel, gx_motor_spinSectionNum)->sectionTimeMSec))
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

            spinSection = app_nm_dry_motor_spinProfileSectionData_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel, gx_motor_spinSectionNum);

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

static void operation_dryingSpin_haltMotor_handle(uint32_t period)
{
    app_motor_data_get()->spinState = SPIN_RUN_STAGE;
    gu32_motor_maxContinuousSpinTimeMSec = 0;
}
static void operation_dryingSpin_operation_handle(uint32_t period)
{
    const app_motor_spinProfileSectionData_t *spinSection = NULL;

    switch (app_motor_data_get()->spinState)
    {
        case SPIN_RUN_STAGE:
        {
            gu32_motor_maxContinuousSpinTimeMSec += period;
            if (gu32_motor_maxContinuousSpinTimeMSec >= APP_MOTOR_SPIN_BREAK_INTERVAL_MSEC)
            {
                app_motor_data_get()->spinState = SPIN_BREAK_STAGE;
                gu32_motor_maxContinuousSpinTimeMSec = 0;
            }

            spinSection = app_nm_dry_motor_spinProfileSectionData_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel, gx_motor_spinSectionNum);

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

static void operation_untangle_operation_handle(uint32_t period)
{
    const app_nm_dry_motor_statePattern_t *motorPattern = NULL;
    const app_nm_dry_motor_motionPattern_t *agiPattern = NULL;

    motorPattern = app_nm_dry_motor_motionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel);
    if (motorPattern != NULL)
    {
        agiPattern = &(motorPattern->untangleState.pattern);
        app_motor_agitationPattern_handle(agiPattern);
    }
}

static void operation_heating_clutchSwitching_handle(uint32_t period)
{
    if (gx_motor_data.operation.heatingState == APP_NORMAL_MODE_DRY_OPERATION_HEATING_MD)
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
static void operation_heating_operation_handle(uint32_t period)
{
    const app_nm_dry_motor_statePattern_t *motorPattern = NULL;
    const app_nm_dry_motor_motionPattern_t *agiPattern = NULL;

    gu32_motor_subStateElapsedTimeMSec += period;
    if (gu32_motor_subStateElapsedTimeMSec >= app_nm_dry_motor_heatingStateIntervalMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel, gx_motor_data.operation.heatingState))
    {
        switch (gx_motor_data.operation.heatingState)
        {
            case APP_NORMAL_MODE_DRY_OPERATION_HEATING_MD:
            {
                gx_motor_data.operation.heatingState = APP_NORMAL_MODE_DRY_OPERATION_HEATING_M1;
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
            }
            break;

            case APP_NORMAL_MODE_DRY_OPERATION_HEATING_M1:
            case APP_NORMAL_MODE_DRY_OPERATION_HEATING_M2:
            {
                gx_motor_data.operation.heatingState++;
            }
            break;

            case APP_NORMAL_MODE_DRY_OPERATION_HEATING_M3:
            {
                gx_motor_data.operation.heatingState = APP_NORMAL_MODE_DRY_OPERATION_HEATING_MD;
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

    motorPattern = app_nm_dry_motor_motionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel);
    if (motorPattern != NULL)
    {
        agiPattern = &(motorPattern->heatingState.pattern[gx_motor_data.operation.heatingState]);
        app_motor_agitationPattern_handle(agiPattern);
    }
}

static void operation_condense1_clutchSwitching_handle(uint32_t period)
{
    if (gx_motor_data.operation.condense1State == APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_MD)
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
static void operation_condense1_operation_handle(uint32_t period)
{
    const app_nm_dry_motor_statePattern_t *motorPattern = NULL;
    const app_nm_dry_motor_motionPattern_t *agiPattern = NULL;

    gu32_motor_subStateElapsedTimeMSec += period;
    if (gu32_motor_subStateElapsedTimeMSec >= app_nm_dry_motor_condense1StateIntervalMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel, gx_motor_data.operation.condense1State))
    {
        switch (gx_motor_data.operation.condense1State)
        {
            case APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_MD:
            {
                gx_motor_data.operation.condense1State = APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_M1;
                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
            }
            break;

            case APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_M1:
            case APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_M2:
            {
                gx_motor_data.operation.condense1State++;
            }
            break;

            case APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_M3:
            {
                gx_motor_data.operation.condense1State = APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_MD;
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

    motorPattern = app_nm_dry_motor_motionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel);
    if (motorPattern != NULL)
    {
        agiPattern = &(motorPattern->condense1State.pattern[gx_motor_data.operation.condense1State]);
        app_motor_agitationPattern_handle(agiPattern);
    }
}

static void operation_condense2_operation_handle(uint32_t period)
{
    static uint32_t gu32_motor_condense2BreakTimeoutMSec = 0;
    const app_nm_dry_motor_statePattern_t *motorPattern = NULL;
    const app_nm_dry_motor_motionPattern_t *agiPattern = NULL;

    gu32_motor_condense2BreakTimeoutMSec += period;
    if (gu32_motor_condense2BreakTimeoutMSec >= app_nm_dry_motor_condense2PhaseTimeMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel))
    {
        gx_motor_data.operation.condense2State = APP_NORMAL_MODE_DRY_OPERATION_CONDENSE2_BREAK;
        gu32_motor_condense2BreakTimeoutMSec = 0;
        gu32_motor_subStateElapsedTimeMSec = 0;
    }
    else
    {
        gu32_motor_subStateElapsedTimeMSec += period;
        if (gu32_motor_subStateElapsedTimeMSec >= app_nm_dry_motor_condense2StateIntervalMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel, gx_motor_data.operation.condense2State))
        {
            if (gx_motor_data.operation.condense2State < APP_NORMAL_MODE_DRY_OPERATION_CONDENSE2_M6)
            {
                gx_motor_data.operation.condense2State++;
            }
            else
            {
                gx_motor_data.operation.condense2State = APP_NORMAL_MODE_DRY_OPERATION_CONDENSE2_M4;
            }
            gu32_motor_subStateElapsedTimeMSec = 0;
        }
    }

    motorPattern = app_nm_dry_motor_motionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel);
    if (motorPattern != NULL)
    {
        agiPattern = &(motorPattern->condense2State.pattern[gx_motor_data.operation.condense2State]);
        app_motor_agitationPattern_handle(agiPattern);
    }
}

static void operation_extraTime_operation_handle(uint32_t period)
{
    const app_nm_dry_motor_statePattern_t *motorPattern = NULL;
    const app_nm_dry_motor_motionPattern_t *agiPattern = NULL;

    gu32_motor_extraTimeBreakTimeoutMSec += period;
    if (gu32_motor_extraTimeBreakTimeoutMSec >= app_nm_dry_motor_extraTimePhaseTimeMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel))
    {
        gx_motor_data.operation.extraTimeState = APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME_BREAK;
        gu32_motor_extraTimeBreakTimeoutMSec = 0;
        gu32_motor_subStateElapsedTimeMSec = 0;
    }
    else
    {
        gu32_motor_subStateElapsedTimeMSec += period;
        if (gu32_motor_subStateElapsedTimeMSec >= app_nm_dry_motor_extraTimeStateIntervalMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel, gx_motor_data.operation.extraTimeState))
        {
            if (gx_motor_data.operation.extraTimeState < APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME_M6)
            {
                gx_motor_data.operation.extraTimeState++;
            }
            else
            {
                gx_motor_data.operation.extraTimeState = APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME_M4;
            }
            gu32_motor_subStateElapsedTimeMSec = 0;
        }
    }

    motorPattern = app_nm_dry_motor_motionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel);
    if (motorPattern != NULL)
    {
        agiPattern = &(motorPattern->extraTimeState.pattern[gx_motor_data.operation.extraTimeState]);
        app_motor_agitationPattern_handle(agiPattern);
    }
}

static void operation_coolingWater_operation_handle(uint32_t period)
{
    const app_nm_dry_motor_statePattern_t *motorPattern = NULL;
    const app_nm_dry_motor_motionPattern_t *agiPattern = NULL;

    motorPattern = app_nm_dry_motor_motionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel);
    if (motorPattern != NULL)
    {
        agiPattern = &(motorPattern->coolingWaterState.pattern);
        app_motor_agitationPattern_handle(agiPattern);
    }
}

static void operation_coolingAir_operation_handle(uint32_t period)
{
    const app_nm_dry_motor_statePattern_t *motorPattern = NULL;
    const app_nm_dry_motor_motionPattern_t *agiPattern = NULL;

    gu32_motor_subStateElapsedTimeMSec += period;
    if (gu32_motor_subStateElapsedTimeMSec >= app_nm_dry_motor_coolingAirStateIntervalMSec_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel, gx_motor_data.operation.coolingAirState))
    {
        if (gx_motor_data.operation.coolingAirState < APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR_M3)
        {
            gx_motor_data.operation.coolingAirState++;
        }
        else
        {
            gx_motor_data.operation.coolingAirState = APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR_M1;
        }
        gu32_motor_subStateElapsedTimeMSec = 0;
    }

    motorPattern = app_nm_dry_motor_motionPattern_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.dry.dryTimeSelection, app_userInterface_data_get()->normalMode.dry.dryWaterLevel);
    if (motorPattern != NULL)
    {
        agiPattern = &(motorPattern->coolingAirState.pattern[gx_motor_data.operation.coolingAirState]);
        app_motor_agitationPattern_handle(agiPattern);
    }
}
