#ifndef FINPUT_MODULE_H
#define FINPUT_MODULE_H

#include "components/hw_timer/hw_timer.h"
#include "gpio/gpio.h"
#include "def.h"

typedef struct finputModule_internalHandler *finputModule_handle;

typedef struct
{
    hwTimer_t *timerModule;
    gpio_t gpio;
    hwTimer_eventEdge_t edge;
    uint32_t updatePeriod;
} finputModule_t;

#define FINPUT_MODULE_0 HW_TIMER_0
#define FINPUT_MODULE_1 HW_TIMER_1
#define FINPUT_MODULE_2 HW_TIMER_2
#define FINPUT_MODULE_3 HW_TIMER_3
#define FINPUT_MODULE_4 HW_TIMER_4
#define FINPUT_MODULE_5 HW_TIMER_5
#define FINPUT_MODULE_6 HW_TIMER_6
#define FINPUT_MODULE_7 HW_TIMER_7

#define FINPUT_MODULE_EDGE_RISING  HW_TIMER_EDGE_RISING
#define FINPUT_MODULE_EDGE_FALLING HW_TIMER_EDGE_FALLING

finputModule_handle finputModule_init(finputModule_t *finputModule);

void finputModule_update(uint32_t period);

uint32_t finputModule_value_get(finputModule_handle finputModule);

#endif
