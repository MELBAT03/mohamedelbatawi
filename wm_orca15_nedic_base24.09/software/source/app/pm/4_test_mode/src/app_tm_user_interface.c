#include "def.h"
#include "app_user_interface.h"
#include "app_buzzer.h"
#include "app_unbalance.h"
#include "app_drain.h"

#include "app_test_mode.h"

#include "handlers/input/inc/handler_keypad.h"

#include "os.h"

static void userInterface_testModeKeyConfig_set(app_tm_keyConfig_t *keyConfig);

static app_tm_keyConfig_t gx_userInterface_testModeKeyConfig;

void app_tm_userInterface_process(uint32_t period)
{
    // do nothing
}

void app_tm_userInterface_setup(void)
{
    app_tm_keyConfig_t testModeKeyConfig;
    static uint8_t keyPressCnt = 0;

    if (app_userInterface_keyState_get()->waterLevelKeyState != HANDLER_KEY_RELEASED)
    {
        if (app_userInterface_keyEdgeState_get()->courseWhiteKeyEdgeState == HANDLER_KEY_PRESSED)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            keyPressCnt++;
            if (keyPressCnt >= 2U)
            {
                userInterface_testModeKeyConfig_set(NULL);
                app_tm_type_set(APP_TM_WATER_LEVEL_CALIBRATION);
                keyPressCnt = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
            }
        }
        else if (app_userInterface_keyEdgeState_get()->courseDarksKeyEdgeState == HANDLER_KEY_PRESSED)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            keyPressCnt++;
            if (keyPressCnt >= 2U)
            {
                userInterface_testModeKeyConfig_set(NULL);
                app_tm_type_set(APP_TM_WEIGHT_CALIBRATION);
                keyPressCnt = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
            }
        }
        else if (app_userInterface_keyEdgeState_get()->courseSportsKeyEdgeState == HANDLER_KEY_PRESSED)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            keyPressCnt++;
            if (keyPressCnt >= 2U)
            {
                userInterface_testModeKeyConfig_set(NULL);
                app_tm_type_set(APP_TM_LV_CALIBRATION);
                keyPressCnt = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
            }
        }
        else if (app_userInterface_keyEdgeState_get()->courseLightKeyEdgeState == HANDLER_KEY_PRESSED)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            keyPressCnt++;
            if (keyPressCnt >= 2)
            {
                testModeKeyConfig.fnSW[0] = &(app_userInterface_keyEdgeState_get()->waterLevelKeyEdgeState);
                testModeKeyConfig.fnNum = 1;
                userInterface_testModeKeyConfig_set(&testModeKeyConfig);
                app_tm_type_set(APP_TM_ELECTRICAL_PARTS);
                keyPressCnt = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
            }
        }
        else if (app_userInterface_keyEdgeState_get()->courseCottonKeyEdgeState == HANDLER_KEY_PRESSED)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            keyPressCnt++;
            if (keyPressCnt >= 2)
            {
                testModeKeyConfig.fnSW[0] = &(app_userInterface_keyEdgeState_get()->startPauseKeyEdgeState);
                testModeKeyConfig.fnNum = 1;
                userInterface_testModeKeyConfig_set(&testModeKeyConfig);
                app_tm_type_set(APP_TM_UNBALANCE_SPIN);
                keyPressCnt = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
            }
        }
        else if (app_userInterface_keyEdgeState_get()->courseBeddingKeyEdgeState == HANDLER_KEY_PRESSED)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            keyPressCnt++;
            if (keyPressCnt >= 2)
            {
                testModeKeyConfig.fnSW[0] = &(app_userInterface_keyEdgeState_get()->waterLevelKeyEdgeState);
                testModeKeyConfig.fnNum = 1;
                userInterface_testModeKeyConfig_set(&testModeKeyConfig);
                app_tm_type_set(APP_TM_WATER_LEVEL_FREQ);
                keyPressCnt = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
            }
        }
        else if (app_userInterface_keyEdgeState_get()->courseBabyCareKeyEdgeState == HANDLER_KEY_PRESSED)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            keyPressCnt++;
            if (keyPressCnt >= 2)
            {
                testModeKeyConfig.fnSW[0] = &(app_userInterface_keyEdgeState_get()->courseWhiteKeyEdgeState);
                testModeKeyConfig.fnSW[1] = &(app_userInterface_keyEdgeState_get()->courseDarksKeyEdgeState);
                testModeKeyConfig.fnSW[2] = &(app_userInterface_keyEdgeState_get()->courseLightKeyEdgeState);
                testModeKeyConfig.fnSW[3] = &(app_userInterface_keyEdgeState_get()->courseCottonKeyEdgeState);
                testModeKeyConfig.fnSW[4] = &(app_userInterface_keyEdgeState_get()->courseBeddingKeyEdgeState);
#ifdef AUTO_DETERGENT_HEATER_MODEL
                testModeKeyConfig.fnSW[5] = &(app_userInterface_keyEdgeState_get()->courseECOKeyEdgeState);
                testModeKeyConfig.fnSW[10] = &(app_userInterface_keyEdgeState_get()->courseAlergyKeyEdgeState);
#else
                testModeKeyConfig.fnSW[5] = &(app_userInterface_keyEdgeState_get()->courseWoolKeyEdgeState);
                testModeKeyConfig.fnSW[10] = &(app_userInterface_keyEdgeState_get()->courseRapidWashKeyEdgeState);
#endif
                testModeKeyConfig.fnSW[6] = &(app_userInterface_keyEdgeState_get()->courseJeansKeyEdgeState);
                testModeKeyConfig.fnSW[7] = &(app_userInterface_keyEdgeState_get()->courseMixKeyEdgeState);
                testModeKeyConfig.fnSW[8] = &(app_userInterface_keyEdgeState_get()->courseDelicateKeyEdgeState);
                testModeKeyConfig.fnSW[9] = &(app_userInterface_keyEdgeState_get()->courseSportsKeyEdgeState);
                testModeKeyConfig.fnSW[11] = &(app_userInterface_keyEdgeState_get()->waterLevelKeyEdgeState);
                testModeKeyConfig.fnSW[12] = &(app_userInterface_keyEdgeState_get()->waterTempKeyEdgeState);
                testModeKeyConfig.fnSW[13] = &(app_userInterface_keyEdgeState_get()->washKeyEdgeState);
                testModeKeyConfig.fnSW[14] = &(app_userInterface_keyEdgeState_get()->delayStartKeyEdgeState);
                testModeKeyConfig.fnSW[15] = &(app_userInterface_keyEdgeState_get()->rinseKeyEdgeState);
#ifdef AUTO_DETERGENT_HEATER_MODEL
                testModeKeyConfig.fnSW[16] = &(app_userInterface_keyEdgeState_get()->gelDetergentKeyEdgeState);
                testModeKeyConfig.fnSW[20] = &(app_userInterface_keyEdgeState_get()->steamTechKeyEdgeState);
#else
                testModeKeyConfig.fnSW[16] = &(app_userInterface_keyEdgeState_get()->soakKeyEdgeState);
                testModeKeyConfig.fnSW[20] = &(app_userInterface_keyEdgeState_get()->extraSpinKeyEdgeState);
#endif
                testModeKeyConfig.fnSW[17] = &(app_userInterface_keyEdgeState_get()->spinKeyEdgeState);
                testModeKeyConfig.fnSW[18] = &(app_userInterface_keyEdgeState_get()->tubCleanKeyEdgeState);
                testModeKeyConfig.fnSW[19] = &(app_userInterface_keyEdgeState_get()->stainLevelKeyEdgeState);
                testModeKeyConfig.fnSW[21] = &(app_userInterface_keyEdgeState_get()->startPauseKeyEdgeState);
                testModeKeyConfig.fnSW[22] = NULL;
                testModeKeyConfig.fnSW[23] = NULL;
                testModeKeyConfig.fnSW[24] = NULL;
                testModeKeyConfig.fnNum = 25;
                userInterface_testModeKeyConfig_set(&testModeKeyConfig);
                app_tm_type_set(APP_TM_PCB);
                keyPressCnt = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
            }
        }
