#ifndef APP_NM_DRY_H
#define APP_NM_DRY_H

#include "def.h"
#include "app_user_interface.h"
#include "app_nm.h"
#include "app_nm_dry_data.h"

#include "app_memory.h"

#define APP_NORMAL_MODE_DRY_MAX_WATER_LEVEL (6U)

typedef enum app_nm_dry_state
{
    APP_NORMAL_MODE_DRY_INIT,
    APP_NORMAL_MODE_DRY_PRE_SETUP,
    APP_NORMAL_MODE_DRY_SETUP,
    APP_NORMAL_MODE_DRY_OPERATION,
    APP_NORMAL_MODE_DRY_FINISH,
    APP_NORMAL_MODE_DRY_DONE
} app_nm_dry_state_t;

typedef enum app_nm_dry_preSetup_state
{
    APP_NORMAL_MODE_DRY_PRE_SETUP_INIT,
    APP_NORMAL_MODE_DRY_PRE_SETUP_DRAIN,
    APP_NORMAL_MODE_DRY_PRE_SETUP_SPIN,
    APP_NORMAL_MODE_DRY_PRE_SETUP_UNTANGLE,
    APP_NORMAL_MODE_DRY_PRE_SETUP_FINISH,
    APP_NORMAL_MODE_DRY_PRE_SETUP_DONE
} app_nm_dry_preSetup_state_t;

typedef enum app_nm_dry_setup_state
{
    APP_NORMAL_MODE_DRY_SETUP_INIT,
    APP_NORMAL_MODE_DRY_SETUP_WEIGHT_SENSE,
    APP_NORMAL_MODE_DRY_SETUP_FINISH,
    APP_NORMAL_MODE_DRY_SETUP_DONE
} app_nm_dry_setup_state_t;

typedef enum app_nm_dry_operation_state
{
    APP_NORMAL_MODE_DRY_OPERATION_INIT,
    APP_NORMAL_MODE_DRY_OPERATION_DRAIN,
    APP_NORMAL_MODE_DRY_OPERATION_FINAL_SPIN,
    APP_NORMAL_MODE_DRY_OPERATION_DRYING_SPIN,
    APP_NORMAL_MODE_DRY_OPERATION_BRAKE,
    APP_NORMAL_MODE_DRY_OPERATION_UNTANGLE,
    APP_NORMAL_MODE_DRY_OPERATION_HEATING,
    APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_1,
    APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_2,
    APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME,
    APP_NORMAL_MODE_DRY_OPERATION_COOLING_WATER,
    APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR,
    APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED,
    APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED_RESET,
    APP_NORMAL_MODE_DRY_OPERATION_FINISH,
    APP_NORMAL_MODE_DRY_OPERATION_DONE
} app_nm_dry_operation_state_t;

typedef struct app_dry_operation_timeTable
{
    uint32_t stateTotalTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_STATES_WITH_DATA];
} app_dry_operation_timeTable_t;

typedef struct app_dry_setup_timeTable
{
    uint32_t stateTotalTimeMSec[APP_NORMAL_MODE_DRY_PRE_SETUP_STATES_WITH_DATA];
} app_dry_setup_timeTable_t;

// NOTE: main dry process
app_nm_state_t app_nm_dry_process(uint32_t period);

app_nm_dry_state_t app_nm_dry_state_get(void);
void app_nm_dry_state_set(app_nm_dry_state_t value);

// NOTE: presetup process
void app_nm_dry_preSetup_process(uint32_t period);

void app_nm_dry_data_update(void);

void app_nm_dry_totalTime_update(void);
void app_nm_dry_currentUI_save(void);

app_nm_dry_preSetup_state_t app_nm_dry_preSetup_state_get(void);
void app_nm_dry_preSetup_state_set(app_nm_dry_preSetup_state_t value);
void app_nm_dry_preSetup_reset(void);
const app_dry_setup_timeTable_t *app_nm_dry_preSetup_stateTotalTimeMSec_get(app_nm_userInterface_dryTimeSelection_t selection);

// NOTE: setup process
void app_nm_dry_setup_process(uint32_t period);

app_nm_dry_setup_state_t app_nm_dry_setup_state_get(void);
void app_nm_dry_setup_state_set(app_nm_dry_setup_state_t value);
uint32_t app_nm_dry_setup_weightData_get(void);
void app_nm_dry_setup_reset(void);

// NOTE: operation process
void app_nm_dry_operation_process(uint32_t period);

app_nm_dry_operation_state_t app_nm_dry_operation_state_get(void);
void app_nm_dry_operation_state_set(app_nm_dry_operation_state_t value);
void app_nm_dry_operation_reset(void);

const app_dry_operation_timeTable_t *app_nm_dry_operation_stateTotalMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel, app_nm_userInterface_drynessIntensitySelection_t dryness);

void app_nm_dry_reset(void);

#endif
