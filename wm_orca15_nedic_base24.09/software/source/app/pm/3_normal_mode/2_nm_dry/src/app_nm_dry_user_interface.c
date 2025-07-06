#include "def.h"

#include "app_user_interface.h"
#include "app_nm_dry_user_interface.h"

#include "app_nm_dry.h"
#include "app_buzzer.h"

#include "app_nm_dry_defaults.h"

static void dry_userInterface_normalMode_handle(void);
static void wash_dry_userinterface_normalMode_handle(void);
static void dry_dry_userinterface_normalMode_handle(void);

static void dry_normalMode_operation_update(void);
static void dry_normalMode_course_update(void);
static void dry_normalMode_delayStart_update(void);
static void dry_normalMode_childLock_update(void);
static void dry_normalMode_tubclean_update(void);

static void dry_normalMode_waterTemp_update(void);
static void dry_normalMode_waterLevel_update(void);
static void dry_normalMode_soak_update(void);
static void dry_normalMode_wash_update(void);
static void dry_normalMode_rinse_update(void);
static void dry_normalMode_extraRinse_update(void);
static void dry_normalMode_spin_update(void);
static void dry_normalMode_superSpin_update(void);
static void dry_normalMode_soilLevel_update(void);
static void dry_normalMode_antiWrinkle_update(void);

static void dry_normalMode_dryTime_update(void);
static void dry_normalMode_drynessIntensity_update(void);

static const app_nm_dry_userInterafce_defaults_data_t *ptrDefaultData[] = {
    app_dry_defaults_courseRegularDefaultData,
    app_dry_defaults_courseWoolDefaultData,
    app_dry_defaults_courseSensitiveDefaultData,
    app_dry_defaults_courseQuickDefaultData,
    app_dry_defaults_courseHygieneDefaultData,
    app_dry_defaults_courseHeavyDefaultData,
    app_dry_defaults_courseTubWashDefaultData,
    app_dry_defaults_courseBabyCareDefaultData,
    app_dry_defaults_courseBlanketDefaultData,
    app_dry_defaults_courseLowTempDryDefaultData,
    app_dry_defaults_courseEasyIronDefaultData,
    app_dry_defaults_courseTubDryDefaultData};

static const app_nm_dry_userInterafce_defaults_data_t *ptrLowLimitData[] = {
    app_dry_defaults_courseRegularLowLimitData,
    app_dry_defaults_courseWoolLowLimitData,
    app_dry_defaults_courseSensitiveLowLimitData,
    app_dry_defaults_courseQuickLowLimitData,
    app_dry_defaults_courseHygieneLowLimitData,
    app_dry_defaults_courseHeavyLowLimitData,
    app_dry_defaults_courseTubWashLowLimitData,
    app_dry_defaults_courseBabyCareLowLimitData,
    app_dry_defaults_courseBlanketLowLimitData,
    app_dry_defaults_courseLowTempDryLowLimitData,
    app_dry_defaults_courseEasyIronLowLimitData,
    app_dry_defaults_courseTubDryLowLimitData};

static const app_nm_dry_userInterafce_defaults_data_t *ptrHighLimitData[] = {
    app_dry_defaults_courseRegularHighLimitData,
    app_dry_defaults_courseWoolHighLimitData,
    app_dry_defaults_courseSensitiveHighLimitData,
    app_dry_defaults_courseQuickHighLimitData,
    app_dry_defaults_courseHygieneHighLimitData,
    app_dry_defaults_courseHeavyHighLimitData,
    app_dry_defaults_courseTubWashHighLimitData,
    app_dry_defaults_courseBabyCareHighLimitData,
    app_dry_defaults_courseBlanketHighLimitData,
    app_dry_defaults_courseLowTempDryHighLimitData,
    app_dry_defaults_courseEasyIronHighLimitData,
    app_dry_defaults_courseTubDryHighLimitData};

