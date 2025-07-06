#ifndef UART_H
#define UART_H

#include "mcal.h"
#include "def.h"

typedef enum
{
    UART_0 = (uint32_t)0,
    UART_1 = (uint32_t)1,
    UART_2 = (uint32_t)2,
    UART_3 = (uint32_t)3,
    UART_SW = (uint32_t)4,
    UART_NONE = (uint32_t)0xff
} uart_t;

typedef mcal_uartConfig_t uart_config_t;

typedef mcal_uart_t uart_interface_t;
#define UART_INTERFACE_0 (MCAL_UART_UART0)
#define UART_INTERFACE_1 (MCAL_UART_UART1)
#define UART_INTERFACE_2 (MCAL_UART_UART2)
#define UART_INTERFACE_3 (MCAL_UART_UART3)

typedef mcal_uartDataBitsEnum_t uart_dataBits_t;
#define UART_DATA_LEN_7_BIT (MCAL_UART_DATA_BITS_7)
#define UART_DATA_LEN_8_BIT (MCAL_UART_DATA_BITS_8)
#define UART_DATA_LEN_9_BIT (MCAL_UART_DATA_BITS_9)

typedef mcal_uartStopBitsEnum_t uart_stopBits_t;
#define UART_STOP_LEN_1_BIT (MCAL_UART_STOP_BITS_1)
#define UART_STOP_LEN_2_BIT (MCAL_UART_STOP_BITS_2)

typedef mcal_uartParityEnum_t uart_parity_t;
#define UART_PARITY_NONE (MCAL_UART_PARITY_NO)
#define UART_PARITY_EVEN (MCAL_UART_PARITY_EVEN)
#define UART_PARITY_ODD  (MCAL_UART_PARITY_ODD)

typedef mcal_uartModeEnum_t uart_mode_t;
#define UART_MODE_RX   (MCAL_UART_MODE_RX)
#define UART_MODE_TX   (MCAL_UART_MODE_TX)
#define UART_MODE_TXRX (UART_MODE_TX | UART_MODE_RX)

typedef mcal_uartFlowControlEnum_t uart_flowControl_t;
#define UART_FLOW_CONTROL_NONE (MCAL_UART_FLOW_CTRL_NONE)

typedef mcal_uartDirectionEnum_t uart_direction_t;
#define UART_DIR_RX (MCAL_UART_DIR_RX)
#define UART_DIR_TX (MCAL_UART_DIR_TX)

void uart_init(uart_t uartx, uart_config_t *uartConfig);
void uart_deinit(void);
void uart_update(uint32_t period);
int8_t uart_dataSent_check(uart_t uartx);
void uart_byte_put(uart_t uartx, const uint8_t character);
void uart_reset(uart_t uartx);
uint8_t uart_byte_get(uart_t uartx);
void uart_data_put(uart_t uartx, uint8_t *pData);
void uart_string_get(uart_t uartx, uint8_t *pString);
void uart_data_get(uart_t uartx, uint8_t *pData, uint8_t size);
int16_t uart_bytesAvailable_get(uart_t uartx, uint8_t *pData);
bool_t uart_recv_check(uart_t uartx);
void uart_recv_int(uart_t uartx);

void uart_multiplier_get(uint8_t *pData);

#endif
