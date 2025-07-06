#include "def.h"
#include "os.h"
#include "config.h"

#include "app_user_interface.h"
#include "app_memory.h"
#include "app_buzzer.h"
#include "app_nm.h"
#include "app_nm_dry.h"

#include "app_processor.h"

#include "app_startup_user_interface.h"
#include "app_setup_user_interface.h"
#include "app_standby_user_interface.h"
#include "app_nm_user_interface.h"
#include "app_tm_user_interface.h"
#include "app_fault_user_interface.h"
#include "app_finish_user_interface.h"

#include "handlers/input/inc/handler_keypad.h"

#define CHILD_LOCK_ACTIVATION_TIMEOUT_MSEC (2500U)

static void userInterface_sw_check(void);

static app_userInterface_keyEdgeState_t gx_userInterface_keyEdgeState;
static app_userInterface_keyState_t gx_userInterface_keyState;

static app_userInterface_data_t gx_userInterface_data;
static app_userInterface_specialFeatures_t uiState = UI_ALTERNATE_NONE;
static uint32_t ssdBlinkTimeoutMSec = 0;

void app_userInterface_update(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        app_startup_userInterface_process,
        app_setup_userInterface_process,
        app_standby_userInterface_process,
        app_nm_userInterface_process,
        app_tm_userInterface_process,
        app_fault_userInterface_process,
        app_finish_userInterface_process,
        NULL};

    userInterface_sw_check();

    if (app_processor_entryFlag_get() == 0)
    {
        return;
    }

    if (handlers[app_processor_state_get()] != NULL)
    {
        handlers[app_processor_state_get()](period);
    }

    if (uiState != UI_ALTERNATE_NONE)
    {
        ssdBlinkTimeoutMSec += period;
        if (ssdBlinkTimeoutMSec > UI_ALTERNATE_FUNCTION_TIMEOUT_MSEC)
        {
            ssdBlinkTimeoutMSec = 0;
            uiState = UI_ALTERNATE_NONE;
        }
    }
    else
    {
        ssdBlinkTimeoutMSec = 0;
    }
}

app_userInterface_specialFeatures_t app_userInterface_state_get(void)
{
    return uiState;
}

app_userInterface_specialFeatures_t app_userInterface_state_set(app_userInterface_specialFeatures_t value)
{
    app_userInterface_specialFeatures_t ret = uiState;
    uiState = value;
    ssdBlinkTimeoutMSec = 0;
    return ret;
}

const app_userInterface_data_t *app_userInterface_data_get(void)
{
    return &gx_userInterface_data;
}

app_userInterface_data_t *app_userInterface_data_set(void)
{
    return &gx_userInterface_data;
}

app_userInterface_keyEdgeState_t *app_userInterface_keyEdgeState_get(void)
{
    return &gx_userInterface_keyEdgeState;
}

app_userInterface_keyState_t *app_userInterface_keyState_get(void)
{
    return &gx_userInterface_keyState;
}

uint32_t app_userInterface_MultipleEdgePressedKeysCnt_get(void)
{
    return handler_keypad_edgePressedKeyCount_get();
}
uint32_t app_userInterface_PressedKeysCnt_get(void)
{
    return handler_keypad_pressedKeyCount_get();
}

handler_keypad_state_t app_userInterface_courseKeyEdgeState_get(void)
{
    handler_keypad_state_t ret = HANDLER_KEY_RELEASED;

#ifdef COURSE_SELECTION_SINGLE_BUTTON
    ret = gx_userInterface_keyEdgeState.courseKeyEdgeState;
#elif defined(COURSE_SELECTION_MULTIPLE_BUTTONS)
    ret = (handler_keypad_state_t)(gx_userInterface_keyEdgeState.courseCottonKeyEdgeState | gx_userInterface_keyEdgeState.courseWhiteKeyEdgeState | gx_userInterface_keyEdgeState.courseBeddingKeyEdgeState | gx_userInterface_keyEdgeState.courseMixKeyEdgeState | gx_userInterface_keyEdgeState.courseDarksKeyEdgeState | gx_userInterface_keyEdgeState.courseBabyCareKeyEdgeState | gx_userInterface_keyEdgeState.courseDelicateKeyEdgeState | gx_userInterface_keyEdgeState.courseLightKeyEdgeState
#ifdef AUTO_DETERGENT_HEATER_MODEL
           | gx_userInterface_keyEdgeState.courseECOKeyEdgeState | gx_userInterface_keyEdgeState.courseAlergyKeyEdgeState
#else
           | gx_userInterface_keyEdgeState.courseWoolKeyEdgeState | gx_userInterface_keyEdgeState.courseRapidWashKeyEdgeState
#endif
           | gx_userInterface_keyEdgeState.courseSportsKeyEdgeState | gx_userInterface_keyEdgeState.courseJeansKeyEdgeState);
#endif

    return ret;
}

