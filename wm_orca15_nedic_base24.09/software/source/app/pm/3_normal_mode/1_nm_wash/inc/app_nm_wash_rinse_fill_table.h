#ifndef APP_NM_WASH_RINSE_FILL_TABLE_H
#define APP_NM_WASH_RINSE_FILL_TABLE_H

#include "def.h"

#include "app_nm_wash_fill.h"

static const app_nm_wash_rinse_fillSoftnerWaterLevel_t rinse_softnerWaterLevelTable[] = {
    {.softnerWaterLevelLiter = 0},
    {.softnerWaterLevelLiter = 0},
    {.softnerWaterLevelLiter = 0},
    {.softnerWaterLevelLiter = 0},
    {.softnerWaterLevelLiter = 68U},
    {.softnerWaterLevelLiter = 77U},
    {.softnerWaterLevelLiter = 85U},
    {.softnerWaterLevelLiter = 94U},
    {.softnerWaterLevelLiter = 94U},
    {.softnerWaterLevelLiter = 94U}};

static const uint32_t rinse_autoSoftenerTimeoutTable[] = {
    10000U,
    10000U,
    10000U,
    10000U,
    10000U,
    10000U,
    10000U,
    10000U,
    10000U,
    10000U};

#endif
