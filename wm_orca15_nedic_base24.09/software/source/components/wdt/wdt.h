#ifndef WDT_H
#define WDT_H

#include "mcal.h"

typedef enum
{
    WDT_TIMEOUT_LV0 = MCAL_WDT_TIMEOUT_LV0,
    WDT_TIMEOUT_LV1 = MCAL_WDT_TIMEOUT_LV1,
    WDT_TIMEOUT_LV2 = MCAL_WDT_TIMEOUT_LV2,
    WDT_TIMEOUT_LV3 = MCAL_WDT_TIMEOUT_LV3,
    WDT_TIMEOUT_LV4 = MCAL_WDT_TIMEOUT_LV4,
    WDT_TIMEOUT_LV5 = MCAL_WDT_TIMEOUT_LV5
} wdt_timeoutConfig_t;

typedef enum
{
    WDT_OUTPUT_INT = MCAL_WDT_OUTPUT_INT,
    WDT_OUTPUT_RESET = MCAL_WDT_OUTPUT_RESET
} wdt_outputConfig_t;

// Used to indicate if WDT is running
#define WDT_RUNNING       (0xAAAAAAAAU)
#define WDT_NOT_RUNNING   (0x55555555U)
#define WDT_UNKNOWN_STATE (0x55AA55AAU)

void wdt_init(void);
void wdt_update(uint32_t period);

void wdt_feed(void);

void wdt_enable(void);
void wdt_disable(void);

#endif
