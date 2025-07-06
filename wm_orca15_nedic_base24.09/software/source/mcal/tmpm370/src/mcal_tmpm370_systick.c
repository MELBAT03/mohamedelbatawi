#include "mcal.h"

void mcal_sysTick_init(void)
{
    // do nothing
}

void mcal_sysTick_set(uint32_t u32_tickms)
{
    uint32_t x;
    uint32_t val = 0;
    val = u32_tickms * 80000;
    x = SysTick_Config(val);

    if (x != 0)
    {
        // sys tick init failed
    }
}

void mcal_sysTick_resume(void)
{
}
