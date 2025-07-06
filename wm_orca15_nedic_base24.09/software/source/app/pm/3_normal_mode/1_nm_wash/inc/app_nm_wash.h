#ifndef APP_NM_WASH_H
#define APP_NM_WASH_H

#include "def.h"
#include "app_user_interface.h"
#include "app_nm.h"
#include "app_nm_wash_data.h"

#define APP_NORMAL_MODE_WASH_MAX_WATER_LEVEL ((app_nm_wash_userInterface_waterLevelSelection_t)10)

#define WASH_FILL_MINIMUM_ALLOWED_VARIANCE (50U)
#define WASH_FILL_MAXIMUM_ALLOWED_VARIANCE (100U)

typedef enum app_nm_wash_state
{
    APP_NORMAL_MODE_WASH_INIT = 0,
    APP_NORMAL_MODE_WASH_SETUP = 1,
    APP_NORMAL_MODE_WASH_SOAK = 2,
    APP_NORMAL_MODE_WASH_WASH = 3,
    APP_NORMAL_MODE_WASH_STEAM = 4,
    APP_NORMAL_MODE_WASH_RINSE = 5,
    APP_NORMAL_MODE_WASH_SPIN = 6,
    APP_NORMAL_MODE_WASH_FINISH = 7,
    APP_NORMAL_MODE_WASH_DONE = 8
} app_nm_wash_state_t;

typedef enum app_nm_wash_setup_state
{
    APP_NORMAL_MODE_WASH_SETUP_INIT = 0,
    APP_NORMAL_MODE_WASH_SETUP_CHECK_WATER_LEVEL = 1,
    APP_NORMAL_MODE_WASH_SETUP_WEIGHT_DETECTION = 2,
    APP_NORMAL_MODE_WASH_SETUP_DETERGENT_ADD = 3,
    APP_NORMAL_MODE_WASH_SETUP_FINISH = 4,
    APP_NORMAL_MODE_WASH_SETUP_DONE = 5
} app_nm_wash_setup_state_t;

typedef enum app_nm_wash_soak_state
{
    APP_NORMAL_MODE_WASH_SOAK_INIT = 0,
    APP_NORMAL_MODE_WASH_SOAK_OPERATION = 1,
    APP_NORMAL_MODE_WASH_SOAK_FINISH = 2,
    APP_NORMAL_MODE_WASH_SOAK_WASH_TRANSITION = 3,
    APP_NORMAL_MODE_WASH_SOAK_DONE = 4
} app_nm_wash_soak_state_t;

typedef enum app_nm_wash_wash_state
{
    APP_NORMAL_MODE_WASH_WASH_INIT = 0,
    APP_NORMAL_MODE_WASH_WASH_REFILL = 1,
    APP_NORMAL_MODE_WASH_WASH_SETUP = 2,
    APP_NORMAL_MODE_WASH_WASH_M0 = 3,
    APP_NORMAL_MODE_WASH_WASH_MH = 4,
    APP_NORMAL_MODE_WASH_WASH_MD = 5,
    APP_NORMAL_MODE_WASH_WASH_M1 = 6,
    APP_NORMAL_MODE_WASH_WASH_M2 = 7,
    APP_NORMAL_MODE_WASH_WASH_M3 = 8,
    APP_NORMAL_MODE_WASH_WASH_M4 = 9,
    APP_NORMAL_MODE_WASH_WASH_HEATING = 10,
    APP_NORMAL_MODE_WASH_WASH_STIR = 11,
    APP_NORMAL_MODE_WASH_WASH_UNTANGLE = 12,
    APP_NORMAL_MODE_WASH_WASH_FINISH = 13,
    APP_NORMAL_MODE_WASH_WASH_SOAK_TRANSITION = 14,
    APP_NORMAL_MODE_WASH_WASH_RINSE_TRANSITION = 15,
    APP_NORMAL_MODE_WASH_WASH_DONE = 16,
    APP_NORMAL_MODE_WASH_WASH_STATE_NUM
} app_nm_wash_wash_state_t;

typedef enum app_nm_wash_steamTech_state
{
    APP_NORMAL_MODE_WASH_STEAM_TECH_INIT = 0U,
    APP_NORMAL_MODE_WASH_STEAM_TECH_MH = 1U,
    APP_NORMAL_MODE_WASH_STEAM_TECH_STOP1 = 2U,
    APP_NORMAL_MODE_WASH_STEAM_TECH_MD = 3U,
    APP_NORMAL_MODE_WASH_STEAM_TECH_STOP2 = 4U,
    APP_NORMAL_MODE_WASH_STEAM_TECH_FINISH = 5U,
    APP_NORMAL_MODE_WASH_STEAM_TECH_DONE = 6U
} app_nm_wash_steamTech_state_t;

