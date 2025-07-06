#ifndef APP_NM_WASH_DATA_H
#define APP_NM_WASH_DATA_H

#include "def.h"
#include "app_nm_user_interface.h"

#define APP_NORMAL_MODE_SOAK_STATES_WITH_DATA            (2U)
#define APP_NORMAL_MODE_WASH_STATES_WITH_DATA            (13U)   // the number of states that has data in wash table + init state for indexing
#define APP_NORMAL_MODE_WASH_STEAM_TECH_STATES_WITH_DATA (4U)
#define APP_NORMAL_MODE_STEAM_STATES_WITH_DATA           (4U)
#define APP_NORMAL_MODE_SPIN_STATES_WITH_DATA            (6U)
#define APP_NORMAL_MODE_TUBCLEAN_STATES_WITH_DATA        (9U)

#define APP_NM_TUBCLEAN_REPETITION_COUNT (2U)

typedef struct app_nm_wash_data
{
    uint32_t washTotalTimeMSec;

    uint32_t soakTotalTimeMSec;
    uint32_t soakTimeMSec;
    uint32_t soakStateTimeMSec[APP_NORMAL_MODE_WASH_STATES_WITH_DATA];
    uint8_t soakStateWaterLevelLiter[APP_NORMAL_MODE_WASH_STATES_WITH_DATA];

    uint32_t washWashTotalTimeMSec;
    uint32_t washStateTimeMSec[APP_NORMAL_MODE_WASH_STATES_WITH_DATA];
    uint8_t washStateWaterLevelLiter[APP_NORMAL_MODE_WASH_STATES_WITH_DATA];

    uint32_t steamTotalTimeMSec;
    uint32_t steamStateTimeMSec[APP_NORMAL_MODE_STEAM_STATES_WITH_DATA];

    uint32_t rinseTotalTimeMSec;
    uint8_t rinseNum;
    uint32_t rinseFillWaterLevelLiter;
    uint32_t rinseDrainTimeMSec[APP_NORMAL_MODE_UI_RINSE_SELECTION_NUM];
    uint32_t rinseSpinTimeMSec[APP_NORMAL_MODE_UI_RINSE_SELECTION_NUM];
    uint32_t rinseBrakeTimeMSec[APP_NORMAL_MODE_UI_RINSE_SELECTION_NUM];
    uint32_t rinseFillTimeMSec[APP_NORMAL_MODE_UI_RINSE_SELECTION_NUM];
    uint32_t rinseStirTimeMSec[APP_NORMAL_MODE_UI_RINSE_SELECTION_NUM];
    uint32_t rinseUntangleTimeMSec[APP_NORMAL_MODE_UI_RINSE_SELECTION_NUM];
    uint32_t rinseUnbalanceFillTimeMSec;
    uint32_t rinseUnbalanceStirTimeMSec;
    uint32_t rinseUnbalanceDrainTimeMSec;

    uint32_t spinTotalTimeMSec;
    uint32_t spinStateTimeMSec[APP_NORMAL_MODE_SPIN_STATES_WITH_DATA];
    uint32_t spinUnbalanceFillTimeMSec;
    uint32_t spinUnbalanceStirTimeMSec;
    uint32_t spinUnbalanceDrainTimeMSec;
    uint32_t spinUnbalanceSuperSpinStirTimeMSec;

    uint32_t tubcleanTotalTimeMSec;
    uint8_t tubcleanNum;
    uint32_t tubcleanStateTimeMSec[APP_NORMAL_MODE_TUBCLEAN_STATES_WITH_DATA][APP_NM_TUBCLEAN_REPETITION_COUNT];
} app_nm_wash_data_t;

#endif
