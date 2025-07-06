#include "def.h"

#include "app_standby_user_interface.h"

#include "app_nm_tubclean_user_interface.h"

#include "app_buzzer.h"
#include "app_memory.h"
#include "app_fill.h"
#include "app_display.h"
#include "app_nm_user_interface.h"
#include "app_user_interface.h"

#include "app_test_mode.h"
#include "app_tm_user_interface.h"

#include "handlers/input/inc/handler_detergent_sensor.h"

typedef enum userInterface_mode
{
    UI_NORMAL_MODE = 0,
    UI_TEST_MODE = 1
} userInterface_mode_t;

typedef enum standby_first_press
{
    STANDBY_FIRST_PRESS_WASH,
    STANDBY_FIRST_PRESS_RINSE,
    STANDBY_FIRST_PRESS_SPIN,
    STANDBY_FIRST_PRESS_OTHERS
} standby_first_press_t;

static void userInterface_standby_handle(app_memory_shutdownState_t memState);
static void wash_userInterface_standby_handle(void);
#ifdef DRYER_ENABLED
static void dry_userInterface_standby_handle(void);
#endif

static void standby_operation_update(void);
static void standby_course_update(void);
static void standby_delayStart_update(void);
static void standby_tubclean_update(void);

static void standby_waterTemp_update(void);
static void standby_waterLevel_update(void);
static void standby_soak_update(void);
static void standby_wash_update(void);
static void standby_steam_update(void);
static void standby_rinse_update(void);
static void standby_extraRinse_update(void);
static void standby_spin_update(void);
static void standby_superSpin_update(void);
static void standby_soilLevel_update(void);
static void standby_antiWrinkle_update(void);
static void standby_steamTech_update(void);
static void standby_gelDetergent_update(void);
static void standby_heating_update(void);

#ifdef DRYER_ENABLED
static void standby_dryTime_update(void);
static void standby_drynessIntensity_update(void);
#endif

static void wash_userInterface_refresh(void);
#ifdef DRYER_ENABLED
static void dry_userInterface_refresh(void);
#endif

static void standby_userInterface_firstPress_handle(standby_first_press_t selection);

static userInterface_mode_t userInterface_modeType_detect(uint32_t period);

void app_standby_userInterface_process(uint32_t period)
{
    if (MEMORY_SHUTDOWN_ABNORMAL != app_memory_shutdownState_get())
    {
        if (UI_TEST_MODE == userInterface_modeType_detect(period))
        {
            app_tm_userInterface_setup();
        }
        else
        {
            userInterface_standby_handle(app_memory_shutdownState_get());
        }

        app_userInterface_childLock_check(period);
    }
}

static void userInterface_standby_handle(app_memory_shutdownState_t memState)
{
    if (MEMORY_SHUTDOWN_NORMAL == memState)
    {
        standby_operation_update();
        standby_course_update();
        standby_delayStart_update();
        standby_tubclean_update();

        wash_userInterface_standby_handle();
#ifdef DRYER_ENABLED
        dry_userInterface_standby_handle();
#endif
    }
}

static void wash_userInterface_standby_handle(void)
{
    standby_waterTemp_update();
    standby_waterLevel_update();
    standby_soak_update();
    standby_wash_update();
    standby_steam_update();
    standby_rinse_update();
    standby_extraRinse_update();
    standby_spin_update();
    standby_superSpin_update();
    standby_soilLevel_update();
    standby_antiWrinkle_update();
    standby_steamTech_update();
    standby_gelDetergent_update();
    standby_heating_update();

    if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF)
    {

        if ((APP_NORMAL_MODE_UI_WASH_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.washSelection) && (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0 == app_userInterface_data_get()->normalMode.wash.waterLevelSelection) && ((APP_NORMAL_MODE_UI_RINSE_0_TIMES != app_userInterface_data_get()->normalMode.wash.rinseSelection) || (APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN != app_userInterface_data_get()->normalMode.wash.spinSelection)))
        {
            app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT;
            app_userInterface_data_set()->normalMode.wash.steamTechSelection = APP_NORMAL_MODE_UI_STEAM_TECH_OFF;
            app_userInterface_data_set()->normalMode.wash.waterHeatingSelection = APP_NORMAL_MODE_UI_WATER_HEATING_OFF;
        }

        if ((APP_NORMAL_MODE_UI_WASH_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.washSelection) && (APP_NORMAL_MODE_UI_RINSE_0_TIMES != app_userInterface_data_get()->normalMode.wash.rinseSelection))
        {
            app_userInterface_data_set()->normalMode.wash.waterHeatingSelection = APP_NORMAL_MODE_UI_WATER_HEATING_OFF;
            app_userInterface_data_set()->normalMode.wash.steamTechSelection = APP_NORMAL_MODE_UI_STEAM_TECH_OFF;
            app_userInterface_data_set()->normalMode.wash.waterTempSelection = APP_NORMAL_MODE_UI_WATER_TEMP_COLD;
        }
        else if ((APP_NORMAL_MODE_UI_WASH_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.washSelection) && (APP_NORMAL_MODE_UI_RINSE_0_TIMES == app_userInterface_data_get()->normalMode.wash.rinseSelection) && (APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN != app_userInterface_data_get()->normalMode.wash.spinSelection))
        {
            app_userInterface_data_set()->normalMode.wash.waterTempSelection = APP_NORMAL_MODE_UI_WATER_TEMP_NONE;
        }
        else if ((APP_NORMAL_MODE_UI_WASH_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.washSelection) && (APP_NORMAL_MODE_UI_RINSE_0_TIMES == app_userInterface_data_get()->normalMode.wash.rinseSelection) && (APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.spinSelection))
        {
            app_userInterface_data_set()->normalMode.wash.waterTempSelection = APP_NORMAL_MODE_UI_WATER_TEMP_NONE;
        }
        else
        {
            // do nothing
        }

        if (app_fill_autoDetergentState_get() == APP_FILL_AUTO_DETERGENT_LOW)
        {
            app_userInterface_data_set()->normalMode.wash.gelDetergentSelection = APP_NORMAL_MODE_UI_GEL_DETERGENT_OFF;
        }
    }
}

