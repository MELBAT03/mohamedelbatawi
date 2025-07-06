#ifndef TWI_EEPROM_H
#define TWI_EEPROM_H

#include "def.h"
#include "component_config.h"
#include "components/stwi/stwi.h"

void twieeprom_init(hwTimer_t *tm, twi_pinConfig_t *pins);
void twieeprom_data_set(uint32_t deviceAddr, uint8_t *pBuffer, uint16_t writeAddr, uint16_t numByteToWrite);
int8_t twieeprom_data_get(uint32_t deviceAddr, uint8_t *pBuffer, uint16_t readAddr, uint16_t numByteToRead);

#endif
