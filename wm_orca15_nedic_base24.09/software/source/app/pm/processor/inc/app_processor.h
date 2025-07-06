#ifndef APP_PROCESSOR_H
#define APP_PROCESSOR_H

#include "def.h"

#include "app_memory.h"

typedef enum app_processor_state
{
    APP_PROCESSOR_STARTUP_MODE = 0,
    APP_PROCESSOR_SETUP_MODE = 1,
    APP_PROCESSOR_STANDBY_MODE = 2,
    APP_PROCESSOR_NORMAL_MODE = 3,
    APP_PROCESSOR_TEST_MODE = 4,
    APP_PROCESSOR_FAULT_MODE = 5,
    APP_PROCESSOR_FINISH_MODE = 6,
    APP_PROCESSOR_DONE_MODE = 7
} app_processor_state_t;

void app_processor_update(uint32_t period);

uint8_t app_processor_entryFlag_get(void);

app_processor_state_t app_processor_state_get(void);
void app_processor_state_set(app_processor_state_t value);

app_processor_state_t app_processor_previousState_get(void);

app_memory_shutdownState_t app_processor_shutdownState_get(void);
void app_processor_shutdownState_set(app_memory_shutdownState_t state);

#endif
