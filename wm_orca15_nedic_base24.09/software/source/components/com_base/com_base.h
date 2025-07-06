/****************************************************************************
 * Title                 :   Communication Dryer11/13Kg base
 * Filename              :   com_base.h
 * Author                :   Amgd Raghed
 * Origin Date           :   2/27/2024
 * Version               :   1.0.0 (s15b Component Refactoring)
 * Compiler              :   TBD
 * Target                :   Component Layer
 *****************************************************************************/
/** @file com_base.h
 *  @brief  Communication component for dryer11/13Kg display board to communicate with base board
 *  Data is transmitted through a UART interface as a packet of bytes.
 *  The sending side configures the byte packet and packs data.
 *  The other side receives the packet checks it and parses it if the packet arrived undamaged.
 *
 *   *****************************************
 *   *       UART Frame Protocol Design      *
 *   *****************************************
 *   +--------+---------+--------------+-----+
 *   | Header | Seq Num | Data 1 ... n | CRC |
 *   +--------+---------+--------------+-----+
 *
 *  Each new Packet starts with a “Header” byte,
 *  This byte is used to determine the beginning of the packet,
 *  (Also you can use different Header values to define a particular command of communication)
 *  and reports the sequence number to make sure the communication is going correctly,
 *  and fill that bytes which contain data follow it,
 *  The specified number of bytes of data is followed by the “CRC" byte.
 *  The sending part calculate the “CRC” for the data and sends the package.
 *  The receiving part reads the package and considers its crc.
 *  If the crc is equal to the one specified by the sending side,
 *  the packet arrived undamaged and it can be used.
 */

#ifndef COM_BASE_H
#define COM_BASE_H

/******************************************************************************
 * Includes
 *******************************************************************************/
#include "def.h"
#include "mcal.h"

/******************************************************************************
 * Typedefs
 *******************************************************************************/
/**
 * Defines the possible states for communication.
 */
typedef enum
{
    COM_BASE_RESET,
    COM_BASE_RESET_RELEASE,
    COM_BASE_SYNC,
    COM_BASE_SYNC_ACK,
    COM_BASE_WAIT_HEADER,
    COM_BASE_SEND,
    COM_BASE_RECEIVE
} com_base_state_t;

/**
 * Defines the communication configuration elements that are used
 * by comm_mng_displayinit to configure.
 */
typedef struct
{
    uint32_t (*ptr_keypadGeter)(void);
    mcal_gpio_t port;
    uint8_t commID;
    uint32_t totalTimeMS;
    mcal_gpio_t resetSignalPin;
    uint8_t (*resetReleaseSignal)(void);
} com_base_config_t;

void com_base_init(com_base_config_t *com_base_config);

void com_base_update(uint32_t period);

uint8_t com_base_buffer_get(uint8_t index);

/*FOR TESTING*/
void com_base_buffer_set(uint8_t index, uint8_t value);

void com_base_state_set(com_base_state_t stateValue);

void com_base_reset(void);

#endif
