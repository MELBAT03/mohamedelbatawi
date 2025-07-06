/**===========================================================================
 * @file		McProject.h
==============================================================================*/

#ifndef	_MC_PROJECT_H_
#define	_MC_PROJECT_H_

/***************************************************************************************************
 *	[Include File]
 **************************************************************************************************/
#define _VERSION_SW_			1.0

/***************************************************************************************************
 *	[Define]
 **************************************************************************************************/
// Select motor type (=모터 타입 선택) //===================================================================
// #define _MOTOR_LD_
#define _MOTOR_SD_
// #define _MOTOR_NIDEC_ 
// #define _MOTOR_PARAM_		// Motor parameter tuning (=모터 파라메터 튜닝)
// // // 
// Controller H/W environment selection (=제어기 H/W 환경 선택)  //==============================================================
// #define _HW_ELUX_374F_	//  Washing machine separate controller (=세탁기 분리형 제어기) (Micom : TMPM374F) (IAR 6.20)
#define _HW_280SD_370F_		// Washing machine fully automatic vector control PCB (=세탁기 전자동 벡터제어 PCB) (IAR 6.20)
//

// Select speed & magnetic flux position estimator (multiple selection possible) (=속도&자속위치 추정기 선택(다중선택가능)) //===========================================
// #define _EST_SENSORLESS_	// Sensorless estimation (=센서리스 추정)
#define _EST_HALL_OBS_			//
// #define _EST_HALL_MT_	
// #define _EST_ENCODER_
//
// 
// Sensorless speed & magnetic flux position estimator selection (=센서리스 속도&자속위치 추정기 선택) //================================================
// #define _SL_MATSUI_	
// #define _SL_EX_EMF_
// #define _SL_MO_EMF_
// #define _SL_FLUX_
// #define _SL_INTEGRAL_
// #define _SL_QUASI_
// 
// Select control method (=제어 방식 선택) //=====================================================================
// #define _NON_CC_				// 전압 추정 제어->Renesas 방식을 사용하려면 여기서만 저장하면 됩니다. (=전압 추정 제어->르네사스방식을 쓰려면 여기만 살리면 된다.)
// #define _NON_CC_VFF_			// Current Controller Voltage Feedforward None (=전류제어기 전압 Feedforward 없음) 
// #define _PWM_SV_				// Space vector voltage modulation setting option (Default=120 degree discontinuous voltage modulation) (=공간벡터 전압 변조 설정 옵션 (Default= 120도 불연속 전압 변조))
// //
// #define _NON_STARTUP_
#define _SYNC_ACCEL_
// #define _NON_MTPA_
// #define _TUNING_MTPA_
// #define _MEASURE_RS_
// #define _DEAD_TIME_COMPEN_T_	// Simple deadtime compensation algorithm for three-phase inverter (=3상 인버터에 대한 간단한 데드타임 보상 알고리즘)
// #define _DEAD_TIME_COMPEN_V_	// Phase voltage compensation corresponding to IPM Dead Time (=IPM Dead Time에 해당하는 상전압 보상) 
//
// #define _MATSUI_AUTOTUNING_
//
// #define _ONE_SHUNT_			// Motor phase current 1 shunt measurement setting option (Default= 3 shunt measurements) (=모터 상전류 1 션트 측정 설정 옵션 (Default= 3 션트 측정))
// #define _OPAMP_INVERTING_
// #define _PHASE_OUT_ACB_
//
// #define _NON_ENCODER_Z_		// Encoder index pulse not used (=엔코더 인덱스 펄스 사용않함) 
// #define _ENCODER_ORDER_BA_	// Change encoder rotation direction (=엔코더 회전방향 변경) 
// 
// #define _DISABLE_EMG_
// #define _MTR_VOLT_FAIL_CHECK_MASK_	// Low/high voltage check disable setting option (=저/고전압 체크 금지 설정 옵션)

// #define _FLT_TRAPEZOIDAL_	// (Default= _FLT_BACKWARD_)
// #define _FLT_FORWARD_
// 
#define _INLINE_

// Select the Application specification //============================================================
#define _APP_WASHING_MACHINE_		// Washing machine single process (=세탁기 단독행정)  
// #define _APP_DEBUG_MOTOR_		// Monitoring and stirring, Spin, Motor control, DAC output (=모니터링 및 교반,탈수,모터제어, DAC 출력)
// #define _APP_SLAVE_MC_			// Operates as a slave single controller (motor operation and status response with commands from the master controller) (=Slave 단독 제어기로 동작(Master제어기의 지령으로 모터 운전 및 상태 응답)) 
// 
//
// DEV Peripheral Apply settings (=적용 설정) //=========================================================
// #define _USE_UART_4800_
// #define _USE_UART_9600_
// #define _USE_EEPROM_
// #define _USE_ADC_
// #define _USE_DAC_
//#define   printf_open   0
// Apply CPU settings for each controller H/W (=제어기 H/W 별 CPU 설정 적용) ///////////////////////////////////////////////////////
#if defined(_HW_ELUX_374F_)
#define _CPU_TMPM374FW_
#elif defined(_HW_280SD_370F_)
#define _CPU_TMPM370FY_
#endif

// DEV Peripheral application for each application (=Application별 DEV Peripheral 적용) //////////////////////////////////////////////////
#if defined(_APP_WASHING_MACHINE_)

#elif defined(_APP_DEBUG_MOTOR_)
#define _USE_UART_9600_
#define _USE_ADC_
#define _USE_DAC_
#elif defined(_APP_SLAVE_MC_)
#define _USE_UART_9600_
#define _USE_ADC_
#endif

// Define sensorless estimator operation (=센서리스 추정기 동작 Define) ///////////////////////////
#if defined(_SL_MATSUI_) || defined(_SL_EX_EMF_) || defined(_SL_MO_EMF_) || defined(_SL_FLUX_) || defined(_SL_INTEGRAL_) || defined(_SL_QUASI_)
#define _ACT_SENSORLESS_		
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

#endif	//_MC_PROJECT_H_
/***************************************************************************************************
 *	[End Of Header File]
 **************************************************************************************************/ 
