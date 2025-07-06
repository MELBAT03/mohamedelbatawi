#ifndef APP_NM_WASH_SOAK_TABLE_H
#define APP_NM_WASH_SOAK_TABLE_H

#include "def.h"

#include "app_nm_wash_data.h"

typedef struct app_nm_soak_wash_stubData
{
    uint32_t timeMSec;
} app_nm_soak_wash_stubData_t;

typedef struct app_nm_soak_wash_stateTime
{
    app_nm_soak_wash_stubData_t state[APP_NORMAL_MODE_WASH_STATES_WITH_DATA];
} app_nm_soak_wash_stateTime_t;

// SOAK WASH TABLE FOR 5 MIN SELECTION only vs waterlevels (wash before soak)
static const app_nm_wash_wash_stateTime_t course_soak_wash_timeTable[] = {
    {{{0}, {0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 120000U, .waterLevelLiter = 50U}, {.timeMSec = 30000U, .waterLevelLiter = 0}, {.timeMSec = 60000U, .waterLevelLiter = 50U}, {.timeMSec = 60000U, .waterLevelLiter = 50U}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 150000U, .waterLevelLiter = 50U}, {.timeMSec = 60000U, .waterLevelLiter = 0}}},

    {{{0}, {0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 120000U, .waterLevelLiter = 59U}, {.timeMSec = 30000U, .waterLevelLiter = 0}, {.timeMSec = 60000U, .waterLevelLiter = 59U}, {.timeMSec = 60000U, .waterLevelLiter = 59U}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 150000U, .waterLevelLiter = 59U}, {.timeMSec = 60000U, .waterLevelLiter = 0}}},

    {{{0}, {0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 120000U, .waterLevelLiter = 68U}, {.timeMSec = 30000U, .waterLevelLiter = 0}, {.timeMSec = 60000U, .waterLevelLiter = 68U}, {.timeMSec = 60000U, .waterLevelLiter = 68U}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 150000U, .waterLevelLiter = 68U}, {.timeMSec = 60000U, .waterLevelLiter = 0}}},

    {{{0}, {0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 120000U, .waterLevelLiter = 77U}, {.timeMSec = 30000U, .waterLevelLiter = 0}, {.timeMSec = 60000U, .waterLevelLiter = 77U}, {.timeMSec = 60000U, .waterLevelLiter = 77U}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 150000U, .waterLevelLiter = 77U}, {.timeMSec = 60000U, .waterLevelLiter = 0}}},

    {{{0}, {0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 120000U, .waterLevelLiter = 85U}, {.timeMSec = 30000U, .waterLevelLiter = 0}, {.timeMSec = 60000U, .waterLevelLiter = 85U}, {.timeMSec = 60000U, .waterLevelLiter = 85U}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 150000U, .waterLevelLiter = 85U}, {.timeMSec = 60000U, .waterLevelLiter = 0}}},

    {{{0}, {0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 120000U, .waterLevelLiter = 94U}, {.timeMSec = 30000U, .waterLevelLiter = 0}, {.timeMSec = 60000U, .waterLevelLiter = 94U}, {.timeMSec = 60000U, .waterLevelLiter = 94U}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 150000U, .waterLevelLiter = 94U}, {.timeMSec = 60000U, .waterLevelLiter = 0}}},

    {{{0}, {0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 120000U, .waterLevelLiter = 103U}, {.timeMSec = 30000U, .waterLevelLiter = 0}, {.timeMSec = 60000U, .waterLevelLiter = 103U}, {.timeMSec = 60000U, .waterLevelLiter = 103U}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 150000U, .waterLevelLiter = 103U}, {.timeMSec = 60000U, .waterLevelLiter = 0}}},

    {{{0}, {0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 120000U, .waterLevelLiter = 112U}, {.timeMSec = 30000U, .waterLevelLiter = 0}, {.timeMSec = 60000U, .waterLevelLiter = 112U}, {.timeMSec = 60000U, .waterLevelLiter = 112U}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 150000U, .waterLevelLiter = 112U}, {.timeMSec = 60000U, .waterLevelLiter = 0}}},

    {{{0}, {0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 120000U, .waterLevelLiter = 112U}, {.timeMSec = 30000U, .waterLevelLiter = 0}, {.timeMSec = 60000U, .waterLevelLiter = 112U}, {.timeMSec = 60000U, .waterLevelLiter = 112U}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 150000U, .waterLevelLiter = 112U}, {.timeMSec = 60000U, .waterLevelLiter = 0}}},

    {{{0}, {0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 120000U, .waterLevelLiter = 112U}, {.timeMSec = 30000U, .waterLevelLiter = 0}, {.timeMSec = 60000U, .waterLevelLiter = 112U}, {.timeMSec = 60000U, .waterLevelLiter = 112U}, {.timeMSec = 0, .waterLevelLiter = 0}, {.timeMSec = 150000U, .waterLevelLiter = 112U}, {.timeMSec = 60000U, .waterLevelLiter = 0}}}};

// soak wash selection vs stub wash times for wash cycle after soak ends (only the states that needs to be stubbed are added)
static const app_nm_soak_wash_stateTime_t course_soak_wash_stubTimeTable[] = {
    {{{0}, {0}, {0}, {0}, {0}, {0}, {0}, {20000U}, {20000U}, {20000U}, {0}, {120000U}, {0}}},    // 3 min
    {{{0}, {0}, {0}, {0}, {0}, {0}, {0}, {30000U}, {60000U}, {60000U}, {0}, {195000U}, {0}}},    // 9 min
    {{{0}, {0}, {0}, {0}, {0}, {0}, {0}, {30000U}, {60000U}, {60000U}, {0}, {190000U}, {0}}},    // 12 min
    {{{0}, {0}, {0}, {0}, {0}, {0}, {0}, {30000U}, {45000U}, {45000U}, {0}, {240000U}, {0}}}};   // 18 min

#endif
