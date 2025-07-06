#ifndef APP_UNBALANCE_H
#define APP_UNBALANCE_H

#include "def.h"

#include "handlers/input/inc/handler_mems.h"

typedef handler_mems_unbalanceState_t app_unbalance_state_t;

#define APP_UNBALANCE_DETECTED     (UNBALANCE_DETECTED)
#define APP_UNBALANCE_NOT_DETECTED (UNBALANCE_NOT_DETECTED)

app_unbalance_state_t app_unbalance_state_get(void);
void app_unbalance_reset(void);

void app_unbalance_detection_set(uint8_t value);
uint8_t app_unbalance_detection_get(void);

#endif
