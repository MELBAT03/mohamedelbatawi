/****************************************************************************
 * Title                 :   Communication base
 * Filename              :   com_base.c
 * Author                :   Amgd Raghed
 * Origin Date           :   2/27/2024
 * Version               :   1.0.0 (s15b Component Refactoring)
 * Compiler              :   TBD
 * Target                :   Component Layer
 *****************************************************************************/
/******************************************************************************
 * Includes
 *******************************************************************************/
#include "com_base.h"
#include "def.h"
#include "component_config.h"
#include "uart/uart.h"
#include "config.h"
#include "version.h"

/******************************************************************************
 * Module Preprocessor Macros
 *******************************************************************************/
#define BYTE (8U)
#define MASK (0xFFU)

/******************************************************************************
 * Module Variable Definitions
 *******************************************************************************/
static uint8_t gau8_com_base_rxBuffer[COM_BASE_PACKET_SIZE] = {0}, gua8_com_base_rxDataBuffer[COM_BASE_DATA_PACKET_SIZE] = {0};
static com_base_state_t com_base_state = COM_BASE_RESET;
static uint8_t com_base_id;
static uint8_t seqNum = 0x00;

static uint32_t (*ptr_functionKeypad_get)(void) = NULL;
static uint8_t (*ptr_functionRelease_get)(void) = NULL;

/******************************************************************************
 * Module Function Definitions
 *******************************************************************************/
static void com_base_packet_send(void);
static uint8_t com_base_crc_calculate(uint8_t *ptr, uint8_t size);

/******************************************************************************
 * Function Definitions
 *******************************************************************************/

static mcal_gpio_t com_base_resetSignal;

void com_base_init(com_base_config_t *com_base_config)
{
    com_base_id = com_base_config->commID;
    ptr_functionKeypad_get = com_base_config->ptr_keypadGeter;

    com_base_resetSignal.port = com_base_config->resetSignalPin.port;
    com_base_resetSignal.pin = com_base_config->resetSignalPin.pin;
    com_base_resetSignal.ioState = com_base_config->resetSignalPin.ioState;
    com_base_resetSignal.pinState = com_base_config->resetSignalPin.pinState;

    ptr_functionRelease_get = com_base_config->resetReleaseSignal;

    gpio_pinIOState_set(com_base_resetSignal.port, com_base_resetSignal.pin, MCAL_GPIO_OUTPUT);
    gpio_pinState_set(com_base_resetSignal.port, com_base_resetSignal.pin, MCAL_GPIO_LOW);
}

