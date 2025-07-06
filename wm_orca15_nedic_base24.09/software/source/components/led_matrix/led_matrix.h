#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "def.h"
#include "component_config.h"
#include "gpio/gpio.h"
#include "led_matrix/led_matrix_def.h"

typedef enum
{
    SSD_0_SYMBOL = 0,
    SSD_1_SYMBOL,
    SSD_2_SYMBOL,
    SSD_3_SYMBOL,
    SSD_4_SYMBOL,
    SSD_5_SYMBOL,
    SSD_6_SYMBOL,
    SSD_7_SYMBOL,
    SSD_8_SYMBOL,
    SSD_9_SYMBOL,
    SSD_A_SYMBOL,
    SSD_B_SYMBOL,
    SSD_C_SYMBOL,
    SSD_D_SYMBOL,
    SSD_E_SYMBOL,
    SSD_F_SYMBOL,
    SSD_G_SYMBOL,
    SSD_h_SYMBOL,
    SSD_I_SYMBOL,
    SSD_J_SYMBOL,
    SSD_K_SYMBOL,
    SSD_1_DASH_SYMBOL,
    SSD_r_SYMBOL,
    SSD_S_SYMBOL,
    SSD_t_SYMBOL,
    SSD_R_SYMBOL,
    SSD_U_SYMBOL,
    SSD_n_SYMBOL,
    SSD_P_SYMBOL,
    SSD_u_SYMBOL,
    SSD_H_SYMBOL,
    SSD_d_SYMBOL,
    SSD_L_SYMBOL,
    SSD_DOT_SYMBOL,
    SSD_NULL_SYMBOL,
    SSD_e_SYMBOL,
    SSD_a_SYMBOL,
    SSD_b_SYMBOL,
    SSD_q_SYMBOL,
    SSD_c_SYMBOL,
    SSD_o_SYMBOL,
    SSD_TRIPLE_DASH_SYMBOL,
    SSD_A_SEG_SYMBOL,
    SSD_B_SEG_SYMBOL,
    SSD_C_SEG_SYMBOL,
    SSD_D_SEG_SYMBOL,
    SSD_E_SEG_SYMBOL,
    SSD_F_SEG_SYMBOL,
    SSD_G_SEG_SYMBOL,
    SSD_ALL_ON_SYMBOL,
    SSD_SYMMBOL_NUM
} ssd_symbolEnum_t;

typedef struct
{
    gpio_port_t colPort[LED_MATRIX_COL_NUMBER];
    gpio_port_t rowPort[LED_MATRIX_ROW_NUMBER];
    gpio_pin_t colPin[LED_MATRIX_COL_NUMBER];
    gpio_pin_t rowPin[LED_MATRIX_ROW_NUMBER];

#if LED_MATRIX_ALTER_ROW_NUMBER != 0
    gpio_port_t alterRowPort[LED_MATRIX_ALTER_ROW_NUMBER];
    gpio_pin_t alterRowPin[LED_MATRIX_ALTER_ROW_NUMBER];
#endif

#if LED_MATRIX_PWM_ROW_NUMBER != 0
    mcal_pwmConfig_t pwmRow[LED_MATRIX_PWM_ROW_NUMBER];
    uint8_t pwmRowNum[LED_MATRIX_PWM_ROW_NUMBER];
#endif
} ledMatrix_t;

typedef struct ledMatrix_ssd
{
    ledMatrix_ledEnum_t seg[7];
} ledMatrix_ssd_t;

void ledMatrix_init(ledMatrix_t *px_matrixHandler);
void ledMatrix_update(uint32_t period);

void ledMatrix_led_set(ledMatrix_ledEnum_t led, uint8_t value);
void ledMatrix_ssd_set(ledMatrix_ssd_t *ssd, ssd_symbolEnum_t symbol);
void ledMatrix_ssdDP_set(ledMatrix_ledEnum_t ssdDP, uint8_t dpState);
void ledMatrix_off(void);
void ledMatrix_on(void);

#endif
