#include "def.h"
#include "config.h"
#include "app_nm_wash.h"
#include "app_fill.h"

#include "app_nm_wash_wash_table.h"
#include "app_nm_wash_wash_detergent_table.h"
#include "app_nm_wash_soak_table.h"
#include "app_nm_wash_motor.h"

#include "app_nm_wash_user_interface.h"

#define WASH_SOIL_LEVEL_HEAVY_ADDIDTION_TIME_MSEC (30000U)

static const app_nm_wash_wash_stateTime_t *const washStatePtr[] = {
    course_group_1_stateTimeTable,
    course_group_3_stateTimeTable,
    course_group_3_stateTimeTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    course_eco_stateTimeTable,
#else
    course_group_3_stateTimeTable,
#endif
    course_group_1_stateTimeTable,
    course_group_2_stateTimeTable,
    course_group_3_stateTimeTable,
    course_group_3_stateTimeTable,
    course_group_1_stateTimeTable,
    course_group_1_stateTimeTable,
    course_bedding_stateTimeTable,
    course_group_1_stateTimeTable};

const app_nm_wash_wash_stateTime_t *app_nm_wash_wash_stateData_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_userInterface_washTimeSelection_t washSelection, app_nm_wash_userInterface_soilLevelSelection_t soilLevel)
{
    static app_nm_wash_wash_stateTime_t internalData;
    const app_nm_wash_wash_stateTime_t *ret = NULL;
    static uint8_t soak_stub_flag = 0;
    uint32_t sum = 0;

#ifdef REMAINING_TIME_ALWAYS_IN_STANDBY
    if (waterLevel == APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0)
    {
        waterLevel = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_DURATION_DEFAULT;
    }
#endif

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_MAX_WATER_LEVEL) && (washSelection > APP_NORMAL_MODE_UI_WASH_TIME_0_MIN && washSelection <= APP_NORMAL_MODE_UI_WASH_TIME_18_MIN) && (soilLevel <= APP_NORMAL_MODE_UI_SOIL_LEVEL_HEAVY))
    {
        internalData = washStatePtr[course - 1][waterLevel - 1];

        if ((app_userInterface_data_get()->normalMode.wash.soakSelection != APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR) || (soak_stub_flag == 1))
        {
            soak_stub_flag = 1;

            for (uint8_t i = 0; i < APP_NORMAL_MODE_WASH_STATES_WITH_DATA; i++)
            {
                if ((course_soak_wash_stubTimeTable[0].state[i].timeMSec != 0) && (washSelection == APP_NORMAL_MODE_UI_WASH_TIME_3_MIN))
                {
                    internalData.state[i].timeMSec = course_soak_wash_stubTimeTable[0].state[i].timeMSec;
                }
                else if ((course_soak_wash_stubTimeTable[1].state[i].timeMSec != 0) && (washSelection == APP_NORMAL_MODE_UI_WASH_TIME_9_MIN))
                {
                    internalData.state[i].timeMSec = course_soak_wash_stubTimeTable[1].state[i].timeMSec;
                }
                else if ((course_soak_wash_stubTimeTable[2].state[i].timeMSec != 0) && (washSelection == APP_NORMAL_MODE_UI_WASH_TIME_12_MIN))
                {
                    internalData.state[i].timeMSec = course_soak_wash_stubTimeTable[2].state[i].timeMSec;
                }
                else if ((course_soak_wash_stubTimeTable[3].state[i].timeMSec != 0) && (washSelection == APP_NORMAL_MODE_UI_WASH_TIME_18_MIN))
                {
                    internalData.state[i].timeMSec = course_soak_wash_stubTimeTable[3].state[i].timeMSec;
                }
                else
                {
                    // do nothing
                }
            }
        }

#ifdef WASH_STIR_TIME_CALCULATION_BY_USER_SELECTION
        internalData.state[APP_NORMAL_MODE_WASH_WASH_STIR].timeMSec = app_userInterface_data_get()->normalMode.wash.washSelection * 1000 * 60;
#elif defined(WASH_STIR_TIME_CALCULATION_BY_SYSTEM_TABLES)
        for (uint8_t i = 0; i < WASH_STIR_STATE_NUM; i++)
        {
            sum += app_nm_wash_wash_motorStirStateMotion_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection, (app_nm_wash_motor_stirState_t)i);
        }
        if (app_userInterface_data_get()->normalMode.wash.washSelection == APP_NORMAL_MODE_UI_WASH_TIME_3_MIN)
        {
            internalData.state[APP_NORMAL_MODE_WASH_WASH_STIR].timeMSec = sum;
        }
        else if (app_userInterface_data_get()->normalMode.wash.washSelection == APP_NORMAL_MODE_UI_WASH_TIME_9_MIN)
        {
            internalData.state[APP_NORMAL_MODE_WASH_WASH_STIR].timeMSec = 2 * sum;
        }
        else if (app_userInterface_data_get()->normalMode.wash.washSelection == APP_NORMAL_MODE_UI_WASH_TIME_12_MIN)
        {
            internalData.state[APP_NORMAL_MODE_WASH_WASH_STIR].timeMSec = 3 * sum;
        }
        else if (app_userInterface_data_get()->normalMode.wash.washSelection == APP_NORMAL_MODE_UI_WASH_TIME_18_MIN)
        {
            internalData.state[APP_NORMAL_MODE_WASH_WASH_STIR].timeMSec = 4 * sum;
        }
        else
        {
            // do nothing
        }
