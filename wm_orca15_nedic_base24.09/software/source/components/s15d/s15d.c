#include "board.h"

#include "def.h"
#include "os.h"

#include "components/s15d/s15d.h"
#include "components/ssd/ssd.h"
#include "components/uart/uart.h"
#include "components/dinput_module/dinput_module.h"
#include "handlers/output/inc/handler_display.h"
#include "components/keypad/keypad.h"
#include "utils.h"

#define S15D_HEADER (0xA0)

static void s15d_packet_send(void);
static uint8_t s15d_crc_calculate(uint8_t *ptr, uint8_t size);

static s15d_state_t s15dState = S15D_RESET_DONE;

static mcal_gpio_t resetSignal;
static uint8_t s15d_id;
static uint32_t s15d_totalTimeoutMS = 0;
static uint32_t s15d_resendCnt = 0;
static uint8_t s15d_displayVersion = 0;
static uint32_t s15d_frameTimeMSec;

static uint32_t gu32_s15d_keypadBuffer = 0;
static uint8_t gau8_s15d_rxBuffer[50] = {0};
static uint8_t seqNum = 0x00;

void s15d_init(const s15d_config_t *s15)
{
    resetSignal.port = s15->port.port;
    resetSignal.pin = s15->port.pin;
    resetSignal.ioState = s15->port.ioState;
    resetSignal.pinState = s15->port.pinState;

    s15d_id = s15->s15dID;

    s15d_frameTimeMSec = s15->frameTimeMSec;
    s15d_totalTimeoutMS = s15->timeoutMSec;

    mcal_gpio_pin_init(&resetSignal);

    mcal_gpio_pinState_set(&resetSignal, MCAL_GPIO_HIGH);   // in order to avoid the display firing before the base
}

void s15d_update(uint32_t period)
{
    static uint32_t internalRXTimeout = 0;
    int16_t ret = 0;
    static uint8_t rxCounter = 0;
    static uint32_t s15d_timeOutCnt = 0;

    switch (s15dState)
    {
        case S15D_RESET_INIT:
            mcal_gpio_pinState_set(&resetSignal, MCAL_GPIO_HIGH);
            s15dState = S15D_RESET_DONE;
            break;

        case S15D_RESET_DONE:
            mcal_gpio_pinState_set(&resetSignal, MCAL_GPIO_LOW);
            s15dState = S15D_INTERVAL_WAIT;
            break;

        case S15D_RESEND:
            s15d_timeOutCnt += period;
            s15d_resendCnt++;
            if (s15d_resendCnt >= 120)
            {
                s15dState = S15D_ERROR;
                LOG('0', 'w', "DError");
            }
            else if (s15d_resendCnt >= 100)
            {
                s15dState = S15D_RESET_INIT;
                LOG('0', 'w', "DReset");
            }
            else
            {
                s15dState = S15D_SEND;
                LOG('0', 'w', "DResend %d", s15d_resendCnt);
            }
            break;

        case S15D_SEND:
            s15d_timeOutCnt += period;
            s15d_packet_send();
            s15dState = S15D_RECEIVE;
            break;

        case S15D_RECEIVE:
            s15d_timeOutCnt += period;
            if (uart_recv_check(s15d_id) == TRUE)
            {
                internalRXTimeout = 0;
                if ((ret = uart_bytesAvailable_get(s15d_id, &gau8_s15d_rxBuffer[rxCounter])) != -1)
                {
                    rxCounter += ret;
                }

                // :TODO: remove all magic numbers
                if (rxCounter >= 7)
                {
                    // check header
                    if (gau8_s15d_rxBuffer[0] != S15D_HEADER)
                    {
                        s15dState = S15D_RESEND;
                        LOG('0', 'w', "DHeader Error");
                    }
                    else
                    {
                        // check seq number
                        if (gau8_s15d_rxBuffer[1] != seqNum)
                        {
                            s15dState = S15D_RESEND;
                            LOG('0', 'w', "DSeq Number Error");
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

                            // check crc
                            if (gau8_s15d_rxBuffer[6] != s15d_crc_calculate(gau8_s15d_rxBuffer, 6))
                            {
                                s15dState = S15D_RESEND;
                                LOG('0', 'w', "DCRC Error");
                            }
                            else
                            {
                                gu32_s15d_keypadBuffer = 0x00000000 | (uint32_t)(gau8_s15d_rxBuffer[2]) | ((uint32_t)(gau8_s15d_rxBuffer[3]) << 8) | ((uint32_t)(gau8_s15d_rxBuffer[4]) << 16);   // :TODO: will be used for error?? ((uint32_t)(gau8_s15d_rxBuffer[5]) << 24);

                                s15d_displayVersion = gau8_s15d_rxBuffer[5];

                                s15dState = S15D_INTERVAL_WAIT;
                                s15d_resendCnt = 0;
                            }
                        }
                    }

                    rxCounter = 0;
                }
                LOG('d', 'v', "s15d %2x %2x %x %x %x %x %2x", gau8_s15d_rxBuffer[0], gau8_s15d_rxBuffer[1], gau8_s15d_rxBuffer[2], gau8_s15d_rxBuffer[3], gau8_s15d_rxBuffer[4], gau8_s15d_rxBuffer[5], gau8_s15d_rxBuffer[6]);
            }
            else
            {
                internalRXTimeout += period;
                if (internalRXTimeout >= s15d_totalTimeoutMS)
                {
                    LOG('0', 'w', "DTimeout");
                    internalRXTimeout = 0;
                    s15dState = S15D_RESEND;
                    rxCounter = 0;
                    for (uint8_t i = 0; i < 7; i++)
                    {
                        gau8_s15d_rxBuffer[i] = 0;
                    }
                }
            }
            break;

        case S15D_INTERVAL_WAIT:
            s15d_timeOutCnt += period;
            if (s15d_timeOutCnt >= s15d_frameTimeMSec)
            {
                s15dState = S15D_SEND;
                s15d_timeOutCnt = 0;
            }
            break;

        case S15D_ERROR:
            // do nothing
            // :TODO: add an error reporting mechanism
            break;

        case S15D_DONE:
            mcal_gpio_pinState_set(&resetSignal, MCAL_GPIO_HIGH);
            break;

        default:
            // do nothing
            break;
    }
}

