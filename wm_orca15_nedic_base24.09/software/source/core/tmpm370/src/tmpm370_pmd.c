/**
 *******************************************************************************
 * @file    tmpm370_pmd.c
 * @brief   This file provides API functions for PMD driver.
 * @version V2.0.2.2
 * @date    2014/05/20
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm370_pmd.h"

#if defined(__TMPM370_PMD_H)

/** @addtogroup TX03_Periph_Driver
  * @{
  */

/** @defgroup PMD
  * @brief PMD driver modules
  * @{
  */

/** @defgroup PMD_Private_Defines
  * @{
  */

#define PMD_MDEN_PWMEN_SET                  ((uint32_t)0x00000001)
#define PMD_MDEN_PWMEN_CLEAR                ((uint32_t)0x00000000)

#define PMD_CNTSTA_UPDWN_MASK               ((uint32_t)0x00000001)

#define PMD_MDCR_SYNTMD_MASK                ((uint32_t)0xFFFFFFDF)
#define PMD_MDCR_DTYMD_MASK                 ((uint32_t)0xFFFFFFEF)

#define PMD_MDOUT_UOC_MASK                  ((uint32_t)0xFFFFFEFC)
#define PMD_MDOUT_VOC_MASK                  ((uint32_t)0xFFFFFDF3)
#define PMD_MDOUT_WOC_MASK                  ((uint32_t)0xFFFFFBCF)
#define PMD_MDOUT_ALL_CLEAR                 ((uint32_t)0x00000000)

#define PMD_MDPOT_POLH_MASK                 ((uint32_t)0xFFFFFFF7)
#define PMD_MDPOT_POLL_MASK                 ((uint32_t)0xFFFFFFFB)
#define PMD_MDPOT_PSYNCS_MASK               ((uint32_t)0xFFFFFFFC)

#define PMD_EMGCR_EMGEN_SET                 ((uint32_t)0x00000001)
#define PMD_EMGCR_EMGEN_CLEAR               ((uint32_t)0xFFFFFFFE)
#define PMD_EMGREL_DISABLE_COMMAND_1        ((uint32_t)0x0000005A)
#define PMD_EMGREL_DISABLE_COMMAND_2        ((uint32_t)0x000000A5)
#define PMD_EMGCR_EMGCNT_MASK               ((uint32_t)0xFFFFF0FF)
#define PMD_EMGCR_INHEN_MASK                ((uint32_t)0xFFFFFFDF)
#define PMD_EMGCR_EMGMD_MASK                ((uint32_t)0xFFFFFFE7)
#define PMD_EMGCR_EMGISEL_MASK              ((uint32_t)0xFFFFFFFB)
#define PMD_EMGCR_EMGRS_SET                 ((uint32_t)0x00000002)

#define PMD_EMGSTA_EMGI_MASK                ((uint32_t)0x00000002)
#define PMD_EMGSTA_EMGST_MASK               ((uint32_t)0x00000001)

#define PMD_TRGCR_TRG0MD_MASK               ((uint32_t)0xFFFFFFF8)
#define PMD_TRGCR_TRG1MD_MASK               ((uint32_t)0xFFFFFF8F)
#define PMD_TRGCR_TRG0BE_MASK               ((uint32_t)0xFFFFFFF7)
#define PMD_TRGCR_TRG1BE_MASK               ((uint32_t)0xFFFFFF7F)

#define PMD_TRGCR_TRG2MD_MASK               ((uint32_t)0xFFFFF8FF)
#define PMD_TRGCR_TRG3MD_MASK               ((uint32_t)0xFFFF8FFF)
#define PMD_TRGCR_TRG2BE_MASK               ((uint32_t)0xFFFFF7FF)
#define PMD_TRGCR_TRG3BE_MASK               ((uint32_t)0xFFFF7FFF)

#define PMD_TRGMD_EMGTGE_SET                ((uint32_t)0x00000001)
#define PMD_TRGMD_EMGTGE_CLEAR              ((uint32_t)0xFFFFFFFE)

#define PMD_TRGMD_TRGOUT_MASK               ((uint32_t)0xFFFFFFFD)
#define PMD_TRGSEL_TRGSEL_MASK              ((uint32_t)0xFFFFFFF8)

#define PMD_OVVCR_OVVEN_SET                 ((uint32_t)0x00000001)
#define PMD_OVVCR_OVVEN_CLEAR               ((uint32_t)0xFFFFFFFE)

#define PMD_OVVCR_OVVCNT_MASK               ((uint32_t)0xFFFFF0FF)
#define PMD_OVVCR_OVVMD_MASK                ((uint32_t)0xFFFFFFE7)
#define PMD_OVVCR_OVVISEL_MASK              ((uint32_t)0xFFFFFFFB)

#define PMD_OVVCR_ADIN0EN_SET               ((uint32_t)0x00000020)
#define PMD_OVVCR_ADIN0EN_CLEAR             ((uint32_t)0xFFFFFFDF)
#define PMD_OVVCR_ADIN1EN_SET               ((uint32_t)0x00000040)
#define PMD_OVVCR_ADIN1EN_CLEAR             ((uint32_t)0xFFFFFFBF)

#define PMD_OVVCR_OVVRS_SET                 ((uint32_t)0x00000002)
#define PMD_OVVCR_OVVRS_CLEAR               ((uint32_t)0xFFFFFFFD)

#define PMD_OVVSTA_OVVI_MASK                ((uint32_t)0x00000002)
#define PMD_OVVSTA_OVVST_MASK               ((uint32_t)0x00000001)

/*mabrouk edit pmd */
#define PMD_ENABLE_EMG_AND_RELEASE          ((uint32_t)0x00000F3B)
static uint8_t t_PMDPortOutput[2][8] = {
    /*L_L    L_H    H_L    H_H  IPWM_PWM IPWM_H  H_IPWM/PWM  PWM_IPWM *//*POLH POLL  = 00b */
    {0x03U, 0x02U, 0x01U, 0x00U, 0x03U, 0x02U, 0x01U, 0x00U},
    /*L_L    L_H    H_L    H_H  IPWM_PWM L_PWM/IPWM  PWM_L PWM_IPWM *//*POLH POLL  = 11b */
    {0x00U, 0x01U, 0x02U, 0x03U, 0x00U, 0x01U, 0x02U, 0x03U}
};
uint32_t t_MDOUTMask[4] = { PMD_MDOUT_UOC_MASK, PMD_MDOUT_VOC_MASK, PMD_MDOUT_WOC_MASK, 0U };


