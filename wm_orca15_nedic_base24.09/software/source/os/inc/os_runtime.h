#ifndef OS_RUNTIME_H
#define OS_RUNTIME_H 1

#include "def.h"

void os_runTime_init(void);
void os_runTime_userTask_register(void (*p)(void), uint8_t repeated, uint32_t period);
void os_runTime_update(uint32_t period);

#endif
