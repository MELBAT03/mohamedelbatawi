#ifndef APP_NM_TUBCLEAN_H
#define APP_NM_TUBCLEAN_H

#include "def.h"
#include "app_nm.h"

typedef enum app_nm_tubclean_state
{
    APP_NORMAL_MODE_TUBCLEAN_INIT = 0,
    APP_NORMAL_MODE_TUBCLEAN_FILL = 1,
    APP_NORMAL_MODE_TUBCLEAN_MH = 2,
    APP_NORMAL_MODE_TUBCLEAN_MD = 3,
    APP_NORMAL_MODE_TUBCLEAN_FILL_STIR = 4,
    APP_NORMAL_MODE_TUBCLEAN_SOAK = 5,
    APP_NORMAL_MODE_TUBCLEAN_WASH = 6,
    APP_NORMAL_MODE_TUBCLEAN_DRAIN = 7,
    APP_NORMAL_MODE_TUBCLEAN_SPIN = 8,
    APP_NORMAL_MODE_TUBCLEAN_REGROUP = 9,
    APP_NORMAL_MODE_TUBCLEAN_FINISH = 10,
    APP_NORMAL_MODE_TUBCLEAN_DONE = 11
} app_nm_tubclean_state_t;

typedef enum app_nm_tubclean_fillStirState
{
    APP_NORMAL_MODE_TUBCLEAN_FILL_BY_LEVEL,
    APP_NORMAL_MODE_TUBCLEAN_STIR,
    APP_NORMAL_MODE_TUBCLEAN_FILL_BY_TIME
} app_nm_tubclean_fillState_t;

typedef enum app_nm_tubclean_soakState
{
    APP_NORMAL_MODE_TUBCLEAN_SOAK_STIR,
    APP_NORMAL_MODE_TUBCLEAN_SOAK_STOP
} app_nm_tubclean_soakState_t;

typedef enum app_nm_tubclean_drainState
{
    APP_NORMAL_MODE_TUBCLEAN_DRAIN_SPIN,
    APP_NORMAL_MODE_TUBCLEAN_DRAIN_STOP
} app_nm_tubclean_drainState_t;

typedef struct app_nm_tubclean_fillStatus
{
    uint8_t type;
    uint32_t timeMSec;
    uint32_t waterLevelLiter;
} app_nm_tubclean_fillStatus_t;

app_nm_state_t app_nm_tubclean_process(uint32_t period);

app_nm_tubclean_state_t app_nm_tubclean_state_get(void);
void app_nm_tubclean_state_set(app_nm_tubclean_state_t state);

uint8_t app_nm_tubclean_fillCounter_get(void);

const uint32_t *app_nm_tubclean_timeTable_get(void);
uint32_t app_nm_tubclean_fillWaterLevel_get(void);
const app_nm_tubclean_fillStatus_t *app_nm_tubclean_fillStirTable_get(uint8_t index);

#endif
