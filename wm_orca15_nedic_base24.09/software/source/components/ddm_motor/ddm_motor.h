#ifndef DDM_MOTOR_H
#define DDM_MOTOR_H

#include "def.h"
#include "components/uart/uart.h"

typedef enum DDM_error
{
    DDM_NO_ERROR = 0x00,
    DDM_ERROR_GENERAL_FAULT = 0x01,
    DDM_ERROR_IPM_OV_TEMP = 0x02,
    DDM_ERROR_BUS_VOLTAGE = 0x04,
    DDM_ERROR_MOTOR_STALL_TACHO_MISSING = 0x08,
    DDM_ERROR_HW_OV_CURRENT = 0x10,
    DDM_ERROR_BUS_UNDER_VOLTAGE = 0x20,
    DDM_ERROR_LOST_PHASE = 0x40,
    DDM_ERROR_MOTOR_OV_TEMP = 0x80,

    DDM_ERROR_NO_COM = 0x81,
    DDM_ERROR_NACK = 0x82,
    DDM_ERROR_MODE = 0x83,
    DDM_ERROR_FAULT = 0x84,
    DDM_ERROR_CRC = 0x85
} DDM_error_t;

#define DDM_IDLE_TIMEOUT_PERIOD_MS (500U)

#define DDM_HEADER        (0xa5U)
#define DDM_FEEDBACK_ACK  (0xa5U)
#define DDM_FEEDBACK_NACK (0x5AU)

#ifdef DDM_CRC_XOR
#define DDM_TX_BUFFER_SIZE 11U
#define DDM_RX_BUFFER_SIZE 18U
#define DDM_RX_DATA_SIZE   7U
#elif defined(DDM_CRC_CCITT)
#define DDM_TX_BUFFER_SIZE 12U
#define DDM_RX_BUFFER_SIZE 20U
#define DDM_RX_DATA_SIZE   8U
#endif

typedef enum DDM_state
{
    DDM_SYNC_STATE,
    DDM_BRAKE_STATE,
    DDM_FORCE_BRAKE_STARTUP,
    DDM_FORCE_BRAKE_CANCEL,
    DDM_FORCE_BRAKE_STARTUP_DONE,
    DDM_OPERATION_STATE,
    DDM_FAULT_STATE
} DDM_state_t;

typedef enum DDM_command
{
    DDM_COMMAND_PING = 0x02,
    DDM_COMMAND_REQUEST = 0x05,
    DDM_COMMAND_CW = 0x09,
    DDM_COMMAND_CCW = 0x0b,
    DDM_COMMAND_AGITATION = 0x0c,
    DDM_COMMAND_SPIN = 0x0d,
    DDM_COMMAND_SPIN_PULSATOR = 0x1d,
    DDM_COMMAND_STOP = 0x027,
    DDM_COMMAND_BRAKE = 0x29,
    DDM_COMMAND_VOLTAGE_DETECTION = 0x2b,
    DDM_COMMAND_WEIGHT_DETECTION = 0x2c,
    DDM_COMMAND_FABRIC_DETECTION = 0x2d,
    DDM_COMMAND_CLUTCH = 0x19,
    DDM_COMMAND_DECLUTCH = 0x1A,
    DDM_COMMAND_FORCED_BRAKE = 0x1b
} DDM_command_t;

typedef enum DDM_dataIndex
{
    DDM_DATA_INDEX_HEADER = 0,
    DDM_DATA_INDEX_MODE = 1,
    DDM_DATA_INDEX_ERROR = 2,
    DDM_DATA_INDEX_PLATFORM = 2,
    DDM_DATA_INDEX_MOTOR_SPEED_HIGH_BYTE = 3,
    DDM_DATA_INDEX_MOTOR_SPEED_LOW_BYTE = 4,
    DDM_DATA_INDEX_DC_BUS_VOLTAGE_HIGH_BYTE = 3,
    DDM_DATA_INDEX_DC_BUS_VOLTAGE_LOW_BYTE = 4,
    DDM_DATA_INDEX_SW_VERSION_HIGH_BYTE = 3,
    DDM_DATA_INDEX_SW_VERSION_LOW_BYTE = 4,
    DDM_DATA_INDEX_WEIGHT_HIGH_BYTE = 3,
    DDM_DATA_INDEX_WEIGHT_LOW_BYTE = 4,
    DDM_DATA_INDEX_WEIGHT = 5,
    DDM_DATA_INDEX_FABRIC = 5,
    DDM_DATA_INDEX_CLUTCH_DONE_FLAG = 5,
    DDM_DATA_INDEX_CRC = 6
} DDM_dataIndex_t;

typedef struct DDM_motionPattern
{
    uint32_t cwOnTimeMSec;
    uint32_t cwOffTimeMSec;
    uint32_t ccwOnTimeMSec;
    uint32_t ccwOffTimeMSec;
    uint16_t accTimeMSec;
    uint16_t decTimeMSec;
    uint16_t rpm;
} DDM_motionPattern_t;

typedef struct DDM
{
    uart_t uartx;
    uint32_t txIntervalMS;   // between adjacent commands
    uint32_t receiveTimeoutMS;
    uint8_t platform;
    uint16_t rawTubValue;
} DDM_t;

void DDM_init(DDM_t *motorConfig);

void DDM_update(uint32_t period);

void DDM_command_set(DDM_command_t command, const DDM_motionPattern_t *pattern);

DDM_state_t DDM_state_get(void);
void DDM_state_set(DDM_state_t value);

uint16_t DDM_weightGramFeedback_get(void);
void DDM_weightGramFeedback_set(uint16_t value);
uint16_t DDM_weightRawFeedback_get(void);

uint16_t DDM_swVersion_get(void);

void DDM_clutchDoneFlag_set(uint8_t value);
uint8_t DDM_clutchDoneFlag_get(void);

DDM_error_t DDM_error_get(void);
void DDM_error_set(DDM_error_t value);
uint32_t DDM_speed_get(void);
void DDM_speed_set(uint32_t value);

#endif
