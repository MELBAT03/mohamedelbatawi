#ifndef ZCD_H
#define ZCD_H

#include "def.h"

typedef struct zcd_config
{
    mcal_gpio_t pin;
    mcal_exti_requestPinNumber_t irqPinNumber;
    mcal_exti_irqNumber_t irqReq;
    mcal_exti_detection_type_t edge;
    uint8_t minDetectionCycleCnt;
} zcd_config_t;

typedef enum zcd_sate
{
    ZCD_DETECTION_NOK = 0,
    ZCD_DETECTION_OK = 1
} zcd_sate_t;

void (*zcd_init(zcd_config_t *config))(void);
void zcd_update(uint32_t period);
zcd_sate_t zcd_state_get(void);
uint8_t zcd_freq_get(void);

#endif
