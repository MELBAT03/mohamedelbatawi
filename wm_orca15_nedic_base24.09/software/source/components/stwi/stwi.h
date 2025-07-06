#ifndef _STWI_H
#define _STWI_H

#include "mcal.h"
#include "gpio/gpio.h"
#include "components/hw_timer/hw_timer.h"

#define TWI_CLOCK_FREQ_100K (100000ul)

typedef struct
{
    gpio_port_t dataPort;
    gpio_port_t clockPort;
    gpio_pin_t dataPin;
    gpio_pin_t clockPin;
    uint8_t eepromAddr;
    uint16_t memSize;
} twi_pinConfig_t;

/*Define SCL Frequency*/
#define SCL_FREQUENCY_100KHz (7)
#define SCL_FREQUENCY_300KHz (2)

// #define TWI_MASTER_WRITE_SET(addr) ((addr) & ~(1 << 0))
// #define TWI_MASTER_READ_SET(addr) ((addr) | 0x01)
#define TWI_WRITE_SET(addr) ((addr) & ~(1 << 0))
#define TWI_READ_SET(addr) ((addr) | 0x01)

#define TWI_ACK_SET 1
#define TWI_ACK_CLR 0

void twi_init(hwTimer_t *tm, twi_pinConfig_t *px_pinConfig, uint32_t u32_freq);
void twi_start(void);
void twi_stop(void);
uint8_t twi_write(uint8_t data);
uint8_t twi_read(uint8_t ack);
uint8_t twi_status(void);

#endif
