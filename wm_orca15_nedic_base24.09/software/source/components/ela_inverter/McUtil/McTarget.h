/**===========================================================================
 * @file		McTarget.h
==============================================================================*/

#ifndef	_MC_TARGET_H_
#define	_MC_TARGET_H_

/***************************************************************************************************
 *	[Include File]
 **************************************************************************************************/
#include "cpu.h"
#include "../McDev/McMotor.h"



/***************************************************************************************************
 *	[Define]
 **************************************************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(_HW_ELUX_374F_)
//=====[ Hardware setting constant (=�ϵ���� ���� ���) ]=================================================
#define IPM_ACTIVE_LEVEL		HI // ((HI << 1) | LOW)		// IPM Active setting 

#define V_ADC_REF				5.0
#define AD_BIT_NO				12

#define R_SHUNT_IPM				0.06						

#define GAIN_OPAMP_IPM			(10.0 / 2.4)

#define R_V_DC_HIGH				(270.0 * 2.0)				/* 540K */
#define R_V_DC_LOW				5.7							/* 4.7K */

#define FREQ_CLK				80000000.0

//=====[ PORT / register setting (=�������� ����) ]=================================================
#define PWM_CON(xOnOff)			do { if (xOnOff) PMD1MDOUT = 0x73F; else  PMD1MDOUT = 0x000; } while(0)
#define READ_PORT_HALL()		((PFDATA & 0x0C) >> 2)
#define READ_PORT_IPM_FO()		(PMD1EMGSTA_bit.EMGI)	// (PGDATA_bit.PG6)

// AD Result value Register Redefinition (=AD ����� �������� ������) 
#define AD15_REG_I_A			(Q15)(ADBREG0 >> 1)
#if defined(_PHASE_OUT_ACB_)
#define AD15_REG_I_B			(Q15)(ADBREG2 >> 1)
#define AD15_REG_I_C			(Q15)(ADBREG1 >> 1)
#else
#define AD15_REG_I_B			(Q15)(ADBREG1 >> 1)
#define AD15_REG_I_C			(Q15)(ADBREG2 >> 1)
#endif

#define AD15_REG_I_SIG			(Q15)(ADBREG0 >> 1)
#define AD15_REG_I_DUB			(Q15)(ADBREG1 >> 1)

#define AD15_REG_V_DC			(Q15)(ADBREG3 >> 1)

// PWM Duty Setting Macro (=PWM Duty ���� ��ũ��) 
#if defined(_PHASE_OUT_ACB_)
#define SET_PWM_DUTY(xDutyA, xDutyB, xDutyC)	\
					do { PMD1CMPU = (WORD)(xDutyA);	PMD1CMPW = (WORD)(xDutyB);	PMD1CMPV = (WORD)(xDutyC);	} while(0)
#else
#define SET_PWM_DUTY(xDutyA, xDutyB, xDutyC)	\
					do { PMD1CMPU = (WORD)(xDutyA);	PMD1CMPV = (WORD)(xDutyB);	PMD1CMPW = (WORD)(xDutyC);	} while(0)
#endif

// AD Triger Time setting macro 
#define SET_TRGAD_I_SIG(xTclk)	do { PMD1TRGCMP0 = (WORD)(xTclk);	} while(0)
#define SET_TRGAD_I_DUB(xTclk)	do { PMD1TRGCMP1 = (WORD)(xTclk);	} while(0)

// EMG Enable /Disable setting macro  
#define ENABLE_EMG()			do { PMD1EMGCR = 0x0F3B;	} while(0)		// EMG Protection Relrase & Enable
#define DISABLE_EMG()			do { PMD1EMGREL = 0x5A;	PMD1EMGREL = 0xA5;		PMD1EMGCR = 0x0000;	\
									 CLRPEND0 = (0x1 << 13);	} while(0)	// EMG Disable & Pending Clear

#if defined(_EST_HALL_MT_)
GLOBAL unsigned long unFrTmrBufMT;

#define GET_MT_FR_TIMER()			(WORD)(unFrTmrBufMT >> 7)
#define HOOK_ISR_HALL()			do{	\
	static S16 snHallCnt;	\
	unFrTmrBufMT += EN1CNT;	\
	if (EN1TNCR_bit.U_D == 0) snHallCnt++;	\
	else snHallCnt--;		}while(0)
#endif

#define IS_PWM_OFF_SEQUENCE()	((PMD1CNTSTA_bit.UPDWN) == 0 ? TRUE : FALSE)		// UP Cout => TRUE

#define INIT_DBG_PORT()				do { PECR_bit.PE0C = 1; }while(0)
#define SET_PWM_DBG_PORT(xHiLow)	do { PEDATA_bit.PE0 = (xHiLow); if (xHiLow == 0) ADBMOD0_bit.ADSS = 0x1;	}while(0)

/////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(_HW_280SD_370F_)
//=====[ Hardware setting constant ]=================================================
#define IPM_ACTIVE_LEVEL		HI				// IPM Active setting 

