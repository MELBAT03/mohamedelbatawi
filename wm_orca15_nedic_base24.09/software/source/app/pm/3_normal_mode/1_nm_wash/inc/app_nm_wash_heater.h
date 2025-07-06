#ifndef APP_NM_WASH_HEATER_H
#define APP_NM_WASH_HEATER_H

#include "def.h"

#include "app_nm_user_interface.h"

typedef enum app_wash_steamTech_heaterState
{
    HEATER_STEAM_TECH_REQUIRED_ON,
    HEATER_STEAM_TECH_REQUIRED_OFF
} app_wash_steamTech_heaterState_t;

void app_nm_wash_heater_process(uint32_t period);

app_wash_steamTech_heaterState_t app_nm_wash_heater_steamTechHeaterState_get(void);
void app_nm_wash_heater_steamTechHeaterState_set(app_wash_steamTech_heaterState_t state);

uint32_t app_nm_wash_heater_steamTechTargetONTemp_get(app_nm_userInterface_courseSelection_t course);
uint32_t app_nm_wash_heater_steamTechTargetOFFTemp_get(app_nm_userInterface_courseSelection_t course);

uint32_t app_nm_wash_heater_steamTechONTemp_get(void);
uint32_t app_nm_wash_heater_steamTechOFFTemp_get(void);

#endif
