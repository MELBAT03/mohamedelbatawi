/**===========================================================================
 * @file		DevMotor.h
==============================================================================*/

#ifndef	_DEV_MOTOR_H_
#define	_DEV_MOTOR_H_

/***************************************************************************************************
 *	[Include File]
 **************************************************************************************************/
#include "../McUtil/McUtil.h"
#include "../McUtil/McTarget.h"
#include "../McUtil/cpu.h"
#include "../McApp/McProject.h"

//#define INTERRUPT		void	
//#define PUBLIC 


/***************************************************************************************************
 *	[Define]
 **************************************************************************************************/

// Parameter variable conversion macro (=�Ķ���� ���� ������ ��ũ��) =================================================
#define CONVERT_ANGLE(xAngle)		Q_THETA_R(PI * (xAngle) / 180.0)

#define CONVERT_CURRENT(xCurrent)	Q_CURRENT(xCurrent)
#define INVERT_CURRENT(xCurrent)	(S16)(((S32)(QMAX_CURRENT *2.0) * (xCurrent)) >> 16)
#define CONVERT_VOLT(xVolt) 			Q_VOLT(xVolt)
#define INVERT_VOLT(xVolt)			(S16)(((S32)(QMAX_VOLT *2.0) * (xVolt)) >> 16)
#define CONVERT_TORQUE(xTorque) 	Q_TORQUE(xTorque)

#define CONVERT_RPM(xRpm)			Q_OMEGA_RPM_DRUM(xRpm)
#define INVERT_RPM(xRpm)				(S16)(((S32)(QMAX_OMEGA_RPM_DRUM *2.0) * (xRpm) + (1L <<15)) >>16)
#define CONVERT_ACCEL(xRpmPerS) 		Q_SLOPE_RPM_DRUM(xRpmPerS)

#define CONVERT_ACCEL_MAX				(-1)

#define CONVERT_FREQUENCY(xFreq)	Q_FREQUENCY(xFreq)


// Motor Sensing Id definition -------------------------------------
#define MTR_SENS_UNBALREAL			0x01 	// Instantaneous unbalance detection (=���� ��߶��� ����)
#define MTR_SENS_TORQSLOPE			0x02 	// Torque slope detection (=��ũ ���� ����)
#define MTR_SENS_TORQLOAD			0x04 	// Torque load detection (=��ũ ���� ����)
#define MTR_SENS_WEIGHT			 	0x08 	// Acquire weight sensing data (=���� ���� ������ ���)
#define MTR_SENS_UNBALAVG			0x10 	// Acquisition of unbalance average data (=��߶��� ��յ����� ���)
#define MTR_SENS_FINDACCEL			0x20 	// Determination of optimal acceleration point (=���� ���ӽ��� �Ǵ�)
#define MTR_SENS_RESERVED1			0x40
#define MTR_SENS_RESERVED2			0x80

#define MTR_SENS_SWDUNBAL		MTR_SENS_UNBALREAL

/***************************************************************************************************
 *	[Constant & Variable Declarations]
 **************************************************************************************************/


/***************************************************************************************************
 *	[Define]
 **************************************************************************************************/
// Control current & voltage limiting constants (=���� ���� & ���� ���� ���) --------------------------
#ifndef VS_LIMIT
#define VS_LIMIT				(400 / SQRT3)	//(215.0 * SQRT2 / SQRT3)
#endif

#ifndef IS_LIMIT
#define IS_LIMIT				(10.0) // (3.05)	// 
#endif

#ifndef IS_MIN					// Minimum current limit to prevent sensorless estimation errors (=�������� �������� ������ ���� �ּ����� ���Ѱ�) 
#define IS_MIN					(0.0)	// 
#endif

#ifndef ID_FW_LIMIT				// Current limit of field weakening controller (=����� ������� ���� ����ġ) (0.0 < ID_FW_LIMIT <=  IS_LIMIT)
#define ID_FW_LIMIT				(IS_LIMIT * 1.0)		// = KE/LD
#endif

#ifndef K_ISID_LIM				// Maximum current limit value reduction gain due to field weakening current command reduction (=����� �������� ���ҿ� ���� �ִ� ���� ���Ѱ� ���� ����)  (0.0 <= K_ISID_LIM <= 1.0)
#define K_ISID_LIM				(0.97)
#endif

