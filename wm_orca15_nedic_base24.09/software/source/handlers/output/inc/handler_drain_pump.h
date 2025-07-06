#ifndef HANDLER_DRAIN_PUMP_H
#define HANDLER_DRAIN_PUMP_H 1

#include "def.h"

typedef enum handler_drainPump_state
{
    HANDLER_DRAIN_PUMP_INACTIVE = 0,
    HANDLER_DRAIN_PUMP_ACTIVE = 1
} handler_drainPump_state_t;

void handler_drainPump_init(void);
void handler_drainPump_state_set(handler_drainPump_state_t state);
handler_drainPump_state_t handler_drainPump_state_get(void);

#endif
