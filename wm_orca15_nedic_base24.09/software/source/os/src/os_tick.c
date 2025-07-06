#include "os_tick.h"
#include "os_systick_handler.h"

volatile uint32_t gu32_os_tick_timestamp;

void os_tick_init(uint32_t u32_tickPeriod)
{
    os_sysTickHandler_init(u32_tickPeriod);
    // TODO: add check for the return
}

void os_tick_update(void (*updateFn)(void), uint32_t tickPeriod)
{
    os_sysTickHandler_resume();
    updateFn();
    gu32_os_tick_timestamp += tickPeriod;
}

uint32_t os_tick_millis_get(void)
{
    return gu32_os_tick_timestamp;
}
