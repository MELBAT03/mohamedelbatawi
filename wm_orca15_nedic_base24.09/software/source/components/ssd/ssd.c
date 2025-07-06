#include "ssd/ssd.h"
#include "component_config.h"

#include "def.h"
#include "gpio/gpio.h"

/*
//7seg symbol
    aaaaa
    f	b
    ggggg
    e	c
    ddddd
*/
const uint8_t gau8_ssd_dataArray[SSD_SYMMBOL_NUM] =
    {
        // 0bHGFEDCBA
        ~0x3F,   //~0b00111111, // 0
        ~0x06,   //~0b00000110, // 1
        ~0x5B,   //~0b01011011, // 2
        ~0x4F,   //~0b01001111, // 3
        ~0x66,   //~0b01100110, // 4
        ~0x6D,   //~0b01101101, // 5
        ~0x7D,   //~0b01111101, // 6
        ~0x07,   //~0b00000111, // 7
        ~0x7F,   //~0b01111111, // 8
        ~0x6F,   //~0b01101111, // 9
        ~0x77,   //~0b01110111, // A
        ~0x7C,   //~0b01111100, // B
        ~0x39,   //~0b00111001, // C
        ~0x5E,   //~0b01011110, // D
        ~0x79,   //~0b01111001, // E
        ~0x71,   //~0b01110001, // F
        ~0x6F,   //~0b01101111, // G
        ~0x74,   //~0b01110100, // h
        ~0x30,   //~0b00110000, // I
        ~0x0E,   //~0b00001110, // J
        ~0x00,   //~0b00000000, // K---
        ~0x08,   //~0b00001000, // 1 dash
        ~0x50,   //~0b01010000, // r
        ~0x6D,   //~0b01101101, // S
        ~0x78,   //~0b01111000, // t---
        ~0x31,   //~0b00110001, // R---
        ~0x3E,   //~0b00111110, // U
        ~0x64,   //~0b01010100, // n---
        ~0x73,   //~0b01110011, // P
        ~0x1C,   //~0b00011100, // u
        ~0x76,   //~0b01110110, // H
        ~0x5E,   //~0b01011110, // d
        ~0x38,   //~0b00111000, // L
        ~0x7F,   //~0b10000000, // dot
        ~0x00,   //~0b00000000, // NULL
        ~0x7B,   //~0b01111011, // e
        ~0x5F,   //~0b01011111, // a
        ~0x7C,   //~0b01111100, // b
        ~0x67,   //~0b01100111, // q
        ~0x58,   //~0b01011000, // c
        ~0x5C,   //~0b01011100, // o
        ~0x49,   //~0b01001001, //Triple Dash
        ~0x01,   //~0b00000001, //seg a
        ~0x02,   //~0b00000010, //seg b
        ~0x04,   //~0b00000100, //seg c
        ~0x08,   //~0b00001000, //seg d
        ~0x10,   //~0b00010000, //seg e
        ~0x20,   //~0b00100000, //seg f
        ~0x40,   //~0b01000000, //seg g
        ~0xff    //~0b11111111, //ALL_ON
};

typedef struct
{
    ssd_t ssd;
    ssd_symbolEnum_t ssdData;
    uint8_t ssdDP;
} ssd_internalHandler_t;

static ssd_internalHandler_t gx_ssd_internalHandler[SSD_DIGITS_NUM];

void ssd_init(ssd_t *ssd)
{
    uint8_t i = 0;

    for (i = 0; i < SSD_DATA_PINS; i++)
    {
        gx_ssd_internalHandler[ssd->ssdID].ssd.ssdDataPort[i] = ssd->ssdDataPort[i];
        gx_ssd_internalHandler[ssd->ssdID].ssd.ssdDataPin[i] = ssd->ssdDataPin[i];
        gpio_pinIOState_set(gx_ssd_internalHandler[ssd->ssdID].ssd.ssdDataPort[i], gx_ssd_internalHandler[ssd->ssdID].ssd.ssdDataPin[i], GPIO_OUTPUT);
        gpio_pinState_set(gx_ssd_internalHandler[ssd->ssdID].ssd.ssdDataPort[i], gx_ssd_internalHandler[ssd->ssdID].ssd.ssdDataPin[i], SSD_OFF);
    }

    gx_ssd_internalHandler[ssd->ssdID].ssd.ssdDigitPort = ssd->ssdDigitPort;
    gx_ssd_internalHandler[ssd->ssdID].ssd.ssdDigitPin = ssd->ssdDigitPin;
    gpio_pinIOState_set(gx_ssd_internalHandler[ssd->ssdID].ssd.ssdDigitPort, gx_ssd_internalHandler[ssd->ssdID].ssd.ssdDigitPin, GPIO_OUTPUT);
    gpio_pinState_set(gx_ssd_internalHandler[ssd->ssdID].ssd.ssdDigitPort, gx_ssd_internalHandler[ssd->ssdID].ssd.ssdDigitPin, SSD_OFF);

    gx_ssd_internalHandler[ssd->ssdID].ssd.ssdID = ssd->ssdID;
    gx_ssd_internalHandler[ssd->ssdID].ssdData = SSD_NULL_SYMBOL;
    gx_ssd_internalHandler[ssd->ssdID].ssdDP = SSD_DP_OFF;
}

