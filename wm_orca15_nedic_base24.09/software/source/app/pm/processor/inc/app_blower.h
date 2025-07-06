#ifndef APP_BLOWER_H
#define APP_BLOWER_H

#include "def.h"
#include "handlers/output/inc/handler_blower.h"

typedef handler_blower_power_t app_blower_power_t;
#define APP_BLOWER_OFF      (HANDLER_BLOWER_OFF)
#define APP_BLOWER_POWER_10 (HANDLER_BLOWER_POWER_10)
#define APP_BLOWER_POWER_20 (HANDLER_BLOWER_POWER_20)
#define APP_BLOWER_POWER_30 (HANDLER_BLOWER_POWER_30)
#define APP_BLOWER_POWER_40 (HANDLER_BLOWER_POWER_40)
#define APP_BLOWER_POWER_50 (HANDLER_BLOWER_POWER_50)
#define APP_BLOWER_POWER_60 (HANDLER_BLOWER_POWER_60)
#define APP_BLOWER_POWER_70 (HANDLER_BLOWER_POWER_70)
#define APP_BLOWER_POWER_80 (HANDLER_BLOWER_POWER_80)
#define APP_BLOWER_POWER_90 (HANDLER_BLOWER_POWER_90)
#define APP_BLOWER_ON       (HANDLER_BLOWER_ON)

#ifndef BLOWER_TARGET_POWER
#define BLOWER_TARGET_POWER (APP_BLOWER_POWER_60)
#endif

void app_blower_update(uint32_t period);

void app_blower_speedFeedback_set(uint32_t value);
uint32_t app_blower_speedFeedback_get(void);

void app_blower_speedControl_set(app_blower_power_t power);

uint8_t app_blower_error_get(void);

void app_blower_error_set(uint8_t value);

#endif
