#include "def.h"
#include "os.h"

#include "app_nm.h"

#include "app_nm_wash.h"
#include "app_nm_user_interface.h"
#include "app_nm_pause_user_interface.h"
#include "app_nm_wash_user_interface.h"

#include "app_nm_tubclean.h"
#include "app_nm_tubclean_user_interface.h"

#include "app_fill.h"
#include "app_nm.h"
#include "app_buzzer.h"

static void pause_userInterface_normalMode_handle(void);
static void wash_pause_userinterface_normalMode_handle(void);
#ifdef DRYER_ENABLED
static void dry_pause_userinterface_normalMode_handle(void);
#endif

static void pause_normalMode_operation_update(void);
static void pause_normalMode_course_update(void);
static void pause_normalMode_delayStart_update(void);
static void pause_normalMode_childLock_update(void);
static void pause_normalMode_tubclean_update(void);

static void pause_normalMode_waterTemp_update(void);
static void pause_normalMode_waterLevel_update(void);
static void pause_normalMode_soak_update(void);
static void pause_normalMode_wash_update(void);
static void pause_normalMode_steam_update(void);
static void pause_normalMode_rinse_update(void);
static void pause_normalMode_extraRinse_update(void);
static void pause_normalMode_spin_update(void);
static void pause_normalMode_superSpin_update(void);
static void pause_normalMode_soilLevel_update(void);
static void pause_normalMode_antiWrinkle_update(void);
static void pause_normalMode_steamTech_update(void);
static void pause_normalMode_gelDetergent_update(void);

#ifdef DRYER_ENABLED
static void pause_normalMode_dryTime_update(void);
static void pause_normalMode_drynessIntensity_update(void);
#endif

#ifdef COURSE_CHANGE_DURING_CYCLE_ENABLE
static void wash_userInterface_refresh(void);
#endif
#ifdef DRYER_ENABLED
static void dry_userInterface_refresh(void);
#endif

void app_nm_pause_userInterface_process(uint32_t period)
{
    pause_userInterface_normalMode_handle();
}

static void pause_userInterface_normalMode_handle(void)
{
    pause_normalMode_operation_update();
    pause_normalMode_course_update();
    pause_normalMode_delayStart_update();
    pause_normalMode_childLock_update();
    pause_normalMode_tubclean_update();

    wash_pause_userinterface_normalMode_handle();
#ifdef DRYER_ENABLED
    dry_pause_userinterface_normalMode_handle();
#endif
}

static void wash_pause_userinterface_normalMode_handle(void)
{
    pause_normalMode_waterTemp_update();
    pause_normalMode_waterLevel_update();
    pause_normalMode_soak_update();
    pause_normalMode_wash_update();
    pause_normalMode_steam_update();
    pause_normalMode_rinse_update();
    pause_normalMode_extraRinse_update();
    pause_normalMode_spin_update();
    pause_normalMode_superSpin_update();
    pause_normalMode_soilLevel_update();
    pause_normalMode_antiWrinkle_update();
    pause_normalMode_steamTech_update();
    pause_normalMode_gelDetergent_update();
}

#ifdef DRYER_ENABLED
static void dry_pause_userinterface_normalMode_handle(void)
{
    pause_normalMode_dryTime_update();
    pause_normalMode_drynessIntensity_update();
}
#endif

