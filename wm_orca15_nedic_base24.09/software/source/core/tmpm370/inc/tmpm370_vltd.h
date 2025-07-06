/**
 *******************************************************************************
 * @file    tmpm370_vltd.h
 * @brief   This file provides all the functions prototypes for VLTD driver. 
 * @version V2.0.2.2
 * @date    2014/05/20
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM370_VLTD_H
#define __TMPM370_VLTD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "TMPM370.h"
#include "tx03_common.h"

#if defined(__TMPM370_VLTD_H) || defined(__TMPM372_VLTD_H) || defined(__TMPM373_VLTD_H) || defined(__TMPM374_VLTD_H)
/** @addtogroup TX03_Periph_Driver
  * @{
  */

/** @addtogroup VLTD
  * @{
  */

/** @defgroup VLTD_Exported_Types
  * @{
  */

/** @} */
/* End of group VLTD_Exported_Types */

/** @defgroup VLTD_Exported_Constants
  * @{
  */

#define VLTD_DETECT_VOLTAGE_41          ((uint32_t)0x00000002)
#define VLTD_DETECT_VOLTAGE_44          ((uint32_t)0x00000004)
#define VLTD_DETECT_VOLTAGE_46          ((uint32_t)0x00000006)

#define IS_VLTD_DETECT_VOLTAGE(param)   (((param) == VLTD_DETECT_VOLTAGE_41) || \
                                         ((param) == VLTD_DETECT_VOLTAGE_44) || \
                                         ((param) == VLTD_DETECT_VOLTAGE_46))

/** @} */
/* End of VLTD_Exported_Constants */

/** @defgroup VLTD_Exported_FunctionPrototypes
  * @{
  */
    void VLTD_Enable(void);
    void VLTD_Disable(void);
    void VLTD_SetVoltage(uint32_t Voltage);

/** @} */
/* End of VLTD_Exported_FunctionPrototypes */

/** @} */
/* End of group VLTD */

/** @} */
/* End of group TX03_Periph_Driver */
#endif                          /* defined(__TMPM370_VLTD_H) || defined(__TMPM372_VLTD_H) || defined(__TMPM373_VLTD_H) || defined(__TMPM374_VLTD_H) */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __TMPM370_VLTD_H */
