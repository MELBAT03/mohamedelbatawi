#include "def.h"
#include "app_nm_wash_user_interface.h"
#include "app_nm_wash.h"
#include "app_nm_wash_motor.h"

#include "app_nm_wash_motor_table.h"

static const app_nm_wash_motor_stirStateTime_t * const washStirStatePtr[] = {
    course_group_1_stirTimeTable,
    course_group_3_stirTimeTable,
    course_group_3_stirTimeTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    course_eco_stirTimeTable,
#else
    course_group_3_stirTimeTable,
#endif
    course_group_1_stirTimeTable,
    course_group_2_stirTimeTable,
    course_group_3_stirTimeTable,
    course_group_3_stirTimeTable,
    course_group_1_stirTimeTable,
    course_group_1_stirTimeTable,
    course_bedding_stirTimeTable,
    course_group_1_stirTimeTable};

static const app_nm_wash_soak_motorStatePattern_t *const motor_soak_patternTable[] = {
    course_group_1_soakPatternTable,
    course_group_3_soakPatternTable,
    course_group_3_soakPatternTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    course_eco_soakPatternTable,
#else
    NULL,
#endif
    course_group_1_soakPatternTable,
    course_group_2_soakPatternTable,
    course_group_3_soakPatternTable,
    course_group_3_soakPatternTable,
    course_group_1_soakPatternTable,
    course_group_1_soakPatternTable,
    NULL,
    course_group_1_soakPatternTable};

static const app_nm_wash_motor_wash_statePattern_t *const motor_wash_patternTable[] = {
    course_group_1_washMotorPattern,
    course_group_3_washMotorPattern,
    course_group_3_washMotorPattern,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    course_eco_washMotorPattern,
#else
    course_group_3_washMotorPattern,
#endif
    course_group_1_washMotorPattern,
    course_group_2_washMotorPattern,
    course_group_3_washMotorPattern,
    course_group_3_washMotorPattern,
    course_group_1_washMotorPattern,
    course_group_1_washMotorPattern,
    course_bedding_washMotorPattern,
    course_group_1_washMotorPattern};

static const app_nm_wash_steam_motor_statePattern_t *const motor_steam_patternTable[] = {
    course_group_1_steamPatternTable,
    course_group_1_steamPatternTable,
    course_group_1_steamPatternTable,
    course_group_1_steamPatternTable,
    course_group_1_steamPatternTable,
    course_group_1_steamPatternTable,
    course_group_1_steamPatternTable,
    course_group_1_steamPatternTable,
    course_group_1_steamPatternTable,
    course_group_1_steamPatternTable,
    course_group_1_steamPatternTable,
    course_group_1_steamPatternTable};

static const app_nm_wash_rinse_motorStirWaterLevel_t *const motor_rinse_fillStirPtr[] = {
    rinse_fillStirWaterLevelTable,
    rinse_fillStirWaterLevelTable,
    rinse_fillStirWaterLevelTable,
    rinse_fillStirWaterLevelTable,
    rinse_fillStirWaterLevelTable,
    rinse_fillStirWaterLevelTable,
    rinse_fillStirWaterLevelTable,
    rinse_fillStirWaterLevelTable,
    rinse_fillStirWaterLevelTable,
    rinse_fillStirWaterLevelTable,
    rinse_fillStirWaterLevelTable,
    rinse_fillStirWaterLevelTable};

static const app_nm_wash_rinse_motorStatePattern_t *const motor_rinse_patternTable[] = {
    course_group_1_rinseMotorPatternTable,
    course_group_3_rinseMotorPatternTable,
    course_group_3_rinseMotorPatternTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    course_eco_rinseMotorPatternTable,
#else
    course_group_3_rinseMotorPatternTable,
#endif
    course_group_1_rinseMotorPatternTable,
    course_group_2_rinseMotorPatternTable,
    course_group_3_rinseMotorPatternTable,
    course_group_3_rinseMotorPatternTable,
    course_group_1_rinseMotorPatternTable,
    course_group_1_rinseMotorPatternTable,
    course_bedding_rinseMotorPatternTable,
    course_group_1_rinseMotorPatternTable};

static const app_nm_wash_rinse_motor_spinTable_t *const motor_rinse_SpinCurvePtr[] = {
    &course_group_1_rinseSpinPatternTable,
    &course_group_2_rinseSpinPatternTable,
    &course_group_2_rinseSpinPatternTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    &course_group_1_rinseSpinPatternTable,
#else
    &course_group_2_rinseSpinPatternTable,
#endif
    &course_group_1_rinseSpinPatternTable,
    &course_group_1_rinseSpinPatternTable,
    &course_group_2_rinseSpinPatternTable,
    &course_group_1_rinseSpinPatternTable,
    &course_group_1_rinseSpinPatternTable,
    &course_group_1_rinseSpinPatternTable,
    &course_group_1_rinseSpinPatternTable,
    &course_group_1_rinseSpinPatternTable};

