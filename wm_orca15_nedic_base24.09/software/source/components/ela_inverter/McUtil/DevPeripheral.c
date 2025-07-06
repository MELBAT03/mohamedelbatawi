/**===========================================================================
* @file		DevPeripheral.c
==============================================================================*/


/***************************************************************************************************
*	[Include File]
**************************************************************************************************/
#include "../McApp/McProject.h"
#include "../McUtil/DevPeripheral.h"
#include "../McUtil/McTarget.h"
#include "../core/tmpm370/inc/tmpm370_uart.h"
#include "../core/tmpm370/inc/tmpm370_gpio.h"
#include <stdio.h>
/***************************************************************************************************
*	[Define]
**************************************************************************************************/

/***************************************************************************************************
*	[Enumeration & TypeDeclarations]
**************************************************************************************************/


/***************************************************************************************************
*	[Constant & Variable Declarations]
**************************************************************************************************/

/***************************************************************************************************
*	[Function Declaration]
**************************************************************************************************/


static void mc_util_software_debug_init(void);

static void mc_util_software_debug_configuration(void);
/***************************************************************************************************
Name:        mc_util_software_debug   
Description:  
***************************************************************************************************/

PUBLIC void mc_util_software_debug(void)
{
#if defined(MC_DEBUG_UART_ENABLED) ||defined (COMMAND_WITH_TTL)
  mc_util_software_debug_init(); 
#else
  /*do nothing*/
#endif
}

/***************************************************************************************************
*	[ UART Function Implementation]
**************************************************************************************************/

#define UART_TX_ING				0x0001

#define SIZE_BUF_TX				64
#define SIZE_BUF_RX				64

#define UART_SET_TXEND()		do{  }while(0)
#if defined(COMMAND_WITH_TTL)
#define UART1_SET_TXBUF(xData)	do{ UART_SetTxData(UART1,xData); }while(0)                       //SC1BUF  for channel 1
#define UART1_GET_RXBUF()		(UART_GetRxData(UART1))                                        // SC1BUF       for channel
#endif

