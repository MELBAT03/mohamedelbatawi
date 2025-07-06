#include "def.h"

#include "app_user_interface.h"
#include "app_nm_finish_user_interface.h"
#include "app_buzzer.h"

static void finish_userInterface_normalMode_handle(void);
static void wash_finish_userinterface_normalMode_handle(void);
static void dry_finish_userinterface_normalMode_handle(void);

static void finish_normalMode_operation_update(void);
static void finish_normalMode_course_update(void);
static void finish_normalMode_delayStart_update(void);
static void finish_normalMode_childLock_update(void);
static void finish_normalMode_tubclean_update(void);

static void finish_normalMode_waterTemp_update(void);
static void finish_normalMode_waterLevel_update(void);
static void finish_normalMode_soak_update(void);
static void finish_normalMode_wash_update(void);
static void finish_normalMode_rinse_update(void);
static void finish_normalMode_extraRinse_update(void);
static void finish_normalMode_spin_update(void);
static void finish_normalMode_superSpin_update(void);
static void finish_normalMode_soilLevel_update(void);
static void finish_normalMode_antiWrinkle_update(void);

static void finish_normalMode_dryTime_update(void);
static void finish_normalMode_drynessIntensity_update(void);

void app_nm_finish_userInterface_process(uint32_t period)
{
    finish_userInterface_normalMode_handle();

    app_userInterface_data_set()->normalMode.common.childLockSelection = APP_NORMAL_MODE_UI_CHILD_LOCK_OFF;
}

static void finish_userInterface_normalMode_handle(void)
{
    finish_normalMode_operation_update();
    finish_normalMode_course_update();
    finish_normalMode_delayStart_update();
    finish_normalMode_childLock_update();
    finish_normalMode_tubclean_update();

    wash_finish_userinterface_normalMode_handle();
    dry_finish_userinterface_normalMode_handle();
}

static void wash_finish_userinterface_normalMode_handle(void)
{
    finish_normalMode_waterTemp_update();
    finish_normalMode_waterLevel_update();
    finish_normalMode_soak_update();
    finish_normalMode_wash_update();
    finish_normalMode_rinse_update();
    finish_normalMode_extraRinse_update();
    finish_normalMode_spin_update();
    finish_normalMode_superSpin_update();
    finish_normalMode_soilLevel_update();
    finish_normalMode_antiWrinkle_update();
}

static void dry_finish_userinterface_normalMode_handle(void)
{
    finish_normalMode_dryTime_update();
    finish_normalMode_drynessIntensity_update();
}

static void finish_normalMode_operation_update(void)
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
#else
    // do nothing
#endif
}
static void finish_normalMode_course_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_courseKeyEdgeState_get())
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void finish_normalMode_delayStart_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->delayStartKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void finish_normalMode_childLock_update(void)
{
    // do nothing
}
static void finish_normalMode_tubclean_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->tubCleanKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}

static void finish_normalMode_waterTemp_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->waterTempKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void finish_normalMode_waterLevel_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->waterLevelKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void finish_normalMode_soak_update(void)
{
#ifndef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->soakKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
static void finish_normalMode_wash_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->washKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void finish_normalMode_rinse_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->rinseKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void finish_normalMode_extraRinse_update(void)
{
    // NOTE: doesn't exist in the current model
}
static void finish_normalMode_spin_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->spinKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void finish_normalMode_superSpin_update(void)
{
    // NOTE: doesn't exist in the current model
}
static void finish_normalMode_soilLevel_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->stainLevelKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void finish_normalMode_antiWrinkle_update(void)
{
    // NOTE: can't change in this model
}

static void finish_normalMode_dryTime_update(void)
{
#ifdef DRYER_ENABLED
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->dryTimeKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
static void finish_normalMode_drynessIntensity_update(void)
{
#ifdef DRYER_ENABLED
    if (HANDLER_KEY_LONG_PRESSED == app_userInterface_keyEdgeState_get()->dryTimeKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