#ifdef DRYER_ENABLED
static void dry_userInterface_standby_handle(void)
{
    standby_dryTime_update();
    standby_drynessIntensity_update();
}
#endif

static void standby_operation_update(void)
{
#ifdef DRYER_ENABLED
    if ((HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->dryOnlyKeyEdgeState) && (APP_NORMAL_MODE_UI_OPERATION_DRY_ONLY != app_userInterface_data_set()->normalMode.common.operationSelection))
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
        standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);

        app_userInterface_data_set()->normalMode.common.operationSelection = APP_NORMAL_MODE_UI_OPERATION_DRY_ONLY;
        app_userInterface_data_set()->normalMode.common.courseSelection = APP_NORMAL_MODE_UI_DEFAULT_COURSE;

        wash_userInterface_refresh();
        dry_userInterface_refresh();
    }
    else if ((HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->washOnlyKeyEdgeState) && (APP_NORMAL_MODE_UI_OPERATION_WASH_ONLY != app_userInterface_data_set()->normalMode.common.operationSelection))
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
        standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);

        app_userInterface_data_set()->normalMode.common.operationSelection = APP_NORMAL_MODE_UI_OPERATION_WASH_ONLY;
        app_userInterface_data_set()->normalMode.common.courseSelection = APP_NORMAL_MODE_UI_DEFAULT_COURSE;

        wash_userInterface_refresh();
        dry_userInterface_refresh();
    }
    else if ((HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->washnDryKeyEdgeState) && (APP_NORMAL_MODE_UI_OPERATION_WASH_DRY != app_userInterface_data_set()->normalMode.common.operationSelection))
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
        standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);

        app_userInterface_data_set()->normalMode.common.operationSelection = APP_NORMAL_MODE_UI_OPERATION_WASH_DRY;
        app_userInterface_data_set()->normalMode.common.courseSelection = APP_NORMAL_MODE_UI_DEFAULT_COURSE;

        wash_userInterface_refresh();
        dry_userInterface_refresh();
    }
    else
    {
        // do nothing
    }
