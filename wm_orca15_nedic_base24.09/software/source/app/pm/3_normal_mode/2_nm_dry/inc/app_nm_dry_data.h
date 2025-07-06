#ifndef APP_NM_DRY_DATA_H
#define APP_NM_DRY_DATA_H

#include "def.h"

#define APP_NORMAL_MODE_DRY_PRE_SETUP_STATES_WITH_DATA (4U)
#define APP_NORMAL_MODE_DRY_OPERATION_STATES_WITH_DATA (12U)

typedef struct app_nm_dry_data
{
    uint32_t dryTotalTimeMSec;
    uint32_t preSetupTimeMSec[APP_NORMAL_MODE_DRY_PRE_SETUP_STATES_WITH_DATA];
    uint32_t operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_STATES_WITH_DATA];
} app_nm_dry_data_t;

#endif