#ifdef AUTO_DETERGENT_HEATER_MODEL
        else if (app_userInterface_keyEdgeState_get()->courseECOKeyEdgeState == HANDLER_KEY_PRESSED)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            keyPressCnt++;
            if (keyPressCnt >= 2)
            {
                testModeKeyConfig.fnSW[0] = &(app_userInterface_keyEdgeState_get()->startPauseKeyEdgeState);
                testModeKeyConfig.fnSW[1] = &(app_userInterface_keyEdgeState_get()->waterTempKeyEdgeState);
                testModeKeyConfig.fnSW[2] = &(app_userInterface_keyEdgeState_get()->spinKeyEdgeState);
                testModeKeyConfig.fnSW[3] = &(app_userInterface_keyState_get()->washKeyState);
                testModeKeyConfig.fnNum = 4;
                userInterface_testModeKeyConfig_set(&testModeKeyConfig);
                app_tm_type_set(APP_TM_ERROR_COUNT_DISPLAY);
                keyPressCnt = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
            }
        }
#else
        else if (app_userInterface_keyEdgeState_get()->courseWoolKeyEdgeState == HANDLER_KEY_PRESSED)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            keyPressCnt++;
            if (keyPressCnt >= 2)
            {
                testModeKeyConfig.fnSW[0] = &(app_userInterface_keyEdgeState_get()->startPauseKeyEdgeState);
                testModeKeyConfig.fnSW[1] = &(app_userInterface_keyEdgeState_get()->waterTempKeyEdgeState);
                testModeKeyConfig.fnSW[2] = &(app_userInterface_keyEdgeState_get()->spinKeyEdgeState);
                testModeKeyConfig.fnSW[3] = &(app_userInterface_keyState_get()->washKeyState);
                testModeKeyConfig.fnNum = 4;
                userInterface_testModeKeyConfig_set(&testModeKeyConfig);
                app_tm_type_set(APP_TM_ERROR_COUNT_DISPLAY);
                keyPressCnt = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
            }
        }
