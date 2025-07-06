/**
 *******************************************************************************
 * @file    tmpm370_pmd.h
 * @brief   This file provides all the functions prototypes for PMD driver. 
 * @version V2.0.2.2
 * @date    2014/05/20
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM370_PMD_H
#define __TMPM370_PMD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "TMPM370.h"
#include "tx03_common.h"

/** @addtogroup TX03_Periph_Driver
  * @{
  */
/** @addtogroup PMD
  * @{
  */

/** @addtogroup PMD Exported_types
  * @{
  */

/** @brief PMD initialization structure definition
  * @{
  */
    typedef struct {
        uint32_t CycleMode;     /* Specify PWM cycle extension mode */
        uint32_t DutyMode;      /* Choose DUTY mode */
        uint32_t IntTiming;     /* Choose PWM interrupt timing when PWM mode 1 (triangle wave) is set */
        uint32_t IntCycle;      /* Choose PWM interrupt cycle */
        uint32_t CarrierMode;   /* Specify PWM carrier wave */
        uint32_t CycleTiming;   /* Set PWM cycle */
        uint32_t ModeSelectBus_VE;   /* Set the select mode of the specified PMD channel *//*mabrouk edit pmd 1*/
    } PMD_InitTypeDef;

/** @} */
/* End of PMD_Exported_types */

/** @defgroup PMD_Exported_Constants
  * @{
  */
#define PMD0                               TSB_PMD0
#define PMD1                               TSB_PMD1
#define IS_PMD_PERIPH(param)               (((param) == PMD0) || ((param) == PMD1))

#define PMD_PORT_MODE_0                    ((uint32_t)0x00000000)       /* Upper phases = High-Z / Lower phases = High-Z */
#define PMD_PORT_MODE_1                    ((uint32_t)0x00000001)       /* Upper phases = High-Z / Lower phases = PMD output */
#define PMD_PORT_MODE_2                    ((uint32_t)0x00000002)       /* Upper phases = PMD output / Lower phases = High-Z */
#define PMD_PORT_MODE_3                    ((uint32_t)0x00000003)       /* Upper phases = PMD output / Lower phases = PMD output */
#define IS_PMD_PORT_MODE(param)            ((param) <= PMD_PORT_MODE_3)

#define PMD_BUS_MODE                       ((uint32_t)0x00000000)       /* Bus mode */
#define PMD_VE_MODE                        ((uint32_t)0x00000001)       /* VE mode */
#define IS_PMD_SELECT_MODE(param)          ((param) <= PMD_VE_MODE)

#define PMD_PWM_NORMAL_CYCLE               ((uint32_t)0x00000000)       /* Normal cycle */
#define PMD_PWM_4_FOLD_CYCLE               ((uint32_t)0x00000001)       /* 4-fold cycle */
#define IS_PMD_CYCLE_MODE(param)           ((param) <= PMD_PWM_4_FOLD_CYCLE)

#define PMD_DUTY_MODE_U_PHASE              ((uint32_t)0x00000000)       /* U-phase in common */
#define PMD_DUTY_MODE_3_PHASE              ((uint32_t)0x00000001)       /* 3-phase independent */
#define IS_PMD_DUTY_MODE(param)            ((param) <= PMD_DUTY_MODE_3_PHASE)

#define PMD_PWM_INT_TIMING_MINIMUM         ((uint32_t)0x00000000)       /* When PWM count MDCNT="1" is set, (minimum) interrupt request occurs */
#define PMD_PWM_INT_TIMING_MAXIMUM         ((uint32_t)0x00000001)       /* When PWM count MDCNT=PMDxMDPRD<MDPRD> is set, (maximum) interrupt request occurs */
#define IS_PMD_PWM_INT_TIMING(param)       ((param) <= PMD_PWM_INT_TIMING_MAXIMUM)

