#ifndef APP_DRAIN_H
#define APP_DRAIN_H

#include "def.h"

#include "app_user_interface.h"

#include "handlers/output/inc/handler_drain_valve.h"

#define APP_DRAIN_WATER_ESTIMATED_INTERVAL       (120000UL)
#define APP_DRAIN_SHORT_WATER_ESTIMATED_INTERVAL (5000UL)

typedef handler_drainValve_state_t app_drain_state_t;
#define APP_DRAIN_OPEN           HANDLER_DRAIN_VALVE_OPEN
#define APP_DRAIN_CLOSE          HANDLER_DRAIN_VALVE_CLOSE
#define APP_DRY_DRAIN_PROCESSING HANDLER_DRAIN_VALVE_PROCESSING

typedef enum app_drain_failure_state
{
    APP_DRAIN_FAILURE_NONE = 0,
    APP_DRAIN_FAILURE_LOW_RATE = 1,
    APP_DRAIN_FAILURE_BLOCKED = 2,
    APP_DRAIN_FAILURE_WATER_DURING_SPIN_NORMAL = 3,
    APP_DRAIN_FAILURE_WATER_DURING_SPIN_DANGEROUS = 4
} app_drain_failure_state_t;

typedef enum app_drain_tubState
{
    APP_DRAIN_TUB_NOT_EMPTY = 0,
    APP_DRAIN_TUB_EMPTY = 1
} app_drain_tubState_t;

void app_drain_update(uint32_t period);

void app_drain_state_set(app_drain_state_t value);
app_drain_state_t app_drain_state_get(void);
void app_drain_switch(app_drain_state_t state);

app_drain_tubState_t app_drain_tubState_get(void);

app_drain_failure_state_t app_drain_failureState_get(void);

uint32_t app_drain_waitTimeoutMSec_get(void);

void app_drain_pumpSafteyOff_handle(uint32_t period);

void app_drain_error_check(uint32_t period);
void app_drain_spinError_check(uint32_t period);

uint8_t app_drain_shortDrainFlag_get(void);
void app_drain_shortDrainFlag_set(uint8_t value);

#endif
