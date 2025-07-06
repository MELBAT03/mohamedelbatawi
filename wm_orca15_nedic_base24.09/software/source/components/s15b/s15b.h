#ifndef S15B_H
#define S15B_H 1

#include "def.h"
#include "mcal.h"

typedef enum
{
    S15B_SEND,
    S15B_RECEIVE
} s15b_state_t;

typedef struct
{
    mcal_gpio_t port;
    uint8_t s15bID;
    uint32_t totalTimeMS;
} s15b_config_t;

void s15b_init(s15b_config_t *s15);

void s15b_update(uint32_t period);

uint8_t s15b_buffer_get(uint8_t index);

void s15b_state_set(s15b_state_t value);

#endif