#endif
}
static void standby_course_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_courseKeyEdgeState_get())
    {
        (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
        standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);

        app_userInterface_data_set()->normalMode.common.courseSelection = app_nm_userInterface_nextCourseInOperation_get(app_userInterface_data_get()->normalMode.common.courseSelection);

        wash_userInterface_refresh();
#ifdef DRYER_ENABLED
        dry_userInterface_refresh();
#endif
    }
}
static void standby_delayStart_update(void)
{
    static uint32_t timeoutMSec = 0;
    static uint8_t oneTime_flag = 0;
    static uint32_t doublePressTimeoutMSec = 0;

    if (HANDLER_KEY_LONG_PRESSED == app_userInterface_keyState_get()->delayStartKeyState)
    {
        timeoutMSec += 100;
        if (timeoutMSec >= 600)
        {
            timeoutMSec = 0;

            if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF)
            {
                if (oneTime_flag == 0)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    oneTime_flag = 1;
                }
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);

                if (app_userInterface_state_set(UI_ALTERNATE_DELAY_START) == UI_ALTERNATE_DELAY_START)
                {
                    if (APP_NORMAL_MODE_UI_DELAY_START_TIME_END > app_userInterface_data_get()->normalMode.common.delayStartHourSelection)
                    {
                        app_userInterface_data_set()->normalMode.common.delayStartHourSelection += APP_NORMAL_MODE_UI_DELAY_START_TIME_1_HOUR;
                    }
                    else
                    {
                        if (oneTime_flag == 0)
                        {
                            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                            oneTime_flag = 1;
                        }
                        app_userInterface_data_set()->normalMode.common.delayStartHourSelection = APP_NORMAL_MODE_UI_DELAY_START_TIME_0_HOUR;
                    }
                }
            }
            else
            {
                if (oneTime_flag == 0)
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
                    oneTime_flag = 1;
                }
            }
        }
    }
    else if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->delayStartKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF)
        {
            if (doublePressTimeoutMSec >= 100 && doublePressTimeoutMSec <= 400)
            {
                doublePressTimeoutMSec = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);

                app_userInterface_data_set()->normalMode.common.delayStartHourSelection = APP_NORMAL_MODE_UI_DELAY_START_TIME_0_HOUR;
            }
            else
            {
                doublePressTimeoutMSec = 0;
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);

                if (app_userInterface_state_set(UI_ALTERNATE_DELAY_START) == UI_ALTERNATE_DELAY_START)
                {
                    if (APP_NORMAL_MODE_UI_DELAY_START_TIME_END > app_userInterface_data_get()->normalMode.common.delayStartHourSelection)
                    {
                        app_userInterface_data_set()->normalMode.common.delayStartHourSelection += APP_NORMAL_MODE_UI_DELAY_START_TIME_1_HOUR;
                    }
                    else
                    {
                        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                        app_userInterface_data_set()->normalMode.common.delayStartHourSelection = APP_NORMAL_MODE_UI_DELAY_START_TIME_0_HOUR;
                    }
                }
            }
        }
        else
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
    }
    else if (HANDLER_KEY_RELEASED == app_userInterface_keyState_get()->delayStartKeyState)
    {
        oneTime_flag = 0;
        doublePressTimeoutMSec += 100;
    }
    else
    {
        // do nothing
    }
}
static void standby_tubclean_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->tubCleanKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);

        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF)
        {
            app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_ON;
            app_userInterface_data_set()->normalMode.common.courseSelection = APP_NORMAL_MODE_UI_COURSE_NONE;
        }
        else
        {
            app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_OFF;
            if ((app_memory_preferedCourse_get() > APP_NORMAL_MODE_UI_COURSE_NONE) && (app_memory_preferedCourse_get() < APP_NORMAL_MODE_UI_COURSE_NUM))
            {
                app_userInterface_data_set()->normalMode.common.courseSelection = (app_nm_userInterface_courseSelection_t)app_memory_preferedCourse_get();
            }
            else
            {
                app_userInterface_data_set()->normalMode.common.courseSelection = APP_NORMAL_MODE_UI_DEFAULT_COURSE;
            }
        }

        wash_userInterface_refresh();
#ifdef DRYER_ENABLED
        dry_userInterface_refresh();
#endif
    }
}

static void standby_waterTemp_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->waterTempKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF)
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if ((APP_NORMAL_MODE_UI_WASH_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.washSelection) || ((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_NONE) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWaterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_NONE)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.waterTempSelection < app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterTemp)
            {
                app_userInterface_data_set()->normalMode.wash.waterTempSelection++;
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);
            }
            else if (app_userInterface_data_get()->normalMode.wash.waterTempSelection >= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterTemp)
            {
                app_userInterface_data_set()->normalMode.wash.waterTempSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWaterTemp;
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);
            }
            else
            {
                // do nothing
            }
        }
        else
        {
            if (((app_nm_tubclean_userInterface_highLimitDefaults_data_get()->defaultWaterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_NONE) && (app_nm_tubclean_userInterface_lowLimitDefaults_data_get()->defaultWaterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_NONE)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.waterTempSelection < app_nm_tubclean_userInterface_highLimitDefaults_data_get()->defaultWaterTemp)
            {
                app_userInterface_data_set()->normalMode.wash.waterTempSelection++;
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.waterTempSelection >= app_nm_tubclean_userInterface_highLimitDefaults_data_get()->defaultWaterTemp)
            {
                app_userInterface_data_set()->normalMode.wash.waterTempSelection = app_nm_tubclean_userInterface_lowLimitDefaults_data_get()->defaultWaterTemp;
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            }
            else
            {
                // do nothing
            }
        }
    }
}
static void standby_waterLevel_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->waterLevelKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_ON)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if (((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterLevel == APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWaterLevel == APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0)) || ((APP_NORMAL_MODE_UI_WASH_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.washSelection) && ((APP_NORMAL_MODE_UI_RINSE_0_TIMES == app_userInterface_data_get()->normalMode.wash.rinseSelection))))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.waterLevelSelection < app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterLevel)
            {
                if (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0 == app_userInterface_data_set()->normalMode.wash.waterLevelSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1;
                }
                else if (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 == app_userInterface_data_set()->normalMode.wash.waterLevelSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_3;
                }
                else if (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_3 == app_userInterface_data_set()->normalMode.wash.waterLevelSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_5;
                }
                else if (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_5 == app_userInterface_data_set()->normalMode.wash.waterLevelSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_7;
                }
                else if (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_7 == app_userInterface_data_set()->normalMode.wash.waterLevelSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_8;
                }
                else if (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_8 == app_userInterface_data_set()->normalMode.wash.waterLevelSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.waterLevelSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWaterLevel;
                }
                else
                {
                    // do nothing
                }
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);

                if (APP_NORMAL_MODE_UI_WASH_TIME_0_MIN != app_userInterface_data_get()->normalMode.wash.washSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.washSelection = app_nm_wash_userInterface_washTimePerWaterLevel_get();
                }
            }
            else if (app_userInterface_data_get()->normalMode.wash.waterLevelSelection >= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterLevel)
            {
                app_userInterface_data_set()->normalMode.wash.waterLevelSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWaterLevel;
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);

                if (APP_NORMAL_MODE_UI_WASH_TIME_0_MIN != app_userInterface_data_get()->normalMode.wash.washSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.washSelection = app_nm_wash_userInterface_washTimePerWaterLevel_get();
                }
                else if (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0 == app_userInterface_data_get()->normalMode.wash.waterLevelSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.waterLevelSelection++;
                }
                else
                {
                    // do nothing
                }
            }
            else
            {
                // do nothing
            }
        }
    }
}
static void standby_soak_update(void)
{
#ifndef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->soakKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_ON)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if (((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSoakTime == APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSoakTime == APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.soakSelection < app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSoakTime)
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);
                if (app_userInterface_state_set(UI_ALTERNATE_SOAK) == UI_ALTERNATE_SOAK)
                {
                    if (APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR == app_userInterface_data_get()->normalMode.wash.soakSelection)
                    {
                        app_userInterface_data_set()->normalMode.wash.soakSelection = APP_NORMAL_MODE_UI_SOAK_TIME_1_HOUR;
                    }
                    else if (APP_NORMAL_MODE_UI_SOAK_TIME_1_HOUR == app_userInterface_data_get()->normalMode.wash.soakSelection)
                    {
                        app_userInterface_data_set()->normalMode.wash.soakSelection = APP_NORMAL_MODE_UI_SOAK_TIME_2_HOUR;
                    }
                    else if (APP_NORMAL_MODE_UI_SOAK_TIME_2_HOUR == app_userInterface_data_get()->normalMode.wash.soakSelection)
                    {
                        app_userInterface_data_set()->normalMode.wash.soakSelection = APP_NORMAL_MODE_UI_SOAK_TIME_4_HOUR;
                    }
                    else if (APP_NORMAL_MODE_UI_SOAK_TIME_4_HOUR == app_userInterface_data_get()->normalMode.wash.soakSelection)
                    {
                        app_userInterface_data_set()->normalMode.wash.soakSelection = APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR;
                    }
                    else
                    {
                        // do nothing
                    }
                }
            }
            else if (app_userInterface_data_get()->normalMode.wash.soakSelection >= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSoakTime)
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);

                if (app_userInterface_state_set(UI_ALTERNATE_SOAK) == UI_ALTERNATE_SOAK)
                {
                    app_userInterface_data_set()->normalMode.wash.soakSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSoakTime;
                }
            }
            else
            {
                // do nothing
            }
        }
    }
