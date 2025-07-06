#include "def.h"
#include "os.h"
#include "app_nm_wash.h"
#include "app_nm_user_interface.h"
#include "app_nm_wash_user_interface.h"

#include "app_fill.h"
#include "app_nm.h"
#include "app_buzzer.h"

#include "app_nm_wash_defaults.h"
#include "app_nm_wash_motor.h"

#define WASH_WATER_LEVEL_LIMIT_AT_STARTING_LITTER (20U)
#define WASH_RINSE_WATER_TEMP_SETTING             (APP_NORMAL_MODE_UI_WATER_TEMP_COLD)

static void setup_init_handle(uint32_t period);
static void setup_weightDetection_handle(uint32_t period);

static void wash_userInterface_normalMode_handle(void);
static void wash_wash_userinterface_normalMode_handle(void);
static void dry_wash_userinterface_normalMode_handle(void);

static void wash_normalMode_operation_update(void);
static void wash_normalMode_course_update(void);
static void wash_normalMode_delayStart_update(void);
static void wash_normalMode_childLock_update(void);
static void wash_normalMode_tubclean_update(void);

static void wash_normalMode_waterTemp_update(void);
static void wash_normalMode_waterLevel_update(void);
static void wash_normalMode_soak_update(void);
static void wash_normalMode_wash_update(void);
static void wash_normalMode_rinse_update(void);
static void wash_normalMode_extraRinse_update(void);
static void wash_normalMode_spin_update(void);
static void wash_normalMode_superSpin_update(void);
static void wash_normalMode_soilLevel_update(void);
static void wash_normalMode_antiWrinkle_update(void);
static void wash_normalMode_steamTech_update(void);
static void wash_normalMode_gelDetergent_update(void);

static void wash_normalMode_dryTime_update(void);
static void wash_normalMode_drynessIntensity_update(void);

static const uint8_t *const ptrOverloadingData[] = {
    &course_white_overloadingErrorEnable,
    &course_light_overloadingErrorEnable,
    &course_dark_overloadingErrorEnable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    &course_eco_overloadingErrorEnable,
#else
    &course_wool_overloadingErrorEnable,
#endif
    &course_cotton_overloadingErrorEnable,
    &course_jeans_overloadingErrorEnable,
    &course_delicate_overloadingErrorEnable,
    &course_sports_overloadingErrorEnable,
    &course_mix_overloadingErrorEnable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    &course_alergy_overloadingErrorEnable,
#else
    &course_rapidWash_overloadingErrorEnable,
#endif
    &course_bedding_overloadingErrorEnable,
    &course_babycare_overloadingErrorEnable};

static const app_nm_wash_userInterface_defaults_data_t *const ptrDefaultData[] = {
    course_white_defaultsTable,
    course_light_defaultsTable,
    course_dark_defaultsTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    course_eco_defaultsTable,
#else
    course_wool_defaultsTable,
#endif
    course_cotton_defaultsTable,
    course_jeans_defaultsTable,
    course_delicate_defaultsTable,
    course_sports_defaultsTable,
    course_mix_defaultsTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    course_alergy_defaultsTable,
#else
    course_rapidWash_defaultsTable,
#endif
    course_bedding_defaultsTable,
    course_babycare_defaultsTable};

static const app_nm_wash_userInterface_defaults_data_t *const ptrLowLimitData[] = {
    course_white_defaulsLowLimitTable,
    course_light_defaulsLowLimitTable,
    course_dark_defaulsLowLimitTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    course_eco_defaulsLowLimitTable,
#else
    course_wool_defaulsLowLimitTable,
#endif
    course_cotton_defaulsLowLimitTable,
    course_jeans_defaulsLowLimitTable,
    course_delicate_defaulsLowLimitTable,
    course_sports_defaulsLowLimitTable,
    course_mix_defaulsLowLimitTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    course_alergy_defaulsLowLimitTable,
#else
    course_rapidWash_defaulsLowLimitTable,
#endif
    course_bedding_defaulsLowLimitTable,
    course_babycare_defaulsLowLimitTable};

