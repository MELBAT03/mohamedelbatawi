#include "component_config.h"
#include "def.h"
#include "doutput_module/doutput_module.h"

#include "os.h"

#define DOUTPUT_MODULE_MARK (0xD0U)

typedef struct doutputModule_internalHandler
{
    uint8_t mark;
    gpio_t gpio;
} doutputModule_internalHandler_t;

static doutputModule_internalHandler_t gax_doutputModule_internalHandler[DOUTPUT_MODULE_DEVICE_NUM];

static uint8_t gu8_handlerItemsCounter = 0;

// TODO: add checking for the input parameters
doutputModule_handle doutputModule_init(doutputModule_t *doutputModule)
{
    doutputModule_handle handler = NULL;

    if (doutputModule != NULL && gu8_handlerItemsCounter < DOUTPUT_MODULE_DEVICE_NUM)
    {
        gax_doutputModule_internalHandler[gu8_handlerItemsCounter].gpio.port = doutputModule->gpio.port;
        gax_doutputModule_internalHandler[gu8_handlerItemsCounter].gpio.pin = doutputModule->gpio.pin;
        gax_doutputModule_internalHandler[gu8_handlerItemsCounter].gpio.ioState = GPIO_OUTPUT;
        gax_doutputModule_internalHandler[gu8_handlerItemsCounter].gpio.state = doutputModule->gpio.state;

        gpio_pin_init(&(gax_doutputModule_internalHandler[gu8_handlerItemsCounter].gpio));
        gpio_pinState_set(gax_doutputModule_internalHandler[gu8_handlerItemsCounter].gpio.port, gax_doutputModule_internalHandler[gu8_handlerItemsCounter].gpio.pin, gax_doutputModule_internalHandler[gu8_handlerItemsCounter].gpio.state);
        gax_doutputModule_internalHandler[gu8_handlerItemsCounter].mark = DOUTPUT_MODULE_MARK;

        handler = &gax_doutputModule_internalHandler[gu8_handlerItemsCounter];
        gu8_handlerItemsCounter++;
    }

    return handler;
}

ret_t doutputModule_state_set(doutputModule_handle doutputModule, gpio_state_t state)
{
    ret_t ret = RET_NOK;

    if (doutputModule != NULL && doutputModule->mark == DOUTPUT_MODULE_MARK)
    {
        doutputModule->gpio.state = state;
        ret = RET_OK;
    }

    return ret;
}

gpio_state_t doutputModule_state_get(doutputModule_handle doutputModule)
{
    gpio_state_t state = GPIO_LOW;

    if ((doutputModule != NULL) && (doutputModule->mark == DOUTPUT_MODULE_MARK))
    {
        state = doutputModule->gpio.state;
    }

    return state;
}

void doutputModule_update(uint32_t period)
{
    // gpio_state_t oldState, newState;
    uint8_t buffer[DOUTPUT_MODULE_DEVICE_NUM] = {0};

    for (uint8_t i = 0; i < gu8_handlerItemsCounter; i++)
    {
        // oldState = gax_doutputModule_internalHandler[i].gpio.state;
        gpio_pinState_set(gax_doutputModule_internalHandler[i].gpio.port, gax_doutputModule_internalHandler[i].gpio.pin, gax_doutputModule_internalHandler[i].gpio.state);

        buffer[i] = (uint8_t)gax_doutputModule_internalHandler[i].gpio.state;

        // newState = gpio_pinState_get(gax_doutputModule_internalHandler[i].gpio.port, gax_doutputModule_internalHandler[i].gpio.pin);

        // if (newState != oldState)
        // {
        // log_errorLog_record(moduleName, functionName, gax_doutputModule_internalHandler[i].outputID, PIN_NOT_RESPONDING)
        // }
    }

    LOG_CB('o', 'v', buffer, DOUTPUT_MODULE_DEVICE_NUM);
}

// NOTE: for testing purposes only
doutputModule_t *doutputModule_internalData_get(doutputModule_handle doutputModule)
{
    doutputModule_t *ret = NULL;

    if (doutputModule != NULL && doutputModule->mark == DOUTPUT_MODULE_MARK)
    {
        ret = (doutputModule_t *)&doutputModule->gpio;
    }

    return ret;
}

void doutputModule_reset(void)
{
    for (uint8_t i = 0; i < DOUTPUT_MODULE_DEVICE_NUM; i++)
    {
        gax_doutputModule_internalHandler[i].gpio.port = (gpio_port_t)0xff;
        gax_doutputModule_internalHandler[i].gpio.pin = (gpio_pin_t)0xff;
        gax_doutputModule_internalHandler[i].gpio.state = (gpio_state_t)0xff;
        gax_doutputModule_internalHandler[i].gpio.ioState = (gpio_ioState_t)0xff;
    }
}
