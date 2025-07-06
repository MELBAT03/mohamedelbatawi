#include "dinput_module/dinput_module.h"
#include "def.h"
#include "board.h"
#include "handlers/input/inc/handler_softener_sensor.h"

static dinputModule_handle gx_softenerSensor;

void handler_softenerSensor_init(void)
{
    dinputModule_t softenerSensor;

    softenerSensor.type = DINPUT_GPIO_INPUT;
    softenerSensor.activeLevel = 1;
    softenerSensor.longPressMultiplier = UINT32_MAX;
    softenerSensor.gpio.port = SOFTNER_LIMIT_PORT;
    softenerSensor.gpio.pin = SOFTNER_LIMIT_PIN;
    softenerSensor.shortPressMultiplier = 4;
    gx_softenerSensor = dinputModule_init(&softenerSensor);
}

handler_softenerSensor_state_t handler_softenerSensor_state_get(void)
{
    handler_softenerSensor_state_t ret;

    ret = (handler_softenerSensor_state_t)dinputModule_state_get(gx_softenerSensor);

    return ret;
}

void handler_softenerSensor_state_set(handler_softenerSensor_state_t value)
{
}

