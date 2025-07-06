/*
 * IK2108_LEDDriver_def.h
 *
 *  Created on: Sep 21, 2021
 *      Author: MMORGA03
 */

#ifndef IK2108_LEDDRIVER_DEF_H_
#define IK2108_LEDDRIVER_DEF_H_

#include "mcal.h"

#define SSD_DP_ON  1
#define SSD_DP_OFF 0

typedef enum
{
    // COL0
    LEDMX00 = 0,
    LEDMX10 = 10,
    LEDMX20 = 20,
    LEDMX30 = 30,
    LEDMX40 = 40,
    LEDMX50 = 50,
    LEDMX60 = 60,
    LEDMX70 = 70,
    LEDMX80 = 80,
    // COL1
    LEDMX01 = 1,
    LEDMX11 = 11,
    LEDMX21 = 21,
    LEDMX31 = 31,
    LEDMX41 = 41,
    LEDMX51 = 51,
    LEDMX61 = 61,
    LEDMX71 = 71,
    LEDMX81 = 81,
    // COL2
    LEDMX02 = 2,
    LEDMX12 = 12,
    LEDMX22 = 22,
    LEDMX32 = 32,
    LEDMX42 = 42,
    LEDMX52 = 52,
    LEDMX62 = 62,
    LEDMX72 = 72,
    LEDMX82 = 82,
    // COL3
    LEDMX03 = 3,
    LEDMX13 = 13,
    LEDMX23 = 23,
    LEDMX33 = 33,
    LEDMX43 = 43,
    LEDMX53 = 53,
    LEDMX63 = 63,
    LEDMX73 = 73,
    LEDMX83 = 83,

    LEDMXALL = 0xff
} ledDriver_ledEnum_t;

typedef union
{
    unsigned char Byte;
    struct
    {
        unsigned char disp_mode_bits : 3;
        unsigned char disp_setting : 1;
    } BIT;
} Display_Mode_Setting_t;

typedef union
{
    unsigned char Byte;
    struct
    {
        unsigned char data_write_read_setting : 2;
        unsigned char address_increment : 1;
        unsigned char mode_setting_bits : 1;

    } BIT;
} Data_Setting_t;

typedef union
{
    unsigned char Byte;
    struct
    {
        unsigned char address : 6;
        unsigned char b6 : 1;
        unsigned char b7 : 1;

    } BIT;
} Address_Setting_t;

#endif /* IK2108_LEDDRIVER_DEF_H_ */
