#include "def.h"
#include "app_user_interface.h"
#include "app_nm_wash_fill.h"

#include "app_nm_wash_rinse_fill_table.h"

static const app_nm_wash_rinse_fillSoftnerWaterLevel_t *const stateWaterLevelPtr[] = {
    rinse_softnerWaterLevelTable,
    rinse_softnerWaterLevelTable,
    rinse_softnerWaterLevelTable,
    rinse_softnerWaterLevelTable,
    rinse_softnerWaterLevelTable,
    rinse_softnerWaterLevelTable,
    rinse_softnerWaterLevelTable,
    rinse_softnerWaterLevelTable,
    rinse_softnerWaterLevelTable,
    rinse_softnerWaterLevelTable,
    rinse_softnerWaterLevelTable,
    rinse_softnerWaterLevelTable};

const app_nm_wash_rinse_fillSoftnerWaterLevel_t *app_nm_wash_fill_softnerWaterLevel_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel)
{
    const app_nm_wash_rinse_fillSoftnerWaterLevel_t *ret = NULL;

    if (((course > APP_NORMAL_MODE_UI_COURSE_NONE) && (course < APP_NORMAL_MODE_UI_COURSE_NUM)) && ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1) && (waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT)))
    {
        ret = &(stateWaterLevelPtr[course - 1][waterLevel - 1]);
    }

    return ret;
}

uint32_t app_nm_wash_fill_autoSoftenerTimeout_get(void)
{
    return rinse_autoSoftenerTimeoutTable[app_userInterface_data_get()->normalMode.wash.waterLevelSelection];
}