#endif
}
static void standby_wash_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->washKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_ON)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if ((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWashTime == APP_NORMAL_MODE_UI_WASH_TIME_0_MIN) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWashTime == APP_NORMAL_MODE_UI_WASH_TIME_0_MIN))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.washSelection < app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWashTime)
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_WASH);

                if (app_userInterface_state_set(UI_ALTERNATE_WASH) == UI_ALTERNATE_WASH)
                {
                    if (APP_NORMAL_MODE_UI_WASH_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.washSelection)
                    {
                        app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_3_MIN;
                        app_userInterface_data_set()->normalMode.wash.waterTempSelection = app_nm_wash_userInterface_defaults_data_get()->defaultWaterTemp;
                        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = app_nm_wash_userInterface_defaults_data_get()->defaultWaterLevel;
                    }
                    else if (APP_NORMAL_MODE_UI_WASH_TIME_3_MIN == app_userInterface_data_get()->normalMode.wash.washSelection)
                    {
                        app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_9_MIN;
                    }
                    else if (APP_NORMAL_MODE_UI_WASH_TIME_9_MIN == app_userInterface_data_get()->normalMode.wash.washSelection)
                    {
                        app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_12_MIN;
                    }
                    else if (APP_NORMAL_MODE_UI_WASH_TIME_12_MIN == app_userInterface_data_get()->normalMode.wash.washSelection)
                    {
                        app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_18_MIN;
                    }
                    else if (APP_NORMAL_MODE_UI_WASH_TIME_18_MIN == app_userInterface_data_get()->normalMode.wash.washSelection)
                    {
                        app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_0_MIN;
                    }
                    else
                    {
                        // do nothing
                    }
                }
            }
            else if (app_userInterface_data_get()->normalMode.wash.washSelection >= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWashTime)
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);

                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_WASH);
                if (app_userInterface_state_set(UI_ALTERNATE_WASH) == UI_ALTERNATE_WASH)
                {
                    app_userInterface_data_set()->normalMode.wash.washSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWashTime;
                }
            }
            else
            {
                // do nothing
            }

            if (APP_NORMAL_MODE_UI_WASH_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.washSelection)
            {
                app_userInterface_data_set()->normalMode.wash.soakSelection = APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR;
                app_userInterface_data_set()->normalMode.wash.soilLevelSelection = APP_NORMAL_MODE_UI_SOIL_LEVEL_NONE;
                app_userInterface_data_set()->normalMode.wash.steamSelection = APP_NORMAL_MODE_UI_STEAM_OFF;
            }
        }
    }
}
static void standby_steam_update(void)
{
#ifdef STEAM_ENABLED
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->steamKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_ON)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if (((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSteam == APP_NORMAL_MODE_UI_STEAM_OFF) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSteam == APP_NORMAL_MODE_UI_STEAM_OFF)) || (app_userInterface_data_get()->normalMode.wash.washSelection == APP_NORMAL_MODE_UI_WASH_TIME_0_MIN))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.steamSelection < app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSteam)
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                if (APP_NORMAL_MODE_UI_STEAM_SELECTION_NUM > app_userInterface_data_get()->normalMode.wash.steamSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.steamSelection++;
                }
                else
                {
                    app_userInterface_data_set()->normalMode.wash.steamSelection = APP_NORMAL_MODE_UI_STEAM_OFF;
                }
            }
            else if (app_userInterface_data_get()->normalMode.wash.steamSelection >= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSteam)
            {
                app_userInterface_data_set()->normalMode.wash.steamSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSteam;
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            }
            else
            {
                // do nothing
            }
        }
    }
