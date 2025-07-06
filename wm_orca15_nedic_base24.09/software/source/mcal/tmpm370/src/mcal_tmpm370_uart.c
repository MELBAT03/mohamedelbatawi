#include "mcal.h"
#include "board.h"
#include "core/tmpm370/inc/tmpm370_uart.h"

void mcal_uart_init(void)
{
    // do nothing
}

void mcal_uart_channel_set(mcal_uart_t x_uart, mcal_uartConfig_t *px_uartConfig)
{
    TSB_SC_TypeDef *uartx = TSB_SC0;
    UART_InitTypeDef conf;

    if (x_uart == MCAL_UART_UART0)
    {
        uartx = TSB_SC0;
        GPIO_SetOutputEnableReg(MCAL_GPIO_PORTE, MCAL_GPIO_PIN0, ENABLE);
        GPIO_EnableFuncReg(MCAL_GPIO_PORTE, GPIO_FUNC_REG_1, MCAL_GPIO_PIN0);
        GPIO_EnableFuncReg(MCAL_GPIO_PORTE, GPIO_FUNC_REG_1, MCAL_GPIO_PIN1);
        GPIO_SetInputEnableReg(MCAL_GPIO_PORTE, MCAL_GPIO_PIN1, ENABLE);
        if (px_uartConfig->rxInterruptEN == 1)
        {
            NVIC_EnableIRQ(INTRX0_IRQn);
        }
    }
    else if (x_uart == MCAL_UART_UART1)
    {
        uartx = TSB_SC1;
        GPIO_SetOutputEnableReg(MCAL_GPIO_PORTA, MCAL_GPIO_PIN5, ENABLE);
        GPIO_EnableFuncReg(MCAL_GPIO_PORTA, GPIO_FUNC_REG_1, MCAL_GPIO_PIN5);
        GPIO_EnableFuncReg(MCAL_GPIO_PORTA, GPIO_FUNC_REG_1, MCAL_GPIO_PIN6);
        GPIO_SetInputEnableReg(MCAL_GPIO_PORTA, MCAL_GPIO_PIN6, ENABLE);

        if (px_uartConfig->rxInterruptEN == 1)
        {
            NVIC_EnableIRQ(INTRX1_IRQn);
        }
    }
    else if (x_uart == MCAL_UART_UART2)
    {
        uartx = TSB_SC2;
        GPIO_SetOutputEnableReg(MCAL_GPIO_PORTD, MCAL_GPIO_PIN5, ENABLE);
        GPIO_EnableFuncReg(MCAL_GPIO_PORTD, GPIO_FUNC_REG_1, MCAL_GPIO_PIN5);
        GPIO_EnableFuncReg(MCAL_GPIO_PORTD, GPIO_FUNC_REG_1, MCAL_GPIO_PIN6);
        GPIO_SetInputEnableReg(MCAL_GPIO_PORTD, MCAL_GPIO_PIN6, ENABLE);
        if (px_uartConfig->rxInterruptEN == 1)
        {
            NVIC_EnableIRQ(INTRX2_IRQn);
        }
    }
    else if (x_uart == MCAL_UART_UART3)
    {
        uartx = TSB_SC3;
        GPIO_SetOutputEnableReg(MCAL_GPIO_PORTF, MCAL_GPIO_PIN3, ENABLE);
        GPIO_EnableFuncReg(MCAL_GPIO_PORTF, GPIO_FUNC_REG_2, MCAL_GPIO_PIN3);
        GPIO_EnableFuncReg(MCAL_GPIO_PORTF, GPIO_FUNC_REG_2, MCAL_GPIO_PIN4);
        GPIO_SetInputEnableReg(MCAL_GPIO_PORTF, MCAL_GPIO_PIN4, ENABLE);
        if (px_uartConfig->rxInterruptEN == 1)
        {
            NVIC_EnableIRQ(INTRX3_IRQn);
        }
    }
    else
    {
        /*  Do Nothing */
    }

    /* configure SIO0 for reception */
    UART_Enable(uartx);
    conf.BaudRate = px_uartConfig->baudRate;
    conf.DataBits = px_uartConfig->dataBits;
    conf.StopBits = px_uartConfig->stopBits;
    conf.Parity = px_uartConfig->parity;
    conf.Mode = px_uartConfig->mode;
    conf.FlowCtrl = MCAL_UART_FLOW_CTRL_NONE;

    UART_Init(uartx, (UART_InitTypeDef *)&conf);
}

void mcal_uart_data_put(mcal_uart_t x_uart, uint8_t u8_data)
{
    TSB_SC_TypeDef *uartx = NULL;
    uint32_t data = u8_data & 0x000000ff;
    static uint32_t timeoutCnt = 0;

    switch (x_uart)
    {
        case MCAL_UART_UART0:
            uartx = TSB_SC0;
            break;
        case MCAL_UART_UART1:
            uartx = TSB_SC1;
            break;
        case MCAL_UART_UART2:
            uartx = TSB_SC2;
            break;
        case MCAL_UART_UART3:
            uartx = TSB_SC3;
            break;
        default:
            // do nothing
            break;
    }

    if (uartx != NULL)
    {
        while ((UART_GetBufState(uartx, MCAL_UART_DIR_TX)) == BUSY)
        {
            timeoutCnt++;
            if (timeoutCnt > 80000U)
            {
                timeoutCnt = 0;
                break;
            }
        }

        UART_SetTxData(uartx, data);
    }
}

uint8_t mcal_uart_txBusyFlag_get(mcal_uart_t x_uart)
{
    uint8_t ret = 1;
    TSB_SC_TypeDef *uartx = TSB_SC0;

    switch (x_uart)
    {
        case MCAL_UART_UART0:
            uartx = TSB_SC0;
            break;
        case MCAL_UART_UART1:
            uartx = TSB_SC1;
            break;
        case MCAL_UART_UART2:
            uartx = TSB_SC2;
            break;
        case MCAL_UART_UART3:
            uartx = TSB_SC3;
            break;
        default:
            // do nothing
            break;
    }

    if (UART_GetBufState(uartx, MCAL_UART_DIR_TX) == BUSY)
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }

    return ret;
}

uint8_t mcal_uart_data_get(mcal_uart_t x_uart)
{
    uint8_t u8_data;
    TSB_SC_TypeDef *uartx = TSB_SC0;
    static volatile uint32_t timeoutCnt = 0;

    switch (x_uart)
    {
        case MCAL_UART_UART0:
            uartx = TSB_SC0;
            break;
        case MCAL_UART_UART1:
            uartx = TSB_SC1;
            break;
        case MCAL_UART_UART2:
            uartx = TSB_SC2;
            break;
        case MCAL_UART_UART3:
            uartx = TSB_SC3;
            break;
        default:
            // do nothing
            break;
    }

    if (uartx != NULL)
    {
        while ((UART_GetBufState(uartx, MCAL_UART_DIR_RX)) == BUSY)
        {
            timeoutCnt++;
            if (timeoutCnt > 80000U)
            {
                timeoutCnt = 0;
                break;
            }
        }
        u8_data = (uint8_t)UART_GetRxData(uartx);
    }
    return u8_data;
}

void mcal_uart_dma_init(void)
{
    // not available in this mcu
}

uint32_t mcal_uart_dma_set(uint8_t *pu8_txBuffer, uint32_t u32_length, uint32_t u32_channel)
{
    // not available in this mcu
    return 0xffffffff;
}

uint32_t mcal_uart_dma_get(uint8_t *pu8_rxBuffer, uint32_t u32_length, uint32_t u32_channel)
{
    // not available in this mcu
    return 0xffffffff;
}
