#ifndef HANDLER_MEMORY_H
#define HANDLER_MEMORY_H 1

#include "def.h"

void handler_memory_init(void);

void handler_memory_data_get(uint8_t *buff, uint16_t readAddr, uint16_t numByteToRead);
uint8_t handler_memory_resetState_get(void);

uint8_t handler_memory_data_set(uint8_t *buff, uint16_t writeAddr, uint16_t numByteToWrite);

uint8_t handler_memory_reset(void);

#endif