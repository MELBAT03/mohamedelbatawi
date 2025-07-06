#ifndef APP_CONDENSER_H
#define APP_CONDENSER_H

#include "def.h"
#include "handlers/output/inc/handler_water_valve.h"

typedef handler_waterValve_state_t app_condenser_state_t;
#define APP_CONDENSER_OFF WATER_VALVE_INACTIVE
#define APP_CONDENSER_ON  WATER_VALVE_ACTIVE

void app_condenser_update(uint32_t period);

void app_condenser_state_set(app_condenser_state_t state);
uint32_t app_condenser_state_get(void);

#endif
