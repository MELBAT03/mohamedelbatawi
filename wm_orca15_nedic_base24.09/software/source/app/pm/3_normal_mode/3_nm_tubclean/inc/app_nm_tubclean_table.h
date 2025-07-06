#ifndef APP_NM_TUBCLEAN_TABLE_H
#define APP_NM_TUBCLEAN_TABLE_H

#include "def.h"
#include "app_nm_tubclean.h"

#define TUBCLEAN_FILL_STATE_WATER_LEVEL_LITER (10U)

static const uint32_t gu32_app_nm_tubclean_timeMSec[] = {0, 30000U, 180000U, 300000U, 1380000U, 1200000U, 180000U, 120000U, 100000U};

static const app_nm_tubclean_fillStatus_t gxa_waterSupplyLevelValues_litter[] = {
    {.type = APP_NORMAL_MODE_TUBCLEAN_FILL_BY_LEVEL, .timeMSec = 0U, .waterLevelLiter = 45U},
    {.type = APP_NORMAL_MODE_TUBCLEAN_STIR, .timeMSec = 120000U, .waterLevelLiter = 45U},
    {.type = APP_NORMAL_MODE_TUBCLEAN_FILL_BY_LEVEL, .timeMSec = 0U, .waterLevelLiter = 70U},
    {.type = APP_NORMAL_MODE_TUBCLEAN_STIR, .timeMSec = 120000U, .waterLevelLiter = 70U},
    {.type = APP_NORMAL_MODE_TUBCLEAN_FILL_BY_LEVEL, .timeMSec = 0U, .waterLevelLiter = 90U},
    {.type = APP_NORMAL_MODE_TUBCLEAN_STIR, .timeMSec = 120000U, .waterLevelLiter = 90U},
    {.type = APP_NORMAL_MODE_TUBCLEAN_FILL_BY_LEVEL, .timeMSec = 0U, .waterLevelLiter = 105U},
    {.type = APP_NORMAL_MODE_TUBCLEAN_STIR, .timeMSec = 120000U, .waterLevelLiter = 105U},
    {.type = APP_NORMAL_MODE_TUBCLEAN_FILL_BY_TIME, .timeMSec = 80000U, .waterLevelLiter = 105U},
    {.type = APP_NORMAL_MODE_TUBCLEAN_STIR, .timeMSec = 120000U, .waterLevelLiter = 105U},
    {.type = APP_NORMAL_MODE_TUBCLEAN_FILL_BY_TIME, .timeMSec = 30000U, .waterLevelLiter = 105U},
    {.type = APP_NORMAL_MODE_TUBCLEAN_STIR, .timeMSec = 120000U, .waterLevelLiter = 105U}};

#endif
