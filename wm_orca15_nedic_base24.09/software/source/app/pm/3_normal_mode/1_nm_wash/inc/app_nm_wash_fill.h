#ifndef APP_NM_WASH_FILL_H
#define APP_NM_WASH_FILL_H

#include "def.h"

#include "app_user_interface.h"

typedef struct app_nm_wash_rinse_fillSoftnerWaterLevel
{
    uint8_t softnerWaterLevelLiter;
} app_nm_wash_rinse_fillSoftnerWaterLevel_t;

void app_nm_wash_fill_process(uint32_t period);

uint16_t app_nm_wash_fill_waterTempDuringWashFilling_get(void);

const app_nm_wash_rinse_fillSoftnerWaterLevel_t *app_nm_wash_fill_softnerWaterLevel_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel);
uint32_t app_nm_wash_fill_autoSoftenerTimeout_get(void);

#endif