void mc_util_software_debug_configuration(void)
{
  #if defined(MC_DEBUG_UART_ENABLED)
  UART_InitTypeDef myUART;              // create the struct to have the uart configurations
  NVIC_DisableIRQ(MC_DEBUG_UART_RX_IRG);
  NVIC_DisableIRQ(MC_DEBUG_UART_TX_IRG);
  /* uart 0 */
  UART_Enable(MC_DEBUG_UART);  
  myUART.Parity = UART_NO_PARITY;
  myUART.FlowCtrl = UART_NONE_FLOW_CTRL;
  myUART.DataBits = UART_DATA_BITS_8;
  UART_SetFIFOTransferMode(MC_DEBUG_UART,UART_TRANSFER_FULLDPX);//0x60 full duplex
  
  myUART.StopBits = UART_STOP_BITS_1; /* 1-bit stop, LSB,  ((W-buff enable : the 4 in 0x84)) and 0x14*/
  myUART.BaudRate = 1250000;
  myUART.Mode = UART_ENABLE_TX | UART_ENABLE_RX; //both
  UART_Init(MC_DEBUG_UART, &myUART);
  UART_FIFOConfig(MC_DEBUG_UART,DISABLE);
  
  myUART.Mode = UART_ENABLE_TX /*| UART_ENABLE_RX*/; //both
  /*  portE fr*/
  GPIO_EnableFuncReg(GPIO_PE,GPIO_FUNC_REG_1,GPIO_BIT_0);
  GPIO_EnableFuncReg(GPIO_PE,GPIO_FUNC_REG_1,GPIO_BIT_1);
  /* portE cr*/
  GPIO_SetOutputEnableReg(GPIO_PE,GPIO_BIT_0,ENABLE); 
  GPIO_SetOutputEnableReg(GPIO_PE,GPIO_BIT_1,ENABLE);
  // transmite interrupt setting
  NVIC_SetPriority(MC_DEBUG_UART_TX_IRG,0x5);
  NVIC_ClearPendingIRQ(MC_DEBUG_UART_TX_IRG);
  NVIC_EnableIRQ(MC_DEBUG_UART_TX_IRG);
  
  // Serial reception 
//  NVIC_SetPriority(MC_DEBUG_UART_RX_IRG,0x5);
//  NVIC_ClearPendingIRQ(MC_DEBUG_UART_RX_IRG);
//  NVIC_EnableIRQ(MC_DEBUG_UART_RX_IRG);
#endif
  /***************************************************************************************************************/
 #if defined(COMMAND_WITH_TTL)
  /*  uart 1 */
  UART_InitTypeDef myUART1;              // create the struct to have the uart configurations
  NVIC_DisableIRQ(INTRX1_IRQn);
  NVIC_DisableIRQ(INTTX1_IRQn);  
  UART_Enable(UART1);  
  myUART1.Parity = UART_ODD_PARITY;
  myUART1.FlowCtrl = UART_NONE_FLOW_CTRL;
  myUART1.DataBits = UART_DATA_BITS_8;
  UART_SetFIFOTransferMode(UART1,UART_TRANSFER_FULLDPX);//0x60 full duplex
  myUART1.StopBits = UART_STOP_BITS_1; /* 1-bit stop, LSB,  ((W-buff enable : the 4 in 0x84)) and 0x14*/
  myUART1.BaudRate = 4800;
  myUART1.Mode = UART_ENABLE_TX | UART_ENABLE_RX; //both
  UART_Init(UART1, &myUART1);
  UART_FIFOConfig(UART1,DISABLE);
  
  GPIO_EnableFuncReg(GPIO_PA,GPIO_FUNC_REG_1,GPIO_BIT_5);
  GPIO_EnableFuncReg(GPIO_PA,GPIO_FUNC_REG_1,GPIO_BIT_6);
  
  /* portE cr*/
  GPIO_SetOutputEnableReg(GPIO_PA,GPIO_BIT_5,ENABLE); 
  GPIO_SetOutputEnableReg(GPIO_PA,GPIO_BIT_6,DISABLE);
  
  // Serial transmit 
  NVIC_SetPriority(INTTX1_IRQn,0x5);
  NVIC_ClearPendingIRQ(INTTX1_IRQn);
  NVIC_EnableIRQ(INTTX1_IRQn);
  
  // Serial reception 
  NVIC_SetPriority(INTRX1_IRQn,0x5);
  NVIC_ClearPendingIRQ(INTRX1_IRQn);
  NVIC_EnableIRQ(INTRX1_IRQn);
#endif
}


#if defined(MC_DEBUG_UART_ENABLED)
// UART Local Variable ====================================
BYTE abUart0TxBuf[SIZE_BUF_TX];
volatile unsigned int unIdxTxIn0;
volatile unsigned int unIdxTxOut0;
volatile unsigned int unCntTxBuf0;

BYTE abUart0RxBuf[SIZE_BUF_RX];
volatile unsigned int unIdxRxIn0;
volatile unsigned int unIdxRxOut0;
volatile unsigned int unCntRxBuf0;

volatile WORD wUart0Flag;
#endif
#if defined(COMMAND_WITH_TTL)
/*mabrouk edit uart1 */
BYTE abUart1TxBuf[SIZE_BUF_TX];
volatile unsigned int unIdxTxIn1;
volatile unsigned int unIdxTxOut1;
volatile unsigned int unCntTxBuf1;

BYTE abUart1RxBuf[SIZE_BUF_RX];
volatile unsigned int unIdxRxIn1;
volatile unsigned int unIdxRxOut1;
volatile unsigned int unCntRxBuf1;

volatile WORD wUart1Flag; 
#endif
void mc_util_software_debug_init(void)
{
  #if defined(MC_DEBUG_UART_ENABLED)
  unIdxTxIn0 = 0;
  unIdxTxOut0 = 0;
  unCntTxBuf0 = 0;
  unIdxRxIn0 = 0;
  unIdxRxOut0 = 0;
  unCntRxBuf0 = 0;
  wUart0Flag = 0;
#endif
  #if defined(COMMAND_WITH_TTL)
  /*mabrouk edit uart1*/
  unIdxTxIn1 = 0;
  unIdxTxOut1 = 0;
  unCntTxBuf1 = 0;
  unIdxRxIn1 = 0;
  unIdxRxOut1 = 0;
  unCntRxBuf1 = 0;
  wUart1Flag = 0;
#endif
  mc_util_software_debug_configuration();
  
}

