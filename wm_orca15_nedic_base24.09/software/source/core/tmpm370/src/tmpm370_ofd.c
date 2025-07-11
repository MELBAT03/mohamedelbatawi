/**
 *******************************************************************************
 * @file    tmpm370_ofd.c
 * @brief   This file provides API functions for OFD driver.
 * @version V2.0.2.2
 * @date    2014/05/20
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm370_ofd.h"

#if defined(__TMPM370_OFD_H) || defined(__TMPM372_OFD_H) || defined(__TMPM373_OFD_H) || defined(__TMPM374_OFD_H)
/** @addtogroup TX03_Periph_Driver
  * @{
  */
/** @defgroup OFD
  * @brief OFD driver modules
  * @{
  */

/** @defgroup OFD_Private_Defines
  * @{
  */
#define OFD_REG_WRITE_DISABLE ((uint32_t)0x00000006U)
#define OFD_REG_WRITE_ENABLE  ((uint32_t)0x000000F9U)
#define OFD_DISABLE           ((uint32_t)0x00000000U)
#define OFD_ENABLE            ((uint32_t)0x000000E4U)
/** @} */
/* End of group OFD_Private_Defines */

/** @defgroup OFD_Exported_Functions
  * @{
  */

/**
  * @brief  Enable or disable the writing of OFDCR2/OFDMNPLLON/OFDMXPLLON/OFDMNPLLOFF/OFDMXPLLOFF.
  * @param  NewState: the new state of enable or disable
  *             This parameter can be one of the following values:
  *             ENABLE or DISABLE
  * @retval None
  */
void OFD_SetRegWriteMode(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Set OFDCR1<CLKWEN7:0> to enable or disable the writing of 
       OFDCR2/OFDMNPLLON/OFDMXPLLON/OFDMNPLLOFF/OFDMXPLLOFF. */
    if (NewState == ENABLE) {
        TSB_OFD->CR1 = OFD_REG_WRITE_ENABLE;
    } else {
        TSB_OFD->CR1 = OFD_REG_WRITE_DISABLE;
    }
}

/**
  * @brief  Enable the OFD function.
  * @param  None
  * @retval None
  */
void OFD_Enable(void)
{
    /* OFDCR2<CLKSEN7:0> = 0xE4 */
    TSB_OFD->CR2 = OFD_ENABLE;
}

/**
  * @brief  Disable the OFD function.
  * @param  None
  * @retval None
  */
void OFD_Disable(void)
{
    /* OFDCR2<CLKSEN7:0> = 0x00 */
    TSB_OFD->CR2 = OFD_DISABLE;
}

/**
  * @brief  Set the count value of detection frequency.
  * @param  State: State of PLL.
  *             This parameter can be one of the following values:
  *             OFD_PLL_ON or OFD_PLL_OFF
  *         HigherDetectionCount: the count value of higher detection frequency.
  *         LowerDetectionCount: the count value of lower detection frequency.
  * @retval None
  */
void OFD_SetDetectionFrequency(OFD_PLL_State State, uint32_t HigherDetectionCount,
                               uint32_t LowerDetectionCount)
{
    assert_param(IS_OFD_PLL_State(State));
    assert_param(IS_OFD_DETECT_FREQ_HIGHER(HigherDetectionCount));
    assert_param(IS_OFD_DETECT_FREQ_LOWER(LowerDetectionCount));
    /* Set the OFDMNPLLON<OFDMNPLLON8:0> and  OFDMXPLLON<OFDMXPLLON8:0> bits or 
       the OFDMNPLLOFF<OFDMNPLLOFF8:0> and  OFDMXPLLOFF<OFDMXPLLOFF8:0> bits */

    if (State == OFD_PLL_ON) {
        TSB_OFD->MNPLLON = LowerDetectionCount;
        TSB_OFD->MXPLLON = HigherDetectionCount;
    } else {
        TSB_OFD->MNPLLOFF = LowerDetectionCount;
        TSB_OFD->MXPLLOFF = HigherDetectionCount;
    }
}


/** @} */
/* End of group OFD_Exported_Functions */
/** @} */
/* End of group OFD */
/** @} */
/* End of group TX03_Periph_Driver */
#endif                          /* defined(__TMPM370_OFD_H) || defined(__TMPM372_OFD_H) || defined(__TMPM373_OFD_H) || defined(__TMPM374_OFD_H) */
