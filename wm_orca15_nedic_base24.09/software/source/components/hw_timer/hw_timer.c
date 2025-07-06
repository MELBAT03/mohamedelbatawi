#include "mcal.h"
#include "gpio/gpio.h"
#include "hw_timer/hw_timer.h"

void hwTimer_init(void)
{
    mcal_timer_init();
}

void hwTimer_timerMode_set(hwTimer_timerModeEnum_t x_timerMode, hwTimer_t *px_tb, uint32_t u32_timeoutPeriod)
{
    switch (x_timerMode)
    {
        case HW_TIMER_MS:
            mcal_timer_timerModeMS_init(px_tb, u32_timeoutPeriod);
            break;

        case HW_TIMER_US:
            mcal_timer_timerModeUS_init(px_tb, u32_timeoutPeriod);
            break;

        default:
            // do nothing
            break;
    }
}

void hwTimer_counterMode_set(hwTimer_t *px_tb, gpio_t *px_portConfig, hwTimer_eventEdge_t x_edge)
{
    mcal_gpio_t gpio;
    gpio.port = px_portConfig->port;
    gpio.pin = px_portConfig->pin;
    gpio.ioState = px_portConfig->ioState;
    gpio.pinState = px_portConfig->state;

    mcal_timer_eventMode_init(px_tb, &gpio, x_edge);
}

uint32_t hwTimer_count_get(hwTimer_t *px_tb)
{
    return mcal_timer_timerCounter_get(px_tb);
}

void hwTimer_count_reset(hwTimer_t *px_tb)
{
    mcal_timer_timerCounter_reset(px_tb);
}

uint32_t hwTimer_flag_get(hwTimer_t *px_tb)
{
    return mcal_timer_timerFlag_get(px_tb);
}

void hwTimer_flag_clear(hwTimer_t *px_tb)
{
    mcal_timer_timerFlag_clear(px_tb);
}

void hwTimer_state_set(hwTimer_t *px_tb, hwTimer_state_t x_state)
{
    mcal_timer_timerState_set(px_tb, x_state);
}

void hwTimer_channel_disable(hwTimer_t *px_tb)
{
    mcal_timer_timerChannel_disable(px_tb);
}

void hwTimer_channel_enable(hwTimer_t *px_tb)
{
    mcal_timer_timerChannel_enable(px_tb);
}

void hwTimer_interruptState_set(hwTimer_t *px_tb, hwTimer_intState_t x_intState)
{
    mcal_timer_timerInterruptState_set(px_tb, x_intState);
}
