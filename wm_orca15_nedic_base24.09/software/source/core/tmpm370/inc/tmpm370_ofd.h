/**
 *******************************************************************************
 * @file    tmpm370_ofd.h
 * @brief   This file provides all the functions prototypes for OFD driver.
 * @version V2.0.2.2
 * @date    2014/05/20
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM370_OFD_H
#define __TMPM370_OFD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "TMPM370.h"
#include "tx03_common.h"

#if defined(__TMPM370_OFD_H) || defined(__TMPM372_OFD_H) || defined(__TMPM373_OFD_H) || defined(__TMPM374_OFD_H)
/** @addtogroup TX03_Periph_Driver
  * @{
  */
/** @addtogroup OFD
  * @{
  */
/** @addtogroup OFD_Exported_Types
  * @{
  */
/** @} */
/* End of group OFD_Exported_Types */
/** @defgroup OFD_Exported_Constants
  * @{
  */
#define IS_OFD_DETECT_FREQ_HIGHER(param)    ((param) <= 0x000001FFU)
#define IS_OFD_DETECT_FREQ_LOWER(param)     ((param) <= 0x000001FFU)

    typedef enum {
        OFD_PLL_ON = 0U,
        OFD_PLL_OFF = 1U
    } OFD_PLL_State;
#define IS_OFD_PLL_State(param)      (((param) == OFD_PLL_ON) || \
                                      ((param) == OFD_PLL_OFF))

/** @} */
/* End of OFD_Exported_Constants */

/** @defgroup OFD_Exported_FunctionPrototypes
  * @{
  */
    void OFD_SetRegWriteMode(FunctionalState NewState);
    void OFD_Enable(void);
    void OFD_Disable(void);
    void OFD_SetDetectionFrequency(OFD_PLL_State State, uint32_t HigherDetectionCount, uint32_t LowerDetectionCount);

/** @} */
/* End of group OFD_Exported_FunctionPrototypes */
/** @} */
/* End of group OFD */
/** @} */
/* End of group TX03_Periph_Driver */
#endif                          /* defined(__TMPM370_OFD_H) || defined(__TMPM372_OFD_H) || defined(__TMPM373_OFD_H) || defined(__TMPM374_OFD_H) */
#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __TMPM370_OFD_H */