#define V_ADC_REF				5.0
#define AD_BIT_NO				12

#define R_SHUNT_IPM				0.06

#define GAIN_OPAMP_IPM			(2.0)			// = 2.5*(1-(2/(2+8)))

#define R_V_DC_HIGH				(150.0 * 3.0)
#define R_V_DC_LOW				(4.7)

#define FREQ_CLK				80000000.0

// 
//=====[ PORT / register setting ]=================================================
          /*mabrouk edit pmd 2 */
#define PWM_CON(xOnOff)			do { if (xOnOff) PMD_ON(); else  PMD_OFF(); } while(0)
// #define READ_PORT_HALL()		(PORTD & 0x03)
 uint8_t read_port_hall(void);

// #define READ_PORT_HALL()		(((PORTD & 0x01)<<1) | ((PORTD & 0x02)>>1))

/* mabrouk edit pmd 4 */ 
#define READ_PORT_IPM_FO()		(PMD_GetEMGAbnormalLevel(PMD0))
//#define READ_PORT_IPM_FO()		(EMGSTA0_bit.EMGI)	// FO_ENG (PORTC_bit.PC6)

// AD Result value Register Redefinition (=AD ����� �������� ������) 
#define AD15_REG_I_A			(Q15)(ADAREG0 >> 1)
#if defined(_PHASE_OUT_ACB_)
#define AD15_REG_I_B			(Q15)(ADAREG2 >> 1)
#define AD15_REG_I_C			(Q15)(ADAREG1 >> 1)
#else
#define AD15_REG_I_B			(Q15)(ADAREG1 >> 1)
#define AD15_REG_I_C			(Q15)(ADAREG2 >> 1)
#endif

#define AD15_REG_I_SIG			(Q15)(ADAREG0 >> 1)
#define AD15_REG_I_DUB			(Q15)(ADAREG1 >> 1)

#define AD15_REG_V_DC			(Q15)(ADAREG3 >> 1)

// PWM Duty Setting Macro (=PWM Duty ���� ��ũ��) 
#if defined(_PHASE_OUT_ACB_)
#define SET_PWM_DUTY(xDutyA, xDutyB, xDutyC)	\
					do { CMPU0 = (WORD)(xDutyA);	CMPW0 = (WORD)(xDutyB);	CMPV0 = (WORD)(xDutyC);	} while(0)
#else
#define SET_PWM_DUTY(xDutyA, xDutyB, xDutyC)	\
					do { CMPU0 = (WORD)(xDutyA);	CMPV0 = (WORD)(xDutyB);	CMPW0 = (WORD)(xDutyC);	} while(0)
#endif

// AD Triger Time setting macro 
#define SET_TRGAD_I_SIG(xTclk)	do { TRGCMP00 = (WORD)(xTclk);	} while(0)
#define SET_TRGAD_I_DUB(xTclk)	do { TRGCMP10 = (WORD)(xTclk);	} while(0)

                                          
/* mabrouk edit pmd 2 */
/*mabrouk edit nvic clrpend*/
                                       
#define ENABLE_EMG()			do { PMD_EnableEMG_andRelease(PMD0);	} while(0)			// EMG Protection Relrase & Enable
#define DISABLE_EMG()			do { PMD_DisableEMG(PMD0);    NVIC_ClearPendingIRQ(INTEMG0_IRQn);	} while(0)                                          
// IPM_FO Enable /Disable setting macro  
//#define ENABLE_EMG()			do { EMGCR0 = 0x0F3B;	} while(0)			// EMG Protection Relrase & Enable
//#define DISABLE_EMG()			do { EMGREL0 = 0x5A;	EMGREL0 = 0xA5;		EMGCR0 = 0x0000;	\
									 CLRPEND0 = (0x1 << 12);	} while(0)	// EMG Disable & Pending Clear
							 
#if defined(_EST_HALL_MT_)
GLOBAL unsigned long unFrTmrBufMT;

#define GET_MT_FR_TIMER()			(WORD)(unFrTmrBufMT >> 7)
#define HOOK_ISR_HALL()			do{	\
	static S16 snHallCnt;	\
	unFrTmrBufMT += EN0CNT;	\
	if (EN0TNCR_bit.U_D == 0) snHallCnt++;	\
	else snHallCnt--;		}while(0)
#endif

          /* mabrouk edit pmd 4 */
#define IS_PWM_OFF_SEQUENCE()	((PMD_GetCntFlag(PMD0)) == 0 ? TRUE : FALSE)
//#define IS_PWM_OFF_SEQUENCE()	((CNTSTA0_bit.UPDWN) == 0 ? TRUE : FALSE)		// UP Cout => TRUE


          /*mabrouk edit portG cr*/
#define INIT_DBG_PORT()				do { GPIO_SetOutputEnableReg(GPIO_PG,GPIO_BIT_0,ENABLE);  }while(0)
//#define INIT_DBG_PORT()				do {/* PCCR_bit.PC6C = 1;*/}while(0) //PCCR_bit.PC7C = 1;
          /*mabrouk edit portG data*/