typedef enum app_nm_wash_steam_state
{
    APP_NORMAL_MODE_WASH_STEAM_INIT = 0,
    APP_NORMAL_MODE_WASH_STEAM_DRAIN = 1,
    APP_NORMAL_MODE_WASH_STEAM_SPIN = 2,
    APP_NORMAL_MODE_WASH_STEAM_STEAM = 3,
    APP_NORMAL_MODE_WASH_STEAM_FINISH = 4,
    APP_NORMAL_MODE_WASH_STEAM_RINSE_TRANSITION = 5,
    APP_NORMAL_MODE_WASH_STEAM_DONE = 6
} app_nm_wash_steam_state_t;

typedef enum app_nm_wash_rinse_state
{
    APP_NORMAL_MODE_WASH_RINSE_INIT = 0,
    APP_NORMAL_MODE_WASH_RINSE_DRAIN = 1,
    APP_NORMAL_MODE_WASH_RINSE_SPIN = 2,
    APP_NORMAL_MODE_WASH_RINSE_SPIN_BRAKE = 3,
    APP_NORMAL_MODE_WASH_RINSE_FILL = 4,
    APP_NORMAL_MODE_WASH_RINSE_STIR = 5,
    APP_NORMAL_MODE_WASH_RINSE_UNTANGLE = 6,
    APP_NORMAL_MODE_WASH_RINSE_UNBALANCE = 7,
    APP_NORMAL_MODE_WASH_RINSE_REGROUP = 8,
    APP_NORMAL_MODE_WASH_RINSE_FINISH = 9,
    APP_NORMAL_MODE_WASH_RINSE_SPIN_TRANSITION = 10,
    APP_NORMAL_MODE_WASH_RINSE_DONE = 11
} app_nm_wash_rinse_state_t;

typedef enum app_nm_wash_rinse_unbalance_state
{
    APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_INIT = 0,
    APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_SETUP = 1,
    APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_FILL = 2,
    APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_STIR = 3,
    APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_DRAIN = 4,
    APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_FINISH = 5,
    APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_DONE = 6
} app_nm_wash_rinse_unbalance_state_t;

typedef enum app_nm_wash_spin_state
{
    APP_NORMAL_MODE_WASH_SPIN_INIT = 0,
    APP_NORMAL_MODE_WASH_SPIN_DRAIN = 1,
    APP_NORMAL_MODE_WASH_SPIN_SPIN = 2,
    APP_NORMAL_MODE_WASH_SPIN_SUPER_SPIN = 3,
    APP_NORMAL_MODE_WASH_SPIN_BRAKE = 4,
    APP_NORMAL_MODE_WASH_SPIN_ANTI_WRINKLE = 5,
    APP_NORMAL_MODE_WASH_SPIN_UNBALANCE = 6,
    APP_NORMAL_MODE_WASH_SPIN_FINISH = 7,
    APP_NORMAL_MODE_WASH_SPIN_END_TRANSITION = 8,
    APP_NORMAL_MODE_WASH_SPIN_DONE = 9
} app_nm_wash_spin_state_t;

typedef enum app_nm_wash_spin_unbalance_state
{
    APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_INIT = 0,
    APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_SETUP = 1,
    APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_FILL = 2,
    APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_STIR = 3,
    APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_DRAIN = 4,
    APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_SUPER_SPIN_STIR = 5,
    APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_FINISH = 6,
    APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_DONE = 7,
    APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_ERROR = 8
} app_nm_wash_spin_unbalance_state_t;

typedef enum app_nm_wash_wash_waterLeakage_state
{
    APP_NORMAL_MODE_WASH_WASH_NO_LEAKAGE,
    APP_NORMAL_MODE_WASH_WASH_LEAKAGE
} app_nm_wash_wash_waterLeakage_state_t;

// wash processor
app_nm_state_t app_nm_wash_process(uint32_t period);
app_nm_wash_state_t app_nm_wash_state_get(void);

void app_nm_wash_reset(void);

void app_nm_wash_state_set(app_nm_wash_state_t value);

void app_nm_wash_data_update(void);
void app_nm_wash_totalTimeMSec_update(void);
void app_nm_wash_soakTotalTime_update(void);
void app_nm_wash_washTotalTime_update(void);
void app_nm_wash_steamTotalTime_update(void);
void app_nm_wash_rinseTotalTime_update(void);
void app_nm_wash_spinTotalTime_update(void);
void app_nm_wash_tubcleanTotalTime_update(void);
void app_nm_wash_currentUI_save(void);