#ifndef K_FW_V_REF				// Field weakening control command voltage value gain (=����� ���� �������а� ����) 
#define K_FW_V_REF				(0.95)
#endif

#ifndef K_OVER_MODULATION		// Overmodulation limit command voltage value gain (=������ ���� �������а� ����) (1.0 <= K_OVER_MODULATION <= 1.154)
#define K_OVER_MODULATION		(1.0)
#endif

// current controller constant (=���� ����� ���) ---------------------------------
#ifndef WC_LPF_IDQ
#define WC_LPF_IDQ				(2.0 * PI / (TS_CC * 3.2)) 
#endif

#ifndef WC_LPF_VDC
#define WC_LPF_VDC				(2.0 * PI * 120.0)
#endif

#define TS_LPF_VDC				TS_CC

#ifndef WC_CC
#define WC_CC					(2.0 * PI * FREQ_PWM / 32.0)	// 24.0)	// 42.0)	//80.0)
#endif

#define KP_CC_D					(LD * WC_CC)
#define KP_CC_Q					(LQ * WC_CC)

#ifndef TI_CC
#define KI_CC_D					(RS * WC_CC)
#define KI_CC_Q					(RS * WC_CC)
#else							// TI_CC : Intergration time constant (=���� ������) = 5.(����) ~ 10.(����)
#define KI_CC_D					((KP_CC_D * WC_CC) / TI_CC)
#define KI_CC_Q					((KP_CC_Q * WC_CC) / TI_CC)
#endif

#define KA_CC_D					(1.0 / KP_CC_D)		// (1/3Kp <= K <= 3/Kp)
#define KA_CC_Q					(1.0 / KP_CC_Q)

// speed controller constant (=�ӵ� ����� ���) --------------------------------
#ifndef JM_SC
#define JM_SC					(0.5)  // (0.026 // 
#endif
#ifndef WC_SC
#define WC_SC					(1.0 * 2.0 * PI)	//(5.0 * 2.0 * PI)
#endif
#ifndef KP_SC
#define KP_SC(JM_SC_id)					(JM_SC_id * WC_SC)  // 		JM_SC_id = 	JM_SC or JM_SC_BASKET
#endif
#ifndef KI_SC
#define KI_SC(JM_SC_id)					(JM_SC_id * WC_SC * WC_SC / 5.0)  // 		JM_SC_id = 	JM_SC or JM_SC_BASKET
#endif
#define KA_SC(JM_SC_id)					(2.0 / KP_SC(JM_SC_id))	// (1/3Kp <= K <= 3/Kp)

#ifndef KP_SC_ROLL
#define KP_SC_ROLL				(KP_SC *1.5)	// 2.0)	// (5.0)	//(24.0)//(18.0)// 
#endif
#ifndef KI_SC_ROLL
#define KI_SC_ROLL				(KI_SC *3.5)	// 1.2)	// (10.0)	//(20.0)//(12.0)// 
#endif
#define KA_SC_ROLL(JM_SC_id)				(3.0 / KP_SC_ROLL(JM_SC_id))		// (1/3Kp <= K <= 3/Kp)

#ifndef RPMD_ROLL_EXIT							// Low speed speed controller gain application, field weakening controller stop release speed setting (=���� �ӵ������ ��������, ����� ����� ���� ���� �ӵ� ����) 
#define RPMD_ROLL_EXIT			(55.0 * QMAX_OMEGA_RPM_DRUM / 1800.0) 	// (55.0)
#endif

// Field weakening controller constant (=����� ����� ���) --------------------------------
#ifndef KP_FWC
#define KP_FWC					0.0025	//0.0018		// = 2 Wb Zeta /(WcWeLs -1/WeLs)	//Zeta:0.7~2
#endif
#ifndef KI_FWC
#define KI_FWC					0.6502	//0.0867		// = Wb^2 / Wc We Ls
#endif

// Motor control constant definitions (=���� ���� ��� ����) ---------------------------------------------
#ifndef T_READY
#define T_READY					0.05
#endif

