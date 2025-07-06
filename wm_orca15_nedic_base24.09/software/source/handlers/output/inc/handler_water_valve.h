#ifndef HANDLER_WATER_VALVE_H
#define HANDLER_WATER_VALVE_H

#include "def.h"

typedef enum handler_waterValve_state
{
    WATER_VALVE_INACTIVE = 0,
    WATER_VALVE_ACTIVE = 1
} handler_waterValve_state_t;

void handler_waterValve_init(void);

void handler_coldWaterValve_state_set(handler_waterValve_state_t state);
void handler_hotWaterValve_state_set(handler_waterValve_state_t state);
void handler_softenerWaterValve_state_set(handler_waterValve_state_t state);
void handler_showerWaterValve_state_set(handler_waterValve_state_t state);
void handler_condenserWaterValve_state_set(handler_waterValve_state_t state);

handler_waterValve_state_t handler_coldWaterValve_state_get(void);
handler_waterValve_state_t handler_hotWaterValve_state_get(void);
handler_waterValve_state_t handler_softenerWaterValve_state_get(void);
handler_waterValve_state_t handler_showerWaterValve_state_get(void);
handler_waterValve_state_t handler_condenserWaterValve_state_get(void);

#endif
