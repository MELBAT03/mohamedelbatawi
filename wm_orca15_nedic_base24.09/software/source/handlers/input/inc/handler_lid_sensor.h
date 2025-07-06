#ifndef HANDLER_LID_SENSOR_H
#define HANDLER_LID_SENSOR_H

#include "def.h"

#include "components/dinput_module/dinput_module.h"

typedef dinputModule_stateEnum_t handler_lidSensor_state_t;
#define HANDLER_LID_SENSOR_CLOSED DINPUT_MODULE_PRESSED_SHORT_PRESS
#define HANDLER_LID_SENSOR_OPENED DINPUT_MODULE_RELEASED

void handler_lidSensor_init(void);
handler_lidSensor_state_t handler_lidSensor_state_get(void);
handler_lidSensor_state_t handler_lidSensor_edgeState_get(void);

void handler_lidSensor_state_set(handler_lidSensor_state_t value);

#endif
