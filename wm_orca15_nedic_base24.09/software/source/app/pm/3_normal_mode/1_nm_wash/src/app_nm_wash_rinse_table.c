#include "def.h"

#include "app_drain.h"
#include "app_fill.h"
#include "app_motor.h"

#include "app_nm_wash_rinse_table.h"

static const app_nm_wash_rinse_stateTime_t *const stateTimePtr[] = {
    &course_white_rinseTimeTable,
    &course_light_rinseTimeTable,
    &course_dark_rinseTimeTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    &course_eco_rinseTimeTable,
#else
    &course_wool_rinseTimeTable,
#endif
    &course_cotton_rinseTimeTable,
    &course_jeans_rinseTimeTable,
    &course_sensitive_rinseTimeTable,
    &course_sports_rinseTimeTable,
    &course_mix_rinseTimeTable,
#ifdef AUTO_DETERGENT_HEATER_MODEL
    &course_alergy_rinseTimeTable,
#else
    &course_rapidWash_rinseTimeTable,
#endif
    &course_bedding_rinseTimeTable,
    &course_babycare_rinseTimeTable};

static const app_nm_wash_rinse_waterLevel_t *const stateWaterLevelPtr[] = {
    rinse_waterLevelTable,
    rinse_waterLevelTable,
    rinse_waterLevelTable,
    rinse_waterLevelTable,
    rinse_waterLevelTable,
    rinse_waterLevelTable,
    rinse_waterLevelTable,
    rinse_waterLevelTable,
    rinse_waterLevelTable,
    rinse_waterLevelTable,
    rinse_waterLevelTable,
    rinse_waterLevelTable};

const app_nm_wash_rinse_stateTime_t *app_nm_wash_rinse_stateTime_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_userInterface_extraRinseSelection_t extraRinse)
{
    static app_nm_wash_rinse_stateTime_t internalData;
    const app_nm_wash_rinse_stateTime_t *ret = NULL;

#ifdef REMAINING_TIME_ALWAYS_IN_STANDBY
    if (waterLevel == APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0)
    {
        waterLevel = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_DURATION_DEFAULT;
    }
#endif

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT) && (extraRinse <= APP_NORMAL_MODE_UI_RINSE_EXTENDED))
    {
        internalData = *(stateTimePtr[course - 1]);
        if (extraRinse == APP_NORMAL_MODE_UI_RINSE_EXTENDED)
        {
            internalData.stirTimeMSec *= 2;
        }
        ret = &internalData;
    }

    return ret;
}

const app_nm_wash_rinse_waterLevel_t *app_nm_wash_rinse_stateWaterLevel_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel)
{
    const app_nm_wash_rinse_waterLevel_t *ret = NULL;

#ifdef REMAINING_TIME_ALWAYS_IN_STANDBY
    if (waterLevel == APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0)
    {
        waterLevel = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_DURATION_DEFAULT;
    }
#endif

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT))
    {
        ret = &(stateWaterLevelPtr[course - 1][waterLevel - 1]);
    }

    return ret;
}
