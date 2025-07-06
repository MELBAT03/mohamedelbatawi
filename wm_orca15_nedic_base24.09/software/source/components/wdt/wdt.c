#include <board.h>
#include "mcal.h"
#include "wdt.h"

void wdt_init(void)
{
    mcal_wdt_config_t config;

    config.output = (mcal_wdt_outputConfig_t)WDT_OUTPUT;
    config.timeout = (mcal_wdt_timeoutConfig_t)WDT_RESET_MSEC;
    mcal_wdt_init(&config);
}

void wdt_update(uint32_t period)
{
    mcal_wdt_feed();
}

void wdt_feed(void)
{
    mcal_wdt_feed();
}

void wdt_enable(void)
{
    mcal_wdt_enable();
}

void wdt_disable(void)
{
    mcal_wdt_disable();
}
