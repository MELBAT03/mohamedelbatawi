#ifndef OS_SYSTICK_HANDLER_H
#define OS_SYSTICK_HANDLER_H

#include "def.h"

void os_sysTickHandler_init(uint32_t tickPeriod);
void os_sysTickHandler_resume(void);

#endif
