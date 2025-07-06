// test super

#ifndef WL90AT_H
#define WL90AT_H 1

#include "def.h"
#include "components/finput_module/finput_module.h"

typedef struct
{
    uint32_t waterLevel_litter;
    uint32_t waterLevel_freq;
} wl90at_data_t;

typedef struct
{
    finputModule_t channel;
    wl90at_data_t *wl90atBuffer;
    uint8_t size;
    uint32_t upperThreshold;
    uint32_t lowerThreshold;
    uint32_t samplesPerSec;
} wl90at_config_t;

void wl90at_init(wl90at_config_t *wl90at);

void wl90at_update(uint32_t period);

uint32_t wl90at_freq_get(void);
uint8_t wl90at_error_get(void);
uint32_t wl90at_freqForLitter_get(uint32_t litter);

uint8_t wl90at_resetLevel_check(uint32_t acceptedDiff);

void wl90at_freq_set(uint32_t value);
void wl90at_resetLevel_set(void);

#endif
