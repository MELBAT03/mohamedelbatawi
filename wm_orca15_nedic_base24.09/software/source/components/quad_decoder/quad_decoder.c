#include "gpio/gpio.h"
#include "core/atmega32/inc/atmega32.h"
#include "quad_decoder/quad_decoder.h"

static volatile uint8_t gu32_quadDecoder_direction;
static volatile uint8_t gu32_quadDecoder_prevDirection;
static volatile uint32_t gu32_quadDecoder_counter;

void qDecoder_init(qDecoderConfig_t *config)
{
    gu32_quadDecoder_counter = 0;
    gu32_quadDecoder_direction = 0;
    gu32_quadDecoder_prevDirection = 0;

    EXINT1_ON_RISING_EDGE_SET();
    EXINT1_INT_ENABLE();

    DDRD &= ~(1 << 4);
}

void qDecoder_update(uint32_t period)
{
    // timeout 500 msec, after which clear the buffer

    static uint32_t timeoutInterval = 0;
    static uint32_t prevReading = 0;

    if (gu32_quadDecoder_counter != prevReading)
    {
        timeoutInterval = 0;
    }
    else
    {
        timeoutInterval += 20;
    }

    if (timeoutInterval >= 500)
    {
        gu32_quadDecoder_counter = 0;
        timeoutInterval = 0;
    }

    prevReading = gu32_quadDecoder_counter;
}

uint32_t qDecoder_value_get(void)
{
    return gu32_quadDecoder_counter;
}

uint8_t qDecoder_dir_get(void)
{
    return gu32_quadDecoder_direction;
}

void qDecoder_dataCallback_process(void)
{
    if (PIND & (1 << 4))
    {
        gu32_quadDecoder_direction = 1;
    }
    else
    {
        gu32_quadDecoder_direction = 0;
    }
    if (gu32_quadDecoder_direction != gu32_quadDecoder_prevDirection)
        gu32_quadDecoder_counter = 0;

    gu32_quadDecoder_counter++;
    gu32_quadDecoder_prevDirection = gu32_quadDecoder_direction;
}
