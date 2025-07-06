/**
 *******************************************************************************
 * @file    tmpm370_vltd.c
 * @brief   This file provides API functions for VLTD driver. 
 * @version V2.0.2.2
 * @date    2014/05/20
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm370_vltd.h"

#if defined(__TMPM370_VLTD_H) || defined(__TMPM372_VLTD_H) || defined(__TMPM373_VLTD_H) || defined(__TMPM374_VLTD_H)
/** @addtogroup TX03_Periph_Driver
  * @{
  */
/** @defgroup VLTD
  * @brief VLTD driver modules
  * @{
  */

/** @defgroup VLTD_Private_Defines
  * @{
  */

#define VLTD_VDLVL_MASK         ((uint32_t)0xFFFFFFF9)

/** @} */
/* End of group VLTD_Private_Defines */

/** @defgroup VLTD_Exported_Functions
  * @{
  */

/**
  * @brief  Enable the operation of voltage detection.
  * @param  None
  * @retval None
  */
void VLTD_Enable(void)
{
    TSB_VD_CR_VDEN = 1U;
}

/**
  * @brief  Disable the operation of voltage detection.
  * @param  None
  * @retval None
  */
void VLTD_Disable(void)
{
    TSB_VD_CR_VDEN = 0U;
}

/**
  * @brief  Select for detection voltage.
  * @param  Voltage : detection voltage, which can be:
  *             VLTD_DETECT_VOLTAGE_41, VLTD_DETECT_VOLTAGE_44,
  *             VLTD_DETECT_VOLTAGE_46.
  * @retval None
  */
void VLTD_SetVoltage(uint32_t Voltage)
{
    uint32_t regval = TSB_VD->CR;

    /* Check the parameter */
    assert_param(IS_VLTD_DETECT_VOLTAGE(Voltage));

    regval &= VLTD_VDLVL_MASK;
    regval |= Voltage;
    TSB_VD->CR = regval;
}

/** @} */
/* End of group VLTD_Exported_Functions */
/** @} */
/* End of group VLTD */
/** @} */
/* End of group TX03_Periph_Driver */
#endif                          /* defined(__TMPM370_VLTD_H) || defined(__TMPM372_VLTD_H) || defined(__TMPM373_VLTD_H) || defined(__TMPM374_VLTD_H) */
