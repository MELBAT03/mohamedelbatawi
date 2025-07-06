/**===========================================================================
* @file		McMotor.h
==============================================================================*/

#ifndef	_MC_MOTOR_H_
#define	_MC_MOTOR_H_

/***************************************************************************************************
*	[Include File]
**************************************************************************************************/
#include "cpu.h"
#include "../McApp/McProject.h"
// #include "DevMotor.h"



/***************************************************************************************************
*	[Define]
**************************************************************************************************/

// _MOTOR_LD_ Motor =====================================================================================
#if defined(_MOTOR_LD_)	
#define POLE_PAIR				24

#define RS						5.975
#define LD						0.0375		//	(H)
#define LQ						0.0391		//	(H)
#define KE						0.134	 	// (Vpeak / Wr)

#define X_OBS					28.0 // 18.0 // 64.0

// #define WC_LPF_VDC				(2.0 * PI * 120.0)
// #define WC_LPF_IDQ				(2.0 * PI / (TS_CC * 3.2))
// #define _NON_CC_				// Voltage estimation control (=���� ���� ����)
// #define _NON_CC_VFF_			// Current Controller Voltage Feedforward None (=��������� ���� Feedforward ����) 

// #define _DEAD_TIME_COMPEN_V_	// Phase voltage compensation corresponding to IPM Dead Time (=IPM Dead Time�� �ش��ϴ� ������ ����) 
// #define T_DEAD_COMPEN			(1.0 * T_IPM_DEAD)
// #define T_PWMCMP				(1.0 * TS_CC)
// #define T_OBSCMP				(1.0 * TS_CC)

#define WC_CC					(2.0 * PI * FREQ_PWM / 35.0) // 60.0)	// 

#define JM_SC					(0.2)  // (0.026 // 
#define WC_SC					(1.5 * 2.0 * PI)	//(5.0 * 2.0 * PI)

#define KP_FWC					0.0005  // 0.0012	// 0.0021	//	0.0018	// // = 2 Wb Zeta /(WcWeLs -1/WeLs)	//Zeta:0.7~2
#define KI_FWC					0.0844	// 0.22//0.1506	// 0.2813	//	// = Wb^2 / Wc We Ls

#if defined(_EST_SENSORLESS_) && defined(_SYNC_ACCEL_)
#define T_ALIGN					0.15
#define T_MAX_STARTUP			1.5
#define I_ALIGN					1.0
#define I_STARTUP_INIT			1.0
#define I_STARTUP_MAX			3.0
#define RPMD_SYNC_MAX			50.0
#define SLOPE_RPMD_SYNC			50.0

#else

#define T_ALIGN					0.15
#define T_MAX_STARTUP			1.2
#define I_ALIGN					1.0
#define I_STARTUP_INIT			1.0
#define I_STARTUP_MAX			6.0
#endif

// #define _PHASE_OUT_ACB_

// Fully automatic Newmotech SD Motor =========================================================================
#elif defined(_MOTOR_SD_)

#if(0)	
#define POLE_PAIR				12

#define RS						5.85	// (Ohm)	// 25degree: 6.1	// 112.4degree: 
#define LD						0.02403 // 0.02403(H)
#define LQ						0.02603 // 0.02403(H)
#define KE						0.1208	// (Vpeak / Wr)

#define WC_LPF_IDQ				(2.0 * PI / (TS_CC * 6.4)) //3.2))
#define WC_LPF_VDC				(2.0 * PI * 60.0)//120)
#define SCALE_VEQ				12
#define SCALE_INV_KE			14

#define X_OBS					32//64

#define WC_CC					(2.0 * PI * FREQ_PWM / 50.0) // 28.0)			

#define JM_SC					(0.05)
#define WC_SC					(0.6 * 2.0 * PI)

#define KP_SC_ROLL				(1.0 * (JM_SC * WC_SC))					//(1.5 * (JM_SC * WC_SC))				  
#define KI_SC_ROLL				(1.0 * (JM_SC * WC_SC * WC_SC / 5.0)) 	//(1.5 * (JM_SC * WC_SC * WC_SC / 5.0)) 

#define RPMD_ROLL_EXIT			100.0//20.0//5.0			
#define RPMD_SPEED_FAIL			500.0
#define T_OVERLOAD_FAIL			(0.8)
#define _PHASE_OUT_ACB_

#define T_ALIGN					0.15						
#define T_MAX_STARTUP			0.5
#define I_ALIGN					1.5
#define I_STARTUP_INIT			0.3
#define I_STARTUP_MAX			3.0

#if(0)
#define T_ALIGN					0.15						
#define T_MAX_STARTUP			1.2
#define I_ALIGN					1.5
#define I_STARTUP_INIT			0.3
#define I_STARTUP_MAX			3.0
#endif

#endif

#define POLE_PAIR				12