#endif
        else if (app_userInterface_keyEdgeState_get()->courseJeansKeyEdgeState == HANDLER_KEY_PRESSED)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            keyPressCnt++;
            if (keyPressCnt >= 2)
            {
                testModeKeyConfig.fnSW[0] = &(app_userInterface_keyEdgeState_get()->startPauseKeyEdgeState);
                testModeKeyConfig.fnSW[1] = &(app_userInterface_keyEdgeState_get()->waterTempKeyEdgeState);
                testModeKeyConfig.fnSW[2] = &(app_userInterface_keyEdgeState_get()->spinKeyEdgeState);
                testModeKeyConfig.fnSW[3] = &(app_userInterface_keyState_get()->washKeyState);
                testModeKeyConfig.fnNum = 4;
                userInterface_testModeKeyConfig_set(&testModeKeyConfig);
                app_tm_type_set(APP_TM_CYCLE_COUNT_DISPLAY);
                keyPressCnt = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
            }
        }
        else if (app_userInterface_keyEdgeState_get()->courseMixKeyEdgeState == HANDLER_KEY_PRESSED)
        {
            keyPressCnt++;
            if (keyPressCnt >= 6 && app_drain_shortDrainFlag_get() == 0)
            {
                keyPressCnt = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
                app_drain_shortDrainFlag_set(1);
            }
            else if (keyPressCnt >= 6 && app_drain_shortDrainFlag_get() == 1)
            {
                keyPressCnt = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
                app_drain_shortDrainFlag_set(0);
            }
            else
            {
                // do nothing
            }
        }
        else if (app_userInterface_keyEdgeState_get()->courseDelicateKeyEdgeState == HANDLER_KEY_PRESSED)
        {
            keyPressCnt++;
            if (keyPressCnt >= 6U && app_unbalance_detection_get() == 0U)
            {
                keyPressCnt = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
                app_unbalance_detection_set(1);
            }
            else if (keyPressCnt >= 6U && app_unbalance_detection_get() == 1U)
            {
                keyPressCnt = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
                app_unbalance_detection_set(0);
            }
        }
        else
        {
            // do nothing
        }
    }
    else if ((app_userInterface_keyEdgeState_get()->waterTempKeyEdgeState == HANDLER_KEY_PRESSED && app_userInterface_keyState_get()->rinseKeyState == HANDLER_KEY_PRESSED) || (app_userInterface_keyState_get()->waterTempKeyState == HANDLER_KEY_PRESSED && app_userInterface_keyEdgeState_get()->rinseKeyEdgeState == HANDLER_KEY_PRESSED))
    {
        testModeKeyConfig.fnSW[0] = &(app_userInterface_keyEdgeState_get()->startPauseKeyEdgeState);
        testModeKeyConfig.fnSW[1] = &(app_userInterface_keyEdgeState_get()->courseCottonKeyEdgeState);
        testModeKeyConfig.fnSW[2] = &(app_userInterface_keyState_get()->washKeyState);
        testModeKeyConfig.fnSW[3] = &(app_userInterface_keyState_get()->spinKeyState);
        testModeKeyConfig.fnSW[4] = &(app_userInterface_keyState_get()->waterTempKeyState);
        testModeKeyConfig.fnSW[5] = &(app_userInterface_keyState_get()->rinseKeyState);
        testModeKeyConfig.fnSW[6] = &(app_userInterface_keyState_get()->waterLevelKeyState);
        testModeKeyConfig.fnNum = 7;
        userInterface_testModeKeyConfig_set(&testModeKeyConfig);
        app_tm_type_set(APP_TM_MIDDLE_INSPECTION);
        app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
    }
    else if ((app_userInterface_keyEdgeState_get()->waterTempKeyEdgeState == HANDLER_KEY_PRESSED && app_userInterface_keyState_get()->spinKeyState == HANDLER_KEY_PRESSED) || (app_userInterface_keyState_get()->waterTempKeyState == HANDLER_KEY_PRESSED && app_userInterface_keyEdgeState_get()->spinKeyEdgeState == HANDLER_KEY_PRESSED))
    {
        testModeKeyConfig.fnSW[0] = &(app_userInterface_keyEdgeState_get()->startPauseKeyEdgeState);
        testModeKeyConfig.fnSW[1] = &(app_userInterface_keyEdgeState_get()->rinseKeyEdgeState);
        testModeKeyConfig.fnSW[2] = &(app_userInterface_keyEdgeState_get()->waterLevelKeyEdgeState);
        testModeKeyConfig.fnSW[3] = &(app_userInterface_keyEdgeState_get()->waterTempKeyEdgeState);
        testModeKeyConfig.fnSW[4] = &(app_userInterface_keyState_get()->courseCottonKeyState);
        testModeKeyConfig.fnSW[5] = &(app_userInterface_keyEdgeState_get()->washKeyEdgeState);
        testModeKeyConfig.fnSW[6] = &(app_userInterface_keyEdgeState_get()->spinKeyEdgeState);
        testModeKeyConfig.fnNum = 7;
        userInterface_testModeKeyConfig_set(&testModeKeyConfig);
        app_tm_type_set(APP_TM_FINAL_INSPECTION);
        app_buzzer_tone_set(APP_BUZZER_TONE_CHILDLOCK_ACTIVE);
    }
    else
    {
        // do nothing
    }
}

app_tm_keyConfig_t app_tm_userInterface_keyConfig_get(void)
{
    return gx_userInterface_testModeKeyConfig;
}

static void userInterface_testModeKeyConfig_set(app_tm_keyConfig_t *keyConfig)
{
    if (keyConfig != NULL && keyConfig->fnNum <= APP_TEST_MODE_FUNCTION_TEST_MAX_NUM)
    {
        gx_userInterface_testModeKeyConfig.fnNum = keyConfig->fnNum;
        for (uint8_t i = 0; i < gx_userInterface_testModeKeyConfig.fnNum; i++)
        {
            gx_userInterface_testModeKeyConfig.fnSW[i] = keyConfig->fnSW[i];
        }
    }
}
