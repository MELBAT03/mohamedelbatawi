#ifndef APP_NM_DRY_MOTOR_H
#define APP_NM_DRY_MOTOR_H

#include "def.h"
#include "app_user_interface.h"
#include "app_nm_dry.h"
#include "app_motor.h"

typedef enum app_nm_dry_heatingState
{
    APP_NORMAL_MODE_DRY_OPERATION_HEATING_MD,
    APP_NORMAL_MODE_DRY_OPERATION_HEATING_M1,
    APP_NORMAL_MODE_DRY_OPERATION_HEATING_M2,
    APP_NORMAL_MODE_DRY_OPERATION_HEATING_M3,
    APP_NORMAL_MODE_DRY_OPERATION_HEATING_STATE_NUM
} app_nm_dry_heatingState_t;

typedef enum app_nm_dry_condense1State
{
    APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_MD,
    APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_M1,
    APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_M2,
    APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_M3,
    APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_STATE_NUM
} app_nm_dry_condense1State_t;

typedef enum app_nm_dry_condense2State
{
    APP_NORMAL_MODE_DRY_OPERATION_CONDENSE2_M4,
    APP_NORMAL_MODE_DRY_OPERATION_CONDENSE2_M5,
    APP_NORMAL_MODE_DRY_OPERATION_CONDENSE2_M6,
    APP_NORMAL_MODE_DRY_OPERATION_CONDENSE2_BREAK,
    APP_NORMAL_MODE_DRY_OPERATION_CONDENSE2_STATE_NUM
} app_nm_dry_condense2State_t;

typedef enum app_nm_dry_extraTimeState
{
    APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME_M4,
    APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME_M5,
    APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME_M6,
    APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME_BREAK,
    APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME_STATE_NUM
} app_nm_dry_extraTimeState_t;

typedef enum app_nm_dry_coolingWaterState
{
    APP_NORMAL_MODE_DRY_OPERATION_COOLING_WATER_MD,
    APP_NORMAL_MODE_DRY_OPERATION_COOLING_WATER_STATE_NUM
} app_nm_dry_coolingWaterState_t;

typedef enum app_nm_dry_coolingAirState
{
    APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR_M1,
    APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR_M2,
    APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR_M3,
    APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR_STATE_NUM
} app_nm_dry_coolingAirState_t;

typedef struct app_nm_dry_motor_operationData
{
    app_nm_dry_heatingState_t heatingState;
    app_nm_dry_condense1State_t condense1State;
    app_nm_dry_condense2State_t condense2State;
    app_nm_dry_extraTimeState_t extraTimeState;
    app_nm_dry_coolingWaterState_t coolingWaterState;
    app_nm_dry_coolingAirState_t coolingAirState;
} app_nm_dry_motor_operationData_t;

typedef struct app_nm_dry_motor_data
{
    app_nm_dry_motor_operationData_t operation;
} app_nm_dry_motor_data_t;

typedef struct app_nm_dry_motor_spinTable
{
    app_motor_spinProfile_t table[APP_NORMAL_MODE_DRY_MAX_WATER_LEVEL];
} app_nm_dry_motor_spinTable_t;

typedef app_motor_motionPattern_t app_nm_dry_motor_motionPattern_t;

typedef struct app_nm_dry_motor_untangleStateTimings
{
    uint32_t untangleStateTimeMSec;
} app_nm_dry_motor_untangleStateTimings_t;

typedef struct app_nm_dry_motor_heatingSubStateTimings
{
    uint32_t heatingStateTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_HEATING_STATE_NUM];
} app_nm_dry_motor_heatingSubStateTimings_t;

typedef struct app_nm_dry_motor_condense1SubStateTimings
{
    uint32_t condense1StateTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_STATE_NUM];
} app_nm_dry_motor_condense1SubStateTimings_t;

typedef struct app_nm_dry_motor_condense2SubStateTimings
{
    uint32_t condense2PhaseTimeMSec;
    uint32_t condense2StateTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_CONDENSE2_STATE_NUM];
} app_nm_dry_motor_condense2SubStateTimings_t;

typedef struct app_nm_dry_motor_extraTimeSubStateTimings
{
    uint32_t extraTimePhaseTimeMSec;
    uint32_t extraTimeStateTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME_STATE_NUM];
} app_nm_dry_motor_extraTimeSubStateTimings_t;

typedef struct app_nm_dry_motor_coolingWaterSubStateTimings
{
    uint32_t coolingWaterMDStateTotalTimeMSec;
} app_nm_dry_motor_coolingWaterSubStateTimings_t;

typedef struct app_nm_dry_motor_coolingAirSubStateTimings
{
    uint32_t coolingAirStateTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR_STATE_NUM];
} app_nm_dry_motor_coolingAirSubStateTimings_t;

