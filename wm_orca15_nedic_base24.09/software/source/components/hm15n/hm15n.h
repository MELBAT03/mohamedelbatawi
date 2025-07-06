#ifndef HM_15N_H
#define HM_15N_H 1

#include "def.h"
#include "components/doutput_module/doutput_module.h"
#include "components/finput_module/finput_module.h"

typedef enum hm15n_sensor_state
{
    HM15N_SENSOR_INACTIVE = 0,
    HM15N_SENSOR_ACTIVE = 1
} hm15n_sensor_state_t;

typedef enum
{
    HM15N_IDLE = 3,
    HM15N_RESET = 4,
    HM15N_PROCESSING = 2,
    HM15N_ACTIVE = 1,
    HM15N_INACTIVE = 0
} hm_15n_state_t;

typedef struct
{
    doutputModule_t hm15n;
    finputModule_t sensor;
    uint32_t errorInterval;
} hm15n_config_t;

typedef struct hm15n_internalHandler *hm15n_handle;

hm15n_handle hm15n_init(hm15n_config_t *hm);

void hm15n_update(uint32_t period);

void hm15n_state_set(hm15n_handle hm15n, hm_15n_state_t state);
hm_15n_state_t hm15n_state_get(hm15n_handle hm15n);
void hm15n_currentState_set(hm15n_handle hm15n, hm_15n_state_t value);

void hm15n_error_set(hm15n_handle hm15n, uint8_t state);
uint8_t hm15n_error_get(hm15n_handle hm15n);

#endif