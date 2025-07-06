/****************************************************************************
 * Title                 :   Communication Dryer11/13Kg Display
 * Filename              :   com_display.c
 * Author                :   Amgd Raghed
 * Origin Date           :   2/21/2024
 * Version               :   1.0.0 (s15d Component Refactoring)
 * Compiler              :   TBD
 * Target                :   Component Layer
 *****************************************************************************/
/******************************************************************************
 * Includes
 *******************************************************************************/
#include "com_display.h"
#include "component_config.h"
#include "uart/uart.h"

#include "os_log.h"

/******************************************************************************
 * Module Preprocessor Macros
 *******************************************************************************/
#define PACKET_BYTE           (8U)
#define COM_DISPLAY_ERROR_CNT (100U)

/******************************************************************************
 * Module Variable Definitions
 *******************************************************************************/
static uint8_t com_display_id;
static uint32_t com_display_frameWaitTimeMSec;
static uint32_t com_display_receiveTimeoutMSec;
static uint8_t seqNum = 0x00;
static uint8_t com_display_version = 0;
static uint32_t gu32_com_display_keypadBuffer = 0;
static uint8_t gau8_com_display_rxBuffer[COM_DISPLAY_PACKET_SIZE] = {0};
static uint8_t gau8_com_display_txBuffer[COM_BASE_PACKET_SIZE] = {0};

static com_display_state_t com_display_state = COM_DISPLAY_SYNC;

static uint8_t (*ptr_functionDim_get)(void) = NULL;
static uint8_t (*ptr_functionSSD_get)(uint8_t) = NULL;
static uint8_t (*ptr_functionDP_get)(void) = NULL;
static uint8_t (*ptr_functionLED_get)(uint8_t) = NULL;

/******************************************************************************
 * Module Function Definitions
 *******************************************************************************/
static void com_display_sync(void);
static uint8_t com_display_sync_check(void);
static void com_display_reset(void);
static void com_display_packet_send(void);
static uint8_t com_display_crc_calculate(uint8_t *ptr, uint8_t size);
static void com_display_handleReceivedPacket(uint8_t *rxIndexCnt, uint32_t *com_display_resendCnt);

/******************************************************************************
 * Function Definitions
 *******************************************************************************/
void com_display_init(const com_display_config_t *com_display_config)
{
    ptr_functionDim_get = com_display_config->ptr_dimGeter;
    ptr_functionSSD_get = com_display_config->ptr_ssdGeter;
    ptr_functionDP_get = com_display_config->ptr_dpGeter;
    ptr_functionLED_get = com_display_config->ptr_ledGeter;

    com_display_id = com_display_config->commDisplayID;

    com_display_frameWaitTimeMSec = com_display_config->frameWaitTimeMSec;
    com_display_receiveTimeoutMSec = com_display_config->receiveTimeoutMSec;
}

