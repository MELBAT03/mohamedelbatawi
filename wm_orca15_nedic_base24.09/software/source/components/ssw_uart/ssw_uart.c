#include "ssw_uart.h"
#include "def.h"
#include "driver/gpio/gpio.h"
#include "driver/uart/uart.h"
#include "utils/utils.h"

typedef enum
{
    TX_STATE_IDLE,
    TX_STATE_START,
    TX_STATE_DATA,
    TX_STATE_STOP
} ssw_uart_txState_t;

typedef enum
{
    RX_STATE_START,
    RX_STATE_DATA,
    RX_STATE_STOP
} ssw_uart_rxState_t;

ssw_uart_txState_t uartTXState;
ssw_uart_rxState_t uartRXState;

static uint8_t txBuffer[10];
static uint8_t rxBuff[10];
static uint32_t baudCounter, rxBaudCounter;
static uint8_t txInputIndex = 0, txOutputIndex = 0;
static uint8_t rxInputIndex = 0, rxOutputIndex = 0;
static uint8_t gu8_txActive_flag = 0, gu8_rxActive_flag = 0;
static uint8_t gu8_rxAvailable_flag = 0;
static uint8_t inputData, outputData, txRequest = 0;

static mcal_gpio_t gx_ssw_uartTXPort, gx_ssw_uartRXPort;

void ssw_uart_init(mcal_uartConfig_t *uartConfig)
{
    uint8_t i;
    baudCounter = 1000 / uartConfig->baudRate;
    rxBaudCounter = baudCounter;
    gu8_txActive_flag = 0;
    uartTXState = TX_STATE_IDLE;
    uartRXState = RX_STATE_START;
    inputData = 0;

    gx_ssw_uartTXPort.port = uartConfig->txPin.port;
    gx_ssw_uartTXPort.pin = uartConfig->txPin.pin;
    gx_ssw_uartRXPort.port = uartConfig->rxPin.port;
    gx_ssw_uartRXPort.pin = uartConfig->rxPin.pin;

    uartConfig->txPin.ioState = GPIO_OUTPUT;
    uartConfig->txPin.pinState = GPIO_HIGH;
    uartConfig->txPin.extIntState = MCAL_GPIO_EXTINT_DISABLE;
    uartConfig->txPin.extIntPolarity = MCAL_GPIO_EXTINT_NONE;
    uartConfig->rxPin.ioState = GPIO_INPUT;
    uartConfig->rxPin.pinState = MCAL_GPIO_NONE;
    uartConfig->rxPin.extIntState = MCAL_GPIO_EXTINT_ENABLE;
    uartConfig->rxPin.extIntPolarity = MCAL_GPIO_EXTINT_FALLING_EDGE;

    for (i = 0; i < 10; i++)
    {
        txBuffer[i] = 0;
        rxBuff[i] = 0;
    }

    mcal_gpio_pin_init(&uartConfig->txPin);
    mcal_gpio_pin_init(&uartConfig->rxPin);
}

void ssw_uart_send(uint8_t data)
{
    // while (sw_uart_txActiveFlag_get())
    //     ;
    txBuffer[txInputIndex] = data;
    txInputIndex++;
    // outputData = data;
    // txRequest = 1;
}

uint8_t ssw_uart_get(void)
{
    uint8_t data, i = 0;

    data = rxBuff[rxOutputIndex++];
    return data;
}

uint8_t ssw_uart_recv_check(void)
{
    return rxInputIndex - rxOutputIndex;
}

int16_t ssw_uart_bytesAvailable_get(uint8_t *pData)
{
    uint8_t j = 0, i = 0;

    while (rxOutputIndex < rxInputIndex)
    {
        pData[j] = rxBuff[rxOutputIndex++];
        j++;
    }

    return j;
}

uint8_t ssw_uart_txActiveFlag_get(void)
{
    return gu8_txActive_flag;
}

void ssw_uart_txActiveFlag_clear(void)
{
    gu8_txActive_flag = 0;
}

void ssw_uart_update(uint32_t period)
{
    static uint8_t i = 0;
    static uint8_t j = 0;
    uint8_t k = 0;
    static uint32_t txInternalCounter = 0, rxInternalCounter = 0;
    uint8_t buff[10] = {0};
    static mcal_gpio_stateEnum_t txPinValue = GPIO_HIGH;

    mcal_gpio_pinState_set(&gx_ssw_uartTXPort, txPinValue);

    // RX PART
    if (gu8_rxAvailable_flag == 1)
    {
        rxInternalCounter += period;
        if (rxInternalCounter >= ((rxBaudCounter + 1) >> 1))
        {
            rxInternalCounter = 0;

            switch (uartRXState)
            {
                case RX_STATE_START:
                    gu8_rxActive_flag = 1;
                    rxBaudCounter = baudCounter * 2;
                    inputData = 0;
                    uartRXState = RX_STATE_DATA;
                    break;

                case RX_STATE_DATA:
                    inputData = inputData | (mcal_gpio_pinState_get(&gx_ssw_uartRXPort) << j);
                    j++;
                    if (j == 8)
                    {
                        j = 0;
                        uartRXState = RX_STATE_STOP;
                    }
                    break;

                case RX_STATE_STOP:
                    gu8_rxActive_flag = 0;
                    gu8_rxAvailable_flag = 0;
                    rxBaudCounter = baudCounter;
                    rxBuff[rxInputIndex] = inputData;
                    rxInputIndex++;
                    // if (rxInputIndex >= 20)
                    // {
                    //     rxInputIndex = 0;
                    // }
                    uartRXState = RX_STATE_START;
                    break;

                default:
                    // do nothing
                    break;
            }
        }
    }

    if (rxInputIndex == rxOutputIndex)
    {
        rxInputIndex = 0;
        rxOutputIndex = 0;
    }

    // TX PART
    txInternalCounter += period;
    if (txInternalCounter >= baudCounter)
    {
        txInternalCounter = 0;

        switch (uartTXState)
        {
            case TX_STATE_IDLE:
                mcal_gpio_pinState_set(&gx_ssw_uartTXPort, GPIO_HIGH);
                txPinValue = GPIO_HIGH;

                if (txInputIndex > 0)
                {
                    uartTXState = TX_STATE_START;
                }
                break;

            case TX_STATE_START:
                mcal_gpio_pinState_set(&gx_ssw_uartTXPort, GPIO_LOW);
                txPinValue = GPIO_LOW;
                gu8_txActive_flag = 1;
                uartTXState = TX_STATE_DATA;
                break;

            case TX_STATE_DATA:
                mcal_gpio_pinState_set(&gx_ssw_uartTXPort, (txBuffer[txOutputIndex] >> i) & 0x01);
                txPinValue = (txBuffer[txOutputIndex] >> i) & 0x01;
                i++;
                if (i == 8)
                {
                    i = 0;
                    uartTXState = TX_STATE_STOP;
                }
                break;

            case TX_STATE_STOP:
                mcal_gpio_pinState_set(&gx_ssw_uartTXPort, GPIO_HIGH);
                txPinValue = GPIO_HIGH;
                gu8_txActive_flag = 0;
                txOutputIndex++;
                // txRequest = 0;
                uartTXState = TX_STATE_IDLE;
                break;

            default:
                // do nothing
                break;
        }
    }
    if (txInputIndex == txOutputIndex)
    {
        txInputIndex = 0;
        txOutputIndex = 0;
    }
}

void ssw_uart_rxRdy_mark(void)
{
    gu8_rxAvailable_flag = 1;
}
