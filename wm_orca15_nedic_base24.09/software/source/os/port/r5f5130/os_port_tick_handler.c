#include "def.h"
#include "mcal.h"

void os_sysTickHandler_init(uint32_t tickPeriod)
{
    mcal_sysTick_set(tickPeriod);
}

void os_sysTickHandler_resume(void)
{
    mcal_sysTick_resume();
}