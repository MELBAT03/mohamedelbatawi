#ifndef AINPUT_MODULE_H
#define AINPUT_MODULE_H

#include "mcal.h"
#include "def.h"
#include "gpio/gpio.h"

typedef struct ainputModule_internalHandler *ainputModule_handle;

typedef struct
{
    mcal_adc_blkEnum_t adcBlock;
    mcal_adc_chEnum_t adcCh;
    gpio_t gpio;   // in case the mcu requires the pins as gpio to be identified
} ainputModule_t;

ainputModule_handle ainputModule_init(ainputModule_t *ainput);

void ainputModule_update(uint32_t period);

uint32_t ainputModule_value_get(ainputModule_handle ainputModule);

ret_t ainputModule_value_set(ainputModule_handle ainputModule, uint32_t value);

#endif
