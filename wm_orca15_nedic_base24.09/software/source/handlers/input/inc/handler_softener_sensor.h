#ifndef HANDLER_SOFTENER_SENSOR_H
#define HANDLER_SOFTENER_SENSOR_H

#include "def.h"

#include "components/dinput_module/dinput_module.h"

typedef dinputModule_stateEnum_t handler_softenerSensor_state_t;
#define HANDLER_SOFTENER_LOW DINPUT_MODULE_PRESSED_SHORT_PRESS

void handler_softenerSensor_init(void);

handler_softenerSensor_state_t handler_softenerSensor_state_get(void);

void handler_softenerSensor_state_set(handler_softenerSensor_state_t value);

#endif
