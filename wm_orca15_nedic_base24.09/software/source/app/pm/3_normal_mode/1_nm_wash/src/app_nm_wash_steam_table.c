#include "def.h"
#include "app_nm_wash.h"

#include "app_nm_wash_steam_table.h"

static const app_nm_wash_steam_stateTime_t *const steamStatePtr[] = {
    course_group_1_steamTimeTable,
    course_group_1_steamTimeTable,
    course_group_1_steamTimeTable,
    course_group_1_steamTimeTable,
    course_group_1_steamTimeTable,
    course_group_1_steamTimeTable,
    course_group_1_steamTimeTable,
    course_group_1_steamTimeTable,
    course_group_1_steamTimeTable,
    course_group_1_steamTimeTable,
    course_group_1_steamTimeTable,
    course_group_1_steamTimeTable};

static const app_nm_wash_steam_stateVsTempTime_t *const steamVsTempPtr[] = {
    course_group_1_steamStateVsTempTable,
    course_group_1_steamStateVsTempTable,
    course_group_1_steamStateVsTempTable,
    course_group_1_steamStateVsTempTable,
    course_group_1_steamStateVsTempTable,
    course_group_1_steamStateVsTempTable,
    course_group_1_steamStateVsTempTable,
    course_group_1_steamStateVsTempTable,
    course_group_1_steamStateVsTempTable,
    course_group_1_steamStateVsTempTable,
    course_group_1_steamStateVsTempTable,
    course_group_1_steamStateVsTempTable};

app_nm_wash_userInterface_waterTempSelection_t app_nm_wash_steam_waterTemp_map(uint16_t waterTemp)
{
    app_nm_wash_userInterface_waterTempSelection_t ret = APP_NORMAL_MODE_UI_WATER_TEMP_NONE;

    if (waterTemp <= APP_NM_WASH_STEAM_COLD_TEMP)
    {
        ret = APP_NORMAL_MODE_UI_WATER_TEMP_COLD;
    }
    else if (waterTemp <= APP_NM_WASH_STEAM_WARM_TEMP)
    {
        ret = APP_NORMAL_MODE_UI_WATER_TEMP_WARM;
    }
    else if (waterTemp <= APP_NM_WASH_STEAM_HOT_TEMP)
    {
        ret = APP_NORMAL_MODE_UI_WATER_TEMP_HOT;
    }
    else
    {
        // do nothing
    }

    return ret;
}

const app_nm_wash_steam_stateTime_t *app_nm_wash_steam_stateData_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_userInterface_steamSelection_t steamSelection, app_nm_wash_userInterface_waterTempSelection_t waterTemp)
{
    static app_nm_wash_steam_stateTime_t internalData;
    const app_nm_wash_steam_stateTime_t *ret = NULL;

#ifdef REMAINING_TIME_ALWAYS_IN_STANDBY
    if (waterLevel == APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0)
    {
        waterLevel = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_DURATION_DEFAULT;
    }
#endif

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (steamSelection == APP_NORMAL_MODE_UI_STEAM_ON) && (waterTemp >= APP_NORMAL_MODE_UI_WATER_TEMP_COLD && waterTemp <= APP_NORMAL_MODE_UI_WATER_TEMP_HOT))
    {
        internalData = steamStatePtr[course - 1][waterLevel - 1];

        if (waterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_COLD)
        {
            internalData.state[APP_NORMAL_MODE_WASH_STEAM_STEAM] = steamVsTempPtr[course - 1][waterLevel - 1].tempCold;
        }
        else if (waterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_WARM)
        {
            internalData.state[APP_NORMAL_MODE_WASH_STEAM_STEAM] = steamVsTempPtr[course - 1][waterLevel - 1].tempWarm;
        }
        else if (waterTemp == APP_NORMAL_MODE_UI_WATER_TEMP_HOT)
        {
            internalData.state[APP_NORMAL_MODE_WASH_STEAM_STEAM] = steamVsTempPtr[course - 1][waterLevel - 1].tempHot;
        }
        else
        {
            // do nothing
        }

        ret = &internalData;
    }

    return ret;
}
