#include "doutput_module/doutput_module.h"
#include "def.h"
#include "board.h"

#include "handler_softener_pump.h"

static doutputModule_handle gx_softenerPump;

#ifdef COMP_MOCKER_ENABLE
static uint8_t gu8_mock_sPumpState;
#endif

void handler_softenerPump_init(void)
{
    doutputModule_t softenerPump;

    softenerPump.gpio.port = SOFTNER_PUMP_PORT;
    softenerPump.gpio.pin = SOFTNER_PUMP_PIN;
    softenerPump.gpio.state = HANDLER_SOFTENER_PUMP_INACTIVE;
    gx_softenerPump = doutputModule_init(&softenerPump);
}

void handler_softenerPump_state_set(handler_softenerPump_state_t state)
{
#ifdef COMP_MOCKER_ENABLE
    (void)doutputModule_state_set(gx_softenerPump, state);
    gu8_mock_sPumpState = state;
#else
    (void)doutputModule_state_set(gx_softenerPump, state);
#endif
}

handler_softenerPump_state_t handler_softenerPump_state_get(void)
{
#ifdef COMP_MOCKER_ENABLE
    (void)doutputModule_state_get(gx_softenerPump);
    return gu8_mock_sPumpState;
#else
    return (handler_softenerPump_state_t)doutputModule_state_get(gx_softenerPump);
#endif
}
