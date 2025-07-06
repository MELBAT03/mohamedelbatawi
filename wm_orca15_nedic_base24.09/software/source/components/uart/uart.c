#include "component_config.h"
#include "uart/uart.h"
#include "mcal.h"
#include "def.h"

// #include "components/ssw_uart/ssw_uart.h"

#define UART_UPDATE_TIME_UTILIZATION_FACTOR(x) (((x) * (20.0f)) / (100.0f))
#define UART_CONVERT_BPS_TO_MSEC(x)            ((10000.0f) / (x))
#define UART_CONVERT_BPS_TO_USEC(x)            ((10000000.0f) / (x))

static uint8_t gx_uart_interfacesCount = 0;

typedef struct
{
    uart_t actualInterface;
    uart_t virtualInterface;
    uint8_t multiplier;
    uint8_t counter;
    int8_t dataSentFlag;
} uart_mappingStructure_t;

static uart_mappingStructure_t gx_uart_map[UART_INTERFACES_NO_LOG_NUM];

// :NOTE: always the idx must be higher in value than its corresponding id0
static uint32_t gau32_uart_recvBufID0[UART_INTERFACES_NO_LOG_NUM];   // index at which the s/w will read from
static uint32_t gau32_uart_recvBufIDx[UART_INTERFACES_NO_LOG_NUM];   // index at which the h/w will write received data to

static uint32_t gau32_uart_transBufID0[UART_INTERFACES_NO_LOG_NUM];   // index at which the h/w will read what data to send
static uint32_t gau32_uart_transBufIDx[UART_INTERFACES_NO_LOG_NUM];   // index at which the s/w will put data to be sent later by h/w

static uint8_t gaau8_uart_recvBuffer[UART_INTERFACES_NO_LOG_NUM][UART_RECV_BUFFER_SIZE];
static uint8_t gaau8_uart_transBuffer[UART_INTERFACES_NO_LOG_NUM][UART_TRANS_BUFFER_SIZE];

#ifdef LOG_ENABLE
static uint32_t gau32_uart_recvLogBufID0;    // index at which the s/w will read from
static uint32_t gau32_uart_recvLogBufIDx;    // index at which the h/w will write received data to
static uint32_t gau32_uart_transLogBufID0;   // index at which the h/w will read what data to send
static uint32_t gau32_uart_transLogBufIDx;   // index at which the s/w will put data to be sent later by h/w
static uint8_t gaau8_uart_recvLogBuffer[UART_LOG_RECV_BUFFER_SIZE];
static uint8_t gaau8_uart_transLogBuffer[UART_LOG_TRANS_BUFFER_SIZE];
static int8_t g8_uart_logChannelDataSent_flag = 1;
#endif

// static bool_t gb_uart_initDone_flag = FALSE;

static void uart_dataReg_put(uart_t uartx, const uint8_t character);
static void uart_dataReg_get(uart_t uartx, uart_t vInterface);

void uart_init(uart_t uartx, uart_config_t *uartConfig)
{
#ifdef LOG_ENABLE
    if (uartx != LOG_CHANNEL_ID)
    {
        gx_uart_map[gx_uart_interfacesCount].actualInterface = uartx;
        gx_uart_map[gx_uart_interfacesCount].virtualInterface = (uart_t)gx_uart_interfacesCount;

        if (uartConfig->chainInterval == 0)
        {
            gx_uart_map[gx_uart_interfacesCount].multiplier = 1;
        }
        else
        {
            gx_uart_map[gx_uart_interfacesCount].multiplier = (uint32_t)(UART_CONVERT_BPS_TO_MSEC(uartConfig->baudRate)) + 1 + uartConfig->chainInterval;
        }
        gx_uart_map[gx_uart_interfacesCount].counter = 0;
        gx_uart_map[gx_uart_interfacesCount].dataSentFlag = 1;

        gx_uart_interfacesCount++;
    }
    else
    {
        g8_uart_logChannelDataSent_flag = 1;
    }
#else
    gx_uart_map[gx_uart_interfacesCount].actualInterface = uartx;
    gx_uart_map[gx_uart_interfacesCount].virtualInterface = gx_uart_interfacesCount;

    if (uartConfig->chainInterval == 0)
    {
        gx_uart_map[gx_uart_interfacesCount].multiplier = 1;
    }
    else
    {
        gx_uart_map[gx_uart_interfacesCount].multiplier = (uint32_t)(UART_CONVERT_BPS_TO_MSEC(uartConfig->baudRate)) + 1 + uartConfig->chainInterval;
    }
    gx_uart_map[gx_uart_interfacesCount].counter = 0;
    gx_uart_map[gx_uart_interfacesCount].dataSentFlag = 1;

    gx_uart_interfacesCount++;
#endif

    if (uartx != UART_SW)
    {
        mcal_uart_channel_set(uartx, uartConfig);
    }
#if SSW_UART_ENABLED == 1
    else if (uartx == UART_SW)
    {
        ssw_uart_init(uartConfig);
    }
#endif
}

