#ifndef HANDLER_CLUTCH_HM15N_H
#define HANDLER_CLUTCH_HM15N_H

#include "def.h"
#include "components/hm15n/hm15n.h"

typedef enum handler_clutch_state
{
    HANDLER_CLUTCH_WITH_BASKET = HM15N_ACTIVE,
    HANDLER_CLUTCH_WITHOUT_BASKET = HM15N_INACTIVE,
    HANDLER_CLUTCH_PROCESSING = HM15N_PROCESSING
} handler_clutch_state_t;

void handler_clutch_init(void);

handler_clutch_state_t handler_clutch_switch(handler_clutch_state_t state);

uint8_t handler_clutch_error_get(void);
void handler_clutch_error_set(uint8_t state);

#endif
