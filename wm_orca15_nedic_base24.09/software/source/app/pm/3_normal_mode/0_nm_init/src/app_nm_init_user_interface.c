#include "def.h"

#include "app_user_interface.h"
#include "app_nm_init_user_interface.h"

#include "app_nm_tubclean_user_interface.h"

#include "app_nm.h"
#include "app_buzzer.h"

static void init_userInterface_normalMode_handle(void);
static void wash_init_userinterface_normalMode_handle(void);
static void dry_init_userinterface_normalMode_handle(void);

static void init_normalMode_operation_update(void);
static void init_normalMode_course_update(void);
static void init_normalMode_delayStart_update(void);
static void init_normalMode_childLock_update(void);
static void init_normalMode_tubclean_update(void);

static void init_normalMode_waterTemp_update(void);
static void init_normalMode_waterLevel_update(void);
static void init_normalMode_soak_update(void);
static void init_normalMode_wash_update(void);
static void init_normalMode_rinse_update(void);
static void init_normalMode_extraRinse_update(void);
static void init_normalMode_spin_update(void);
static void init_normalMode_superSpin_update(void);
static void init_normalMode_soilLevel_update(void);
static void init_normalMode_antiWrinkle_update(void);
static void init_normalMode_waterHeating_update(void);
static void init_normalMode_steamTech_update(void);
static void init_normalMode_gelDetergent_update(void);

static void init_normalMode_dryTime_update(void);
static void init_normalMode_drynessIntensity_update(void);

void app_nm_init_userInterface_process(uint32_t period)
{
    app_userInterface_childLock_check(period);
    if (app_userInterface_data_get()->normalMode.common.childLockSelection == APP_NORMAL_MODE_UI_CHILD_LOCK_OFF)
    {
        init_userInterface_normalMode_handle();
    }

    if (0 == app_nm_data_get()->delayStart)
    {
        app_userInterface_data_set()->normalMode.common.delayStartHourSelection = APP_NORMAL_MODE_UI_DELAY_START_TIME_0_HOUR;
    }
}

static void init_userInterface_normalMode_handle(void)
{
    init_normalMode_operation_update();
    init_normalMode_course_update();
    init_normalMode_delayStart_update();
    init_normalMode_childLock_update();
    init_normalMode_tubclean_update();

    wash_init_userinterface_normalMode_handle();
    dry_init_userinterface_normalMode_handle();
}

static void wash_init_userinterface_normalMode_handle(void)
{
    init_normalMode_waterTemp_update();
    init_normalMode_waterLevel_update();
    init_normalMode_soak_update();
    init_normalMode_wash_update();
    init_normalMode_rinse_update();
    init_normalMode_extraRinse_update();
    init_normalMode_spin_update();
    init_normalMode_superSpin_update();
    init_normalMode_soilLevel_update();
    init_normalMode_antiWrinkle_update();
    init_normalMode_waterHeating_update();
    init_normalMode_steamTech_update();
    init_normalMode_gelDetergent_update();
}

static void dry_init_userinterface_normalMode_handle(void)
{
    init_normalMode_dryTime_update();
    init_normalMode_drynessIntensity_update();
}

static void init_normalMode_operation_update(void)
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
static void init_normalMode_course_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_courseKeyEdgeState_get())
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void init_normalMode_delayStart_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->delayStartKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void init_normalMode_childLock_update(void)
{
    // do nothing
}
static void init_normalMode_tubclean_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->tubCleanKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}

static void init_normalMode_waterTemp_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->waterTempKeyEdgeState)
    {
#ifdef WATER_TEMP_CHANGE_DURING_CYCLE_ENABLE
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF)
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if ((0 == app_nm_data_get()->wash.washWashTotalTimeMSec) || (app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_NONE && app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWaterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_NONE))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.waterTempSelection < app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterTemp)
            {
                app_userInterface_data_set()->normalMode.wash.waterTempSelection++;
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.waterTempSelection >= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterTemp)
            {
                app_userInterface_data_set()->normalMode.wash.waterTempSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWaterTemp;
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            }
            else
            {
                // do nothing
            }
        }
        else
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if ((app_nm_tubclean_userInterface_highLimitDefaults_data_get()->defaultWaterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_NONE && app_nm_tubclean_userInterface_lowLimitDefaults_data_get()->defaultWaterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_NONE))
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
        }
#else
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
#endif
    }
}
static void init_normalMode_waterLevel_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->waterLevelKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void init_normalMode_soak_update(void)
{
#ifndef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->soakKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
static void init_normalMode_wash_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->washKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void init_normalMode_rinse_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->rinseKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void init_normalMode_extraRinse_update(void)
{
    // NOTE: doesn't exist in the current model
}
static void init_normalMode_spin_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->spinKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void init_normalMode_superSpin_update(void)
{
    // NOTE: doesn't exist in the current model
}
static void init_normalMode_soilLevel_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->stainLevelKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void init_normalMode_antiWrinkle_update(void)
{
    // NOTE: can't change in this model
}

static void init_normalMode_waterHeating_update(void)
{
    // NOTE: can't change in this model
}

static void init_normalMode_steamTech_update(void)
{
#ifdef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->steamTechKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}

static void init_normalMode_gelDetergent_update(void)
{
#ifdef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->gelDetergentKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}

static void init_normalMode_dryTime_update(void)
{
#ifdef DRYER_ENABLED
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->dryTimeKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
static void init_normalMode_drynessIntensity_update(void)
{
#ifdef DRYER_ENABLED
    if (HANDLER_KEY_LONG_PRESSED == app_userInterface_keyEdgeState_get()->dryTimeKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