#if 0
void uart_deinit(void)
{
    gu32_uart_recvBufID0 = 0;
    gu32_uart_recvBufIDx = 0;
    gu32_uart_transBufID0 = 0;
    gu32_uart_transBufIDx = 0;

    gb_uart_initDone_flag = FLASE;
}
#endif

void uart_update(uint32_t period)
{
    static uint8_t nonLogChannelsCnt = 0;

#ifdef LOG_ENABLE
    if (gau32_uart_transLogBufIDx > gau32_uart_transLogBufID0)
    {
        uart_dataReg_put((uart_t)LOG_CHANNEL_ID, gaau8_uart_transLogBuffer[gau32_uart_transLogBufID0]);
        gau32_uart_transLogBufID0++;
        g8_uart_logChannelDataSent_flag = 0;
    }

    if (gau32_uart_transLogBufIDx == gau32_uart_transLogBufID0)
    {
        gau32_uart_transLogBufID0 = 0;
        gau32_uart_transLogBufIDx = 0;
        g8_uart_logChannelDataSent_flag = 1;
    }

    if (gau32_uart_recvLogBufID0 == gau32_uart_recvLogBufIDx)
    {
        gau32_uart_recvLogBufID0 = 0;
        gau32_uart_recvLogBufIDx = 0;
    }
#endif

    for (nonLogChannelsCnt = 0; nonLogChannelsCnt < gx_uart_interfacesCount; nonLogChannelsCnt++)
    {
        gx_uart_map[nonLogChannelsCnt].counter += period;
        if (gx_uart_map[nonLogChannelsCnt].counter >= gx_uart_map[nonLogChannelsCnt].multiplier)
        {
            gx_uart_map[nonLogChannelsCnt].counter = 0;
            if (gau32_uart_transBufIDx[nonLogChannelsCnt] > gau32_uart_transBufID0[nonLogChannelsCnt])
            {
                uart_dataReg_put(gx_uart_map[nonLogChannelsCnt].actualInterface, gaau8_uart_transBuffer[nonLogChannelsCnt][gau32_uart_transBufID0[nonLogChannelsCnt]]);
                gau32_uart_transBufID0[nonLogChannelsCnt]++;
                gx_uart_map[nonLogChannelsCnt].dataSentFlag = 0;
            }
            else
            {
                gau32_uart_transBufID0[nonLogChannelsCnt] = 0;
                gau32_uart_transBufIDx[nonLogChannelsCnt] = 0;
                gx_uart_map[nonLogChannelsCnt].dataSentFlag = 1;
            }

            if (gau32_uart_recvBufID0[nonLogChannelsCnt] == gau32_uart_recvBufIDx[nonLogChannelsCnt])
            {
                gau32_uart_recvBufID0[nonLogChannelsCnt] = 0;
                gau32_uart_recvBufIDx[nonLogChannelsCnt] = 0;
            }
        }
    }

    // nonLogChannelsCnt++;
    // if (nonLogChannelsCnt >= gx_uart_interfacesCount)
    // {
    //     nonLogChannelsCnt = 0;
    // }

#if SSW_UART_ENABLED == 1
    if (ssw_uart_recv_check() == 1)
    {
        uart_recv_int(UART_SW);
    }
#endif
}

