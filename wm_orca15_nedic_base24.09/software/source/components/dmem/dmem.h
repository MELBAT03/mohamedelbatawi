#ifndef DMEM_H
#define DMEM_H 1

#include "def.h"

void dmem_init(void);

uint8_t dmem_data_get(uint16_t recordNum);
uint8_t dmem_resetFlag_get(void);

uint8_t dmem_data_set(uint16_t recordNum, uint8_t data);

uint8_t dmem_reset(void);

#endif
