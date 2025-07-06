#ifndef APP_ENTRY_STARTUP_H
#define APP_ENTRY_STARTUP_H

#include "def.h"

void app_entry_fastStartup_init(void);
void app_entry_startup_init(void);
uint32_t app_entry_startup_condition_check(void);

#endif