void com_display_update(uint32_t period)
{
    int16_t returnValue = 0;
    static uint8_t rxIndexCnt = 0;
    static uint32_t internalRXTimeout = 0;
    static uint32_t com_display_timeOutCnt = 0;
    static uint32_t com_display_resendCnt = 0;

    switch (com_display_state)
    {
        case COM_DISPLAY_SYNC:
        {
            com_display_sync();
            com_display_state = COM_DISPLAY_SYNC_CHECK;
        }
        break;

        case COM_DISPLAY_SYNC_CHECK:
        {
            if (uart_recv_check((uart_t)com_display_id) == TRUE)
            {
                internalRXTimeout = 0;
                if ((returnValue = uart_bytesAvailable_get((uart_t)com_display_id, &gau8_com_display_rxBuffer[rxIndexCnt])) != -1)
                {
                    rxIndexCnt += returnValue;
                }

                if (rxIndexCnt >= COM_DISPLAY_PACKET_SIZE)
                {
                    if (com_display_sync_check() == 0U)
                    {
                        gu32_com_display_keypadBuffer = 0xffffffff;
                        com_display_state = COM_DISPLAY_INTERVAL_WAIT;
                        rxIndexCnt = 0;
                    }
                    else
                    {
                        com_display_state = COM_DISPLAY_SYNC;
                    }
                }
            }
        }
        break;

        /* Interval Wait Time between each packet send */
        case COM_DISPLAY_INTERVAL_WAIT:
        {
            com_display_timeOutCnt += period;
            if (com_display_timeOutCnt >= com_display_frameWaitTimeMSec)
            {
                com_display_state = COM_DISPLAY_SEND;
                com_display_timeOutCnt = 0;
            }
        }
        break;

        /* Send state */
        case COM_DISPLAY_SEND:
        {
            com_display_timeOutCnt += period;
            com_display_packet_send();
            com_display_state = COM_DISPLAY_RECEIVE;
        }
        break;

        /* Receive State */
        case COM_DISPLAY_RECEIVE:
        {
            com_display_timeOutCnt += period;

            if (uart_recv_check((uart_t)com_display_id) == TRUE)
            {
                internalRXTimeout = 0;
                if ((returnValue = uart_bytesAvailable_get((uart_t)com_display_id, &gau8_com_display_rxBuffer[rxIndexCnt])) != -1)
                {
                    rxIndexCnt += returnValue;
                }

                if (rxIndexCnt >= COM_DISPLAY_PACKET_SIZE)
                {
                    com_display_handleReceivedPacket(&rxIndexCnt, &com_display_resendCnt);
                }
            }
            /* No Response Handling */
            else
            {
                internalRXTimeout += period;
                if (internalRXTimeout >= com_display_receiveTimeoutMSec)
                {
                    internalRXTimeout = 0;
                    com_display_state = COM_DISPLAY_RESEND;
                    rxIndexCnt = 0;
                    for (uint8_t i = 0; i < COM_DISPLAY_PACKET_SIZE; i++)
                    {
                        gau8_com_display_rxBuffer[i] = 0;
                    }
                }
            }
        }
        break;

        case COM_DISPLAY_RESEND:
        {
            com_display_timeOutCnt += period;
            com_display_resendCnt++;

            if (com_display_resendCnt >= COM_DISPLAY_ERROR_CNT)
            {
                com_display_state = COM_DISPLAY_ERROR;
            }
            else
            {
                com_display_state = COM_DISPLAY_INTERVAL_WAIT;
            }
        }
        break;

        case COM_DISPLAY_ERROR:
        {
            // do nothing
        }
        break;

        case COM_DISPLAY_DONE:
        {
            com_display_reset();
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }

    LOG('d', 'v', "dispCom [%d] TX: %2x %2x  [%2x %2x %2x %2x %2x %2x %2x %2x %2x %2x]  %2x           RX: %2x %2x %2x %2x %2x %2x  Keys: %x", com_display_state, gau8_com_display_txBuffer[0], gau8_com_display_txBuffer[1], gau8_com_display_txBuffer[2], gau8_com_display_txBuffer[3], gau8_com_display_txBuffer[4], gau8_com_display_txBuffer[5], gau8_com_display_txBuffer[6], gau8_com_display_txBuffer[7], gau8_com_display_txBuffer[8], gau8_com_display_txBuffer[9], gau8_com_display_txBuffer[10], gau8_com_display_txBuffer[11], gau8_com_display_txBuffer[12], gau8_com_display_rxBuffer[0], gau8_com_display_rxBuffer[1], gau8_com_display_rxBuffer[2], gau8_com_display_rxBuffer[3], gau8_com_display_rxBuffer[4], gau8_com_display_rxBuffer[5], gu32_com_display_keypadBuffer);
}

com_display_state_t com_display_state_get(void)
{
    return com_display_state;
}

void com_display_state_set(com_display_state_t value)
{
    com_display_state = value;
}

uint32_t *com_display_keypadBuffer_get(void)
{
    return &gu32_com_display_keypadBuffer;
}

uint8_t com_display_displayVersion_get(void)
{
    return com_display_version;
}

static void com_display_sync(void)
{
    uint8_t dataBuff[COM_BASE_PACKET_SIZE] = {0};

    for (uint8_t i = 0; i < COM_BASE_PACKET_SIZE; i++)
    {
        dataBuff[i] = 0xAA;
    }

    for (uint8_t i = 0; i < COM_BASE_PACKET_SIZE; i++)
    {
        uart_byte_put((uart_t)com_display_id, dataBuff[i]);
        gau8_com_display_txBuffer[i] = dataBuff[i];
    }
}

static uint8_t com_display_sync_check(void)
{
    uint8_t ret = 0;

    for (uint8_t i = 0; i < COM_DISPLAY_PACKET_SIZE; i++)
    {
        if (gau8_com_display_rxBuffer[i] != 0xBB)
        {
            ret = 1;
        }
    }

    return ret;
}

static void com_display_reset(void)
{
    uint8_t dataBuff[COM_BASE_PACKET_SIZE] = {0};

    for (uint8_t i = 0; i < COM_BASE_PACKET_SIZE; i++)
    {
        dataBuff[i] = 0xEE;
    }

    for (uint8_t i = 0; i < COM_BASE_PACKET_SIZE; i++)
    {
        uart_byte_put((uart_t)com_display_id, dataBuff[i]);
        gau8_com_display_txBuffer[i] = dataBuff[i];
    }
}

static void com_display_packet_send(void)
{
    uint8_t dataBuff[COM_BASE_PACKET_SIZE] = {0};

    dataBuff[COM_BASE_HEADER_BYTE_OFFSET] = HEADER_VALUE;
    dataBuff[COM_BASE_SEQ_NUM_BYTE_OFFSET] = seqNum;

    dataBuff[COM_BASE_DIM_ENABLE_BYTE_OFFSET] = ptr_functionDim_get();

    /* SSD Data Packing */
    for (uint8_t i = 0; i < COM_BASE_SSD_NUM; i++)
    {
        dataBuff[i + COM_BASE_SSD_BYTE_OFFSET] = ptr_functionSSD_get(i);
    }

    dataBuff[COM_BASE_SSD_BYTE_OFFSET + COM_BASE_SSD_NUM] = ptr_functionDP_get();

    /* LED Data Packing */
    for (uint8_t i = 0; i < COM_BASE_LED_NUM; i++)
    {
        uint8_t index = i / PACKET_BYTE;

        dataBuff[index + COM_BASE_LED_BYTE_OFFSET] |= (ptr_functionLED_get(i) << (i % PACKET_BYTE));
    }

    dataBuff[COM_BASE_CRC_BYTE_OFFSET] = com_display_crc_calculate(dataBuff, COM_BASE_PACKET_SIZE - CRC_SIZE);

    for (uint8_t i = 0; i < COM_BASE_PACKET_SIZE; i++)
    {
        uart_byte_put((uart_t)com_display_id, dataBuff[i]);
        gau8_com_display_txBuffer[i] = dataBuff[i];
    }
}

static void com_display_handleReceivedPacket(uint8_t *rxIndexCnt, uint32_t *resendCnt)
{
    /* Check Header */
    if (gau8_com_display_rxBuffer[COM_DISPLAY_HEADER_BYTE_OFFSET] != HEADER_VALUE)
    {
        com_display_state = COM_DISPLAY_RESEND;
    }
    else
    {
        /* Check seqNum */
        if (gau8_com_display_rxBuffer[COM_DISPLAY_SEQ_NUM_BYTE_OFFSET] != seqNum)
        {
            com_display_state = COM_DISPLAY_RESEND;
        }
        else
        {
            if (seqNum < 0xff)
            {
                seqNum++;
            }
            else
            {
                seqNum = 0;
            }

            /* Check CRC */
            if (gau8_com_display_rxBuffer[COM_DISPLAY_CRC_BYTE_OFFSET] != com_display_crc_calculate(gau8_com_display_rxBuffer, COM_DISPLAY_CRC_BYTE_OFFSET))
            {
                com_display_state = COM_DISPLAY_RESEND;
            }
            /* All checks is okay. Packet arrived undamaged and it can be used */
            else
            {
                gu32_com_display_keypadBuffer = 0x00000000U;

                for (uint8_t i = 0; i < COM_DISPLAY_KEY_BYTE_NUM; i++)
                {
                    gu32_com_display_keypadBuffer |= ((uint32_t)(gau8_com_display_rxBuffer[COM_DISPLAY_KEYPAD_BYTE_OFFSET + i]) << (PACKET_BYTE * i));
                }

                com_display_version = gau8_com_display_rxBuffer[COM_DISPLAY_VERSION_BYTE_OFFSET];

                com_display_state = COM_DISPLAY_INTERVAL_WAIT;
                *resendCnt = 0;
            }
        }
        *rxIndexCnt = 0;
    }
}

static uint8_t com_display_crc_calculate(uint8_t *ptr, uint8_t size)
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
