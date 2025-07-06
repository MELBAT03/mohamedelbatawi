#ifndef HANDLER_BUZZER_H
#define HANDLER_BUZZER_H 1

#include "def.h"
#include "components/buzzer/buzzer.h"

typedef enum handler_buzzer_toneSelection
{
    HANDLER_BUZZER_START_UP_TONE,
    HANDLER_BUZZER_SHUTDOWN_TONE,
    HANDLER_BUZZER_KEY_VALID_PRESS_TONE,
    HANDLER_BUZZER_KEY_INVALID_PRESS_TONE,
    HANDLER_BUZZER_CHILD_LOCK_ACTIVE_TONE,
    HANDLER_BUZZER_CHILD_LOCK_INACTIVE_TONE,
    HANDLER_BUZZER_CYCLE_START_TONE,
    HANDLER_BUZZER_CYCLE_PAUSE_TONE,
    HANDLER_BUZZER_CYCLE_FINISH_TONE,
    HANDLER_BUZZER_FAULT_TONE,
    HANDLER_BUZZER_UNBALANCE_TONE,
    HANDLER_BUZZER_TONE_NUM
} handler_buzzer_toneSelection_t;

typedef enum handler_buzzer_state
{
    HANDLER_BUZZER_INACTIVE = BUZZER_INACTIVE,
    HANDLER_BUZZER_ACTIVE = BUZZER_ACTIVE
} handler_buzzer_state_t;

void handler_buzzer_init(void);
void handler_buzzer_tone_set(handler_buzzer_toneSelection_t tone);
void handler_buzzer_state_set(handler_buzzer_state_t state);

#endif