static void pause_normalMode_operation_update(void)
{
#ifdef DRYER_ENABLED
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->dryOnlyKeyEdgeState)
    {
        if ((APP_NORMAL_MODE_INIT == app_nm_prevState_get()) || (APP_NORMAL_MODE_WASH == app_nm_prevState_get()))
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);

            app_userInterface_data_set()->normalMode.common.operationSelection = APP_NORMAL_MODE_UI_OPERATION_DRY_ONLY;

            // TODO: handle better
            app_nm_reset();

            wash_userInterface_refresh();
            dry_userInterface_refresh();
        }
        else
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
    }
    else if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->washOnlyKeyEdgeState)
    {
        if ((APP_NORMAL_MODE_INIT == app_nm_prevState_get()) || (APP_NORMAL_MODE_WASH == app_nm_prevState_get()))
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);

            app_userInterface_data_set()->normalMode.common.operationSelection = APP_NORMAL_MODE_UI_OPERATION_WASH_ONLY;

            // TODO: handle better
            app_nm_reset();

            wash_userInterface_refresh();
            dry_userInterface_refresh();
        }
        else
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
    }
    else if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->washnDryKeyEdgeState)
    {
        if ((APP_NORMAL_MODE_INIT == app_nm_prevState_get()) || (APP_NORMAL_MODE_WASH == app_nm_prevState_get()))
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);

            app_userInterface_data_set()->normalMode.common.operationSelection = APP_NORMAL_MODE_UI_OPERATION_WASH_DRY;

            // TODO: handle better
            app_nm_reset();

            wash_userInterface_refresh();
            dry_userInterface_refresh();
        }
        else
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
    }
    else
    {
        // do nothing
    }
#endif
}
static void pause_normalMode_course_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_courseKeyEdgeState_get())
    {
#ifdef COURSE_CHANGE_DURING_CYCLE_ENABLE
        if ((APP_NORMAL_MODE_INIT == app_nm_prevState_get()) || (APP_NORMAL_MODE_WASH == app_nm_prevState_get()))
        {
            (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
            app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);

            if (APP_NORMAL_MODE_UI_COURSE_NUM - 1 > app_userInterface_data_get()->normalMode.common.courseSelection)
            {
                app_userInterface_data_set()->normalMode.common.courseSelection = app_nm_userInterface_nextCourseInOperation_get(app_userInterface_data_get()->normalMode.common.courseSelection);
            }
            else
            {
                app_userInterface_data_set()->normalMode.common.courseSelection = APP_NORMAL_MODE_UI_DEFAULT_COURSE;
            }

            // TODO: handle better
            app_nm_reset();

            wash_userInterface_refresh();
#ifdef DRYER_ENABLED
            dry_userInterface_refresh();
#endif
        }
        else
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
#else
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
#endif
    }
}
static void pause_normalMode_delayStart_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->delayStartKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void pause_normalMode_childLock_update(void)
{
    // do nothing
}
static void pause_normalMode_tubclean_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->tubCleanKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}