static const app_nm_wash_spin_motor_spinTable_t *const motor_spin_SpinCurvePtr[] = {
    &course_group_1_spinPatternTable,
    &course_group_2_spinPatternTable,
    &course_group_2_spinPatternTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    &course_group_1_spinPatternTable,
#else
    &course_group_2_spinPatternTable,
#endif
    &course_group_1_spinPatternTable,
    &course_group_1_spinPatternTable,
    &course_group_2_spinPatternTable,
    &course_group_1_spinPatternTable,
    &course_group_1_spinPatternTable,
    &course_group_1_spinPatternTable,
    &course_group_1_spinPatternTable,
    &course_group_1_spinPatternTable};

// -------------------------------------------------------------------
// NOTE: soak
uint32_t app_nm_wash_soak_motorStateTable_get(app_nm_wash_userInterface_soakTimeSelection_t soakSelection, app_nm_wash_motor_soak_state_t state)
{
    uint32_t ret = 0;

    if (((soakSelection >= APP_NORMAL_MODE_UI_SOAK_TIME_1_HOUR) && (soakSelection <= APP_NORMAL_MODE_UI_SOAK_TIME_4_HOUR)) && (state <= APP_NORMAL_MODE_WASH_MOTOR_SOAK_MOTION_2))
    {
        ret = wash_motor_soak_stateTable[soakSelection - 1].soakState[state];
    }

    return ret;
}

const app_nm_wash_soak_motorStatePattern_t *app_nm_wash_motor_soak_motorMotionPattern_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_motor_soak_state_t state, app_nm_wash_userInterface_waterLevelSelection_t waterLevel)
{
    const app_nm_wash_soak_motorStatePattern_t *ret = NULL;

    if ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (state == APP_NORMAL_MODE_WASH_MOTOR_SOAK_MOTION_1 || state == APP_NORMAL_MODE_WASH_MOTOR_SOAK_MOTION_2))
    {
        ret = &(motor_soak_patternTable[course - 1][waterLevel - 1]);
    }

    return ret;
}

// -------------------------------------------------------------------
// NOTE: wash
uint8_t app_nm_wash_pulsatorVsTubState_get(void)
{
    uint8_t ret = 0;

    if ((app_userInterface_data_get()->normalMode.wash.waterLevelSelection >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1) && (app_userInterface_data_get()->normalMode.wash.waterLevelSelection <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT))
    {
        ret = pulsatorVsTubDuringWashSetup[app_userInterface_data_get()->normalMode.wash.waterLevelSelection - 1];
    }

    return ret;
}

const app_nm_wash_motor_wash_statePattern_t *app_nm_wash_wash_motorMotionPattern_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel)
{
    const app_nm_wash_motor_wash_statePattern_t *ret = NULL;

    if ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM))
    {
        ret = &(motor_wash_patternTable[course - 1][waterLevel - 1]);
    }

    return ret;
}

uint32_t app_nm_wash_wash_motorMHStateMotion_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_steamMH_motor_stirState_t state)
{
    uint32_t ret = 0;

    if ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (state <= STEAM_TECH_MH_STOP))
    {
        ret = app_nm_wash_motor_steamTech_MHInterStateTable[waterLevel - 1].stirState[state];
    }

    return ret;
}

uint32_t app_nm_wash_wash_motorMDStateMotion_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_steamMD_motor_stirState_t state)
{
    uint32_t ret = 0;

    if ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (state <= STEAM_TECH_MD_STOP))
    {
        ret = app_nm_wash_motor_steamTech_MDInterStateTable[waterLevel - 1].stirState[state];
    }

    return ret;
}

uint32_t app_nm_wash_wash_motorHeatingStateMotion_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_steamHeating_motor_stirState_t state)
{
    uint32_t ret = 0;

    if ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (state <= STEAM_TECH_HEATING_STOP))
    {
        ret = app_nm_wash_motor_steamTech_HeatingInterStateTable[waterLevel - 1].stirState[state];
    }

    return ret;
}

uint32_t app_nm_wash_wash_motorStirStateMotion_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_motor_stirState_t state)
{
    uint32_t ret = 0;
    static uint8_t soak_stub_flag = 0;

    if ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (state <= WASH_STIR_MU))
    {
        if ((app_userInterface_data_get()->normalMode.wash.soakSelection != APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR) || (soak_stub_flag == 1))
        {
            soak_stub_flag = 1;

            if (app_userInterface_data_get()->normalMode.wash.washSelection == APP_NORMAL_MODE_UI_WASH_TIME_3_MIN)
            {
                ret = course_soak_stubStirTimeTable[0].stirState[state];
            }
            else if (app_userInterface_data_get()->normalMode.wash.washSelection == APP_NORMAL_MODE_UI_WASH_TIME_9_MIN)
            {
                ret = course_soak_stubStirTimeTable[1].stirState[state];
            }
            else if (app_userInterface_data_get()->normalMode.wash.washSelection == APP_NORMAL_MODE_UI_WASH_TIME_12_MIN)
            {
                ret = course_soak_stubStirTimeTable[2].stirState[state];
            }
            else if (app_userInterface_data_get()->normalMode.wash.washSelection == APP_NORMAL_MODE_UI_WASH_TIME_18_MIN)
            {
                ret = course_soak_stubStirTimeTable[3].stirState[state];
            }
            else
            {
                // do nothing
            }
        }
        else
        {
            ret = washStirStatePtr[course - 1][waterLevel - 1].stirState[state];
        }
    }

    return ret;
}

