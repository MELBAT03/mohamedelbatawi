#ifndef APP_NM_DRY_HEATER_H
#define APP_NM_DRY_HEATER_H

#include "def.h"

#include "app_heater.h"
#include "app_nm_user_interface.h"

#ifndef DRY_CF_DETECTION_ENABLE
#define DRY_CF_DETECTION_ENABLE (1U)
#endif

#ifndef DRY_HEATER_TARGET_TEMP
#define DRY_HEATER_TARGET_TEMP (85UL)
#endif

typedef enum app_nm_dry_heater_autoTerminationState
{
    APP_NORMAL_MODE_DRY_HEATER_AUTO_TERMINATION_NOK = 0,
    APP_NORMAL_MODE_DRY_HEATER_AUTO_TERMINATION_OK = 1
} app_nm_dry_heater_autoTerminationState_t;

typedef enum app_nm_dry_heater_lintFilterCloggingState
{
    APP_NORMAL_MODE_DRY_HEATER_LINT_FILTER_NOT_CLOGGED = 0,
    APP_NORMAL_MODE_DRY_HEATER_LINT_FILTER_CLOGGED = 1
} app_nm_dry_heater_lintFilterCloggingState_t;

typedef enum app_nm_dry_heater_patternEnum
{
    APP_NORMAL_MODE_DRY_HEATER_INIT_PATTERN,
    APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN
} app_nm_dry_heater_patternEnum_t;

typedef enum app_nm_dry_heater_initialPatternEnum
{
    APP_NORMAL_MODE_DRY_HEATER_INIT_PATTERN_STEP_0_0_PLATES,
    APP_NORMAL_MODE_DRY_HEATER_INIT_PATTERN_STEP_1_2_PLATES,
    APP_NORMAL_MODE_DRY_HEATER_INIT_PATTERN_STEP_2_1_PLATE,
    APP_NORMAL_MODE_DRY_HEATER_INIT_PATTERN_STEP_NUM
} app_nm_dry_heater_initialPatternEnum_t;

typedef enum app_nm_dry_heater_mainPatternEnum
{
    APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_0_2_PLATES,
    APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_1_1_PLATE,
    APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_2_0_PLATE,
    APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_3_1_PLATE,
    APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_NUM
} app_nm_dry_heater_mainPatternEnum_t;

typedef struct app_nm_dry_heater_pattern
{
    uint32_t initialPatternIntervalMSec[APP_NORMAL_MODE_DRY_HEATER_INIT_PATTERN_STEP_NUM];
    uint32_t mainPatternIntervalMSec[APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_NUM];
} app_nm_dry_heater_pattern_t;

typedef enum app_nm_dry_heater_patternSelection
{
    APP_NORMAL_MODE_DRY_HEATER_DEFAULT_PATTERN = 0,
    APP_NORMAL_MODE_DRY_HEATER_MANUAL_PATTERN = APP_NORMAL_MODE_DRY_HEATER_DEFAULT_PATTERN,
    APP_NORMAL_MODE_DRY_HEATER_AUTO_1_PATTERN = 1,
    APP_NORMAL_MODE_DRY_HEATER_AUTO_2_PATTERN = 2,
    APP_NORMAL_MODE_DRY_HEATER_AUTO_3_PATTERN = 3,
    APP_NORMAL_MODE_DRY_HEATER_PATTERN_NUM = 4
} app_nm_dry_heater_patternSelection_t;

void app_nm_dry_heater_process(uint32_t period);

uint32_t app_nm_dry_heater_patternPeriod_get(void);
void app_nm_dry_heater_patternPeriod_set(uint32_t value);

app_nm_dry_heater_patternEnum_t app_nm_dry_heater_patternState_get(void);
app_nm_dry_heater_initialPatternEnum_t app_nm_dry_heater_initialPatternState_get(void);
app_nm_dry_heater_mainPatternEnum_t app_nm_dry_heater_mainPatternState_get(void);
app_nm_dry_heater_patternSelection_t app_nm_dry_heater_patternSelection_get(void);

app_nm_dry_heater_autoTerminationState_t app_nm_dry_heater_autoTerminationState_get(void);
app_nm_dry_heater_lintFilterCloggingState_t app_nm_dry_heater_lintFilterCloggingState_get(void);
void app_nm_dry_heater_lintFilterCloggingState_set(app_nm_dry_heater_lintFilterCloggingState_t value);

app_heater_plateState_t app_nm_dry_heater_allowedMaxPlatesPerCourse_get(app_nm_userInterface_courseSelection_t course);

const app_nm_dry_heater_pattern_t *app_nm_dry_heater_table_get(app_nm_dry_heater_patternSelection_t selection);

#endif