#define PMD_PWM_INT_CYCLE_HALF             ((uint32_t)0x00000000)       /* Every PWM 0.5 cycle (can be set in PWM mode1 (triangle wave) */
#define PMD_PWM_INT_CYCLE_1                ((uint32_t)0x00000001)       /* Every PWM 1 cycle */
#define PMD_PWM_INT_CYCLE_2                ((uint32_t)0x00000002)       /* Every PWM 2 cycles */
#define PMD_PWM_INT_CYCLE_4                ((uint32_t)0x00000003)       /* Every PWM 4 cycles */
#define IS_PMD_PWM_INT_CYCLE_MODE_0(param) (((param) == PMD_PWM_INT_CYCLE_1)||\
                                            ((param) == PMD_PWM_INT_CYCLE_2)||\
                                            ((param) == PMD_PWM_INT_CYCLE_4))

#define IS_PMD_PWM_INT_CYCLE_MODE_1(param) (((param) == PMD_PWM_INT_CYCLE_HALF)||\
                                            ((param) == PMD_PWM_INT_CYCLE_1)||\
                                            ((param) == PMD_PWM_INT_CYCLE_2)||\
                                            ((param) == PMD_PWM_INT_CYCLE_4))

#define PMD_CARRIER_WAVE_MODE_0            ((uint32_t)0x00000000)       /* PWM mode 0 (edge PWM, sawtooth) */
#define PMD_CARRIER_WAVE_MODE_1            ((uint32_t)0x00000001)       /* PWM mode 1 (center PWM, triangle wave) */
#define IS_PMD_CARRIER_WAVE_MODE(param)    ((param) <= PMD_CARRIER_WAVE_MODE_1)

#define PMD_COUNTER_UP                     ((uint32_t)0x00000000)       /* Up-counting */
#define PMD_COUNTER_DOWN                   ((uint32_t)0x00000001)       /* Down-counting */

#define PMD_PHASE_U                        ((uint32_t)0x00000000)
#define PMD_PHASE_V                        ((uint32_t)0x00000001)
#define PMD_PHASE_W                        ((uint32_t)0x00000002)
#define PMD_PHASE_ALL                      ((uint32_t)0x00000003)
#define IS_PMD_PHASE(param)                ((param) <= PMD_PHASE_ALL)

#define PMD_PORT_OUTPUT_MODE_0             ((uint32_t)0x00000000)
#define PMD_PORT_OUTPUT_MODE_1             ((uint32_t)0x00000001)
#define IS_PMD_PORT_OUTPUT_MODE(param)     ((param) <= PMD_PORT_OUTPUT_MODE_1)

#define PMD_OUTPUT_PHASE_LOWER             ((uint32_t)0x00000000)       /* Lower phase output port */
#define PMD_OUTPUT_PHASE_UPPER             ((uint32_t)0x00000001)       /* Upper phase output port */
#define IS_PMD_OUTPUT_PHASE(param)         ((param) <= PMD_OUTPUT_PHASE_UPPER)

#define PMD_POLARITY_LOW                   ((uint32_t)0x00000000)       /* Low active */
#define PMD_POLARITY_HIGH                  ((uint32_t)0x00000001)       /* High active */
#define IS_PMD_POLARITY(param)             ((param) <= PMD_POLARITY_HIGH)

#define PMD_REFLECTED_TIME_WRITE           ((uint32_t)0x00000000)       /* Reflect when write */
#define PMD_REFLECTED_TIME_MIN             ((uint32_t)0x00000001)       /* Reflect when PWM counter MDCNT="1"(minimum) */
#define PMD_REFLECTED_TIME_MAX             ((uint32_t)0x00000002)       /* Reflect when PWM counter MDCNT=PMDxMDPRD<MDPRD>(maximum) */
#define PMD_REFLECTED_TIME_MIN_MAX         ((uint32_t)0x00000003)       /* Reflect when PWM counter MDCNT="1"(minimum) or PMDxMDPRD<MDPRD>(maximum) */
#define IS_PMD_REFLECTED_TIME(param)       ((param) <= PMD_REFLECTED_TIME_MIN_MAX)