/** @} */
/* End of group PMD_Private_Defines */


/** @defgroup PMD_Private_FunctionPrototypes
  * @{
  */

/** @} */
/* End of group PMD_Private_FunctionPrototypes */

/** @defgroup PMD_Private_Functions
  * @{
  */

/** @} */
/* End of group group PMD_Private_Functions */

/** @defgroup PMD_Exported_Functions
  * @{
  */

/**
  * @brief  Enable the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @retval None.
  */
void PMD_Enable(TSB_PMD_TypeDef * PMDx)
{
    /* Check the parameter */
    assert_param(IS_PMD_PERIPH(PMDx));
    /* Set PMDxMDEN <PWMEN> to enable PMD operation */
    PMDx->MDEN = PMD_MDEN_PWMEN_SET;
}

/**
  * @brief  Disable the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @retval None.
  */
void PMD_Disable(TSB_PMD_TypeDef * PMDx)
{
    /* Check the parameter */
    assert_param(IS_PMD_PERIPH(PMDx));
    /* Clear PMDxMDEN <PWMEN> to disable PMD operation */
    PMDx->MDEN = PMD_MDEN_PWMEN_CLEAR;
}

/**
  * @brief  Set PMD port control of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  PortMode: The port output mode of PMD.
  *   This parameter can be one of the following values:
  *   PMD_PORT_MODE_0, PMD_PORT_MODE_1, PMD_PORT_MODE_2, PMD_PORT_MODE_3.
  * @retval None.
  */
void PMD_SetPortControl(TSB_PMD_TypeDef * PMDx, uint32_t PortMode)
{
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_PORT_MODE(PortMode));
    /* Set PMDxPORTMD <PORTMD> to control PMD port mode */
    PMDx->PORTMD = PortMode;
}

/**
  * @brief  Initialize the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  InitStruct: The structure containing basic PMD configuration.
  * @retval None.
  */
void PMD_Init(TSB_PMD_TypeDef * PMDx, PMD_InitTypeDef * InitStruct)
{
    uint32_t tmp = 0U;
    tmp = PMDx->MDCR & (~PMD_MDCR_SYNTMD_MASK);
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(InitStruct));
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_CYCLE_MODE(InitStruct->CycleMode));
    assert_param(IS_PMD_DUTY_MODE(InitStruct->DutyMode));
    assert_param(IS_PMD_CARRIER_WAVE_MODE(InitStruct->CarrierMode));
    assert_param(IS_PMD_VALUE(InitStruct->CycleTiming));
    /* Set PMDxMDCR <PWMMD> to specify PWM carrier wave */
    tmp |= InitStruct->CarrierMode;
    /* Judge the PWM carrier wave */
    if (InitStruct->CarrierMode == PMD_CARRIER_WAVE_MODE_1) {
        /* Check the parameters */
        assert_param(IS_PMD_PWM_INT_TIMING(InitStruct->IntTiming));
        assert_param(IS_PMD_PWM_INT_CYCLE_MODE_1(InitStruct->IntCycle));
        /* Set PMDxMDCR <PINT> to choose PWM interrupt timing when PWM mode 1 (triangle wave) is set */
        tmp |= (InitStruct->IntTiming << 3U);
    } else {
        /* Check the parameter */
        assert_param(IS_PMD_PWM_INT_CYCLE_MODE_0(InitStruct->IntCycle));
    }
    /* Set PMDxMDCR <PWMCK> to specify PWM cycle extension mode */
    tmp |= (InitStruct->CycleMode << 6U);
    /* Set PMDxMDCR <DTYMD> to choose DUTY mode */
    tmp |= (InitStruct->DutyMode << 4U);
    /* Set PMDxMDCR <INTPRD> to choose PWM interrupt cycle */
    tmp |= (InitStruct->IntCycle << 1U);
    PMDx->MDCR = tmp;
    /* Specify PMDxMDPRD <MDPRD> to set PWM cycle */
    PMDx->MDPRD = InitStruct->CycleTiming;
        /*mabrouk edit pmd 1*/
    /*Set the select mode of the specified PMD channel*/
    PMDx->MODESEL = InitStruct->ModeSelectBus_VE;
}

/**
  * @brief  Change the PWM cycle of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  CycleTiming: PWM cycle, from 0x0000 to 0xFFFF
  * @retval None.
  */
void PMD_ChangePWMCycle(TSB_PMD_TypeDef * PMDx, uint32_t CycleTiming)
{
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_VALUE(CycleTiming));
    /* Specify PMDxMDPRD <MDPRD> to set PWM cycle */
    PMDx->MDPRD = CycleTiming;
}

/**
  * @brief  Get the PWM counter flag of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @retval The PWM counter flag.
  *   The value returned can be one of the following values:
  *   PMD_COUNTER_UP, PMD_COUNTER_DOWN.
  */
uint32_t PMD_GetCntFlag(TSB_PMD_TypeDef * PMDx)
{
    uint32_t tmp = 0U;
    /* Check the parameter */
    assert_param(IS_PMD_PERIPH(PMDx));
    /* Read PMDxCNTSTA <UPDWN> to get the PWM counter flag */
    tmp = PMDx->CNTSTA & PMD_CNTSTA_UPDWN_MASK;
    return tmp;
}

/**
  * @brief  Get the count value of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @retval Count value of the specified PMD channel.
  */
uint16_t PMD_GetCntValue(TSB_PMD_TypeDef * PMDx)
{
    uint16_t tmp = 0U;
    /* Check the parameter */
    assert_param(IS_PMD_PERIPH(PMDx));
    /* Read PMDxMDCNT <MDCNT> to get the count value */
    tmp = (uint16_t) PMDx->MDCNT;
    return tmp;
}

/**
  * @brief  Set the compare value of the specified phase of the specified PMD channel. 
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  PMDPhase: Select the phase of PMD channel.
  *   This parameter can be one of the following values:
  *   PMD_PHASE_U, PMD_PHASE_V, PMD_PHASE_W, PMD_PHASE_ALL.
  * @param  Timing: Compare value, from 0x0000 to 0xFFFF.
  * @retval None.
  */
