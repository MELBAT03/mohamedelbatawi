/**===========================================================================
* @file		McTarget.c
==============================================================================*/


/***************************************************************************************************
*	[Include File]
**************************************************************************************************/
#include "../McApp/McProject.h"
#include "McTarget.h"
#include "McUtil.h"
#include "cpu.h"
#include "../core/tmpm370/inc/tmpm370_gpio.h"
#include "../McApp/McBoard.h"
#include "../core/tmpm370/inc/tmpm370_adc.h"
#include "../core/tmpm370/inc/tmpm370_pmd.h"
#include <string.h>
#include <stdio.h>



/***************************************************************************************************
*	[Define]
**************************************************************************************************/


/***************************************************************************************************
*	[Enumeration & TypeDeclarations]
**************************************************************************************************/


/***************************************************************************************************
*	[Constant & Variable Declarations]
**************************************************************************************************/


#define THETA_HALL_OFFSET		(60.0 * PI / 180.0)
const Q15 caqThetaHall[4] =
{								//	Hb	Ha
  Q_THETA_R(THETA_HALL_OFFSET + PI * 0.0 /4.0),	//	0	0
  Q_THETA_R(THETA_HALL_OFFSET + PI * -2.0 /4.0),	//	0	1
  Q_THETA_R(THETA_HALL_OFFSET + PI * 2.0 /4.0),	//	1	0
  Q_THETA_R(THETA_HALL_OFFSET + PI * -4.0 /4.0)	//	1	1
};


//Why const Q15**?
// The array is declared const, so its address must also be treated as a pointer to a const object.
// Function to get a pointer to the array
Q15 getCaqThetaHallPointer( U08 index) 
{
  Q15 hal_value =0;
  if (index < 4) 
  {
    hal_value=caqThetaHall[index];  // Assign the address of the array to the pointer
  }
  return hal_value;
}

uint8_t read_port_hall(void) 
{
  /*mabrouk edit portD data*/
  //    uint8_t temp = GPIO_ReadData(GPIO_PD); //PORTD;   // may be: GPIO_ReadData(GPIO_PD);if they want to read the whole port
  //    return (((temp & 0x01) << 1) | ((temp & 0x02) >> 1));
  //    
  
  uint8_t temp = GPIO_ReadData((GPIO_Port)(MC_HALL_A_PORT| MC_HALL_B_PORT)); 
  return (((temp & MC_HALL_A_PIN) << 1) | ((temp & MC_HALL_B_PIN) >> 1));
}

/***************************************************************************************************
*	[Function Declaration]
**************************************************************************************************/

void mc_util_disable_mc_interrupts(void)
{
  /* mabrouk edit nvic clrenint*/
  NVIC_DisableIRQ(INTEMG0_IRQn);				// IPM_FO Interrupt Disable (EMG)
  NVIC_DisableIRQ(INTADAPDA_IRQn);			// PWM_ADC Interrupt Disable
  NVIC_DisableIRQ(INTADAPDB_IRQn);			// PWM_REAR Interrupt Disable
  NVIC_DisableIRQ(INTENC0_IRQn);	            // Encoder input0 interrupt Disable
}

void mc_util_interrupt_priority_set(void)
{
  // PWM_ADC Interrupt Priority Set, Pending Clear & Enable
  NVIC_SetPriority(INTADAPDA_IRQn,0x2);
  NVIC_ClearPendingIRQ(INTADAPDA_IRQn);
  NVIC_EnableIRQ(INTADAPDA_IRQn);
  
  // IPM_FO Interrupt Priority Set, Pending Clear & Enable (EMG)
  NVIC_SetPriority(INTEMG0_IRQn,0x2);
  NVIC_ClearPendingIRQ(INTEMG0_IRQn);
  NVIC_EnableIRQ(INTEMG0_IRQn);
  
}

