#ifndef QUAD_ENCODER_H
#define QUAD_ENCODER_H

#include "gpio/gpio.h"
#include "def.h"

typedef struct
{
    gpio_t gpio;
    gpio_t extIntSource;
} qDecoderConfig_t;

void qDecoder_init(qDecoderConfig_t *config);
uint32_t qDecoder_value_get(void);
uint8_t qDecoder_dir_get(void);
void qDecoder_dataCallback_process(void);
void qDecoder_update(uint32_t period);

#endif