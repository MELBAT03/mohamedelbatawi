#ifndef DLS_35S_H
#define DLS_35S_H 1

#include "def.h"
#include "components/doutput_module/doutput_module.h"
#include "components/dinput_module/dinput_module.h"

#define DLS35S_SENSOR_ACTIVE   (0)
#define DLS35S_SENSOR_INACTIVE (1)

typedef enum
{
    DLS35S_INIT = 5,
    DLS35S_IDLE = 3,
    DLS35S_RESET = 4,
    DLS35S_PROCESSING = 2,
    DLS35S_ACTIVE = 1,
    DLS35S_INACTIVE = 0
} dls35s_state_t;

typedef struct
{
    doutputModule_t dls35s;
    dinputModule_t sensor;
    uint32_t errorInterval;
} dls35s_config_t;

void dls35s_init(dls35s_config_t *dls);

void dls35s_update(uint32_t period);

void dls35s_state_set(dls35s_state_t u8_state);
void dls35s_currentState_set(dls35s_state_t value);
void dls35s_error_set(uint8_t state);

dls35s_state_t dls35s_state_get(uint8_t id);
dls35s_state_t dls35s_currentState_get(void);
uint8_t dls35s_sensorCurrentState_get(void);
uint8_t dls35s_error_get(void);

#endif
