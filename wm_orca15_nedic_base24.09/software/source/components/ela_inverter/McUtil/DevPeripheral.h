/**===========================================================================
 * @file		DevPeripheral.h
==============================================================================*/

#ifndef	_DEV_PERIPHERAL_H_
#define	_DEV_PERIPHERAL_H_

/***************************************************************************************************
 *	[Include File]
 **************************************************************************************************/
#include "cpu.h"
#include "MC_config.h"
#include "../McApp/McProject.h"


/***************************************************************************************************
 *	[ GPIO Function Declaration]
 **************************************************************************************************/
   

/***************************************************************************************************
 *	[UART Function Declaration]
 **************************************************************************************************/
 
INTERRUPT ISR_UART_TX_EMPTY(void);
INTERRUPT ISR_UART_RX_END(void);
INTERRUPT ISR_INT_TX1(void);
INTERRUPT ISR_INT_TX0(void);
INTERRUPT ISR_INT_RX0(void);
PUBLIC void mc_util_software_debug(void);

//#if defined(_USE_UART_4800_) || defined(_USE_UART_9600_)
#if defined(MC_DEBUG_UART_ENABLED)
PUBLIC unsigned int SendUart0Tx(BYTE *xpBuf, unsigned int xunCntTx);
PUBLIC unsigned int ResiveUart0Rx(BYTE *xpBuf, unsigned int xunCntRx);
#endif

#if defined(COMMAND_WITH_TTL)
/*mabrouk edit uart1 */
PUBLIC unsigned int SendUart1Tx(BYTE *xpBuf, unsigned int xunCntTx);
PUBLIC unsigned int ResiveUart1Rx(BYTE *xpBuf, unsigned int xunCntRx);
#endif
/***************************************************************************************************
 *	[End Of Header File]
 **************************************************************************************************/
#endif