#define SET_PWM_DBG_PORT(xHiLow)	do { GPIO_WriteDataBit(GPIO_PG,GPIO_BIT_0,xHiLow); }while(0)
//#define SET_PWM_DBG_PORT(xHiLow)	do {/* PORTC_bit.PC6 = (xHiLow);*/}while(0)
#define TOGGLE_PWM_DBG_PORT             	do { /*PORTC^=(1<<6); */}while(0)
          
#define INIT_HEART_BEAT_PIN()				do { PGCR_bit.PG2C = 1;}while(0) //PCCR_bit.PC7C = 1;
#define SET_HEART_BEAT_PIN(xHiLow)	                do { PORTG_bit.PG2 = (xHiLow);}while(0)
#define TOGGLE_HEART_BEAT_PIN()                    do { PORTG^=(1<<2); }while(0)


#if 0
//=====[ PORT / Register setting ]=================================================
#define PWM_CON(xOnOff)			do { if (xOnOff) MDOUT1 = 0x73F; else  MDOUT1 = 0x000; } while(0)
// #define READ_PORT_HALL()		((PORTF & 0xC) >> 2)
#define READ_PORT_IPM_FO()		(EMGSTA1_bit.EMGI)	// (PORTG_bit.PG6)	/* FO_ENG */

// AD Result value Register Redefinition (=AD ����� �������� ������)
#define AD15_REG_I_A			(Q15)(ADAREG0 >> 1)
#if defined(_PHASE_OUT_ACB_)
#define AD15_REG_I_B			(Q15)(ADAREG2 >> 1)
#define AD15_REG_I_C			(Q15)(ADAREG1 >> 1)
#else
#define AD15_REG_I_B			(Q15)(ADAREG1 >> 1)
#define AD15_REG_I_C			(Q15)(ADAREG2 >> 1)
#endif

#define AD15_REG_I_SIG			(Q15)(ADAREG0 >> 1)
#define AD15_REG_I_DUB			(Q15)(ADAREG1 >> 1)

#define AD15_REG_V_DC			(Q15)(ADAREG3 >> 1)

// PWM Duty Setting Macro (=PWM Duty ���� ��ũ��) 
#if defined(_PHASE_OUT_ACB_)
#define SET_PWM_DUTY(xDutyA, xDutyB, xDutyC)	\
					do { CMPU1 = (WORD)(xDutyA);	CMPW1 = (WORD)(xDutyB);	CMPV1 = (WORD)(xDutyC);	} while(0)
#else
#define SET_PWM_DUTY(xDutyA, xDutyB, xDutyC)	\
					do { CMPU1 = (WORD)(xDutyA);	CMPV1 = (WORD)(xDutyB);	CMPW1 = (WORD)(xDutyC);	} while(0)
#endif

// AD Triger Time setting macro
#define SET_TRGAD_I_SIG(xTclk)	do { TRGCMP01 = (WORD)(xTclk);	} while(0)
#define SET_TRGAD_I_DUB(xTclk)	do { TRGCMP11 = (WORD)(xTclk);	} while(0)

// IPM_FO Enable /Disable setting macro /* FO_ENG */
#define ENABLE_EMG()			do { EMGCR1 = 0x0F3B;	} while(0)			// EMG Protection Relrase & Enable
#define DISABLE_EMG()			do { EMGREL1 = 0x5A;	EMGREL1 = 0xA5;		EMGCR1 = 0x0000;	\
									 CLRPEND0 = (0x1 << 13);	} while(0)	// EMG Disable & Pending Clear
									 
#if defined(_EST_HALL_MT_)
GLOBAL unsigned long unFrTmrBufMT;

#define GET_MT_FR_TIMER()			(WORD)(unFrTmrBufMT >> 7)
#define HOOK_ISR_HALL()			do{	\
	static S16 snHallCnt;	\
	unFrTmrBufMT += EN1CNT;	\
	if (EN1TNCR_bit.U_D == 0) snHallCnt++;	\
	else snHallCnt--;		}while(0)
#endif

#define IS_PWM_OFF_SEQUENCE()	((CNTSTA1_bit.UPDWN) == 0 ? TRUE : FALSE)		// UP Cout => TRUE

#define INIT_DBG_PORT()				do { }while(0)
#define SET_PWM_DBG_PORT(xHiLow)	do { }while(0)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

/***************************************************************************************************
 *	[Enumeration & TypeDeclarations]
 **************************************************************************************************/


/***************************************************************************************************
 *	[Constant & Variable Declarations]
 **************************************************************************************************/




/***************************************************************************************************
 *	[Function Declaration]
 **************************************************************************************************/

void mc_util_disable_mc_interrupts(void);
void mc_util_interrupt_priority_set(void);
void mc_util_set_config_peripheral (void);
Q15 getCaqThetaHallPointer( U08 index) ;
/***************************************************************************************************
 *	[End Of Header File]
 **************************************************************************************************/
#endif
