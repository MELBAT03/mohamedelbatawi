#ifndef ACIN_H
#define ACIN_H

#include "def.h"
#include "components/ainput_module/ainput_module.h"

typedef enum acin_state
{
    ACIN_STARTUP_TIMEOUT,
    ACIN_NORMAL_VOLTAGE,
    ACIN_MAYBE_POWER_DIPPING,
    ACIN_UNDER_VOLTAGE,
    ACIN_DANGEROUS_UNDER_VOLTAGE
} acin_state_t;

typedef struct acinConfig
{
    ainputModule_t ain;
    mcal_timer_t *tm;   // if zcd on time is used for voltage measurements
    uint32_t underVoltageValue;
    uint32_t underVoltageErrorTimeoutMSec;
    uint8_t (*relation)(uint32_t measurement, uint32_t ref);
} acinConfig_t;

void acin_init(acinConfig_t *config);

void acin_update(uint32_t period);

uint32_t acin_value_get(void);
acin_state_t acin_state_get(void);

void acin_calculation_start(void);
void acin_calculation_record(void);

#endif
