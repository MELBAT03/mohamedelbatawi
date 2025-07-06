#include "def.h"
#include "app_nm_dry_motor.h"
#include "app_user_interface.h"

#include "app_nm_dry_motor_table.h"

static const app_nm_dry_motor_stateTable_t *courseRegular_dry_motor_operationStateTimeTable[] =
    {
        app_dry_motor_normal_30MinManualStateTable,
        app_dry_motor_normal_60MinManualStateTable,
        app_dry_motor_normal_90MinManualStateTable,
        app_dry_motor_normal_autoStateTable};

static const app_nm_dry_motor_stateTable_t *courseQuick_dry_motor_operationStateTimeTable[] =
    {
        app_dry_motor_normal_30MinManualStateTable,
        app_dry_motor_normal_60MinManualStateTable,
        app_dry_motor_normal_90MinManualStateTable,
        app_dry_motor_normal_autoStateTable};

static const app_nm_dry_motor_stateTable_t *courseHygiene_dry_motor_operationStateTimeTable[] =
    {
        app_dry_motor_normal_30MinManualStateTable,
        app_dry_motor_normal_60MinManualStateTable,
        app_dry_motor_normal_90MinManualStateTable,
        app_dry_motor_normal_autoStateTable};

static const app_nm_dry_motor_stateTable_t *courseHeavy_dry_motor_operationStateTimeTable[] =
    {
        app_dry_motor_normal_30MinManualStateTable,
        app_dry_motor_normal_60MinManualStateTable,
        app_dry_motor_normal_90MinManualStateTable,
        app_dry_motor_normal_autoStateTable};

static const app_nm_dry_motor_stateTable_t *courseBabyCare_dry_motor_operationStateTimeTable[] =
    {
        app_dry_motor_softer_30MinManualStateTable,
        app_dry_motor_softer_60MinManualStateTable,
        app_dry_motor_softer_90MinManualStateTable,
        app_dry_motor_softer_autoStateTable};

static const app_nm_dry_motor_stateTable_t *courseLowTempDry_dry_motor_operationStateTimeTable[] =
    {
        app_dry_motor_softer_30MinManualStateTable,
        app_dry_motor_softer_60MinManualStateTable,
        app_dry_motor_softer_90MinManualStateTable,
        app_dry_motor_softer_autoStateTable};

static const app_nm_dry_motor_stateTable_t *courseEasyIron_dry_motor_operationStateTimeTable[] =
    {
        app_dry_motor_softer_30MinManualStateTable,
        app_dry_motor_softer_60MinManualStateTable,
        app_dry_motor_softer_90MinManualStateTable,
        app_dry_motor_softer_autoStateTable};

static const app_nm_dry_motor_statePattern_t *courseRegular_dry_motor_operationStateMotorPatternTable[] =
    {
        app_dry_motor_normal_30MinPatternTable,
        app_dry_motor_normal_60MinPatternTable,
        app_dry_motor_normal_90MinPatternTable,
        app_dry_motor_normal_autoMinPatternTable};

static const app_nm_dry_motor_statePattern_t *courseQuick_dry_motor_operationStateMotorPatternTable[] =
    {
        app_dry_motor_normal_30MinPatternTable,
        app_dry_motor_normal_60MinPatternTable,
        app_dry_motor_normal_90MinPatternTable,
        app_dry_motor_normal_autoMinPatternTable};

static const app_nm_dry_motor_statePattern_t *courseHygiene_dry_motor_operationStateMotorPatternTable[] =
    {
        app_dry_motor_normal_30MinPatternTable,
        app_dry_motor_normal_60MinPatternTable,
        app_dry_motor_normal_90MinPatternTable,
        app_dry_motor_normal_autoMinPatternTable};

static const app_nm_dry_motor_statePattern_t *courseHeavy_dry_motor_operationStateMotorPatternTable[] =
    {
        app_dry_motor_normal_30MinPatternTable,
        app_dry_motor_normal_60MinPatternTable,
        app_dry_motor_normal_90MinPatternTable,
        app_dry_motor_normal_autoMinPatternTable};

static const app_nm_dry_motor_statePattern_t *courseBabyCare_dry_motor_operationStateMotorPatternTable[] =
    {
        app_dry_motor_softer_30MinPatternTable,
        app_dry_motor_softer_60MinPatternTable,
        app_dry_motor_softer_90MinPatternTable,
        app_dry_motor_softer_autoMinPatternTable};

static const app_nm_dry_motor_statePattern_t *courseLowTempDry_dry_motor_operationStateMotorPatternTable[] =
    {
        app_dry_motor_softer_30MinPatternTable,
        app_dry_motor_softer_60MinPatternTable,
        app_dry_motor_softer_90MinPatternTable,
        app_dry_motor_softer_autoMinPatternTable};

