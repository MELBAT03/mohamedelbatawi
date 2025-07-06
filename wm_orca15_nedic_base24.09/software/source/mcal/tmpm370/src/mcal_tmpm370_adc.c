#include "mcal.h"

void mcal_adc_init(mcal_adc_blkEnum_t x_adcBlock)
{
    TSB_AD_TypeDef *adcBlk = TSB_ADA;
    switch (x_adcBlock)
    {
        case MCAL_ADC_BLK_A:
            adcBlk = TSB_ADA;
            break;

        case MCAL_ADC_BLK_B:
            adcBlk = TSB_ADB;
            break;
        default:
        {
            // do nothing
        }
        break;
    }
    ADC_SetClk(adcBlk, ADC_HOLD_FIX, ADC_FC_DIVIDE_LEVEL_8);
}

void mcal_adc_channel_set(mcal_adc_blkEnum_t x_adcBlock, mcal_adc_chEnum_t x_adcCh)
{
    TSB_AD_TypeDef *adcBlk = TSB_ADA;
    switch (x_adcBlock)
    {
        case MCAL_ADC_BLK_A:
            adcBlk = TSB_ADA;
            break;

        case MCAL_ADC_BLK_B:
            adcBlk = TSB_ADB;
            break;
    }
    ADC_SetSWTrg(adcBlk, ADC_REG0, TRG_ENABLE((x_adcCh)));
    ADC_Enable(adcBlk);
}

void mcal_adc_conversion_start(mcal_adc_blkEnum_t x_adcBlock, mcal_adc_chEnum_t x_adcCh)
{
    TSB_AD_TypeDef *adcBlk = TSB_ADA;
    switch (x_adcBlock)
    {
        case MCAL_ADC_BLK_A:
            adcBlk = TSB_ADA;
            break;

        case MCAL_ADC_BLK_B:
            adcBlk = TSB_ADB;
            break;
    }
    ADC_SetSWTrg(adcBlk, ADC_REG0, TRG_ENABLE((x_adcCh)));
    ADC_Enable(adcBlk);
    ADC_Start(adcBlk, ADC_TRG_SW);
}

uint32_t mcal_adc_conversionResult_get(mcal_adc_blkEnum_t x_adcBlock, mcal_adc_chEnum_t x_adcCh)
{
    TSB_AD_TypeDef *adcBlk = TSB_ADA;
    switch (x_adcBlock)
    {
        case MCAL_ADC_BLK_A:
            adcBlk = TSB_ADA;
            break;

        case MCAL_ADC_BLK_B:
            adcBlk = TSB_ADB;
            break;
    }
    ADC_Result ret = ADC_GetConvertResult(adcBlk, ADC_REG0);
    return (uint32_t)ret.Bit.ADResult;
}

uint32_t mcal_adc_conversionStatus_get(mcal_adc_blkEnum_t x_adcBlock, mcal_adc_chEnum_t x_adcCh)
{
    TSB_AD_TypeDef *adcBlk = TSB_ADA;
    switch (x_adcBlock)
    {
        case MCAL_ADC_BLK_A:
            adcBlk = TSB_ADA;
            break;

        case MCAL_ADC_BLK_B:
            adcBlk = TSB_ADB;
            break;
    }
    return (uint32_t)ADC_GetConvertState(adcBlk, ADC_TRG_SW);
}

void mcal_adc_flag_clear(mcal_adc_blkEnum_t x_adcBlock, mcal_adc_chEnum_t x_adcCh)
{
    // do nothing
}
