/*
 * LedDriver.h
 *
 *  Created on: Aug 29, 2021
 *      Author: MMORGA03
 */

#ifndef SMC_GEN_LEDDRIVER_H_
#define SMC_GEN_LEDDRIVER_H_

#include <iodefine.h>
#include "mcal.h"
#include "IK2108_LedDriver_def.h"
/*TO BE CHANGED with our GPIO Layers*/

#define NUM_OF_ROWS 8
#define DIO_HIGH    PORTA.PODR.BIT.B0 = 1
#define DIO_LOW     PORTA.PODR.BIT.B0 = 0

#define CLK_HIGH PORTA.PODR.BIT.B1 = 1
#define CLK_LOW  PORTA.PODR.BIT.B1 = 0

#define STB_HIGH PORTA.PODR.BIT.B3 = 1
#define STB_LOW  PORTA.PODR.BIT.B3 = 0.

typedef enum
{
    RAW_1_ID,
    RAW_2_ID,
    RAW_3_ID,
    RAW_4_ID,
    RAW_5_ID,
    RAW_6_ID,
    RAW_7_ID,
    RAW_8_ID,
    MAX_RAWS
} RAWS;
typedef enum
{
    DISP_12GRID_10SEG,
    DISP_11GRID_11SEG,
    DISP_10GRID_12SEG,
    DISP_9GRID_13SEG,
    DISP_8GRID_14SEG,
    MAX_DISP_MODE,
} DISP_MODE;

typedef enum
{
    DISP_ON,
    DISP_OFF,
    MAX_DISP_SETTING
} DISP_SETTING;

typedef enum
{
    WRITE_DATA_TO_DISPLAY,
    READ_DATA_FROM_DISPALY,
    READ_KEY_DATA,
    READ_DATA_FROM_COMMAND_REGISTER,
    MAX_WRITE_READ_SETTING
} WRITE_READ_SETTING;

typedef enum
{
    INCREMENT_ADDRESS,
    FIXED_ADDRESS,
    MAX_ADDRESS_MODE_SETTING
} ADDRESS_MODE_SETTING;

typedef enum
{
    NOMRAL_OP_MODE,
    TEST_MODE,
    MAX_MODE_SETTING
} MODE_SETTING;

typedef enum
{
    LEDDRIVER_DIMMING_1_16,
    LEDDRIVER_DIMMING_2_16,
    LEDDRIVER_DIMMING_3_16,
    LEDDRIVER_DIMMING_5_16,
    LEDDRIVER_DIMMING_7_16,
    LEDDRIVER_DIMMING_10_16,
    LEDDRIVER_DIMMING_14_16,
    MAX_DIMMING_SETTING
} DIMMING_SETTING;

typedef struct
{
    DISP_MODE disp_mode;
    DISP_SETTING disp_setting;
    WRITE_READ_SETTING write_read_setting;
    ADDRESS_MODE_SETTING address_mode;
    MODE_SETTING mode_setting;
    uint8_t address;
} IK2108_CONFIG_t;
typedef enum
{
    COMMAND_1,
    COMMAND_2,
    COMMAND_3,
    MAX_COMMANDS
} COMMAND_t;

typedef union
{
    uint8_t Byte;
    struct
    {
        unsigned char b0 : 1;
        unsigned char b1 : 1;
        unsigned char b2 : 1;
        unsigned char b3 : 1;
        unsigned char b4 : 1;
        unsigned char b5 : 1;
        unsigned char b6 : 1;
        unsigned char b7 : 1;

    } BIT;
} ROW;

typedef union
{
    uint8_t Byte;
    struct
    {
        unsigned char dim : 3;
        unsigned char data_update : 1;

    } BIT;
} Dimming;

/*************************************************/

void IK2108_Write_8bit(uint8_t data);
void init_LEDDriver(IK2108_CONFIG_t *config);
void set_display_mode(DISP_MODE disp_mode);
void set_disp_on(DISP_SETTING disp_setting);
void set_read_write_setting(WRITE_READ_SETTING write_read_setting);
void set_address_mode(ADDRESS_MODE_SETTING address_mode);
void set_OP_mode(MODE_SETTING mode_setting);
void send_command(COMMAND_t command);
void ledDriver_update(uint32_t period);
void LedDriver_row_set(uint8_t rownum, uint8_t value);
void LedDriver_LedState_set(ledDriver_ledEnum_t lednum, uint8_t state);
void LedDriver_row_clear(uint8_t rownum);
void LedDriver_clear(void);
void LedDriver_dp_set(uint8_t rownum, uint8_t state);
#endif /* SMC_GEN_LEDDRIVER_H_ */
