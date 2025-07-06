#ifndef APP_HEATER_H
#define APP_HEATER_H

#include "def.h"

#include "handlers/output/inc/handler_heater.h"

typedef handler_heater_state_t app_heater_plateState_t;
#define APP_HEATER_OFF        (HANDLER_HEATER_OFF)
#define APP_HEATER_ONE_PLATE  (HANDLER_HEATER_PLATE_1)
#define APP_HEATER_TWO_PLATES (HANDLER_HEATER_PLATE_2)

void app_heater_update(uint32_t period);

uint8_t app_heater_heaterOnWhileBlowerOffFault_get(void);
uint8_t app_heater_overCurrentFault_get(void);
uint8_t app_heater_notOperatingFault_get(void);
uint8_t app_heater_shouldntOperatingFault_get(void);
uint8_t app_heater_tempSensorFault_get(void);

app_heater_plateState_t app_heater_plateState_get(void);
void app_heater_plateState_set(app_heater_plateState_t state);
uint32_t app_heater_temp_get(void);
void app_heater_temp_set(uint32_t value);
double app_heater_currentFeedback_get(void);
void app_heater_currentFeedback_set(float value);

#endif
