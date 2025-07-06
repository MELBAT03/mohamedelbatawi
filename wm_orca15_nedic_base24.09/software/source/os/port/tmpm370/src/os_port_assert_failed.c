#include "def.h"
#include "TMPM370.h"
#include "core_cm3.h"
#include "mcal.h"
#include "os.h"

static void putcha(uint32_t data, int n);

__attribute__((naked)) void assert_failed(char const *file, int line)
{
    /* TODO: damage control */

    // disable wdt
    TSB_WD_MOD_WDTE = 0U;
    TSB_WD->CR = 0x000000B1;

    if (file != NULL)
    {
        UART_SetTxData(TSB_SC3, *file);
        while ((UART_GetBufState(TSB_SC3, UART_TX)) == BUSY)
            ;
    }

    putcha(SCB->CFSR, 32);
    // putcha(SCB->HFSR, 32);
    // putcha(SCB->DFSR, 32);
    // putcha(SCB->BFAR, 32);

    NVIC_SystemReset(); /* reset the system */
}

static void putcha(uint32_t data, int n)
{
    while ((UART_GetBufState(TSB_SC3, UART_TX)) == BUSY)
        ;
    UART_SetTxData(TSB_SC3, 'x');
    while ((UART_GetBufState(TSB_SC3, UART_TX)) == BUSY)
        ;

    for (uint8_t i = 0; i < n; i++)
    {
        UART_SetTxData(TSB_SC3, 0X30 | (data & 0x01));
        data >>= 1;
        while ((UART_GetBufState(TSB_SC3, UART_TX)) == BUSY)
            ;
    }

    UART_SetTxData(TSB_SC3, '\n');
    while ((UART_GetBufState(TSB_SC3, UART_TX)) == BUSY)
        ;
    UART_SetTxData(TSB_SC3, '\r');
    while ((UART_GetBufState(TSB_SC3, UART_TX)) == BUSY)
        ;
}
