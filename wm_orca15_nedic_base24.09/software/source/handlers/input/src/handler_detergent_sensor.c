#include "dinput_module/dinput_module.h"
#include "def.h"
#include "board.h"
#include "handlers/input/inc/handler_detergent_sensor.h"

static dinputModule_handle gx_detergentSensor;

void handler_detergentSensor_init(void)
{
    dinputModule_t detergentSensor;

    detergentSensor.type = DINPUT_GPIO_INPUT;
    detergentSensor.activeLevel = 1;
    detergentSensor.longPressMultiplier = UINT32_MAX;
    detergentSensor.gpio.port = DETERGENT_LIMIT_PORT;
    detergentSensor.gpio.pin = DETERGENT_LIMIT_PIN;
    detergentSensor.shortPressMultiplier = 4;
    gx_detergentSensor = dinputModule_init(&detergentSensor);
}

handler_detergentSensor_state_t handler_detergentSensor_state_get(void)
{
    handler_detergentSensor_state_t ret;

    ret = (handler_detergentSensor_state_t)dinputModule_state_get(gx_detergentSensor);

    return ret;
}

void handler_detergnetSensor_state_set(handler_detergentSensor_state_t value)
{
}