// setup
void app_nm_wash_setup_process(uint32_t period);
app_nm_wash_setup_state_t app_nm_wash_setup_state_get(void);
void app_nm_wash_setup_state_set(app_nm_wash_setup_state_t value);

uint32_t app_nm_wash_setup_weightData_get(void);

void app_nm_wash_setup_reset(void);

uint8_t app_nm_wash_setup_overloadFlag_get(void);
void app_nm_wash_setup_overloadFlag_set(uint8_t value);

// soak
void app_nm_wash_soak_process(uint32_t period);
app_nm_wash_soak_state_t app_nm_wash_soak_state_get(void);
void app_nm_wash_soak_state_set(app_nm_wash_soak_state_t value);

void app_nm_wash_soak_reset(void);

// wash
void app_nm_wash_wash_process(uint32_t period);
app_nm_wash_wash_state_t app_nm_wash_wash_state_get(void);
app_nm_wash_wash_state_t app_nm_wash_wash_refillExecludedState_get(void);
void app_nm_wash_wash_state_set(app_nm_wash_wash_state_t value);

void app_nm_wash_wash_reset(void);

app_nm_wash_wash_waterLeakage_state_t app_nm_wash_wash_leakageState_get(void);

// steam
void app_nm_wash_steam_process(uint32_t period);
app_nm_wash_steam_state_t app_nm_wash_steam_state_get(void);
void app_nm_wash_steam_state_set(app_nm_wash_steam_state_t value);

void app_nm_wash_steam_reset(void);

// rinse
void app_nm_wash_rinse_process(uint32_t period);
app_nm_wash_rinse_state_t app_nm_wash_rinse_state_get(void);
void app_nm_wash_rinse_state_set(app_nm_wash_rinse_state_t value);
app_nm_wash_rinse_unbalance_state_t app_nm_wash_rinse_unbalanceState_get(void);

void app_nm_wash_rinse_reset(void);

// spin
void app_nm_wash_spin_process(uint32_t period);
app_nm_wash_spin_state_t app_nm_wash_spin_state_get(void);
void app_nm_wash_spin_state_set(app_nm_wash_spin_state_t value);

app_nm_wash_spin_unbalance_state_t app_nm_wash_spin_unbalanceState_get(void);

void app_nm_wash_spin_reset(void);

// auto detergent table
typedef struct app_nm_wash_autoDetergent_data
{
    uint32_t coldTimeMSec;
    uint32_t warmTimeMSec;
    uint32_t hotTimeMSec;
} app_nm_wash_autoDetergent_data_t;

// wash table
typedef struct app_nm_wash_wash_data
{
    uint32_t timeMSec;
    uint8_t waterLevelLiter;
} app_nm_wash_wash_data_t;

typedef struct app_nm_wash_wash_stateTime
{
    app_nm_wash_wash_data_t state[APP_NORMAL_MODE_WASH_STATES_WITH_DATA];
} app_nm_wash_wash_stateTime_t;

typedef struct app_nm_wash_steamTech_stateTime
{
    uint32_t state[APP_NORMAL_MODE_WASH_STEAM_TECH_STATES_WITH_DATA + 1];
} app_nm_wash_steamTech_stateTime_t;

typedef struct app_nm_wash_steam_stateTime
{
    uint32_t state[APP_NORMAL_MODE_STEAM_STATES_WITH_DATA];
} app_nm_wash_steam_stateTime_t;

typedef struct app_nm_wash_steam_stateVsTempTime
{
    uint32_t tempCold;
    uint32_t tempWarm;
    uint32_t tempHot;
} app_nm_wash_steam_stateVsTempTime_t;

const app_nm_wash_wash_stateTime_t *app_nm_wash_wash_stateData_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_userInterface_washTimeSelection_t washSelection, app_nm_wash_userInterface_soilLevelSelection_t soilLevel);
const app_nm_wash_wash_stateTime_t *app_nm_wash_wash_soak_stateData_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel);
uint32_t app_nm_wash_wash_detergent_time_get(app_nm_wash_userInterface_waterLevelSelection_t waterLevel);

app_nm_wash_userInterface_waterTempSelection_t app_nm_wash_steam_waterTemp_map(uint16_t waterTemp);
const app_nm_wash_steam_stateTime_t *app_nm_wash_steam_stateData_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_userInterface_steamSelection_t steamSelection, app_nm_wash_userInterface_waterTempSelection_t waterTemp);

#endif
