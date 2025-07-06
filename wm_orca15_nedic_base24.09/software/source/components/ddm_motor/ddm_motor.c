#include "component_config.h"
#include "components/ddm_motor/ddm_motor.h"
#include "def.h"
#include "components/uart/uart.h"
#include "os.h"

#include "utils.h"

#if DDM_MODEL == DDM_INTERNAL
#include "communication.h"
#endif

#define CRC_INITIAL          0xFFFF
#define POLYNOMIAL           0x1021
#define WIDTH                (8 * sizeof(uint16_t))
#define TOP_BIT              (1 << (WIDTH - 1))
#define CRC_TABLE_GENERATION FALSE
#define CRC_FAST             FALSE

static void DDM_highLevel_handle(uint32_t period);
static void DDM_lowLevel_handle(uint32_t period);
static void DDM_data_process(void);

static void DDM_command_generate(DDM_command_t command, DDM_motionPattern_t *controlBlock, uint8_t *buffer);
static uint16_t DDM_crc_get(uint8_t *arr, uint8_t size);
static uint8_t DDM_dataMatch_check(uint8_t *data, uint8_t *ref, uint16_t size);

static uint8_t gau8_DDM_internalTXBuffer[DDM_TX_BUFFER_SIZE] = {0};
static uint8_t gau8_DDM_internalOldTXBuffer[DDM_TX_BUFFER_SIZE] = {0};
static uint8_t gau8_DDM_internalRXBuffer[DDM_RX_BUFFER_SIZE] = {0};
static uint8_t gau8_DDM_feedbackBuffer[DDM_RX_DATA_SIZE] = {0};
static uint8_t gau8_DDM_internalIdleBuffer[DDM_TX_BUFFER_SIZE] = {0};

static DDM_t gx_DDM_internalHandler;
static DDM_motionPattern_t gx_DDM_internalMotorControlBlock;

static uint8_t forcedBrake_flag = 0;

static DDM_state_t gx_DDM_state;

static DDM_command_t gx_DDM_command = DDM_COMMAND_REQUEST;

static DDM_error_t gx_DDM_internalErrorHandler;
static uint16_t gu16_DDM_swVersion = 0;

static uint16_t rawWeightFeedback = 0;
static uint16_t gramWeightFeedback = 0;
static uint8_t clutchDone_flag = 0;
static uint32_t motorSpeed = 0;

void DDM_init(DDM_t *motorConfig)
{
    gx_DDM_state = DDM_SYNC_STATE;

    gx_DDM_internalHandler.uartx = motorConfig->uartx;
    gx_DDM_internalHandler.txIntervalMS = motorConfig->txIntervalMS;
    gx_DDM_internalHandler.receiveTimeoutMS = motorConfig->receiveTimeoutMS;
    gx_DDM_internalHandler.platform = motorConfig->platform;
    gx_DDM_internalHandler.rawTubValue = motorConfig->rawTubValue;

    DDM_command_generate(DDM_COMMAND_REQUEST, NULL, gau8_DDM_internalIdleBuffer);

    gx_DDM_internalErrorHandler = DDM_NO_ERROR;
}

void DDM_update(uint32_t period)
{
    DDM_highLevel_handle(period);
    DDM_lowLevel_handle(period);
}

void DDM_command_set(DDM_command_t command, const DDM_motionPattern_t *pattern)
{
    gx_DDM_command = command;

    if (pattern != NULL)
    {
        gx_DDM_internalMotorControlBlock.accTimeMSec = pattern->accTimeMSec;
        gx_DDM_internalMotorControlBlock.ccwOffTimeMSec = pattern->ccwOffTimeMSec;
        gx_DDM_internalMotorControlBlock.ccwOnTimeMSec = pattern->ccwOnTimeMSec;
        gx_DDM_internalMotorControlBlock.cwOffTimeMSec = pattern->cwOffTimeMSec;
        gx_DDM_internalMotorControlBlock.cwOnTimeMSec = pattern->cwOnTimeMSec;
        gx_DDM_internalMotorControlBlock.decTimeMSec = pattern->decTimeMSec;

#ifdef MOTOR_SPEED_LIMIT_ENABLE
        if (gx_DDM_internalMotorControlBlock.rpm > _DDM_MOTOR_SPEED_LIMIT)
        {
            gx_DDM_internalMotorControlBlock.rpm = _DDM_MOTOR_SPEED_LIMIT;
        }
        else
        {
            gx_DDM_internalMotorControlBlock.rpm = pattern->rpm;
        }
#else
        gx_DDM_internalMotorControlBlock.rpm = pattern->rpm;
#endif
        if (DDM_COMMAND_AGITATION == command)
        {
            gx_DDM_internalMotorControlBlock.rpm *= 5.33;
        }
    }
}

