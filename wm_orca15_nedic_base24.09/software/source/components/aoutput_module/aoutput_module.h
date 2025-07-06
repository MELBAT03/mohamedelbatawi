#ifndef AOUTPUT_MODULE_H
#define AOUTPUT_MODULE_H

#include "def.h"
#include "mcal.h"
#include "gpio/gpio.h"

typedef struct
{
    mcal_dac_blkEnum_t *dacBlock;
    mcal_dac_chEnum_t dacCh;
    gpio_t gpio;   // in case the mcu requires the pins as gpio to be identified
    uint8_t dacDeviceID;
} aoutputModule_t;

void aoutputModule_init(aoutputModule_t *aoutput);
void aoutputModule_value_set(uint8_t u8_dacDeviceID, uint32_t value);
void aoutputModule_update(uint32_t period);

#endif