typedef struct app_nm_dry_motor_stateTable
{
    app_nm_dry_motor_untangleStateTimings_t untangleState;
    app_nm_dry_motor_heatingSubStateTimings_t heatingState;
    app_nm_dry_motor_condense1SubStateTimings_t condense1State;
    app_nm_dry_motor_condense2SubStateTimings_t condense2State;
    app_nm_dry_motor_extraTimeSubStateTimings_t extraTimeState;
    app_nm_dry_motor_coolingWaterSubStateTimings_t coolingWaterState;
    app_nm_dry_motor_coolingAirSubStateTimings_t coolingAirState;
} app_nm_dry_motor_stateTable_t;

typedef struct app_nm_dry_motor_untangleStatePattern
{
    app_nm_dry_motor_motionPattern_t pattern;
} app_nm_dry_motor_untangleStatePattern_t;

typedef struct app_nm_dry_motor_heatingStatePattern
{
    app_nm_dry_motor_motionPattern_t pattern[APP_NORMAL_MODE_DRY_OPERATION_HEATING_STATE_NUM];
} app_nm_dry_motor_heatingStatePattern_t;

typedef struct app_nm_dry_motor_condense1StatePattern
{
    app_nm_dry_motor_motionPattern_t pattern[APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_STATE_NUM];
} app_nm_dry_motor_condense1StatePattern_t;

typedef struct app_nm_dry_motor_condense2StatePattern
{
    app_nm_dry_motor_motionPattern_t pattern[APP_NORMAL_MODE_DRY_OPERATION_CONDENSE2_STATE_NUM];
} app_nm_dry_motor_condense2StatePattern_t;

typedef struct app_nm_dry_motor_extraTimeStatePattern
{
    app_nm_dry_motor_motionPattern_t pattern[APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME_STATE_NUM];
} app_nm_dry_motor_extraTimeStatePattern_t;

typedef struct app_nm_dry_motor_coolingWaterStatePattern
{
    app_nm_dry_motor_motionPattern_t pattern;
} app_nm_dry_motor_coolingWaterStatePattern_t;

typedef struct app_nm_dry_motor_coolingAirStatePattern
{
    app_nm_dry_motor_motionPattern_t pattern[APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR_STATE_NUM];
} app_nm_dry_motor_coolingAirStatePattern_t;

typedef struct app_nm_dry_motor_statePattern
{
    app_nm_dry_motor_untangleStatePattern_t untangleState;
    app_nm_dry_motor_heatingStatePattern_t heatingState;
    app_nm_dry_motor_condense1StatePattern_t condense1State;
    app_nm_dry_motor_condense2StatePattern_t condense2State;
    app_nm_dry_motor_extraTimeStatePattern_t extraTimeState;
    app_nm_dry_motor_coolingWaterStatePattern_t coolingWaterState;
    app_nm_dry_motor_coolingAirStatePattern_t coolingAirState;
} app_nm_dry_motor_statePattern_t;

uint32_t app_nm_dry_preSpinStateTotalTimeMSec_get(app_nm_userInterface_courseSelection_t course, app_nm_dry_userInterface_waterLevelSelection_t waterLevel);
const app_motor_spinProfileSectionData_t *app_nm_dry_motor_preSetupSpinProfileSectionData_get(app_motor_spinProfileSection_t spinProfileSection);
const app_motor_spinProfileSectionData_t *app_nm_dry_motor_spinProfileSectionData_get(app_nm_userInterface_courseSelection_t course, app_nm_dry_userInterface_waterLevelSelection_t waterLevel, app_motor_spinProfileSection_t spinProfileSection);

uint32_t app_nm_dry_motor_heatingStateIntervalMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel, const app_nm_dry_heatingState_t state);
uint32_t app_nm_dry_motor_heatingStateTotalTimeMSec_get(const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel);

uint32_t app_nm_dry_motor_condense1StateIntervalMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel, const app_nm_dry_condense1State_t state);
uint32_t app_nm_dry_motor_condense1StateTotalTimeMSec_get(const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel);

uint32_t app_nm_dry_motor_condense2StateIntervalMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel, const app_nm_dry_condense2State_t state);
uint32_t app_nm_dry_motor_condense2PhaseTimeMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel);

uint32_t app_nm_dry_motor_extraTimeStateIntervalMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel, const app_nm_dry_extraTimeState_t state);
uint32_t app_nm_dry_motor_extraTimePhaseTimeMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel);

uint32_t app_nm_dry_motor_coolingAirStateIntervalMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel, const app_nm_dry_coolingAirState_t state);
uint32_t app_nm_dry_motor_coolingAirStateTotalTimeMSec_get(const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel);

const app_nm_dry_motor_statePattern_t *app_nm_dry_motor_preSetupMotionPattern_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection);
const app_nm_dry_motor_statePattern_t *app_nm_dry_motor_motionPattern_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel);

void app_nm_dry_motor_process(uint32_t period);

app_nm_dry_motor_data_t *app_nm_dry_motor_data_get(void);

app_motor_spinProfileSection_t app_nm_dry_motor_spinProfileSection_get(void);
uint32_t app_nm_dry_motor_maxContinuousSpinCounter_get(void);

#endif
