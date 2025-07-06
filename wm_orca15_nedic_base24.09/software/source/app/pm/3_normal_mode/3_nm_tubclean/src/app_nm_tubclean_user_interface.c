#include "def.h"

#include "app_user_interface.h"
#include "app_nm_tubclean_user_interface.h"

#include "app_buzzer.h"
#include "app_memory.h"
#include "app_nm_user_interface.h"
#include "app_user_interface.h"

#include "app_nm_tubclean.h"
#include "app_nm_tubclean_defaults.h"

static void tubclean_userInterface_common_handle(void);
static void tubclean_userInterface_wash_handle(void);
static void tubclean_userInterface_dry_handle(void);

static void tubclean_normalMode_operation_update(void);
static void tubclean_normalMode_course_update(void);
static void tubclean_normalMode_delayStart_update(void);
static void tubclean_normalMode_childLock_update(void);
static void tubclean_normalMode_tubclean_update(void);

static void tubclean_normalMode_waterTemp_update(void);
static void tubclean_normalMode_waterLevel_update(void);
static void tubclean_normalMode_soak_update(void);
static void tubclean_normalMode_wash_update(void);
static void tubclean_normalMode_rinse_update(void);
static void tubclean_normalMode_extraRinse_update(void);
static void tubclean_normalMode_spin_update(void);
static void tubclean_normalMode_superSpin_update(void);
static void tubclean_normalMode_soilLevel_update(void);
static void tubclean_normalMode_antiWrinkle_update(void);
static void tubclean_normalMode_steamTech_update(void);
static void tubclean_normalMode_gelDetergent_update(void);

static void tubclean_normalMode_dryTime_update(void);
static void tubclean_normalMode_drynessIntensity_update(void);

void app_nm_tubclean_userInterface_process(uint32_t period)
{
    app_userInterface_childLock_check(period);
    if (app_userInterface_data_get()->normalMode.common.childLockSelection == APP_NORMAL_MODE_UI_CHILD_LOCK_OFF)
    {
        tubclean_userInterface_common_handle();
    }

    if (APP_NORMAL_MODE_TUBCLEAN_FINISH == app_nm_tubclean_state_get())
    {
        app_userInterface_data_set()->normalMode.wash.waterTempSelection = APP_NORMAL_MODE_UI_WATER_TEMP_NONE;
        app_userInterface_data_set()->normalMode.wash.steamTechSelection = APP_NORMAL_MODE_UI_STEAM_TECH_OFF;
    }
}

const app_nm_tubclean_userInterface_defaults_data_t *app_nm_tubclean_userInterface_defaults_data_get(void)
{
    const app_nm_tubclean_userInterface_defaults_data_t *ret = NULL;

    ret = &app_tubclean_defaults[app_userInterface_data_get()->normalMode.common.operationSelection];

    return ret;
}

const app_nm_tubclean_userInterface_defaults_data_t *app_nm_tubclean_userInterface_lowLimitDefaults_data_get(void)
{
    const app_nm_tubclean_userInterface_defaults_data_t *ret = NULL;

    ret = &app_tubclean_lowLimits[app_userInterface_data_get()->normalMode.common.operationSelection];

    return ret;
}

const app_nm_tubclean_userInterface_defaults_data_t *app_nm_tubclean_userInterface_highLimitDefaults_data_get(void)
{
    const app_nm_tubclean_userInterface_defaults_data_t *ret = NULL;

    ret = &app_tubclean_highLimits[app_userInterface_data_get()->normalMode.common.operationSelection];

    return ret;
}

static void tubclean_userInterface_common_handle(void)
{
    tubclean_normalMode_operation_update();
    tubclean_normalMode_course_update();
    tubclean_normalMode_delayStart_update();
    tubclean_normalMode_childLock_update();
    tubclean_normalMode_tubclean_update();

    tubclean_userInterface_wash_handle();
    tubclean_userInterface_dry_handle();
}

static void tubclean_userInterface_wash_handle(void)
{
    tubclean_normalMode_waterTemp_update();
    tubclean_normalMode_waterLevel_update();
    tubclean_normalMode_soak_update();
    tubclean_normalMode_wash_update();
    tubclean_normalMode_rinse_update();
    tubclean_normalMode_extraRinse_update();
    tubclean_normalMode_spin_update();
    tubclean_normalMode_superSpin_update();
    tubclean_normalMode_soilLevel_update();
    tubclean_normalMode_antiWrinkle_update();
    tubclean_normalMode_steamTech_update();
    tubclean_normalMode_gelDetergent_update();
}

static void tubclean_userInterface_dry_handle(void)
{
    tubclean_normalMode_dryTime_update();
    tubclean_normalMode_drynessIntensity_update();
}

static void tubclean_normalMode_operation_update(void)
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
static void tubclean_normalMode_course_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_courseKeyEdgeState_get())
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void tubclean_normalMode_delayStart_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->delayStartKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void tubclean_normalMode_childLock_update(void)
{
    // do nothing
}
static void tubclean_normalMode_tubclean_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->tubCleanKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}

static void tubclean_normalMode_waterTemp_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->waterTempKeyEdgeState)
    {
#ifdef WATER_TEMP_CHANGE_DURING_CYCLE_ENABLE
        if (((app_nm_tubclean_userInterface_highLimitDefaults_data_get()->defaultWaterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_NONE) && (app_nm_tubclean_userInterface_lowLimitDefaults_data_get()->defaultWaterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_NONE)))
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else if (app_userInterface_data_get()->normalMode.wash.waterTempSelection < app_nm_tubclean_userInterface_highLimitDefaults_data_get()->defaultWaterTemp)
        {
            app_userInterface_data_set()->normalMode.wash.waterTempSelection++;
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
        }
        else if (app_userInterface_data_get()->normalMode.wash.waterTempSelection >= app_nm_tubclean_userInterface_highLimitDefaults_data_get()->defaultWaterTemp)
        {
            app_userInterface_data_set()->normalMode.wash.waterTempSelection = app_nm_tubclean_userInterface_lowLimitDefaults_data_get()->defaultWaterTemp;
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
        }
        else
        {
            // do nothing
        }
#else
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
#endif
    }
}
static void tubclean_normalMode_waterLevel_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->waterLevelKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void tubclean_normalMode_soak_update(void)
{
#ifndef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->soakKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
static void tubclean_normalMode_wash_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->washKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void tubclean_normalMode_rinse_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->rinseKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void tubclean_normalMode_extraRinse_update(void)
{
    // NOTE: doesn't exist in the current model
}
static void tubclean_normalMode_spin_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->spinKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void tubclean_normalMode_superSpin_update(void)
{
    // NOTE: doesn't exist in the current model
}
static void tubclean_normalMode_soilLevel_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->stainLevelKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void tubclean_normalMode_antiWrinkle_update(void)
{
    // NOTE: can't change in this model
}
static void tubclean_normalMode_steamTech_update(void)
{
#ifdef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->steamTechKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
static void tubclean_normalMode_gelDetergent_update(void)
{
#ifdef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->gelDetergentKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}

static void tubclean_normalMode_dryTime_update(void)
{
#ifdef DRYER_ENABLED
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->dryTimeKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
static void tubclean_normalMode_drynessIntensity_update(void)
{
#ifdef DRYER_ENABLED
    if (HANDLER_KEY_LONG_PRESSED == app_userInterface_keyEdgeState_get()->dryTimeKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