INTERRUPT ISR_INT_TX0(void)   //ISR_INT_TX1   ISR_UART_TX_EMPTY
{
  #if defined(MC_DEBUG_UART_ENABLED)
  CPU_ENTER_INTERRUPT();
  if (unCntTxBuf0)
  {
    BYTE tbDataTx;
    tbDataTx = abUart0TxBuf[unIdxTxOut0];
    if (++unIdxTxOut0 >= SIZE_BUF_TX) unIdxTxOut0 = 0;
    unCntTxBuf0--;
    wUart0Flag |= UART_TX_ING;
    UART_SetTxData(MC_DEBUG_UART,tbDataTx);
    
  }
  else 
  {
    wUart0Flag &= ~UART_TX_ING;
    UART_SET_TXEND();
  }
  CPU_EXIT_INTERRUPT();
#endif
}
INTERRUPT ISR_INT_RX0(void)
{
  #if defined(MC_DEBUG_UART_ENABLED)
  BYTE bRx;	
  CPU_ENTER_INTERRUPT();
  bRx = UART_GetRxData(MC_DEBUG_UART);
  
  if (unCntRxBuf0 < SIZE_BUF_RX)
  {
    abUart0RxBuf[unIdxRxIn0] = bRx;
    
    if (++unIdxRxIn0 >= SIZE_BUF_RX) unIdxRxIn0 = 0;
    unCntRxBuf0++;
  }
  CPU_EXIT_INTERRUPT();
#endif
}
#if defined(COMMAND_WITH_TTL)
INTERRUPT ISR_INT_TX1(void)   //ISR_INT_TX1   ISR_UART_TX_EMPTY
{
  CPU_ENTER_INTERRUPT();
  if (unCntTxBuf1)
  {
    BYTE tbDataTx;
    tbDataTx = abUart1TxBuf[unIdxTxOut1];
    if (++unIdxTxOut1 >= SIZE_BUF_TX) unIdxTxOut1 = 0;
    unCntTxBuf1--;
    wUart1Flag |= UART_TX_ING;
    UART1_SET_TXBUF(tbDataTx);
  }
  else 
  {
    wUart1Flag &= ~UART_TX_ING;
    UART_SET_TXEND();    
  }
  CPU_EXIT_INTERRUPT();
}



INTERRUPT ISR_INT_RX1(void)
{
  BYTE bRx;	
  CPU_ENTER_INTERRUPT();
  bRx = UART1_GET_RXBUF();
  
  if (unCntRxBuf1 < SIZE_BUF_RX)
  {
    abUart1RxBuf[unIdxRxIn1] = bRx;
    
    if (++unIdxRxIn1 >= SIZE_BUF_RX) unIdxRxIn1 = 0;
    unCntRxBuf1++;
  }
  CPU_EXIT_INTERRUPT();
}
#endif
INTERRUPT ISR_UART_RX_END(void)
{
  //  BYTE bRx;	
  //  CPU_ENTER_INTERRUPT();
  //  bRx = UART_GetRxData(MC_DEBUG_UART);
  //  
  //  if (unCntRxBuf < SIZE_BUF_RX)
  //  {
  //    abUartRxBuf[unIdxRxIn] = bRx;
  //    
  //    if (++unIdxRxIn >= SIZE_BUF_RX) unIdxRxIn = 0;
  //    unCntRxBuf++;
  //  }
  //  CPU_EXIT_INTERRUPT();
}


