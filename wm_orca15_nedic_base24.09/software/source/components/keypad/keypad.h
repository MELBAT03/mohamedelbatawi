#ifndef KEYPAD_H
#define KEYPAD_H

#include "mcal.h"
#include "component_config.h"
#include "gpio/gpio.h"
#include "keypad_def.h"

typedef enum
{
    KEYPAD_INPUT_NONE = 0,
    KEYPAD_REG_INPUT = 1,
    KEYPAD_GPIO_INPUT = 2
} keypad_inputType_t;

typedef struct
{
    keypad_inputType_t type;
    uint32_t *reg;   // :NOTE: if the reading is from a register, either use this or the gpio option
    uint8_t activeLevel;
    gpio_port_t colPort[KEYPAD_COL_NUMBER];
    gpio_port_t rowPort[KEYPAD_ROW_NUMBER];
    gpio_pin_t colPin[KEYPAD_COL_NUMBER];
    gpio_pin_t rowPin[KEYPAD_ROW_NUMBER];
    uint32_t shortPressMultiplier;   // multiplies of the period
    uint32_t longPressMultiplier;
} keypad_t;

#define KEYPAD_KEY_PRESSED (KEYPAD_KEY_PRESSED_SHORT_PRESS)
typedef enum
{
    KEYPAD_KEY_RELEASED,
    KEYPAD_KEY_PRESSED_SHORT_PRESS,
    KEYPAD_KEY_PRESSED_LONG_PRESS,
    KEYPAD_KEY_FAULT = -1
} keypad_stateEnum_t;

void keypad_init(keypad_t *px_keypadHandler);
void keypad_activeSwitchPerCol_scan(uint8_t u8_col);
void keypad_scan(uint8_t col);
keypad_stateEnum_t keypad_state_get(keypad_keyEnum_t key);
void keypad_state_set(keypad_keyEnum_t key, keypad_stateEnum_t value);
keypad_stateEnum_t keypad_edgeState_get(keypad_keyEnum_t key);
void keypad_edgeState_set(keypad_keyEnum_t key, keypad_stateEnum_t value);
keypad_stateEnum_t keypad_onReleaseState_get(keypad_keyEnum_t key);
uint32_t keypad_longPressTicks_get(keypad_keyEnum_t key);
void keypad_shortPressMultiplier_set(keypad_keyEnum_t key, uint32_t u32_newMultiplier);
void keypad_longPressMultiplier_set(keypad_keyEnum_t key, uint32_t u32_newMultiplier);
void keypad_keyMirror_get(uint8_t (*arr)[4]);
void keypad_keyEdgeMirror_get(uint8_t (*arr)[4]);
void keypad_keyEdge_clear(void);
void keypad_update(uint32_t period);

uint32_t keypad_pressedKeyCount_get(void);
uint32_t keypad_edgePressedKeyCount_get(void);

#endif
