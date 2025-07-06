#include "def.h"
#include "components/twi_eeprom/twi_eeprom.h"
#include "board.h"

void handler_memory_init(void)
{
    twi_pinConfig_t eeprom_config = {
        .dataPort = TWI_DATA_PORT,
        .dataPin = TWI_DATA_PIN,
        .clockPort = TWI_CLOCK_PORT,
        .clockPin = TWI_CLOCK_PIN,
        .eepromAddr = EEPROM_ADDRESS,
        .memSize = MEMORY_SIZE};

    twieeprom_init(EEPROM_TIMER_MODULE, &eeprom_config);
}

// NOTE: total read takes around 10 msec
void handler_memory_data_get(uint8_t *buff, uint16_t readAddr, uint16_t numByteToRead)
{
    twieeprom_data_get(EEPROM_ADDRESS, buff, readAddr, numByteToRead);
}

// NOTE: 4 byte write takes around 22 msec & requires 10 msec for internal page write after sending stop (this ic works only with 4 byte max)
uint8_t handler_memory_data_set(uint8_t *buff, uint16_t writeAddr, uint16_t numByteToWrite)
{
    twieeprom_data_set(EEPROM_ADDRESS, buff, writeAddr, numByteToWrite);

    return 0;
}

uint8_t handler_memory_reset(void)
{
    return 0;
}

uint8_t handler_memory_resetState_get(void)
{
    return 0;
}