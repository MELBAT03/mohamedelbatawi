#include "def.h"
#include "app_nm_dry.h"
#include "app_nm_dry_motor.h"
#include "app_user_interface.h"
#include "app_nm_dry_operation_table.h"

#define APP_DRY_OPERATION_EXTRA_TIME_MSEC (1200000U)

static const app_dry_operation_timeTable_t *dry_operation_stateTotalTimeTableSelection[] = {
    app_dry_operation_30MinManualTimeTable,
    app_dry_operation_60MinManualTimeTable,
    app_dry_operation_90MinManualTimeTable,
    app_dry_operation_autoTimeTable};

static const uint32_t *dry_operation_condense2ModificationVsCourse[] = {
    regular_dry_operation_autoCondense2VsCourseTable,
    wool_dry_operation_autoCondense2VsCourseTable,
    sensitive_dry_operation_autoCondense2VsCourseTable,
    quick_dry_operation_autoCondense2VsCourseTable,
    hygiene_dry_operation_autoCondense2VsCourseTable,
    heavy_dry_operation_autoCondense2VsCourseTable,
    tubWash_dry_operation_autoCondense2VsCourseTable,
    babyCare_dry_operation_autoCondense2VsCourseTable,
    blanket_dry_operation_autoCondense2VsCourseTable,
    lowTempDry_dry_operation_autoCondense2VsCourseTable,
    easyIron_dry_operation_autoCondense2VsCourseTable,
    tubDry_dry_operation_autoCondense2VsCourseTable};

const app_dry_operation_timeTable_t *app_nm_dry_operation_stateTotalMSec_get(const app_nm_userInterface_courseSelection_t course, const app_nm_userInterface_dryTimeSelection_t selection, app_nm_dry_userInterface_waterLevelSelection_t waterLevel, app_nm_userInterface_drynessIntensitySelection_t dryness)
{
    static app_dry_operation_timeTable_t internalData;
    const app_dry_operation_timeTable_t *ret = NULL;

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM) && (waterLevel >= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_1 && waterLevel <= APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6) && (selection >= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN && selection <= APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO))
    {
        internalData = dry_operation_stateTotalTimeTableSelection[(uint8_t)selection - 1U][(uint8_t)waterLevel - 1U];

        if (selection == APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO)
        {
            internalData.stateTotalTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_2] = dry_operation_condense2ModificationVsCourse[course - 1][(uint8_t)waterLevel - 1U];
        }

        if (dryness == APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_HEAVY)
        {
            internalData.stateTotalTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME] = APP_DRY_OPERATION_EXTRA_TIME_MSEC;
        }

        ret = &internalData;
    }

    return ret;
}
