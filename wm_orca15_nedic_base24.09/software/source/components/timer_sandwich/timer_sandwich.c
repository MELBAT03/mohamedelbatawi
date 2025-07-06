#include "mcal.h"
#include "board.h"

mcal_timer_t *gx_sandwichTimer;

void timerSandwich_init(mcal_timer_t *px_timer)
{
    gx_sandwichTimer = px_timer;
    mcal_timer_init();
}

void timerSandwich_timeMS_set(mcal_timer_t *px_timer, uint32_t u32_timeMS)
{
    if (px_timer == NULL)
    {
        mcal_timer_timerState_set(gx_sandwichTimer, MCAL_TIMER_STOP);
        mcal_timer_timerFlag_clear(gx_sandwichTimer);
        mcal_timer_timerModeMS_init(gx_sandwichTimer, u32_timeMS);
        mcal_timer_timerState_set(gx_sandwichTimer, MCAL_TIMER_START);
    }
    else
    {
        mcal_timer_timerState_set(px_timer, MCAL_TIMER_STOP);
        mcal_timer_timerFlag_clear(px_timer);
        mcal_timer_timerModeMS_init(px_timer, u32_timeMS);
        mcal_timer_timerState_set(px_timer, MCAL_TIMER_START);
    }
}

void timerSandwich_timeMS_start(mcal_timer_t *px_timer)
{
    if (px_timer == NULL)
    {
        mcal_timer_timerState_set(gx_sandwichTimer, MCAL_TIMER_STOP);
        mcal_timer_timerFlag_clear(gx_sandwichTimer);
        mcal_timer_timerState_set(gx_sandwichTimer, MCAL_TIMER_START);
    }
    else
    {
        mcal_timer_timerState_set(px_timer, MCAL_TIMER_STOP);
        mcal_timer_timerFlag_clear(px_timer);
        mcal_timer_timerState_set(px_timer, MCAL_TIMER_START);
    }
}

void timerSandwich_timeUS_set(mcal_timer_t *px_timer, uint32_t u32_timeUS)
{
    if (px_timer == NULL)
    {
        mcal_timer_timerState_set(gx_sandwichTimer, MCAL_TIMER_STOP);
        mcal_timer_timerFlag_clear(gx_sandwichTimer);
        mcal_timer_timerModeUS_init(gx_sandwichTimer, u32_timeUS);
        mcal_timer_timerState_set(gx_sandwichTimer, MCAL_TIMER_START);
    }
    else
    {
        mcal_timer_timerState_set(px_timer, MCAL_TIMER_STOP);
        mcal_timer_timerFlag_clear(px_timer);
        mcal_timer_timerModeUS_init(px_timer, u32_timeUS);
        mcal_timer_timerState_set(px_timer, MCAL_TIMER_START);
    }
}

void timerSandwich_timeUS_start(mcal_timer_t *px_timer)
{
    if (px_timer == NULL)
    {
        mcal_timer_timerState_set(gx_sandwichTimer, MCAL_TIMER_STOP);
        mcal_timer_timerFlag_clear(gx_sandwichTimer);
        mcal_timer_timerState_set(gx_sandwichTimer, MCAL_TIMER_START);
    }
    else
    {
        mcal_timer_timerState_set(px_timer, MCAL_TIMER_STOP);
        mcal_timer_timerFlag_clear(px_timer);
        mcal_timer_timerState_set(px_timer, MCAL_TIMER_START);
    }
}

void timerSandwich_timeout_wait(mcal_timer_t *px_timer)
{
    if (px_timer == NULL)
    {
        for (; mcal_timer_timerFlag_get(gx_sandwichTimer) == 0;)
            ;
    }
    else
    {
        while (mcal_timer_timerFlag_get(px_timer) == 0)
            ;
    }
}
