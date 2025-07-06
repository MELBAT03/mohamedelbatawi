#ifndef APP_USER_INTERFACE_H
#define APP_USER_INTERFACE_H

#include "def.h"
#include "config.h"

#include "app_nm_user_interface.h"

#include "handlers/input/inc/handler_keypad.h"

#if APP_TICK_MULTIPLIER == 1
#define UI_ALTERNATE_FUNCTION_TIMEOUT_MSEC (3000U)
#else
#define UI_ALTERNATE_FUNCTION_TIMEOUT_MSEC (300000U)
#endif

typedef enum app_userInterface_specialFeatures
{
    UI_ALTERNATE_NONE,
    UI_ALTERNATE_DELAY_START,
    UI_ALTERNATE_SOAK,
    UI_ALTERNATE_ANTI_WRINKLE,
    UI_ALTERNATE_EXTRA_SPIN,
    UI_ALTERNATE_WASH,
    UI_ALTERNATE_RINSE,
    UI_ALTERNATE_SPIN
} app_userInterface_specialFeatures_t;

#define APP_NORMAL_MODE_UI_DEFAULT_COURSE     (APP_NORMAL_MODE_UI_COURSE_COTTON)
#define APP_NORMAL_MODE_UI_DEFAULT_OPERATION  (APP_NORMAL_MODE_UI_OPERATION_WASH_ONLY)
#define APP_NORMAL_MODE_UI_DEFAUL_DELAY_START (APP_NORMAL_MODE_UI_DELAY_START_TIME_0_HOUR)
#define APP_NORMAL_MODE_UI_DEFAUL_CHILD_LOCK  (APP_NORMAL_MODE_UI_CHILD_LOCK_OFF)
#define APP_NORMAL_MODE_UI_DEFAULT_CYCLE_TYPE (APP_NORMAL_MODE_UI_SINGLE_CYCLE)
#define APP_NORMAL_MODE_UI_DEFAULT_TUBCLEAN   (APP_NORMAL_MODE_UI_TUBCLEAN_OFF)

typedef struct app_userInterface_keyEdgeState
{
#ifdef COURSE_SELECTION_SINGLE_BUTTON
    handler_keypad_state_t courseKeyEdgeState;
#elif defined(COURSE_SELECTION_MULTIPLE_BUTTONS)
    handler_keypad_state_t courseCottonKeyEdgeState;
    handler_keypad_state_t courseWhiteKeyEdgeState;
    handler_keypad_state_t courseBeddingKeyEdgeState;
    handler_keypad_state_t courseMixKeyEdgeState;
    handler_keypad_state_t courseDarksKeyEdgeState;
    handler_keypad_state_t courseBabyCareKeyEdgeState;
    handler_keypad_state_t courseDelicateKeyEdgeState;
    handler_keypad_state_t courseLightKeyEdgeState;
    handler_keypad_state_t courseSportsKeyEdgeState;
    handler_keypad_state_t courseJeansKeyEdgeState;
#ifdef AUTO_DETERGENT_HEATER_MODEL
    handler_keypad_state_t courseECOKeyEdgeState;
    handler_keypad_state_t courseAlergyKeyEdgeState;
#else
    handler_keypad_state_t courseWoolKeyEdgeState;
    handler_keypad_state_t courseRapidWashKeyEdgeState;
#endif
#endif

    handler_keypad_state_t waterLevelKeyEdgeState;
    handler_keypad_state_t waterTempKeyEdgeState;
    handler_keypad_state_t washKeyEdgeState;
    handler_keypad_state_t delayStartKeyEdgeState;
    handler_keypad_state_t rinseKeyEdgeState;
    handler_keypad_state_t spinKeyEdgeState;
    handler_keypad_state_t tubCleanKeyEdgeState;
    handler_keypad_state_t stainLevelKeyEdgeState;

#ifdef AUTO_DETERGENT_HEATER_MODEL
    handler_keypad_state_t gelDetergentKeyEdgeState;
    handler_keypad_state_t steamTechKeyEdgeState;
#else
    handler_keypad_state_t soakKeyEdgeState;
    handler_keypad_state_t extraSpinKeyEdgeState;
#endif

    handler_keypad_state_t startPauseKeyEdgeState;
} app_userInterface_keyEdgeState_t;

typedef struct app_userInterface_keyState
{
#ifdef COURSE_SELECTION_SINGLE_BUTTON
    handler_keypad_state_t courseKeyState;
#elif defined(COURSE_SELECTION_MULTIPLE_BUTTONS)
    handler_keypad_state_t courseCottonKeyState;
    handler_keypad_state_t courseWhiteKeyState;
    handler_keypad_state_t courseBeddingKeyState;
    handler_keypad_state_t courseMixKeyState;
    handler_keypad_state_t courseDarksKeyState;
    handler_keypad_state_t courseBabyCareKeyState;
    handler_keypad_state_t courseSensitiveKeyState;
    handler_keypad_state_t courseLightKeyState;
    handler_keypad_state_t courseSportsKeyState;
    handler_keypad_state_t courseJeansKeyState;
#ifdef AUTO_DETERGENT_HEATER_MODEL
    handler_keypad_state_t courseECOKeyState;
    handler_keypad_state_t courseAlergyKeyState;
#else
    handler_keypad_state_t courseWoolKeyState;
    handler_keypad_state_t courseRapidWashKeyState;
#endif
#endif

    handler_keypad_state_t waterLevelKeyState;
    handler_keypad_state_t waterTempKeyState;
    handler_keypad_state_t washKeyState;
    handler_keypad_state_t delayStartKeyState;
    handler_keypad_state_t rinseKeyState;
    handler_keypad_state_t spinKeyState;
    handler_keypad_state_t tubCleanKeyState;
    handler_keypad_state_t stainLevelKeyState;

#ifdef AUTO_DETERGENT_HEATER_MODEL
    handler_keypad_state_t gelDetergentKeyState;
    handler_keypad_state_t steamTechKeyState;
#else
    handler_keypad_state_t soakKeyState;
    handler_keypad_state_t extraSpinKeyState;
#endif

    handler_keypad_state_t startPauseKeyState;
} app_userInterface_keyState_t;

typedef struct app_userInterface_data
{
    app_nm_userInterface_data_t normalMode;
} app_userInterface_data_t;

void app_userInterface_update(uint32_t period);

app_userInterface_specialFeatures_t app_userInterface_state_get(void);
app_userInterface_specialFeatures_t app_userInterface_state_set(app_userInterface_specialFeatures_t value);

const app_userInterface_data_t *app_userInterface_data_get(void);
app_userInterface_data_t *app_userInterface_data_set(void);

app_userInterface_keyEdgeState_t *app_userInterface_keyEdgeState_get(void);
app_userInterface_keyState_t *app_userInterface_keyState_get(void);

uint32_t app_userInterface_MultipleEdgePressedKeysCnt_get(void);
uint32_t app_userInterface_PressedKeysCnt_get(void);

handler_keypad_state_t app_userInterface_courseKeyEdgeState_get(void);
handler_keypad_state_t app_userInterface_courseKeyState_get(void);

void app_userInterface_childLock_check(uint32_t period);

#endif