static void pause_normalMode_waterTemp_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->waterTempKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF)
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if ((APP_NORMAL_MODE_WASH_WASH < app_nm_wash_state_get()) ||
                ((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_NONE) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWaterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_NONE)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.waterTempSelection < app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterTemp)
            {
                app_userInterface_data_set()->normalMode.wash.waterTempSelection++;
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.waterTempSelection >= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterTemp)
            {
                app_userInterface_data_set()->normalMode.wash.waterTempSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWaterTemp;
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
            // NOTE: check first if it is disabled in this course by checking the limits
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
static void pause_normalMode_waterLevel_update(void)
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
            if ((APP_NORMAL_MODE_WASH_RINSE < app_nm_wash_state_get()) ||
                ((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterLevel == APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWaterLevel == APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.waterLevelSelection < app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterLevel)
            {
                if (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0 == app_userInterface_data_set()->normalMode.wash.waterLevelSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1;
                }
                else if ((APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 <= app_userInterface_data_set()->normalMode.wash.waterLevelSelection) && (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_3 > app_userInterface_data_set()->normalMode.wash.waterLevelSelection))
                {
                    app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_3;
                }
                else if ((APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_3 <= app_userInterface_data_set()->normalMode.wash.waterLevelSelection) && (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_5 > app_userInterface_data_set()->normalMode.wash.waterLevelSelection))
                {
                    app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_5;
                }
                else if ((APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_5 <= app_userInterface_data_set()->normalMode.wash.waterLevelSelection) && (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_7 > app_userInterface_data_set()->normalMode.wash.waterLevelSelection))
                {
                    app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_7;
                }
                else if ((APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_7 <= app_userInterface_data_set()->normalMode.wash.waterLevelSelection) && (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_8 > app_userInterface_data_set()->normalMode.wash.waterLevelSelection))
                {
                    app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_8;
                }
                else if (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_8 <= app_userInterface_data_set()->normalMode.wash.waterLevelSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.waterLevelSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWaterLevel;
                }
                else
                {
                    // do nothing
                }
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.waterLevelSelection >= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterLevel)
            {
                app_userInterface_data_set()->normalMode.wash.waterLevelSelection = app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWaterLevel;
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
static void pause_normalMode_soak_update(void)
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
            if ((0U == app_userInterface_data_get()->normalMode.wash.washSelection) || (APP_NORMAL_MODE_WASH_WASH < app_nm_wash_state_get()) ||
                ((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSoakTime == APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSoakTime == APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.soakSelection < app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSoakTime)
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
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
static void pause_normalMode_wash_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->washKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_ON)
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else
        {
            // NOTE: check first if the running cycle has wash enabled from standby & there is wash option for the selected course
            if ((APP_NORMAL_MODE_WASH_WASH < app_nm_wash_state_get()) ||
                ((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWashTime == APP_NORMAL_MODE_UI_WASH_TIME_0_MIN) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWashTime == APP_NORMAL_MODE_UI_WASH_TIME_0_MIN)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.washSelection < app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWashTime)
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                if (app_userInterface_state_set(UI_ALTERNATE_WASH) == UI_ALTERNATE_WASH)
                {
                    if (APP_NORMAL_MODE_UI_WASH_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.washSelection)
                    {
                        app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_3_MIN;
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
                app_userInterface_data_set()->normalMode.wash.steamSelection = APP_NORMAL_MODE_UI_STEAM_OFF;
            }
        }
    }
}
static void pause_normalMode_steam_update(void)
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
            if ((APP_NORMAL_MODE_WASH_STEAM < app_nm_wash_state_get()) ||
                ((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSteam == APP_NORMAL_MODE_UI_STEAM_OFF) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSteam == APP_NORMAL_MODE_UI_STEAM_OFF)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.steamSelection < app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSteam)
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
                if (APP_NORMAL_MODE_UI_STEAM_OFF == app_userInterface_data_get()->normalMode.wash.steamSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.steamSelection = APP_NORMAL_MODE_UI_STEAM_ON;
                }
                else if (APP_NORMAL_MODE_UI_STEAM_ON == app_userInterface_data_get()->normalMode.wash.steamSelection)
                {
                    app_userInterface_data_set()->normalMode.wash.steamSelection = APP_NORMAL_MODE_UI_STEAM_OFF;
                }
                else
                {
                    // do nothing
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
static void pause_normalMode_rinse_update(void)
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
            if ((APP_NORMAL_MODE_WASH_RINSE < app_nm_wash_state_get()) ||
                ((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultRinseTime == APP_NORMAL_MODE_UI_RINSE_0_TIMES) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultRinseTime == APP_NORMAL_MODE_UI_RINSE_0_TIMES)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.rinseSelection <= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultRinseTime)
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
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
static void pause_normalMode_extraRinse_update(void)
{
    // NOTE: doesn't exist in the current model
}
static void pause_normalMode_spin_update(void)
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
            if ((APP_NORMAL_MODE_WASH_SPIN < app_nm_wash_state_get()) ||
                ((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSpinTime == APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSpinTime == APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.spinSelection <= app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSpinTime)
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
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
            else
            {
                // do nothing
            }

            if (APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.spinSelection)
            {
                app_userInterface_data_set()->normalMode.wash.antiWrinkleSelection = APP_NORMAL_MODE_UI_ANTI_WRINKLE_TIME_0_MIN;
            }
        }
    }
}
static void pause_normalMode_superSpin_update(void)
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
static void pause_normalMode_soilLevel_update(void)
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
            if ((0U == app_userInterface_data_get()->normalMode.wash.washSelection) || (APP_NORMAL_MODE_WASH_WASH < app_nm_wash_state_get()) ||
                ((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSoilLevel == APP_NORMAL_MODE_UI_SOIL_LEVEL_NONE) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultSoilLevel == APP_NORMAL_MODE_UI_SOIL_LEVEL_NONE)))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.wash.soilLevelSelection < app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultSoilLevel)
            {
                (void)app_userInterface_state_set(UI_ALTERNATE_NONE);
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
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
            }
            else
            {
                // do nothing
            }
        }
    }
}
static void pause_normalMode_antiWrinkle_update(void)
{
    // NOTE: can't change in this model
}
static void pause_normalMode_steamTech_update(void)
{
#ifdef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->steamTechKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF)
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if ((0U == app_userInterface_data_get()->normalMode.wash.washSelection) || (APP_NORMAL_MODE_WASH_WASH < app_nm_wash_state_get()) ||
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
            // NOTE: check first if it is disabled in this course by checking the limits
            if ((APP_NORMAL_MODE_TUBCLEAN_MD < app_nm_tubclean_state_get()) ||
                ((app_nm_tubclean_userInterface_highLimitDefaults_data_get()->defaultSteamTech == APP_NORMAL_MODE_UI_STEAM_TECH_OFF) && (app_nm_tubclean_userInterface_lowLimitDefaults_data_get()->defaultSteamTech == APP_NORMAL_MODE_UI_STEAM_TECH_OFF)))
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
static void pause_normalMode_gelDetergent_update(void)
{
#ifdef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->gelDetergentKeyEdgeState)
    {
        if (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF)
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if ((0U == app_userInterface_data_get()->normalMode.wash.washSelection) || (APP_NORMAL_MODE_WASH_SETUP < app_nm_wash_state_get()) ||
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

#ifdef DRYER_ENABLED
static void pause_normalMode_dryTime_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->dryTimeKeyEdgeState)
    {
        if ((APP_NORMAL_MODE_INIT == app_nm_prevState_get()) || (APP_NORMAL_MODE_WASH == app_nm_prevState_get()) || (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF))
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if ((app_nm_dry_userInterface_highLimit_data_get()->defaultDryTime == APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN) && (app_nm_dry_userInterface_lowLimit_data_get()->defaultDryTime == APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.dry.dryTimeSelection < app_nm_dry_userInterface_highLimit_data_get()->defaultDryTime)
            {
                app_userInterface_data_set()->normalMode.dry.dryTimeSelection++;
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.dry.dryTimeSelection >= app_nm_dry_userInterface_highLimit_data_get()->defaultDryTime)
            {
                app_userInterface_data_set()->normalMode.dry.dryTimeSelection = app_nm_dry_userInterface_lowLimit_data_get()->defaultDryTime;
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
}
#endif

#ifdef DRYER_ENABLED
static void pause_normalMode_drynessIntensity_update(void)
{
    if (HANDLER_KEY_LONG_PRESSED == app_userInterface_keyEdgeState_get()->dryTimeKeyEdgeState)
    {
        if ((APP_NORMAL_MODE_INIT == app_nm_prevState_get()) || (APP_NORMAL_MODE_WASH == app_nm_prevState_get()) || (app_userInterface_data_get()->normalMode.common.tubcleanSelection == APP_NORMAL_MODE_UI_TUBCLEAN_OFF))
        {
            // NOTE: check first if it is disabled in this course by checking the limits
            if ((app_nm_dry_userInterface_highLimit_data_get()->defaultDryIntensity == APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE) && (app_nm_dry_userInterface_lowLimit_data_get()->defaultDryIntensity == APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE))
            {
                app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.dry.dryIntensitySelection < app_nm_dry_userInterface_highLimit_data_get()->defaultDryIntensity)
            {
                app_userInterface_data_set()->normalMode.dry.dryIntensitySelection++;
                app_buzzer_tone_set(APP_BUZZER_TONE_VALID_KEY);
            }
            else if (app_userInterface_data_get()->normalMode.dry.dryIntensitySelection >= app_nm_dry_userInterface_highLimit_data_get()->defaultDryIntensity)
            {
                app_userInterface_data_set()->normalMode.dry.dryIntensitySelection = app_nm_dry_userInterface_lowLimit_data_get()->defaultDryIntensity;
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
}
#endif

#ifdef COURSE_CHANGE_DURING_CYCLE_ENABLE
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
#endif

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
