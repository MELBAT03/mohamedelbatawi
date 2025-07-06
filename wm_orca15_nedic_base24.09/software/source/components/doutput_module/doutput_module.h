#ifndef DOUTPUT_MODULE_H
#define DOUTPUT_MODULE_H

#include "gpio/gpio.h"
#include "def.h"

typedef struct doutputModule_internalHandler *doutputModule_handle;

typedef struct doutputModule_t
{
    gpio_t gpio;
} doutputModule_t;

doutputModule_handle doutputModule_init(doutputModule_t *doutputModule);
ret_t doutputModule_state_set(doutputModule_handle doutputModule, gpio_state_t state);
gpio_state_t doutputModule_state_get(doutputModule_handle doutputModule);
void doutputModule_update(uint32_t period);

// :NOTE: for testing purposes only
doutputModule_t *doutputModule_internalData_get(doutputModule_handle doutputModule);
void doutputModule_reset(void);

#endif
