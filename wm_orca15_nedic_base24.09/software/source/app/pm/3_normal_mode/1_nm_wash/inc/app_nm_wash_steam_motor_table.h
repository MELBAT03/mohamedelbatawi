#ifndef APP_NM_WASH_STEAM_MOTOR_TABLE_H
#define APP_NM_WASH_STEAM_MOTOR_TABLE_H

#include "def.h"

#include "app_nm_wash.h"
#include "app_nm_wash_user_interface.h"

// NOTE: steam
#define STEAM_MOTOR_PRE_SPIN_LAST_SECTOR_NUM (5U)

static const app_nm_wash_steam_motorStateTime_t app_nm_wash_steam_motor_stirInterStateTable[] = {
    {.stirStateTimeMSec[STEAM_STIR_UNTANGLE] = 120000U, .stirStateTimeMSec[STEAM_STIR_MD] = 180000U, .stirStateTimeMSec[STEAM_STIR_M1] = 40000U, .stirStateTimeMSec[STEAM_STIR_M2] = 40000U, .stirStateTimeMSec[STEAM_STIR_M3] = 40000U},
    {.stirStateTimeMSec[STEAM_STIR_UNTANGLE] = 120000U, .stirStateTimeMSec[STEAM_STIR_MD] = 180000U, .stirStateTimeMSec[STEAM_STIR_M1] = 40000U, .stirStateTimeMSec[STEAM_STIR_M2] = 40000U, .stirStateTimeMSec[STEAM_STIR_M3] = 40000U},
    {.stirStateTimeMSec[STEAM_STIR_UNTANGLE] = 120000U, .stirStateTimeMSec[STEAM_STIR_MD] = 180000U, .stirStateTimeMSec[STEAM_STIR_M1] = 40000U, .stirStateTimeMSec[STEAM_STIR_M2] = 40000U, .stirStateTimeMSec[STEAM_STIR_M3] = 40000U},
    {.stirStateTimeMSec[STEAM_STIR_UNTANGLE] = 120000U, .stirStateTimeMSec[STEAM_STIR_MD] = 180000U, .stirStateTimeMSec[STEAM_STIR_M1] = 40000U, .stirStateTimeMSec[STEAM_STIR_M2] = 40000U, .stirStateTimeMSec[STEAM_STIR_M3] = 40000U},
    {.stirStateTimeMSec[STEAM_STIR_UNTANGLE] = 120000U, .stirStateTimeMSec[STEAM_STIR_MD] = 180000U, .stirStateTimeMSec[STEAM_STIR_M1] = 40000U, .stirStateTimeMSec[STEAM_STIR_M2] = 40000U, .stirStateTimeMSec[STEAM_STIR_M3] = 40000U},
    {.stirStateTimeMSec[STEAM_STIR_UNTANGLE] = 120000U, .stirStateTimeMSec[STEAM_STIR_MD] = 180000U, .stirStateTimeMSec[STEAM_STIR_M1] = 40000U, .stirStateTimeMSec[STEAM_STIR_M2] = 40000U, .stirStateTimeMSec[STEAM_STIR_M3] = 40000U},
    {.stirStateTimeMSec[STEAM_STIR_UNTANGLE] = 120000U, .stirStateTimeMSec[STEAM_STIR_MD] = 180000U, .stirStateTimeMSec[STEAM_STIR_M1] = 40000U, .stirStateTimeMSec[STEAM_STIR_M2] = 40000U, .stirStateTimeMSec[STEAM_STIR_M3] = 40000U},
    {.stirStateTimeMSec[STEAM_STIR_UNTANGLE] = 120000U, .stirStateTimeMSec[STEAM_STIR_MD] = 180000U, .stirStateTimeMSec[STEAM_STIR_M1] = 40000U, .stirStateTimeMSec[STEAM_STIR_M2] = 40000U, .stirStateTimeMSec[STEAM_STIR_M3] = 40000U},
    {.stirStateTimeMSec[STEAM_STIR_UNTANGLE] = 120000U, .stirStateTimeMSec[STEAM_STIR_MD] = 180000U, .stirStateTimeMSec[STEAM_STIR_M1] = 40000U, .stirStateTimeMSec[STEAM_STIR_M2] = 40000U, .stirStateTimeMSec[STEAM_STIR_M3] = 40000U},
    {.stirStateTimeMSec[STEAM_STIR_UNTANGLE] = 120000U, .stirStateTimeMSec[STEAM_STIR_MD] = 180000U, .stirStateTimeMSec[STEAM_STIR_M1] = 40000U, .stirStateTimeMSec[STEAM_STIR_M2] = 40000U, .stirStateTimeMSec[STEAM_STIR_M3] = 40000U}};