void ssd_value_set(uint8_t ssdID, ssd_symbolEnum_t symbol)
{
    gx_ssd_internalHandler[ssdID].ssdData = symbol;
}

ssd_symbolEnum_t ssd_value_get(uint8_t ssdID)
{
    return gx_ssd_internalHandler[ssdID].ssdData;
}

void ssd_off(void)
{
    uint8_t i = 0;

    for (i = 0; i < SSD_DIGITS_NUM; i++)
    {
        gx_ssd_internalHandler[i].ssdData = SSD_NULL_SYMBOL;
        gx_ssd_internalHandler[i].ssdDP = SSD_DP_OFF;
    }
}

void ssd_on(void)
{
    uint8_t i = 0;

    for (i = 0; i < SSD_DIGITS_NUM; i++)
    {
        gx_ssd_internalHandler[i].ssdData = SSD_8_SYMBOL;
        gx_ssd_internalHandler[i].ssdDP = SSD_DP_ON;
    }
}

void ssd_dp_set(uint8_t ssdID, uint8_t dpState)
{
    gx_ssd_internalHandler[ssdID].ssdDP = dpState;
}

uint8_t ssd_dataArray_get(uint8_t symbol)
{
    return gau8_ssd_dataArray[symbol];
}

void ssd_update(uint32_t period)
{
    static uint8_t gx_ssd_currentSSD = 0;
    uint8_t i = 0;

    for (i = 0; i < SSD_DIGITS_NUM; i++)
    {
        ssd_digit_set(i, SSD_OFF);
    }

    ssd_data_set(gx_ssd_currentSSD);

    ssd_digit_set(gx_ssd_currentSSD, SSD_ON);

    gx_ssd_currentSSD++;
    if (gx_ssd_currentSSD >= SSD_DIGITS_NUM)
    {
        gx_ssd_currentSSD = 0;
    }
}

void ssd_data_set(uint8_t ssdDigit)
{
    uint8_t ssd_data = 0;
    uint8_t i = 0;

#if (SSD_COM_TYPE == SSD_COM_ANODE)
    if (gx_ssd_internalHandler[ssdDigit].ssdDP == SSD_DP_OFF)
    {
        ssd_data = gau8_ssd_dataArray[gx_ssd_internalHandler[ssdDigit].ssdData] | (1 << 7);
    }
    else if (gx_ssd_internalHandler[ssdDigit].ssdDP == SSD_DP_ON)
    {
        ssd_data = gau8_ssd_dataArray[gx_ssd_internalHandler[ssdDigit].ssdData] & ~(1 << 7);
    }
#elif (SSD_COM_TYPE == SSD_COM_CATHODE)
    if (gx_ssd_internalHandler[ssdDigit].ssdDP == SSD_DP_OFF)
    {
        ssd_data = ~(gau8_ssd_dataArray[gx_ssd_internalHandler[ssdDigit].ssdData] & ~(1 << 7));
    }
    else if (gx_ssd_internalHandler[ssdDigit].ssdDP == SSD_DP_ON)
    {
        ssd_data = ~(gau8_ssd_dataArray[gx_ssd_internalHandler[ssdDigit].ssdData] | (1 << 7));
    }
#endif

#if (SSD_DATA_INVERTED == 1)
    ssd_data = ~gau8_ssd_dataArray[gx_ssd_internalHandler[ssdDigit].ssdData];
#endif

    for (i = 0; i < SSD_DATA_PINS; i++)
    {
        gpio_pinState_set(gx_ssd_internalHandler[ssdDigit].ssd.ssdDataPort[i], gx_ssd_internalHandler[ssdDigit].ssd.ssdDataPin[i], ((ssd_data >> i) & 0x01));
    }
}

void ssd_digit_set(uint8_t ssdDigit, uint8_t state)
{
    gpio_pinState_set(gx_ssd_internalHandler[ssdDigit].ssd.ssdDigitPort, gx_ssd_internalHandler[ssdDigit].ssd.ssdDigitPin, state);
}