static const app_nm_dry_motor_statePattern_t *courseEasyIron_dry_motor_operationStateMotorPatternTable[] =
    {
        app_dry_motor_softer_30MinPatternTable,
        app_dry_motor_softer_60MinPatternTable,
        app_dry_motor_softer_90MinPatternTable,
        app_dry_motor_softer_autoMinPatternTable};

static const app_nm_dry_motor_stateTable_t **dry_motor_timeTablePtr[] = {
    courseRegular_dry_motor_operationStateTimeTable,
    courseRegular_dry_motor_operationStateTimeTable,   // NOTE: doesn't have a dry
    courseRegular_dry_motor_operationStateTimeTable,   // NOTE: doesn't have a dry
    courseQuick_dry_motor_operationStateTimeTable,
    courseHygiene_dry_motor_operationStateTimeTable,
    courseHeavy_dry_motor_operationStateTimeTable,
    courseRegular_dry_motor_operationStateTimeTable,   // NOTE: doesn't have a dry
    courseBabyCare_dry_motor_operationStateTimeTable,
    courseRegular_dry_motor_operationStateTimeTable,   // NOTE: doesn't have a dry
    courseLowTempDry_dry_motor_operationStateTimeTable,
    courseEasyIron_dry_motor_operationStateTimeTable,
    courseRegular_dry_motor_operationStateTimeTable};   // NOTE: doesn't have a dry

static const app_nm_dry_motor_statePattern_t **dry_motor_operationStateMotorPatternPtr[] = {
    courseRegular_dry_motor_operationStateMotorPatternTable,
    courseRegular_dry_motor_operationStateMotorPatternTable,   // NOTE: doesn't have a dry
    courseRegular_dry_motor_operationStateMotorPatternTable,   // NOTE: doesn't have a dry
    courseQuick_dry_motor_operationStateMotorPatternTable,
    courseHygiene_dry_motor_operationStateMotorPatternTable,
    courseHeavy_dry_motor_operationStateMotorPatternTable,
    courseRegular_dry_motor_operationStateMotorPatternTable,   // NOTE: doesn't have a dry
    courseBabyCare_dry_motor_operationStateMotorPatternTable,
    courseRegular_dry_motor_operationStateMotorPatternTable,   // NOTE: doesn't have a dry
    courseLowTempDry_dry_motor_operationStateMotorPatternTable,
    courseEasyIron_dry_motor_operationStateMotorPatternTable,
    courseRegular_dry_motor_operationStateMotorPatternTable};   // NOTE: doesn't have a dry

static const app_nm_dry_motor_spinTable_t *ptrSpinCurve[] = {
    &dry_motor_courseX_spinTable,
    &dry_motor_courseX_spinTable,
    &dry_motor_courseX_spinTable,
    &dry_motor_courseX_spinTable,
    &dry_motor_courseX_spinTable,
    &dry_motor_courseX_spinTable,
    &dry_motor_courseX_spinTable,
    &dry_motor_courseX_spinTable,
    &dry_motor_courseX_spinTable,
    &dry_motor_courseX_spinTable,
    &dry_motor_courseX_spinTable,
    &dry_motor_courseX_spinTable};

uint32_t app_nm_dry_preSpinStateTotalTimeMSec_get(app_nm_userInterface_courseSelection_t course, app_nm_dry_userInterface_waterLevelSelection_t waterLevel)
{
    uint32_t preSpinTimeSec = 0;

    if ((waterLevel >= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM))
    {
        for (uint8_t i = 0; i <= MOTOR_PRE_SPIN_LAST_SECTOR_NUM; i++)
        {
            preSpinTimeSec += ptrSpinCurve[course - 1]->table[waterLevel - 1].profile[i].sectionTimeMSec;
        }
    }

    return preSpinTimeSec;
}

const app_motor_spinProfileSectionData_t *app_nm_dry_motor_preSetupSpinProfileSectionData_get(app_motor_spinProfileSection_t spinProfileSection)
{
    const app_motor_spinProfileSectionData_t *ret = NULL;

    if ((spinProfileSection >= SPIN_PROFILE_SECTION_0) && (spinProfileSection <= SPIN_PROFILE_SECTION_6))
    {
        ret = &dry_motor_preSetupTable.profile[spinProfileSection];
    }

    return ret;
}