DDM_state_t DDM_state_get(void)
{
    return gx_DDM_state;
}
void DDM_state_set(DDM_state_t value)
{
    gx_DDM_state = value;
}

uint16_t DDM_weightGramFeedback_get(void)
{
    return gramWeightFeedback;
}
void DDM_weightGramFeedback_set(uint16_t value)
{
    gramWeightFeedback = value;
}
uint16_t DDM_weightRawFeedback_get(void)
{
    return rawWeightFeedback;
}

uint16_t DDM_swVersion_get(void)
{
    return gu16_DDM_swVersion;
}

void DDM_clutchDoneFlag_set(uint8_t value)
{
    clutchDone_flag = value;
}
uint8_t DDM_clutchDoneFlag_get(void)
{
    return clutchDone_flag;
}

DDM_error_t DDM_error_get(void)
{
    return gx_DDM_internalErrorHandler;
}
void DDM_error_set(DDM_error_t value)
{
    gx_DDM_internalErrorHandler = value;
}

uint32_t DDM_speed_get(void)
{
    return motorSpeed;
}
void DDM_speed_set(uint32_t value)
{
    motorSpeed = value;
}

static void DDM_highLevel_handle(uint32_t period)
{
    static uint32_t timeoutMSec = 0;
    static uint32_t faultTimeoutMSec = 0;

    if (DDM_NO_ERROR != gx_DDM_internalErrorHandler)
    {
        faultTimeoutMSec += period;
        if (faultTimeoutMSec >= 10000U)
        {
            faultTimeoutMSec = 0;
            gx_DDM_state = DDM_FAULT_STATE;
        }
    }
    else
    {
        faultTimeoutMSec = 0;
        if (gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_REQUEST)
        {
            motorSpeed = (gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MOTOR_SPEED_HIGH_BYTE] << 8) | gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MOTOR_SPEED_LOW_BYTE];
            gx_DDM_internalErrorHandler = (DDM_error_t)gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_ERROR];
        }
    }

    switch (gx_DDM_state)
    {
        case DDM_SYNC_STATE:
        {
            DDM_command_generate(DDM_COMMAND_PING, NULL, gau8_DDM_internalTXBuffer);

            if (gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_PING)
            {
                gx_DDM_state = DDM_BRAKE_STATE;
                gu16_DDM_swVersion = (uint16_t)((uint16_t)gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_SW_VERSION_HIGH_BYTE] << 8) | (gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_SW_VERSION_LOW_BYTE]);
            }
        }
        break;

        case DDM_BRAKE_STATE:
        {
            DDM_command_generate(DDM_COMMAND_BRAKE, NULL, gau8_DDM_internalTXBuffer);

            if (gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_BRAKE)
            {
                if ((gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_ERROR] == 0x00) && (gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MOTOR_SPEED_HIGH_BYTE] == 0 && gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MOTOR_SPEED_LOW_BYTE] == 0))
                {
                    gx_DDM_state = DDM_FORCE_BRAKE_STARTUP;
                }
                else
                {
                    gx_DDM_internalErrorHandler = DDM_ERROR_GENERAL_FAULT;
                }
            }
        }
        break;

        case DDM_FORCE_BRAKE_STARTUP:
        {
            forcedBrake_flag = 1;
            DDM_command_generate(DDM_COMMAND_FORCED_BRAKE, NULL, gau8_DDM_internalTXBuffer);

            if (gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_FORCED_BRAKE)
            {
                if ((gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_ERROR] == 0x00) && (gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MOTOR_SPEED_HIGH_BYTE] == 0 && gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MOTOR_SPEED_LOW_BYTE] == 0))
                {
                    timeoutMSec += period;
                    if (timeoutMSec >= 1000)
                    {
                        timeoutMSec = 0;
                        gx_DDM_state = DDM_FORCE_BRAKE_CANCEL;
                    }
                }
                else
                {
                    gx_DDM_internalErrorHandler = DDM_ERROR_GENERAL_FAULT;
                }
            }
        }
        break;

        case DDM_FORCE_BRAKE_CANCEL:
        {
            forcedBrake_flag = 0;
            DDM_command_generate(DDM_COMMAND_FORCED_BRAKE, NULL, gau8_DDM_internalTXBuffer);

            timeoutMSec += period;
            if (timeoutMSec >= 1000)
            {
                timeoutMSec = 0;
                gx_DDM_state = DDM_FORCE_BRAKE_STARTUP_DONE;
            }
        }
        break;

        case DDM_FORCE_BRAKE_STARTUP_DONE:
        {
            DDM_command_generate(DDM_COMMAND_BRAKE, NULL, gau8_DDM_internalTXBuffer);

            if ((gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_BRAKE) || (gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_REQUEST))
            {
                timeoutMSec += period;
                if (timeoutMSec >= 1000)
                {
                    timeoutMSec = 0;
                    gx_DDM_state = DDM_OPERATION_STATE;
                }
            }
        }
        break;

        case DDM_OPERATION_STATE:
        {
            DDM_command_generate(gx_DDM_command, &gx_DDM_internalMotorControlBlock, gau8_DDM_internalTXBuffer);

            if (gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_CW || gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_CCW || gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_AGITATION || gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_SPIN || gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_SPIN_PULSATOR || gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_STOP || gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_BRAKE || gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_FORCED_BRAKE)
            {
                motorSpeed = (gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MOTOR_SPEED_HIGH_BYTE] << 8) | gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MOTOR_SPEED_LOW_BYTE];
                gx_DDM_internalErrorHandler = (DDM_error_t)gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_ERROR];
                clutchDone_flag = 0;
            }
            else if (gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_WEIGHT_DETECTION)
            {
                gx_DDM_internalErrorHandler = (DDM_error_t)gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_ERROR];
                if (gx_DDM_internalErrorHandler == DDM_NO_ERROR)
                {
                    rawWeightFeedback = ((uint32_t)gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_WEIGHT_HIGH_BYTE] << 8) | gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_WEIGHT_LOW_BYTE];
                    gramWeightFeedback = gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_WEIGHT] * 100;
                }
                else
                {
                    gx_DDM_internalErrorHandler = DDM_NO_ERROR;
                    rawWeightFeedback = UINT16_MAX;
                    gramWeightFeedback = UINT16_MAX;
                }
                motorSpeed = 0;
            }
            else if (gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_CLUTCH || gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_MODE] == DDM_COMMAND_DECLUTCH)
            {
                clutchDone_flag = gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_CLUTCH_DONE_FLAG];
                motorSpeed = 0;
                gx_DDM_internalErrorHandler = (DDM_error_t)gau8_DDM_feedbackBuffer[DDM_DATA_INDEX_ERROR];
            }
            else
            {
                // do nothing
            }
        }
        break;

        case DDM_FAULT_STATE:
        {
            motorSpeed = UINT32_MAX;   // to indicate fault
            clutchDone_flag = 0;
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}

