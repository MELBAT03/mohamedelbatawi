#include "dac/dac.h"
#include "mcal.h"

typedef struct
{
    dac_block_t *dacBlock;
    dac_channel_t dacCh;
    float dacValue;
} dac_chHandler_t;

static dac_chHandler_t gax_dac_channels[DAC_CHANNELS_NO];
static dac_channel_t gx_dac_chUpdateIndex;
static dac_channel_t gx_dac_chAddedIndex;

// :NOTE: to be called only once in the app
void dac_init(dac_block_t *x_dacBlock)
{
    uint8_t u8_index = 0;

    gx_dac_chUpdateIndex = DAC_CH_0;
    gx_dac_chAddedIndex = DAC_CH_0;
    for (u8_index = 0; u8_index < DAC_CHANNELS_NO; u8_index++)
    {
        gax_dac_channels[u8_index].dacCh = DAC_CH_0;
        gax_dac_channels[u8_index].dacBlock = DAC_BLK_A;
        gax_dac_channels[u8_index].dacValue = 0;
    }

    mcal_dac_set(x_dacBlock);
}

// :NOTE: to be called for every components/app that requires usage of the adc
void dac_channel_set(dac_block_t *x_dacBlock, dac_channel_t x_dacCh)
{
    gax_dac_channels[gx_dac_chAddedIndex].dacBlock = x_dacBlock;
    gax_dac_channels[gx_dac_chAddedIndex].dacCh = x_dacCh;

    gx_dac_chAddedIndex++;

    mcal_dac_channel_set(x_dacBlock, x_dacCh);
}

void dac_value_set(dac_block_t *x_dacBlock, dac_channel_t x_dacCh, float value)
{
    uint8_t i = 0;

    for (i = 0; i < DAC_CHANNELS_NO; i++)
    {
        if (x_dacCh == gax_dac_channels[i].dacCh && x_dacBlock == gax_dac_channels[i].dacBlock)
        {
            gax_dac_channels[i].dacValue = value;
        }
    }
}

void dac_conversion_start(dac_block_t *x_dacBlock, dac_channel_t x_dacCh)
{
    // mcal_dac_channel_enable(x_dacBlock, x_dacCh);
}

void dac_update(uint32_t period)
{
    if (mcal_dac_conversionStatus_get(gax_dac_channels[gx_dac_chUpdateIndex].dacBlock, gax_dac_channels[gx_dac_chUpdateIndex].dacCh) == 0)
        return;

    mcal_dac_flag_clear(gax_dac_channels[gx_dac_chUpdateIndex].dacBlock, gax_dac_channels[gx_dac_chUpdateIndex].dacCh);

    mcal_dac_value_set(gax_dac_channels[gx_dac_chUpdateIndex].dacBlock, gax_dac_channels[gx_dac_chUpdateIndex].dacCh, gax_dac_channels[gx_dac_chUpdateIndex].dacValue);

    gx_dac_chUpdateIndex++;
    if (gx_dac_chUpdateIndex >= DAC_CHANNELS_NO)
    {
        gx_dac_chUpdateIndex = DAC_CH_0;
    }

    dac_conversion_start(gax_dac_channels[gx_dac_chUpdateIndex].dacBlock, gax_dac_channels[gx_dac_chUpdateIndex].dacCh);
}
