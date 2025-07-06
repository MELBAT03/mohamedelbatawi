#ifndef SANDWICH_H
#define SANDWICH_H

#include "mcal.h"

void timerSandwich_init(mcal_timer_t *px_timer);
void timerSandwich_timeMS_set(mcal_timer_t *px_timer, uint32_t u32_timeMS);
void timerSandwich_timeMS_start(mcal_timer_t *px_timer);
void timerSandwich_timeUS_set(mcal_timer_t *px_timer, uint32_t u32_timeUS);
void timerSandwich_timeUS_start(mcal_timer_t *px_timer);
void timerSandwich_timeout_wait(mcal_timer_t *px_timer);

#endif
