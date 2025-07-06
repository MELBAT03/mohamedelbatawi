#ifndef DAC_H
#define DAC_H

#include "mcal.h"

typedef mcal_dac_chEnum_t dac_channel_t;
#define DAC_CH_0 MCAL_DAC_CH0
#define DAC_CH_1 MCAL_DAC_CH1

typedef mcal_dac_blkEnum_t dac_block_t;
#define DAC_BLK_A MCAL_ADC_BLK_A

void dac_init(dac_block_t *x_dacBlock);
void dac_channel_set(dac_block_t *x_dacBlock, dac_channel_t x_dacCh);
void dac_value_set(dac_block_t *x_dacBlock, dac_channel_t x_dacCh, float value);
void dac_conversion_start(dac_block_t *x_dacBlock, dac_channel_t x_dacCh);
void dac_update(uint32_t period);

#endif