#ifndef T_ALIGN
#define T_ALIGN					0.15
#endif

#ifndef I_ALIGN
#define I_ALIGN					1.5
#endif

#ifndef T_MAX_STARTUP
#define T_MAX_STARTUP			1.2
#endif

#ifndef I_STARTUP_INIT
#define I_STARTUP_INIT			1.5
#endif

#ifndef I_STARTUP_MAX
#define I_STARTUP_MAX			8.0
#endif

#ifndef SLOPE_I_SYNC
#define SLOPE_I_SYNC			((I_STARTUP_MAX - I_STARTUP_INIT) / T_MAX_STARTUP)
#endif 

#ifndef RPMD_STARTUP_EXIT
#define RPMD_STARTUP_EXIT		6.0
#endif

#ifndef RPMD_SYNC_MAX
#define RPMD_SYNC_MAX			40.0
#endif

#ifndef RPMD_SYNC_EXIT
#define RPMD_SYNC_EXIT			(RPMD_SYNC_MAX * 0.25)
#endif

#ifndef SLOPE_RPMD_SYNC
#define SLOPE_RPMD_SYNC			50.0
#endif

#ifndef RPMD_SPEED_FAIL
#define RPMD_SPEED_FAIL			(120.0 * QMAX_OMEGA_RPM_DRUM / 1800.0)
#endif

#ifndef T_OVERLOAD_FAIL			// Restraint failure when motor stops and reverses (=���� ���� �� ��ȸ���� ���� ����)
#define T_OVERLOAD_FAIL			(0.25)
#endif

#ifndef CURRENT_OVER_FAIL
#define CURRENT_OVER_FAIL		(QMAX_CURRENT * 0.45)
#endif

#ifndef V_ENCODER_OFFSET
#define V_ENCODER_OFFSET		(25.0)
#endif

#ifndef VOLT_UNDER2OVER
#define VOLT_UNDER2OVER			390.0
#endif
#ifndef VOLT_OVER2UNDER
#define VOLT_OVER2UNDER			200.0
#endif
#ifndef VOLT_UNDER2OK
#define VOLT_UNDER2OK			200.0						/* ????? */
#endif
#ifndef VOLT_OVER2OK
#define VOLT_OVER2OK			425.0						/* ????? */
#endif
#ifndef VOLT_OK2OVER
#define VOLT_OK2OVER			450.0
#endif
#ifndef VOLT_OK2UNDER
#define VOLT_OK2UNDER			140.0
#endif


// Motor controller FLAG -----------------------------------------
#define MCFLAG_AD_OFFSET			0x0001
#define MCFLAG_FWC_ON				0x0002
#define MCFLAG_SC_ON				0x0004
#define MCFLAG_SC_ROLL_GAIN			0x0008
#define MCFLAG_CC_ON				0x0010
#define MCFLAG_CC_VFF				0x0020
#define MCFLAG_PWM_OFF				0x0040
#define MCFLAG_PWM_ZERO				0x0080


#define FAILFLAG_IPM_FO				0x0001
#define FAILFLAG_UNDER_VOLT			0x0002
#define FAILFLAG_OVER_VOLT			0x0004
#define FAILFLAG_OVER_CURRENT		0x0008
#define FAILFLAG_HALLSENSOR			0x0010
#define FAILFLAG_CURRENT			0x0020
#define FAILFLAG_IPM_FO_MASK		0x0100
#define FAILFLAG_UNDER_VOLT_MASK	0x0200
#define FAILFLAG_ALIGN				0x0400
#define FAILFLAG_STARTUP			0x0800
#define FAILFLAG_SPEED				0x1000
#define FAILFLAG_OVERLOAD			0x2000
#define FAILFLAG_OFFSET				0x4000


#define IsAcZCroseOK()						TRUE

#define DevGetMotorPolePair()				(U08)(POLE_PAIR)
#define DevGetMotorControlMode()			(U08)(eMcCtrlMode)			// Motor drive mode

#define DevGetMotorVoltMeter()				(U16)(qVdcBus) 				// DC link voltage applied to the inverter (=�ι��Ϳ� �ΰ��Ǵ� DC ��ũ ����)
#define DevGetMotorInstantVotMeter()		(U16)(qVdcBus)