PUBLIC unsigned int SendUart0Tx(BYTE *xpBuf, unsigned int xunCntTx)
{
#if defined(MC_DEBUG_UART_ENABLED)
  TCpuSR cpuSR = 0;
  unsigned int tunCnt = xunCntTx;
  
  CPU_ENTER_CRITICAL();
  while (tunCnt && (unCntTxBuf0 < SIZE_BUF_TX))
  {
    abUart0TxBuf[unIdxTxIn0] = *xpBuf;
    if (++unIdxTxIn0 >= SIZE_BUF_TX) unIdxTxIn0 = 0;
    unCntTxBuf0++;			// Critical variable (=u????? ????) 
    xpBuf++;
    tunCnt--;
  }
  tunCnt = xunCntTx - tunCnt;
  if (((wUart0Flag & UART_TX_ING) == 0) && unCntTxBuf0)
  {
    BYTE tbDataTx;
    tbDataTx = abUart0TxBuf[unIdxTxOut0];
    if (++unIdxTxOut0 >= SIZE_BUF_TX) unIdxTxOut0 = 0;
    unCntTxBuf0--;
    wUart0Flag |= UART_TX_ING;
    
    UART_SetTxData(MC_DEBUG_UART,tbDataTx);
  }
  CPU_EXIT_CRITICAL();
    return tunCnt;

#else
  return 0;
#endif
}


PUBLIC unsigned int ResiveUart0Rx(BYTE *xpBuf, unsigned int xunCntRx)
{
  unsigned int tunCnt = 0;
    #if defined(MC_DEBUG_UART_ENABLED)
  TCpuSR cpuSR = 0;

  if (unCntRxBuf0 == 0) 
  {
    return tunCnt;
  }
  
  CPU_ENTER_CRITICAL();
  while (1)
  {
    if ((unCntRxBuf0 == 0) || (tunCnt >= xunCntRx)) break;
    
    xpBuf[tunCnt] = abUart0RxBuf[unIdxRxOut0];
    if (++unIdxRxOut0 >= SIZE_BUF_RX) unIdxRxOut0 = 0;
    unCntRxBuf0--;			// Critical variable (=u????? ????) 
    tunCnt++;		  
  }  
  CPU_EXIT_CRITICAL();
#endif
  return tunCnt;
}
#if defined(COMMAND_WITH_TTL)
PUBLIC unsigned int SendUart1Tx(BYTE *xpBuf1, unsigned int xunCntTx)
{
  TCpuSR cpuSR = 0;
  unsigned int tunCnt = xunCntTx;
  
  CPU_ENTER_CRITICAL();
  while (tunCnt && (unCntTxBuf1 < SIZE_BUF_TX))
  {
    abUart1TxBuf[unIdxTxIn1] = *xpBuf1;
    if (++unIdxTxIn1 >= SIZE_BUF_TX) unIdxTxIn1 = 0;
    unCntTxBuf1++;			// Critical variable (=u????? ????) 
    xpBuf1++;
    tunCnt--;
  }
  tunCnt = xunCntTx - tunCnt;
  if (((wUart1Flag & UART_TX_ING) == 0) && unCntTxBuf1)
  {
    BYTE tbDataTx;
    tbDataTx = abUart1TxBuf[unIdxTxOut1];
    if (++unIdxTxOut1 >= SIZE_BUF_TX) unIdxTxOut1 = 0;
    unCntTxBuf1--;
    wUart1Flag |= UART_TX_ING;
    
    UART1_SET_TXBUF(tbDataTx);
  }
  CPU_EXIT_CRITICAL();
  return tunCnt;
}




PUBLIC unsigned int ResiveUart1Rx(BYTE *xpBuf1, unsigned int xunCntRx1)
{
  TCpuSR cpuSR = 0;
  unsigned int tunCnt = 0;
  
  if (unCntRxBuf1 == 0) 
  {
    return tunCnt;
  }
  
  CPU_ENTER_CRITICAL();
  while (1)
  {
    if ((unCntRxBuf1 == 0) || (tunCnt >= xunCntRx1)) break;
    
    xpBuf1[tunCnt] = abUart1RxBuf[unIdxRxOut1];
    if (++unIdxRxOut1 >= SIZE_BUF_RX) unIdxRxOut1 = 0;
    unCntRxBuf1--;			// Critical variable (=u????? ????) 
    tunCnt++;		  
  }  
  CPU_EXIT_CRITICAL();
  return tunCnt;
}

#endif




/***************************************************************************************************
*	[End Of Source File]
**************************************************************************************************/