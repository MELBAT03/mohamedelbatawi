#ifndef APP_NM_INIT_H
#define APP_NM_INIT_H

#include "def.h"
#include "app_nm.h"

typedef enum app_nm_init_state
{
    APP_NORMAL_MODE_INIT_INIT = 0,
    APP_NORMAL_MODE_INIT_MOTOR_CLUTCH_RESET = 1,
    APP_NORMAL_MODE_INIT_LINT_FILTER_CHECK = 2,
    APP_NORMAL_MODE_INIT_DELAY_START = 3,
    APP_NORMAL_MODE_INIT_FINISH = 4,
    APP_NORMAL_MODE_INIT_DONE = 5
} app_nm_init_state_t;

typedef enum app_nm_init_lintFilterCheckState
{
    LINT_FILTER_CHECK_INIT,
    LINT_FILTER_CHECK_REMOVE,
    LINT_FILTER_CHECK_RETURN,
    LINT_FILTER_CHECK_FINISH
} app_nm_init_lintFilterCheckState_t;

app_nm_state_t app_nm_init_process(uint32_t period);

app_nm_init_state_t app_nm_init_state_get(void);
void app_nm_init_state_set(app_nm_init_state_t value);

app_nm_init_lintFilterCheckState_t app_nm_init_lintFilterCheckState_get(void);
void app_nm_init_lintFilterCheckState_set(app_nm_init_lintFilterCheckState_t state);

void app_nm_init_reset(void);

#endif
