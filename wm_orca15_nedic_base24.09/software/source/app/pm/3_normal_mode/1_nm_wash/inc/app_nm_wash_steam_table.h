#ifndef APP_NM_WASH_STEAM_TABLE_H
#define APP_NM_WASH_STEAM_TABLE_H

#include "def.h"

#define APP_NM_WASH_STEAM_COLD_TEMP (30U)
#define APP_NM_WASH_STEAM_WARM_TEMP (50U)
#define APP_NM_WASH_STEAM_HOT_TEMP  (75U)

static const app_nm_wash_steam_stateTime_t course_group_1_steamTimeTable[] = {
    {{0, 120000U, 60000U, 2400000U}},
    {{0, 120000U, 60000U, 2400000U}},
    {{0, 120000U, 60000U, 2400000U}},
    {{0, 120000U, 60000U, 3000000U}},
    {{0, 120000U, 60000U, 3000000U}},
    {{0, 120000U, 60000U, 3000000U}},
    {{0, 120000U, 60000U, 3000000U}},
    {{0, 120000U, 60000U, 3000000U}},
    {{0, 120000U, 60000U, 3000000U}},
    {{0, 120000U, 60000U, 3000000U}}};

static const app_nm_wash_steam_stateVsTempTime_t course_group_1_steamStateVsTempTable[] = {
    {.tempCold = 2400000U, .tempWarm = 1800000U, .tempHot = 900000U},
    {.tempCold = 2400000U, .tempWarm = 1800000U, .tempHot = 900000U},
    {.tempCold = 2400000U, .tempWarm = 1800000U, .tempHot = 900000U},
    {.tempCold = 3000000U, .tempWarm = 2400000U, .tempHot = 1200000U},
    {.tempCold = 3000000U, .tempWarm = 2400000U, .tempHot = 1200000U},
    {.tempCold = 3000000U, .tempWarm = 2400000U, .tempHot = 1200000U},
    {.tempCold = 3000000U, .tempWarm = 2400000U, .tempHot = 1200000U},
    {.tempCold = 3000000U, .tempWarm = 2400000U, .tempHot = 1200000U},
    {.tempCold = 3000000U, .tempWarm = 2400000U, .tempHot = 1200000U},
    {.tempCold = 3000000U, .tempWarm = 2400000U, .tempHot = 1200000U}};

#endif
