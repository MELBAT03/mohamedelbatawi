#ifndef HANDLER_DETERGENT_PUMP_H
#define HANDLER_DETERGENT_PUMP_H

#include "def.h"

typedef enum handler_detergentPump_state
{
    HANDLER_DETERGENT_PUMP_INACTIVE = 0,
    HANDLER_DETERGENT_PUMP_ACTIVE = 1
} handler_detergentPump_state_t;

void handler_detergentPump_init(void);
void handler_detergentPump_state_set(handler_detergentPump_state_t state);
handler_detergentPump_state_t handler_detergentPump_state_get(void);

#endif
