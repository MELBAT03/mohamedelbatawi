/****************************************************************************
 * Title                 :   Communication Dryer11/13Kg Display
 * Filename              :   com_display.h
 * Author                :   Amgd Raghed
 * Origin Date           :   2/21/2024
 * Version               :   1.0.0 (s15d Component Refactoring)
 * Compiler              :   TBD
 * Target                :   Component Layer
 *****************************************************************************/

/** @file com_display.h
 *  @brief  Communication component for dryer11/13Kg base board to communicate with display board
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

#ifndef COM_DISPLAY_H_
#define COM_DISPLAY_H_

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
    // COM_DISPLAY_RESET_INIT,
    // COM_DISPLAY_RESET_DONE,
    COM_DISPLAY_SYNC,
    COM_DISPLAY_SYNC_CHECK,
    COM_DISPLAY_INTERVAL_WAIT,
    COM_DISPLAY_SEND,
    COM_DISPLAY_RECEIVE,
    COM_DISPLAY_RESEND,
    COM_DISPLAY_ERROR,
    COM_DISPLAY_DONE
} com_display_state_t;

/**
 * Defines the communication configuration elements that are used
 * by comm_mng_displayinit to configure.
 */
typedef struct
{
    uint8_t (*ptr_dimGeter)(void);
    uint8_t (*ptr_ssdGeter)(uint8_t);
    uint8_t (*ptr_dpGeter)(void);
    uint8_t (*ptr_ledGeter)(uint8_t);
    mcal_gpio_t resetSignalPin;
    uint32_t frameWaitTimeMSec;
    uint32_t receiveTimeoutMSec;
    uint8_t commDisplayID;
} com_display_config_t;

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/
void com_display_init(const com_display_config_t *com_display_config);

void com_display_update(uint32_t period);

com_display_state_t com_display_state_get(void);

void com_display_state_set(com_display_state_t value);

uint32_t *com_display_keypadBuffer_get(void);

uint8_t com_display_displayVersion_get(void);

#endif /* COM_DISPLAY_H_ */
