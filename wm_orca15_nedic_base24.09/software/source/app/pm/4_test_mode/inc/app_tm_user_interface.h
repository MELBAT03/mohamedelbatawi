#ifndef APP_TM_USER_INTERFACE_H
#define APP_TM_USER_INTERFACE_H

#include "def.h"

#include "app_test_mode.h"

void app_tm_userInterface_process(uint32_t period);

void app_tm_userInterface_setup(void);

app_tm_keyConfig_t app_tm_userInterface_keyConfig_get(void);

#endif
