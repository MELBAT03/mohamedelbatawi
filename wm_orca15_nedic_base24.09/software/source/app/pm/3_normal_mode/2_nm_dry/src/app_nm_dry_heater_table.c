#include "def.h"
#include "app_heater.h"
#include "app_nm_dry_heater.h"

#include "app_user_interface.h"
#include "app_nm_user_interface.h"

static const app_heater_plateState_t app_dry_heater_allowedMaxPlates[APP_NORMAL_MODE_UI_COURSE_NUM] = {
    APP_HEATER_TWO_PLATES,
    APP_HEATER_TWO_PLATES,
    APP_HEATER_TWO_PLATES,
    APP_HEATER_TWO_PLATES,
    APP_HEATER_TWO_PLATES,
    APP_HEATER_TWO_PLATES,
    APP_HEATER_TWO_PLATES,
    APP_HEATER_TWO_PLATES,
    APP_HEATER_TWO_PLATES,
    APP_HEATER_ONE_PLATE,
    APP_HEATER_TWO_PLATES,
    APP_HEATER_TWO_PLATES};

static const app_nm_dry_heater_pattern_t app_dry_heater_pattern[APP_NORMAL_MODE_DRY_HEATER_PATTERN_NUM] = {
    {.initialPatternIntervalMSec = {10000, 0xffffffff, 0}, .mainPatternIntervalMSec = {0xffffffff, 300000, 0, 0}},
    {.initialPatternIntervalMSec = {10000, 0xffffffff, 0}, .mainPatternIntervalMSec = {0xffffffff, 300000, 300000, 300000}},
    {.initialPatternIntervalMSec = {10000, 0xffffffff, 300000}, .mainPatternIntervalMSec = {0xffffffff, 300000, 300000, 300000}},
    {.initialPatternIntervalMSec = {10000, 0xffffffff, 0}, .mainPatternIntervalMSec = {0xffffffff, 300000, 0, 0}}};

app_heater_plateState_t app_nm_dry_heater_allowedMaxPlatesPerCourse_get(app_nm_userInterface_courseSelection_t course)
{
    app_heater_plateState_t ret = APP_HEATER_OFF;

    if (course > APP_NORMAL_MODE_UI_COURSE_NONE && course < APP_NORMAL_MODE_UI_COURSE_NUM)
    {
        ret = app_dry_heater_allowedMaxPlates[course - 1];
    }

    return ret;
}

const app_nm_dry_heater_pattern_t *app_nm_dry_heater_table_get(app_nm_dry_heater_patternSelection_t selection)
{
    const app_nm_dry_heater_pattern_t *ret = NULL;

    if (selection >= APP_NORMAL_MODE_DRY_HEATER_DEFAULT_PATTERN && selection <= APP_NORMAL_MODE_DRY_HEATER_AUTO_3_PATTERN)
    {
        ret = &app_dry_heater_pattern[selection];
    }

    return ret;
}
