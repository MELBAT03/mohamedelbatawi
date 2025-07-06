#ifndef HW_TIMER_H
#define HW_TIMER_H

#include "mcal.h"
#include "gpio/gpio.h"

typedef enum
{
    HW_TIMER_MS,
    HW_TIMER_US
} hwTimer_timerModeEnum_t;

typedef mcal_timer_t hwTimer_t;
#define HW_TIMER_0 MCAL_TIMER_0
#define HW_TIMER_1 MCAL_TIMER_1
#define HW_TIMER_2 MCAL_TIMER_2
#define HW_TIMER_3 MCAL_TIMER_3
#define HW_TIMER_4 MCAL_TIMER_4
#define HW_TIMER_5 MCAL_TIMER_5
#define HW_TIMER_6 MCAL_TIMER_6
#define HW_TIMER_7 MCAL_TIMER_7

typedef mcal_timer_timerState_t hwTimer_state_t;
#define HW_TIMER_START MCAL_TIMER_START
#define HW_TIMER_STOP  MCAL_TIMER_STOP

typedef mcal_timer_eventEdgeConfig_t hwTimer_eventEdge_t;
#define HW_TIMER_EDGE_RISING  MCAL_TIMER_EDGE_RISING
#define HW_TIMER_EDGE_FALLING MCAL_TIMER_EDGE_FALLING

typedef mcal_timer_intModeEnum_t hwTimer_intState_t;
#define HW_TIMER_INT_ENABLE  MCAL_TIMER_INT_ENABLE
#define HW_TIMER_INT_DISABLE MCAL_TIMER_INT_DISABLE

void hwTimer_init(void);
void hwTimer_timerMode_set(hwTimer_timerModeEnum_t x_timerMode, hwTimer_t *px_tb, uint32_t u32_timeoutPeriod);
void hwTimer_counterMode_set(hwTimer_t *px_tb, gpio_t *px_portConfig, hwTimer_eventEdge_t x_edge);
uint32_t hwTimer_count_get(hwTimer_t *px_tb);
void hwTimer_count_reset(hwTimer_t *px_tb);
uint32_t hwTimer_flag_get(hwTimer_t *px_tb);
void hwTimer_flag_clear(hwTimer_t *px_tb);
void hwTimer_state_set(hwTimer_t *px_tb, hwTimer_state_t x_state);
void hwTimer_channel_disable(hwTimer_t *px_tb);
void hwTimer_channel_enable(hwTimer_t *px_tb);
void hwTimer_interruptState_set(hwTimer_t *px_tb, hwTimer_intState_t x_intState);

#endif
