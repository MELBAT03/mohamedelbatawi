#ifndef HANDLER_RELAY_H
#define HANDLER_RELAY_H

#include "def.h"

typedef enum handler_relay_state
{
    RELAY_ACTIVE = 1,
    RELAY_INACTIVE = 0
} handler_relay_state_t;

void handler_relay_init(void);

void handler_mainRelay_state_set(handler_relay_state_t state);
void handler_DDMRelay_state_set(handler_relay_state_t state);

#endif
