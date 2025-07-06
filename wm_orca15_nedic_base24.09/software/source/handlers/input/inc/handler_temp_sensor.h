#ifndef HANDLER_TEMP_SENSOR_H
#define HANDLER_TEMP_SENSOR_H

#include "def.h"
#include "component_config.h"

void handler_tempSensor_init(void);

uint16_t handler_waterTempSensor_temp_get(void);
uint16_t handler_ambientTempSensor_temp_get(void);
uint32_t handler_heaterTempSensor_temp_get(void);
uint16_t handler_airBTempSensor_temp_get(void);

uint8_t handler_waterTempSensor_error_get(void);
uint8_t handler_ambientTempSensor_error_get(void);
uint8_t handler_airATempSensor_error_get(void);
uint8_t handler_airBTempSensor_error_get(void);

#endif
