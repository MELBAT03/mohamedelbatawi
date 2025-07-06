#ifndef HANDLER_KEYPAD_H
#define HANDLER_KEYPAD_H

#include "def.h"
#include "components/uart/uart.h"

#include "keypad/keypad_def.h"
#include "keypad/keypad.h"

// NOTE: get the key value from the touch board allocated by QE touch
typedef enum keyID
{
    KEY_COTTON_ID = 11,
    KEY_WHITE_ID = 0,
    KEY_BEDDING_ID = 4,
    KEY_MIX_ID = 1,
    KEY_DARKS_ID = 3,
    KEY_BABYCARE_ID = 6,
    KEY_SENSITIVE_ID = 8,
    KEY_LIGHT_ID = 10,
    KEY_ECO_ID = 7,
    KEY_SPORTS_ID = 5,
    KEY_JEANS_ID = 2,
    KEY_ALERGY_ID = 9,

    KEY_WATER_LEVEL_ID = 23,
    KEY_WATER_TEMP_ID = 12,
    KEY_WASH_ID = 22,
    KEY_DELAY_START_ID = 14,
    KEY_RINSE_ID = 21,
    KEY_GEL_DETERGENT_ID = 20,
    KEY_SPIN_ID = 15,
    KEY_TUBCLEAN_ID = 16,
    KEY_STAIN_LEVEL_ID = 19,
    KEY_STEAM_TECH_ID = 13,

    KEY_START_PAUSE_ID = 17,
    KEY_ON_OFF_ID = 18,

    KEY_WOOL_ID = KEY_ECO_ID,
    KEY_RAPIDWASH_ID = KEY_ALERGY_ID,

    KEY_SOAK_ID = KEY_STEAM_TECH_ID,
    KEY_EXTRA_SPIN_ID = KEY_GEL_DETERGENT_ID,
} keyID_t;

typedef enum handler_keypad_state
{
    HANDLER_KEY_RELEASED = KEYPAD_KEY_RELEASED,
    HANDLER_KEY_PRESSED = KEYPAD_KEY_PRESSED_SHORT_PRESS,
    HANDLER_KEY_LONG_PRESSED = KEYPAD_KEY_PRESSED_LONG_PRESS
} handler_keypad_state_t;

void handler_keypad_init(void);

handler_keypad_state_t handler_keypad_state_get(keyID_t keyID);
handler_keypad_state_t handler_keypad_edgeState_get(keyID_t keyID);
uint32_t handler_keypad_edgePressedKeyCount_get(void);
uint32_t handler_keypad_pressedKeyCount_get(void);

void handler_keypad_edgeState_set(keyID_t keyID, uint8_t value);
void handler_keypad_state_set(keyID_t keyID, uint8_t value);
void handler_keypad_edgePressedKeyCount_set(uint32_t value);
uint32_t *handler_keypad_buffer_get(void);

#endif
