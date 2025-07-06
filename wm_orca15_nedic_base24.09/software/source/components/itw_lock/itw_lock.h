#ifndef ITW_LOCK_H
#define ITW_LOCK_H

#include "def.h"
#include "components/doutput_module/doutput_module.h"
#include "components/dinput_module/dinput_module.h"

#define ITW_LOCK_ON  (1)
#define ITW_LOCK_OFF (0)

typedef enum
{
    ITW_LOCK_INIT = 2,
    ITW_LOCK_PROCESSING = 3,
    ITW_LOCK_ACTIVE = 1,
    ITW_LOCK_INACTIVE = 0,
    ITW_LOCK_TRANSITION = 4
} itwLock_state_t;

typedef struct
{
    doutputModule_t lock;
    dinputModule_t sensor;
    uint32_t errorInterval;
} itwLock_config_t;

void itwLock_init(itwLock_config_t *dls);

void itwLock_update(uint32_t period);

void itwLock_state_set(itwLock_state_t state);
void itwLock_currentState_set(itwLock_state_t value);
void itwLock_error_set(uint8_t state);

uint8_t itwLock_state_get(uint8_t id);
itwLock_state_t itwLock_currentState_get(void);
uint8_t itwLock_sensorCurrentState_get(void);
uint8_t itwLock_error_get(void);

#endif
