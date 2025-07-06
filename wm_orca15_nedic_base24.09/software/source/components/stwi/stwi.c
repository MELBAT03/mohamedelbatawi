// :NOTE: this driver requires external pull ups

#include "stwi/stwi.h"
#include "hw_timer/hw_timer.h"
#include "timer_sandwich/timer_sandwich.h"
#include "gpio/gpio.h"
#include "board.h"
#include "mcal.h"

typedef enum
{
    TWI_MASTER_WRITE_MODE,
    TWI_MASTER_READ_MODE,
    TWI_SLAVE_WRITE_MODE,
    TWI_SLAVE_READ_MODE
} twi_stateEnum_t;

gpio_t gx_stwiDataConfig, gx_stwiClockConfig;
uint32_t gu32_twi_freq;

static void twi_mode_set(twi_stateEnum_t x_mode);

#define SDA_CLEAR() (gpio_pinState_set(gx_stwiDataConfig.port, gx_stwiDataConfig.pin, GPIO_LOW))
#define SDA_SET()   (gpio_pinState_set(gx_stwiDataConfig.port, gx_stwiDataConfig.pin, GPIO_HIGH))
#define SCL_CLEAR() (gpio_pinState_set(gx_stwiClockConfig.port, gx_stwiClockConfig.pin, GPIO_LOW))
#define SCL_SET()   (gpio_pinState_set(gx_stwiClockConfig.port, gx_stwiClockConfig.pin, GPIO_HIGH))

void twi_init(hwTimer_t *tm, twi_pinConfig_t *px_pinConfig, uint32_t u32_freq)
{
    SDA_SET();
    SCL_SET();

    // :NOTE: make sure to set the values before the direction as in some cases, setting the direction before the value to be output.
    //      causes the outupt to flicker to low before going to high again, this will cause an issue with the first start condition.
    gx_stwiDataConfig.port = px_pinConfig->dataPort;
    gx_stwiDataConfig.pin = px_pinConfig->dataPin;
    gx_stwiDataConfig.ioState = GPIO_OUTPUT;
    gx_stwiDataConfig.state = GPIO_HIGH;

    gx_stwiClockConfig.port = px_pinConfig->clockPort;
    gx_stwiClockConfig.pin = px_pinConfig->clockPin;
    gx_stwiClockConfig.ioState = GPIO_OUTPUT;
    gx_stwiClockConfig.state = GPIO_HIGH;

    if (u32_freq <= 100000)
    {
        gu32_twi_freq = SCL_FREQUENCY_100KHz;
    }
    else if (u32_freq > 100000)
    {
        gu32_twi_freq = SCL_FREQUENCY_300KHz;
    }

    gpio_pin_init(&gx_stwiDataConfig);
    gpio_pin_init(&gx_stwiClockConfig);

    twi_mode_set(TWI_MASTER_WRITE_MODE);

    timerSandwich_init(tm);
    timerSandwich_timeUS_set(tm, gu32_twi_freq);
}

void twi_start(void)
{
    SDA_SET();
    SCL_SET();

    timerSandwich_timeUS_start(NULL);
    SDA_CLEAR();
    timerSandwich_timeout_wait(NULL);

    timerSandwich_timeUS_start(NULL);
    SCL_CLEAR();
    timerSandwich_timeout_wait(NULL);
}

void twi_stop(void)
{
    timerSandwich_timeUS_start(NULL);
    SDA_CLEAR();
    timerSandwich_timeout_wait(NULL);

    timerSandwich_timeUS_start(NULL);
    SCL_SET();
    timerSandwich_timeout_wait(NULL);

    timerSandwich_timeUS_start(NULL);
    SDA_SET();
    timerSandwich_timeout_wait(NULL);
}

static void twi_mode_set(twi_stateEnum_t x_mode)
{
    switch (x_mode)
    {
        case TWI_MASTER_WRITE_MODE:
        {
            gpio_pinIOState_set(gx_stwiDataConfig.port, gx_stwiDataConfig.pin, GPIO_OUTPUT);
        }
        break;

        case TWI_MASTER_READ_MODE:
        {
            gpio_pinIOState_set(gx_stwiDataConfig.port, gx_stwiDataConfig.pin, GPIO_INPUT);
        }
        break;

        case TWI_SLAVE_WRITE_MODE:
        case TWI_SLAVE_READ_MODE:
        {
            // do nothing
        }
        break;
    }
    return;
}

// change the pin state before changing its direction.
uint8_t twi_write(uint8_t data)
{
    uint8_t u8_ackBit = 0;
    uint8_t u8a_buffer[8] = {0};

    for (uint8_t j = 0; j < 8; j++)
    {
        u8a_buffer[j] = (data >> (7 - j)) & 1;
    }

    /* timerSandwich_timeUS_start(NULL);
    SCL_CLEAR();
    timerSandwich_timeout_wait(NULL); */

    for (uint8_t i = 0; i < 8;)
    {
        timerSandwich_timeUS_start(NULL);
        gpio_pinState_set(gx_stwiDataConfig.port, gx_stwiDataConfig.pin, u8a_buffer[i]);
        SCL_SET();
        timerSandwich_timeout_wait(NULL);

        timerSandwich_timeUS_start(NULL);
        SCL_CLEAR();
        i++;
        timerSandwich_timeout_wait(NULL);
    }

    twi_mode_set(TWI_MASTER_READ_MODE);

    timerSandwich_timeUS_start(NULL);
    SCL_SET();
    timerSandwich_timeout_wait(NULL);

    u8_ackBit = gpio_pinState_get(gx_stwiDataConfig.port, gx_stwiDataConfig.pin);

    timerSandwich_timeUS_start(NULL);
    SCL_CLEAR();
    timerSandwich_timeout_wait(NULL);

    twi_mode_set(TWI_MASTER_WRITE_MODE);
    timerSandwich_timeUS_start(NULL);
    timerSandwich_timeout_wait(NULL);
    timerSandwich_timeUS_start(NULL);
    timerSandwich_timeout_wait(NULL);

    return u8_ackBit;
}

uint8_t twi_read(uint8_t ack)
{
    uint8_t ret_data = 0;

    // :NOTE: Must enable the pullup here as the slave is working with standard i2c
    twi_mode_set(TWI_MASTER_READ_MODE);

    // timerSandwich_timeUS_start(NULL);
    SCL_CLEAR();
    // timerSandwich_timeout_wait(NULL);

    for (uint8_t i = 0; i < 8;)
    {
        timerSandwich_timeUS_start(NULL);
        SCL_SET();
        ret_data = (ret_data << 1) | gpio_pinState_get(gx_stwiDataConfig.port, gx_stwiDataConfig.pin);
        timerSandwich_timeout_wait(NULL);

        timerSandwich_timeUS_start(NULL);
        SCL_CLEAR();
        i++;
        timerSandwich_timeout_wait(NULL);
    }

    // Sending ACK bit
    twi_mode_set(TWI_MASTER_WRITE_MODE);

    if (ack)
    {
        SDA_CLEAR();
    }
    else
    {
        SDA_SET();
    }

    timerSandwich_timeUS_start(NULL);
    SCL_SET();
    timerSandwich_timeout_wait(NULL);

    timerSandwich_timeUS_start(NULL);
    SCL_CLEAR();
    timerSandwich_timeout_wait(NULL);

    timerSandwich_timeUS_start(NULL);
    timerSandwich_timeout_wait(NULL);
    timerSandwich_timeUS_start(NULL);
    timerSandwich_timeout_wait(NULL);

    return ret_data;
}

uint8_t twi_status(void)
{
    return 0;
}