static void DDM_lowLevel_handle(uint32_t period)
{
#if DDM_MODEL == DDM_INTERNAL
    static uint8_t *gau8_DDM_RX;
#elif DDM_MODEL == DDM_EXTERNAL
    int16_t ret = 0;
    static uint8_t rxCounter = 0;
#endif
    static uint32_t internalTXIntervalCounter = 0, internalRXTimeout = 0;

    internalTXIntervalCounter += period;
    if ((internalTXIntervalCounter >= gx_DDM_internalHandler.txIntervalMS))
    {
        internalTXIntervalCounter = 0;
#if DDM_MODEL == DDM_EXTERNAL
        rxCounter = 0;
#endif

        if (gau8_DDM_internalTXBuffer[DDM_DATA_INDEX_MODE] != DDM_COMMAND_CLUTCH && gau8_DDM_internalTXBuffer[DDM_DATA_INDEX_MODE] != DDM_COMMAND_DECLUTCH && gau8_DDM_internalTXBuffer[DDM_DATA_INDEX_MODE] != DDM_COMMAND_WEIGHT_DETECTION && gau8_DDM_internalTXBuffer[DDM_DATA_INDEX_MODE] != DDM_COMMAND_FORCED_BRAKE && DDM_dataMatch_check(gau8_DDM_internalOldTXBuffer, gau8_DDM_internalTXBuffer, DDM_TX_BUFFER_SIZE))
        {
            for (uint8_t i = 0; i < DDM_TX_BUFFER_SIZE; i++)
            {
#if DDM_MODEL == DDM_EXTERNAL
                uart_byte_put(gx_DDM_internalHandler.uartx, gau8_DDM_internalIdleBuffer[i]);
#elif DDM_MODEL == DDM_INTERNAL
                mc_set_integ_TX_buffer(gau8_DDM_internalIdleBuffer[i]);
#endif
            }
        }
        else
        {
            for (uint8_t i = 0; i < DDM_TX_BUFFER_SIZE; i++)
            {
                gau8_DDM_internalOldTXBuffer[i] = gau8_DDM_internalTXBuffer[i];
#if DDM_MODEL == DDM_EXTERNAL
                uart_byte_put(gx_DDM_internalHandler.uartx, gau8_DDM_internalTXBuffer[i]);
#elif DDM_MODEL == DDM_INTERNAL
                mc_set_integ_TX_buffer(gau8_DDM_internalTXBuffer[i]);
#endif
            }
        }
    }

#if DDM_MODEL == DDM_EXTERNAL
    if (uart_recv_check(gx_DDM_internalHandler.uartx) == TRUE)
    {
        internalRXTimeout = 0;
        if ((ret = uart_bytesAvailable_get(gx_DDM_internalHandler.uartx, &gau8_DDM_internalRXBuffer[rxCounter])) != -1)
        {
            rxCounter += ret;
            // printf("count %d\n\r", rxCounter);
            // for (uint8_t i = 0; i < rxCounter; i++)
            // {
            //     printf("%02x ", gau8_DDM_internalRXBuffer[i]);
            // }
            // printf("\n\r");
        }

        if (rxCounter >= DDM_RX_BUFFER_SIZE)
        {
            internalRXTimeout = 0;
            rxCounter = 0;

            LOG('n', 'v', "%2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x - %2x %2x %2x %2x %2x %2x %2x %2x - %d %d", gau8_DDM_internalRXBuffer[0], gau8_DDM_internalRXBuffer[1], gau8_DDM_internalRXBuffer[2], gau8_DDM_internalRXBuffer[3], gau8_DDM_internalRXBuffer[4], gau8_DDM_internalRXBuffer[5], gau8_DDM_internalRXBuffer[6], gau8_DDM_internalRXBuffer[7], gau8_DDM_internalRXBuffer[8], gau8_DDM_internalRXBuffer[9], gau8_DDM_internalRXBuffer[10], gau8_DDM_internalRXBuffer[11], gau8_DDM_internalRXBuffer[12], gau8_DDM_internalRXBuffer[13], gau8_DDM_internalRXBuffer[14], gau8_DDM_internalRXBuffer[15], gau8_DDM_internalRXBuffer[16], gau8_DDM_internalRXBuffer[17], gau8_DDM_internalRXBuffer[18], gau8_DDM_internalRXBuffer[19], gx_DDM_state, gx_DDM_internalErrorHandler);

            DDM_data_process();
        }
#elif DDM_MODEL == DDM_INTERNAL
    if (mc_get_integ_RX_buffer_flag())
    {
        internalRXTimeout = 0;

        gau8_DDM_RX = mc_get_integ_RX_buffer(NULL);

        for (uint8_t i = 0; i < DDM_TX_BUFFER_SIZE; i++)
        {
            gau8_DDM_internalRXBuffer[i] = 0xcc;   // add correct data
        }

        for (uint8_t i = DDM_TX_BUFFER_SIZE; i < DDM_RX_BUFFER_SIZE; i++)
        {
            gau8_DDM_internalRXBuffer[i] = gau8_DDM_RX[i - DDM_TX_BUFFER_SIZE];
        }

#ifdef DDM_CRC_XOR
        LOG('n', 'v', "RX inside : %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x          %2x %2x %2x %2x %2x %2x %2x  State:%d   Err:%d", gau8_DDM_internalRXBuffer[0], gau8_DDM_internalRXBuffer[1], gau8_DDM_internalRXBuffer[2], gau8_DDM_internalRXBuffer[3], gau8_DDM_internalRXBuffer[4], gau8_DDM_internalRXBuffer[5], gau8_DDM_internalRXBuffer[6], gau8_DDM_internalRXBuffer[7], gau8_DDM_internalRXBuffer[8], gau8_DDM_internalRXBuffer[9], gau8_DDM_internalRXBuffer[10], gau8_DDM_internalRXBuffer[11], gau8_DDM_internalRXBuffer[12], gau8_DDM_internalRXBuffer[13], gau8_DDM_internalRXBuffer[14], gau8_DDM_internalRXBuffer[15], gau8_DDM_internalRXBuffer[16], gau8_DDM_internalRXBuffer[17], gx_DDM_state, gx_DDM_internalErrorHandler);
#elif defined(DDM_CRC_CCITT)
        LOG('n', 'v', "RX inside : %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x          %2x %2x %2x %2x %2x %2x %2x %2x  State:%d   Err:%d", gau8_DDM_internalRXBuffer[0], gau8_DDM_internalRXBuffer[1], gau8_DDM_internalRXBuffer[2], gau8_DDM_internalRXBuffer[3], gau8_DDM_internalRXBuffer[4], gau8_DDM_internalRXBuffer[5], gau8_DDM_internalRXBuffer[6], gau8_DDM_internalRXBuffer[7], gau8_DDM_internalRXBuffer[8], gau8_DDM_internalRXBuffer[9], gau8_DDM_internalRXBuffer[10], gau8_DDM_internalRXBuffer[11], gau8_DDM_internalRXBuffer[12], gau8_DDM_internalRXBuffer[13], gau8_DDM_internalRXBuffer[14], gau8_DDM_internalRXBuffer[15], gau8_DDM_internalRXBuffer[16], gau8_DDM_internalRXBuffer[17], gau8_DDM_internalRXBuffer[18], gau8_DDM_internalRXBuffer[19], gx_DDM_state, gx_DDM_internalErrorHandler);
#endif
        DDM_data_process();
#endif
    }
    else
    {
        internalRXTimeout += period;
        if (internalRXTimeout >= gx_DDM_internalHandler.receiveTimeoutMS)
        {
            internalRXTimeout = 0;
            gx_DDM_internalErrorHandler = DDM_ERROR_NO_COM;
#if DDM_MODEL == DDM_EXTERNAL
            rxCounter = 0;
#endif
            for (uint8_t i = 0; i < DDM_RX_BUFFER_SIZE; i++)
            {
                gau8_DDM_internalRXBuffer[i] = 0;
            }
        }
    }
}

static void DDM_data_process(void)
{
    if (gau8_DDM_internalRXBuffer[DDM_DATA_INDEX_HEADER + DDM_TX_BUFFER_SIZE] == DDM_FEEDBACK_ACK)
    {
        if (gau8_DDM_internalRXBuffer[DDM_DATA_INDEX_MODE + DDM_TX_BUFFER_SIZE] == gau8_DDM_internalTXBuffer[DDM_DATA_INDEX_MODE] || gau8_DDM_internalRXBuffer[DDM_DATA_INDEX_MODE + DDM_TX_BUFFER_SIZE] == DDM_COMMAND_REQUEST)
        {
#ifdef DDM_CRC_XOR
            if (DDM_crc_get(&gau8_DDM_internalRXBuffer[DDM_TX_BUFFER_SIZE], DDM_RX_DATA_SIZE - 1) != gau8_DDM_internalRXBuffer[DDM_DATA_INDEX_CRC + DDM_TX_BUFFER_SIZE])
#elif defined(DDM_CRC_CCITT)
            if (DDM_crc_get(&gau8_DDM_internalRXBuffer[DDM_TX_BUFFER_SIZE], DDM_RX_DATA_SIZE - 2) != (((uint16_t)((uint16_t)gau8_DDM_internalRXBuffer[DDM_DATA_INDEX_CRC + DDM_TX_BUFFER_SIZE]) << 8) | ((uint16_t)gau8_DDM_internalRXBuffer[DDM_DATA_INDEX_CRC + DDM_TX_BUFFER_SIZE + 1])))
#endif
            {
                gx_DDM_internalErrorHandler = DDM_ERROR_CRC;
            }
            else
            {
                for (uint8_t j = 0; j < DDM_RX_DATA_SIZE; j++)
                {
                    gau8_DDM_feedbackBuffer[j] = gau8_DDM_internalRXBuffer[j + DDM_TX_BUFFER_SIZE];
                }
            }
        }
        else
        {
            gx_DDM_internalErrorHandler = DDM_ERROR_MODE;
        }
    }
    else if (gau8_DDM_internalRXBuffer[DDM_DATA_INDEX_HEADER + DDM_TX_BUFFER_SIZE] == DDM_FEEDBACK_NACK)
    {
        gx_DDM_internalErrorHandler = DDM_ERROR_NACK;
    }
    else
    {
        gx_DDM_internalErrorHandler = DDM_ERROR_NO_COM;
    }
}

static void DDM_command_generate(DDM_command_t command, DDM_motionPattern_t *controlBlock, uint8_t *buffer)
{
    uint8_t j = 0;
    uint8_t usedBytes = 0;

    buffer[j++] = DDM_HEADER;

    switch (command)
    {
        case DDM_COMMAND_PING:
        {
            if (gx_DDM_internalHandler.rawTubValue >= 0x0fff || gx_DDM_internalHandler.rawTubValue < 1800)
            {
                gx_DDM_internalHandler.rawTubValue = 0;
            }
            buffer[j++] = (uint8_t)command;
            buffer[j++] = gx_DDM_internalHandler.platform;
            buffer[j++] = (uint8_t)(gx_DDM_internalHandler.rawTubValue >> 8);
            buffer[j++] = (uint8_t)(gx_DDM_internalHandler.rawTubValue & 0x00ff);
            usedBytes = j;
        }
        break;

        // case DDM_COMMAND_SPIN:
        // case DDM_COMMAND_SPIN_PULSATOR:
        case DDM_COMMAND_CW:
        case DDM_COMMAND_CCW:
        {
            if (NULL != controlBlock)
            {
                if (controlBlock->rpm > DDM_MOTOR_MAX_RPM)
                {
                    controlBlock->rpm = DDM_MOTOR_MAX_RPM;
                }

                buffer[j++] = (uint8_t)command;
                buffer[j++] = (uint8_t)(controlBlock->rpm >> 8) & 0x00ff;
                buffer[j++] = (uint8_t)(controlBlock->rpm & 0x00ff);
                buffer[j++] = (uint8_t)((controlBlock->accTimeMSec / 100) >> 8) & 0x00ff;
                buffer[j++] = (uint8_t)(controlBlock->accTimeMSec / 100) & 0x00ff;
                usedBytes = j;
            }
        }
        break;

        case DDM_COMMAND_AGITATION:
        {
            if (NULL != controlBlock)
            {
                if (controlBlock->rpm > DDM_MOTOR_MAX_RPM)
                {
                    controlBlock->rpm = DDM_MOTOR_MAX_RPM;
                }

                buffer[j++] = (uint8_t)command;
                buffer[j++] = (uint8_t)(controlBlock->rpm >> 8) & 0x00ff;
                buffer[j++] = (uint8_t)(controlBlock->rpm & 0x00ff);
                buffer[j++] = (uint8_t)(controlBlock->accTimeMSec / 100);
                buffer[j++] = (uint8_t)(controlBlock->decTimeMSec / 100);
                buffer[j++] = (uint8_t)(controlBlock->cwOnTimeMSec / 100);
                buffer[j++] = (uint8_t)(controlBlock->cwOffTimeMSec / 100);
                buffer[j++] = (uint8_t)(controlBlock->ccwOnTimeMSec / 100);
                buffer[j++] = (uint8_t)(controlBlock->ccwOffTimeMSec / 100);
                usedBytes = j;
            }
        }
        break;

        case DDM_COMMAND_REQUEST:
        case DDM_COMMAND_STOP:
        case DDM_COMMAND_BRAKE:
        case DDM_COMMAND_VOLTAGE_DETECTION:
        case DDM_COMMAND_WEIGHT_DETECTION:
        case DDM_COMMAND_FABRIC_DETECTION:
        {
            buffer[j++] = (uint8_t)command;
            usedBytes = j;
        }
        break;

        case DDM_COMMAND_CLUTCH:
        {
            buffer[j++] = (uint8_t)command;
            buffer[j++] = 1;
            usedBytes = j;
        }
        break;

        case DDM_COMMAND_DECLUTCH:
        {
            buffer[j++] = 0x19;
            buffer[j++] = 0;
            usedBytes = j;
        }
        break;

        case DDM_COMMAND_FORCED_BRAKE:
        {
            buffer[j++] = (uint8_t)command;
            buffer[j++] = (uint8_t)(forcedBrake_flag);   // set to 1 to enable, 0 to disable
            usedBytes = j;
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }

    // fill the rest of the command with zeros (must)
    for (uint8_t i = usedBytes; i < DDM_TX_BUFFER_SIZE - 1; i++)
    {
        buffer[i] = 0x00;
    }
#ifdef DDM_CRC_XOR
    buffer[DDM_TX_BUFFER_SIZE - 1] = DDM_crc_get(buffer, DDM_TX_BUFFER_SIZE - 1);
#elif defined(DDM_CRC_CCITT)
    uint16_t crc = DDM_crc_get(buffer, DDM_TX_BUFFER_SIZE - 2);
    buffer[DDM_TX_BUFFER_SIZE - 2] = (uint8_t)(((uint16_t)crc & 0xff00) >> 8U);
    buffer[DDM_TX_BUFFER_SIZE - 1] = (uint8_t)((uint16_t)crc & 0x00ff);
#endif
}

static uint16_t DDM_crc_get(uint8_t *arr, uint8_t size)
{
#ifdef DDM_CRC_XOR
    uint8_t ret, i = 0;

    ret = 0;

    for (i = 0; i < size; i++)
    {
        ret ^= arr[i];
    }

    return ret;

#elif defined(DDM_CRC_CCITT)
    uint16_t bytes;
    uint16_t remainder;
    uint8_t bit;

    remainder = CRC_INITIAL;

    for (bytes = 0; bytes < size; bytes++)
    {
        /* Bring next byte into the remainder */
        remainder ^= (arr[bytes] << (WIDTH - 8));

        /* Perform mod-2 division bit by bit */
        for (bit = 8; bit > 0; bit--)
        {
            /* Try to divide the current data bit */
            if (remainder & TOP_BIT)
            {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
    }

    return remainder; /* The final remainder is the CRC */
#endif
}

static uint8_t DDM_dataMatch_check(uint8_t *data, uint8_t *ref, uint16_t size)
{
    uint16_t i = 0;

    while (size > i)
    {
        if (data[i] != ref[i])
        {
            return 0;
        }
        i++;
    }
    return 1;
}
