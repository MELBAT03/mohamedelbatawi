#ifndef FOUTPUT_MODULE_H
#define FOUTPUT_MODULE_H 1

#include "mcal.h"
#include "def.h"

typedef mcal_pwm_t foutputModuleInterface_t;

typedef struct
{
    foutputModuleInterface_t pwmInterface;
    mcal_gpio_portEnum_t port;
    mcal_gpio_pinEnum_t pin;
    uint32_t freq;
    uint32_t duty;
    uint32_t period;
    uint8_t type;
    uint8_t foutputID;
    uint8_t enable;
} foutputModule_t;

#define FOUTPUT_MODULE_0 MCAL_PWM_0
#define FOUTPUT_MODULE_1 MCAL_PWM_1
#define FOUTPUT_MODULE_2 MCAL_PWM_2
#define FOUTPUT_MODULE_3 MCAL_PWM_3
#define FOUTPUT_MODULE_4 MCAL_PWM_4
#define FOUTPUT_MODULE_5 MCAL_PWM_5
#define FOUTPUT_MODULE_6 MCAL_PWM_6
#define FOUTPUT_MODULE_7 MCAL_PWM_7

void foutputModule_init(foutputModule_t *x_foutputModule);
void foutputModule_freq_set(uint8_t foutputID, uint32_t freq, uint32_t duty, uint32_t period);
void foutputModule_state_set(uint8_t foutputID, uint8_t state);
uint8_t foutputModule_state_get(uint8_t foutputID);
void foutputModule_update(uint32_t period);

#endif