void PMD_SetCompareValue(TSB_PMD_TypeDef * PMDx, uint32_t PMDPhase, uint32_t Timing)
{
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_PHASE(PMDPhase));
    assert_param(IS_PMD_VALUE(Timing));
    /* Judge the phase */
    switch (PMDPhase) {
    case PMD_PHASE_U:
        /* Set PMDxCMPU <CMPU> to specify compare value in phase U */
        PMDx->CMPU = Timing;
        break;
    case PMD_PHASE_V:
        /* Set PMDxCMPV <CMPV> to specify compare value in phase V */
        PMDx->CMPV = Timing;
        break;
    case PMD_PHASE_W:
        /* Set PMDxCMPW <CMPW> to specify compare value in phase W */
        PMDx->CMPW = Timing;
        break;
    case PMD_PHASE_ALL:
        /* Set PMDxCMPU <CMPU> to specify compare value in phase U */
        PMDx->CMPU = Timing;
        /* Set PMDxCMPV <CMPV> to specify compare value in phase V */
        PMDx->CMPV = Timing;
        /* Set PMDxCMPW <CMPW> to specify compare value in phase W */
        PMDx->CMPW = Timing;
        break;
    default:
        /* Do nothing */
        break;
    }
}

/**
  * @brief  Set the mode of port output of the specified PMD channel. 
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  Mode: The mode of port output.
  *   This parameter can be one of the following values:
  *   PMD_PORT_OUTPUT_MODE_0, PMD_PORT_OUTPUT_MODE_1.
  * @retval None.
  */
void PMD_SetPortOutputMode(TSB_PMD_TypeDef * PMDx, uint32_t Mode)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_PORT_OUTPUT_MODE(Mode));
    /* Set PMDxMDCR <SYNTMD> to choose port output mode */
    tmp = PMDx->MDCR & PMD_MDCR_SYNTMD_MASK;
    tmp |= (Mode << 5U);
    PMDx->MDCR = tmp;
}

/**
  * @brief  Set polarity of the specified output port phase of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  OutputPhase: Select the specified output port phase.
  *   This parameter can be one of the following values:
  *   PMD_OUTPUT_PHASE_LOWER, PMD_OUTPUT_PHASE_UPPER.
  * @param  Polarity: Select the polarity.
  *   This parameter can be one of the following values:
  *   PMD_POLARITY_LOW, PMD_POLARITY_HIGH.
  * @retval None.
  */
void PMD_SetOutputPhasePolarity(TSB_PMD_TypeDef * PMDx, uint32_t OutputPhase, uint32_t Polarity)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_OUTPUT_PHASE(OutputPhase));
    assert_param(IS_PMD_POLARITY(Polarity));
    /* Judge the output port phase */
    if (OutputPhase == PMD_OUTPUT_PHASE_UPPER) {
        /* Specify PMDxMDPOT <POLH> to select the polarity */
        tmp = PMDx->MDPOT & PMD_MDPOT_POLH_MASK;
        tmp |= (Polarity << 3U);
        PMDx->MDPOT = tmp;
    } else {
        /* Specify PMDxMDPOT <POLL> to select the polarity */
        tmp = PMDx->MDPOT & PMD_MDPOT_POLL_MASK;
        tmp |= (Polarity << 2U);
        PMDx->MDPOT = tmp;
    }
}

/**
  * @brief  Choose the timing when port outputs of U-, V- and W- phase output setting is reflected of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  ReflectedTime: Select the reflected time.
  *   This parameter can be one of the following values:
  *   PMD_REFLECTED_TIME_WRITE, PMD_REFLECTED_TIME_MIN,
  *   PMD_REFLECTED_TIME_MAX, PMD_REFLECTED_TIME_MIN_MAX.
  * @retval None.
  */
void PMD_SetReflectTime(TSB_PMD_TypeDef * PMDx, uint32_t ReflectedTime)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_REFLECTED_TIME(ReflectedTime));
    /* Specify PMDxMDPOT <PSYNCS> to select the reflected time */
    tmp = PMDx->MDPOT & PMD_MDPOT_PSYNCS_MASK;
    tmp |= ReflectedTime;
    PMDx->MDPOT = tmp;
}

/**
  * @brief  Enable EMG protection of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @retval None.
  */
void PMD_EnableEMG(TSB_PMD_TypeDef * PMDx)
{
    /* Check the parameter */
    assert_param(IS_PMD_PERIPH(PMDx));
    /* Set PMDxEMGCR <EMGEN> to enable EMG protection */
    PMDx->EMGCR |= PMD_EMGCR_EMGEN_SET;
}

/**
  * @brief  Disable EMG protection of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @retval None.
  */
void PMD_DisableEMG(TSB_PMD_TypeDef * PMDx)
{
    /* Check the parameter */
    assert_param(IS_PMD_PERIPH(PMDx));
    /* To prohibit EMG function, three instructions must be executed consecutively */
    /* Step1: Set "0x5A" to PMDxEMGREL <EMGREL> */
    PMDx->EMGREL = PMD_EMGREL_DISABLE_COMMAND_1;
    /* Step2: Set "0xA5" to PMDxEMGREL <EMGREL> */
    PMDx->EMGREL = PMD_EMGREL_DISABLE_COMMAND_2;
    /* Step3: Set "0" to PMDxEMGCR <EMGEN> */
    PMDx->EMGCR &= PMD_EMGCR_EMGEN_CLEAR;
}

/**
  * @brief  Set the noise elimination time for abnormal condition detection input of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  NoiseElimination: Select the noise elimination time.
  *   This parameter can be one of the following values:
  *   PMD_NOISE_ELIMINATION_NONE, PMD_NOISE_ELIMINATION_16, PMD_NOISE_ELIMINATION_32,
  *   PMD_NOISE_ELIMINATION_48, PMD_NOISE_ELIMINATION_64, PMD_NOISE_ELIMINATION_80,
  *   PMD_NOISE_ELIMINATION_96, PMD_NOISE_ELIMINATION_112, PMD_NOISE_ELIMINATION_128,
  *   PMD_NOISE_ELIMINATION_144, PMD_NOISE_ELIMINATION_160, PMD_NOISE_ELIMINATION_176,
  *   PMD_NOISE_ELIMINATION_192, PMD_NOISE_ELIMINATION_208, PMD_NOISE_ELIMINATION_224,
  *   PMD_NOISE_ELIMINATION_240.
  * @retval None.
  */
