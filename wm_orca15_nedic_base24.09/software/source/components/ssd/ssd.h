#ifndef SSD_H
#define SSD_H

#include "component_config.h"
#include "def.h"
#include "gpio/gpio.h"

typedef struct
{
    gpio_port_t ssdDataPort[SSD_DATA_PINS];
    gpio_pin_t ssdDataPin[SSD_DATA_PINS];
    gpio_port_t ssdDigitPort;
    gpio_pin_t ssdDigitPin;
    uint8_t ssdID;
} ssd_t;

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

void ssd_init(ssd_t *x_ssd);
void ssd_value_set(uint8_t u8_ssdID, ssd_symbolEnum_t x_symbol);
ssd_symbolEnum_t ssd_value_get(uint8_t u8_ssdID);
void ssd_customDataFn_set(void (*ptr)(uint8_t ssdID));
void ssd_off(void);
void ssd_on(void);
void ssd_dp_set(uint8_t u8_ssdID, uint8_t u8_dpState);
uint8_t ssd_dataArray_get(uint8_t symbol);
void ssd_data_set(uint8_t u8_ssdID);
void ssd_digit_set(uint8_t u8_ssdID, uint8_t x_state);
void ssd_update(uint32_t period);

#endif