void mc_util_set_config_peripheral (void)
{
  // Built-in OP-AMP (Vector Engine) Setting 
  /*mabrouk edit*/
  /*added to ADC_PmdTrg()
  (Gain: 2.5, Enable AMP)
  */ 
  
  
  /************************* ADC Setting ********************/
  // ADC Clock Setting
  ADC_SetClk(TSB_ADA, ADC_HOLD_FIX, ADC_FC_DIVIDE_LEVEL_2);
  
  
  // [3 Shunt current detection]
  // ADC PMD TRIGGER initialization 
  ADC_PmdTrg();
  
  /************************* PMD Setting ********************/
  PMD_Disable(PMD0);
  /* init struct initialization */
  PMD_InitTypeDef m_pmd;
  /* PMD0 initialization */
  m_pmd.CycleMode = PMD_PWM_NORMAL_CYCLE;
  m_pmd.DutyMode = PMD_DUTY_MODE_3_PHASE;                 /* 3-phase independent */
  m_pmd.IntTiming = PMD_PWM_INT_TIMING_MINIMUM;           //INTPWM  at counter = top                         ::options:: PMD_PWM_INT_TIMING_MINIMUM  : PMD_PWM_INT_TIMING_MAXIMUM
  m_pmd.IntCycle = PMD_PWM_INT_CYCLE_HALF;                //INTPWM once a cycle   PMD_PWM_INT_CYCLE_1   PMD_PWM_INT_CYCLE_HALF
  m_pmd.CarrierMode = PMD_CARRIER_WAVE_MODE_1;            /* PWM mode 1 (center PWM, triangle wave) */
  m_pmd.CycleTiming =(DWORD)(FREQ_CLK / (FREQ_PWM *2));   //2500;done   // refer to 16KHz frequancy PWM output   = (Fcpu / (2 * Fpwm)) = (80000000/(16000*2))         /* 0x3FFFU; */
  m_pmd.ModeSelectBus_VE = PMD_BUS_MODE;
  PMD_Init(PMD0,&m_pmd);
  
  PMD_SetDeadTime(PMD0,(DWORD)(T_IPM_DEAD * FREQ_CLK / 8));  // DEAD_TIME 0x0FU ==> 1.5micro according to the system calc
  
  PMD_SetAllPhaseCompareValue(PMD0, 0, 0, 0);
  
  PMD_SetOutputPhasePolarity(PMD0,PMD_OUTPUT_PHASE_UPPER,PMD_POLARITY_HIGH);
  PMD_SetOutputPhasePolarity(PMD0,PMD_OUTPUT_PHASE_LOWER,PMD_POLARITY_HIGH);
  PMD_SetReflectTime(PMD0,PMD_REFLECTED_TIME_MIN_MAX);
  
  PWM_CON(OFF);			// Open Output
  
  DISABLE_EMG();			// EMG Disable & Pending Clear 
  
  PMD_SetTrgCmpValue(PMD0,0,0,(DWORD)((TCLK_PWM_PERIOD >>1) - TCLK_AD_CONV + (TCLK_IPM_DEAD >>1)),(DWORD)((TCLK_PWM_PERIOD >>1) - 2 * TCLK_AD_CONV + (TCLK_IPM_DEAD >>1)));
  
  PMD_SetTrgMode(PMD0, PMD_ADC_TRG_0, PMD_TRG_MODE_2);
  PMD_SetTrgMode(PMD0, PMD_ADC_TRG_1, PMD_TRG_MODE_1);
  PMD_SetTrgMode(PMD0, PMD_ADC_TRG_2, PMD_TRG_MODE_2);
  PMD_SetTrgMode(PMD0, PMD_ADC_TRG_3, PMD_TRG_MODE_2);
  
  /* Output enable setting during EMG protection operation */
  PMD_SetEMGTrg(PMD0,ENABLE); //::options::  ENABLE : DISABLE			// Enable Trigger output in the protecttion state 
  
  
  PMD_SetPortControl(PMD0,PMD_PORT_MODE_0);///* Upper phases = High-Z / Lower phases = High-Z *// Upper/Lower Phase Hi-Z Output (Debug Stop)
  
  PMD_Enable(PMD0);			// PMD Enable
  
  // ENC Setting ==================================================================
  
  // PWM Port Enable Output
  GPIO_EnableFuncReg(MC_U0_PORT,GPIO_FUNC_REG_1,MC_U0_PIN);
  GPIO_EnableFuncReg(MC_X0_PORT,GPIO_FUNC_REG_1,MC_X0_PIN);
  GPIO_EnableFuncReg(MC_V0_PORT,GPIO_FUNC_REG_1,MC_V0_PIN);
  GPIO_EnableFuncReg(MC_Y0_PORT,GPIO_FUNC_REG_1,MC_Y0_PIN);
  GPIO_EnableFuncReg(MC_W0_PORT,GPIO_FUNC_REG_1,MC_W0_PIN);
  GPIO_EnableFuncReg(MC_Z0_PORT,GPIO_FUNC_REG_1,MC_Z0_PIN);
  
  GPIO_SetOutputEnableReg(MC_U0_PORT,MC_U0_PIN,ENABLE);   //set the bit to be output without changing any other bit configuration (F R )
  GPIO_SetOutputEnableReg(MC_X0_PORT,MC_X0_PIN,ENABLE);
  GPIO_SetOutputEnableReg(MC_V0_PORT,MC_V0_PIN,ENABLE);
  GPIO_SetOutputEnableReg(MC_Y0_PORT,MC_Y0_PIN,ENABLE);
  GPIO_SetOutputEnableReg(MC_W0_PORT,MC_W0_PIN,ENABLE);
  GPIO_SetOutputEnableReg(MC_Z0_PORT,MC_Z0_PIN,ENABLE);
  
  // IPM_FO EMG Port
  GPIO_SetInputEnableReg(MC_EMG0_PORT,MC_EMG0_PIN,ENABLE);
  GPIO_SetPullUp(MC_EMG0_PORT,MC_EMG0_PIN,ENABLE);
  GPIO_EnableFuncReg(MC_EMG0_PORT,GPIO_FUNC_REG_1,MC_EMG0_PIN);
  
  // Hall Sensor Port Enable input
  GPIO_EnableFuncReg(MC_HALL_A_PORT,GPIO_FUNC_REG_1,MC_HALL_A_PIN);
  GPIO_EnableFuncReg(MC_HALL_B_PORT,GPIO_FUNC_REG_1,MC_HALL_B_PIN);
  GPIO_DisableFuncReg(MC_HALL_A_PORT,GPIO_FUNC_REG_1,MC_HALL_A_PIN);
  GPIO_DisableFuncReg(MC_HALL_B_PORT,GPIO_FUNC_REG_1,MC_HALL_B_PIN);
  GPIO_SetInputEnableReg(MC_HALL_A_PORT,MC_HALL_A_PIN,ENABLE);
  GPIO_SetInputEnableReg(MC_HALL_B_PORT,MC_HALL_B_PIN,ENABLE);
}




/***************************************************************************************************
*	[End Of Source File]
**************************************************************************************************/