void PMD_SetEMGNoiseElimination(TSB_PMD_TypeDef * PMDx, uint32_t NoiseElimination)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_NOISE_ELIMINATION(NoiseElimination));
    /* Set PMDxEMGCR <EMGCNT> to select the noise elimination time */
    tmp = PMDx->EMGCR & PMD_EMGCR_EMGCNT_MASK;
    tmp |= (NoiseElimination << 8U);
    PMDx->EMGCR = tmp;
}

/**
  * @brief  Choose PMD output status at tool break of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  Status: PMD output status at tool break.
  *   This parameter can be one of the following values:
  *   PMD_BREAK_STATUS_PMD, PMD_BREAK_STATUS_HIGH_IMPEDANCE.
  * @retval None.
  */
void PMD_SetToolBreakOutput(TSB_PMD_TypeDef * PMDx, uint32_t Status)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_BREAK_STATUS(Status));
    /* Set PMDxEMGCR <INHEN> to select output status */
    tmp = PMDx->EMGCR & PMD_EMGCR_INHEN_MASK;
    tmp |= (Status << 5U);
    PMDx->EMGCR = tmp;
}

/**
  * @brief  Set EMG protection mode of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  Mode: EMG protection mode.
  *   This parameter can be one of the following values:
  *   PMD_EMG_MODE_0, PMD_EMG_MODE_1, PMD_EMG_MODE_2, PMD_EMG_MODE_3.
  * @retval None.
  */
void PMD_SetEMGMode(TSB_PMD_TypeDef * PMDx, uint32_t Mode)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_EMG_MODE(Mode));
    /* Set PMDxEMGCR <EMGMD> to select EMG protection mode */
    tmp = PMDx->EMGCR & PMD_EMGCR_EMGMD_MASK;
    tmp |= (Mode << 3U);
    PMDx->EMGCR = tmp;
}

/**
  * @brief  Release EMG protection status of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @retval None.
  */
void PMD_EMGRelease(TSB_PMD_TypeDef * PMDx)
{
    /* Check the parameter */
    assert_param(IS_PMD_PERIPH(PMDx));
    /* There are two steps to release EMG protection */
    /* Step1: Set PMDxMDOUT<WPWM><VPWM><UPWM><WOC><VOC><UOC> to 0 */
    PMDx->MDOUT = PMD_MDOUT_ALL_CLEAR;
    /* Step2: Set PMDxEMGCR <EMGRS> to release EMG protection */
    PMDx->EMGCR |= PMD_EMGCR_EMGRS_SET;
}

/**
  * @brief  Get the level of abnormal condition EMG input of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @retval The level of abnormal condition input.
  *   The value returned can be one of the following values:
  *   PMD_ABNORMAL_LEVEL_L, PMD_ABNORMAL_LEVEL_H.
  */
uint32_t PMD_GetEMGAbnormalLevel(TSB_PMD_TypeDef * PMDx)
{
    uint32_t tmp = 0U;
    /* Check the parameter */
    assert_param(IS_PMD_PERIPH(PMDx));
    /* Read PMDxEMGSTA <EMGI> to get the level of abnormal condition input */
    tmp = PMDx->EMGSTA & PMD_EMGSTA_EMGI_MASK;
    return tmp;
}

/**
  * @brief  Get the EMG protection condition of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @retval The EMG protection condition.
  *   The value returned can be 0 or 1.
  *   0 means normal operation.
  *   1 means during in EMG protection.
  */
uint32_t PMD_GetEMGCondition(TSB_PMD_TypeDef * PMDx)
{
    uint32_t tmp = 0U;
    /* Check the parameter */
    assert_param(IS_PMD_PERIPH(PMDx));
    /* Read PMDxEMGSTA <EMGST> to get the EMG protection condition */
    tmp = PMDx->EMGSTA & PMD_EMGSTA_EMGST_MASK;
    return tmp;
}

/**
  * @brief  Set dead time of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  Time: Dead time, from 0x00 to 0xFF.
  * @retval None.
  */
void PMD_SetDeadTime(TSB_PMD_TypeDef * PMDx, uint32_t Time)
{
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_DEAD_TIME(Time));
    /* Set PMDxDTR <DTR> to select dead time */
    PMDx->DTR = Time;
}

/**
  * @brief  Set the compare values of the all phases of the specified PMD channel. 
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  UPhaseTiming: Compare value of phase U, from 0x0000 to 0xFFFF.
  * @param  VPhaseTiming: Compare value of phase V, from 0x0000 to 0xFFFF.
  * @param  WPhaseTiming: Compare value of phase W, from 0x0000 to 0xFFFF.
  * @retval None.
  */
void PMD_SetAllPhaseCompareValue(TSB_PMD_TypeDef * PMDx, uint32_t UPhaseTiming,
                                 uint32_t VPhaseTiming, uint32_t WPhaseTiming)
{
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_VALUE(UPhaseTiming));
    assert_param(IS_PMD_VALUE(VPhaseTiming));
    assert_param(IS_PMD_VALUE(WPhaseTiming));
    /*mabrouk edit pmd switchv/w*/
    /* Set PMDxCMPU <CMPU> to specify compare value in phase U */
    PMDx->CMPU = UPhaseTiming;
    /* Set PMDxCMPV <CMPV> to specify compare value in phase w */
    PMDx->CMPV = WPhaseTiming;
    /* Set PMDxCMPW <CMPW> to specify compare value in phase v */
    PMDx->CMPW = VPhaseTiming;
}

/**
  * @brief  Change duty mode of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  DutyMode: The duty mode of PMD.
  *   This parameter can be one of the following values:
  *   PMD_DUTY_MODE_U_PHASE, PMD_DUTY_MODE_3_PHASE.
  * @retval None.
  */
void PMD_ChangeDutyMode(TSB_PMD_TypeDef * PMDx, uint32_t DutyMode)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_DUTY_MODE(DutyMode));
    /* Change PMDxMDCR <DTYMD> to choose DUTY mode */
    tmp = PMDx->MDCR & PMD_MDCR_DTYMD_MASK;
    tmp |= (DutyMode << 4U);
    PMDx->MDCR = tmp;
}