uint32_t *s15d_keypadBuffer_get(void)
{
    return &gu32_s15d_keypadBuffer;
}

uint8_t s15d_displayVersion_get(void)
{
    return s15d_displayVersion;
}

void s15d_state_set(s15d_state_t value)
{
    s15dState = value;
}

s15d_state_t s15d_state_get(void)
{
    return s15dState;
}

static void s15d_packet_send(void)
{
    uint8_t dataBuff[15] = {0};

    dataBuff[0] = S15D_HEADER;
    dataBuff[1] = seqNum;

    dataBuff[2] = handler_display_ssd_get(0);   // + dp
    dataBuff[3] = handler_display_ssd_get(1);   // + dp
    dataBuff[4] = handler_display_ssd_get(2);
    dataBuff[5] = handler_display_ssd_get(3);

    for (uint8_t i = 0; i < LED_MATRIX_LED_NUM; i++)
    {
        if (i < 8)
        {
            dataBuff[6] |= (handler_display_ledState_get(i) << i);
        }
        else if (i >= 8 && i < 16)
        {
            dataBuff[7] |= (handler_display_ledState_get(i) << (i - 8));
        }
        else if (i >= 16 && i < 24)
        {
            dataBuff[8] |= (handler_display_ledState_get(i) << (i - 16));
        }
        else if (i >= 24 && i < 32)
        {
            dataBuff[9] |= (handler_display_ledState_get(i) << (i - 24));
        }
        else if (i >= 32 && i < 40)
        {
            dataBuff[10] |= (handler_display_ledState_get(i) << (i - 32));
        }
    }

    dataBuff[11] = 0;
    dataBuff[12] = 0;
    dataBuff[13] = 0;

    dataBuff[14] = s15d_crc_calculate(dataBuff, 14);

    for (uint8_t i = 0; i < 15; i++)
    {
        uart_byte_put(s15d_id, dataBuff[i]);
    }
}

static uint8_t s15d_crc_calculate(uint8_t *ptr, uint8_t size)
{
    uint8_t ret = 0;
    uint32_t crc = 0;

    for (uint8_t i = 1; i < size; i++)
    {
        crc += (uint32_t)ptr[i];
    }

    ret = (uint8_t)crc;

    return ret;
}
