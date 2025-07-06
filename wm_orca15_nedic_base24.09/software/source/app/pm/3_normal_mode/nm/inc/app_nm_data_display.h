#ifndef APP_NM_DATA_DISPLAY_H
#define APP_NM_DATA_DISPLAY_H

#include "def.h"
#include "app_nm_user_interface.h"

typedef enum app_nm_dataDisplay_state
{
    APP_NM_DATA_DISPLAY_STANDBY,
    APP_NM_DATA_DISPLAY_WATER_LEVEL_FREQ,
    APP_NM_DATA_DISPLAY_LOAD_SENSE_DATA,
    APP_NM_DATA_DISPLAY_MOTOR_RPM,
    APP_NM_DATA_DISPLAY_MEMS_X,
    APP_NM_DATA_DISPLAY_MEMS_Y,
    APP_NM_DATA_DISPLAY_MEMS_Z,
    APP_NM_DATA_DISPLAY_MEMS_UNBALANCE_X,
    APP_NM_DATA_DISPLAY_MEMS_UNBALANCE_Y,
    APP_NM_DATA_DISPLAY_MEMS_UNBALANCE_Z,
    APP_NM_DATA_DISPLAY_MEMS_NOTMAL_HITS,
    APP_NM_DATA_DISPLAY_MEMS_CRITICAL_HITS,
    APP_NM_DATA_DISPLAY_MEMS_CALIBRATED_X,
    APP_NM_DATA_DISPLAY_MEMS_CALIBRATED_Y,
    APP_NM_DATA_DISPLAY_MEMS_CALIBRATED_Z,
    APP_NM_DATA_DISPLAY_MEMS_FAULT_X,
    APP_NM_DATA_DISPLAY_MEMS_FAULT_Y,
    APP_NM_DATA_DISPLAY_MEMS_FAULT_Z,
    APP_NM_DATA_DISPLAY_ACIN,
    APP_NM_DATA_DISPLAY_INTERNAL_WATER_LEVEL,
    APP_NM_DATA_DISPLAY_CALIBRATED_WATER_LEVEL
} app_nm_dataDisplay_state_t;

void app_nm_userInterface_dataDisplay_handle(void);
void app_nm_display_dataDisplay_handle(uint32_t period);

app_nm_userInterface_state_t app_nm_userInterface_dataDisplay_entry_check(uint32_t period);
app_nm_userInterface_state_t app_nm_userInterface_dataDisplay_exit_check(void);

app_nm_dataDisplay_state_t app_nm_userInterface_dataDisplayState_get(void);

#endif
