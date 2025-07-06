#include "dinput_module/dinput_module.h"
#include "def.h"
#include "board.h"
#include "handlers/input/inc/handler_lid_sensor.h"

#ifndef LID_CLOSE_PERMENENT
static dinputModule_handle gx_lidSensor;
#endif

void handler_lidSensor_init(void)
{
#ifndef LID_CLOSE_PERMENENT
    dinputModule_t lidSensor;

    lidSensor.type = DINPUT_GPIO_INPUT;
    lidSensor.activeLevel = 1;
    lidSensor.longPressMultiplier = UINT32_MAX;
    lidSensor.gpio.port = LID_SENSOR_PORT;
    lidSensor.gpio.pin = LID_SENSOR_PIN;
    lidSensor.shortPressMultiplier = 4;

    gx_lidSensor = dinputModule_init(&lidSensor);
#else
    // do nothing
#endif
}

handler_lidSensor_state_t handler_lidSensor_state_get(void)
{
    handler_lidSensor_state_t ret;

#ifndef LID_CLOSE_PERMENENT

    ret = (handler_lidSensor_state_t)dinputModule_state_get(gx_lidSensor);

#else
    ret = HANDLER_LID_SENSOR_CLOSED;
#endif

    return ret;
}

handler_lidSensor_state_t handler_lidSensor_edgeState_get(void)
{
    handler_lidSensor_state_t ret;

#ifndef LID_CLOSE_PERMENENT

    ret = dinputModule_edgeState_get(gx_lidSensor);

#else
    ret = HANDLER_LID_SENSOR_CLOSED;
#endif
    return ret;
}

void handler_lidSensor_state_set(handler_lidSensor_state_t value)
{
  // do nothing
}