/**
  * @brief  Set the specified output of the specified phase of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  PMDPhase: Select the phase of PMD channel.
  *   This parameter can be one of the following values:
  *   PMD_PHASE_U, PMD_PHASE_V, PMD_PHASE_W, PMD_PHASE_ALL.
  * @param  Output: Select the output.
  *   This parameter can be one of the following values:
  *   0: PMD_OUTPUT_L_L,  1: PMD_OUTPUT_L_H,        2: PMD_OUTPUT_H_L,
  *   3: PMD_OUTPUT_H_H,  4: PMD_OUTPUT_PWM_IPWM,
  *   5: PMD_OUTPUT_L_PWM/PMD_OUTPUT_L_IPWM/PMD_OUTPUT_IPWM_H,
  *   6: PMD_OUTPUT_PWM_L/PMD_OUTPUT_H_IPWM/PMD_OUTPUT_H_PWM,
  *   7: PMD_OUTPUT_PWM_IPWM.
  * @retval  Success or not
  *   The value returned can be one of the following values:
  *   SUCCESS or ERROR
  */
Result PMD_SetPortOutput(TSB_PMD_TypeDef * PMDx, uint32_t PMDPhase, uint8_t Output)
{
    Result retval = ERROR;
    uint8_t fPOLHL = 0U;
    uint32_t value_PWM = 0U;
    uint32_t value_OC = 0U;
    uint32_t reg = 0U;

    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_PHASE(PMDPhase));
    assert_param(IS_PMD_OUTPUT(Output));

    /* Get the value of PMDxMDPOT <POLH> and PMDxMDPOT <POLL> */
    fPOLHL = (uint8_t) (PMDx->MDPOT >> 2U);
    fPOLHL &= 0x03U;

    value_PWM = (uint32_t) Output / 4U;
    reg = PMDx->MDOUT & t_MDOUTMask[PMDPhase];

    if ((fPOLHL == 3U) || (fPOLHL == 0U)) {
        fPOLHL >>= 1U;
        value_OC = (uint32_t) t_PMDPortOutput[fPOLHL][Output];
        value_PWM <<= 8U;
        if (PMDPhase == PMD_PHASE_ALL) {
            reg = value_OC | (value_OC << 2U) | (value_OC << 4U) |
                value_PWM | (value_PWM << 1U) | (value_PWM << 2U);
        } else {
            value_PWM <<= PMDPhase;
            PMDPhase <<= 1U;
            reg |= (value_PWM | (value_OC << PMDPhase));
        }
        PMDx->MDOUT = reg;
        retval = SUCCESS;
    } else {
        /* Do nothing */
    }
    return retval;
}

/**
  * @brief  Set the ADC trigger compare registers' value of the specified PMD channel. 
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  TRGCMP0Timing: Value of ADC trigger compare register 0, from 0x0001 to [MDPRD set value 每 1].
  * @param  TRGCMP1Timing: Value of ADC trigger compare register 1, from 0x0001 to [MDPRD set value 每 1].
  * @param  TRGCMP2Timing: Value of ADC trigger compare register 2, from 0x0001 to [MDPRD set value 每 1].
  * @param  TRGCMP3Timing: Value of ADC trigger compare register 3, from 0x0001 to [MDPRD set value 每 1].
  * @retval None.
  * @note PMDxTRGCMPn should be set in a range of 1 to [MDPRD set value 每 1].
  */
void PMD_SetTrgCmpValue(TSB_PMD_TypeDef * PMDx, uint32_t TRGCMP0Timing, uint32_t TRGCMP1Timing,
                        uint32_t TRGCMP2Timing, uint32_t TRGCMP3Timing)
{
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_VALUE(TRGCMP0Timing));
    assert_param(IS_PMD_VALUE(TRGCMP1Timing));
    assert_param(IS_PMD_VALUE(TRGCMP2Timing));
    assert_param(IS_PMD_VALUE(TRGCMP3Timing));

    /* Set PMDxTRGCMP0 <TRGCMP0> */
    PMDx->TRGCMP0 = TRGCMP0Timing;
    /* Set PMDxTRGCMP1 <TRGCMP1> */
    PMDx->TRGCMP1 = TRGCMP1Timing;
    /* Set PMDxTRGCMP2 <TRGCMP2> */
    PMDx->TRGCMP2 = TRGCMP2Timing;
    /* Set PMDxTRGCMP3 <TRGCMP3> */
    PMDx->TRGCMP3 = TRGCMP3Timing;
}

/**
  * @brief  Set trigger mode of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  PMDTrg: Select the PMD Trigger.
  *   This parameter can be one of the following values:
  *   PMD_ADC_TRG_0, PMD_ADC_TRG_1, PMD_ADC_TRG_2, PMD_ADC_TRG_3.
  * @param  Mode: PMD trigger mode.
  *   This parameter can be one of the following values:
  *   PMD_TRG_MODE_0, PMD_TRG_MODE_1, PMD_TRG_MODE_2, PMD_TRG_MODE_3,
  *   PMD_TRG_MODE_4, PMD_TRG_MODE_5, PMD_TRG_MODE_6, PMD_TRG_MODE_7.
  * @retval None.
  */
void PMD_SetTrgMode(TSB_PMD_TypeDef * PMDx, uint32_t PMDTrg, uint32_t Mode)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_ADC_TRG(PMDTrg));
    assert_param(IS_PMD_TRG_MODE(Mode));

    /* Judge the trigger */
    switch (PMDTrg) {
    case PMD_ADC_TRG_0:
        /* Set PMDxTRGCR <TRG0MD> to select trigger 0 mode */
        tmp = PMDx->TRGCR & PMD_TRGCR_TRG0MD_MASK;
        tmp |= (Mode << 0U);
        PMDx->TRGCR = tmp;
        break;
    case PMD_ADC_TRG_1:
        /* Set PMDxTRGCR <TRG1MD> to select trigger 1 mode */
        tmp = PMDx->TRGCR & PMD_TRGCR_TRG1MD_MASK;
        tmp |= (Mode << 4U);
        PMDx->TRGCR = tmp;
        break;
    case PMD_ADC_TRG_2:
        /* Set PMDxTRGCR <TRG2MD> to select trigger 2 mode */
        tmp = PMDx->TRGCR & PMD_TRGCR_TRG2MD_MASK;
        tmp |= (Mode << 8U);
        PMDx->TRGCR = tmp;
        break;
    case PMD_ADC_TRG_3:
        /* Set PMDxTRGCR <TRG3MD> to select trigger 3 mode */
        tmp = PMDx->TRGCR & PMD_TRGCR_TRG3MD_MASK;
        tmp |= (Mode << 12U);
        PMDx->TRGCR = tmp;
        break;
    default:
        /* Do nothing */
        break;
    }
}