handler_keypad_state_t app_userInterface_courseKeyState_get(void)
{
    handler_keypad_state_t ret = HANDLER_KEY_RELEASED;

#ifdef COURSE_SELECTION_SINGLE_BUTTON
    ret = gx_userInterface_keyState.courseKeyState;
#elif defined(COURSE_SELECTION_MULTIPLE_BUTTONS)
    ret = (handler_keypad_state_t)(gx_userInterface_keyState.courseCottonKeyState | gx_userInterface_keyState.courseWhiteKeyState | gx_userInterface_keyState.courseBeddingKeyState | gx_userInterface_keyState.courseMixKeyState | gx_userInterface_keyState.courseDarksKeyState | gx_userInterface_keyState.courseBabyCareKeyState | gx_userInterface_keyState.courseSensitiveKeyState | gx_userInterface_keyState.courseLightKeyState
#ifdef AUTO_DETERGENT_HEATER_MODEL
           | gx_userInterface_keyState.courseECOKeyState | gx_userInterface_keyState.courseAlergyKeyState
#else
           | gx_userInterface_keyState.courseWoolKeyState | gx_userInterface_keyState.courseRapidWashKeyState
#endif
           | gx_userInterface_keyState.courseSportsKeyState | gx_userInterface_keyState.courseJeansKeyState);
#endif

    return ret;
}

void app_userInterface_childLock_check(uint32_t period)
{
    static uint32_t betweenPressTimeoutMSec = 0;

    if ((app_userInterface_keyState_get()->spinKeyState != HANDLER_KEY_RELEASED))
    {
        betweenPressTimeoutMSec += period;
        if (betweenPressTimeoutMSec > CHILD_LOCK_ACTIVATION_TIMEOUT_MSEC)
        {
            betweenPressTimeoutMSec = 0;
            if (app_userInterface_data_get()->normalMode.common.childLockSelection == APP_NORMAL_MODE_UI_CHILD_LOCK_OFF)
            {
                app_userInterface_data_set()->normalMode.common.childLockSelection = APP_NORMAL_MODE_UI_CHILD_LOCK_ON;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
            }
            else if (app_userInterface_data_get()->normalMode.common.childLockSelection == APP_NORMAL_MODE_UI_CHILD_LOCK_ON)
            {
                app_userInterface_data_set()->normalMode.common.childLockSelection = APP_NORMAL_MODE_UI_CHILD_LOCK_OFF;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_INACTIVE);
            }
            else
            {
                // do nothing
            }
        }
    }
    else
    {
        betweenPressTimeoutMSec = 0;
    }
}

