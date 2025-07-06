#include "handler_relay.h"
#include "board.h"

#include "def.h"
#include "doutput_module/doutput_module.h"

static doutputModule_handle gx_relay;

void handler_relay_init(void)
{
    doutputModule_t relay;

    relay.gpio.port = RELAY_PORT;
    relay.gpio.pin = RELAY_PIN;
    relay.gpio.state = RELAY_INACTIVE;
    gx_relay = doutputModule_init(&relay);
    handler_mainRelay_state_set(RELAY_INACTIVE);
}

void handler_mainRelay_state_set(handler_relay_state_t state)
{
    // NOTE: added douput with gpio. as in case of missing zcd the update won't be called and the relay won't be operational
    gpio_pinState_set(RELAY_PORT, RELAY_PIN, state);
    doutputModule_state_set(gx_relay, state);
}

void handler_DDMRelay_state_set(handler_relay_state_t state)
{
    // do nothing
}
