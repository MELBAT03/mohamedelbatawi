#ifndef SSW_UART_H
#define SSW_UART_H 1

#include "def.h"

void ssw_uart_init(mcal_uartConfig_t *uartConfig);
void ssw_uart_send(uint8_t data);
uint8_t ssw_uart_get(void);
uint8_t ssw_uart_recv_check(void);
int16_t ssw_uart_bytesAvailable_get(uint8_t *pData);
uint8_t ssw_uart_txActiveFlag_get(void);
void ssw_uart_txActiveFlag_clear(void);
void ssw_uart_rxRdy_mark(void);
void ssw_uart_update(uint32_t period);

#endif