#define PMD_NOISE_ELIMINATION_NONE         ((uint32_t)0x00000000)       /* Noise filter is not used */
#define PMD_NOISE_ELIMINATION_16           ((uint32_t)0x00000001)       /* Input noise elimination time 1*16/fsys[s] */
#define PMD_NOISE_ELIMINATION_32           ((uint32_t)0x00000002)       /* Input noise elimination time 2*16/fsys[s] */
#define PMD_NOISE_ELIMINATION_48           ((uint32_t)0x00000003)       /* Input noise elimination time 3*16/fsys[s] */
#define PMD_NOISE_ELIMINATION_64           ((uint32_t)0x00000004)       /* Input noise elimination time 4*16/fsys[s] */
#define PMD_NOISE_ELIMINATION_80           ((uint32_t)0x00000005)       /* Input noise elimination time 5*16/fsys[s] */
#define PMD_NOISE_ELIMINATION_96           ((uint32_t)0x00000006)       /* Input noise elimination time 6*16/fsys[s] */
#define PMD_NOISE_ELIMINATION_112          ((uint32_t)0x00000007)       /* Input noise elimination time 7*16/fsys[s] */
#define PMD_NOISE_ELIMINATION_128          ((uint32_t)0x00000008)       /* Input noise elimination time 8*16/fsys[s] */
#define PMD_NOISE_ELIMINATION_144          ((uint32_t)0x00000009)       /* Input noise elimination time 9*16/fsys[s] */
#define PMD_NOISE_ELIMINATION_160          ((uint32_t)0x0000000A)       /* Input noise elimination time 10*16/fsys[s] */
#define PMD_NOISE_ELIMINATION_176          ((uint32_t)0x0000000B)       /* Input noise elimination time 11*16/fsys[s] */
#define PMD_NOISE_ELIMINATION_192          ((uint32_t)0x0000000C)       /* Input noise elimination time 12*16/fsys[s] */
#define PMD_NOISE_ELIMINATION_208          ((uint32_t)0x0000000D)       /* Input noise elimination time 13*16/fsys[s] */
#define PMD_NOISE_ELIMINATION_224          ((uint32_t)0x0000000E)       /* Input noise elimination time 14*16/fsys[s] */
#define PMD_NOISE_ELIMINATION_240          ((uint32_t)0x0000000F)       /* Input noise elimination time 15*16/fsys[s] */
#define IS_PMD_NOISE_ELIMINATION(param)    ((param) <= PMD_NOISE_ELIMINATION_240)
#define IS_PMD_OVV_NOISE_ELIMINATION(param)    (((param) >= PMD_NOISE_ELIMINATION_16) && ((param) <= PMD_NOISE_ELIMINATION_240))

#define PMD_BREAK_STATUS_PMD               ((uint32_t)0x00000000)       /* PMD output is continued */
#define PMD_BREAK_STATUS_HIGH_IMPEDANCE    ((uint32_t)0x00000001)       /* High-impedance */
#define IS_PMD_BREAK_STATUS(param)         ((param) <= PMD_BREAK_STATUS_HIGH_IMPEDANCE)

#define PMD_EMG_MODE_0                     ((uint32_t)0x00000000)       /* PWM output control disabled / Port output = All phases High-Z */
#define PMD_EMG_MODE_1                     ((uint32_t)0x00000001)       /* All upper phases ON, all lower phases OFF / Port output = Lower phases High-Z */
#define PMD_EMG_MODE_2                     ((uint32_t)0x00000002)       /* All upper phases OFF, all lower phases ON / Port output = Upper phases High-Z */
#define PMD_EMG_MODE_3                     ((uint32_t)0x00000003)       /* All phases OFF / Port output = All phases High-Z */
#define IS_PMD_EMG_MODE(param)             ((param) <= PMD_EMG_MODE_3)

#define PMD_EMG_PORT_INPUT                 ((uint32_t)0x00000000)       /* Port input as the EMG input signal */
#define PMD_EMG_COMPARATOR_OUTPUT          ((uint32_t)0x00000001)       /* Comparator output as the EMG input signal */
#define IS_PMD_EMG_INPUT_SOURCE(param)     ((param) <= PMD_EMG_COMPARATOR_OUTPUT)

