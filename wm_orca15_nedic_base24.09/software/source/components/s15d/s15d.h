#ifndef S15D_H
#define S15D_H 1

#include "def.h"
#include "mcal.h"

typedef enum
{
    S15D_RESET_INIT,
    S15D_RESET_DONE,
    S15D_SEND,
    S15D_RESEND,
    S15D_RECEIVE,
    S15D_INTERVAL_WAIT,
    S15D_ERROR,
    S15D_DONE
} s15d_state_t;
typedef struct
{
    mcal_gpio_t port;
    uint8_t s15dID;
    uint32_t frameTimeMSec;
    uint32_t timeoutMSec;
} s15d_config_t;

void s15d_init(const s15d_config_t *s15);

void s15d_update(uint32_t period);

uint32_t *s15d_keypadBuffer_get(void);
s15d_state_t s15d_state_get(void);
uint8_t s15d_displayVersion_get(void);

void s15d_state_set(s15d_state_t value);

#endif