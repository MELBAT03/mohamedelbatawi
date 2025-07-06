#include "board.h"
#include "def.h"

#include "handler_keypad.h"
#include "handler_off_sw.h"

void handler_offSW_init(void)
{
    // do nothing
}

handler_offSW_state_t handler_offSW_state_get(void)
{
    return (handler_offSW_state_t)keypad_state_get((keypad_keyEnum_t)KEY_ON_OFF_ID);
}

void handler_offSW_state_set(handler_offSW_state_t value)
{
    keypad_state_set((keypad_keyEnum_t)KEY_ON_OFF_ID, (keypad_stateEnum_t)value);
    keypad_edgeState_set((keypad_keyEnum_t)KEY_ON_OFF_ID, (keypad_stateEnum_t)value);
}
