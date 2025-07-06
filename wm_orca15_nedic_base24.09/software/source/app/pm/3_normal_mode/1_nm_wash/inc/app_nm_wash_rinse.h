#ifndef APP_NM_WASH_RINSE_H
#define APP_NM_WASH_RINSE_H

#include "def.h"

#define RINSE_FILL_MINIMUM_ALLOWED_VARIANCE (0U)
#define RINSE_FILL_MAXIMUM_ALLOWED_VARIANCE (200U)

typedef struct app_nm_wash_rinse_stateTime
{
    uint32_t drainTimeMSec;
    uint32_t spinTimeMSec;
    uint32_t spinBrakeTimeMSec;
    uint32_t fillTimeMSec;
    uint32_t stirTimeMSec;
    uint32_t untangleTimeMSec;
} app_nm_wash_rinse_stateTime_t;

typedef struct app_nm_wash_rinse_waterLevel
{
    uint8_t fillWaterLevelLiter;
} app_nm_wash_rinse_waterLevel_t;

const app_nm_wash_rinse_stateTime_t *app_nm_wash_rinse_stateTime_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel, app_nm_wash_userInterface_extraRinseSelection_t extraRinse);

const app_nm_wash_rinse_waterLevel_t *app_nm_wash_rinse_stateWaterLevel_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel);

#endif
