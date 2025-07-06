#include "os.h"
#include "board.h"
#include "mcal.h"

void os_init(void)
{
    mcal_cg_init();

#ifdef F_OSC
    if (SystemCoreClock != F_OSC * PLL_FACTOR)
    {
        os_scheduler_fault_set(OS_SCH_SYS_CLOCK_FAULT);
    }
#endif
}
