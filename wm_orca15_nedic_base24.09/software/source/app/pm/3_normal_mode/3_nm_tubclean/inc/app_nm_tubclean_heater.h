#ifndef APP_NM_TUBCLEAN_HEATER_H
#define APP_NM_TUBCLEAN_HEATER_H

#include "def.h"

typedef enum app_nm_tubclean_steamTech_heaterState
{
    TUBCLEAN_HEATER_STEAM_TECH_REQUIRED_ON,
    TUBCLEAN_HEATER_STEAM_TECH_REQUIRED_OFF
} app_nm_tubclean_steamTech_heaterState_t;

void app_nm_tubclean_heater_process(uint32_t period);

app_nm_tubclean_steamTech_heaterState_t app_nm_tubclean_heater_steamTechHeaterState_get(void);
void app_nm_tubclean_heater_steamTechHeaterState_set(app_nm_tubclean_steamTech_heaterState_t state);

uint32_t app_nm_tubclean_heater_steamTechTargetONTemp_get(void);
uint32_t app_nm_tubclean_heater_steamTechTargetOFFTemp_get(void);

#endif
