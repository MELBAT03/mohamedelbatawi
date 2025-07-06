#ifndef APP_NM_DRY_USER_INTERFACE_H
#define APP_NM_DRY_USER_INTERFACE_H

#include "def.h"

typedef enum app_nm_userInterface_dryTimeSelection
{
    APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN = 0,
    APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN = 1,
    APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_60_MIN = 2,
    APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_90_MIN = 3,
    APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO = 4
} app_nm_userInterface_dryTimeSelection_t;

typedef enum app_nm_userInterface_drynessIntensitySelection
{
    APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE = 0,
    APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL = 1,
    APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_HEAVY = 2
} app_nm_userInterface_drynessIntensitySelection_t;

typedef enum app_nm_dry_userInterface_waterLevelSelection
{
    APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0 = 0,
    APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_1 = 1,
    APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_2 = 2,
    APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_3 = 3,
    APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_4 = 4,
    APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_5 = 5,
    APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6 = 6
} app_nm_dry_userInterface_waterLevelSelection_t;

typedef struct app_nm_dry_userInterface_data
{
    app_nm_userInterface_dryTimeSelection_t dryTimeSelection;
    app_nm_userInterface_drynessIntensitySelection_t dryIntensitySelection;
    app_nm_dry_userInterface_waterLevelSelection_t dryWaterLevel;
} app_nm_dry_userInterface_data_t;

typedef struct app_nm_dry_userInterafce_defaults_data
{
    app_nm_userInterface_dryTimeSelection_t defaultDryTime;
    app_nm_userInterface_drynessIntensitySelection_t defaultDryIntensity;
    app_nm_dry_userInterface_waterLevelSelection_t defaultDryWaterLevel;
} app_nm_dry_userInterafce_defaults_data_t;

void app_nm_dry_userInterface_process(uint32_t period);

// NOTE: dry defaults
const app_nm_dry_userInterafce_defaults_data_t *app_nm_dry_userInterface_defaults_data_get(void);
const app_nm_dry_userInterafce_defaults_data_t *app_nm_dry_userInterface_lowLimit_data_get(void);
const app_nm_dry_userInterafce_defaults_data_t *app_nm_dry_userInterface_highLimit_data_get(void);

#endif
