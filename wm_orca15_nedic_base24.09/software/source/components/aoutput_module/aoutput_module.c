
#include "mcal.h"
#include "def.h"
#include "dac/dac.h"
#include "aoutput_module/aoutput_module.h"

typedef struct
{
    dac_block_t *dacBlock;
    dac_channel_t dacCh;
    gpio_t gpio;   // in case the mcu requires the pins as gpio to be identified
    uint8_t dacDeviceID;
    uint32_t dacValue;
} aoutputModule_internalHandler_t;

static aoutputModule_internalHandler_t gax_aoutputModule_internalHandler[AOUTPUT_MODULES_NUM];
static uint8_t gu8_aoutputModule_internalCounter = 0;

void aoutputModule_init(aoutputModule_t *aoutput)
{
    gax_aoutputModule_internalHandler[gu8_aoutputModule_internalCounter].gpio.port = aoutput->gpio.port;
    gax_aoutputModule_internalHandler[gu8_aoutputModule_internalCounter].gpio.pin = aoutput->gpio.pin;
    gax_aoutputModule_internalHandler[gu8_aoutputModule_internalCounter].gpio.ioState = aoutput->gpio.ioState;
    gax_aoutputModule_internalHandler[gu8_aoutputModule_internalCounter].gpio.state = aoutput->gpio.state;
    gax_aoutputModule_internalHandler[gu8_aoutputModule_internalCounter].dacDeviceID = aoutput->dacDeviceID;
    gax_aoutputModule_internalHandler[gu8_aoutputModule_internalCounter].dacValue = 0;
    gax_aoutputModule_internalHandler[gu8_aoutputModule_internalCounter].dacBlock = aoutput->dacBlock;
    gax_aoutputModule_internalHandler[gu8_aoutputModule_internalCounter].dacCh = aoutput->dacCh;

    dac_channel_set(aoutput->dacBlock, aoutput->dacCh);

    gu8_aoutputModule_internalCounter++;
}

void aoutputModule_value_set(uint8_t u8_dacDeviceID, uint32_t value)
{
    uint8_t i;

    for (i = 0; i < gu8_aoutputModule_internalCounter; i++)
    {
        if (gax_aoutputModule_internalHandler[i].dacDeviceID == u8_dacDeviceID)
        {
            gax_aoutputModule_internalHandler[i].dacValue = value;
        }
    }
}

void aoutputModule_update(uint32_t period)
{
    uint8_t i;

    for (i = 0; i < gu8_aoutputModule_internalCounter; i++)
    {
        dac_value_set(gax_aoutputModule_internalHandler[i].dacBlock, gax_aoutputModule_internalHandler[i].dacCh, gax_aoutputModule_internalHandler[i].dacValue);
    }
}
