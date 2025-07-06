#include "doutput_module/doutput_module.h"
#include "def.h"
#include "board.h"

#include "handler_drain_pump.h"

static doutputModule_handle gx_drainPump;

#ifdef COMP_MOCKER_ENABLE
static uint8_t gu8_mock_dPumpState;
#endif

void handler_drainPump_init(void)
{
    doutputModule_t drainPump;

    drainPump.gpio.port = DRAIN_PUMP_PORT;
    drainPump.gpio.pin = DRAIN_PUMP_PIN;
    drainPump.gpio.state = HANDLER_DRAIN_PUMP_INACTIVE;
    gx_drainPump = doutputModule_init(&drainPump);
}

void handler_drainPump_state_set(handler_drainPump_state_t state)
{
#ifdef COMP_MOCKER_ENABLE
    (void)doutputModule_state_set(gx_drainPump, state);
    gu8_mock_dPumpState = state;
#else
    (void)doutputModule_state_set(gx_drainPump, state);
#endif
}

handler_drainPump_state_t handler_drainPump_state_get(void)
{
#ifdef COMP_MOCKER_ENABLE
    (void)doutputModule_state_get(gx_drainPump);
    return gu8_mock_dPumpState;
#else
    return (handler_drainPump_state_t)doutputModule_state_get(gx_drainPump);
#endif
}
