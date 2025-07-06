#ifndef APP_NM_WASH_SPIN_H
#define APP_NM_WASH_SPIN_H

#include "def.h"
#include "app_nm_wash.h"

typedef struct app_nm_wash_spin_stateTime
{
    uint32_t state[APP_NORMAL_MODE_SPIN_STATES_WITH_DATA];
} app_nm_wash_spin_stateTime_t;

typedef struct app_nm_wash_spin_unbalance_stateTime
{
    uint32_t drainTimeMSec;
    uint32_t fillTimeMSec;
    uint32_t stirTimeMSec;
    uint32_t superSpinStirTimeMSec;
} app_nm_wash_spin_unbalance_stateTime_t;

typedef struct app_nm_wash_spin_unbalance_waterLevel
{
    uint8_t fillWaterLevelLiter;
} app_nm_wash_spin_unbalance_waterLevel_t;

const app_nm_wash_spin_stateTime_t *app_nm_wash_spin_stateTime_get(app_nm_userInterface_courseSelection_t course);
const app_nm_wash_spin_unbalance_stateTime_t *app_nm_wash_spin_unabalanceStateTime_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel);
uint32_t app_nm_wash_spin_unbalanceStateWaterLevel_get(app_nm_wash_userInterface_waterLevelSelection_t waterLevel);

#endif