#endif
}
static void standby_rinse_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->rinseKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_ON)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if (((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultRinseTime == APP_NORMAL_MODE_UI_RINSE_0_TIMES) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultRinseTime == APP_NORMAL_MODE_UI_RINSE_0_TIMES)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.rinseSelection <= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultRinseTime)
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_RINSE);
                if (app_userInterface_state_set(UI_ALTERNATE_RINSE) == UI_ALTERNATE_RINSE)
                {
                    if ((APP_NORMAL_MODE_UI_RINSE_SELECTION_NUM > app_userInterface_data_get()->normalMode.wash.rinseSelection) && (app_userInterface_data_set()->normalMode.wash.extraRinseSelection != APP_NORMAL_MODE_UI_RINSE_EXTENDED))
                    {
                        if (APP_NORMAL_MODE_UI_RINSE_3_TIMES == app_userInterface_data_get()->normalMode.wash.rinseSelection)
                        {
                            app_userInterface_data_set()->normalMode.wash.rinseSelection = APP_NORMAL_MODE_UI_RINSE_3_TIMES;
                            app_userInterface_data_set()->normalMode.wash.extraRinseSelection = APP_NORMAL_MODE_UI_RINSE_EXTENDED;
                        }
                        else
                        {
                            app_userInterface_data_set()->normalMode.wash.rinseSelection++;
                        }
                    }
                    else
                    {
                        app_userInterface_data_set()->normalMode.wash.rinseSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultRinseTime;
                        app_userInterface_data_set()->normalMode.wash.extraRinseSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultExtraRinse;
                    }
                }
            }
            else
            {
                // do nothing
            }
        }
    }
}
static void standby_extraRinse_update(void)
{
    // NOTE: doesn't exist in the current model
}
static void standby_spin_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->spinKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_ON)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if (((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSpinTime == APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSpinTime == APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.spinSelection <= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSpinTime)
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_SPIN);
                if (app_userInterface_state_set(UI_ALTERNATE_SPIN) == UI_ALTERNATE_SPIN)
                {
                    if (APP_NORMAL_MODE_UI_SPIN_SELECTION_NUM > app_userInterface_data_get()->normalMode.wash.spinSelection)
                    {
                        if (APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.spinSelection)
                        {
                            app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_1_MIN;
                        }
                        else if (APP_NORMAL_MODE_UI_SPIN_TIME_1_MIN == app_userInterface_data_get()->normalMode.wash.spinSelection)
                        {
                            app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_5_MIN;
                        }
                        else if (APP_NORMAL_MODE_UI_SPIN_TIME_5_MIN == app_userInterface_data_get()->normalMode.wash.spinSelection)
                        {
                            app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_9_MIN;
                        }
#ifdef AUTO_DETERGENT_HEATER_MODEL
                        else if (APP_NORMAL_MODE_UI_SPIN_TIME_9_MIN == app_userInterface_data_get()->normalMode.wash.spinSelection)
                        {
                            app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_20_MIN;
                        }
                        else if (APP_NORMAL_MODE_UI_SPIN_TIME_20_MIN == app_userInterface_data_get()->normalMode.wash.spinSelection)
                        {
                            app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN;
                        }
#else
                        else if (APP_NORMAL_MODE_UI_SPIN_TIME_9_MIN == app_userInterface_data_get()->normalMode.wash.spinSelection)
                        {
                            app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN;
                        }
#endif
                        else
                        {
                            // do nothing
                        }
                    }
                    else
                    {
                        app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                        if (app_userInterface_state_set(UI_ALTERNATE_SPIN) == UI_ALTERNATE_SPIN)
                        {
                            app_userInterface_data_set()->normalMode.wash.spinSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSpinTime;
                        }
                    }
                }
            }
            else
            {
                // do nothing
            }

            if (APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.spinSelection)
            {
                app_userInterface_data_set()->normalMode.wash.antiWrinkleSelection = APP_NORMAL_MODE_UI_ANTI_WRINKLE_TIME_0_MIN;
            }
            else
            {
                app_userInterface_data_set()->normalMode.wash.antiWrinkleSelection = app_nm_wash_userInterface_defaults_data_get()->defaultAntiWrinkle;
            }
        }
    }
}
static void standby_superSpin_update(void)
{
#ifndef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->extraSpinKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_ON)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if (((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSuperSpinTime == APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSuperSpinTime == APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.superSpinSelection <= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSuperSpinTime)
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);
                if (app_userInterface_state_set(UI_ALTERNATE_EXTRA_SPIN) == UI_ALTERNATE_EXTRA_SPIN)
                {
                    if (APP_NORMAL_MODE_UI_SUPER_SPIN_SELECTION_NUM > app_userInterface_data_get()->normalMode.wash.superSpinSelection)
                    {
                        if (APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.superSpinSelection)
                        {
                            app_userInterface_data_set()->normalMode.wash.superSpinSelection = APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_20_MIN;
                        }
                        else if (APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_20_MIN == app_userInterface_data_get()->normalMode.wash.superSpinSelection)
                        {
                            app_userInterface_data_set()->normalMode.wash.superSpinSelection = APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN;
                        }
                        else
                        {
                            // do nothing
                        }
                    }
                }
                else
                {
                    app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                    if (app_userInterface_state_set(UI_ALTERNATE_EXTRA_SPIN) == UI_ALTERNATE_EXTRA_SPIN)
                    {
                        app_userInterface_data_set()->normalMode.wash.superSpinSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSuperSpinTime;
                    }
                }
            }
            else
            {
                // do nothing
            }
        }
    }