#define DevGetMotorActualRpm()				(S16)(qWrpmActual)			// Motor measurement rotation speed low-pass filter value (=���� ���� ȸ�� �ӵ� ����������� ��)
#define DevGetMotorTargetRpm()				(S16)(oRampWrpm.qTar)		// Final target speed value (=���� ��ǥ �ӵ���)
#define DevGetMotorReferenceRpm()			(S16)(oRampWrpm.qRef)		// Instantaneous control speed value (=���� ���� �ӵ���)

#define DevGetMotorDAxisCurrent()			(S16)(oIdqse.qD)			// Rotor D-axis Current value (=ȸ���� D�� ������)
#define DevGetMotorQAxisCurrent()			(S16)(oIdqse.qQ)			// Rotor Q-axis Current value (proportional to motor torque) (=ȸ���� Q�� ������ (���� ��ũ�� ���))
#define DevGetMotorTorque() 				(S16)(oIdqse.qQ)
#define DevGetMotorStatorVolt() 			(U16)(qVsRefCC) 			// Motor stator phase voltage (=���� ������ ������)
#define DevGetMotorLeadAngle()				(S16)(qThetaVsLead)			// Stator phase voltage angle based on the D axis of the rotor coordinate system (=ȸ���� ��ǥ���� D����� ������ ������ ����)

#define DevGetMotorHallCnt()				(S16)(nRevolCnt)			// Increase/decrease counter value according to rotation direction when hall signal is detected (=Ȧ��ȣ ������ ȸ�����⿡ ���� ���� ī���� ��)
#define DevGetMotorIRMS()					(U16)(0)

#define DevGetMotorSensId() 				(U08)(wMcSensId)
#define DevGetMotorSensEvent()				(U08)(wMcSensEvent)




/***************************************************************************************************
 *	[Enumeration & TypeDeclarations]
 **************************************************************************************************/

// motor driving mode (=���� ���� ���) ---------------------------------------
typedef enum
{
	MTR_MODE_CLOSEDLOOP_HALL = 0,	// 0:
	MTR_MODE_OPENLOOP_TORQUE,		// 1:
	MTR_MODE_OPENLOOP_LANGLEVOLT,	// 2:
	MTR_MODE_OPENLOOP_CURRENT,		// 3:
	MTR_MODE_STOP_OPEN,				// 4:
	MTR_MODE_STOP_BRAKE,				// 5:
	MTR_MODE_TEST_FREQUENCYVOLT,		// 6:
	MTR_MODE_TEST_FREQUENCYCURRENT,	// 7:
	MTR_MODE_END
} EMcCtrlMode;

#define MTR_MODE_CLOSEDLOOP_SENSORLESS		MTR_MODE_CLOSEDLOOP_HALL
#define MTR_MODE_CLOSEDLOOP_NOLOAD			MTR_MODE_CLOSEDLOOP_HALL

// controller variable (=����� ����) -----------------------------
typedef enum
{
	MTR_VAR_THETA_TEST = 0,	
	MTR_VAR_AUTO_ENCODER_OFFSET,
	MTR_VAR_MANU_ENCODER_OFFSET,
	MTR_VAR_INIT_POSIT,
	MTR_VAR_KPI_SC
} EMcVariable;

// motor failure event (=���� ���� �̺�Ʈ) ---------------------------------------
typedef enum
{
	MTR_FAIL_NON = 0,
	MTR_FAIL_STARTUP,					// 1: Motor start failure (=���� �⵿ ����)
	MTR_FAIL_OVERLOAD,					// 2: Overload, delay in detecting hall signals (=�� ����, Ȧ��ȣ ���� ����)
	MTR_FAIL_HALLSENSOR,				// 3: Hall sensor defective (=Ȧ ���� �ҷ�)
	MTR_FAIL_ALIGN,						// 4: Motor misalignment (=���� ���� �ҷ�) 
	MTR_FAIL_SPEED,						// 5: Excessive deviation between target speed and measured speed (=��ǥ�ӵ��� �����ӵ� ���� ����)
	MTR_FAIL_OVERCURRENT,				// 6: Overcurrent detection status (=������ ���� ����)
	MTR_FAIL_ADCOFFSET,					// 7: Current sensing ADC OFFSET error (=���� ���� ADC OFFSET ����)
	MTR_FAIL_IPMFAULT,					// 8: IPM overcurrent detection (=IPM ������ ����)
	MTR_FAIL_OVERVOLT,					// 9: high voltage (=�� ����)
	MTR_FAIL_UNDERVOLT,					// A: Low voltage (=�� ����)
	MTR_FAIL_PROTECTED_VAL,				// Standard protect error (=�԰� ������Ʈ ����)
	MTR_FAIL_END   
} EMcFailEvent;

