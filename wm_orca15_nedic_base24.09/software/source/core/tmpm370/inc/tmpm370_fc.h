/**
 *******************************************************************************
 * @file    tmpm370_fc.h
 * @brief   This file provides all the functions prototypes for FC driver.
 * @version V2.0.2.2
 * @date    2014/05/20
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __TMPM370_FC_H
#define __TMPM370_FC_H

#if !defined(TSB_M37xFY)
#define TSB_M37xFY
#endif

/* #define SINGLE_BOOT_MODE */

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "TMPM370.h"
#include "tx03_common.h"

#if defined(TSB_M37xFY) || defined(TSB_M37xFW)
/** @addtogroup TX03_Periph_Driver
  * @{
  */

/** @addtogroup FC
  * @{
  */

/** @defgroup FC_Exported_Types
  * @{
  */
    typedef enum {
        FC_SUCCESS = 0U,
        FC_ERROR_PROTECTED = 1U,
        FC_ERROR_OVER_TIME = 2U
    } FC_Result;

/** @} */
/* End of group FC_Exported_Types */

/** @defgroup FC_Exported_Constants
  * @{
  */
#define FLASH_PAGE_SIZE                 ((uint32_t)0x00000100)  /* Page size is 256 Byte */

#define FC_BLOCK_0                      ((uint8_t)0x00)
#define FC_BLOCK_1                      ((uint8_t)0x01)
#define FC_BLOCK_2                      ((uint8_t)0x02)
#define FC_BLOCK_3                      ((uint8_t)0x03)
#define FC_BLOCK_4                      ((uint8_t)0x04)
#define FC_BLOCK_5                      ((uint8_t)0x05)

#define FC_BLOCK_GROUP_0                 ((uint8_t)0x00)
#define FC_BLOCK_GROUP_1                 ((uint8_t)0x01)

#ifdef TSB_M37xFY
#define FLASH_CHIP_SIZE                 ((uint32_t)0x00040000)  /* Flash chip size is 256KByte */
#define IS_FC_BLOCK_NUM(param)          (((param) == FC_BLOCK_0) || \
                                         ((param) == FC_BLOCK_1) || \
                                         ((param) == FC_BLOCK_2) || \
                                         ((param) == FC_BLOCK_3) || \
                                         ((param) == FC_BLOCK_4) || \
                                         ((param) == FC_BLOCK_5))

#define IS_FC_BLOCK_GROUP(param)        (((param) == FC_BLOCK_GROUP_0) || \
                                         ((param) == FC_BLOCK_GROUP_1))
#endif

#ifdef TSB_M37xFW
#define FLASH_CHIP_SIZE                 ((uint32_t)0x00020000)  /* Flash chip size is 128KByte */
#define IS_FC_BLOCK_NUM(param)          (((param) == FC_BLOCK_0) || \
                                         ((param) == FC_BLOCK_1) || \
                                         ((param) == FC_BLOCK_2) || \
                                         ((param) == FC_BLOCK_3))

#define IS_FC_BLOCK_GROUP(param)        ((param) == FC_BLOCK_GROUP_0)
#endif

#ifdef SINGLE_BOOT_MODE
#define FLASH_START_ADDR                 ((uint32_t)0x3F800000) /* SINGLE_BOOT_MODE */
#else
#define FLASH_START_ADDR                 ((uint32_t)0x00000000) /* User Boot Mode As Default */
#endif

#define FLASH_END_ADDR                       (FLASH_START_ADDR + FLASH_CHIP_SIZE - 1U)

#ifdef SINGLE_BOOT_MODE
#define IS_FC_ADDR(param)                (((param) >= FLASH_START_ADDR) && \
                                          ((param) <= FLASH_END_ADDR))
#else
#define IS_FC_ADDR(param)                ((param) <= FLASH_END_ADDR)
#endif

#define IS_FC_PAGE_ADDR(param)          ((((param) - FLASH_START_ADDR) % FLASH_PAGE_SIZE) == 0U)

/** @} */
/* End of group FC_Exported_Constants */


/** @defgroup FC_Exported_FunctionPrototypes
  * @{
  */

    void FC_SetSecurityBit(FunctionalState NewState);
    FunctionalState FC_GetSecurityBit(void);
    WorkState FC_GetBusyState(void);
    FunctionalState FC_GetBlockProtectState(uint8_t BlockNum);
    FC_Result FC_ProgramBlockProtectState(uint8_t BlockNum);
    FC_Result FC_EraseBlockProtectState(uint8_t BlockGroup);
    FC_Result FC_WritePage(uint32_t PageAddr, uint32_t * Data);
    FC_Result FC_EraseBlock(uint32_t BlockAddr);
    FC_Result FC_EraseChip(void);

/** @} */
/* End of group FC_Exported_FunctionPrototypes */

/** @} */
/* End of group FC */

/** @} */
/* End of group TX03_Periph_Driver */
#endif                          /* defined(TSB_M37xFY) || defined(TSB_M37xFW) */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __TMPM370_FC_H */
