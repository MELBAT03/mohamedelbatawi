#include "mcal.h"
#include "board.h"

typedef TMRB_INTFactor mcal_timer_interruptFlagsState_t;

typedef struct
{
    uint32_t Mode;             // Select TMRB mode between internal interval timer mode and external event counter
    uint32_t ClkDiv;           // Select the division for TMRB source clock
    uint32_t TrailingTiming;   // Specify the trailingtiming value to be written into TBnRG1
    uint32_t UpCntCtrl;        // Select up-counter work mode between freerun and auto-reload
    uint32_t LeadingTiming;    // Specify the leadingtiming value to be written into TBnRG0
} mcal_timer_timerConfig_t;

typedef struct
{
    uint32_t time1MS;
    uint32_t time1US;
} mcal_timer_timingConfig_t;

typedef enum
{
    TMRB_DIV8_1MS = 0x2710U,
    TMRB_DIV2_1MS = 0x9C40U,
    TMRB_DIV32_1MS = 0x9C4U,
    TMRB_DIV2_1uS = 40
} mcal_timer_prescalerConfigEnum_t;

#define MCAL_TIMER_MS_PRESCALER (32)
#define MCAL_TIMER_US_PRESCALER (2)

mcal_timer_timingConfig_t gx_mcal_timerConfigStruct;

/* fsys = fc = 10MHz * PLL = 80MHz, fphiT0 = fperiph = fgear = fc */
/* ftmrb = 1/32fphiT0 = (1/32)* 80 = 2.5MHz,Ttmrb = 0.4us, 1000us/0.4us = 2500 = 0x9C4 */
/* ftmrb = 1/8fphiT0 = (1/8) * 80 = 10MHz, Ttmrb = 0.1us, 1000us/0.1us = 10000 = 0x2710 */
/* ftmrb = 1/2fphiT0 = (1/2) * 80 = 40MHz, Ttmrb = 0.025us, 1000us/0.025us = 40000 = 0x9C40 */

void mcal_timer_init(void)
{
    gx_mcal_timerConfigStruct.time1MS = (uint32_t)((float)((float)(F_CPU * PLL_FACTOR) / (float)MCAL_TIMER_MS_PRESCALER) / 1000.0f);
    // gx_mcal_timerConfigStruct.time1US = (uint32_t)((float)((float)(F_CPU * PLL_FACTOR) / (float)MCAL_TIMER_US_PRESCALER) / 1000000.0f);
}

// NOTE: max period is 26 msec
void mcal_timer_timerModeMS_init(mcal_timer_t *px_tb, uint32_t u32_timeMS)
{
    mcal_timer_timerConfig_t x_tmrb;
    uint32_t u32_time;

    u32_time = (uint32_t)(gx_mcal_timerConfigStruct.time1MS * u32_timeMS);

    x_tmrb.Mode = TMRB_INTERVAL_TIMER;
    x_tmrb.ClkDiv = TMRB_CLK_DIV_32;
    x_tmrb.TrailingTiming = u32_time; /* periodic time is 1ms */
    x_tmrb.UpCntCtrl = TMRB_AUTO_CLEAR;
    x_tmrb.LeadingTiming = u32_time; /* periodic time is 1ms */

    TMRB_Enable(px_tb);
    TMRB_Init(px_tb, (TMRB_InitTypeDef *)&x_tmrb);
}

void mcal_timer_timerModeUS_init(mcal_timer_t *px_tb, uint32_t u32_timeUS)
{
    mcal_timer_timerConfig_t x_tmrb;
    uint32_t u32_time;
    uint32_t preScaler;
    uint32_t clkDiv;
    uint32_t time1US;

    if (u32_timeUS <= 1638U)
    {
        preScaler = TMRB_CLK_DIV_2;
        clkDiv = 2;
    }
    else if ((u32_timeUS > 1638U) && (u32_timeUS <= 6553U))
    {
        preScaler = TMRB_CLK_DIV_8;
        clkDiv = 8;
    }
    else if (u32_timeUS > 6553U)
    {
        preScaler = TMRB_CLK_DIV_32;
        clkDiv = 32;
    }
    else
    {
        // do nothing
    }

    time1US = (uint32_t)((float)((float)(F_CPU * PLL_FACTOR) / (float)clkDiv) / 1000000.0f);

    x_tmrb.Mode = TMRB_INTERVAL_TIMER;
    u32_time = (uint32_t)(time1US * u32_timeUS);
    x_tmrb.ClkDiv = preScaler;
    x_tmrb.TrailingTiming = u32_time; /* periodic time is 1us */
    x_tmrb.UpCntCtrl = TMRB_AUTO_CLEAR;
    x_tmrb.LeadingTiming = u32_time; /* periodic time is 1us */

    TMRB_Enable(px_tb);
    TMRB_Init(px_tb, (TMRB_InitTypeDef *)&x_tmrb);
}

