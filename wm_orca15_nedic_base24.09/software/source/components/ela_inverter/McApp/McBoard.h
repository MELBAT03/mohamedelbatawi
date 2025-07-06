#ifndef MC_BOARD__H
#define MC_BOARD__H

//#include "../core/tmpm370/inc/tmpm370_gpio.h"

/* PMD u-v-w x-y-z pins*/
#define MC_U0_PORT   GPIO_PC
#define MC_U0_PIN    GPIO_BIT_0

#define MC_X0_PORT   GPIO_PC
#define MC_X0_PIN    GPIO_BIT_1

#define MC_V0_PORT   GPIO_PC
#define MC_V0_PIN    GPIO_BIT_2

#define MC_Y0_PORT   GPIO_PC
#define MC_Y0_PIN    GPIO_BIT_3

#define MC_W0_PORT   GPIO_PC
#define MC_W0_PIN    GPIO_BIT_4

#define MC_Z0_PORT   GPIO_PC
#define MC_Z0_PIN    GPIO_BIT_5

/* EMG emergency protecrion pins */
#define MC_EMG0_PORT   GPIO_PC
#define MC_EMG0_PIN    GPIO_BIT_6

/* OVV protection control */
#define MC_OVV0_PORT   GPIO_PC
#define MC_OVV0_PIN    GPIO_BIT_7


/* HALL sensors */
//A
#define MC_HALL_A_PORT   GPIO_PD
#define MC_HALL_A_PIN    GPIO_BIT_0

//B
#define MC_HALL_B_PORT   GPIO_PD
#define MC_HALL_B_PIN    GPIO_BIT_1

/* ADC channels */
// U-PHASE
#define MC_ADC_U_PHASE_PORT    GPIO_PI
#define MC_ADC_U_PHASE_PIN     GPIO_BIT_3

// V-PHASE
#define MC_ADC_V_PHASE_PORT    GPIO_PI
#define MC_ADC_V_PHASE_PIN     GPIO_BIT_2

// W-PHASE
#define MC_ADC_W_PHASE_PORT    GPIO_PI
#define MC_ADC_W_PHASE_PIN     GPIO_BIT_1







#endif