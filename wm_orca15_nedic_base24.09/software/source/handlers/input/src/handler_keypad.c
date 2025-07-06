#include "board.h"
#include "def.h"

#include "handler_keypad.h"
#include "components/uart/uart.h"
#include "os.h"
#include "utils.h"
#include "components/dinput_module/dinput_module.h"
#include "components/keypad/keypad.h"
#include "components/com_display/com_display.h"

void handler_keypad_init(void)
{
    keypad_t keypad;

    keypad.type = KEYPAD_REG_INPUT;
    keypad.activeLevel = 1;
    keypad.reg = com_display_keypadBuffer_get();

    keypad.shortPressMultiplier = 1;
    keypad.longPressMultiplier = 150;

    keypad_init(&keypad);
}

handler_keypad_state_t handler_keypad_state_get(keyID_t keyID)
{
    return (handler_keypad_state_t)keypad_state_get((keypad_keyEnum_t)keyID);
}

void handler_keypad_state_set(keyID_t keyID, uint8_t value)
{
    keypad_state_set((keypad_keyEnum_t)keyID, (keypad_stateEnum_t)value);
}

void handler_keypad_edgeState_set(keyID_t keyID, uint8_t value)
{
    keypad_edgeState_set((keypad_keyEnum_t)keyID, (keypad_stateEnum_t)value);
}

handler_keypad_state_t handler_keypad_edgeState_get(keyID_t keyID)
{
    return (handler_keypad_state_t)keypad_edgeState_get((keypad_keyEnum_t)keyID);
}

uint32_t handler_keypad_edgePressedKeyCount_get(void)
{
    return keypad_edgePressedKeyCount_get();
}
uint32_t handler_keypad_pressedKeyCount_get(void)
{
    return keypad_pressedKeyCount_get();
}

void handler_keypad_edgePressedKeyCount_set(uint32_t value)
{
}

uint32_t *handler_keypad_buffer_get(void)
{
    return com_display_keypadBuffer_get();
}