#define RS						5.9		// (Ohm)	 
#define LD						0.02403 // 0.02403(H)
#define LQ						0.02603 // 0.02403(H)
#define KE						0.1208	// (Vpeak / Wr)

#define SCALE_VEQ				12
#define SCALE_INV_KE			14

//#define X_OBS					64.0
#define X_OBS					25.0//32.0

#define WC_CC					(2.0 * PI * FREQ_PWM / 60.0)//60.0(Works well under overload conditions (=���������ǿ��� �ߵ�)))//28.0(default (=�⺻��))

#define JM_SC					(0.05)  
#define WC_SC					(0.6 * 2.0 * PI)	

#define KP_SC_ROLL(JM_SC_id)				(1.5 * (JM_SC_id * WC_SC)) // 		JM_SC_id = 	JM_SC or JM_SC_BASKET	  
#define KI_SC_ROLL(JM_SC_id)				(1.5 * (JM_SC_id * WC_SC * WC_SC / 5.0)) // 		JM_SC_id = 	JM_SC or JM_SC_BASKET

#define RPMD_ROLL_EXIT			20.0			
#define RPMD_SPEED_FAIL			500.0
#define T_OVERLOAD_FAIL			(0.8) 
#define _PHASE_OUT_ACB_

#define T_ALIGN					0.15						
#define T_MAX_STARTUP			1.2
#define I_ALIGN					1.5
#define I_STARTUP_INIT			0.3
#define I_STARTUP_MAX			3.0
#define KP_SC_ROLL_PULSATOR          KP_SC_ROLL(JM_SC)
#define KI_SC_ROLL_PULSATOR           KI_SC_ROLL(JM_SC)
#define KA_SC_ROLL_PULSATOR           KA_SC_ROLL(JM_SC)
#define KP_SC_PULSATOR                KP_SC(JM_SC)
#define KI_SC_PULSATOR                 KI_SC(JM_SC)
#define KA_SC_PULSATOR                 KA_SC(JM_SC)

#define T_ALIGN_BASKET					(0.5)//(0.5)						
#define T_MAX_STARTUP_BASKET			(2.7)//(1.7)
#define I_ALIGN_BASKET					(2)//(1.5)
#define I_STARTUP_INIT_BASKET			(2.3)//(1.7)
#define I_STARTUP_MAX_BASKET			(3)//(2.5)
#define JM_SC_BASKET					(0.1)//(0.15)  
#define KP_SC_ROLL_BASKET           KP_SC_ROLL(JM_SC_BASKET)
#define KI_SC_ROLL_BASKET           KI_SC_ROLL(JM_SC_BASKET)
#define KA_SC_ROLL_BASKET           KA_SC_ROLL(JM_SC_BASKET)
#define KP_SC_BASKET                KP_SC(JM_SC_BASKET)
#define KI_SC_BASKET                KI_SC(JM_SC_BASKET)
#define KA_SC_BASKET                KA_SC(JM_SC_BASKET)

#if(0)
#define RPMD_ROLL_EXIT			100.0						/* 1st EPP Immediately previous value */
#define T_ALIGN					0.15						
#define T_MAX_STARTUP			1.2
#define I_ALIGN					1.5
#define I_STARTUP_INIT			0.5
#define I_STARTUP_MAX			8.0
#endif

// Fully automatic NIDEC Motor =========================================================================
#elif defined(_MOTOR_NIDEC_)

#define POLE_PAIR				12

#define RS						5.9		// (Ohm)	 
#define LD						0.02403 // 0.02403(H)
#define LQ						0.02603 // 0.02403(H)
#define KE						0.1208	// (Vpeak / Wr)

#define SCALE_VEQ				12
#define SCALE_INV_KE			14

//#define X_OBS					64.0
#define X_OBS					32.0

#define WC_CC					(2.0 * PI * FREQ_PWM / 60.0)//60.0(Works well under overload conditions (=���������ǿ��� �ߵ�)))//28.0(default (=�⺻��))

#define JM_SC					(0.05)  
#define WC_SC					(0.6 * 2.0 * PI)	

#define KP_SC_ROLL				(1.5 * (JM_SC * WC_SC))				  
#define KI_SC_ROLL				(1.5 * (JM_SC * WC_SC * WC_SC / 5.0)) 

#define RPMD_ROLL_EXIT			20.0			
#define RPMD_SPEED_FAIL			500.0
#define T_OVERLOAD_FAIL			(0.8)
#define _PHASE_OUT_ACB_

#define T_ALIGN					0.15						
#define T_MAX_STARTUP			1.2
#define I_ALIGN					1.5
#define I_STARTUP_INIT			0.3
#define I_STARTUP_MAX			3.0

#endif

// =================================================================================================


#ifndef INIT_MC_PARAM
#define INIT_MC_PARAM()		do {  } while(0)
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


/***************************************************************************************************
*	[End Of Header File]
**************************************************************************************************/
#endif