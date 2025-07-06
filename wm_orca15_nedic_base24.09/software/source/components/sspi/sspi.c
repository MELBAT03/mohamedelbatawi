#include "sspi/sspi.h"
#include "gpio/gpio.h"
#include "board.h"
#include "mcal.h"
#include "os.h"

static void sspi_mode_set(uint8_t mode);
static void sspi_dummy_delay(uint32_t del);

uint8_t sspi_cke = 0;
uint8_t sspi_ckp = 0;

sspi_config_t gx_sspi_internalHandler;

#define BIT_ORDER_MSB(val) val = ((val & 0x01) << 7) | \
                                 ((val & 0x02) << 5) | \
                                 ((val & 0x04) << 3) | \
                                 ((val & 0x08) << 1) | \
                                 ((val & 0x10) >> 1) | \
                                 ((val & 0x20) >> 3) | \
                                 ((val & 0x40) >> 5) | \
                                 ((val & 0x80) >> 7)

void sspi_init(sspi_config_t *config)
{
    gx_sspi_internalHandler.sclPort = config->sclPort;
    gx_sspi_internalHandler.sclPin = config->sclPin;
    gx_sspi_internalHandler.mosiPort = config->mosiPort;
    gx_sspi_internalHandler.mosiPin = config->mosiPin;
    gx_sspi_internalHandler.misoPort = config->misoPort;
    gx_sspi_internalHandler.misoPin = config->misoPin;
    gx_sspi_internalHandler.csPort = config->csPort;
    gx_sspi_internalHandler.csPin = config->csPin;

    gx_sspi_internalHandler.mode = config->mode;
    sspi_mode_set(config->mode);

    gpio_pinIOState_set(gx_sspi_internalHandler.mosiPort, gx_sspi_internalHandler.mosiPin, GPIO_OUTPUT);
    gpio_pinIOState_set(gx_sspi_internalHandler.misoPort, gx_sspi_internalHandler.misoPin, GPIO_INPUT);
    gpio_pinIOState_set(gx_sspi_internalHandler.sclPort, gx_sspi_internalHandler.sclPin, GPIO_OUTPUT);
    gpio_pinIOState_set(gx_sspi_internalHandler.csPort, gx_sspi_internalHandler.csPin, GPIO_OUTPUT);

    gpio_pinState_set(gx_sspi_internalHandler.mosiPort, gx_sspi_internalHandler.mosiPin, GPIO_HIGH);
    gpio_pinState_set(gx_sspi_internalHandler.csPort, gx_sspi_internalHandler.csPin, GPIO_LOW);
    if (gx_sspi_internalHandler.mode == SPI_MODE_0 || gx_sspi_internalHandler.mode == SPI_MODE_1)
    {
        gpio_pinState_set(gx_sspi_internalHandler.sclPort, gx_sspi_internalHandler.sclPin, GPIO_LOW);
    }
    else
    {
        gpio_pinState_set(gx_sspi_internalHandler.sclPort, gx_sspi_internalHandler.sclPin, GPIO_HIGH);
    }

    gx_sspi_internalHandler.bitRate = config->bitRate;
    gx_sspi_internalHandler.dataOrder = config->dataOrder;
}

uint8_t sspi_data_transmit(uint8_t val)
{
    uint8_t del;
    volatile uint32_t bit;
    uint8_t u8a_buffer[8] = {0};
    uint8_t out = 0;
    uint8_t bval = 0;
    uint8_t sck;

    if (gx_sspi_internalHandler.dataOrder == SSPI_MSB_FIRST)
    {
        BIT_ORDER_MSB(val);
    }

    for (uint32_t j = 0; j < 8; j++)
    {
        u8a_buffer[j] = (val >> j) & 1;
    }

    del = 30;
    sck = sspi_ckp;

    for (bit = 0u; bit < 8u; bit++)
    {
        if (gx_sspi_internalHandler.mode == SPI_MODE_1 || gx_sspi_internalHandler.mode == SPI_MODE_3)
        {
            sck = !sck;
            gpio_pinState_set(gx_sspi_internalHandler.sclPort, gx_sspi_internalHandler.sclPin, sck);
            sspi_dummy_delay(del);
        }

        gpio_pinState_set(gx_sspi_internalHandler.mosiPort, gx_sspi_internalHandler.mosiPin, u8a_buffer[bit]);

        sck = !sck;
        gpio_pinState_set(gx_sspi_internalHandler.sclPort, gx_sspi_internalHandler.sclPin, sck);
        sspi_dummy_delay(del);

        bval = gpio_pinState_get(gx_sspi_internalHandler.misoPort, gx_sspi_internalHandler.misoPin);
        out = (out << 1) | bval;

        if (gx_sspi_internalHandler.mode == SPI_MODE_0 || gx_sspi_internalHandler.mode == SPI_MODE_2)
        {
            sck = !sck;
            gpio_pinState_set(gx_sspi_internalHandler.sclPort, gx_sspi_internalHandler.sclPin, sck);
            sspi_dummy_delay(del);
        }
    }

    return out;
}

void sspi_csState_set(uint8_t state)
{
    gpio_pinState_set(gx_sspi_internalHandler.csPort, gx_sspi_internalHandler.csPin, state);
}

static void sspi_mode_set(uint8_t mode)
{
    switch (mode)
    {
        case SPI_MODE_0:
            sspi_ckp = 0;
            sspi_cke = 0;
            break;

        case SPI_MODE_1:
            sspi_ckp = 0;
            sspi_cke = 1;
            break;

        case SPI_MODE_2:
            sspi_ckp = 1;
            sspi_cke = 0;
            break;

        case SPI_MODE_3:
            sspi_ckp = 1;
            sspi_cke = 1;
            break;

        default:
            // do nothing
            break;
    }
}

static void sspi_dummy_delay(uint32_t del)
{
    volatile uint32_t i = 0;

    for (i = 0; i < del; i++)
    {
        // do nothing
    }
}