#endif
}
static void standby_soilLevel_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->stainLevelKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_ON)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if (((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSoilLevel == APP_NORMAL_MODE_UI_SOIL_LEVEL_NONE) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSoilLevel == APP_NORMAL_MODE_UI_SOIL_LEVEL_NONE)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.soilLevelSelection < app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSoilLevel)
            {
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);
                if (APP_NORMAL_MODE_UI_SOIL_LEVEL_HEAVY >= app_userInterface_data_get()->normalMode.wash.soilLevelSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.soilLevelSelection++;
                }
                else
                {
                    app_userInterface_data_set()->normalMode.wash.soilLevelSelection = APP_NORMAL_MODE_UI_SOIL_LEVEL_NONE;
                }
            }
            else if (app_userInterface_data_get()->normalMode.wash.soilLevelSelection >= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSoilLevel)
            {
                app_userInterface_data_set()->normalMode.wash.soilLevelSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSoilLevel;
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);
            }
            else
            {
                // do nothing
            }
        }
    }
}
static void standby_antiWrinkle_update(void)
{
    // NOTE: doesn't have a switch in this model, it is set automatically with other options
}
static void standby_steamTech_update(void)
{
#ifdef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->steamTechKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF)
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if ((0U == app_userInterface_data_get()->normalMode.wash.washSelection) ||
                ((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSteamTech == APP_NORMAL_MODE_UI_STEAM_TECH_OFF) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSteamTech == APP_NORMAL_MODE_UI_STEAM_TECH_OFF)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.steamTechSelection < app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSteamTech)
            {
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                if (APP_NORMAL_MODE_UI_STEAM_TECH_ON >= app_userInterface_data_get()->normalMode.wash.steamTechSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.steamTechSelection++;
                }
                else
                {
                    app_userInterface_data_set()->normalMode.wash.steamTechSelection = APP_NORMAL_MODE_UI_STEAM_TECH_OFF;
                }
            }
            else if (app_userInterface_data_get()->normalMode.wash.steamTechSelection >= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSteamTech)
            {
                app_userInterface_data_set()->normalMode.wash.steamTechSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSteamTech;
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            }
            else
            {
                // do nothing
            }
        }
        else
        {
            if (((app_nm_tubclean_userInterface_highLimitDefaults_data_get()->defaultSteamTech == APP_NORMAL_MODE_UI_STEAM_TECH_OFF) && (app_nm_tubclean_userInterface_lowLimitDefaults_data_get()->defaultSteamTech == APP_NORMAL_MODE_UI_STEAM_TECH_OFF)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.steamTechSelection < app_nm_tubclean_userInterface_highLimitDefaults_data_get()->defaultSteamTech)
            {
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                if (APP_NORMAL_MODE_UI_STEAM_TECH_ON >= app_userInterface_data_get()->normalMode.wash.steamTechSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.steamTechSelection++;
                }
                else
                {
                    app_userInterface_data_set()->normalMode.wash.steamTechSelection = APP_NORMAL_MODE_UI_STEAM_TECH_OFF;
                }
            }
            else if (app_userInterface_data_get()->normalMode.wash.steamTechSelection >= app_nm_tubclean_userInterface_highLimitDefaults_data_get()->defaultSteamTech)
            {
                app_userInterface_data_set()->normalMode.wash.steamTechSelection = app_nm_tubclean_userInterface_lowLimitDefaults_data_get()->defaultSteamTech;
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            }
            else
            {
                // do nothing
            }
        }
    }