#define PMD_ADC_TRG_0                      ((uint32_t)0x00000000)       /* Select trigger 0 */
#define PMD_ADC_TRG_1                      ((uint32_t)0x00000001)       /* Select trigger 1 */
#define PMD_ADC_TRG_2                      ((uint32_t)0x00000002)       /* Select trigger 2 */
#define PMD_ADC_TRG_3                      ((uint32_t)0x00000003)       /* Select trigger 3 */
#define IS_PMD_ADC_TRG(param)              ((param) <= PMD_ADC_TRG_3)

#define PMD_TRG_MODE_0                     ((uint32_t)0x00000000)       /* Trigger output disabled */
#define PMD_TRG_MODE_1                     ((uint32_t)0x00000001)       /* Trigger output at down-count match  */
#define PMD_TRG_MODE_2                     ((uint32_t)0x00000002)       /* Trigger output at up-count match */
#define PMD_TRG_MODE_3                     ((uint32_t)0x00000003)       /* Trigger output at up-/down-count match */
#define PMD_TRG_MODE_4                     ((uint32_t)0x00000004)       /* Trigger output at PWM carrier peak */
#define PMD_TRG_MODE_5                     ((uint32_t)0x00000005)       /* Trigger output at PWM carrier bottom */
#define PMD_TRG_MODE_6                     ((uint32_t)0x00000006)       /* Trigger output at PWM carrier peak/bottom */
#define PMD_TRG_MODE_7                     ((uint32_t)0x00000007)       /* Trigger output disabled */
#define IS_PMD_TRG_MODE(param)             ((param) <= PMD_TRG_MODE_7)

#define PMD_TRG_UPDATE_SYNC                ((uint32_t)0x00000000)
#define PMD_TRG_UPDATE_ASYNC               ((uint32_t)0x00000001)
#define IS_PMD_TRG_TIMING(param)           ((param) <= PMD_TRG_UPDATE_ASYNC)

#define PMD_ABNORMAL_LEVEL_L               ((uint32_t)0x00000000)       /* Abnormal condition input level is "L" */
#define PMD_ABNORMAL_LEVEL_H               ((uint32_t)0x00000002)       /* Abnormal condition input level is "H" */

#define IS_PMD_VALUE(param)                ((param) <= 0x0000FFFFU)
#define IS_PMD_DEAD_TIME(param)            ((param) <= 0x000000FFU)

#define PMD_OUTPUT_L_L                     ((uint8_t)0x00)
#define PMD_OUTPUT_L_H                     ((uint8_t)0x01)
#define PMD_OUTPUT_H_L                     ((uint8_t)0x02)
#define PMD_OUTPUT_H_H                     ((uint8_t)0x03)
#define PMD_OUTPUT_IPWM_PWM                ((uint8_t)0x04)

#define PMD_OUTPUT_PWM_IPWM                ((uint8_t)0x07)

#define PMD_OUTPUT_L_PWM                   ((uint8_t)0x05)
#define PMD_OUTPUT_L_IPWM                  ((uint8_t)0x05)
#define PMD_OUTPUT_IPWM_H                  ((uint8_t)0x05)

#define PMD_OUTPUT_PWM_L                   ((uint8_t)0x06)
#define PMD_OUTPUT_H_IPWM                  ((uint8_t)0x06)
#define PMD_OUTPUT_H_PWM                   ((uint8_t)0x06)

#define IS_PMD_OUTPUT(param)               ((param) <= 7U)

#define PMD_TRG_FIXED_OUTPUT               ((uint32_t)0x00000000)       /* Fixed trigger output */
#define PMD_TRG_VARIABLE_OUTPUT            ((uint32_t)0x00000001)       /* Variable trigger output  */
#define IS_PMD_TRG_OUTPUT_MODE(param)      ((param) <= PMD_TRG_VARIABLE_OUTPUT)