int8_t uart_dataSent_check(uart_t uartx)
{
    uart_t vInterface;
    uint8_t u8_i;

#ifdef LOG_ENABLE
    if (uartx == LOG_CHANNEL_ID)
    {
        return g8_uart_logChannelDataSent_flag;
    }
    else
    {
        for (u8_i = 0; u8_i < gx_uart_interfacesCount; u8_i++)
        {
            if (gx_uart_map[u8_i].actualInterface == uartx)
            {
                vInterface = gx_uart_map[u8_i].virtualInterface;
                return gx_uart_map[vInterface].dataSentFlag;
            }
        }

        return -1;
    }
#else
    for (u8_i = 0; u8_i < gx_uart_interfacesCount; u8_i++)
    {
        if (gx_uart_map[u8_i].actualInterface == uartx)
        {
            vInterface = gx_uart_map[u8_i].virtualInterface;
            return gx_uart_map[vInterface].dataSentFlag;
        }
    }

    return -1;
#endif
}

void uart_byte_put(uart_t uartx, const uint8_t character)
{
    uart_t vInterface;
    uint8_t u8_i;

#ifdef LOG_ENABLE
    if (uartx == LOG_CHANNEL_ID)
    {
        if (gau32_uart_transLogBufIDx < UART_LOG_TRANS_BUFFER_SIZE)
        {
            gaau8_uart_transLogBuffer[gau32_uart_transLogBufIDx] = character;
            gau32_uart_transLogBufIDx++;
            g8_uart_logChannelDataSent_flag = 0;
        }
    }
    else
    {
        for (u8_i = 0; u8_i < gx_uart_interfacesCount; u8_i++)
        {
            if (gx_uart_map[u8_i].actualInterface == uartx)
            {
                vInterface = gx_uart_map[u8_i].virtualInterface;
                if (gau32_uart_transBufIDx[vInterface] < UART_TRANS_BUFFER_SIZE)
                {
                    gaau8_uart_transBuffer[vInterface][gau32_uart_transBufIDx[vInterface]] = character;
                    gau32_uart_transBufIDx[vInterface]++;
                    gx_uart_map[vInterface].dataSentFlag = 0;
                }
            }
        }
    }
#else
    for (u8_i = 0; u8_i < gx_uart_interfacesCount; u8_i++)
    {
        if (gx_uart_map[u8_i].actualInterface == uartx)
        {
            vInterface = gx_uart_map[u8_i].virtualInterface;
            if (gau32_uart_transBufIDx[vInterface] < UART_TRANS_BUFFER_SIZE)
            {
                gaau8_uart_transBuffer[vInterface][gau32_uart_transBufIDx[vInterface]] = character;
                gau32_uart_transBufIDx[vInterface]++;
                gx_uart_map[vInterface].dataSentFlag = 0;
            }
        }
    }
#endif
}

void uart_reset(uart_t uartx)
{
#ifdef LOG_ENABLE
    if (uartx == LOG_CHANNEL_ID)
    {
        gau32_uart_transLogBufID0 = 0;
        gau32_uart_transLogBufIDx = 0;
        g8_uart_logChannelDataSent_flag = 1;
    }
    else
    {
        gau32_uart_transBufID0[uartx] = 0;
        gau32_uart_transBufIDx[uartx] = 0;
        gx_uart_map[uartx].dataSentFlag = 1;
    }
#else
    gau32_uart_transBufID0[uartx] = 0;
    gau32_uart_transBufIDx[uartx] = 0;
    gx_uart_map[uartx].dataSentFlag = 1;
#endif
}

void uart_data_put(uart_t uartx, uint8_t *pData)
{
    uint8_t i = 0;
    for (i = 0; pData[i] != '\0'; i++)
    {
        uart_byte_put(uartx, pData[i]);
    }
}

static void uart_dataReg_put(uart_t uartx, const uint8_t character)
{
    if (uartx != UART_SW)
    {
        mcal_uart_data_put(uartx, character);
    }
#if SSW_UART_ENABLED == 1
    else if (uartx == UART_SW)
    {
        ssw_uart_send(character);
    }
#endif
}

