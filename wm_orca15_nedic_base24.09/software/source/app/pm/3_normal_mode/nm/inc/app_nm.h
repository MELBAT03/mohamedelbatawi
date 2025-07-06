#ifndef APP_NM_H
#define APP_NM_H

#include "def.h"

#include "app_processor.h"
#include "app_nm_wash_data.h"
#ifdef DRYER_ENABLED
#include "app_nm_dry_data.h"
#endif

typedef enum app_nm_state
{
    APP_NORMAL_MODE_INIT = 0,
    APP_NORMAL_MODE_WASH = 1,
    APP_NORMAL_MODE_DRY = 2,
    APP_NORMAL_MODE_TUBCLEAN = 3,
    APP_NORMAL_MODE_PAUSE = 4,
    APP_NORMAL_MODE_FINISH = 5,
    APP_NORMAL_MODE_DONE = 6
} app_nm_state_t;

typedef struct app_nm_data
{
    uint32_t totalTimeMSec;

    app_nm_wash_data_t wash;
#ifdef DRYER_ENABLED
    app_nm_dry_data_t dry;
#endif

    uint32_t delayStart;
} app_nm_data_t;

app_processor_state_t app_nm_process(uint32_t period);

app_nm_state_t app_nm_state_get(void);
app_nm_state_t app_nm_prevState_get(void);

void app_nm_state_set(app_nm_state_t value);
void app_nm_prevState_set(app_nm_state_t value);

const app_nm_data_t *app_nm_data_get(void);
app_nm_data_t *app_nm_data_set(void);
app_nm_userInterface_data_t *app_nm_savedUIData_get(void);

void app_nm_remainingTime_process(uint32_t period, uint32_t *current);
uint32_t app_nm_remainingTimeMin_get(void);

void app_nm_reset(void);

#endif
