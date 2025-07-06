#ifndef HANDLER_OFF_SW_H
#define HANDLER_OFF_SW_H

#include "def.h"

typedef enum handler_offSW_state
{
    HANDLER_OFF_SW_RELEASED,
    HANDLER_OFF_SW_PRESSED,
    HANDLER_OFF_SW_LONG_PRESSED
} handler_offSW_state_t;

void handler_offSW_init(void);
handler_offSW_state_t handler_offSW_state_get(void);
void handler_offSW_state_set(handler_offSW_state_t value);

#endif
