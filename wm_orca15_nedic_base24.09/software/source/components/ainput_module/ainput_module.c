#include "component_config.h"
#include "mcal.h"
#include "def.h"

#include "ainput_module/ainput_module.h"

#define AINPUT_MODULE_MARK (0xA1U)

typedef struct ainputModule_internalHandler
{
    mcal_adc_blkEnum_t adcBlock;
    mcal_adc_chEnum_t adcCh;
    gpio_t gpio;   // in case the mcu requires the pins as gpio to be identified
    uint32_t adcValue;
    uint8_t mark;
} ainputModule_internalHandler_t;

static ainputModule_internalHandler_t gax_ainputModule_internalHandler[AINPUT_MODULE_DEVICES_NUM];

static uint8_t gu8_handlerItemsCounter = 0;

ainputModule_handle ainputModule_init(ainputModule_t *ainput)
{
    ainputModule_handle handler = NULL;

    if (ainput != NULL && gu8_handlerItemsCounter < AINPUT_MODULE_DEVICES_NUM)
    {
        gax_ainputModule_internalHandler[gu8_handlerItemsCounter].gpio.port = ainput->gpio.port;
        gax_ainputModule_internalHandler[gu8_handlerItemsCounter].gpio.pin = ainput->gpio.pin;
        gax_ainputModule_internalHandler[gu8_handlerItemsCounter].gpio.ioState = ainput->gpio.ioState;
        gax_ainputModule_internalHandler[gu8_handlerItemsCounter].gpio.state = ainput->gpio.state;
        gax_ainputModule_internalHandler[gu8_handlerItemsCounter].adcValue = 0;
        gax_ainputModule_internalHandler[gu8_handlerItemsCounter].adcBlock = ainput->adcBlock;
        gax_ainputModule_internalHandler[gu8_handlerItemsCounter].adcCh = ainput->adcCh;
        gax_ainputModule_internalHandler[gu8_handlerItemsCounter].mark = AINPUT_MODULE_MARK;

        mcal_adc_init(ainput->adcBlock);
        mcal_adc_channel_set(ainput->adcBlock, ainput->adcCh);

        handler = &gax_ainputModule_internalHandler[gu8_handlerItemsCounter];
        gu8_handlerItemsCounter++;
    }

    return handler;
}

uint32_t ainputModule_value_get(ainputModule_handle ainputModule)
{
    uint32_t ret = 0;

    if (ainputModule != NULL && ainputModule->mark == AINPUT_MODULE_MARK)
    {
        ret = ainputModule->adcValue;
    }

    return ret;
}

ret_t ainputModule_value_set(ainputModule_handle ainputModule, uint32_t value)
{
    ret_t ret = RET_NOK;

    if (ainputModule != NULL && ainputModule->mark == AINPUT_MODULE_MARK)
    {
        ainputModule->adcValue = value;
    }

    return ret;
}

void ainputModule_update(uint32_t period)
{
    static uint8_t gu8_ainputModule_chUpdateCounter = 0;

    if (mcal_adc_conversionStatus_get(gax_ainputModule_internalHandler[gu8_ainputModule_chUpdateCounter].adcBlock, gax_ainputModule_internalHandler[gu8_ainputModule_chUpdateCounter].adcCh) == 0)
        return;

    mcal_adc_flag_clear(gax_ainputModule_internalHandler[gu8_ainputModule_chUpdateCounter].adcBlock, gax_ainputModule_internalHandler[gu8_ainputModule_chUpdateCounter].adcCh);

    gax_ainputModule_internalHandler[gu8_ainputModule_chUpdateCounter].adcValue = mcal_adc_conversionResult_get(gax_ainputModule_internalHandler[gu8_ainputModule_chUpdateCounter].adcBlock, gax_ainputModule_internalHandler[gu8_ainputModule_chUpdateCounter].adcCh);

    gu8_ainputModule_chUpdateCounter++;
    if (gu8_ainputModule_chUpdateCounter >= gu8_handlerItemsCounter)
    {
        gu8_ainputModule_chUpdateCounter = 0;
    }

    mcal_adc_conversion_start(gax_ainputModule_internalHandler[gu8_ainputModule_chUpdateCounter].adcBlock, gax_ainputModule_internalHandler[gu8_ainputModule_chUpdateCounter].adcCh);
}