#define PMD_TRG_OUTPUT_0                   ((uint32_t)0x00000000)       /* Output from PMDTRG0 */
#define PMD_TRG_OUTPUT_1                   ((uint32_t)0x00000001)       /* Output from PMDTRG1 */
#define PMD_TRG_OUTPUT_2                   ((uint32_t)0x00000002)       /* Output from PMDTRG2 */
#define PMD_TRG_OUTPUT_3                   ((uint32_t)0x00000003)       /* Output from PMDTRG3 */
#define PMD_TRG_OUTPUT_4                   ((uint32_t)0x00000004)       /* Output from PMDTRG4 */
#define PMD_TRG_OUTPUT_5                   ((uint32_t)0x00000005)       /* Output from PMDTRG5 */
#define IS_PMD_TRG_OUTPUT(param)           ((param) <= PMD_TRG_OUTPUT_5)
#define IS_PMD_TRG_FIXED_OUTPUT(param)     ((param) <= PMD_TRG_OUTPUT_3)

#define PMD_ADC_MONITOR_A                  ((uint32_t)0x00000000)       /* ADC A monitor interrupt input */
#define PMD_ADC_MONITOR_B                  ((uint32_t)0x00000001)       /* ADC B monitor interrupt input */
#define IS_PMD_ADC_MONITOR(param)          ((param) <= PMD_ADC_MONITOR_B)

#define PMD_OVV_MODE_0                     ((uint32_t)0x00000000)       /* No output control */
#define PMD_OVV_MODE_1                     ((uint32_t)0x00000001)       /* All upper phases ON, all lower phases OFF */
#define PMD_OVV_MODE_2                     ((uint32_t)0x00000002)       /* All upper phases OFF, all lower phases ON */
#define PMD_OVV_MODE_3                     ((uint32_t)0x00000003)       /* All phases OFF (ON  = High, OFF =  Low [when <POLL>,<POLH>  =  1 (active high)]) */
#define IS_PMD_OVV_MODE(param)             ((param) <= PMD_OVV_MODE_3)

#define PMD_OVV_PORT_INPUT                 ((uint32_t)0x00000000)       /* Port input as the OVV input signal */
#define PMD_OVV_ADC_MONITOR                ((uint32_t)0x00000001)       /* ADC monitor signal as the OVV input signal */
#define IS_PMD_OVV_INPUT_SOURCE(param)     ((param) <= PMD_OVV_ADC_MONITOR)

#define PMD_OVV_NORMAL                     ((uint32_t)0x00000000)       /* OVV protection control circuit is in normal operation */
#define PMD_OVV_PROTECTED                  ((uint32_t)0x00000001)       /* OVV protection control circuit is in protection */

/** @} */
/* End of group PMD_Exported_Constants */


