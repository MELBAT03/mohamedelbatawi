#include "def.h"
#include "os_tick.h"

static uint32_t gu32_os_timeout_timeoutMSec = 0;
static uint32_t gu32_os_timeout_startingTimeMSec = 0;

void os_timeout_start(uint32_t timeoutMSec)
{
    gu32_os_timeout_timeoutMSec = timeoutMSec;
    gu32_os_timeout_startingTimeMSec = os_tick_millis_get();
}

uint8_t os_timeout_watch(void)
{
    uint8_t ret = 0;

    if ((os_tick_millis_get() - gu32_os_timeout_startingTimeMSec) >= gu32_os_timeout_timeoutMSec)
    {
        ret = 1;
    }

    return ret;
}
