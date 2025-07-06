#ifndef HANDLER_ACIN_H
#define HANDLER_ACIN_H

#include "def.h"
#include "components/acin/acin.h"

typedef acin_state_t handler_acin_state_t;

#define HANDLER_NORMAL_VOLTAGE           ACIN_NORMAL_VOLTAGE
#define HANDLER_MAYBE_POWER_INTERRUPTION ACIN_MAYBE_POWER_INTERRUPTION
#define HANDLER_UNDER_VOLTAGE            ACIN_UNDER_VOLTAGE
#define HANDLER_DANGEROUS_UNDER_VOLTAGE  ACIN_DANGEROUS_UNDER_VOLTAGE

void handler_acin_init(void);

uint32_t handler_acin_value_get(void);
void handler_acin_value_set(uint32_t value);

handler_acin_state_t handler_acin_state_get(void);
void handler_acin_state_set(handler_acin_state_t value);

uint32_t handler_acin_defaultUnderVoltageValue_get(void);

#endif