static void userInterface_sw_check(void)
{
#ifdef COURSE_SELECTION_SINGLE_BUTTON
    gx_userInterface_keyEdgeState.courseKeyEdgeState = handler_keypad_edgeState_get(KEY_COURSE_ID);
#elif defined(COURSE_SELECTION_MULTIPLE_BUTTONS)
    gx_userInterface_keyEdgeState.courseCottonKeyEdgeState = handler_keypad_edgeState_get(KEY_COTTON_ID);
    gx_userInterface_keyEdgeState.courseWhiteKeyEdgeState = handler_keypad_edgeState_get(KEY_WHITE_ID);
    gx_userInterface_keyEdgeState.courseBeddingKeyEdgeState = handler_keypad_edgeState_get(KEY_BEDDING_ID);
    gx_userInterface_keyEdgeState.courseMixKeyEdgeState = handler_keypad_edgeState_get(KEY_MIX_ID);
    gx_userInterface_keyEdgeState.courseDarksKeyEdgeState = handler_keypad_edgeState_get(KEY_DARKS_ID);
    gx_userInterface_keyEdgeState.courseBabyCareKeyEdgeState = handler_keypad_edgeState_get(KEY_BABYCARE_ID);
    gx_userInterface_keyEdgeState.courseDelicateKeyEdgeState = handler_keypad_edgeState_get(KEY_SENSITIVE_ID);
    gx_userInterface_keyEdgeState.courseLightKeyEdgeState = handler_keypad_edgeState_get(KEY_LIGHT_ID);
    gx_userInterface_keyEdgeState.courseSportsKeyEdgeState = handler_keypad_edgeState_get(KEY_SPORTS_ID);
    gx_userInterface_keyEdgeState.courseJeansKeyEdgeState = handler_keypad_edgeState_get(KEY_JEANS_ID);
#ifdef AUTO_DETERGENT_HEATER_MODEL
    gx_userInterface_keyEdgeState.courseECOKeyEdgeState = handler_keypad_edgeState_get(KEY_ECO_ID);
    gx_userInterface_keyEdgeState.courseAlergyKeyEdgeState = handler_keypad_edgeState_get(KEY_ALERGY_ID);
#else
    gx_userInterface_keyEdgeState.courseWoolKeyEdgeState = handler_keypad_edgeState_get(KEY_WOOL_ID);
    gx_userInterface_keyEdgeState.courseRapidWashKeyEdgeState = handler_keypad_edgeState_get(KEY_RAPIDWASH_ID);
#endif
#endif

    gx_userInterface_keyEdgeState.waterLevelKeyEdgeState = handler_keypad_edgeState_get(KEY_WATER_LEVEL_ID);
    gx_userInterface_keyEdgeState.waterTempKeyEdgeState = handler_keypad_edgeState_get(KEY_WATER_TEMP_ID);
    gx_userInterface_keyEdgeState.washKeyEdgeState = handler_keypad_edgeState_get(KEY_WASH_ID);
    gx_userInterface_keyEdgeState.delayStartKeyEdgeState = handler_keypad_edgeState_get(KEY_DELAY_START_ID);
    gx_userInterface_keyEdgeState.rinseKeyEdgeState = handler_keypad_edgeState_get(KEY_RINSE_ID);
    gx_userInterface_keyEdgeState.spinKeyEdgeState = handler_keypad_edgeState_get(KEY_SPIN_ID);
    gx_userInterface_keyEdgeState.tubCleanKeyEdgeState = handler_keypad_edgeState_get(KEY_TUBCLEAN_ID);
    gx_userInterface_keyEdgeState.stainLevelKeyEdgeState = handler_keypad_edgeState_get(KEY_STAIN_LEVEL_ID);

#ifdef AUTO_DETERGENT_HEATER_MODEL
    gx_userInterface_keyEdgeState.gelDetergentKeyEdgeState = handler_keypad_edgeState_get(KEY_GEL_DETERGENT_ID);
    gx_userInterface_keyEdgeState.steamTechKeyEdgeState = handler_keypad_edgeState_get(KEY_STEAM_TECH_ID);
#else
    gx_userInterface_keyEdgeState.soakKeyEdgeState = handler_keypad_edgeState_get(KEY_SOAK_ID);
    gx_userInterface_keyEdgeState.extraSpinKeyEdgeState = handler_keypad_edgeState_get(KEY_EXTRA_SPIN_ID);
#endif

    gx_userInterface_keyEdgeState.startPauseKeyEdgeState = handler_keypad_edgeState_get(KEY_START_PAUSE_ID);

#ifdef COURSE_SELECTION_SINGLE_BUTTON
    gx_userInterface_keyState.courseKeyState = handler_keypad_state_get(KEY_COURSE_ID);
#elif defined(COURSE_SELECTION_MULTIPLE_BUTTONS)
    gx_userInterface_keyState.courseCottonKeyState = handler_keypad_state_get(KEY_COTTON_ID);
    gx_userInterface_keyState.courseWhiteKeyState = handler_keypad_state_get(KEY_WHITE_ID);
    gx_userInterface_keyState.courseBeddingKeyState = handler_keypad_state_get(KEY_BEDDING_ID);
    gx_userInterface_keyState.courseMixKeyState = handler_keypad_state_get(KEY_MIX_ID);
    gx_userInterface_keyState.courseDarksKeyState = handler_keypad_state_get(KEY_DARKS_ID);
    gx_userInterface_keyState.courseBabyCareKeyState = handler_keypad_state_get(KEY_BABYCARE_ID);
    gx_userInterface_keyState.courseSensitiveKeyState = handler_keypad_state_get(KEY_SENSITIVE_ID);
    gx_userInterface_keyState.courseLightKeyState = handler_keypad_state_get(KEY_LIGHT_ID);
    gx_userInterface_keyState.courseSportsKeyState = handler_keypad_state_get(KEY_SPORTS_ID);
    gx_userInterface_keyState.courseJeansKeyState = handler_keypad_state_get(KEY_JEANS_ID);
#ifdef AUTO_DETERGENT_HEATER_MODEL
    gx_userInterface_keyState.courseECOKeyState = handler_keypad_state_get(KEY_ECO_ID);
    gx_userInterface_keyState.courseAlergyKeyState = handler_keypad_state_get(KEY_ALERGY_ID);
#else
    gx_userInterface_keyState.courseWoolKeyState = handler_keypad_state_get(KEY_WOOL_ID);
    gx_userInterface_keyState.courseRapidWashKeyState = handler_keypad_state_get(KEY_RAPIDWASH_ID);
#endif
#endif

    gx_userInterface_keyState.waterLevelKeyState = handler_keypad_state_get(KEY_WATER_LEVEL_ID);
    gx_userInterface_keyState.waterTempKeyState = handler_keypad_state_get(KEY_WATER_TEMP_ID);
    gx_userInterface_keyState.washKeyState = handler_keypad_state_get(KEY_WASH_ID);
    gx_userInterface_keyState.delayStartKeyState = handler_keypad_state_get(KEY_DELAY_START_ID);
    gx_userInterface_keyState.rinseKeyState = handler_keypad_state_get(KEY_RINSE_ID);
    gx_userInterface_keyState.spinKeyState = handler_keypad_state_get(KEY_SPIN_ID);
    gx_userInterface_keyState.tubCleanKeyState = handler_keypad_state_get(KEY_TUBCLEAN_ID);
    gx_userInterface_keyState.stainLevelKeyState = handler_keypad_state_get(KEY_STAIN_LEVEL_ID);

#ifdef AUTO_DETERGENT_HEATER_MODEL
    gx_userInterface_keyState.gelDetergentKeyState = handler_keypad_state_get(KEY_GEL_DETERGENT_ID);
    gx_userInterface_keyState.steamTechKeyState = handler_keypad_state_get(KEY_STEAM_TECH_ID);
#else
    gx_userInterface_keyState.soakKeyState = handler_keypad_state_get(KEY_SOAK_ID);
    gx_userInterface_keyState.extraSpinKeyState = handler_keypad_state_get(KEY_EXTRA_SPIN_ID);
#endif

    gx_userInterface_keyState.startPauseKeyState = handler_keypad_state_get(KEY_START_PAUSE_ID);

#ifdef COURSE_SELECTION_SINGLE_BUTTON
    LOG('u', 'v', "C:%d  WO:%d  WD:%d  DO:%d  Dryness:%d  DT:%d  DStart:%d  K:%d  S:%d  R:%d  W:%d  T:%d  ST:%d  WL:%d  SP:%d", gx_userInterface_keyEdgeState.courseKeyEdgeState, gx_userInterface_keyEdgeState.washOnlyKeyEdgeState, gx_userInterface_keyEdgeState.washnDryKeyEdgeState, gx_userInterface_keyEdgeState.dryOnlyKeyEdgeState, gx_userInterface_keyEdgeState.steamKeyEdgeState, gx_userInterface_keyEdgeState.dryTimeKeyEdgeState, gx_userInterface_keyEdgeState.delayStartKeyEdgeState, gx_userInterface_keyEdgeState.soakKeyEdgeState, gx_userInterface_keyEdgeState.spinKeyEdgeState, gx_userInterface_keyEdgeState.rinseKeyEdgeState, gx_userInterface_keyEdgeState.washKeyEdgeState, gx_userInterface_keyEdgeState.waterTempKeyEdgeState, gx_userInterface_keyEdgeState.stainLevelKeyEdgeState, gx_userInterface_keyEdgeState.waterLevelKeyEdgeState, gx_userInterface_keyEdgeState.startPauseKeyEdgeState);
#elif defined(COURSE_SELECTION_MULTIPLE_BUTTONS)
#ifdef AUTO_DETERGENT_HEATER_MODEL
    LOG('u', 'v', "C0:%d  C1:%d  C2:%d  C3:%d  C4:%d  C5:%d  C6:%d  C7:%d  C8:%d  C9:%d  C10:%d  C11:%d  C12:%d  DStart:%d  K:%d  S:%d  R:%d  W:%d  T:%d  ST:%d STEC:%d  WL:%d  SP:%d", gx_userInterface_keyState.courseCottonKeyState, gx_userInterface_keyState.courseWhiteKeyState, gx_userInterface_keyState.courseBeddingKeyState, gx_userInterface_keyState.courseMixKeyState, gx_userInterface_keyState.courseDarksKeyState, gx_userInterface_keyState.courseBabyCareKeyState, gx_userInterface_keyState.courseSensitiveKeyState, gx_userInterface_keyState.courseLightKeyState, gx_userInterface_keyState.courseECOKeyState, gx_userInterface_keyState.courseSportsKeyState, gx_userInterface_keyState.courseJeansKeyState, gx_userInterface_keyState.courseAlergyKeyState, gx_userInterface_keyState.tubCleanKeyState, gx_userInterface_keyState.delayStartKeyState, gx_userInterface_keyState.gelDetergentKeyState, gx_userInterface_keyState.spinKeyState, gx_userInterface_keyState.rinseKeyState, gx_userInterface_keyState.washKeyState, gx_userInterface_keyState.waterTempKeyState, gx_userInterface_keyState.stainLevelKeyState, gx_userInterface_keyState.steamTechKeyState, gx_userInterface_keyState.waterLevelKeyState, gx_userInterface_keyState.startPauseKeyState);
#else
    LOG('u', 'v', "C0:%d  C1:%d  C2:%d  C3:%d  C4:%d  C5:%d  C6:%d  C7:%d  C8:%d  C9:%d  C10:%d  C11:%d  C12:%d  DStart:%d  K:%d  S:%d  R:%d  W:%d  T:%d  ST:%d STEC:%d  WL:%d  SP:%d", gx_userInterface_keyState.courseCottonKeyState, gx_userInterface_keyState.courseWhiteKeyState, gx_userInterface_keyState.courseBeddingKeyState, gx_userInterface_keyState.courseMixKeyState, gx_userInterface_keyState.courseDarksKeyState, gx_userInterface_keyState.courseBabyCareKeyState, gx_userInterface_keyState.courseSensitiveKeyState, gx_userInterface_keyState.courseLightKeyState, gx_userInterface_keyState.courseWoolKeyState, gx_userInterface_keyState.courseSportsKeyState, gx_userInterface_keyState.courseJeansKeyState, gx_userInterface_keyState.courseRapidWashKeyState, gx_userInterface_keyState.tubCleanKeyState, gx_userInterface_keyState.delayStartKeyState, gx_userInterface_keyState.soakKeyState, gx_userInterface_keyState.spinKeyState, gx_userInterface_keyState.rinseKeyState, gx_userInterface_keyState.washKeyState, gx_userInterface_keyState.waterTempKeyState, gx_userInterface_keyState.stainLevelKeyState, gx_userInterface_keyState.extraSpinKeyState, gx_userInterface_keyState.waterLevelKeyState, gx_userInterface_keyState.startPauseKeyState);
#endif
#endif
}
