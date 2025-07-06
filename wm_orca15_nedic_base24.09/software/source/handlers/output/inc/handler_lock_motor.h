#ifndef HANDLER_LOCK_MOTOR_H
#define HANDLER_LOCK_MOTOR_H 1

#include "def.h"
#include "components/dls35s/dls35s.h"

typedef dls35s_state_t handler_lockMotor_state_t;

#define HANDLER_LOCK_MOTOR_ACTIVE   DLS35S_ACTIVE
#define HANDLER_LOCK_MOTOR_INACTIVE DLS35S_INACTIVE
#define HANDLER_LOCK_MOTOR_RESET    DLS35S_RESET

void handler_lockMotor_init(void);

handler_lockMotor_state_t handler_lockMotor_state_get(void);
void handler_lockMotor_state_set(handler_lockMotor_state_t u8_state);

uint8_t handler_lockMotor_sensorState_get(void);
void handler_lockMotor_sensorState_set(uint8_t value);

uint8_t handler_lockMotor_error_get(void);
void handler_lockMotor_error_set(uint8_t value);

#endif
