#ifndef HANDLER_DETERGENT_SENSOR_H
#define HANDLER_DETERGENT_SENSOR_H

#include "def.h"

#include "components/dinput_module/dinput_module.h"

typedef dinputModule_stateEnum_t handler_detergentSensor_state_t;
#define HANDLER_DETERGENT_LOW DINPUT_MODULE_PRESSED_SHORT_PRESS

void handler_detergentSensor_init(void);

handler_detergentSensor_state_t handler_detergentSensor_state_get(void);

void handler_detergentSensor_state_set(handler_detergentSensor_state_t value);

#endif