#endif

        if (app_userInterface_data_get()->normalMode.wash.steamTechSelection == APP_NORMAL_MODE_UI_STEAM_TECH_OFF)
        {
            internalData.state[APP_NORMAL_MODE_WASH_WASH_MH].timeMSec = 0;
            internalData.state[APP_NORMAL_MODE_WASH_WASH_MD].timeMSec = 0;
        }

        if (app_userInterface_data_get()->normalMode.wash.waterHeatingSelection == APP_NORMAL_MODE_UI_WATER_HEATING_OFF)
        {
            internalData.state[APP_NORMAL_MODE_WASH_WASH_HEATING].timeMSec = 0;
        }

        if (APP_NORMAL_MODE_UI_SOIL_LEVEL_HEAVY == soilLevel)
        {
            internalData.state[APP_NORMAL_MODE_WASH_WASH_STIR].timeMSec += WASH_SOIL_LEVEL_HEAVY_ADDIDTION_TIME_MSEC;
        }

        ret = &internalData;
    }

    return ret;
}

const app_nm_wash_wash_stateTime_t *app_nm_wash_wash_soak_stateData_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel)
{
    static app_nm_wash_wash_stateTime_t internalData;
    const app_nm_wash_wash_stateTime_t *ret = NULL;

#ifdef REMAINING_TIME_ALWAYS_IN_STANDBY
    if (waterLevel == APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0)
    {
        waterLevel = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_DURATION_DEFAULT;
    }
#endif

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT))
    {
        internalData = course_soak_wash_timeTable[waterLevel - 1];

        internalData.state[APP_NORMAL_MODE_WASH_WASH_SETUP].timeMSec = washStatePtr[course - 1][waterLevel - 1].state[APP_NORMAL_MODE_WASH_WASH_SETUP].timeMSec;
        internalData.state[APP_NORMAL_MODE_WASH_WASH_SETUP].waterLevelLiter = washStatePtr[course - 1][waterLevel - 1].state[APP_NORMAL_MODE_WASH_WASH_SETUP].waterLevelLiter;
        internalData.state[APP_NORMAL_MODE_WASH_WASH_M0].timeMSec = washStatePtr[course - 1][waterLevel - 1].state[APP_NORMAL_MODE_WASH_WASH_M0].timeMSec;
        internalData.state[APP_NORMAL_MODE_WASH_WASH_M0].waterLevelLiter = washStatePtr[course - 1][waterLevel - 1].state[APP_NORMAL_MODE_WASH_WASH_M0].waterLevelLiter;

        ret = &internalData;
    }

    return ret;
}

uint32_t app_nm_wash_wash_detergent_time_get(app_nm_wash_userInterface_waterLevelSelection_t waterLevel)
{
    uint32_t ret = 0;

#ifdef REMAINING_TIME_ALWAYS_IN_STANDBY
    if (waterLevel == APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0)
    {
        waterLevel = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_DURATION_DEFAULT;
    }
#endif

    if (waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_10)
    {
        if (app_userInterface_data_get()->normalMode.wash.waterTempSelection == APP_NORMAL_MODE_UI_WATER_TEMP_COLD)
        {
            ret = autoDetergent_pumpTimingsTable[waterLevel - 1].coldTimeMSec;
        }
        else if (app_userInterface_data_get()->normalMode.wash.waterTempSelection == APP_NORMAL_MODE_UI_WATER_TEMP_WARM)
        {
            ret = autoDetergent_pumpTimingsTable[waterLevel - 1].warmTimeMSec;
        }
        else if (app_userInterface_data_get()->normalMode.wash.waterTempSelection == APP_NORMAL_MODE_UI_WATER_TEMP_HOT)
        {
            ret = autoDetergent_pumpTimingsTable[waterLevel - 1].hotTimeMSec;
        }
        else
        {
            // do nothing
        }
    }

    return ret;
}
