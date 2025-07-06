#ifndef APP_NM_ENDURANCE_H
#define APP_NM_ENDURANCE_H

#include "def.h"

void app_nm_endurance_restart(void);

void app_nm_endurance_entry_check(uint32_t period);
void app_nm_endurance_display_handle(void);

app_nm_userInterface_data_t *app_nm_endurance_ui_get(void);

uint16_t app_nm_endurance_cycleCnt_get(void);

#endif
