#include "def.h"

#include "app_nm_tubclean.h"
#include "app_nm_tubclean_motor.h"

#include "app_motor.h"

static void init_handle(uint32_t period);
static void fill_handle(uint32_t period);
static void mh_handle(uint32_t period);
static void md_handle(uint32_t period);
static void fillStir_handle(uint32_t period);
static void soak_handle(uint32_t period);
static void wash_handle(uint32_t period);
static void drain_handle(uint32_t period);
static void spin_handle(uint32_t period);
static void regroup_handle(uint32_t period);
static void finish_handle(uint32_t period);

static void motor_tubclean_fill_handle(uint32_t period);
static void motor_tubclean_mh_handle(uint32_t period);
static void motor_tubclean_md_handle(uint32_t period);
static void motor_tubclean_fillStir_handle(uint32_t period);
static void motor_tubclean_soak_handle(uint32_t period);
static void motor_tubclean_wash_handle(uint32_t period);
static void motor_tubclean_drain_handle(uint32_t period);
static void motor_tubclean_spin_handle(uint32_t period);
static void motor_tubclean_finish_handle(uint32_t period);

static app_motor_clutchModeHandlers_t gx_motor_callback;

void app_nm_tubclean_motor_process(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        init_handle,
        fill_handle,
        mh_handle,
        md_handle,
        fillStir_handle,
        soak_handle,
        wash_handle,
        drain_handle,
        spin_handle,
        regroup_handle,
        finish_handle,
        NULL};

    if (APP_NORMAL_MODE_TUBCLEAN == app_nm_state_get())
    {
        if (handlers[app_nm_tubclean_state_get()] != NULL)
        {
            handlers[app_nm_tubclean_state_get()](period);
        }
    }
}

static void init_handle(uint32_t period)
{
    app_motor_reset();
}
static void fill_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = motor_tubclean_fill_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
}
static void mh_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = motor_tubclean_mh_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
}
static void md_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = motor_tubclean_md_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
}
static void fillStir_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = motor_tubclean_fillStir_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
}
static void soak_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = motor_tubclean_soak_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
}
static void wash_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = motor_tubclean_wash_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
}
static void drain_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = motor_tubclean_drain_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
}
static void spin_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = motor_tubclean_spin_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITH_BASKET, APP_MOTOR_COMMAND_CLUTCH, &gx_motor_callback);
}
static void regroup_handle(uint32_t period)
{
    // do nothing
}
static void finish_handle(uint32_t period)
{
    gx_motor_callback.motorHalt = NULL;
    gx_motor_callback.clutchSwitching = NULL;
    gx_motor_callback.operation = motor_tubclean_finish_handle;
    app_motor_process(period, APP_MOTOR_CLUTCH_WITHOUT_BASKET, APP_MOTOR_COMMAND_DECLUTCH, &gx_motor_callback);
}