/**
  * @brief  Set trigger buffer update timing of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  PMDTrg: Select the PMD Trigger.
  *   This parameter can be one of the following values:
  *   PMD_ADC_TRG_0, PMD_ADC_TRG_1, PMD_ADC_TRG_2, PMD_ADC_TRG_3.
  * @param  UpdateTiming: PMDTRG0 to PMDTRG1 buffer update timing.
  *   This parameter can be one of the following values:
  *   PMD_TRG_UPDATE_SYNC, PMD_TRG_UPDATE_ASYNC.
  * @retval None.
  */
void PMD_SetTrgUpdate(TSB_PMD_TypeDef * PMDx, uint32_t PMDTrg, uint32_t UpdateTiming)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_ADC_TRG(PMDTrg));
    assert_param(IS_PMD_TRG_TIMING(UpdateTiming));

    /* Judge the trigger */
    switch (PMDTrg) {
    case PMD_ADC_TRG_0:
        /* Set PMDxTRGCR <TRG0BE> to select trigger 0 buffer update timing */
        tmp = PMDx->TRGCR & PMD_TRGCR_TRG0BE_MASK;
        tmp |= (UpdateTiming << 3U);
        PMDx->TRGCR = tmp;
        break;
    case PMD_ADC_TRG_1:
        /* Set PMDxTRGCR <TRG1BE> to select trigger 1 buffer update timing */
        tmp = PMDx->TRGCR & PMD_TRGCR_TRG1BE_MASK;
        tmp |= (UpdateTiming << 7U);
        PMDx->TRGCR = tmp;
        break;
    case PMD_ADC_TRG_2:
        /* Set PMDxTRGCR <TRG2BE> to select trigger 2 buffer update timing */
        tmp = PMDx->TRGCR & PMD_TRGCR_TRG2BE_MASK;
        tmp |= (UpdateTiming << 11U);
        PMDx->TRGCR = tmp;
        break;
    case PMD_ADC_TRG_3:
        /* Set PMDxTRGCR <TRG3BE> to select trigger 3 buffer update timing */
        tmp = PMDx->TRGCR & PMD_TRGCR_TRG3BE_MASK;
        tmp |= (UpdateTiming << 15U);
        PMDx->TRGCR = tmp;
        break;
    default:
        /* Do nothing */
        break;
    }
}

/**
  * @brief  Enable or disable trigger output in EMG protection state of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  NewState: Output enable in EMG protection state.
  *   This parameter can be one of the following values:
  *   ENABLE, DIABLE.
  * @retval None.
  */
void PMD_SetEMGTrg(TSB_PMD_TypeDef * PMDx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE) {
        PMDx->TRGMD |= PMD_TRGMD_EMGTGE_SET;
    } else {
        PMDx->TRGMD &= PMD_TRGMD_EMGTGE_CLEAR;
    }
}

/**
  * @brief  Set trigger output of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  TrgMode: Select the trigger output mode.
  *   This parameter can be one of the following values:
  *   PMD_TRG_FIXED_OUTPUT, PMD_TRG_VARIABLE_OUTPUT.
  * @param  TrgChannel: Trigger output select.
  *   This parameter can be one of the following values:
  *   when TrgMode == PMD_TRG_FIXED_OUTPUT:
  *   PMD_TRG_OUTPUT_0, PMD_TRG_OUTPUT_1, PMD_TRG_OUTPUT_2, PMD_TRG_OUTPUT_3.
  *   when TrgMode == PMD_TRG_VARIABLE_OUTPUT:
  *   PMD_TRG_OUTPUT_0, PMD_TRG_OUTPUT_1, PMD_TRG_OUTPUT_2, 
  *   PMD_TRG_OUTPUT_3, PMD_TRG_OUTPUT_4, PMD_TRG_OUTPUT_5.
  * @retval None.
  */
void PMD_SetTrgOutput(TSB_PMD_TypeDef * PMDx, uint32_t TrgMode, uint32_t TrgChannel)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_TRG_OUTPUT_MODE(TrgMode));
    /* Judge the trigger output mode */
    if (TrgMode == PMD_TRG_FIXED_OUTPUT) {
        /* Check the parameters */
        assert_param(IS_PMD_TRG_FIXED_OUTPUT(TrgChannel));
    } else {
        /* Check the parameter */
        assert_param(IS_PMD_TRG_OUTPUT(TrgChannel));
    }

    /* Set PMDxTRGMD <TRGOUT> to select trigger output mode */
    tmp = PMDx->TRGMD & PMD_TRGMD_TRGOUT_MASK;
    tmp |= (TrgMode << 1U);
    PMDx->TRGMD = tmp;

    /* Judge the trigger output mode */
    if (TrgMode == PMD_TRG_VARIABLE_OUTPUT) {
        /* Set PMDxTRGSEL <TRGSEL> to select trigger output channel */
        tmp = PMDx->TRGSEL & PMD_TRGSEL_TRGSEL_MASK;
        tmp |= TrgChannel;
        PMDx->TRGSEL = tmp;
    } else {
        /* Do nothing */
    }
}

/**
  * @brief  Set the select mode of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  Mode: The select mode of PMD.
  *   This parameter can be one of the following values:
  *   PMD_BUS_MODE, PMD_VE_MODE.
  * @retval None.
  */
void PMD_SetSelectMode(TSB_PMD_TypeDef * PMDx, uint32_t Mode)
{
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_SELECT_MODE(Mode));
    /* Set PMDxMODESEL <MDSEL> to select mode for loading the second buffer value of each double-buffered register */
    PMDx->MODESEL = Mode;
}

/**
  * @brief  Set EMG input source of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  Source: EMG input source.
  *   This parameter can be one of the following values:
  *   PMD_EMG_PORT_INPUT, PMD_EMG_COMPARATOR_OUTPUT.
  * @retval None.
  */
void PMD_SetEMGInputSrc(TSB_PMD_TypeDef * PMDx, uint32_t Source)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_EMG_INPUT_SOURCE(Source));
    /* Set PMDxEMGCR <EMGISEL> to select EMG input source */
    tmp = PMDx->EMGCR & PMD_EMGCR_EMGISEL_MASK;
    tmp |= (Source << 2U);
    PMDx->EMGCR = tmp;
}

