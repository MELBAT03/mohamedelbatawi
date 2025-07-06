#ifndef SSPI_H
#define SSPI_H 1

#include "mcal.h"
#include "gpio/gpio.h"

/*******************************************************************************************/
/*
 * MODE_0: CPOL=0, CPHA=0 - The first data bit is sampled by the receiver on the first rising SCK edge.
 * MODE_1: CPOL=0, CPHA=1 - The first data bit is sampled by the receiver on the second rising SCK edge.
 * MODE_2: CPOL=1, CPHA=0 - The first data bit is sampled by the receiver on the first falling SCK edge.
 * MODE_3: CPOL=1, CPHA=1 - The first data bit is sampled by the receiver on the second falling SCK edge.
 */
/*******************************************************************************************/

typedef enum
{
    SPI_MODE_0 = 0U,
    SPI_MODE_1 = 1U,
    SPI_MODE_2 = 2U,
    SPI_MODE_3 = 3U
} sspi_mode_t;

typedef enum
{
    SSPI_MSB_FIRST = 1,
    SSPI_LSB_FIRST = 2
} sspi_dataOrder_t;

// typedef enum
// {
//     SPI_CLOCK_DIV2 = 0U,
//     SPI_CLOCK_DIV4 = 1U,
//     SPI_CLOCK_DIV8 = 2U,
//     SPI_CLOCK_DIV16 = 3U,
//     SPI_CLOCK_DIV32 = 4U,
//     SPI_CLOCK_DIV64 = 5U,
//     SPI_CLOCK_DIV128 = 6U
// } sspi_clockDiv_t;

typedef struct
{
    gpio_port_t sclPort;
    gpio_pin_t sclPin;
    gpio_port_t mosiPort;
    gpio_pin_t mosiPin;
    gpio_port_t misoPort;
    gpio_pin_t misoPin;
    gpio_port_t csPort;
    gpio_pin_t csPin;
    sspi_mode_t mode;
    uint32_t bitRate;
    sspi_dataOrder_t dataOrder;
} sspi_config_t;

void sspi_init(sspi_config_t *config);
uint8_t sspi_data_transmit(uint8_t);
void sspi_csState_set(uint8_t state);

#endif