void com_base_update(uint32_t period)
{
    static uint8_t rxIndexCnt = 0;
    uint16_t returnValue = 0;
    uint8_t dataBuff[COM_DISPLAY_PACKET_SIZE] = {0};
    static uint32_t internalRXTimeout = 0;

    switch (com_base_state)
    {
        case COM_BASE_RESET:
        {
            com_base_reset();

            gpio_pinState_set(com_base_resetSignal.port, com_base_resetSignal.pin, MCAL_GPIO_LOW);

            if (ptr_functionRelease_get != NULL && 0U != ptr_functionRelease_get())
            {
                com_base_state = COM_BASE_RESET_RELEASE;
            }
        }
        break;

        case COM_BASE_RESET_RELEASE:
        {
            gpio_pinState_set(com_base_resetSignal.port, com_base_resetSignal.pin, MCAL_GPIO_HIGH);
            com_base_state = COM_BASE_SYNC;
        }
        break;

        case COM_BASE_SYNC:
        {
            if (uart_recv_check(com_base_id) == TRUE)
            {
                if ((returnValue = uart_bytesAvailable_get(com_base_id, &gau8_com_base_rxBuffer[rxIndexCnt])) != -1)
                {
                    rxIndexCnt += returnValue;
                }

                if (rxIndexCnt >= COM_BASE_PACKET_SIZE)
                {
                    for (uint8_t i = 0; i < COM_DISPLAY_PACKET_SIZE; i++)
                    {
                        if (gau8_com_base_rxBuffer[i] != 0xAA)
                        {
                            // com_base_state = COM_BASE_RESET;
                        }
                    }

                    com_base_state = COM_BASE_SYNC_ACK;
                }
            }
        }
        break;

        case COM_BASE_SYNC_ACK:
        {
            for (uint8_t i = 0; i < COM_DISPLAY_PACKET_SIZE; i++)
            {
                dataBuff[i] = 0xBB;
                uart_byte_put(com_base_id, dataBuff[i]);
            }
            com_base_state = COM_BASE_WAIT_HEADER;
            rxIndexCnt = 0;
        }
        break;

        case COM_BASE_WAIT_HEADER:
        {
            if ((uart_recv_check(com_base_id) == TRUE) && uart_byte_get(com_base_id) == HEADER_VALUE)
            {
                gau8_com_base_rxBuffer[0] = HEADER_VALUE;
                rxIndexCnt++;
                com_base_state = COM_BASE_RECEIVE;
            }
            else if ((uart_recv_check(com_base_id) == TRUE) && uart_byte_get(com_base_id) == 0xEE)
            {
                com_base_state = COM_BASE_RESET;
                rxIndexCnt = 0;
                com_base_reset();
            }
            else
            {
                // do nothing
            }
        }
        break;

        case COM_BASE_RECEIVE:
        {
            if (uart_recv_check(com_base_id) == TRUE)
            {
                internalRXTimeout = 0;
                if ((returnValue = uart_bytesAvailable_get(com_base_id, &gau8_com_base_rxBuffer[rxIndexCnt])) != -1)
                {
                    rxIndexCnt += returnValue;
                }

                /* RX Buffer recevied the display packet */
                if (rxIndexCnt >= COM_BASE_PACKET_SIZE)
                {
                    // Check incoming data
                    if (gau8_com_base_rxBuffer[COM_BASE_HEADER_BYTE_OFFSET] != HEADER_VALUE)
                    {
                        // LOG_W("Display Header Error");
                        com_base_state = COM_BASE_WAIT_HEADER;
                        rxIndexCnt = 0;
                        com_base_reset();
                    }
                    else
                    {
                        // Check Seq Number
                        seqNum = gau8_com_base_rxBuffer[COM_BASE_SEQ_NUM_BYTE_OFFSET];

                        // Check CRC
                        if (gau8_com_base_rxBuffer[COM_BASE_CRC_BYTE_OFFSET] != com_base_crc_calculate(gau8_com_base_rxBuffer, COM_BASE_PACKET_SIZE - 1))
                        {
                            // LOG_W("Display CRC Error");
                            com_base_state = COM_BASE_WAIT_HEADER;
                            rxIndexCnt = 0;
                            com_base_reset();
                        }
                        else
                        {
                            for (uint8_t i = 0; i < COM_BASE_DATA_PACKET_SIZE; i++)
                            {
                                gua8_com_base_rxDataBuffer[i] = gau8_com_base_rxBuffer[i + COM_DISPLAY_DATA_PACKET_OFFSET];
                            }
                            com_base_state = COM_BASE_SEND;
                        }
                    }

                    rxIndexCnt = 0;
                }
            }
        }
        break;

        case COM_BASE_SEND:
        {
            com_base_packet_send();
            com_base_state = COM_BASE_WAIT_HEADER;
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}

uint8_t com_base_buffer_get(uint8_t index)
{
    return gua8_com_base_rxDataBuffer[index];
}

void com_base_buffer_set(uint8_t index, uint8_t value)
{
    gua8_com_base_rxDataBuffer[index] = value;
}

void com_base_state_set(com_base_state_t stateValue)
{
    com_base_state = stateValue;
}

static void com_base_packet_send(void)
{
    uint8_t dataBuff[COM_DISPLAY_PACKET_SIZE] = {0};
    uint32_t keyBuffer;

    dataBuff[COM_DISPLAY_HEADER_BYTE_OFFSET] = HEADER_VALUE;
    dataBuff[COM_DISPLAY_SEQ_NUM_BYTE_OFFSET] = seqNum;

    keyBuffer = ptr_functionKeypad_get();
    for (uint32_t i = 0; i < COM_DISPLAY_KEY_BYTE_NUM; ++i)
    {
        dataBuff[i + COM_DISPLAY_KEYPAD_BYTE_OFFSET] = keyBuffer >> (BYTE * i);
    }

    dataBuff[COM_DISPLAY_VERSION_BYTE_OFFSET] = (WM_SW_VERSION_MAJOR * 10) + WM_SW_VERSION_MINOR;
    dataBuff[COM_DISPLAY_CRC_BYTE_OFFSET] = com_base_crc_calculate(dataBuff, COM_DISPLAY_PACKET_SIZE - CRC_SIZE);

    for (uint8_t i = 0; i < COM_DISPLAY_PACKET_SIZE; i++)
    {
        uart_byte_put(com_base_id, dataBuff[i]);
    }
}

static uint8_t com_base_crc_calculate(uint8_t *ptr, uint8_t size)
{
    uint8_t returnValue = 0;
    uint32_t crc = 0;

    for (uint8_t i = 0; i < size; i++)
    {
        crc += (uint32_t)ptr[i];
    }

    returnValue = (uint8_t)crc;

    return returnValue;
}

void com_base_reset(void)
{
    for (uint8_t i = 0; i < COM_BASE_DATA_PACKET_SIZE; i++)
    {
        gua8_com_base_rxDataBuffer[i] = 0;
    }
    gua8_com_base_rxDataBuffer[1] = 0x22;
    gua8_com_base_rxDataBuffer[2] = 0x22;
    gua8_com_base_rxDataBuffer[3] = 0x22;
    gua8_com_base_rxDataBuffer[4] = 0x22;
}
