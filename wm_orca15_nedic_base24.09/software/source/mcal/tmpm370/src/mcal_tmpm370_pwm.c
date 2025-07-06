// NOTE:
// when Freq is 10 mega
// minimum freq -> (10mega/65535) = 153HZ   , maximum freq -> 10mega
// maximum time -> 6 milli                  , minimum time -> 0.1 micro

#include "mcal.h"
#include "os.h"

void mcal_pwm_init(void)
{
    // do nothing
}

void mcal_pwm_channel_set(mcal_pwm_t pwmInerface, mcal_pwmConfig_t *x_pwmConfig)
{
    TSB_TB_TypeDef *tmrb[] = {TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7};
    uint32_t u32_periodUs;
    u32_periodUs = (1000000UL / x_pwmConfig->freq);
    unsigned int reg_one = (u32_periodUs * 10);   // after prescaler && 80MHZ
    unsigned long val = ((x_pwmConfig->duty * reg_one) / 100);
    unsigned int value = reg_one - val;

    TMRB_InitTypeDef m_tmrb;
    TMRB_FFOutputTypeDef PPGFFInital;

    GPIO_SetOutput(x_pwmConfig->port, x_pwmConfig->pin);
    if (pwmInerface == MCAL_PWM_5 || pwmInerface == MCAL_PWM_6)
    {
        GPIO_EnableFuncReg(x_pwmConfig->port, GPIO_FUNC_REG_2, x_pwmConfig->pin);
    }
    else
    {
        GPIO_EnableFuncReg(x_pwmConfig->port, GPIO_FUNC_REG_1, x_pwmConfig->pin);
    }

    GPIO_SetPullUp(x_pwmConfig->port, x_pwmConfig->pin, ENABLE);

    m_tmrb.Mode = TMRB_INTERVAL_TIMER;
    m_tmrb.ClkDiv = TMRB_CLK_DIV_8;
    m_tmrb.UpCntCtrl = TMRB_AUTO_CLEAR;
    m_tmrb.TrailingTiming = reg_one;
    m_tmrb.LeadingTiming = value;
    PPGFFInital.FlipflopCtrl = TMRB_FLIPFLOP_CLEAR;
    PPGFFInital.FlipflopReverseTrg = TMRB_FLIPFLOP_MATCH_TRAILINGTIMING | TMRB_FLIPFLOP_MATCH_LEADINGTIMING;

    TMRB_Enable(tmrb[pwmInerface]);
    TMRB_Init(tmrb[pwmInerface], &m_tmrb);
    TMRB_SetFlipFlop(tmrb[pwmInerface], &PPGFFInital);
    TMRB_SetDoubleBuf(tmrb[pwmInerface], ENABLE, TMRB_WRITE_REG_SEPARATE); /* enable double buffer */
}

void mcal_pwm_channelState_set(mcal_pwm_t pwmInerface, mcal_pwm_state_t x_state)
{
    TSB_TB_TypeDef *tmrb[] = {TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7};
    TMRB_SetRunState(tmrb[pwmInerface], x_state);
}

void mcal_pwm_channel_enable(mcal_pwm_t pwmInerface)
{
    TSB_TB_TypeDef *tmrb[] = {TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7};
    TMRB_SetRunState(tmrb[pwmInerface], TMRB_RUN);
}

void mcal_pwm_channel_disable(mcal_pwm_t pwmInerface)
{
    TSB_TB_TypeDef *tmrb[] = {TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7};
    TMRB_SetRunState(tmrb[pwmInerface], TMRB_STOP);
}