bool_t uart_recv_check(uart_t uartx)
{
    uart_t vInterface;
    uint8_t u8_i;

#ifdef LOG_ENABLE
    if (uartx == LOG_CHANNEL_ID)
    {

        if (gau32_uart_recvLogBufID0 < gau32_uart_recvLogBufIDx)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        for (u8_i = 0; u8_i < gx_uart_interfacesCount; u8_i++)
        {
            if (gx_uart_map[u8_i].actualInterface == uartx)
            {
                vInterface = gx_uart_map[u8_i].virtualInterface;
                if (gau32_uart_recvBufID0[vInterface] < gau32_uart_recvBufIDx[vInterface])
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
        }

        return FALSE;
    }
#else
    for (u8_i = 0; u8_i < gx_uart_interfacesCount; u8_i++)
    {
        if (gx_uart_map[u8_i].actualInterface == uartx)
        {
            vInterface = gx_uart_map[u8_i].virtualInterface;
            if (gau32_uart_recvBufID0[vInterface] < gau32_uart_recvBufIDx[vInterface])
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }

    return FALSE;
#endif
}

#if UART_RX_INT == 1
static void uart_dataReg_get(uart_t uartx, uart_t vInterface)
{
#ifdef LOG_ENABLE
    if (uartx == LOG_CHANNEL_ID)
    {
        if (gau32_uart_recvLogBufIDx < UART_LOG_RECV_BUFFER_SIZE)
        {
            gaau8_uart_recvLogBuffer[gau32_uart_recvLogBufIDx] = mcal_uart_data_get(uartx);
            gau32_uart_recvLogBufIDx++;
        }
    }
    else
    {
        if (gau32_uart_recvBufIDx[vInterface] < UART_RECV_BUFFER_SIZE)
        {
            if (uartx != UART_SW)
            {
                gaau8_uart_recvBuffer[vInterface][gau32_uart_recvBufIDx[vInterface]] = mcal_uart_data_get(uartx);
            }
#if SSW_UART_ENABLED == 1
            else if (uartx == UART_SW)
            {
                gaau8_uart_recvBuffer[vInterface][gau32_uart_recvBufIDx[vInterface]] = ssw_uart_get();
            }
#endif
            gau32_uart_recvBufIDx[vInterface]++;
        }
    }
#else
    if (gau32_uart_recvBufIDx[vInterface] < UART_RECV_BUFFER_SIZE)
    {
        if (uartx != UART_SW)
        {
            gaau8_uart_recvBuffer[vInterface][gau32_uart_recvBufIDx[vInterface]] = mcal_uart_data_get(uartx);
        }
#if SSW_UART_ENABLED == 1
        else if (uartx == UART_SW)
        {
            gaau8_uart_recvBuffer[vInterface][gau32_uart_recvBufIDx[vInterface]] = ssw_uart_get();
        }
#endif
        gau32_uart_recvBufIDx[vInterface]++;
    }
#endif
}

uint8_t uart_byte_get(uart_t uartx)
{
    uint8_t retValue = 0;
    uart_t vInterface;
    uint8_t u8_i;

#ifdef LOG_ENABLE
    if (uartx == LOG_CHANNEL_ID)
    {
        if (gau32_uart_recvLogBufID0 < gau32_uart_recvLogBufIDx)
        {
            retValue = gaau8_uart_recvLogBuffer[gau32_uart_recvLogBufID0];
            gau32_uart_recvLogBufID0++;
        }
        else
        {
            return 0;
        }

        return retValue;
    }
    else
    {
        for (u8_i = 0; u8_i < gx_uart_interfacesCount; u8_i++)
        {
            if (gx_uart_map[u8_i].actualInterface == uartx)
            {
                vInterface = gx_uart_map[u8_i].virtualInterface;
                if (gau32_uart_recvBufID0[vInterface] < gau32_uart_recvBufIDx[vInterface])
                {
                    retValue = gaau8_uart_recvBuffer[vInterface][gau32_uart_recvBufID0[vInterface]];
                    gau32_uart_recvBufID0[vInterface]++;
                }
                else
                {
                    return 0;
                }
                return retValue;
            }
        }

        return 0;
    }
#else
    for (u8_i = 0; u8_i < gx_uart_interfacesCount; u8_i++)
    {
        if (gx_uart_map[u8_i].actualInterface == uartx)
        {
            vInterface = gx_uart_map[u8_i].virtualInterface;
            if (gau32_uart_recvBufID0[vInterface] < gau32_uart_recvBufIDx[vInterface])
            {
                retValue = gaau8_uart_recvBuffer[vInterface][gau32_uart_recvBufID0[vInterface]];
                gau32_uart_recvBufID0[vInterface]++;
            }
            else
            {
                return 0;
            }
            return retValue;
        }
    }

    return 0;
#endif
}

// :NOTE: gets the whole data in the receiver buffer and returns the number of read data, otherwise return -1
int16_t uart_bytesAvailable_get(uart_t uartx, uint8_t *pData)
{
    uart_t vInterface;
    uint16_t j = 0;
    uint8_t u8_i;

#ifdef LOG_ENABLE
    if (uartx == LOG_CHANNEL_ID)
    {
        while (gau32_uart_recvLogBufID0 < gau32_uart_recvLogBufIDx)
        {
            pData[j] = gaau8_uart_recvLogBuffer[gau32_uart_recvLogBufID0];
            gau32_uart_recvLogBufID0++;
            j++;
        }
        return j;
    }
    else
    {
        for (u8_i = 0; u8_i < gx_uart_interfacesCount; u8_i++)
        {
            if (gx_uart_map[u8_i].actualInterface == uartx)
            {
                vInterface = gx_uart_map[u8_i].virtualInterface;
                while (gau32_uart_recvBufID0[vInterface] < gau32_uart_recvBufIDx[vInterface])
                {
                    pData[j] = gaau8_uart_recvBuffer[vInterface][gau32_uart_recvBufID0[vInterface]];
                    gau32_uart_recvBufID0[vInterface]++;
                    j++;
                }
                return j;
            }
        }

        return -1;
    }
#else
    for (u8_i = 0; u8_i < gx_uart_interfacesCount; u8_i++)
    {
        if (gx_uart_map[u8_i].actualInterface == uartx)
        {
            vInterface = gx_uart_map[u8_i].virtualInterface;
            while (gau32_uart_recvBufID0[vInterface] < gau32_uart_recvBufIDx[vInterface])
            {
                pData[j] = gaau8_uart_recvBuffer[vInterface][gau32_uart_recvBufID0[vInterface]];
                gau32_uart_recvBufID0[vInterface]++;
                j++;
            }
            return j;
        }
    }

    return -1;
#endif
}

void uart_string_get(uart_t uartx, uint8_t *pString)
{
    uint8_t i = 0, character = 0;
    for (i = 0; (character = uart_byte_get(uartx)) != 0; i++)
    {
        pString[i] = character;
    }
    if (pString[i] == '\n' || pString[i] == '\r')
        pString[i] = '\0';
}

void uart_data_get(uart_t uartx, uint8_t *pData, uint8_t size)
{
    uint8_t i = 0, character = 0;
    for (i = 0; (character = uart_byte_get(uartx)) != 0 && i < size; i++)
    {
        pData[i] = character;
    }
}

void uart_recv_int(uart_t uartx)
{
    uart_t vInterface;
    uint8_t u8_i;
#ifdef LOG_ENABLE
    if (uartx == LOG_CHANNEL_ID)
    {
        uart_dataReg_get(uartx, (uart_t)0xff);
        if (gau32_uart_recvLogBufID0 == gau32_uart_recvLogBufIDx)
        {
            gau32_uart_recvLogBufID0 = 0;
            gau32_uart_recvLogBufIDx = 0;
        }
    }
    else
    {
        for (u8_i = 0; u8_i < gx_uart_interfacesCount; u8_i++)
        {
            if (gx_uart_map[u8_i].actualInterface == uartx)
            {
                vInterface = gx_uart_map[u8_i].virtualInterface;
                uart_dataReg_get(uartx, vInterface);
                if (gau32_uart_recvBufID0[vInterface] == gau32_uart_recvBufIDx[vInterface])
                {
                    gau32_uart_recvBufID0[vInterface] = 0;
                    gau32_uart_recvBufIDx[vInterface] = 0;
                }
            }
        }
    }
#else
    for (u8_i = 0; u8_i < gx_uart_interfacesCount; u8_i++)
    {
        if (gx_uart_map[u8_i].actualInterface == uartx)
        {
            vInterface = gx_uart_map[u8_i].virtualInterface;
            uart_dataReg_get(uartx, vInterface);
            if (gau32_uart_recvBufID0[vInterface] == gau32_uart_recvBufIDx[vInterface])
            {
                gau32_uart_recvBufID0[vInterface] = 0;
                gau32_uart_recvBufIDx[vInterface] = 0;
            }
        }
    }
#endif
}
#endif

void uart_multiplier_get(uint8_t *pData)
{
    uint8_t i;
    for (i = 0; i < UART_INTERFACES_NO_LOG_NUM; i++)
    {
        pData[i] = gx_uart_map[i].multiplier;
    }
}