/** @defgroup PMD_Exported_FunctionPrototypes
  * @{
  */
    void PMD_Enable(TSB_PMD_TypeDef * PMDx);
    void PMD_Disable(TSB_PMD_TypeDef * PMDx);
    void PMD_SetPortControl(TSB_PMD_TypeDef * PMDx, uint32_t PortMode);
    void PMD_Init(TSB_PMD_TypeDef * PMDx, PMD_InitTypeDef * InitStruct);
    void PMD_ChangePWMCycle(TSB_PMD_TypeDef * PMDx, uint32_t CycleTiming);
    uint32_t PMD_GetCntFlag(TSB_PMD_TypeDef * PMDx);
    uint16_t PMD_GetCntValue(TSB_PMD_TypeDef * PMDx);
    void PMD_SetCompareValue(TSB_PMD_TypeDef * PMDx, uint32_t PMDPhase, uint32_t Timing);
    void PMD_SetPortOutputMode(TSB_PMD_TypeDef * PMDx, uint32_t Mode);
    void PMD_SetOutputPhasePolarity(TSB_PMD_TypeDef * PMDx, uint32_t OutputPhase,
                                    uint32_t Polarity);
    void PMD_SetReflectTime(TSB_PMD_TypeDef * PMDx, uint32_t ReflectedTime);
    void PMD_EnableEMG(TSB_PMD_TypeDef * PMDx);
    void PMD_DisableEMG(TSB_PMD_TypeDef * PMDx);
    void PMD_SetEMGNoiseElimination(TSB_PMD_TypeDef * PMDx, uint32_t NoiseElimination);
    void PMD_SetToolBreakOutput(TSB_PMD_TypeDef * PMDx, uint32_t Status);
    void PMD_SetEMGMode(TSB_PMD_TypeDef * PMDx, uint32_t Mode);
    void PMD_EMGRelease(TSB_PMD_TypeDef * PMDx);
    uint32_t PMD_GetEMGAbnormalLevel(TSB_PMD_TypeDef * PMDx);
    uint32_t PMD_GetEMGCondition(TSB_PMD_TypeDef * PMDx);
    void PMD_SetDeadTime(TSB_PMD_TypeDef * PMDx, uint32_t Time);
    void PMD_SetAllPhaseCompareValue(TSB_PMD_TypeDef * PMDx, uint32_t UPhaseTiming,
                                     uint32_t VPhaseTiming, uint32_t WPhaseTiming);
    void PMD_ChangeDutyMode(TSB_PMD_TypeDef * PMDx, uint32_t DutyMode);
    Result PMD_SetPortOutput(TSB_PMD_TypeDef * PMDx, uint32_t PMDPhase, uint8_t Output);
    void PMD_SetTrgCmpValue(TSB_PMD_TypeDef * PMDx, uint32_t TRGCMP0Timing, uint32_t TRGCMP1Timing,
                            uint32_t TRGCMP2Timing, uint32_t TRGCMP3Timing);
    void PMD_SetTrgMode(TSB_PMD_TypeDef * PMDx, uint32_t PMDTrg, uint32_t Mode);
    void PMD_SetTrgUpdate(TSB_PMD_TypeDef * PMDx, uint32_t PMDTrg, uint32_t UpdateTiming);
    void PMD_SetEMGTrg(TSB_PMD_TypeDef * PMDx, FunctionalState NewState);
    void PMD_SetTrgOutput(TSB_PMD_TypeDef * PMDx, uint32_t TrgMode, uint32_t TrgChannel);
    void PMD_SetSelectMode(TSB_PMD_TypeDef * PMDx, uint32_t Mode);
    void PMD_SetEMGInputSrc(TSB_PMD_TypeDef * PMDx, uint32_t Source);
    void PMD_EnableOVV(TSB_PMD_TypeDef * PMDx);
    void PMD_DisableOVV(TSB_PMD_TypeDef * PMDx);
    void PMD_SetOVVNoiseElimination(TSB_PMD_TypeDef * PMDx, uint32_t NoiseElimination);
    void PMD_SetADCMonitorInput(TSB_PMD_TypeDef * PMDx, uint32_t Monitor, FunctionalState NewState);
    void PMD_SetOVVMode(TSB_PMD_TypeDef * PMDx, uint32_t Mode);
    void PMD_SetOVVInputSrc(TSB_PMD_TypeDef * PMDx, uint32_t Source);
    void PMD_SetOVVAutoRelease(TSB_PMD_TypeDef * PMDx, FunctionalState NewState);
    uint32_t PMD_GetOVVAbnormalLevel(TSB_PMD_TypeDef * PMDx);
    uint32_t PMD_GetOVVCondition(TSB_PMD_TypeDef * PMDx);
    /* mabrouk edit pmd adc_pmdtrg */
    void ADC_PmdTrg (void);
    /*mabrouk edit pmd pmd_on */
    void PMD_ON(void);
    /*mabrouk edit pmd pmd_off */
    void PMD_OFF(void);
    /*mabrouk edit pmd enableandrelease*/
    void PMD_EnableEMG_andRelease(TSB_PMD_TypeDef * PMDx);

/** @} */
/* End of group PMD_Exported_FunctionPrototypes */

/** @} */
/* End of group PMD */

/** @} */
/* End of group TX03_Periph_Driver */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __TMPM370_PMD_H */