static const app_nm_wash_userInterface_defaults_data_t *const ptrHighLimitData[] = {
    course_white_defaulsHighLimitTable,
    course_light_defaulsHighLimitTable,
    course_dark_defaulsHighLimitTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    course_eco_defaulsHighLimitTable,
#else
    course_wool_defaulsHighLimitTable,
#endif
    course_cotton_defaulsHighLimitTable,
    course_jeans_defaulsHighLimitTable,
    course_delicate_defaulsHighLimitTable,
    course_sports_defaulsHighLimitTable,
    course_mix_defaulsHighLimitTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    course_alergy_defaulsHighLimitTable,
#else
    course_rapidWash_defaulsHighLimitTable,
#endif
    course_bedding_defaulsHighLimitTable,
    course_babycare_defaulsHighLimitTable};

static const app_nm_wash_userInterface_washTimeSelection_t *const ptrDefaultWashPerLevel[] = {
    course_white_defaultWashTimePerWaterLevelTable,
    course_light_defaultWashTimePerWaterLevelTable,
    course_dark_defaultWashTimePerWaterLevelTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    course_eco_defaultWashTimePerWaterLevelTable,
#else
    course_wool_defaultWashTimePerWaterLevelTable,
#endif
    course_cotton_defaultWashTimePerWaterLevelTable,
    course_jeans_defaultWashTimePerWaterLevelTable,
    course_delicate_defaultWashTimePerWaterLevelTable,
    course_sports_defaultWashTimePerWaterLevelTable,
    course_mix_defaultWashTimePerWaterLevelTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    course_alergy_defaultWashTimePerWaterLevelTable,
#else
    course_rapidWash_defaultWashTimePerWaterLevelTable,
#endif
    course_bedding_defaultWashTimePerWaterLevelTable,
    course_babycare_defaultWashTimePerWaterLevelTable};

void app_nm_wash_userInterface_process(uint32_t period)
{
    uint32_t sum = 0;

    static void (*const setup_handlers[])(uint32_t period) = {
        setup_init_handle,
        NULL,
        setup_weightDetection_handle,
        NULL,
        NULL,
        NULL};

    app_userInterface_childLock_check(period);
    if (app_userInterface_data_get()->normalMode.common.childLockSelection == APP_NORMAL_MODE_UI_CHILD_LOCK_OFF)
    {
        wash_userInterface_normalMode_handle();
    }

    switch (app_nm_wash_state_get())
    {
        case APP_NORMAL_MODE_WASH_SETUP:
        {
            if (setup_handlers[app_nm_wash_setup_state_get()] != NULL)
            {
                setup_handlers[app_nm_wash_setup_state_get()](period);
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_SOAK:
        {
            if (APP_NORMAL_MODE_WASH_SOAK_WASH_TRANSITION == app_nm_wash_soak_state_get())
            {
                app_userInterface_data_set()->normalMode.wash.soakSelection = APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR;
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH:
        {
            if (APP_NORMAL_MODE_WASH_WASH_RINSE_TRANSITION == app_nm_wash_wash_state_get())
            {
                app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_0_MIN;
                app_userInterface_data_set()->normalMode.wash.soilLevelSelection = APP_NORMAL_MODE_UI_SOIL_LEVEL_NONE;
                app_userInterface_data_set()->normalMode.wash.steamTechSelection = APP_NORMAL_MODE_UI_STEAM_TECH_OFF;
                app_userInterface_data_set()->normalMode.wash.waterHeatingSelection = APP_NORMAL_MODE_UI_WATER_HEATING_OFF;
            }
            else
            {
#ifdef WASH_STIR_TIME_CALCULATION_BY_USER_SELECTION
                if ((app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_STIR] / 60000U) > APP_NORMAL_MODE_UI_WASH_TIME_12_MIN)
                {
                    app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_18_MIN;
                }
                else if ((app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_STIR] / 60000U) > APP_NORMAL_MODE_UI_WASH_TIME_9_MIN)
                {
                    app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_12_MIN;
                }
                else if ((app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_STIR] / 60000U) > APP_NORMAL_MODE_UI_WASH_TIME_3_MIN)
                {
                    app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_9_MIN;
                }
                else if (((app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_STIR] / 60000U) <= APP_NORMAL_MODE_UI_WASH_TIME_3_MIN) && ((app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_STIR] / 60000U) > APP_NORMAL_MODE_UI_WASH_TIME_0_MIN))
                {
                    app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_3_MIN;
                }
                else
                {
                    // do nothing
                }
#elif defined(WASH_STIR_TIME_CALCULATION_BY_SYSTEM_TABLES)
                for (uint8_t i = 0; i < WASH_STIR_STATE_NUM; i++)
                {
                    sum += app_nm_wash_wash_motorStirStateMotion_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, (app_nm_wash_motor_stirState_t)i);
                }

                if (app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_STIR] > sum * 3)
                {
                    app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_18_MIN;
                }
                else if (app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_STIR] > sum * 2)
                {
                    app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_12_MIN;
                }
                else if (app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_STIR] > sum)
                {
                    app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_9_MIN;
                }
                else if ((app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_STIR] <= sum) && (app_nm_data_get()->wash.washStateTimeMSec[APP_NORMAL_MODE_WASH_WASH_STIR] > APP_NORMAL_MODE_UI_WASH_TIME_0_MIN))
                {
                    app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_3_MIN;
                }
                else
                {
                    // do nothing
                }