void app_nm_dry_userInterface_process(uint32_t period)
{
    uint32_t weightData;

    app_userInterface_childLock_check(period);
    if (app_userInterface_data_get()->normalMode.common.childLockSelection == APP_NORMAL_MODE_UI_CHILD_LOCK_OFF)
    {
        dry_userInterface_normalMode_handle();
    }

    switch (app_nm_dry_state_get())
    {
        case APP_NORMAL_MODE_DRY_INIT:
        case APP_NORMAL_MODE_DRY_PRE_SETUP:
        case APP_NORMAL_MODE_DRY_OPERATION:
        case APP_NORMAL_MODE_DRY_FINISH:
        case APP_NORMAL_MODE_DRY_DONE:
        {
            // do nothing
        }
        break;

        case APP_NORMAL_MODE_DRY_SETUP:
        {
            switch (app_nm_dry_setup_state_get())
            {
                case APP_NORMAL_MODE_DRY_SETUP_INIT:
                {
                    // TODO:
                    // app_userInterface_data_set()->normalMode.dry.dryWaterLevel = APP_DRY_UI_DEFAULT_WATER_LEVEL;
                }
                break;
                case APP_NORMAL_MODE_DRY_SETUP_WEIGHT_SENSE:
                {
                    weightData = app_nm_dry_setup_weightData_get();
                    if ((weightData > 0U) && (weightData <= APP_NORMAL_MODE_DRY_MAX_WATER_LEVEL))
                    {
                        app_userInterface_data_set()->normalMode.dry.dryWaterLevel = weightData;
                    }
                }
                break;

                case APP_NORMAL_MODE_DRY_SETUP_FINISH:
                case APP_NORMAL_MODE_DRY_SETUP_DONE:
                default:
                {
                    // do nothing
                }
                break;
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

const app_nm_dry_userInterafce_defaults_data_t *app_nm_dry_userInterface_defaults_data_get(void)
{
    const app_nm_dry_userInterafce_defaults_data_t *ret = NULL;

    ret = (ptrDefaultData[app_userInterface_data_get()->normalMode.common.courseSelection - 1] + (app_userInterface_data_get()->normalMode.common.operationSelection));

    return ret;
}

const app_nm_dry_userInterafce_defaults_data_t *app_nm_dry_userInterface_lowLimit_data_get(void)
{
    const app_nm_dry_userInterafce_defaults_data_t *ret = NULL;

    ret = (ptrLowLimitData[app_userInterface_data_get()->normalMode.common.courseSelection - 1] + (app_userInterface_data_get()->normalMode.common.operationSelection));

    return ret;
}

const app_nm_dry_userInterafce_defaults_data_t *app_nm_dry_userInterface_highLimit_data_get(void)
{
    const app_nm_dry_userInterafce_defaults_data_t *ret = NULL;

    ret = (ptrHighLimitData[app_userInterface_data_get()->normalMode.common.courseSelection - 1] + (app_userInterface_data_get()->normalMode.common.operationSelection));

    return ret;
}

static void dry_userInterface_normalMode_handle(void)
{
    dry_normalMode_operation_update();
    dry_normalMode_course_update();
    dry_normalMode_delayStart_update();
    dry_normalMode_childLock_update();
    dry_normalMode_tubclean_update();

    wash_dry_userinterface_normalMode_handle();
    dry_dry_userinterface_normalMode_handle();
}

static void wash_dry_userinterface_normalMode_handle(void)
{
    dry_normalMode_waterTemp_update();
    dry_normalMode_waterLevel_update();
    dry_normalMode_soak_update();
    dry_normalMode_wash_update();
    dry_normalMode_rinse_update();
    dry_normalMode_extraRinse_update();
    dry_normalMode_spin_update();
    dry_normalMode_superSpin_update();
    dry_normalMode_soilLevel_update();
    dry_normalMode_antiWrinkle_update();
}

static void dry_dry_userinterface_normalMode_handle(void)
{
    dry_normalMode_dryTime_update();
    dry_normalMode_drynessIntensity_update();
}

static void dry_normalMode_operation_update(void)
{
#ifdef DRYER_ENABLED
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->dryOnlyKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
    else if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->washOnlyKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
    else if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->washnDryKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
    else
    {
        // do nothing
    }
#endif
}
static void dry_normalMode_course_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_courseKeyEdgeState_get())
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void dry_normalMode_delayStart_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->delayStartKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void dry_normalMode_childLock_update(void)
{
    // do nothing
}
static void dry_normalMode_tubclean_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->tubCleanKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}

static void dry_normalMode_waterTemp_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->waterTempKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void dry_normalMode_waterLevel_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->waterLevelKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void dry_normalMode_soak_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->soakKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void dry_normalMode_wash_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->washKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void dry_normalMode_rinse_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->rinseKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void dry_normalMode_extraRinse_update(void)
{
    // NOTE: doesn't exist in the current model
}
static void dry_normalMode_spin_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->spinKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void dry_normalMode_superSpin_update(void)
{
    // NOTE: doesn't exist in the current model
}
static void dry_normalMode_soilLevel_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->stainLevelKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void dry_normalMode_antiWrinkle_update(void)
{
    // NOTE: can't change in this model
}

static void dry_normalMode_dryTime_update(void)
{
#ifdef DRYER_ENABLED
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->dryTimeKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
static void dry_normalMode_drynessIntensity_update(void)
{
#ifdef DRYER_ENABLED
    if (HANDLER_KEY_LONG_PRESSED == app_userInterface_keyEdgeState_get()->dryTimeKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
