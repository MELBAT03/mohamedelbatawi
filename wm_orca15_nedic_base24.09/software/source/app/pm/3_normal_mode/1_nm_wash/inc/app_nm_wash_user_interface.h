#ifndef APP_NM_WASH_USER_INTERFACE_H
#define APP_NM_WASH_USER_INTERFACE_H

#include "def.h"

#define APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT            (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_10)
#define APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_DURATION_DEFAULT (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_5)

typedef enum app_nm_wash_userInterface_waterTempSelection
{
    APP_NORMAL_MODE_UI_WATER_TEMP_NONE = 0U,
    APP_NORMAL_MODE_UI_WATER_TEMP_COLD = 1U,
    APP_NORMAL_MODE_UI_WATER_TEMP_WARM = 2U,
    APP_NORMAL_MODE_UI_WATER_TEMP_HOT = 3U
} app_nm_wash_userInterface_waterTempSelection_t;

typedef enum app_nm_wash_userInterface_waterHeatingSelection
{
    APP_NORMAL_MODE_UI_WATER_HEATING_OFF = 0U,
    APP_NORMAL_MODE_UI_WATER_HEATING_ON = 1U
} app_nm_wash_userInterface_waterHeatingSelection_t;

typedef enum app_nm_wash_userInterface_steamTechSelection
{
    APP_NORMAL_MODE_UI_STEAM_TECH_OFF = 0U,
    APP_NORMAL_MODE_UI_STEAM_TECH_ON = 1U
} app_nm_wash_userInterface_steamTechSelection_t;

typedef enum app_nm_wash_userInterface_gelDetergentSelection
{
    APP_NORMAL_MODE_UI_GEL_DETERGENT_OFF = 0U,
    APP_NORMAL_MODE_UI_GEL_DETERGENT_ON = 1U
} app_nm_wash_userInterface_gelDetergentSelection_t;

typedef enum app_nm_wash_userInterface_waterLevelSelection
{
    APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0 = 0U,
    APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 = 1U,
    APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_2 = 2U,
    APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_3 = 3U,
    APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_4 = 4U,
    APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_5 = 5U,
    APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_6 = 6U,
    APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_7 = 7U,
    APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_8 = 8U,
    APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_9 = 9U,
    APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_10 = 10U
} app_nm_wash_userInterface_waterLevelSelection_t;

typedef enum app_nm_wash_userInterface_soakTimeSelection
{
    APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR = 0U,
    APP_NORMAL_MODE_UI_SOAK_TIME_1_HOUR = 1U,
    APP_NORMAL_MODE_UI_SOAK_TIME_2_HOUR = 2U,
    APP_NORMAL_MODE_UI_SOAK_TIME_3_HOUR = 3U,
    APP_NORMAL_MODE_UI_SOAK_TIME_4_HOUR = 4U,
    APP_NORMAL_MODE_UI_SOAK_SELECTION_NUM = 5U
} app_nm_wash_userInterface_soakTimeSelection_t;

typedef enum app_nm_wash_userInterface_washTimeSelection
{
    APP_NORMAL_MODE_UI_WASH_TIME_0_MIN = 0U,
    APP_NORMAL_MODE_UI_WASH_TIME_3_MIN = 3U,
    APP_NORMAL_MODE_UI_WASH_TIME_5_MIN = 5U,
    APP_NORMAL_MODE_UI_WASH_TIME_9_MIN = 9U,
    APP_NORMAL_MODE_UI_WASH_TIME_12_MIN = 12U,
    APP_NORMAL_MODE_UI_WASH_TIME_18_MIN = 18U
} app_nm_wash_userInterface_washTimeSelection_t;

typedef enum app_nm_wash_userInterface_steamSelection
{
    APP_NORMAL_MODE_UI_STEAM_OFF = 0U,
    APP_NORMAL_MODE_UI_STEAM_ON = 1U,
    APP_NORMAL_MODE_UI_STEAM_SELECTION_NUM = 2U
} app_nm_wash_userInterface_steamSelection_t;

typedef enum app_nm_wash_userInterface_rinseTimesSelection
{
    APP_NORMAL_MODE_UI_RINSE_0_TIMES = 0U,
    APP_NORMAL_MODE_UI_RINSE_1_TIMES = 1U,
    APP_NORMAL_MODE_UI_RINSE_2_TIMES = 2U,
    APP_NORMAL_MODE_UI_RINSE_3_TIMES = 3U,
    APP_NORMAL_MODE_UI_RINSE_SELECTION_NUM = 4U
} app_nm_wash_userInterface_rinseTimesSelection_t;

typedef enum app_nm_wash_userInterface_extraRinseSelection
{
    APP_NORMAL_MODE_UI_RINSE_OVERFLOW_OFF_SHOWER_OFF = 0U,
    APP_NORMAL_MODE_UI_RINSE_OVERFLOW_ON_SHOWER_OFF = 1U,
    APP_NORMAL_MODE_UI_RINSE_OVERFLOW_OFF_SHOWER_ON = 2U,
    APP_NORMAL_MODE_UI_RINSE_OVERFLOW_ON_SHOWER_ON = 3U,
    APP_NORMAL_MODE_UI_RINSE_EXTENDED = 4U
} app_nm_wash_userInterface_extraRinseSelection_t;