#endif
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE:
        {
            app_userInterface_data_set()->normalMode.wash.waterTempSelection = WASH_RINSE_WATER_TEMP_SETTING;

            if (APP_NORMAL_MODE_WASH_RINSE_REGROUP == app_nm_wash_rinse_state_get())
            {
                // NOTE: this shouldn't happen for more than one tick
                app_userInterface_data_set()->normalMode.wash.rinseSelection--;
            }
            else if (APP_NORMAL_MODE_WASH_RINSE_SPIN_TRANSITION == app_nm_wash_rinse_state_get())
            {
                app_userInterface_data_set()->normalMode.wash.rinseSelection = APP_NORMAL_MODE_UI_RINSE_0_TIMES;
                app_userInterface_data_set()->normalMode.wash.extraRinseSelection = APP_NORMAL_MODE_UI_RINSE_OVERFLOW_OFF_SHOWER_OFF;
            }
            else
            {
                // do nothing
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN:
        {
            if (APP_NORMAL_MODE_WASH_SPIN_END_TRANSITION == app_nm_wash_spin_state_get())
            {
                app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN;
                app_userInterface_data_set()->normalMode.wash.antiWrinkleSelection = APP_NORMAL_MODE_UI_ANTI_WRINKLE_TIME_0_MIN;
                app_userInterface_data_set()->normalMode.wash.superSpinSelection = APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN;
            }
            else
            {
                if ((app_nm_data_get()->wash.spinStateTimeMSec[APP_NORMAL_MODE_WASH_SPIN_SPIN] / 60000U) == APP_NORMAL_MODE_UI_SPIN_TIME_5_MIN)
                {
                    app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_5_MIN;
                    app_nm_wash_currentUI_save();
                }
                else if (((app_nm_data_get()->wash.spinStateTimeMSec[APP_NORMAL_MODE_WASH_SPIN_SPIN] / 60000U) == APP_NORMAL_MODE_UI_SPIN_TIME_1_MIN) && (app_nm_data_get()->wash.spinStateTimeMSec[APP_NORMAL_MODE_WASH_SPIN_SPIN] / 60000U) > APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN)
                {
                    app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_1_MIN;
                    app_nm_wash_currentUI_save();
                }
                else
                {
                    // do nothing
                }
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_FINISH:
        {
            app_userInterface_data_set()->normalMode.wash.waterTempSelection = APP_NORMAL_MODE_UI_WATER_TEMP_NONE;
            app_userInterface_data_set()->normalMode.wash.soakSelection = APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR;
            app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_0_MIN;
            app_userInterface_data_set()->normalMode.wash.soilLevelSelection = APP_NORMAL_MODE_UI_SOIL_LEVEL_NONE;
            app_userInterface_data_set()->normalMode.wash.rinseSelection = APP_NORMAL_MODE_UI_RINSE_0_TIMES;
            app_userInterface_data_set()->normalMode.wash.extraRinseSelection = APP_NORMAL_MODE_UI_RINSE_OVERFLOW_OFF_SHOWER_OFF;
            app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN;
            app_userInterface_data_set()->normalMode.wash.antiWrinkleSelection = APP_NORMAL_MODE_UI_ANTI_WRINKLE_TIME_0_MIN;
            app_userInterface_data_set()->normalMode.wash.superSpinSelection = APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN;
            app_userInterface_data_set()->normalMode.wash.waterHeatingSelection = APP_NORMAL_MODE_UI_WATER_HEATING_OFF;
            app_userInterface_data_set()->normalMode.wash.steamTechSelection = APP_NORMAL_MODE_UI_STEAM_TECH_OFF;
            app_userInterface_data_set()->normalMode.wash.gelDetergentSelection = APP_NORMAL_MODE_UI_GEL_DETERGENT_OFF;
        }
        break;

        case APP_NORMAL_MODE_WASH_INIT:
        case APP_NORMAL_MODE_WASH_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

uint8_t app_nm_wash_userInterface_overloadingFactor_get(void)
{
    uint8_t ret = 0;

    ret = *(ptrOverloadingData[app_userInterface_data_get()->normalMode.common.courseSelection - 1]);

    return ret;
}

const app_nm_wash_userInterface_defaults_data_t *app_nm_wash_userInterface_defaults_data_get(void)
{
    const app_nm_wash_userInterface_defaults_data_t *ret = NULL;

    ret = (ptrDefaultData[app_userInterface_data_get()->normalMode.common.courseSelection - 1] + (app_userInterface_data_get()->normalMode.common.operationSelection));

    return ret;
}

const app_nm_wash_userInterface_defaults_data_t *app_nm_wash_userInterface_lowLimitDefaults_data_get(void)
{
    const app_nm_wash_userInterface_defaults_data_t *ret = NULL;

    ret = (ptrLowLimitData[app_userInterface_data_get()->normalMode.common.courseSelection - 1] + (app_userInterface_data_get()->normalMode.common.operationSelection));

    return ret;
}

const app_nm_wash_userInterface_defaults_data_t *app_nm_wash_userInterface_highLimitDefaults_data_get(void)
{
    const app_nm_wash_userInterface_defaults_data_t *ret = NULL;

    ret = (ptrHighLimitData[app_userInterface_data_get()->normalMode.common.courseSelection - 1] + (app_userInterface_data_get()->normalMode.common.operationSelection));

    return ret;
}

app_nm_wash_userInterface_washTimeSelection_t app_nm_wash_userInterface_washTimePerWaterLevel_get(void)
{
    app_nm_wash_userInterface_washTimeSelection_t ret = (app_nm_wash_userInterface_washTimeSelection_t)0;

    ret = ptrDefaultWashPerLevel[app_userInterface_data_get()->normalMode.common.courseSelection - 1][app_userInterface_data_get()->normalMode.wash.waterLevelSelection];

    return ret;
}

static void setup_init_handle(uint32_t period)
{
    if (app_userInterface_data_get()->normalMode.wash.washSelection == APP_NORMAL_MODE_UI_WASH_TIME_0_MIN)
    {
        if ((app_userInterface_data_get()->normalMode.wash.waterLevelSelection == APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0) &&
            (app_userInterface_data_get()->normalMode.wash.spinSelection || app_userInterface_data_get()->normalMode.wash.rinseSelection || app_userInterface_data_get()->normalMode.wash.superSpinSelection))
        {
            app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_MAX_WATER_LEVEL;
        }
    }
    else
    {
        if (app_userInterface_data_get()->normalMode.wash.waterLevelSelection == APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0)
        {
            // there is water in tub?
            if (APP_FILL_WATER_LEVEL_OK == app_fill_waterLevelState_get(WASH_WATER_LEVEL_LIMIT_AT_STARTING_LITTER, 0))
            {
                app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_MAX_WATER_LEVEL;
            }
        }
    }
}
static void setup_weightDetection_handle(uint32_t period)
{
    app_userInterface_data_set()->normalMode.wash.waterLevelSelection = (app_nm_wash_userInterface_waterLevelSelection_t)app_nm_wash_setup_weightData_get();
}

static void wash_userInterface_normalMode_handle(void)
{
    wash_normalMode_operation_update();
    wash_normalMode_course_update();
    wash_normalMode_delayStart_update();
    wash_normalMode_childLock_update();
    wash_normalMode_tubclean_update();

    wash_wash_userinterface_normalMode_handle();
    dry_wash_userinterface_normalMode_handle();
}

static void wash_wash_userinterface_normalMode_handle(void)
{
    wash_normalMode_waterTemp_update();
    wash_normalMode_waterLevel_update();
    wash_normalMode_soak_update();
    wash_normalMode_wash_update();
    wash_normalMode_rinse_update();
    wash_normalMode_extraRinse_update();
    wash_normalMode_spin_update();
    wash_normalMode_superSpin_update();
    wash_normalMode_soilLevel_update();
    wash_normalMode_antiWrinkle_update();
    wash_normalMode_steamTech_update();
    wash_normalMode_gelDetergent_update();
}

static void dry_wash_userinterface_normalMode_handle(void)
{
    wash_normalMode_dryTime_update();
    wash_normalMode_drynessIntensity_update();
}

static void wash_normalMode_operation_update(void)
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
static void wash_normalMode_course_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_courseKeyEdgeState_get())
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void wash_normalMode_delayStart_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->delayStartKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void wash_normalMode_childLock_update(void)
{
    // do nothing
}
static void wash_normalMode_tubclean_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->tubCleanKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}

static void wash_normalMode_waterTemp_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->waterTempKeyEdgeState)
    {
#ifdef WATER_TEMP_CHANGE_DURING_CYCLE_ENABLE
        // NOTE: check first if it is disabled in this course by checking the limits
        if ((0 == app_nm_data_get()->wash.washWashTotalTimeMSec) || ((app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_NONE) && (app_nm_wash_userInterface_lowLimitDefaults_data_get()->defaultWaterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_NONE)))
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
#else
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
#endif
    }
}
static void wash_normalMode_waterLevel_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->waterLevelKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void wash_normalMode_soak_update(void)
{
#ifndef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->soakKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
static void wash_normalMode_wash_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->washKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void wash_normalMode_rinse_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->rinseKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void wash_normalMode_extraRinse_update(void)
{
    // NOTE: doesn't exist in the current model
}
static void wash_normalMode_spin_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->spinKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void wash_normalMode_superSpin_update(void)
{
    // NOTE: doesn't exist in the current model
}
static void wash_normalMode_soilLevel_update(void)
{
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->stainLevelKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
}
static void wash_normalMode_antiWrinkle_update(void)
{
    // NOTE: can't change in this model
}
static void wash_normalMode_steamTech_update(void)
{
#ifdef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->steamTechKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
static void wash_normalMode_gelDetergent_update(void)
{
#ifdef AUTO_DETERGENT_HEATER_MODEL
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->gelDetergentKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}

static void wash_normalMode_dryTime_update(void)
{
#ifdef DRYER_ENABLED
    if (HANDLER_KEY_PRESSED == app_userInterface_keyEdgeState_get()->dryTimeKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
static void wash_normalMode_drynessIntensity_update(void)
{
#ifdef DRYER_ENABLED
    if (HANDLER_KEY_LONG_PRESSED == app_userInterface_keyEdgeState_get()->dryTimeKeyEdgeState)
    {
        app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
    }
#endif
}
