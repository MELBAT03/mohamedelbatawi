#ifndef EEPROM_H
#define EEPROM_H

#include "def.h"
#include "components/stwi/stwi.h"

#define EEPROM_ADDRESS (0b10100000)
#define EEPROM_DATA_PORT GPIO_PORTA
#define EEPROM_DATA_PIN GPIO_PIN3

#define EEPROM_CLOCK_PORT GPIO_PORTA
#define EEPROM_CLOCK_PIN GPIO_PIN1

void eeprom_init(twi_pinConfig_t *pins);
void eeprom_data_set(uint16_t devAddr, uint8_t *pBuffer, uint16_t startAddr, uint16_t numByte);
int8_t eeprom_data_get(uint16_t devAddr, uint8_t *pBuffer, uint16_t startAddr, uint16_t numByte);

void eeprom_update(uint32_t period);
#endif