#endif
}
static void standby_gelDetergent_update(void)
{
#ifdef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->gelDetergentKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF)
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if ((app_fill_autoDetergentState_get() == APP_FILL_AUTO_DETERGENT_LOW) || (0U == app_userInterface_data_get()->normalMode.wash.washSelection) ||
                ((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultGelDetergent == APP_NORMAL_MODE_UI_GEL_DETERGENT_OFF) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultGelDetergent == APP_NORMAL_MODE_UI_GEL_DETERGENT_OFF)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.gelDetergentSelection < app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultGelDetergent)
            {
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                if (APP_NORMAL_MODE_UI_GEL_DETERGENT_ON >= app_userInterface_data_get()->normalMode.wash.gelDetergentSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.gelDetergentSelection++;
                }
                else
                {
                    app_userInterface_data_set()->normalMode.wash.gelDetergentSelection = APP_NORMAL_MODE_UI_GEL_DETERGENT_OFF;
                }
            }
            else if (app_userInterface_data_get()->normalMode.wash.gelDetergentSelection >= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultGelDetergent)
            {
                app_userInterface_data_set()->normalMode.wash.gelDetergentSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultGelDetergent;
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            }
            else
            {
                // do nothing
            }
        }
        else
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
    }
#endif
}
static void standby_heating_update(void)
{
    // do nothing
}

#ifdef DRYER_ENABLED
static void standby_dryTime_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->dryTimeKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_ON)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if (((app_nm_dry_userInterface_highLimit_data_get()->defaultDryTime == APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN) && (app_nm_dry_userInterface_lowLimit_data_get()->defaultDryTime == APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.dry.dryTimeSelection < app_nm_dry_userInterface_highLimit_data_get()->defaultDryTime)
            {
                app_userInterface_data_set()->normalMode.dry.dryTimeSelection++;
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);
            }
            else if (app_userInterface_data_get()->normalMode.dry.dryTimeSelection >= app_nm_dry_userInterface_highLimit_data_get()->defaultDryTime)
            {
                app_userInterface_data_set()->normalMode.dry.dryTimeSelection = app_nm_dry_userInterface_lowLimit_data_get()->defaultDryTime;
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);
            }
            else
            {
                // do nothing
            }
        }
    }
}
#endif

#ifdef DRYER_ENABLED
static void standby_drynessIntensity_update(void)
{
    if (HANDLER_KEY_LONG_PRESSED == app_userInterface_keyEdgeState_get()->dryTimeKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_ON)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if (((app_nm_dry_userInterface_highLimit_data_get()->defaultDryIntensity == APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE) && (app_nm_dry_userInterface_lowLimit_data_get()->defaultDryIntensity == APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.dry.dryIntensitySelection < app_nm_dry_userInterface_highLimit_data_get()->defaultDryIntensity)
            {
                app_userInterface_data_set()->normalMode.dry.dryIntensitySelection++;
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);
            }
            else if (app_userInterface_data_get()->normalMode.dry.dryIntensitySelection >= app_nm_dry_userInterface_highLimit_data_get()->defaultDryIntensity)
            {
                app_userInterface_data_set()->normalMode.dry.dryIntensitySelection = app_nm_dry_userInterface_lowLimit_data_get()->defaultDryIntensity;
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                standby_userInterface_firstPress_handle(STANDBY_FIRST_PRESS_OTHERS);
            }
            else
            {
                // do nothing
            }
        }
    }
}
#endif