/***************************************************************************************************
 *	[motor control PARAMETER]
 **************************************************************************************************/
   typedef enum
 {
	 PULSATOR_PARAMETER_INDEX,
	 BASKET_PARAMETER_INDEX,
	 PARAMETER_TOTAL_NUM,
 }parameter_en_type_def;
 typedef struct
 {
	float T_ALIGN_id;
	float T_MAX_STARTUP_id;
	float I_ALIGN_id;
	float I_STARTUP_INIT_id;
	float I_STARTUP_MAX_id;
	float JM_SC_id;
	float KP_SC_ROLL_id;
	float KI_SC_ROLL_id;
	float KA_SC_ROLL_id;
	float KP_SC_id;
	float KI_SC_id;
	float KA_SC_id;
 }motor_parameter_TypeDef;


/***************************************************************************************************
 *	[Function Declaration]
 **************************************************************************************************/





PUBLIC void DevInitMotor(void);
PUBLIC void DevMainLoopMotor(void);

PUBLIC void DevSetMotorDrive(EMcCtrlMode xeMode, Q15 xqParam1, Q15 xqParam2);
PUBLIC void ChangeMcVariable(EMcVariable xeVar, Q15 *xpqVar);
PUBLIC WORD DevSetMotorSensing(WORD xwSensId, Q15 xqParamSens);

PUBLIC void DevSetMotorIPMFaultDisableTime(WORD xwBlockingTime);
PUBLIC EMcFailEvent DevGetMotorIPMFaultState (void);

#ifndef _MTR_VOLT_FAIL_CHECK_MASK_
PUBLIC void DevSetMotorUnderVoltControlMask(BOOLEAN xbOnOff);
PUBLIC EMcFailEvent DevGetMotorVoltFailState (void);
#endif

PUBLIC S16 DevGetMotorSensData(WORD xuwSensId);
/*
#define DevGetMotorSensData(xubSensId)		(S16)(((xubSensId) == MTR_SENS_UNBALREAL) ? (snMtrSensUnbalReal) :\
											(((xubSensId) == MTR_SENS_TORQSLOPE) ? (snMtrSensTorqSlope) :\
											(((xubSensId) == MTR_SENS_TORQLOAD) ? (snMtrSensTorqLoad) :\
											(snMtrSensAppData))))
*/
Q15 qVdcBus_get (void);
Q15 qWrpmActual_get (void);
Q15 qThetaVsLead_get (void);
Q15 qVsRefCC_get (void);
Q15 qIs_get (void);		
Q15 qPowerIn_get (void);
DWORD dwPwmTime_get (void);
short nRevolCnt_get (void);
WORD wMcSensId_get (void);
WORD wMcSensEvent_get (void);
S16 snMtrSensUnbalReal_get (void);
S16 snMtrSensTorqSlope_get (void);
S16 snMtrSensTorqLoad_get (void);
S16 snMtrSensAppData_get (void);
void oObsHall_get(TObserver *oObsHall_cpy);
void oAngle_get(TAngle *oAngle_cpy);
 void oAngle_set(TAngle *oAngle_cpy);
void oIdqse_get(TAxisDQ *oIdqse_cpy);
void eMcFailEvent_get(EMcFailEvent *eMcFailEvent_cpy);
void eMcCtrlMode_get(EMcCtrlMode *eMcCtrlMode_cpy);
void oRampWrpm_get(TRampRef *oRampWrpm_cpy);

void parameter_index_set(parameter_en_type_def parameter_index);
/***************************************************************************************************
 *	[End Of Header File]
 **************************************************************************************************/
#endif