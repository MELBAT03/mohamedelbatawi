#ifndef HANDLER_SOFTENER_PUMP_H
#define HANDLER_SOFTENER_PUMP_H

#include "def.h"

typedef enum handler_softenerPump_state
{
    HANDLER_SOFTENER_PUMP_INACTIVE = 0,
    HANDLER_SOFTENER_PUMP_ACTIVE = 1
} handler_softenerPump_state_t;

void handler_softenerPump_init(void);
void handler_softenerPump_state_set(handler_softenerPump_state_t state);
handler_softenerPump_state_t handler_softenerPump_state_get(void);

#endif
