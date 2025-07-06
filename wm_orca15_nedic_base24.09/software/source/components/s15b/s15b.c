#include "board.h"

#include "def.h"
#include "os.h"

#include "components/s15b/s15b.h"
#include "components/uart/uart.h"
#include "dinput_module/dinput_module.h"
#include "handlers/input/handler_keypad.h"
#include "keypad/keypad.h"
#include "utils/utils.h"
// #include "components/ssd/ssd.h"

#define S15B_HEADER (0xA0)

static void s15b_packet_send(void);
static uint8_t s15b_crc_calculate(uint8_t *ptr, uint8_t size);

static s15b_state_t s15bState = S15B_RECEIVE;

static mcal_gpio_t resetSignal;
static uint8_t s15b_id;
static uint32_t s15b_totalTimeoutMS = 0;
static uint32_t s15b_resendCnt = 0;

static uint32_t gu32_s15b_keypadBuffer = 0;
static uint8_t gau8_s15b_rxBuffer[50] = {0}, gua8_s15b_rxDataBuffer[10] = {0};
static uint8_t seqNum = 0x00;

void s15b_init(s15b_config_t *s15)
{
    s15b_id = s15->s15bID;
}

void s15b_update(uint32_t period)
{
    static uint32_t internalRXTimeout = 0;
    int16_t ret = 0;
    static uint8_t rxCounter = 0;
    static uint32_t s15b_timeOutCnt = 0;

    switch (s15bState)
    {
        case S15B_RECEIVE:
        {
            s15b_timeOutCnt += period;
            if (uart_recv_check(s15b_id) == TRUE)
            {
                internalRXTimeout = 0;
                if ((ret = uart_bytesAvailable_get(s15b_id, &gau8_s15b_rxBuffer[rxCounter])) != -1)
                {
                    rxCounter += ret;
                }

                // :TODO: remove all magic numbers
                if (rxCounter >= 15)
                {
                    // :TODO: check incoming data
                    if (gau8_s15b_rxBuffer[0] != S15B_HEADER)
                    {
                        // LOG_W("Display Header Error");
                    }
                    else
                    {
                        // check seq number
                        seqNum = gau8_s15b_rxBuffer[1];

                        // check crc
                        if (gau8_s15b_rxBuffer[14] != s15b_crc_calculate(gau8_s15b_rxBuffer, 14))
                        {
                            // LOG_W("Display CRC Error");
                        }
                        else
                        {
                            for (uint8_t i = 0; i < 9; i++)
                            {
                                gua8_s15b_rxDataBuffer[i] = gau8_s15b_rxBuffer[i + 2];
                            }
                            s15bState = S15B_SEND;
                        }
                    }

                    rxCounter = 0;
                }
            }
        }
        break;

        case S15B_SEND:
        {
            s15b_timeOutCnt += period;
            s15b_packet_send();
            s15bState = S15B_RECEIVE;
        }
        break;

        default:
            // do nothing
            break;
    }
}

uint8_t s15b_buffer_get(uint8_t index)
{
    return gua8_s15b_rxDataBuffer[index];
}

void s15b_state_set(s15b_state_t value)
{
    s15bState = value;
}

static void s15b_packet_send(void)
{
    uint8_t dataBuff[7] = {0};
    dataBuff[0] = S15B_HEADER;
    dataBuff[1] = seqNum;
    dataBuff[2] = app_keypad_keyBuffer_get() & 0x000000ff;
    dataBuff[3] = (app_keypad_keyBuffer_get() & 0x0000ff00) >> 8;
    dataBuff[4] = (app_keypad_keyBuffer_get() & 0x00ff0000) >> 16;
    // dataBuff[5] = (app_keypad_keyBuffer_get() & 0xff000000) >> 24;
    dataBuff[5] = (WM_SW_VERSION_MAJOR * 10) + WM_SW_VERSION_MINOR;
    dataBuff[6] = s15b_crc_calculate(dataBuff, 6);

    for (uint8_t i = 0; i < 7; i++)
    {
        uart_byte_put(s15b_id, dataBuff[i]);
    }
}

static uint8_t s15b_crc_calculate(uint8_t *ptr, uint8_t size)
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
