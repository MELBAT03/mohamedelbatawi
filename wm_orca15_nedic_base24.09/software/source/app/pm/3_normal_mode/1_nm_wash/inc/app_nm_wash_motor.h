#ifndef APP_NM_WASH_MOTOR_H
#define APP_NM_WASH_MOTOR_H

#include "def.h"

#include "app_motor.h"
#include "app_nm_wash.h"

typedef app_motor_motionPattern_t app_nm_wash_motor_motionPattern_t;

typedef enum app_nm_wash_motor_soak_state
{
    APP_NORMAL_MODE_WASH_MOTOR_SOAK_PAUSE_1 = 0,
    APP_NORMAL_MODE_WASH_MOTOR_SOAK_MOTION_1 = 1,
    APP_NORMAL_MODE_WASH_MOTOR_SOAK_PAUSE_2 = 2,
    APP_NORMAL_MODE_WASH_MOTOR_SOAK_MOTION_2 = 3
} app_nm_wash_motor_soak_state_t;

typedef struct app_nm_wash_motor_operationData
{
    app_nm_wash_motor_soak_state_t soakState;
} app_nm_wash_motor_operationData_t;

typedef struct app_nm_wash_soak_motorStatePattern
{
    app_nm_wash_motor_motionPattern_t m1Pattern;
    app_nm_wash_motor_motionPattern_t m2Pattern;
} app_nm_wash_soak_motorStatePattern_t;

typedef struct app_nm_wash_motor_soak_stateTable
{
    uint32_t soakState[APP_NORMAL_MODE_UI_SOAK_SELECTION_NUM];
} app_nm_wash_motor_soak_stateTable_t;

typedef struct app_nm_wash_motor_data
{
    app_nm_wash_motor_operationData_t operation;
} app_nm_wash_motor_data_t;

typedef struct app_nm_wash_motor_statePattern
{
    app_nm_wash_motor_motionPattern_t setupPattern;
    app_nm_wash_motor_motionPattern_t mhPattern;
    app_nm_wash_motor_motionPattern_t mdPattern;
    app_nm_wash_motor_motionPattern_t m1Pattern;
    app_nm_wash_motor_motionPattern_t m2Pattern;
    app_nm_wash_motor_motionPattern_t m3Pattern;
    app_nm_wash_motor_motionPattern_t m4Pattern;
    app_nm_wash_motor_motionPattern_t heatingPattern;
    app_nm_wash_motor_motionPattern_t m5Pattern;
    app_nm_wash_motor_motionPattern_t m6Pattern;
    app_nm_wash_motor_motionPattern_t m7Pattern;
    app_nm_wash_motor_motionPattern_t mUPattern;
    app_nm_wash_motor_motionPattern_t untanglePattern;
} app_nm_wash_motor_wash_statePattern_t;

typedef enum app_nm_wash_motor_stirState
{
    WASH_STIR_M5,
    WASH_STIR_M6,
    WASH_STIR_M7,
    WASH_STIR_MU,
    WASH_STIR_STATE_NUM
} app_nm_wash_motor_stirState_t;

typedef enum app_nm_wash_steam_motor_stirState
{
    STEAM_STIR_UNTANGLE,
    STEAM_STIR_MD,
    STEAM_STIR_M1,
    STEAM_STIR_M2,
    STEAM_STIR_M3,
    STEAM_STIR_STATE_NUM
} app_nm_wash_steam_motor_stirState_t;

typedef enum app_nm_wash_steamMH_motor_stirState
{
    STEAM_TECH_MH,
    STEAM_TECH_MH_STOP,
    STEAM_TECH_MH_STATE_NUM
} app_nm_wash_steamMH_motor_stirState_t;

typedef enum app_nm_wash_steamMD_motor_stirState
{
    STEAM_TECH_MD,
    STEAM_TECH_MD_STOP,
    STEAM_TECH_MD_STATE_NUM
} app_nm_wash_steamMD_motor_stirState_t;

typedef enum app_nm_wash_steamHeating_motor_stirState
{
    STEAM_TECH_HEATING,
    STEAM_TECH_HEATING_STOP,
    STEAM_TECH_HEATING_STATE_NUM
} app_nm_wash_steamHeating_motor_stirState_t;

typedef struct app_nm_wash_motor_stirStateTime
{
    uint32_t stirState[WASH_STIR_STATE_NUM];
} app_nm_wash_motor_stirStateTime_t;