static void wash_userInterface_refresh(void)
{
    if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF)
    {
        app_userInterface_data_set()->normalMode.wash.waterTempSelection = app_nm_wash_userInterface_defaults_data_get()->defaultWaterTemp;
        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = app_nm_wash_userInterface_defaults_data_get()->defaultWaterLevel;
        app_userInterface_data_set()->normalMode.wash.soakSelection = app_nm_wash_userInterface_defaults_data_get()->defaultSoakTime;
        app_userInterface_data_set()->normalMode.wash.washSelection = app_nm_wash_userInterface_defaults_data_get()->defaultWashTime;
        app_userInterface_data_set()->normalMode.wash.rinseSelection = app_nm_wash_userInterface_defaults_data_get()->defaultRinseTime;
        app_userInterface_data_set()->normalMode.wash.extraRinseSelection = app_nm_wash_userInterface_defaults_data_get()->defaultExtraRinse;
        app_userInterface_data_set()->normalMode.wash.spinSelection = app_nm_wash_userInterface_defaults_data_get()->defaultSpinTime;
        app_userInterface_data_set()->normalMode.wash.soilLevelSelection = app_nm_wash_userInterface_defaults_data_get()->defaultSoilLevel;
        app_userInterface_data_set()->normalMode.wash.superSpinSelection = app_nm_wash_userInterface_defaults_data_get()->defaultSuperSpinTime;
        app_userInterface_data_set()->normalMode.wash.antiWrinkleSelection = app_nm_wash_userInterface_defaults_data_get()->defaultAntiWrinkle;
        app_userInterface_data_set()->normalMode.wash.steamTechSelection = app_nm_wash_userInterface_defaults_data_get()->defaultSteamTech;
        app_userInterface_data_set()->normalMode.wash.waterHeatingSelection = app_nm_wash_userInterface_defaults_data_get()->defaultWaterHeating;
        app_userInterface_data_set()->normalMode.wash.gelDetergentSelection = app_nm_wash_userInterface_defaults_data_get()->defaultGelDetergent;
    }
    else
    {
        app_userInterface_data_set()->normalMode.wash.waterTempSelection = app_nm_tubclean_userInterface_defaults_data_get()->defaultWaterTemp;
        app_userInterface_data_set()->normalMode.wash.steamTechSelection = app_nm_tubclean_userInterface_defaults_data_get()->defaultSteamTech;

        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0;
        app_userInterface_data_set()->normalMode.wash.soakSelection = APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR;
        app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_0_MIN;
        app_userInterface_data_set()->normalMode.wash.rinseSelection = APP_NORMAL_MODE_UI_RINSE_0_TIMES;
        app_userInterface_data_set()->normalMode.wash.extraRinseSelection = APP_NORMAL_MODE_UI_RINSE_OVERFLOW_OFF_SHOWER_OFF;
        app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN;
        app_userInterface_data_set()->normalMode.wash.soilLevelSelection = APP_NORMAL_MODE_UI_SOIL_LEVEL_NONE;
        app_userInterface_data_set()->normalMode.wash.superSpinSelection = APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN;
        app_userInterface_data_set()->normalMode.wash.antiWrinkleSelection = APP_NORMAL_MODE_UI_ANTI_WRINKLE_TIME_0_MIN;
        app_userInterface_data_set()->normalMode.wash.waterHeatingSelection = APP_NORMAL_MODE_UI_WATER_HEATING_OFF;
        app_userInterface_data_set()->normalMode.wash.gelDetergentSelection = APP_NORMAL_MODE_UI_GEL_DETERGENT_OFF;
    }
}

#ifdef DRYER_ENABLED
static void dry_userInterface_refresh(void)
{
    if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF)
    {
        app_userInterface_data_set()->normalMode.dry.dryTimeSelection = app_nm_dry_userInterface_defaults_data_get()->defaultDryTime;
        app_userInterface_data_set()->normalMode.dry.dryIntensitySelection = app_nm_dry_userInterface_defaults_data_get()->defaultDryIntensity;
        app_userInterface_data_set()->normalMode.dry.dryWaterLevel = app_nm_dry_userInterface_defaults_data_get()->defaultDryWaterLevel;
    }
    else
    {
        app_userInterface_data_set()->normalMode.dry.dryTimeSelection = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN;
        app_userInterface_data_set()->normalMode.dry.dryIntensitySelection = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE;
        app_userInterface_data_set()->normalMode.dry.dryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0;
    }
}
#endif

static void standby_userInterface_firstPress_handle(standby_first_press_t selection)
{
#ifdef STANDBY_FIRST_PRESS_ENABLE
    static uint8_t gu8_standby_firstPress_flag = 0;

    if (0U == gu8_standby_firstPress_flag)
    {
        gu8_standby_firstPress_flag = 1;

        if (STANDBY_FIRST_PRESS_WASH == selection)
        {
            app_userInterface_data_set()->normalMode.wash.rinseSelection = APP_NORMAL_MODE_UI_RINSE_0_TIMES;
            app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN;
            app_userInterface_data_set()->normalMode.wash.antiWrinkleSelection = APP_NORMAL_MODE_UI_ANTI_WRINKLE_TIME_0_MIN;
        }
        else if (STANDBY_FIRST_PRESS_RINSE == selection)
        {
            app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_0_MIN;
            app_userInterface_data_set()->normalMode.wash.steamSelection = APP_NORMAL_MODE_UI_STEAM_OFF;
            app_userInterface_data_set()->normalMode.wash.soakSelection = APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR;
            app_userInterface_data_set()->normalMode.wash.soilLevelSelection = APP_NORMAL_MODE_UI_SOIL_LEVEL_NONE;
            app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN;
            app_userInterface_data_set()->normalMode.wash.antiWrinkleSelection = APP_NORMAL_MODE_UI_ANTI_WRINKLE_TIME_0_MIN;
        }
        else if (STANDBY_FIRST_PRESS_SPIN == selection)
        {
            app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_0_MIN;
            app_userInterface_data_set()->normalMode.wash.steamSelection = APP_NORMAL_MODE_UI_STEAM_OFF;
            app_userInterface_data_set()->normalMode.wash.soakSelection = APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR;
            app_userInterface_data_set()->normalMode.wash.soilLevelSelection = APP_NORMAL_MODE_UI_SOIL_LEVEL_NONE;
            app_userInterface_data_set()->normalMode.wash.rinseSelection = APP_NORMAL_MODE_UI_RINSE_0_TIMES;
        }
        else
        {
            // do nothing
        }
    }
#endif
}

static userInterface_mode_t userInterface_modeType_detect(uint32_t period)
{
    userInterface_mode_t ret = UI_NORMAL_MODE;

    if (0U != app_tm_entry_check(period))
    {
        ret = UI_TEST_MODE;
    }

    return ret;
}
