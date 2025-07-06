#ifndef HANDLER_POWER_DETECTION_H
#define HANDLER_POWER_DETECTION_H

#include "mcal.h"
#include "def.h"

#include "components/zcd/zcd.h"

typedef zcd_sate_t handler_powerDetection_sate_t;
#define HANDLER_POWER_DETECTION_NOK ZCD_DETECTION_NOK
#define HANDLER_POWER_DETECTION_OK  ZCD_DETECTION_OK

void handler_powerDetection_init(void);

void handler_powerDetection_edge_set(mcal_exti_detection_type_t edge);

handler_powerDetection_sate_t handler_powerDetection_state_get(void);
void handler_powerDetection_state_set(handler_powerDetection_sate_t value);

uint8_t handler_powerDetection_freq_get(void);

#endif