// steam motor pattern
static const app_nm_wash_steam_motor_statePattern_t course_group_1_steamPatternTable[] = {
    {.pattern[STEAM_STIR_UNTANGLE] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_MD] = {0, 0, 180000, 0, 1000, 1000, 100}, .pattern[STEAM_STIR_M1] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_M2] = {500, 1300, 500, 1300, 300, 500, 110}, .pattern[STEAM_STIR_M3] = {2600, 1900, 2600, 1900, 500, 500, 120}},
    {.pattern[STEAM_STIR_UNTANGLE] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_MD] = {0, 0, 180000, 0, 1000, 1000, 100}, .pattern[STEAM_STIR_M1] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_M2] = {500, 1300, 500, 1300, 300, 500, 110}, .pattern[STEAM_STIR_M3] = {2600, 1900, 2600, 1900, 500, 500, 120}},
    {.pattern[STEAM_STIR_UNTANGLE] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_MD] = {0, 0, 180000, 0, 1000, 1000, 100}, .pattern[STEAM_STIR_M1] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_M2] = {500, 1300, 500, 1300, 300, 500, 110}, .pattern[STEAM_STIR_M3] = {2600, 1900, 2600, 1900, 500, 500, 120}},
    {.pattern[STEAM_STIR_UNTANGLE] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_MD] = {0, 0, 180000, 0, 1000, 1000, 100}, .pattern[STEAM_STIR_M1] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_M2] = {500, 1300, 500, 1300, 300, 500, 110}, .pattern[STEAM_STIR_M3] = {2600, 1900, 2600, 1900, 500, 500, 120}},
    {.pattern[STEAM_STIR_UNTANGLE] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_MD] = {0, 0, 180000, 0, 1000, 1000, 100}, .pattern[STEAM_STIR_M1] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_M2] = {500, 1300, 500, 1300, 300, 500, 110}, .pattern[STEAM_STIR_M3] = {2600, 1900, 2600, 1900, 500, 500, 120}},
    {.pattern[STEAM_STIR_UNTANGLE] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_MD] = {0, 0, 180000, 0, 1000, 1000, 100}, .pattern[STEAM_STIR_M1] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_M2] = {500, 1300, 500, 1300, 300, 500, 110}, .pattern[STEAM_STIR_M3] = {2600, 1900, 2600, 1900, 500, 500, 120}},
    {.pattern[STEAM_STIR_UNTANGLE] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_MD] = {0, 0, 180000, 0, 1000, 1000, 100}, .pattern[STEAM_STIR_M1] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_M2] = {500, 1300, 500, 1300, 300, 500, 110}, .pattern[STEAM_STIR_M3] = {2600, 1900, 2600, 1900, 500, 500, 120}},
    {.pattern[STEAM_STIR_UNTANGLE] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_MD] = {0, 0, 180000, 0, 1000, 1000, 100}, .pattern[STEAM_STIR_M1] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_M2] = {500, 1300, 500, 1300, 300, 500, 110}, .pattern[STEAM_STIR_M3] = {2600, 1900, 2600, 1900, 500, 500, 120}},
    {.pattern[STEAM_STIR_UNTANGLE] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_MD] = {0, 0, 180000, 0, 1000, 1000, 100}, .pattern[STEAM_STIR_M1] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_M2] = {500, 1300, 500, 1300, 300, 500, 110}, .pattern[STEAM_STIR_M3] = {2600, 1900, 2600, 1900, 500, 500, 120}},
    {.pattern[STEAM_STIR_UNTANGLE] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_MD] = {0, 0, 180000, 0, 1000, 1000, 100}, .pattern[STEAM_STIR_M1] = {2600, 1900, 2600, 1900, 500, 500, 120}, .pattern[STEAM_STIR_M2] = {500, 1300, 500, 1300, 300, 500, 110}, .pattern[STEAM_STIR_M3] = {2600, 1900, 2600, 1900, 500, 500, 120}}};

#endif