typedef struct app_nm_wash_motor_MHStateTime
{
    uint32_t stirState[STEAM_TECH_MH_STATE_NUM];
} app_nm_wash_motor_MHStateTime_t;

typedef struct app_nm_wash_motor_MDStateTime
{
    uint32_t stirState[STEAM_TECH_MD_STATE_NUM];
} app_nm_wash_motor_MDStateTime_t;

typedef struct app_nm_wash_steam_motorStateTime
{
    uint32_t untangleStateTimeMSec;
    uint32_t stirStateTimeMSec[STEAM_STIR_STATE_NUM];
} app_nm_wash_steam_motorStateTime_t;

typedef struct app_nm_wash_steam_motor_statePattern
{
    app_nm_wash_motor_motionPattern_t pattern[STEAM_STIR_STATE_NUM];
} app_nm_wash_steam_motor_statePattern_t;

typedef struct app_nm_wash_rinse_motorStirWaterLevel
{
    uint8_t stirWaterLevelLiter;
} app_nm_wash_rinse_motorStirWaterLevel_t;

typedef struct app_nm_wash_rinse_motorStatePattern
{
    app_nm_wash_motor_motionPattern_t fillStirPattern;
    app_nm_wash_motor_motionPattern_t stirPattern;
    app_nm_wash_motor_motionPattern_t untanglePattern;
} app_nm_wash_rinse_motorStatePattern_t;

typedef struct app_nm_wash_rinse_motor_spinTable
{
    app_motor_spinProfile_t table[APP_NORMAL_MODE_WASH_MAX_WATER_LEVEL];
} app_nm_wash_rinse_motor_spinTable_t;

typedef struct app_nm_wash_spin_motor_spinTable
{
    app_motor_spinProfile_t table[APP_NORMAL_MODE_WASH_MAX_WATER_LEVEL];
} app_nm_wash_spin_motor_spinTable_t;

typedef struct app_nm_wash_spin_motorStatePattern
{
    app_nm_wash_motor_motionPattern_t antiWrinklePattern;
    app_nm_wash_motor_motionPattern_t stirPattern;
} app_nm_wash_spin_motorStatePattern_t;

void app_nm_wash_motor_process(uint32_t period);

app_nm_wash_steam_motor_stirState_t app_nm_wash_motor_steamData_get(void);

// motor table
uint32_t app_nm_wash_soak_motorStateTable_get(app_nm_wash_userInterface_soakTimeSelection_t soakSelection, app_nm_wash_motor_soak_state_t state);
const app_nm_wash_soak_motorStatePattern_t *app_nm_wash_motor_soak_motorMotionPattern_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_motor_soak_state_t state, app_nm_wash_userInterface_waterLevelSelection_t waterLevel);

uint8_t app_nm_wash_pulsatorVsTubState_get(void);
const app_nm_wash_motor_wash_statePattern_t *app_nm_wash_wash_motorMotionPattern_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel);

uint32_t app_nm_wash_wash_motorMHStateMotion_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_steamMH_motor_stirState_t state);
uint32_t app_nm_wash_wash_motorMDStateMotion_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_steamMD_motor_stirState_t state);
uint32_t app_nm_wash_wash_motorHeatingStateMotion_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_steamHeating_motor_stirState_t state);

uint32_t app_nm_wash_wash_motorStirStateMotion_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_motor_stirState_t state);

const app_nm_wash_steam_motor_statePattern_t *app_nm_wash_steam_motorMotionPattern_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel);
uint32_t app_nm_wash_steam_motorStirStateMotion_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_steam_motor_stirState_t state);
uint32_t app_nm_wash_steam_preSpinTimeMSec_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel);

const app_nm_wash_rinse_motorStirWaterLevel_t *app_nm_wash_rinse_motorStirWaterLevel_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel);
const app_nm_wash_rinse_motorStatePattern_t *app_nm_wash_rinse_motorMotionPattern_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel);

uint32_t app_nm_wash_rinse_preSpinTimeMSec_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel);
const app_motor_spinProfileSectionData_t *app_nm_wash_rinse_motor_spinProfileSectionData_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_motor_spinProfileSection_t spinProfileSection);

uint32_t app_nm_wash_spin_preSpinTimeMSec_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel);
const app_motor_spinProfileSectionData_t *app_nm_wash_spin_motor_spinProfileSectionData_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_motor_spinProfileSection_t spinProfileSection);
const app_nm_wash_spin_motorStatePattern_t *app_nm_wash_spin_motorMotionPattern_get(app_nm_wash_userInterface_waterLevelSelection_t waterLevel);

#endif
