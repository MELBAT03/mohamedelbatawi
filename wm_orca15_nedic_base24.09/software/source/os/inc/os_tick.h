#if !defined(OS_TICK_H)
#define OS_TICK_H 1

#include "def.h"

extern volatile uint32_t gu32_os_tick_timestamp;

void os_tick_init(uint32_t u32_tickPeriod);

void os_tick_update(void (*updateFn)(void), uint32_t tickPeriod);

uint32_t os_tick_millis_get(void);

#endif
