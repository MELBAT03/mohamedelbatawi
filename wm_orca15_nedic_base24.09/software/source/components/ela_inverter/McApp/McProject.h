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
// Select motor type (=���� Ÿ�� ����) //===================================================================
// #define _MOTOR_LD_
#define _MOTOR_SD_
// #define _MOTOR_NIDEC_ 
// #define _MOTOR_PARAM_		// Motor parameter tuning (=���� �Ķ���� Ʃ��)
// // // 
// Controller H/W environment selection (=����� H/W ȯ�� ����)  //==============================================================
// #define _HW_ELUX_374F_	//  Washing machine separate controller (=��Ź�� �и��� �����) (Micom : TMPM374F) (IAR 6.20)
#define _HW_280SD_370F_		// Washing machine fully automatic vector control PCB (=��Ź�� ���ڵ� �������� PCB) (IAR 6.20)
//

// Select speed & magnetic flux position estimator (multiple selection possible) (=�ӵ�&�ڼ���ġ ������ ����(���߼��ð���)) //===========================================
// #define _EST_SENSORLESS_	// Sensorless estimation (=�������� ����)
#define _EST_HALL_OBS_			//
// #define _EST_HALL_MT_	
// #define _EST_ENCODER_
//
// 
// Sensorless speed & magnetic flux position estimator selection (=�������� �ӵ�&�ڼ���ġ ������ ����) //================================================
// #define _SL_MATSUI_	
// #define _SL_EX_EMF_
// #define _SL_MO_EMF_
// #define _SL_FLUX_
// #define _SL_INTEGRAL_
// #define _SL_QUASI_
// 
// Select control method (=���� ��� ����) //=====================================================================
// #define _NON_CC_				// ���� ���� ����->Renesas ����� ����Ϸ��� ���⼭�� �����ϸ� �˴ϴ�. (=���� ���� ����->���׻罺����� ������ ���⸸ �츮�� �ȴ�.)
// #define _NON_CC_VFF_			// Current Controller Voltage Feedforward None (=��������� ���� Feedforward ����) 
// #define _PWM_SV_				// Space vector voltage modulation setting option (Default=120 degree discontinuous voltage modulation) (=�������� ���� ���� ���� �ɼ� (Default= 120�� �ҿ��� ���� ����))
// //
// #define _NON_STARTUP_
#define _SYNC_ACCEL_
// #define _NON_MTPA_
// #define _TUNING_MTPA_
// #define _MEASURE_RS_
// #define _DEAD_TIME_COMPEN_T_	// Simple deadtime compensation algorithm for three-phase inverter (=3�� �ι��Ϳ� ���� ������ ����Ÿ�� ���� �˰���)
// #define _DEAD_TIME_COMPEN_V_	// Phase voltage compensation corresponding to IPM Dead Time (=IPM Dead Time�� �ش��ϴ� ������ ����) 
//
// #define _MATSUI_AUTOTUNING_
//
// #define _ONE_SHUNT_			// Motor phase current 1 shunt measurement setting option (Default= 3 shunt measurements) (=���� ������ 1 ��Ʈ ���� ���� �ɼ� (Default= 3 ��Ʈ ����))
// #define _OPAMP_INVERTING_
// #define _PHASE_OUT_ACB_
//
// #define _NON_ENCODER_Z_		// Encoder index pulse not used (=���ڴ� �ε��� �޽� ������) 
// #define _ENCODER_ORDER_BA_	// Change encoder rotation direction (=���ڴ� ȸ������ ����) 
// 
// #define _DISABLE_EMG_
// #define _MTR_VOLT_FAIL_CHECK_MASK_	// Low/high voltage check disable setting option (=��/������ üũ ���� ���� �ɼ�)

// #define _FLT_TRAPEZOIDAL_	// (Default= _FLT_BACKWARD_)
// #define _FLT_FORWARD_
// 
#define _INLINE_

// Select the Application specification //============================================================
#define _APP_WASHING_MACHINE_		// Washing machine single process (=��Ź�� �ܵ�����)  
// #define _APP_DEBUG_MOTOR_		// Monitoring and stirring, Spin, Motor control, DAC output (=����͸� �� ����,Ż��,��������, DAC ���)
// #define _APP_SLAVE_MC_			// Operates as a slave single controller (motor operation and status response with commands from the master controller) (=Slave �ܵ� ������ ����(Master������� �������� ���� ���� �� ���� ����)) 
// 
//
// DEV Peripheral Apply settings (=���� ����) //=========================================================
// #define _USE_UART_4800_
// #define _USE_UART_9600_
// #define _USE_EEPROM_
// #define _USE_ADC_
// #define _USE_DAC_
//#define   printf_open   0
// Apply CPU settings for each controller H/W (=����� H/W �� CPU ���� ����) ///////////////////////////////////////////////////////
#if defined(_HW_ELUX_374F_)
#define _CPU_TMPM374FW_
#elif defined(_HW_280SD_370F_)
#define _CPU_TMPM370FY_
#endif

// DEV Peripheral application for each application (=Application�� DEV Peripheral ����) //////////////////////////////////////////////////
#if defined(_APP_WASHING_MACHINE_)

#elif defined(_APP_DEBUG_MOTOR_)
#define _USE_UART_9600_
#define _USE_ADC_
#define _USE_DAC_
#elif defined(_APP_SLAVE_MC_)
#define _USE_UART_9600_
#define _USE_ADC_
#endif

// Define sensorless estimator operation (=�������� ������ ���� Define) ///////////////////////////
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