typedef enum app_nm_wash_userInterface_spinTimeSelection
{
    APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN = 0U,
    APP_NORMAL_MODE_UI_SPIN_TIME_1_MIN = 1U,
    APP_NORMAL_MODE_UI_SPIN_TIME_5_MIN = 5U,
    APP_NORMAL_MODE_UI_SPIN_TIME_9_MIN = 9U,
    APP_NORMAL_MODE_UI_SPIN_TIME_20_MIN = 20U,
    APP_NORMAL_MODE_UI_SPIN_SELECTION_NUM
} app_nm_wash_userInterface_spinTimeSelection_t;

typedef enum app_nm_wash_userInterface_soilLevelSelection
{
    APP_NORMAL_MODE_UI_SOIL_LEVEL_NONE = 0U,
    APP_NORMAL_MODE_UI_SOIL_LEVEL_LIGHT = 1U,
    APP_NORMAL_MODE_UI_SOIL_LEVEL_NORMAL = 2U,
    APP_NORMAL_MODE_UI_SOIL_LEVEL_HEAVY = 3U
} app_nm_wash_userInterface_soilLevelSelection_t;

typedef enum app_nm_wash_userInterface_superSpinSelection
{
    APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN = 0U,
    APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_20_MIN = 20U,
    APP_NORMAL_MODE_UI_SUPER_SPIN_SELECTION_NUM
} app_nm_wash_userInterface_superSpinSelection_t;

typedef enum app_nm_wash_userInterface_antiWrinkleSelection
{
    APP_NORMAL_MODE_UI_ANTI_WRINKLE_TIME_0_MIN = 0U,
    APP_NORMAL_MODE_UI_ANTI_WRINKLE_TIME_1_MIN = 1U,
    APP_NORMAL_MODE_UI_ANTI_WRINKLE_TIME_2_MIN = 2U
} app_nm_wash_userInterface_antiWrinkleSelection_t;

typedef struct app_nm_wash_userInterface_data
{
    app_nm_wash_userInterface_waterTempSelection_t waterTempSelection;
    app_nm_wash_userInterface_waterLevelSelection_t waterLevelSelection;
    app_nm_wash_userInterface_waterLevelSelection_t waterLevelDisplaySelection;
    app_nm_wash_userInterface_soakTimeSelection_t soakSelection;
    app_nm_wash_userInterface_washTimeSelection_t washSelection;
    app_nm_wash_userInterface_steamSelection_t steamSelection;
    app_nm_wash_userInterface_rinseTimesSelection_t rinseSelection;
    app_nm_wash_userInterface_extraRinseSelection_t extraRinseSelection;
    app_nm_wash_userInterface_spinTimeSelection_t spinSelection;
    app_nm_wash_userInterface_soilLevelSelection_t soilLevelSelection;
    app_nm_wash_userInterface_superSpinSelection_t superSpinSelection;
    app_nm_wash_userInterface_antiWrinkleSelection_t antiWrinkleSelection;
    app_nm_wash_userInterface_waterHeatingSelection_t waterHeatingSelection;
    app_nm_wash_userInterface_steamTechSelection_t steamTechSelection;
    app_nm_wash_userInterface_gelDetergentSelection_t gelDetergentSelection;
} app_nm_wash_userInterface_data_t;

// defaults
typedef struct app_nm_wash_userInterface_defaults_data
{
    app_nm_wash_userInterface_waterTempSelection_t defaultWaterTemp;
    app_nm_wash_userInterface_waterLevelSelection_t defaultWaterLevel;
    app_nm_wash_userInterface_soakTimeSelection_t defaultSoakTime;
    app_nm_wash_userInterface_washTimeSelection_t defaultWashTime;
    app_nm_wash_userInterface_steamSelection_t defaultSteam;
    app_nm_wash_userInterface_rinseTimesSelection_t defaultRinseTime;
    app_nm_wash_userInterface_extraRinseSelection_t defaultExtraRinse;
    app_nm_wash_userInterface_spinTimeSelection_t defaultSpinTime;
    app_nm_wash_userInterface_soilLevelSelection_t defaultSoilLevel;
    app_nm_wash_userInterface_superSpinSelection_t defaultSuperSpinTime;
    app_nm_wash_userInterface_antiWrinkleSelection_t defaultAntiWrinkle;
    app_nm_wash_userInterface_waterHeatingSelection_t defaultWaterHeating;
    app_nm_wash_userInterface_steamTechSelection_t defaultSteamTech;
    app_nm_wash_userInterface_gelDetergentSelection_t defaultGelDetergent;
} app_nm_wash_userInterface_defaults_data_t;

void app_nm_wash_userInterface_process(uint32_t period);

uint8_t app_nm_wash_userInterface_overloadingFactor_get(void);

const app_nm_wash_userInterface_defaults_data_t *app_nm_wash_userInterface_defaults_data_get(void);
const app_nm_wash_userInterface_defaults_data_t *app_nm_wash_userInterface_lowLimitDefaults_data_get(void);
const app_nm_wash_userInterface_defaults_data_t *app_nm_wash_userInterface_highLimitDefaults_data_get(void);
app_nm_wash_userInterface_washTimeSelection_t app_nm_wash_userInterface_washTimePerWaterLevel_get(void);

// NOTE: wash defaults
const app_nm_wash_userInterface_defaults_data_t *app_nm_wash_userInterface_defaults_data_get(void);

#endif
