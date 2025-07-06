#ifndef APP_STANDBY_H
#define APP_STANDBY_H

#include "def.h"
#include "app_processor.h"

app_processor_state_t app_standby_process(uint32_t period);

void standby_nm_data_handle(void);

#endif
