#ifndef TWI_SLAVE_H
#define TWI_SLAVE_H 1

#include "def.h"

void twi_slave_init(uint8_t slaveAddr);
uint8_t twi_slave_dataReadyFlag_get(void);
void twi_slave_data_set(uint8_t data);
uint8_t twi_slave_data_get(void);

#endif