const app_motor_spinProfileSectionData_t *app_nm_dry_motor_spinProfileSectionData_get(app_nm_userInterface_courseSelection_t course, app_nm_dry_userInterface_waterLevelSelection_t waterLevel, app_motor_spinProfileSection_t spinProfileSection)
{
    const app_motor_spinProfileSectionData_t *ret = NULL;

    if ((waterLevel >= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (spinProfileSection >= SPIN_PROFILE_SECTION_0 && spinProfileSection <= SPIN_PROFILE_SECTION_6))
    {
        ret = &(ptrSpinCurve[course - 1]->table[waterLevel - 1].profile[spinProfileSection]);
    }

    return ret;
}

uint32_t app_nm_dry_motor_heatingStateIntervalMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel, const app_nm_dry_heatingState_t state)
{
    uint32_t ret = 0;

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (waterLevel >= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6) && (selection >= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN && selection <= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO) && (state >= APP_NORMAL_MODE_DRY_OPERATION_HEATING_MD && state <= APP_NORMAL_MODE_DRY_OPERATION_HEATING_M3))
    {
        ret = (dry_motor_timeTablePtr[course - 1][selection - 1] + (waterLevel - 1))->heatingState.heatingStateTimeMSec[state];
    }

    return ret;
}

uint32_t app_nm_dry_motor_condense1StateIntervalMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel, const app_nm_dry_condense1State_t state)
{
    uint32_t ret = 0;

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (waterLevel >= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6) && (selection >= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN && selection <= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO) && (state >= APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_MD && state <= APP_NORMAL_MODE_DRY_OPERATION_CONDENSE1_M3))
    {
        ret = (dry_motor_timeTablePtr[course - 1][selection - 1] + (waterLevel - 1))->condense1State.condense1StateTimeMSec[state];
    }
    return ret;
}

uint32_t app_nm_dry_motor_condense2StateIntervalMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel, const app_nm_dry_condense2State_t state)
{
    uint32_t ret = 0;

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (waterLevel >= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6) && (selection >= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN && selection <= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO) && (state >= APP_NORMAL_MODE_DRY_OPERATION_CONDENSE2_M4 && state <= APP_NORMAL_MODE_DRY_OPERATION_CONDENSE2_BREAK))
    {
        ret = (dry_motor_timeTablePtr[course - 1][selection - 1] + (waterLevel - 1))->condense2State.condense2StateTimeMSec[state];
    }
    return ret;
}

uint32_t app_nm_dry_motor_condense2PhaseTimeMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel)
{
    uint32_t ret = 0;

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (waterLevel >= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6) && (selection >= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN && selection <= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO))
    {
        ret = (dry_motor_timeTablePtr[course - 1][selection - 1] + (waterLevel - 1))->condense2State.condense2PhaseTimeMSec;
    }
    return ret;
}

uint32_t app_nm_dry_motor_extraTimeStateIntervalMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel, const app_nm_dry_extraTimeState_t state)
{
    uint32_t ret = 0;

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (waterLevel >= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6) && (selection >= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN && selection <= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO) && (state >= APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME_M4 && state <= APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME_BREAK))
    {
        ret = (dry_motor_timeTablePtr[course - 1][selection - 1] + (waterLevel - 1))->extraTimeState.extraTimeStateTimeMSec[state];
    }
    return ret;
}

uint32_t app_nm_dry_motor_extraTimePhaseTimeMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel)
{
    uint32_t ret = 0;

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (waterLevel >= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6) && (selection >= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN && selection <= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO))
    {
        ret = (dry_motor_timeTablePtr[course - 1][selection - 1] + (waterLevel - 1))->extraTimeState.extraTimePhaseTimeMSec;
    }
    return ret;
}

uint32_t app_nm_dry_motor_coolingAirStateIntervalMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel, const app_nm_dry_coolingAirState_t state)
{
    uint32_t ret = 0;

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (waterLevel >= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6) && (selection >= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN && selection <= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO) && (state >= APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR_M1 && state <= APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR_M3))
    {
        ret = (dry_motor_timeTablePtr[course - 1][selection - 1] + (waterLevel - 1))->coolingAirState.coolingAirStateTimeMSec[state];
    }
    return ret;
}

const app_nm_dry_motor_statePattern_t *app_nm_dry_motor_preSetupMotionPattern_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection)
{
    const app_nm_dry_motor_statePattern_t *ret = NULL;

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (selection >= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN) && (selection <= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO))
    {
        ret = (dry_motor_operationStateMotorPatternPtr[course - 1][selection - 1]);
    }

    return ret;
}

const app_nm_dry_motor_statePattern_t *app_nm_dry_motor_motionPattern_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel)
{
    const app_nm_dry_motor_statePattern_t *ret = NULL;

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (waterLevel >= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6) && (selection >= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN && selection <= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO))
    {
        ret = (dry_motor_operationStateMotorPatternPtr[course - 1][selection - 1] + (waterLevel - 1));
    }

    return ret;
}
