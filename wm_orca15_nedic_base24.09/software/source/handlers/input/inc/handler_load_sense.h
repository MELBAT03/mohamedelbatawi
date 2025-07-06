#ifndef HANDLER_LOAD_SENSE_H
#define HANDLER_LOAD_SENSE_H 1

#include "def.h"

void handler_loadSense_init(void);

uint32_t handler_loadSense_weightG_get(void);
uint32_t handler_loadSense_weightRaw_get(void);
ret_t handler_loadSense_resetLoad_check(uint32_t reference);

void handler_loadSense_weightG_set(uint32_t value);

#endif