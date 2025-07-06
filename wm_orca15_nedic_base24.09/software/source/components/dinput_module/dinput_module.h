#ifndef DINPUT_MODULE_H
#define DINPUT_MODULE_H

#include "mcal.h"
#include "gpio/gpio.h"
#include "def.h"

typedef struct dinputModule_internalHandler_t *dinputModule_handle;

typedef enum inputType_t
{
    DINPUT_REG_INPUT,
    DINPUT_GPIO_INPUT,
} inputType_t;

typedef struct dinputModule_t
{
    inputType_t type;
    mcal_gpio_t gpio;
    uint32_t *reg;   // if the reading is from a register, either use this or the gpio option
    uint8_t activeLevel;
    bool_t pullUpEn;
    uint32_t shortPressMultiplier;   // multiplies of the period (so each sw can have different update interval)
    uint32_t longPressMultiplier;
} dinputModule_t;

typedef enum dinputModule_stateEnum_t
{
    DINPUT_MODULE_RELEASED,
    DINPUT_MODULE_PRESSED_SHORT_PRESS,
    DINPUT_MODULE_PRESSED_LONG_PRESS,
    DINPUT_MODULE_FAULT = -1
} dinputModule_stateEnum_t;

dinputModule_handle dinputModule_init(dinputModule_t *dinputModule);
void dinputModule_update(uint32_t period);
dinputModule_stateEnum_t dinputModule_state_get(dinputModule_handle handle);
dinputModule_stateEnum_t dinputModule_edgeState_get(dinputModule_handle handle);
dinputModule_stateEnum_t dinputModule_onReleaseState_get(dinputModule_handle handle);
void dinputModule_shortPressMultiplier_set(dinputModule_handle handle, uint32_t newMultiplier);
void dinputModule_longPressMultiplier_set(dinputModule_handle handle, uint32_t newMultiplier);

#endif