static void motor_tubclean_fill_handle(uint32_t period)
{
    app_motor_brake();
}
static void motor_tubclean_mh_handle(uint32_t period)
{
    static app_nm_tubclean_steamMH_motor_stirState_t gx_motor_stirState = TUBCLEAN_STEAM_TECH_MH;
    static uint32_t stirStateCnt = 0;
    const app_nm_tubclean_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_tubclean_motor_statePattern_t *pattern = NULL;

    stirStateCnt += period;
    if (stirStateCnt >= app_nm_tubclean_motorMHStateMotion_get(gx_motor_stirState))
    {
        stirStateCnt = 0;
        if (TUBCLEAN_STEAM_TECH_MH_STOP <= gx_motor_stirState)
        {
            gx_motor_stirState = TUBCLEAN_STEAM_TECH_MH;
        }
        else
        {
            gx_motor_stirState++;
        }
    }

    pattern = app_nm_tubclean_motorMotionPattern_get();

    if (pattern != NULL)
    {
        switch (gx_motor_stirState)
        {
            case TUBCLEAN_STEAM_TECH_MH:
            {
                agiPattern = &(pattern->mhPattern);
                app_motor_agitationPattern_handle(agiPattern);
            }
            break;

            case TUBCLEAN_STEAM_TECH_MH_STOP:
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
static void motor_tubclean_md_handle(uint32_t period)
{
    static app_nm_tubclean_steamMD_motor_stirState_t gx_motor_stirState = TUBCLEAN_STEAM_TECH_MD;
    static uint32_t stirStateCnt = 0;
    const app_nm_tubclean_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_tubclean_motor_statePattern_t *pattern = NULL;

    stirStateCnt += period;
    if (stirStateCnt >= app_nm_tubclean_motorMDStateMotion_get(gx_motor_stirState))
    {
        stirStateCnt = 0;
        if (TUBCLEAN_STEAM_TECH_MD_STOP <= gx_motor_stirState)
        {
            gx_motor_stirState = TUBCLEAN_STEAM_TECH_MD;
        }
        else
        {
            gx_motor_stirState++;
        }
    }

    pattern = app_nm_tubclean_motorMotionPattern_get();

    if (pattern != NULL)
    {
        switch (gx_motor_stirState)
        {
            case TUBCLEAN_STEAM_TECH_MD:
            {
                agiPattern = &(pattern->mdPattern);
                app_motor_agitationPattern_handle(agiPattern);
            }
            break;

            case TUBCLEAN_STEAM_TECH_MD_STOP:
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
static void motor_tubclean_fillStir_handle(uint32_t period)
{
    const app_nm_tubclean_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_tubclean_motor_statePattern_t *pattern = NULL;

    if (APP_NORMAL_MODE_TUBCLEAN_STIR == app_nm_tubclean_fillStirTable_get(app_nm_tubclean_fillCounter_get())->type)
    {
        pattern = app_nm_tubclean_motorMotionPattern_get();
        if (pattern != NULL)
        {
            agiPattern = &(pattern->fillPattern);
            app_motor_agitationPattern_handle(agiPattern);
        }
    }
    else
    {
        app_motor_brake();
    }
}
static void motor_tubclean_soak_handle(uint32_t period)
{
    const app_nm_tubclean_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_tubclean_motor_statePattern_t *pattern = NULL;

    static uint32_t gu32_motor_subStateElapsedTimeMSec = 0;
    static app_nm_tubclean_soak_motor_stirState_t soakMotorState = TUBCLEAN_SOAK_OPERATION;

    gu32_motor_subStateElapsedTimeMSec += period;
    if (gu32_motor_subStateElapsedTimeMSec >= app_nm_tubclean_soak_motorStateTable_get(soakMotorState))
    {
        if (soakMotorState < TUBCLEAN_SOAK_STOP)
        {
            soakMotorState++;
        }
        else
        {
            soakMotorState = TUBCLEAN_SOAK_OPERATION;
        }
        gu32_motor_subStateElapsedTimeMSec = 0;
    }

    if (TUBCLEAN_SOAK_STOP == soakMotorState)
    {
        app_motor_brake();
    }
    else if (TUBCLEAN_SOAK_OPERATION == soakMotorState)
    {
        pattern = app_nm_tubclean_motorMotionPattern_get();
        if (pattern != NULL)
        {
            agiPattern = &(pattern->soakPattern);
            app_motor_agitationPattern_handle(agiPattern);
        }
    }
    else
    {
        // do nothing
    }
}
static void motor_tubclean_wash_handle(uint32_t period)
{
    const app_nm_tubclean_motor_motionPattern_t *agiPattern = NULL;
    const app_nm_tubclean_motor_statePattern_t *pattern = NULL;

    pattern = app_nm_tubclean_motorMotionPattern_get();
    if (pattern != NULL)
    {
        agiPattern = &(pattern->washPattern);
        app_motor_agitationPattern_handle(agiPattern);
    }
}
static void motor_tubclean_drain_handle(uint32_t period)
{
    const app_motor_spinProfileSectionData_t *spinSection = NULL;
    static uint32_t spinTimeoutMSec = 0U;

    if (spinTimeoutMSec < app_nm_tubclean_motor_drainSpinStateData_get())
    {
        spinTimeoutMSec += period;

        spinSection = app_nm_tubclean_drainSpin_motorMotion_get();
        if (spinSection != NULL)
        {
            app_motor_spinPattern_handle(spinSection);
        }
    }
    else
    {
        spinTimeoutMSec = 0;
        app_motor_brake();
    }
}
static void motor_tubclean_spin_handle(uint32_t period)
{
    const app_motor_spinProfileSectionData_t *spinSection = NULL;

    spinSection = app_nm_tubclean_spin_motorMotion_get();
    if (spinSection != NULL)
    {
        app_motor_spinPattern_handle(spinSection);
    }
}
static void motor_tubclean_finish_handle(uint32_t period)
{
    app_motor_reset();
}