// -------------------------------------------------------------------
// NOTE: steam
const app_nm_wash_steam_motor_statePattern_t *app_nm_wash_steam_motorMotionPattern_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel)
{
    const app_nm_wash_steam_motor_statePattern_t *ret = NULL;

    if ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM))
    {
        ret = &(motor_steam_patternTable[course - 1][waterLevel - 1]);
    }

    return ret;
}

uint32_t app_nm_wash_steam_motorStirStateMotion_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_steam_motor_stirState_t state)
{
    uint32_t ret = 0;

    if ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (state <= STEAM_STIR_M3))
    {
        ret = app_nm_wash_steam_motor_stirInterStateTable[waterLevel - 1].stirStateTimeMSec[state];
    }

    return ret;
}

uint32_t app_nm_wash_steam_preSpinTimeMSec_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel)
{
    uint32_t preSpinTimeSec = 0;

    if ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM))
    {
        for (uint8_t i = 0; i <= STEAM_MOTOR_PRE_SPIN_LAST_SECTOR_NUM; i++)
        {
            preSpinTimeSec += motor_rinse_SpinCurvePtr[course - 1]->table[waterLevel - 1].profile[i].sectionTimeMSec;
        }
    }

    return preSpinTimeSec;
}

// -------------------------------------------------------------------
// NOTE: rinse
const app_nm_wash_rinse_motorStirWaterLevel_t *app_nm_wash_rinse_motorStirWaterLevel_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel)
{
    const app_nm_wash_rinse_motorStirWaterLevel_t *ret = NULL;

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT))
    {
        ret = &(motor_rinse_fillStirPtr[course - 1][waterLevel - 1]);
    }

    return ret;
}

const app_nm_wash_rinse_motorStatePattern_t *app_nm_wash_rinse_motorMotionPattern_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel)
{
    const app_nm_wash_rinse_motorStatePattern_t *ret = NULL;

    if ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM))
    {
        ret = &(motor_rinse_patternTable[course - 1][waterLevel - 1]);
    }

    return ret;
}

uint32_t app_nm_wash_rinse_preSpinTimeMSec_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel)
{
    uint32_t preSpinTimeSec = 0;

    if ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM))
    {
        for (uint8_t i = 0; i <= RINSE_MOTOR_PRE_SPIN_LAST_SECTOR_NUM; i++)
        {
            preSpinTimeSec += motor_rinse_SpinCurvePtr[course - 1]->table[waterLevel - 1].profile[i].sectionTimeMSec;
        }
    }

    return preSpinTimeSec;
}

const app_motor_spinProfileSectionData_t *app_nm_wash_rinse_motor_spinProfileSectionData_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_motor_spinProfileSection_t spinProfileSection)
{
    const app_motor_spinProfileSectionData_t *ret = NULL;

    if ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (spinProfileSection <= SPIN_PROFILE_SECTION_6))
    {
        ret = &(motor_rinse_SpinCurvePtr[course - 1]->table[waterLevel - 1].profile[spinProfileSection]);
    }

    return ret;
}

// -------------------------------------------------------------------
// NOTE: spin
uint32_t app_nm_wash_spin_preSpinTimeMSec_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel)
{
    uint32_t preSpinTimeSec = 0;

    if ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM))
    {
        for (uint8_t i = 0; i <= SPIN_MOTOR_PRE_SPIN_LAST_SECTOR_NUM; i++)
        {
            preSpinTimeSec += motor_spin_SpinCurvePtr[course - 1]->table[waterLevel - 1].profile[i].sectionTimeMSec;
        }
    }

    return preSpinTimeSec;
}

const app_motor_spinProfileSectionData_t *app_nm_wash_spin_motor_spinProfileSectionData_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_motor_spinProfileSection_t spinProfileSection)
{
    const app_motor_spinProfileSectionData_t *ret = NULL;

    if ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (spinProfileSection <= SPIN_PROFILE_SECTION_6))
    {
        ret = &(motor_spin_SpinCurvePtr[course - 1]->table[waterLevel - 1].profile[spinProfileSection]);
    }

    return ret;
}

const app_nm_wash_spin_motorStatePattern_t *app_nm_wash_spin_motorMotionPattern_get(app_nm_wash_userInterface_waterLevelSelection_t waterLevel)
{
    const app_nm_wash_spin_motorStatePattern_t *ret = NULL;

    if (waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT)
    {
        ret = &(course_group_1_antiwrinklePatternTable[waterLevel - 1]);
    }

    return ret;
}
