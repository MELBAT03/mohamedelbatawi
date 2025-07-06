#include "handlers/output/inc/handler_drain_valve_hm15n.h"
#include "board.h"

#include "def.h"
#include "components/doutput_module/doutput_module.h"
#include "os.h"
#include "components/hm15n/hm15n.h"

void handler_drainValve_init(void)
{
    // do nothing
}

handler_drainValve_state_t handler_drainValve_switch(handler_drainValve_state_t state)
{
    return (handler_drainValve_state_t)0;
}

handler_drainValve_state_t handler_drainValve_state_get(void)
{
    return (handler_drainValve_state_t)0;
}

uint8_t handler_drainValve_error_get(void)
{
    return (uint8_t)0;
}

void handler_drainValve_error_set(uint8_t state)
{
}
