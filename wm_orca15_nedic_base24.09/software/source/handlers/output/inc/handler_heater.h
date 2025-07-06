#ifndef HANDLER_HEATER_H
#define HANDLER_HEATER_H

#include "def.h"

typedef enum handler_heater_state
{
    HANDLER_HEATER_OFF = 0,
    HANDLER_HEATER_PLATE_1 = 1,
    HANDLER_HEATER_PLATE_2 = 2
} handler_heater_state_t;

typedef enum handler_heater_outputState
{
    HEATER_STATE_OFF = 0,
    HEATER_STATE_ON = 1
} handler_heater_outputState_t;

void handler_heater_init(void);
double handler_heater_feedback_get(void);
void handler_heater_feedback_set(double value);
void handler_heater_plate_set(handler_heater_state_t plateState);

#endif
