/**
 ****************************************************************************
 * @file     system_TMPM370.h
 * @brief    CMSIS Cortex-M3 Device Peripheral Access Layer Header File for the
 *           TOSHIBA 'TMPM370' Device Series 
 * @version  V2.0.2.2
 * @date     2014/05/20
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *****************************************************************************
 */

#ifndef __SYSTEM_TMPM370_H
#define __SYSTEM_TMPM370_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif 

extern uint32_t SystemCoreClock;    /*!< System Clock Frequency (Core Clock)  */

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);

/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock 
 *         retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif
