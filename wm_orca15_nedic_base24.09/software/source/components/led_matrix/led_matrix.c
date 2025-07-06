#include "component_config.h"
#include "def.h"
#include "gpio/gpio.h"
#include "led_matrix/led_matrix.h"
#include "led_matrix/led_matrix_def.h"
#include "components/keypad/keypad.h"

static void ledMatrix_coloumn_set(uint8_t coloum, uint8_t state);
static void ledMatrix_output_set(uint8_t coloum);

/*
    aaaaa
    f	b
    ggggg
    e	c
    ddddd
*/
const uint8_t gau8_ssd_dataArray[SSD_SYMMBOL_NUM] = {
    // 0bHGFEDCBA
    0x3F,   //~0b00111111, // 0
    0x06,   //~0b00000110, // 1
    0x5B,   //~0b01011011, // 2
    0x4F,   //~0b01001111, // 3
    0x66,   //~0b01100110, // 4
    0x6D,   //~0b01101101, // 5
    0x7D,   //~0b01111101, // 6
    0x07,   //~0b00000111, // 7
    0x7F,   //~0b01111111, // 8
    0x6F,   //~0b01101111, // 9
    0x77,   //~0b01110111, // A
    0x7C,   //~0b01111100, // B
    0x39,   //~0b00111001, // C
    0x5E,   //~0b01011110, // D
    0x79,   //~0b01111001, // E
    0x71,   //~0b01110001, // F
    0x6F,   //~0b01101111, // G
    0x74,   //~0b01110100, // h
    0x30,   //~0b00110000, // I
    0x0E,   //~0b00001110, // J
    0x00,   //~0b00000000, // K---
    0x08,   //~0b00001000, // 1 dash
    0x50,   //~0b01010000, // r
    0x6D,   //~0b01101101, // S
    0x78,   //~0b01111000, // t---
    0x31,   //~0b00110001, // R---
    0x3E,   //~0b00111110, // U
    0x37,   //~0b00110111, // n---
    0x73,   //~0b01110011, // P
    0x1C,   //~0b00011100, // u
    0x76,   //~0b01110110, // H
    0x5E,   //~0b01011110, // d
    0x38,   //~0b00111000, // L
    0x7F,   //~0b10000000, // dot
    0x00,   //~0b00000000, // NULL
    0x7B,   //~0b01111011, // e
    0x5F,   //~0b01011111, // a
    0x7C,   //~0b01111100, // b
    0x67,   //~0b01100111, // q
    0x58,   //~0b01011000, // c
    0x5C,   //~0b01011100, // o
    0x49,   //~0b01001001, //Triple Dash
    0x01,   //~0b00000001, //seg a
    0x02,   //~0b00000010, //seg b
    0x04,   //~0b00000100, //seg c
    0x08,   //~0b00001000, //seg d
    0x10,   //~0b00010000, //seg e
    0x20,   //~0b00100000, //seg f
    0x40,   //~0b01000000, //seg g
    0xff    //~0b11111111, //ALL_ON
};

static ledMatrix_t gx_ledMatrix_internalHandler;
static uint8_t gax_ledMatrix_rowData[LED_MATRIX_ROW_NUMBER][LED_MATRIX_COL_NUMBER];
#if LED_MATRIX_PWM_ROW_NUMBER != 0
static mcal_pwmConfig_t gax_ledMatrix_pwmRowData[LED_MATRIX_PWM_ROW_NUMBER];
static uint8_t gu8_ledMatrix_pwmRowDataNum[LED_MATRIX_PWM_ROW_NUMBER];
#endif

void ledMatrix_init(ledMatrix_t *px_matrixHandler)
{
    uint8_t i = 0;

    for (i = 0; i < LED_MATRIX_COL_NUMBER; i++)
    {
        gx_ledMatrix_internalHandler.colPort[i] = px_matrixHandler->colPort[i];
        gx_ledMatrix_internalHandler.colPin[i] = px_matrixHandler->colPin[i];
        gpio_pinIOState_set(px_matrixHandler->colPort[i], px_matrixHandler->colPin[i], GPIO_OUTPUT);
        gpio_pinState_set(px_matrixHandler->colPort[i], px_matrixHandler->colPin[i], LED_MATRIX_OFF);
    }

    for (i = 0; i < LED_MATRIX_ROW_NUMBER; i++)
    {
        gpio_pinIOState_set(px_matrixHandler->rowPort[i], px_matrixHandler->rowPin[i], GPIO_OUTPUT);
        gx_ledMatrix_internalHandler.rowPort[i] = px_matrixHandler->rowPort[i];
        gx_ledMatrix_internalHandler.rowPin[i] = px_matrixHandler->rowPin[i];
        gpio_pinState_set(px_matrixHandler->rowPort[i], px_matrixHandler->rowPin[i], LED_MATRIX_OFF);
    }

#if LED_MATRIX_ALTER_ROW_NUMBER != 0
    for (i = 0; i < LED_MATRIX_ALTER_ROW_NUMBER; i++)
    {
        gpio_pinIOState_set(px_matrixHandler->alterRowPort[i], px_matrixHandler->alterRowPin[i], GPIO_OUTPUT);
        gx_ledMatrix_internalHandler.alterRowPort[i] = px_matrixHandler->alterRowPort[i];
        gx_ledMatrix_internalHandler.alterRowPin[i] = px_matrixHandler->alterRowPin[i];
        gpio_pinState_set(px_matrixHandler->alterRowPort[i], px_matrixHandler->alterRowPin[i], LED_MATRIX_OFF);
    }
#endif

#if LED_MATRIX_PWM_ROW_NUMBER != 0
    for (i = 0; i < LED_MATRIX_PWM_ROW_NUMBER; i++)
    {
        gax_ledMatrix_pwmRowData[i].channel = px_matrixHandler->pwmRow[i].channel;
        gax_ledMatrix_pwmRowData[i].duty = px_matrixHandler->pwmRow[i].duty;
        gax_ledMatrix_pwmRowData[i].freq = px_matrixHandler->pwmRow[i].freq;
        gax_ledMatrix_pwmRowData[i].mode = px_matrixHandler->pwmRow[i].mode;
        gax_ledMatrix_pwmRowData[i].port = px_matrixHandler->pwmRow[i].port;
        gax_ledMatrix_pwmRowData[i].pin = px_matrixHandler->pwmRow[i].pin;

        gu8_ledMatrix_pwmRowDataNum[i] = px_matrixHandler->pwmRowNum[i];
        mcal_pwm_channel_init(gax_ledMatrix_pwmRowData[i].channel, &gax_ledMatrix_pwmRowData[i]);
        mcal_pwm_channelState_set(gax_ledMatrix_pwmRowData[i].channel, MCAL_PWM_START);
    }
#endif
}

void ledMatrix_update(uint32_t period)
{
    static uint8_t col = 0;
#if KEYPAD_EMBEDDED_MATRIX == 1
    static uint8_t keypadCol = 0;
#endif
    uint8_t i = 0;

    for (i = 0; i < LED_MATRIX_COL_NUMBER; i++)
    {
        ledMatrix_coloumn_set(i, LED_MATRIX_COLOUMN_OFF);
    }

    ledMatrix_output_set(col);

    ledMatrix_coloumn_set(col, LED_MATRIX_COLOUMN_ON);

#if KEYPAD_EMBEDDED_MATRIX == 1
    if (keypadCol < KEYPAD_COL_NUMBER)
    {
        keypad_scan(keypadCol);
        keypadCol++;
    }
#endif

    col++;
    if (col > LED_MATRIX_COL_NUMBER - 1)
    {
        col = 0;
#if KEYPAD_EMBEDDED_MATRIX == 1
        keypadCol = 0;
#endif
    }
}

void ledMatrix_led_set(ledMatrix_ledEnum_t led, uint8_t value)
{
    uint8_t ledCol, ledRow;

    ledCol = (uint8_t)led & 0x0f;
    ledRow = ((uint8_t)led >> 4U) & 0x0f;

    gax_ledMatrix_rowData[ledRow][ledCol] = value;
}

void ledMatrix_ssd_set(ledMatrix_ssd_t *ssd, ssd_symbolEnum_t symbol)
{
    uint8_t ledCol;
    uint8_t ledRow;

    for (uint8_t i = 0; i < 7; i++)
    {
        ledCol = (uint8_t)ssd->seg[i] & 0x0f;
        ledRow = ((uint8_t)ssd->seg[i] >> 4U) & 0x0f;

        gax_ledMatrix_rowData[ledRow][ledCol] = ((gau8_ssd_dataArray[symbol] >> i) & 0x01);
    }
}

void ledMatrix_ssdDP_set(ledMatrix_ledEnum_t ssdDP, uint8_t dpState)
{
    uint8_t ledCol;
    uint8_t ledRow;

    ledCol = (uint8_t)ssdDP & 0x0f;
    ledRow = ((uint8_t)ssdDP >> 4U) & 0x0f;

    gax_ledMatrix_rowData[ledRow][ledCol] = dpState;
}

void ledMatrix_off(void)
{
    uint8_t rowIndex = 0;
    uint8_t colIndex = 0;

    for (rowIndex = 0; rowIndex < LED_MATRIX_ROW_NUMBER; rowIndex++)
    {
        for (colIndex = 0; colIndex < LED_MATRIX_COL_NUMBER; colIndex++)
        {
            gax_ledMatrix_rowData[rowIndex][colIndex] = LED_MATRIX_OFF;
        }
    }
}

void ledMatrix_on(void)
{
    uint8_t rowIndex = 0;
    uint8_t colIndex = 0;

    for (rowIndex = 0; rowIndex < LED_MATRIX_ROW_NUMBER; rowIndex++)
    {
        for (colIndex = 0; colIndex < LED_MATRIX_COL_NUMBER; colIndex++)
        {
            gax_ledMatrix_rowData[rowIndex][colIndex] = LED_MATRIX_ON;
        }
    }
}

static void ledMatrix_coloumn_set(uint8_t coloum, uint8_t state)
{
    gpio_pinState_set(gx_ledMatrix_internalHandler.colPort[coloum], gx_ledMatrix_internalHandler.colPin[coloum], state);
}

static void ledMatrix_output_set(uint8_t coloum)
{
    uint8_t i = 0;

    for (i = 0; i < LED_MATRIX_ROW_NUMBER; i++)
    {
#if LED_MATRIX_PWM_ROW_NUMBER != 0
        if ((LED_MATRIX_PWM_ROW_NUMBER > i) && (gu8_ledMatrix_pwmRowDataNum[i] == i))
        {
            gax_ledMatrix_pwmRowData[i].duty = gax_ledMatrix_rowData[i][coloum];
            mcal_pwm_channel_set(gax_ledMatrix_pwmRowData[i].channel, &gax_ledMatrix_pwmRowData[i]);
        }
        else
        {
            gpio_pinState_set(gx_ledMatrix_internalHandler.rowPort[i], gx_ledMatrix_internalHandler.rowPin[i], gax_ledMatrix_rowData[i][coloum]);
        }
#elif LED_MATRIX_ALTER_ROW_NUMBER != 0
        if ((gax_ledMatrix_rowData[i][coloum]) > (1))
        {
            gpio_pinState_set(gx_ledMatrix_internalHandler.alterRowPort[i], gx_ledMatrix_internalHandler.alterRowPin[i], ((gax_ledMatrix_rowData[i][coloum]) > (0)));
            gpio_pinState_set(gx_ledMatrix_internalHandler.rowPort[i], gx_ledMatrix_internalHandler.rowPin[i], LED_MATRIX_OFF);
        }
        else
        {
            gpio_pinState_set(gx_ledMatrix_internalHandler.alterRowPort[i], gx_ledMatrix_internalHandler.alterRowPin[i], LED_MATRIX_OFF);
            gpio_pinState_set(gx_ledMatrix_internalHandler.rowPort[i], gx_ledMatrix_internalHandler.rowPin[i], ((gax_ledMatrix_rowData[i][coloum]) > (0)));
        }
#else
        gpio_pinState_set(gx_ledMatrix_internalHandler.rowPort[i], gx_ledMatrix_internalHandler.rowPin[i], (gax_ledMatrix_rowData[i][coloum] > 0));
#endif
    }
}
