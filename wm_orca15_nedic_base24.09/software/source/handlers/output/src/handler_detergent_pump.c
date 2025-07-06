#include "doutput_module/doutput_module.h"
#include "def.h"
#include "board.h"

#include "handler_detergent_pump.h"

static doutputModule_handle gx_detergentPump;

#ifdef COMP_MOCKER_ENABLE
static uint8_t gu8_mock_dePumpState;
#endif

void handler_detergentPump_init(void)
{
    doutputModule_t pump;

    pump.gpio.port = DETERGENT_PUMP_PORT;
    pump.gpio.pin = DETERGENT_PUMP_PIN;
    pump.gpio.state = HANDLER_DETERGENT_PUMP_INACTIVE;
    gx_detergentPump = doutputModule_init(&pump);
}

void handler_detergentPump_state_set(handler_detergentPump_state_t state)
{
#ifdef COMP_MOCKER_ENABLE
    (void)doutputModule_state_set(gx_detergentPump, state);
    gu8_mock_dePumpState = state;
#else
    (void)doutputModule_state_set(gx_detergentPump, state);
#endif
}

handler_detergentPump_state_t handler_detergentPump_state_get(void)
{
#ifdef COMP_MOCKER_ENABLE
    (void)doutputModule_state_get(gx_detergentPump);
    return gu8_mock_dePumpState;
#else
    return (handler_detergentPump_state_t)doutputModule_state_get(gx_detergentPump);
#endif
}
