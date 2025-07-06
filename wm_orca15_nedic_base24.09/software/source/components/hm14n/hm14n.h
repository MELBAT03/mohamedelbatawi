#ifndef HM_14N_H
#define HM_14N_H 1

#include "def.h"
#include "components/doutput_module/doutput_module.h"
#include "components/finput_module/finput_module.h"

typedef enum
{
    HM14N_IDLE = 3,
    HM14N_RESET = 4,
    HM14N_PROCESSING = 2,
    HM14N_ACTIVE = 1,
    HM14N_INACTIVE = 0
} hm_14n_state_t;

typedef struct
{
    doutputModule_t hm14n;
} hm14n_config_t;

typedef struct hm14n_internalHandler *hm14n_handle;

hm14n_handle hm14n_init(hm14n_config_t *hm);

void hm14n_update(uint32_t period);

void hm14n_state_set(hm14n_handle hm14n, hm_14n_state_t state);
hm_14n_state_t hm14n_state_get(hm14n_handle hm14n);
void hm14n_currentState_set(hm14n_handle hm14n, hm_14n_state_t value);

uint8_t hm14n_error_get(hm14n_handle hm14n);
void hm14n_error_set(hm14n_handle hm14n, uint8_t state);

#endif