/**
  * @brief  Enable OVV protection of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @retval None.
  */
void PMD_EnableOVV(TSB_PMD_TypeDef * PMDx)
{
    /* Check the parameter */
    assert_param(IS_PMD_PERIPH(PMDx));
    /* Set PMDxOVVCR <OVVEN> to enable OVV protection */
    PMDx->OVVCR |= PMD_OVVCR_OVVEN_SET;
}

/**
  * @brief  Disable OVV protection of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @retval None.
  */
void PMD_DisableOVV(TSB_PMD_TypeDef * PMDx)
{
    /* Check the parameter */
    assert_param(IS_PMD_PERIPH(PMDx));
    /* To prohibit OVV function, three instructions must be executed consecutively */
    /* Step1: Set "0x5A" to PMDxEMGREL <EMGREL> */
    PMDx->EMGREL = PMD_EMGREL_DISABLE_COMMAND_1;
    /* Step2: Set "0xA5" to PMDxEMGREL <EMGREL> */
    PMDx->EMGREL = PMD_EMGREL_DISABLE_COMMAND_2;
    /* Step3: Set "0" to PMDxOVVCR <OVVEN> */
    PMDx->OVVCR &= PMD_OVVCR_OVVEN_CLEAR;
}

/**
  * @brief  Set the noise elimination time for abnormal condition detection input of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  NoiseElimination: Select the noise elimination time.
  *   This parameter can be one of the following values:
  *   PMD_NOISE_ELIMINATION_16, PMD_NOISE_ELIMINATION_32,
  *   PMD_NOISE_ELIMINATION_48, PMD_NOISE_ELIMINATION_64, PMD_NOISE_ELIMINATION_80,
  *   PMD_NOISE_ELIMINATION_96, PMD_NOISE_ELIMINATION_112, PMD_NOISE_ELIMINATION_128,
  *   PMD_NOISE_ELIMINATION_144, PMD_NOISE_ELIMINATION_160, PMD_NOISE_ELIMINATION_176,
  *   PMD_NOISE_ELIMINATION_192, PMD_NOISE_ELIMINATION_208, PMD_NOISE_ELIMINATION_224,
  *   PMD_NOISE_ELIMINATION_240.
  * @retval None.
  */
void PMD_SetOVVNoiseElimination(TSB_PMD_TypeDef * PMDx, uint32_t NoiseElimination)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_OVV_NOISE_ELIMINATION(NoiseElimination));
    /* Set PMDxOVVCR <OVVCNT> to select the noise elimination time */
    tmp = PMDx->OVVCR & PMD_OVVCR_OVVCNT_MASK;
    tmp |= (NoiseElimination << 8U);
    PMDx->OVVCR = tmp;
}

/**
  * @brief  Enable or disable ADC monitor interrupt input of OVV protection of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  Monitor: Select the ADC monitor for OVV protection.
  *   This parameter can be one of the following values:
  *   PMD_ADC_MONITOR_A, PMD_ADC_MONITOR_B.
  * @param  NewState: ADC monitor interrupt input enable in OVV protection state.
  *   This parameter can be one of the following values:
  *   ENABLE, DIABLE.
  * @retval None.
  */
void PMD_SetADCMonitorInput(TSB_PMD_TypeDef * PMDx, uint32_t Monitor, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_ADC_MONITOR(Monitor));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (Monitor == PMD_ADC_MONITOR_A) {
        if (NewState == ENABLE) {
            /* Set PMDxOVVCR <ADIN0EN> to enable ADC A monitor interrupt input */
            PMDx->OVVCR |= PMD_OVVCR_ADIN0EN_SET;
        } else {
            /* Clear PMDxOVVCR <ADIN0EN> to disable ADC A monitor interrupt input */
            PMDx->OVVCR &= PMD_OVVCR_ADIN0EN_CLEAR;
        }
    } else {
        if (NewState == ENABLE) {
            /* Set PMDxOVVCR <ADIN1EN> to enable ADC B monitor interrupt input */
            PMDx->OVVCR |= PMD_OVVCR_ADIN1EN_SET;
        } else {
            /* Clear PMDxOVVCR <ADIN1EN> to disable ADC B monitor interrupt input */
            PMDx->OVVCR &= PMD_OVVCR_ADIN1EN_CLEAR;
        }
    }
}

/**
  * @brief  Set OVV protection mode of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  Mode: OVV protection mode.
  *   This parameter can be one of the following values:
  *   PMD_OVV_MODE_0, PMD_OVV_MODE_1, PMD_OVV_MODE_2, PMD_OVV_MODE_3.
  * @retval None.
  */
void PMD_SetOVVMode(TSB_PMD_TypeDef * PMDx, uint32_t Mode)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_OVV_MODE(Mode));
    /* Set PMDxOVVCR <OVVMD> to select OVV protection mode */
    tmp = PMDx->OVVCR & PMD_OVVCR_OVVMD_MASK;
    tmp |= (Mode << 3U);
    PMDx->OVVCR = tmp;
}

/**
  * @brief  Set OVV input source of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  Source: OVV input source.
  *   This parameter can be one of the following values:
  *   PMD_OVV_PORT_INPUT, PMD_OVV_ADC_MONITOR.
  * @retval None.
  */
void PMD_SetOVVInputSrc(TSB_PMD_TypeDef * PMDx, uint32_t Source)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_PMD_OVV_INPUT_SOURCE(Source));
    /* Set PMDxOVVCR <OVVISEL> to select OVV input source */
    tmp = PMDx->OVVCR & PMD_OVVCR_OVVISEL_MASK;
    tmp |= (Source << 2U);
    PMDx->OVVCR = tmp;
}

  /**
  * @brief  Enable or disable automatic release of OVV protection of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @param  NewState: Automatic release enable in OVV protection state.
  *   This parameter can be one of the following values:
  *   ENABLE, DIABLE.
  * @retval None.
  */
void PMD_SetOVVAutoRelease(TSB_PMD_TypeDef * PMDx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_PMD_PERIPH(PMDx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE) {
        /* Set PMDxOVVCR <OVVRS> to enable automatic release of OVV protection */
        PMDx->OVVCR |= PMD_OVVCR_OVVRS_SET;
    } else {
        /* Clear PMDxOVVCR <OVVRS> to disable automatic release of OVV protection */
        PMDx->OVVCR &= PMD_OVVCR_OVVRS_CLEAR;
    }
}