uint32_t mcal_timer_timerModeUS_conversionFactor_get(mcal_timer_t *px_tb)
{
    uint32_t clkDiv = 2;
    uint32_t tmp = px_tb->MOD & 0x00000003;

    if (tmp == TMRB_CLK_DIV_2)
    {
        clkDiv = 2;
    }
    else if (tmp == TMRB_CLK_DIV_8)
    {
        clkDiv = 8;
    }
    else if (tmp == TMRB_CLK_DIV_32)
    {
        clkDiv = 32;
    }
    else
    {
        // do nothing
    }

    return (uint32_t)((float)((float)(F_CPU * PLL_FACTOR) / (float)clkDiv) / 1000000.0f);
}

void mcal_timer_eventMode_init(mcal_timer_t *px_tb, mcal_gpio_t *px_portConfig, mcal_timer_eventEdgeConfig_t x_edge)
{
    TMRB_InitTypeDef x_tmrb;

    x_tmrb.Mode = TMRB_EVENT_CNT;
    x_tmrb.ClkDiv = 0;
    x_tmrb.TrailingTiming = 0;
    x_tmrb.UpCntCtrl = TMRB_FREE_RUN;
    x_tmrb.LeadingTiming = 0;

    TMRB_Enable(px_tb);
    TMRB_SetRunState(px_tb, TMRB_STOP);

    GPIO_SetInput(px_portConfig->port, px_portConfig->pin);

    if (px_tb == MCAL_TIMER_5 || px_tb == MCAL_TIMER_6)
    {
        mcal_gpio_altFunction_set(px_portConfig, MCAL_GPIO_ALT_FUNC_2);
    }
    else
    {
        mcal_gpio_altFunction_set(px_portConfig, MCAL_GPIO_ALT_FUNC_1);
    }

    TMRB_Init(px_tb, (TMRB_InitTypeDef *)&x_tmrb);
    TMRB_SetExtStartTrg(px_tb, (FunctionalState)DISABLE, x_edge);
    px_tb->FFCR = 0X00000003;
}

void mcal_timer_timerInterruptState_set(mcal_timer_t *px_tb, mcal_timer_intModeEnum_t x_intState)
{
    if (px_tb == MCAL_TIMER_0)
    {
        if (x_intState)
            NVIC_EnableIRQ(INTTB00_IRQn);
        else
            NVIC_DisableIRQ(INTTB00_IRQn);
    }
    else if (px_tb == MCAL_TIMER_1)
    {
        if (x_intState)
            NVIC_EnableIRQ(INTTB10_IRQn);
        else
            NVIC_DisableIRQ(INTTB10_IRQn);
    }
    else if (px_tb == MCAL_TIMER_2)
    {
        if (x_intState)
            NVIC_EnableIRQ(INTTB20_IRQn);
        else
            NVIC_DisableIRQ(INTTB20_IRQn);
    }
    else if (px_tb == MCAL_TIMER_3)
    {
        if (x_intState)
            NVIC_EnableIRQ(INTTB30_IRQn);
        else
            NVIC_DisableIRQ(INTTB30_IRQn);
    }
    else if (px_tb == MCAL_TIMER_4)
    {
        if (x_intState)
            NVIC_EnableIRQ(INTTB40_IRQn);
        else
            NVIC_DisableIRQ(INTTB40_IRQn);
    }
    else if (px_tb == MCAL_TIMER_5)
    {
        if (x_intState)
            NVIC_EnableIRQ(INTTB50_IRQn);
        else
            NVIC_DisableIRQ(INTTB50_IRQn);
    }
    else if (px_tb == MCAL_TIMER_6)
    {
        if (x_intState)
            NVIC_EnableIRQ(INTTB60_IRQn);
        else
            NVIC_DisableIRQ(INTTB60_IRQn);
    }
    else if (px_tb == MCAL_TIMER_7)
    {
        if (x_intState)
            NVIC_EnableIRQ(INTTB70_IRQn);
        else
            NVIC_DisableIRQ(INTTB70_IRQn);
    }
    else
    {
        // do nothing
    }
}

void mcal_timer_timerChannel_enable(mcal_timer_t *px_tb)
{
    TMRB_Enable(px_tb);
}

void mcal_timer_timerChannel_disable(mcal_timer_t *px_tb)
{
    TMRB_Disable(px_tb);
}

void mcal_timer_timerState_set(mcal_timer_t *px_tb, mcal_timer_timerState_t x_state)
{
    TMRB_SetRunState(px_tb, x_state);
}

void mcal_timer_timerFlag_clear(mcal_timer_t *px_tb)
{
    volatile uint32_t u32_val;
    (void)px_tb->ST;
    u32_val = px_tb->ST;
    (void)u32_val;
}

// TODO: check
uint32_t mcal_timer_timerFlag_get(mcal_timer_t *px_tb)
{
    mcal_timer_interruptFlagsState_t x_retVal;
    x_retVal = TMRB_GetINTFactor(px_tb);
    return x_retVal.All;
}

// TODO: check
void mcal_timer_timerCounter_reset(mcal_timer_t *px_tb)
{
    TMRB_SetRunState(px_tb, MCAL_TIMER_STOP);
}

uint32_t mcal_timer_timerCounter_get(mcal_timer_t *px_tb)
{
    uint32_t u32_cntVal;
    u32_cntVal = TMRB_GetUpCntValue(px_tb);
    return u32_cntVal;
}
