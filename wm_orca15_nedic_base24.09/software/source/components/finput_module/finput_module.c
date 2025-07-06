#include "def.h"
#include "component_config.h"

#include "finput_module/finput_module.h"
#include "components/hw_timer/hw_timer.h"

#define FINPUT_MODULE_MARK (0xF1U)

typedef struct finputModule_internalHandler
{
    hwTimer_t *timerModule;
    gpio_t gpio;
    hwTimer_eventEdge_t edge;
    uint32_t updatePeriod;
    uint32_t counterValue;
    uint32_t internalCounter;
    uint8_t mark;
} finputModule_internalHandler_t;

static finputModule_internalHandler_t gax_finputModule_internalHandler[FINPUT_MODULE_DEVICES_NUM];

static uint8_t gu8_handlerItemsCounter = 0;

finputModule_handle finputModule_init(finputModule_t *finputModule)
{
    finputModule_handle handler = NULL;

    if (finputModule != NULL && gu8_handlerItemsCounter < FINPUT_MODULE_DEVICES_NUM)
    {
        gax_finputModule_internalHandler[gu8_handlerItemsCounter].timerModule = finputModule->timerModule;
        gax_finputModule_internalHandler[gu8_handlerItemsCounter].gpio.port = finputModule->gpio.port;
        gax_finputModule_internalHandler[gu8_handlerItemsCounter].gpio.pin = finputModule->gpio.pin;
        gax_finputModule_internalHandler[gu8_handlerItemsCounter].gpio.ioState = finputModule->gpio.ioState;
        gax_finputModule_internalHandler[gu8_handlerItemsCounter].gpio.state = finputModule->gpio.state;
        gax_finputModule_internalHandler[gu8_handlerItemsCounter].updatePeriod = finputModule->updatePeriod;
        gax_finputModule_internalHandler[gu8_handlerItemsCounter].counterValue = 0;
        gax_finputModule_internalHandler[gu8_handlerItemsCounter].internalCounter = 0;
        gax_finputModule_internalHandler[gu8_handlerItemsCounter].mark = FINPUT_MODULE_MARK;

        hwTimer_init();
        hwTimer_counterMode_set(finputModule->timerModule, &finputModule->gpio, finputModule->edge);
        hwTimer_channel_enable(finputModule->timerModule);
        hwTimer_state_set(finputModule->timerModule, HW_TIMER_START);

        handler = &gax_finputModule_internalHandler[gu8_handlerItemsCounter];
        gu8_handlerItemsCounter++;
    }
    return handler;
}

uint32_t finputModule_value_get(finputModule_handle finputModule)
{
    uint32_t ret = 0;

    if (finputModule != NULL && finputModule->mark == FINPUT_MODULE_MARK)
    {
        ret = finputModule->counterValue;
    }

    return ret;
}

void finputModule_update(uint32_t period)
{
    uint8_t i;

    for (i = 0; i < gu8_handlerItemsCounter; i++)
    {
        if (gax_finputModule_internalHandler[i].internalCounter >= gax_finputModule_internalHandler[i].updatePeriod)
        {
            gax_finputModule_internalHandler[i].counterValue = hwTimer_count_get(gax_finputModule_internalHandler[i].timerModule);
            hwTimer_count_reset(gax_finputModule_internalHandler[i].timerModule);
            hwTimer_flag_clear(gax_finputModule_internalHandler[i].timerModule);
            hwTimer_state_set(gax_finputModule_internalHandler[i].timerModule, HW_TIMER_START);
            gax_finputModule_internalHandler[i].internalCounter = 0;
        }
        gax_finputModule_internalHandler[i].internalCounter += period;
    }
}