/**
  * @brief  Get the level of abnormal condition OVV input of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @retval The level of abnormal condition input.
  *   The value returned can be one of the following values:
  *   PMD_ABNORMAL_LEVEL_L, PMD_ABNORMAL_LEVEL_H.
  */
uint32_t PMD_GetOVVAbnormalLevel(TSB_PMD_TypeDef * PMDx)
{
    uint32_t tmp = 0U;
    /* Check the parameter */
    assert_param(IS_PMD_PERIPH(PMDx));
    /* Read PMDxOVVSTA <OVVI> to get the level of abnormal condition input */
    tmp = PMDx->OVVSTA & PMD_OVVSTA_OVVI_MASK;
    return tmp;
}

/**
  * @brief  Get the OVV protection condition of the specified PMD channel.
  * @param  PMDx: Select the PMD channel.
  *   This parameter can be one of the following values:
  *   PMD0, PMD1.
  * @retval The OVV protection condition.
  *   The value returned can be one of the following values:
  *   PMD_OVV_NORMAL, PMD_OVV_PROTECTED.
  */
uint32_t PMD_GetOVVCondition(TSB_PMD_TypeDef * PMDx)
{
    uint32_t tmp = 0U;
    /* Check the parameter */
    assert_param(IS_PMD_PERIPH(PMDx));
    /* Read PMDxOVVSTA <OVVST> to get the OVV protection condition */
    tmp = PMDx->OVVSTA & PMD_OVVSTA_OVVST_MASK;
    return tmp;
}

/* mabrouk edit pmd adc_pmdtrg */
void ADC_PmdTrg (void)
{
  
     
   /* enable the AMP for the 3-shunt resistors "ADC input" */
  
    TSB_AMP->CTLA = 0x03;
    TSB_AMP->CTLB = 0x03;
    TSB_AMP->CTLC = 0x03;
      
  /* select registers 2,3 to carry the programs 2,3 that get triggered by PMD0TRG2,3 */
   /*ADC_SelectPMDTrgProgNum(TSB_ADA, PMD_TRG_PROG_SEL2, TRG_ENABLE(PMD_PROG2));  // PMD0TRG2  ==> ADAPSEL2 ==> Program2
   ADC_SelectPMDTrgProgNum(TSB_ADA, PMD_TRG_PROG_SEL3, TRG_ENABLE(PMD_PROG3));*/  // PMD0TRG3  ==> ADAPSEL3 ==> Program3
   TSB_ADA->PSEL2 = 0x82;
   TSB_ADA->PSEL3 = 0x83;
   
   
   
   /* select the interrupt generated to each program */
  /* PMD_TrgProgINTTypeDef myIntSelection ;
   myIntSelection.INTProg0=0;
   myIntSelection.INTProg1=0;
   myIntSelection.INTProg2=1;
   myIntSelection.INTProg3=0;
   myIntSelection.INTProg4=0;
   myIntSelection.INTProg5=0;
   ADC_SetPMDTrgProgINT(TSB_ADA,myIntSelection);*/
   TSB_ADA->PINTS0 = 0;
   TSB_ADA->PINTS1 = 0;
   TSB_ADA->PINTS2 = 1;  //PMD0TRG2  ==> ADAPSEL2 ==> Program2  ==> PMD_INTADPDA 
   TSB_ADA->PINTS3 = 0;

   
   /* ADC PMD trigger program set " set each regx(0:5) to its AINy(0:11) pin and its VE phase (u,v,w) */
   TSB_ADA->PSET2 = 0x008C8D8E;  //  PMD0TRG2  ==> ADAPSEL2 ==>       Program2  =1.=> PMD_INTADPDA       
                                                                              //=2.=> reg0,1,2 to AIN12,13,14to PHASEu,v,w
   
   TSB_ADA->PSET3 = 0x82000000;  //  PMD0TRG3  ==> ADAPSEL3 ==>       Program3     => reg3 AIN8 to Vdc
   // Program3 (AINA8)  -> Reg3, Vdc
   
   
   /* additional conf */
   TSB_ADA->MOD3 = 0x0560; //fixed value at TGE
   TSB_ADA->MOD0 = 0x02; //close software tigger and on dac control
   TSB_ADA->MOD1 = 0x80; //enable adc

}
/*mabrouk edit pmd pmd_on*/
void PMD_ON(void){
    /*********solve the correct adc conversion refrence (lower)**************/
    //TSB_PMD0->MDOUT = 0x73F;
        // Configure Phase U
    PMD_SetPortOutput(PMD0, PMD_PHASE_U, PMD_OUTPUT_PWM_IPWM);

    // Configure Phase V
    PMD_SetPortOutput(PMD0, PMD_PHASE_V, PMD_OUTPUT_PWM_IPWM);

    // Configure Phase W
    PMD_SetPortOutput(PMD0, PMD_PHASE_W, PMD_OUTPUT_PWM_IPWM);
    /*****************************************************************************/
}
/*mabrouk edit pmd pmd_off*/
void PMD_OFF(void){

    //TSB_PMD0->MDOUT = 0x000;
    PMD_SetPortOutput(PMD0, PMD_PHASE_U, PMD_OUTPUT_L_L);

    // Configure Phase V
    PMD_SetPortOutput(PMD0, PMD_PHASE_V, PMD_OUTPUT_L_L);

    // Configure Phase W
    PMD_SetPortOutput(PMD0, PMD_PHASE_W, PMD_OUTPUT_L_L);
    
    /**********************************************************************/
}

/*mabrouk edit pmd pmdenableandrelease*/
void PMD_EnableEMG_andRelease(TSB_PMD_TypeDef * PMDx)
{
    /* Check the parameter */
    assert_param(IS_PMD_PERIPH(PMDx));

    /* Set PMDxEMGCR to achieve the equivalent of 0x0F3B */
    PMDx->EMGCR = PMD_ENABLE_EMG_AND_RELEASE ;
}
/** @} */
/* End of group PMD_Exported_Functions */

/** @} */
/* End of group PMD */

/** @} */
/* End of group TX03_Periph_Driver */

#endif                          /* defined(__TMPM370_PMD_H) */
