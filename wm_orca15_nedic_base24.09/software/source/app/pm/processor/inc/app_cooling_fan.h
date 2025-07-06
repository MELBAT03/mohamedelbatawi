#ifndef APP_COOLING_FAN_H
#define APP_COOLING_FAN_H

#include "def.h"

#include "handlers/output/inc/handler_cooling_fan.h"

typedef handler_coolingFan_state_t app_coolingFan_state_t;
#define APP_COOLING_FAN_OFF        (HANDLER_COOLING_FAN_OFF)
#define APP_COOLING_FAN_LOW_SPEED  (HANDLER_COOLING_FAN_LOW_SPEED)
#define APP_COOLING_FAN_HIGH_SPEED (HANDLER_COOLING_FAN_HIGH_SPEED)

void app_coolingFan_update(uint32_t period);

void app_coolingFan_state_set(app_coolingFan_state_t state);
uint32_t app_coolingFan_speed_get(void);

#endif
