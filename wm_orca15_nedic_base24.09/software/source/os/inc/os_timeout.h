#ifndef OS_TIMEOUT_H
#define OS_TIMEOUT_H

#include "def.h"

void os_timeout_start(uint32_t timeoutMSec);
uint8_t os_timeout_watch(void);

#endif
