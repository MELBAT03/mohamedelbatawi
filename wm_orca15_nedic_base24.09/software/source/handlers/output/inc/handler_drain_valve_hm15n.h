#ifndef HANDLER_DRAIN_VALVE_HM15N_H
#define HANDLER_DRAIN_VALVE_HM15N_H

#include "def.h"
#include "components/hm15n/hm15n.h"

typedef enum handler_drainValve_state
{
    HANDLER_DRAIN_VALVE_OPEN = HM15N_INACTIVE,
    HANDLER_DRAIN_VALVE_CLOSE = HM15N_ACTIVE,
    HANDLER_DRAIN_VALVE_PROCESSING = HM15N_PROCESSING
} handler_drainValve_state_t;

void handler_drainValve_init(void);

handler_drainValve_state_t handler_drainValve_switch(handler_drainValve_state_t state);

handler_drainValve_state_t handler_drainValve_state_get(void);

uint8_t handler_drainValve_error_get(void);
void handler_drainValve_error_set(uint8_t state);

#endif
