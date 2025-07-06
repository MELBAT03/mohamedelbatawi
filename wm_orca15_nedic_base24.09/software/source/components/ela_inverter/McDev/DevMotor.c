/**===========================================================================
 * @file		DevMotor.c
==============================================================================*/


/***************************************************************************************************
 *	[Include File]
 **************************************************************************************************/
#include "../McApp/McProject.h"

#include "../McUtil/McTarget.h"
#include "../McDev/DevMotor.h"
#include "../McUtil/McUtil.h"
#include "../McUtil/DevPeripheral.h"
#include "../core/tmpm370/inc/tmpm370_pmd.h"
#include "../core/tmpm370/inc/tmpm370_gpio.h"
//#include "../os/inc/mc_perf.h"
#include "../app/config/def.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "MC_config.h"

/***************************************************************************************************
 *	[Enumeration & TypeDeclarations]
 **************************************************************************************************/

/***************************************************************************************************
 *	[Constant & Variable Declarations]
 **************************************************************************************************/



TAxisABC	oIabcs;
TAxisDQ		oIdqss;
//TAxisDQ		oIdqse;

TAxisDQ		oIdqseRef;

TAxisDQ		oVdqse;
TAxisDQ		oVdqss;
TAxisABC	oVabcs;

TAngle		oAngle;

TLpf1_Q15	oLpfIdse;
TLpf1_Q15	oLpfIqse;
TLpf1_Q15	oLpfVdc;
TLpf1_Q15	oLpfPower;
// Q15 		qVdcBus;
// Q15 		qVsRefCC;

// Q15 		qWrpmActual;
// TRampRef	oRampWrpm;

TCtrlPIL	oCtrlPiFw;
TCtrlPIA	oCtrlPiSpd;
TCtrlPIA	oCtrlPiIde;
TCtrlPIA	oCtrlPiIqe;

WORD wPwmSector;
WORD wReturnADC;

WORD wMcFlag;
// EMcCtrlMode eMcCtrlMode;

Q15 		qCtrlParam1;
Q15 		qCtrlParam2;

WORD wFailFlagInt;
WORD wFailFlagBridge;
WORD wFailFlagLoop;

// EMcFailEvent eMcFailEvent;
WORD wAlignFailCnt;
WORD wSpeedFailCnt;
WORD wOverLoadFailCnt;
#ifndef _MTR_VOLT_FAIL_CHECK_MASK_
WORD wPowerFailCnt;
#endif
WORD wIpmFoFailCnt;
WORD wIpmFoBlockingTmr;
int nOverCurrentFailCnt;
TLpf1_Q15	oLpfIerr;

void (*fpDoTsState)(void);		// Control cycle call function pointer for each motor operation status (=���� �ֱ� ���Ϳ��� ���º� ȣ�� �Լ� ������)

TRampRef	oStateRamp1;
TRampRef	oStateRamp2;
TLpf1_Q15	oLpfState;
Q15 qStateTheta;
Q15 qStateVal;
long qdStateInteg;
WORD wStateTmBuf;
WORD wStateIndex;
WORD wStateCnt;
long nStateGain;
long lStateSum;

short qRevolPosit;
//short nRevolCnt;
//WORD wMcSensId;
WORD wSensUserId;
Q15 qAppSensMem;

#if defined(_EST_HALL_OBS_)
//extern const Q15 caqThetaHall[];
#endif

#if defined(_EST_SENSORLESS_) && defined(_EST_ENCODER_) && defined(_MOTOR_PARAM_)
WORD	wFlagEstEnc;
#endif
#if defined(_ACT_SENSORLESS_)
TEstimate	oEstimate;
#endif
#if defined(_EST_HALL_OBS_)
TObserver oObsHall ;
#endif
#if defined(_EST_HALL_MT_)
TMeasureMT oMeasureMT;
#endif
#if defined(_EST_ENCODER_)
TObserver	oObsEncoder;
Q15 qThetaOffsetEncoder;
#endif


TRampRef		oRampWrpm;
EMcCtrlMode 	eMcCtrlMode;
EMcFailEvent 	eMcFailEvent;
TAxisDQ		oIdqse;
Q15 			qVdcBus;
Q15 			qWrpmActual;
Q15 			qThetaVsLead;
Q15 			qVsRefCC;
Q15 			qIs;
Q15 			qPowerIn;
DWORD 			dwPwmTime;
short 			nRevolCnt;
WORD 			wMcSensId;
WORD 			wMcSensEvent;
S16			snMtrSensUnbalReal;
S16			snMtrSensTorqSlope;
S16			snMtrSensTorqLoad;
S16			snMtrSensAppData;

motor_parameter_TypeDef motor_parameter[PARAMETER_TOTAL_NUM] ={0};
parameter_en_type_def parameter_index_id = PULSATOR_PARAMETER_INDEX;
/***************************************************************************************************
 *	[Function Declaration]
 **************************************************************************************************/

void DoCheckFailLoop(void);
void DoCheckFailPwm(void);

void EntryStateStop(void);
void DoStateStop(void);
void EntryStateReady(void);
void DoStateReady(void);
void EntryStateInject(void);
void DoStateInject(void);
void EntryStateAlign(void);
void DoStateAlign(void);
void EntryStateStartup(void);
void DoStateStartup(void);
void EntryStateRun(void);
void DoStateRun(void);
void EntryStateTest(void);
void DoStateTest(void);
#if defined(_EST_ENCODER_) && !defined(_NON_ENCODER_Z_)
void EntryStateEncoderOffset(void);
void DoStateEncoderOffset(void);
#endif 
U08 numtostr(U08 str[],U08 start_index, U08 end_type, int num );
// void EntryStateInitPosit(void);
// void DoStateInitPosit(void);


void DoMcSensingISR(void);
void SetSensUnbalReal(BOOLEAN xOnOff);
void SetSensTorqueSlope(BOOLEAN xOnOff);
void SetSensTorqueLoad(BOOLEAN xOnOff);
void SetSensApplied(WORD xwSensId, Q15 xqParamSens);
void motor_parameter_set(void);
#if defined(_APP_WASHING_MACHINE_)
#define DoPwmApp()
#else
extern void DoPwmApp(void);
#endif

#if defined(_MOTOR_PARAM_)
void UPDATE_MC_GAIN(void)
{
	SET_PIA1_GAIN(oCtrlPiIde, KP_CC_D, KI_CC_D, KA_CC_D, TS_CC, QMAX_CURRENT, QMAX_VOLT);
	SET_PIA1_GAIN(oCtrlPiIqe, KP_CC_Q, KI_CC_Q, KA_CC_Q, TS_CC, QMAX_CURRENT, QMAX_VOLT);
}
void UPDATE_CTRL_GAIN(void)
{
	if (wMcFlag & MCFLAG_SC_ROLL_GAIN)
		SET_PIA1_GAIN(oCtrlPiSpd, KP_SC_ROLL, KI_SC_ROLL, KA_SC_ROLL, TS_SC, QMAX_OMEGA_RD, QMAX_TORQUE);
	else
		SET_PIA1_GAIN(oCtrlPiSpd, KP_SC, KI_SC, KA_SC, TS_SC, QMAX_OMEGA_RD, QMAX_TORQUE);
	SET_PIL1_GAIN(oCtrlPiFw, KP_FWC, KI_FWC, TS_SC, QMAX_VOLT, QMAX_CURRENT);
}
#else
#define UPDATE_MC_GAIN()
#define UPDATE_CTRL_GAIN()
#endif

/***************************************************************************************************
Name:        DevInitMotor   
Description:  
***************************************************************************************************/
PUBLIC void DevInitMotor(void)
{
	mc_util_disable_mc_interrupts();			// interrupt Disable
	INIT_MC_PARAM();
	UPDATE_UTIL_GAIN();
	UPDATE_MC_GAIN();
	UPDATE_CTRL_GAIN();

	// Clear Fail flag (=Fail �÷��� Ŭ����) 
	wFailFlagInt = 0;
	wFailFlagBridge = 0;
	wFailFlagLoop = 0;
	
	INIT_LPF1(oLpfIdse, Q_CURRENT(0.0), WC_LPF_IDQ, TS_CC);
	INIT_LPF1(oLpfIqse, Q_CURRENT(0.0), WC_LPF_IDQ, TS_CC);
	INIT_LPF1(oLpfPower, Q_POWER(0.0), (2.0 * PI * 10.0), TS_CC);

#ifdef _MTR_VOLT_FAIL_CHECK_MASK_
	INIT_LPF1(oLpfVdc, Q_VOLT(310.0), WC_LPF_VDC, TS_LPF_VDC);
	qVdcBus = Q_VOLT(310.0);
#else
	INIT_LPF1(oLpfVdc, Q_VOLT(10.0), WC_LPF_VDC, TS_LPF_VDC);
	qVdcBus = Q_VOLT(10.0);

	wFailFlagInt |= FAILFLAG_UNDER_VOLT;
	wFailFlagBridge |= FAILFLAG_UNDER_VOLT;
	wFailFlagLoop |= FAILFLAG_UNDER_VOLT;

#endif
	motor_parameter_set();
	parameter_index_set(PULSATOR_PARAMETER_INDEX);
	// Current controller initialization (=���� ����� �ʱ�ȭ)
	SET_PIA1_GAIN(oCtrlPiIde, KP_CC_D, KI_CC_D, KA_CC_D, TS_CC, QMAX_CURRENT, QMAX_VOLT);
	SET_PIA1_GAIN(oCtrlPiIqe, KP_CC_Q, KI_CC_Q, KA_CC_Q, TS_CC, QMAX_CURRENT, QMAX_VOLT);

	// Speed, field weakening controller initialization (=�ӵ�, ����� ����� �ʱ�ȭ) 
	SET_PIA1_GAIN(oCtrlPiSpd, (motor_parameter[parameter_index_id].KP_SC_id), (motor_parameter[parameter_index_id].KI_SC_id), (motor_parameter[parameter_index_id].KA_SC_id), TS_SC, QMAX_OMEGA_RD, QMAX_TORQUE);
	oCtrlPiSpd.qVff = 0;
	oCtrlPiSpd.qdInteg = 0;

	SET_PIL1_GAIN(oCtrlPiFw, KP_FWC, KI_FWC, TS_SC, QMAX_VOLT, QMAX_CURRENT);
	oCtrlPiFw.qLimP = 0;
	oCtrlPiFw.qLimN = - Q_CURRENT(ID_FW_LIMIT);
	oCtrlPiFw.qdInteg = 0;

	mc_util_set_config_peripheral ();			// Motor control SFR initialization (=�������� SFR �ʱ�ȭ)

#if !defined(_DISABLE_EMG_)
	if (READ_PORT_IPM_FO())
	{
		ENABLE_EMG();					// EMG Protection Relrase & Enable
	}
	else
	{
		wFailFlagInt |= FAILFLAG_IPM_FO;
		wFailFlagBridge |= FAILFLAG_IPM_FO;
	}
#endif
	wIpmFoFailCnt = 0;

	// Position & Velocity Estimator Initialization (=��ġ & �ӵ� ������ �ʱ�ȭ) 
#if defined(_EST_HALL_OBS_)
    Q15 pArray =getCaqThetaHallPointer(read_port_hall());// Get the pointer to the array
	InitObserver(&oObsHall, pArray);
	oAngle.qTheta = oObsHall.oAngle.qTheta;
	oAngle.qCos = oObsHall.oAngle.qCos;
	oAngle.qSin = oObsHall.oAngle.qSin;
#endif
#if defined(_EST_HALL_MT_)
	{
		WORD twHall;
		PORT_DEBOUNCE_READER(read_port_hall(), twHall, 5);
		InitMeasureMT(&oMeasureMT, twHall);
  #if !defined(_EST_HALL_OBS_)
		oAngle.qTheta = oMeasureMT.oAngle.qTheta;
		oAngle.qCos = oMeasureMT.oAngle.qCos;
		oAngle.qSin = oMeasureMT.oAngle.qSin;		
  #endif
	}
#elif !defined(_EST_HALL_OBS_)
	SetAngle(&oAngle, Q_THETA_R(0.0));
#endif
#if defined(_EST_ENCODER_)
	{
		qThetaOffsetEncoder = Q_THETA_R(0.0);
		InitObserver(&oObsEncoder, Q_THETA_R(0.0));
	}
#endif

#if defined(_ACT_SENSORLESS_)
	InitEstimator(&oEstimate, oAngle.qTheta);
	SetEstFlag(&oEstimate, SLFLAG_WREF_NON);
#endif
	qWrpmActual = Q_OMEGA_RPM(0.0);	

	
	// Control status initialization (������� �ʱ�ȭ)
	qCtrlParam1 = 0;
	qCtrlParam2 = 0;
	eMcCtrlMode = MTR_MODE_STOP_BRAKE;
	SetRampTarget(&oRampWrpm, Q_OMEGA_RPM(0.0), -1);
	EntryStateStop();
#if defined(_EST_ENCODER_) && !defined(_NON_ENCODER_Z_)
//	EntryStateEncoderOffset();
#endif

	INIT_DBG_PORT(); 		//DBG Port setting
	
	mc_util_interrupt_priority_set();			// Interrupt Priority Set, Pending Clear & Enable
	mc_util_software_debug();
//	DevSetMotorIPMFaultDisableTime(0xFFFF);	//DBG
//	DevSetMotorUnderVoltControlMask(1);
	
}

/***************************************************************************************************
Name:        ISR_MCPWM_ADC   
Description:  
***************************************************************************************************/
extern WORD wDacData1;
short qThetaDbgView;	//DBG
Q15 qWrpmDbgView;
Q15 nCntDbg;	//DBG
Q15 nCntDbg1;	//DBG
int nCntTOver;

INTERRUPT ISR_MCPWM_ADC(void)
{
//  MC_Perf_Measure_Start(Task_measurmet_get() , MEASURE_TSK_HighFrequencyTask);
//  SET_PWM_DBG_PORT(HI);		//DBG 

	Q15 tqTemp;
	Q15 tqValErr;
	Q15 tqValLim;
	TAxisDQ	toValDQ;
	
	CPU_ENTER_INTERRUPT();
	
	// Phase current & DC BUS voltage acquisition from AD (=������ & DC BUS ���� ȹ�� from AD) ------------------
	tqTemp = wPwmSector;
	if (wMcFlag & MCFLAG_AD_OFFSET) tqTemp |= ADC_OFFSET_FLAG_DO;
	wReturnADC = ReadADC((WORD)tqTemp, &oIabcs, &tqTemp);
	if (WC_LPF_VDC <= 0.0) qVdcBus = tqTemp;
	else DO_LPF1(oLpfVdc, tqTemp, qVdcBus);
	ConvABC2DQS(&oIabcs, &oIdqss);

	// Magnetic flux position & velocity estimator (=�ڼ� ��ġ & �ӵ� ������) ---------------------------
#if defined(_EST_SENSORLESS_) && defined(_EST_ENCODER_) && defined(_MOTOR_PARAM_)
	if (wFlagEstEnc == 0)
	{
		DoEstimate(&oEstimate, &oIdqss, &oVdqss);
		oAngle.qTheta = oEstimate.oAngle.qTheta;
		oAngle.qCos = oEstimate.oAngle.qCos;
		oAngle.qSin = oEstimate.oAngle.qSin;
//		if (abs(oEstimate.qWrpmLpf) < Q_OMEGA_RPM_DRUM(5.0)) qWrpmActual = Q_OMEGA_RPM_DRUM(0.0);
//		else qWrpmActual = oEstimate.qWrpmLpf;
		qWrpmActual = oEstimate.qWrpmLpf;
	}
	else
	{
		DoObserver(&oObsEncoder, GET_THETA_ENCODER() + qThetaOffsetEncoder);
		oAngle.qTheta = oObsEncoder.oAngle.qTheta;
		oAngle.qCos = oObsEncoder.oAngle.qCos;
		oAngle.qSin = oObsEncoder.oAngle.qSin;
		qWrpmActual = oObsEncoder.qWrpmLpf;		
	}

#elif defined(_EST_SENSORLESS_)	
	DoEstimate(&oEstimate, &oIdqss, &oVdqss);
	oAngle.qTheta = oEstimate.oAngle.qTheta;
	oAngle.qCos = oEstimate.oAngle.qCos;
	oAngle.qSin = oEstimate.oAngle.qSin;
//	if (abs(oEstimate.qWrpmLpf) < Q_OMEGA_RPM_DRUM(5.0)) qWrpmActual = Q_OMEGA_RPM_DRUM(0.0);
//	else qWrpmActual = oEstimate.qWrpmLpf;
	qWrpmActual = oEstimate.qWrpmLpf;
	
#elif defined(_EST_HALL_OBS_)
    Q15 pArray = getCaqThetaHallPointer(read_port_hall());// Get the pointer to the array
	DoObserver(&oObsHall, pArray);
	oAngle.qTheta = oObsHall.oAngle.qTheta;
	oAngle.qCos = oObsHall.oAngle.qCos;
	oAngle.qSin = oObsHall.oAngle.qSin;
	qWrpmActual = oObsHall.qWrpmLpf;
	
#elif defined(_EST_HALL_MT_)
	DoMeasureMT(&oMeasureMT);
	oAngle.qTheta = oMeasureMT.oAngle.qTheta;
	oAngle.qCos = oMeasureMT.oAngle.qCos;
	oAngle.qSin = oMeasureMT.oAngle.qSin;
	qWrpmActual = oMeasureMT.qWrpmLpf;
#elif defined(_EST_ENCODER_)
	DoObserver(&oObsEncoder, GET_THETA_ENCODER() + qThetaOffsetEncoder);
	oAngle.qTheta = oObsEncoder.oAngle.qTheta;
	oAngle.qCos = oObsEncoder.oAngle.qCos;
	oAngle.qSin = oObsEncoder.oAngle.qSin;
	qWrpmActual = oObsEncoder.qWrpmLpf;
#else
 #error "Please select Estimat mathode (in McProject.h file)"
#endif

	// Measurement current synchronization coordinate conversion and LPF processing (=�������� ������ǥ ��ȯ �� LPFó��) ----------------------
	if (WC_LPF_IDQ <= 0.0)
	{
		ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	//DBG Sensing Current LPF not processed (=(�������� LPF ó�� ���� ))
	}
	else
	{
		ConvDQS2DQE(&oIdqss, &toValDQ, &oAngle);
		DO_LPF1(oLpfIdse, toValDQ.qD, oIdqse.qD);
		DO_LPF1(oLpfIqse, toValDQ.qQ, oIdqse.qQ);
	}

	// Check motor control failure and call control function by status (=�������� ���� üũ �� ���º� �����Լ� ȣ��) -----------------
//	DoCheckFailPwm();	// Motor control failure check (=���� ���� ���� üũ) 
	fpDoTsState();			// Call control function by state (=���º� �����Լ� ȣ��) 

	// Field weakening & speed control (=����� & �ӵ� ����) ----------------------------------------
	if (wMcFlag & MCFLAG_FWC_ON)		// Field weakening control (=����� ����) ;;;;;;;;;;;;;
	{
		tqValLim = MUL_SS(Q15_FRAC(K_FW_V_REF / SQRT3), qVdcBus) >> 15;
		if (tqValLim > Q_VOLT(VS_LIMIT)) tqValLim = Q_VOLT(VS_LIMIT);
		tqValErr = tqValLim - qVsRefCC;
		if ((tqValErr < 0) && (MUL_SS(qWrpmActual, oVdqse.qQ) <= 0)) 	//Voltage & Lead Angle Exceeded(=���� & Lead Angle �ʰ�)
		{
#if !defined(_NON_CC_)
			tqValErr = (-tqValErr) >>2;					// Phase voltage phase limited to 180 degrees (=������ ���� 180�� ����) //
#endif
		}
		oIdqseRef.qD = DoCtrlPIL1(&oCtrlPiFw, tqValErr); 	// Field weakening controller (=����� �����) -> IdseRef

		if (wMcFlag & MCFLAG_SC_ON)		// speed control (=�ӵ� ����) ;;;;;;;;;;;;;;;
		{
			DoRampRef(&oRampWrpm);					// Command speed acceleration/deceleration (=���ɼӵ� ������) 

			/* // Cancellation of field weakening control in low speed operation mode (=���� ������忡�� ����� ���� ���) */
			if (wMcFlag & MCFLAG_SC_ROLL_GAIN)		
			{
				oIdqseRef.qD = Q_CURRENT(0.0);
				oCtrlPiFw.qdInteg = 0;
			}	
			/* // Orignal Iq Limit
			oCtrlPiSpd.qLimP = SqrtS32(((long)Q_CURRENT(IS_LIMIT) * Q_CURRENT(IS_LIMIT)) - MUL_SS(oIdqseRef.qD, oIdqseRef.qD));*/
			
			/* // IsLim calculation based on speed and Vs (=�ӵ� �� Vs�� ���� IsLim ���),  IsLim'=IdBase+70*VsLim^2/(Wr^2+200000) */
			
			/* // Is Limit linear reduction based on IdRef (=IdRef�� ���� Is Limit ���Ͼ� ����), IsLim'=IsLim+K*IdRef  */
			#define Q_K_ISID_LIM		(Q15)(K_ISID_LIM * (1L << 13) * (Q_CURRENT(IS_LIMIT) - Q_CURRENT(ID_FW_LIMIT)) / Q_CURRENT(ID_FW_LIMIT))
			tqValLim = Q_CURRENT(IS_LIMIT) + (MUL_SS(oIdqseRef.qD, Q_K_ISID_LIM) >> 13);
			oCtrlPiSpd.qLimP = SqrtS32(((long)tqValLim * tqValLim) - MUL_SS(oIdqseRef.qD, oIdqseRef.qD));
			
			/* // IqLim calculation based on speed and Vs, IqLim=K*Vs/(Wr+m) (division operation) (=�ӵ� �� Vs�� ���� IqLim ���, IqLim=K*Vs/(Wr+m) (������ ���� ����..))
			#define K_IQ_LIM		64.0
			#define Q_K_IQ_LIM		(K_IQ_LIM * 32768. * QMAX_VOLT / (QMAX_CURRENT * QMAX_OMEGA_R))
			if (Q_K_IQ_LIM > ((1L << 30) / Q_VOLT(VS_LIMIT))) oCtrlPiSpd.qLimP = (1L << 30);
			else oCtrlPiSpd.qLimP = DIV_UD(MUL_UU(Q_K_IQ_LIM, tqValLim), (abs(qWrpmActual) + 512));
			tqValLim = SqrtS32(((long)Q_CURRENT(IS_LIMIT) * Q_CURRENT(IS_LIMIT)) - MUL_SS(oIdqseRef.qD, oIdqseRef.qD));
			if (oCtrlPiSpd.qLimP > tqValLim) oCtrlPiSpd.qLimP = tqValLim;*/
			
			// oCtrlPiSpd.qLimP = Q_CURRENT(3.5);			//DBG
			oCtrlPiSpd.qLimN = - oCtrlPiSpd.qLimP;
			// nCntDbg = oCtrlPiSpd.qLimP;					//DBG

			tqValErr = oRampWrpm.qRef - qWrpmActual;
			tqTemp = DoCtrlPIA1(&oCtrlPiSpd, tqValErr);		// speed controller (=�ӵ� �����) -> tqTemp

			CtrlMTPA(tqTemp, &toValDQ);				// MTPA control (=MTPA ����) 
#if defined(_EST_SENSORLESS_)						/* // Minimum current injection to prevent sensorless estimation errors (=�������� �������� �������� �ּ����� ����)  */
			if ((Q_CURRENT(IS_MIN) != 0) && (oIdqseRef.qD == Q_CURRENT(0.0)) && (abs(tqTemp) < Q_CURRENT(IS_MIN)))
			{
				toValDQ.qQ = tqTemp;
				toValDQ.qD = -SqrtS32(((long)Q_CURRENT(IS_MIN) * Q_CURRENT(IS_MIN)) - MUL_SS(tqTemp, tqTemp));
			}  
#endif

			oIdqseRef.qD += toValDQ.qD;
			oIdqseRef.qQ = toValDQ.qQ;

#if !defined(_NON_MTPA_)
		/*	{	 // Recheck current limit after MTPA (=MTPA�� �������� ��Ȯ��)
				long tlLimId = ((long)tqValLim * tqValLim) - MUL_SS(oIdqseRef.qQ, oIdqseRef.qQ);
			//	long tlLimId = ((long)Q_CURRENT(IS_LIMIT) * Q_CURRENT(IS_LIMIT)) - MUL_SS(oIdqseRef.qQ, oIdqseRef.qQ);
				if (MUL_SS(oIdqseRef.qD, oIdqseRef.qD) > tlLimId) 
					oIdqseRef.qD = -SqrtS32(tlLimId); // SqrtS32(tlLimId) * SIGN(oIdqseRef.qD);
			}	*/ 
#endif
 		}	
	}
	
	// current control (=���� ����) -------------------------------------------------------------
	if (wMcFlag & MCFLAG_CC_ON)	
	{
#if defined(_NON_CC_)					// Voltage estimation control mode (=���� ���� ���� ���) ;;;;;;;;;;;;;;
		Q30 tdqVref;
		tdqVref = MUL_SS(Q_VEQ_RS, oIdqseRef.qD) - MUL_SS(Q_VEQ_LQ, (MUL_SS(oIdqseRef.qQ, qWrpmActual) >> 15));
		oCtrlPiIde.qVref = (Q15)(tdqVref >> SCALE_VEQ);
		tdqVref = MUL_SS(Q_VEQ_RS, oIdqseRef.qQ) + MUL_SS(Q_VEQ_LD, (MUL_SS(oIdqseRef.qD, qWrpmActual) >> 15))
				  + MUL_SS(Q_VEQ_KE, qWrpmActual);
		oCtrlPiIqe.qVref = (Q15)(tdqVref >> SCALE_VEQ);

		tqValLim = MUL_SS(Q15_FRAC(K_OVER_MODULATION / SQRT3), qVdcBus) >> 15;
		oVdqse.qD = LIMIT(oCtrlPiIde.qVref, -tqValLim, tqValLim);
		tqValLim = SqrtS32(MUL_SS(tqValLim, tqValLim) - MUL_SS(oVdqse.qD, oVdqse.qD));
		oVdqse.qQ = LIMIT(oCtrlPiIqe.qVref, -tqValLim, tqValLim);
#else									// current control (=���� ����) ;;;;;;;;;;;;;;;;;;;;;;;
#if !defined(_NON_CC_VFF_)
//		if (wMcFlag & MCFLAG_CC_VFF)	// Calculate voltage forward compensation value (=���� ���⺸�� �� ���)
		{	/*   */
			Q30 tdqVff;
			tdqVff = MUL_SS(qWrpmActual, oIdqse.qQ) >> 15;
			tdqVff = MUL_SS(tdqVff, -Q_VEQ_LQ);
			oCtrlPiIde.qVff = (Q15)(tdqVff >> SCALE_VEQ);

			tdqVff = MUL_SS(qWrpmActual, oIdqse.qD) >> 15;
			tdqVff = MUL_SS(tdqVff, Q_VEQ_LD);
			tdqVff += MUL_SS(qWrpmActual, Q_VEQ_KE);
			oCtrlPiIqe.qVff = (Q15)(tdqVff >> SCALE_VEQ); 
			
			/* Q30 tdqVref;
			tdqVref = MUL_SS(Q_VEQ_RS, oIdqseRef.qD) - MUL_SS(Q_VEQ_LQ, (MUL_SS(oIdqseRef.qQ, qWrpmActual) >> 15));
			oCtrlPiIde.qVff = (Q15)(tdqVref >> SCALE_VEQ);
			tdqVref = MUL_SS(Q_VEQ_RS, oIdqseRef.qQ) + MUL_SS(Q_VEQ_LD, (MUL_SS(oIdqseRef.qD, qWrpmActual) >> 15))
					  + MUL_SS(Q_VEQ_KE, qWrpmActual);
			oCtrlPiIqe.qVff = (Q15)(tdqVref >> SCALE_VEQ); */
		}	
#else
//		else
		{
			oCtrlPiIde.qVff = Q_VOLT(0.0);
			oCtrlPiIqe.qVff = Q_VOLT(0.0);
		}   
#endif
		// D/Q axis current control: D axis voltage priority overmodulation limit (=D/Q�� ���� ���� : D������ �켱 ������ ����) 
		tqValLim = MUL_SS(Q15_FRAC(K_OVER_MODULATION / SQRT3), qVdcBus) >> 15;	// D�� �������� = Vdc/sqrt(3)
		oCtrlPiIde.qLimP = tqValLim;
		oCtrlPiIde.qLimN = - tqValLim;
		oVdqse.qD = DoCtrlPIA1(&oCtrlPiIde, oIdqseRef.qD - oIdqse.qD);			// D-axis current control (=D�� ��������)

		tqValLim = SqrtS32(MUL_SS(tqValLim, tqValLim) - MUL_SS(oVdqse.qD, oVdqse.qD));// Q axis voltage limit (=Q�� ��������)  
		oCtrlPiIqe.qLimP = tqValLim;
		oCtrlPiIqe.qLimN = - tqValLim;
		oVdqse.qQ = DoCtrlPIA1(&oCtrlPiIqe, oIdqseRef.qQ - oIdqse.qQ); 			// Q-axis current control (=Q�� ��������) /* */
		
/*		// D/Q axis current control: Atan (Vd, Vq) phase maintenance overmodulation limit (=D/Q�� ���� ���� : Atan(Vd, Vq) �������� ������ ����) 
		oVdqse.qD = DoCtrlPIA1_Ref(&oCtrlPiIde, oIdqseRef.qD - oIdqse.qD);
		oVdqse.qQ = DoCtrlPIA1_Ref(&oCtrlPiIqe, oIdqseRef.qQ - oIdqse.qQ);
		tqTemp = MUL_SS(Q15_FRAC(K_OVER_MODULATION / SQRT3), qVdcBus) >> 15;
		tqValLim = SqrtS32(MUL_SS(oVdqse.qD, oVdqse.qD) + MUL_SS(oVdqse.qQ, oVdqse.qQ));
		if (tqValLim > tqTemp)
		{
			TAngle toVs;
			SetAngle(&toVs, ATan2(oCtrlPiIde.qVref, oCtrlPiIqe.qVref));
			tqValLim = abs(MUL_SS(toVs.qCos, tqTemp) >>15);
			oCtrlPiIde.qLimP = tqValLim;
			oCtrlPiIde.qLimN = - tqValLim;	
			oVdqse.qD = DoCtrlPIA1_Lim(&oCtrlPiIde);

			tqValLim = abs(MUL_SS(toVs.qSin, tqTemp) >>15);
			oCtrlPiIqe.qLimP = tqValLim;
			oCtrlPiIqe.qLimN = - tqValLim;	
			oVdqse.qQ = DoCtrlPIA1_Lim(&oCtrlPiIqe);
		}
*/
#endif
	}	
	
	// Command voltage fixed coordinate system conversion (stator DQ axis voltage temporary variable storage) and PWM output setting (=�������� ������ǥ�� ��ȯ(������ DQ�� ���� �ӽú��� ����) �� PWM ��� ����) ------------
	if (Q_OMEGAINTEG_T_PWMCOMP != 0)	// PWM output delay compensation (=PWM ��� ���� ����) 
	{
		static long qdPwmCompInteg;
		TAngle toAngleEst;
		qdPwmCompInteg += MUL_SS(qWrpmActual, Q_OMEGAINTEG_T_PWMCOMP);
		SetAngle(&toAngleEst, oAngle.qTheta + (qdPwmCompInteg >>SCALE_OMEGAINTEG));
		//DBG nCntDbg = (qdPwmCompInteg >>SCALE_OMEGAINTEG);	
		qdPwmCompInteg &= ((1L << SCALE_OMEGAINTEG) -1);
		ConvDQE2DQS(&oVdqse, &toValDQ, &toAngleEst);
	}  
	else
	{
		ConvDQE2DQS(&oVdqse, &toValDQ, &oAngle);
	}
	ConvDQS2ABC(&toValDQ, &oVabcs);	
	
	if (wMcFlag & MCFLAG_PWM_OFF) wPwmSector = GenPWM(&oVabcs, -1);
	else if (wMcFlag & MCFLAG_PWM_ZERO) wPwmSector = GenPWM(&oVabcs, 0);
	else 
	{
	#if !defined(_DEAD_TIME_COMPEN_T_) && defined(_DEAD_TIME_COMPEN_V_)	// Dead Time Rewards (=Dead Time ����) 
		Q15 tqVdelta;
		TAxisABC toVabc;
		#define Q_K_DT_COMPEN		Q15_FRAC(6.0 * QMAX_CURRENT / QMAX_VOLT)

		tqVdelta = MUL_SS(qVdcBus, Q15_FRAC(T_DEAD_COMPEN * FREQ_PWM)) >> 15;
		
		toVabc.qA = oVabcs.qA + LIMIT((MUL_SS(oIabcs.qA, Q_K_DT_COMPEN) >>15), -tqVdelta, tqVdelta);
		toVabc.qB = oVabcs.qB + LIMIT((MUL_SS(oIabcs.qB, Q_K_DT_COMPEN) >>15), -tqVdelta, tqVdelta);
		toVabc.qC = oVabcs.qC + LIMIT((MUL_SS(oIabcs.qC, Q_K_DT_COMPEN) >>15), -tqVdelta, tqVdelta);
		wPwmSector = GenPWM(&toVabc,qVdcBus); 

	#else
		wPwmSector = GenPWM(&oVabcs,qVdcBus); 
	#endif
	}
	

#if defined(_ONE_SHUNT_)
	if (IS_PWM_OFF_SEQUENCE() == FALSE) nCntTOver++; 
#else
	if (IS_PWM_OFF_SEQUENCE() == TRUE) nCntTOver++; 
#endif
		
	// Sensorless for debugging (previous cycle voltage, current current), Hall sensor magnetic flux angle/velocity estimation (=������ ��������(�����ֱ� ����, ���� ����), Ȧ���� �ڼӰ�/�ӵ� ����) --------
#if defined(_EST_SENSORLESS_)	
  #if defined(_EST_HALL_OBS_)
	DoObserver(&oObsHall, caqThetaHall[read_port_hall()]);
  #endif
  #if defined(_EST_HALL_MT_)
	DoMeasureMT(&oMeasureMT);
  #endif
  #if defined(_EST_ENCODER_)
  	#if defined(_MOTOR_PARAM_)
  	if (wFlagEstEnc != 0)
		DoEstimate(&oEstimate, &oIdqss, &oVdqss);
	else
	#endif
		DoObserver(&oObsEncoder, GET_THETA_ENCODER() + qThetaOffsetEncoder);
  #endif
#elif defined(_EST_HALL_OBS_)
  #if defined(_ACT_SENSORLESS_)
	DoEstimate(&oEstimate, &oIdqss, &oVdqss);
  #endif
  #if defined(_EST_HALL_MT_)
	DoMeasureMT(&oMeasureMT);
  #endif
  #if defined(_EST_ENCODER_)
	DoObserver(&oObsEncoder, GET_THETA_ENCODER() + qThetaOffsetEncoder);
  #endif
#elif defined(_EST_HALL_MT_)
  #if defined(_ACT_SENSORLESS_)
	DoEstimate(&oEstimate, &oIdqss, &oVdqss);
  #endif
  #if defined(_EST_HALL_OBS_)
	DoObserver(&oObsHall, caqThetaHall[read_port_hall()]);
  #endif
  #if defined(_EST_ENCODER_)
	DoObserver(&oObsEncoder, GET_THETA_ENCODER() + qThetaOffsetEncoder);
  #endif
#elif defined(_EST_ENCODER_)
  #if defined(_ACT_SENSORLESS_)
	DoEstimate(&oEstimate, &oIdqss, &oVdqss);
  #endif
  #if defined(_EST_HALL_OBS_)
	DoObserver(&oObsHall, caqThetaHall[read_port_hall()]);
  #endif
  #if defined(_EST_HALL_MT_)
	DoMeasureMT(&oMeasureMT);
  #endif
#endif


#if defined(_ACT_SENSORLESS_)	
  #if defined(_EST_HALL_OBS_)
	qThetaDbgView = (short)(oEstimate.oAngle.qTheta - oObsHall.oAngle.qTheta);
  #elif defined(_EST_HALL_MT_)
	qThetaDbgView = (short)(oEstimate.oAngle.qTheta - oMeasureMT.oAngle.qTheta);
  #elif defined(_EST_ENCODER_)
	qThetaDbgView = (short)(oEstimate.oAngle.qTheta - oObsEncoder.oAngle.qTheta);
  #else
  	qThetaDbgView = (short)(oEstimate.oAngle.qTheta);
  #endif
#else
  #if defined(_EST_HALL_OBS_)
	qThetaDbgView = (short)(oObsHall.oAngle.qTheta);
  #elif defined(_EST_HALL_MT_)
	qThetaDbgView = (short)(oMeasureMT.oAngle.qTheta);
  #elif defined(_EST_ENCODER_)
	qThetaDbgView = (short)(oObsEncoder.oAngle.qTheta);
  #endif
#endif

/* Speed variable for monitor transmission (=����� ���ۿ� �ӵ� ����) */
#if defined(_EST_SENSORLESS_)
  #if defined(_EST_HALL_OBS_)
	qWrpmDbgView = oObsHall.qWrpmLpf;
  #elif defined(_EST_HALL_MT_)
	qWrpmDbgView = oMeasureMT.qWrpmLpf;
   #elif defined(_EST_ENCODER_)
	qWrpmDbgView = oObsEncoder.qWrpmLpf;
  #endif
#else
  #if defined(_ACT_SENSORLESS_)
	qWrpmDbgView = oEstimate.qWrpmLpf;
  #endif
#endif

	// Calculation of phase voltage command value (=������ ���ɰ� ���) --------------------------------------
	if (wMcFlag & MCFLAG_CC_ON)
		qVsRefCC = SqrtS32(MUL_SS(oCtrlPiIde.qVref, oCtrlPiIde.qVref) + MUL_SS(oCtrlPiIqe.qVref, oCtrlPiIqe.qVref));
	else
		qVsRefCC = SqrtS32(MUL_SS(oVdqse.qD, oVdqse.qD) + MUL_SS(oVdqse.qQ, oVdqse.qQ));

	// Voltage phase angle calculation (=���� ���� ���) ----------------------------------------
	qThetaVsLead = ATan2(oVdqse.qD, oVdqse.qQ);

	// Phase current calculation (=������ ���) --------------------------------------
#if !defined(_NON_CC_) || defined(_EST_SENSORLESS_)		// For sensorless estimation or current control (=�������� ���� �Ǵ� ���������)
	qIs = SqrtS32(MUL_SS(oIdqse.qD, oIdqse.qD) + MUL_SS(oIdqse.qQ, oIdqse.qQ));
#else
	qIs = SqrtS32(MUL_SS(oIdqseRef.qD, oIdqseRef.qD) + MUL_SS(oIdqseRef.qQ, oIdqseRef.qQ));
#endif

	// Motor input power calculation (previous cycle voltage * current Current) (=���� �Է����� ��� (�����ֱ� ���� * ���� ����)) ------------------
#if !defined(_NON_CC_) || defined(_EST_SENSORLESS_)
	tqTemp = (MUL_SS(oVdqss.qD, oIdqss.qD) + MUL_SS(oVdqss.qQ, oIdqss.qQ)) >> SCALE_POWER;
#else
	tqTemp = (MUL_SS(oVdqse.qD, oIdqseRef.qD) + MUL_SS(oVdqse.qQ, oIdqseRef.qQ)) >> SCALE_POWER;
#endif
	DO_LPF1(oLpfPower, tqTemp, qPowerIn);
//	qPowerIn = tqTemp;

//	wDacData1 = (WORD)(oVdqss.qD);// (oVabcs.qA); //  - oVabcs.qC); //DBG

	// Stator DQ axis voltage variable update (=������ DQ�� ���к��� ����) --------------------------------------
	oVdqss.qD = toValDQ.qD;	
	oVdqss.qQ = toValDQ.qQ;
	
	dwPwmTime++;
	
	DoCheckFailPwm();	// Motor control failure check // State control function forward when CPU speed is sufficient (=���� ���� ���� üũ // CPU�ӵ� ������ ���������Լ� ������) 
	DoMcSensingISR();		// Motor sensor PWM cycle operation (=���� ���� PWM �ֱ� ����) 
	DoPwmApp();

	CPU_EXIT_INTERRUPT();
//
 #if defined(MC_DEBUG_UART_ENABLED)
     static U16 frame_rate = 0;
 //  static U16 counter_in_ISR=0;
 //  counter_in_ISR++;
 if (frame_rate++ == 400)
 {
   U08  text[50],     size=0;
   //size=numtostr(text,size, ',' ,(Q15)qVdcBus);
   //size=numtostr(text,size, ',' ,(int)current_LPF_test);
//   size=numtostr(text,   size , ','  ,(Q15)((oIabcs.qA)));
//   size=numtostr(text,   size , ','  ,(Q15)((480*qVdcBus) >>15));
   size=numtostr(text,   size , ','  ,(Q15)MOTOR_SPD2RPM(qWrpmActual));
//   size=numtostr(text,   size , ','  ,(Q15)(oRampWrpm.qRef));
//     size=numtostr(text,   size , ','  ,(Q15)(app_drain_state_get()));
//      size=numtostr(text,   size , '\n' ,(Q15)MOTOR_SPD2RPM(qWrpmActual));
//   size=numtostr(text,   size , '\n' ,(Q15)(oRampWrpm.qdInteg));
   size=numtostr(text,   size , '\n' ,(Q15)MOTOR_SPD2RPM(oRampWrpm.qRef));

   frame_rate=SendUart0Tx(text, size);
   //    printf("%d\n",AD15_REG_I_B);
   frame_rate =0;
   
 }
#endif
//      SET_PWM_DBG_PORT(LOW);		//DBG 
//  MC_Perf_Measure_Stop(Task_measurmet_get() , MEASURE_TSK_HighFrequencyTask);

}


/****************************************************************************
Name:        ISR_MC_HALL
Description: 
****************************************************************************/
INTERRUPT ISR_MC_HALL(void)
{
	CPU_ENTER_INTERRUPT();
#if defined(_EST_HALL_MT_)
	WORD twHall;
	
	HOOK_ISR_HALL();
	
	PORT_DEBOUNCE_READER(read_port_hall(), twHall, 5);
	IntHallMeasureMT(&oMeasureMT, twHall);

#endif
	CPU_EXIT_INTERRUPT();
}

/****************************************************************************
Name:        ISR_MC_IPM_FO
Description: 
****************************************************************************/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@IPM FAULT error */
INTERRUPT ISR_MC_IPM_FO(void)
{
	CPU_ENTER_INTERRUPT();
	wFailFlagInt |= FAILFLAG_IPM_FO;
	wFailFlagBridge |= FAILFLAG_IPM_FO;

	wIpmFoFailCnt = 0;
	EntryStateStop();
//	
	DISABLE_EMG();			// EMG Disable & Pending Clear
	CPU_EXIT_INTERRUPT();
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@IPM FAULT error */
/****************************************************************************
Name:        DevSetMotorIPMFaultDisableTime
Description: IPM overcurrent detection set function for blocking for a certain period of time (=IPM ������ ���� �����ð� ���� ���� �Լ�)
****************************************************************************/
PUBLIC void DevSetMotorIPMFaultDisableTime(WORD xwBlockingTime)
{
	HAL_DISABLE_INTERRUPT();
	if (xwBlockingTime == 0)
	{
		if (wFailFlagLoop & FAILFLAG_IPM_FO_MASK) wIpmFoBlockingTmr = 1;
	}
	else
	{
		if ((wFailFlagLoop & FAILFLAG_IPM_FO_MASK) == 0)
		{
			DISABLE_EMG();			// EMG Disable & Pending Clear
			wFailFlagLoop |= FAILFLAG_IPM_FO_MASK;			
			if (wFailFlagInt & FAILFLAG_IPM_FO)
			{
				wFailFlagInt &= ~FAILFLAG_IPM_FO;
				EntryStateStop();
			}
		}

		if (xwBlockingTime == 0xFFFF) wIpmFoBlockingTmr = 0;
		else wIpmFoBlockingTmr = xwBlockingTime;	
	}
	HAL_ENABLE_INTERRUPT();
}

/****************************************************************************
Name:        DevGetMotorIPMFaultState
Description: IPM Fault status check function (=IPM Fault ���� Ȯ�� �Լ�)
****************************************************************************/
PUBLIC EMcFailEvent DevGetMotorIPMFaultState (void)
{
	if ((wFailFlagBridge & FAILFLAG_IPM_FO)
			|| ((wFailFlagLoop & FAILFLAG_IPM_FO_MASK)&&(READ_PORT_IPM_FO() == LOW)))
	{
		return MTR_FAIL_IPMFAULT;
	}
	return MTR_FAIL_NON;
}

#ifndef _MTR_VOLT_FAIL_CHECK_MASK_
/****************************************************************************
Name:        DevSetMotorUnderVoltControl
Description: Function to set whether to perform fault occurrence and control due to low voltage/blackout or abnormal power supply (=������/���� �̻������� ���й߻� �� ���� ���࿩�� �����Լ�)
****************************************************************************/
PUBLIC void DevSetMotorUnderVoltControlMask(BOOLEAN xbOnOff)
{
	WORD twFlagMask = 0;
	if (xbOnOff) twFlagMask = FAILFLAG_UNDER_VOLT_MASK;
	if ((wFailFlagLoop ^ twFlagMask) & FAILFLAG_UNDER_VOLT_MASK)
	{
		if (twFlagMask)
		{
			HAL_DISABLE_INTERRUPT();
			wFailFlagLoop |= FAILFLAG_UNDER_VOLT_MASK;
			if (wFailFlagBridge & FAILFLAG_UNDER_VOLT)
			{
				if (wFailFlagInt & (FAILFLAG_UNDER_VOLT | FAILFLAG_OVER_VOLT))
				{
					wFailFlagInt &= ~(FAILFLAG_UNDER_VOLT | FAILFLAG_OVER_VOLT);
					EntryStateStop();
				}
			}
			else if ((wFailFlagBridge & FAILFLAG_OVER_VOLT)&&(wFailFlagInt & FAILFLAG_UNDER_VOLT))
			{
				wFailFlagInt = (wFailFlagInt | FAILFLAG_OVER_VOLT) & ~FAILFLAG_UNDER_VOLT;
				EntryStateStop();
			}
			HAL_ENABLE_INTERRUPT();
		}
		else
		{
			wFailFlagLoop &= ~FAILFLAG_UNDER_VOLT_MASK;
		}
	}
}

/****************************************************************************
Name:        DevGetMotorVoltFailState
Description: Controller power abnormality check function (=����� ���� �̻���� Ȯ�� �Լ�)
****************************************************************************/
PUBLIC EMcFailEvent DevGetMotorVoltFailState (void)
{
	if (wFailFlagBridge & FAILFLAG_OVER_VOLT) return MTR_FAIL_OVERVOLT;
	else if ((wFailFlagBridge & FAILFLAG_UNDER_VOLT)
			|| ((wFailFlagLoop & FAILFLAG_UNDER_VOLT_MASK)
				&& ((qVdcBus < Q_VOLT(140.0))||(IsAcZCroseOK() == FALSE))))
	{
		return MTR_FAIL_UNDERVOLT;
	}
	return MTR_FAIL_NON;
}
#endif	// #ifndef _MTR_VOLT_FAIL_CHECK_MASK_

/****************************************************************************
Name:        DoCheckFailPwm
Description: Motor operation failure check function (=���� ���� ���� üũ �Լ�)
****************************************************************************/
void DoCheckFailPwm(void)
{
	// IPM FAULT detection control (=IPM FAULT ���� ����)	
	if (wIpmFoBlockingTmr != 0)
	{
		if (--wIpmFoBlockingTmr == 0)
		{
			wFailFlagLoop &= ~FAILFLAG_IPM_FO_MASK;
			if (READ_PORT_IPM_FO())
			{
				ENABLE_EMG();					// EMG Protection Relrase & Enable
			}
			else
			{
				wFailFlagInt |= FAILFLAG_IPM_FO;
				wFailFlagBridge |= FAILFLAG_IPM_FO;
				wIpmFoFailCnt = 0;
				EntryStateStop();
			}
		}
	}
	else if (wFailFlagInt & FAILFLAG_IPM_FO)
	{
		if (READ_PORT_IPM_FO())
		{
			if (++wIpmFoFailCnt >= 20)
			{
				ENABLE_EMG();					// EMG Protection Relrase & Enable
				wFailFlagInt &= ~FAILFLAG_IPM_FO;
				EntryStateStop();
			}
		}
		else wIpmFoFailCnt = 0;
	}
	// Hall sensor error control (=Ȧ���� ���� ����)  
#if !defined(_EST_SENSORLESS_)	&& defined(_EST_HALL_OBS_)/*@@@@@*/
	if (wFailFlagInt & FAILFLAG_HALLSENSOR)
	{
		if (oObsHall.qThetaErr < Q_THETA_R(PI*4/8.0))
		{
			wFailFlagInt &= ~FAILFLAG_HALLSENSOR;
			EntryStateStop();
		}
	}
	else
	{
		/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Hall sensor error (=Ȧ��������) */
		if(oObsHall.qThetaErr > Q_THETA_R(PI*7/8.0))
		/*if (oObsHall.qThetaErr > Q_THETA_R(PI *5/8.0))*/
		{
			wFailFlagInt |= FAILFLAG_HALLSENSOR;
			wFailFlagBridge |= FAILFLAG_HALLSENSOR;
			EntryStateStop();
		}
		/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Hall sensor error (=Ȧ��������) */

	}
#endif
	// Overcurrent detection control (=������ ���� ����)  
#if !defined(_NON_CC_) || defined(_EST_SENSORLESS_)		// Overcurrent error check during sensorless estimation or current control (=�������� ���� �Ǵ� ��������� ������ ���� üũ) 
	if (wFailFlagInt & FAILFLAG_OVER_CURRENT)
	{
		if (qIs <= Q_CURRENT(CURRENT_OVER_FAIL * 0.2))	/* qIs is the vector sum of q-axis current and d-axis current (=qIs�� q�������� d�������� ������) */
		{
			if (++nOverCurrentFailCnt >= 20)
			{
				wFailFlagInt &= ~FAILFLAG_OVER_CURRENT;
				nOverCurrentFailCnt = 0;
				EntryStateStop();
			}
		}
		else nOverCurrentFailCnt = 0;
	}
	else
	{
		/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Overcurrent error (=����Ŀ��Ʈ����) */
		if (qIs > Q_CURRENT(IS_LIMIT * 0.85))
		{
			if (qIs > Q_CURRENT(CURRENT_OVER_FAIL)) nOverCurrentFailCnt += (int)(4.0 / 16.0 / TS_CC);
			else if (wMcFlag & MCFLAG_SC_ON) nOverCurrentFailCnt += 1;

			if (nOverCurrentFailCnt >= (int)(4.0 / TS_CC))
			{
				wFailFlagInt |= FAILFLAG_OVER_CURRENT;
				wFailFlagBridge |= FAILFLAG_OVER_CURRENT;
				nOverCurrentFailCnt = 0;
				EntryStateStop();
			}
		/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Overcurrent error (=����Ŀ��Ʈ����) */	
		}
		else if (nOverCurrentFailCnt) nOverCurrentFailCnt--;
	}/**/
#endif

#ifndef _MTR_VOLT_FAIL_CHECK_MASK_
	// �̻� ���� ���� 
	if (wFailFlagInt & (FAILFLAG_OVER_VOLT | FAILFLAG_UNDER_VOLT))
	{
		if (wFailFlagInt & FAILFLAG_UNDER_VOLT)
		{
			if (qVdcBus > Q_VOLT(VOLT_UNDER2OVER))		// Low voltage -> high voltage conversion (=������ -> ������ ��ȯ)
			{
				wFailFlagInt = (wFailFlagInt | FAILFLAG_OVER_VOLT) & ~FAILFLAG_UNDER_VOLT;
				EntryStateStop();
			}
		}
		else
		{
			if ((qVdcBus < Q_VOLT(VOLT_OVER2UNDER))&&((wFailFlagLoop & FAILFLAG_UNDER_VOLT_MASK) == 0))	// high voltage -> Low voltage conversion (=������ -> ������ ��ȯ)
			{
				wFailFlagInt = (wFailFlagInt | FAILFLAG_UNDER_VOLT) & ~FAILFLAG_OVER_VOLT;
				EntryStateStop();
			}
		}
		if ((qVdcBus >= Q_VOLT(VOLT_UNDER2OK)) && (qVdcBus <= Q_VOLT(VOLT_OVER2OK)) && (IsAcZCroseOK() != FALSE))
		{										// Abnormal power -> Normal power conversion (=�̻����� -> �������� ��ȯ)
			if ((++wPowerFailCnt >= (WORD)(0.15 / TS_CC)) && (qWrpmActual <= Q_OMEGA_RPM_DRUM(5.0)))
			{
				wFailFlagInt &= ~(FAILFLAG_UNDER_VOLT | FAILFLAG_OVER_VOLT);
				EntryStateStop();
			}
		}
		else 
		{
			wPowerFailCnt = 0;		
		}
	}
	else
	{
		/*@@@@@@@@@@@@@@@@@@@@@@@@@@@High voltage error (=�����п���) */
		if (qVdcBus > Q_VOLT(VOLT_OK2OVER))			// normal power -> high voltage conversion (=�������� -> ������ ��ȯ)
		{
			wFailFlagInt |= FAILFLAG_OVER_VOLT;
			if ((wFailFlagBridge & (FAILFLAG_UNDER_VOLT | FAILFLAG_OVER_VOLT)) == 0)
			{
				wFailFlagBridge |= FAILFLAG_OVER_VOLT;
			}
			EntryStateStop();	// Short circuit break settings (=�ܶ� �극��ũ ����)
			wPowerFailCnt = 0;
		}
		/*@@@@@@@@@@@@@@@@@@@@@@@@@@@High voltage error (=�����п���) */
		/*@@@@@@@@@@@@@@@@@@@@@@@@@@@Low voltage error (=�����п���) */
		else if ((qVdcBus < Q_VOLT(VOLT_OK2UNDER))||(IsAcZCroseOK() == FALSE))
		{										// Normal power -> blackout/low voltage conversion (=�������� -> ����/������ ��ȯ)
			if ((wFailFlagLoop & FAILFLAG_UNDER_VOLT_MASK) == 0)
			{			
				wFailFlagInt |= FAILFLAG_UNDER_VOLT;
				if ((wFailFlagBridge & (FAILFLAG_UNDER_VOLT | FAILFLAG_OVER_VOLT)) == 0)
				{
					wFailFlagBridge |= FAILFLAG_UNDER_VOLT;
				}
				EntryStateStop();		// IPM switch All Open
				wPowerFailCnt = 0;
			}
		}
		/*@@@@@@@@@@@@@@@@@@@@@@@@@@@Low voltage error (=�����п���) */
	}
#endif

	// Current control failure detection (=�������� ���� ����) 
/*	if (wMcFlag & MCFLAG_CC_ON)
	{
		Q15 tqIerr;
		DO_LPF1(oLpfIerr, (abs(oIdqseRef.qD - oIdqse.qD) + abs(oIdqseRef.qQ - oIdqse.qQ)), tqIerr);
		if (tqIerr > Q_CURRENT(2.0))
		{
			wFailFlagBridge |= FAILFLAG_CURRENT;
			EntryStateStop();
		}
	}
*/
/*	*/
	if (fpDoTsState == DoStateRun)
	{
		// Speed control speed difference excessive failure detection (=�ӵ����� �ӵ��� ���� ���� ����) 
		if (wMcFlag & MCFLAG_SC_ON)
		{
			if (abs(oRampWrpm.qRef - qWrpmActual) > Q_OMEGA_RPM_DRUM(RPMD_SPEED_FAIL))/* When command-actual exceeds RPMD_SPEED_FAIL (=����-���� �� RPMD_SPEED_FAIL�̻� ����) */
			{
				wSpeedFailCnt += 2;
				if (wSpeedFailCnt >= (WORD)(0.16 * 2 / TS_CC))	// Failure due to excessive speed difference of 0.16 seconds (=0.16�� �ӵ��� ���� ����) 
				{
					wFailFlagBridge |= FAILFLAG_SPEED;
					EntryStateStop();
					return;
				}
			}
			else if (wSpeedFailCnt) wSpeedFailCnt--;
		}
		// Check operation failure due to motor restraint (=���� ���ӿ� ���� ���� ���� üũ)
		/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Overload error (=�����ε忡��)*/
		if ((MUL_SS(oRampWrpm.qRef, qWrpmActual) <= 0) || IsFailEst(&oEstimate))/* IsFailEst(&oEstimate) is sensorless */
		{
			 WORD over_load_count=0;
			 float result = (T_OVERLOAD_FAIL * 6.0) / TS_CC;
			 if (result < 0) {
			     over_load_count = 0;
			 } else {
			     over_load_count = (WORD)result;
			 }
			wOverLoadFailCnt += 6;		
			if (wOverLoadFailCnt >= over_load_count)	// Restraint failure when motor stops and reverses (=���� ���� �� ��ȸ���� ���� ����)
			//if (wOverLoadFailCnt >= (WORD)(T_OVERLOAD_FAIL * 6 / TS_CC))	// Restraint failure when motor stops and reverses (=���� ���� �� ��ȸ���� ���� ����)
			{
				wFailFlagBridge |= FAILFLAG_OVERLOAD;						/* In run mode, when speed = 0 or reverse rotation continues for more than T_OVERLOAD_FAIL time (=����忡�� �ӵ�=0 �̰ų� ��ȸ���� T_OVERLOAD_FAIL�ð� �̻� ���ӵɶ�) */ 
				EntryStateStop();
				return;
			}
		}
		/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Overload error (=�����ε忡��)*/
		else if (wOverLoadFailCnt) wOverLoadFailCnt--;
	}

}

/****************************************************************************
Name:        DoCheckFailLoop
Description: Motor operation failure check function (=���� ���� ���� üũ �Լ�)
****************************************************************************/
void DoCheckFailLoop(void)
{
	eMcFailEvent = MTR_FAIL_NON;
	if (wFailFlagBridge == 0) return;
	HAL_DISABLE_INTERRUPT();
	if (wFailFlagBridge & FAILFLAG_ALIGN)			// Align failure (=����� ����?)
	{
		wFailFlagBridge &= ~FAILFLAG_ALIGN;
		if ((eMcCtrlMode != MTR_MODE_STOP_BRAKE)&&(eMcCtrlMode != MTR_MODE_STOP_OPEN))
		{
			DevSetMotorDrive(MTR_MODE_STOP_BRAKE, 0, 0);
			eMcFailEvent = MTR_FAIL_ALIGN;
		}
	}
	else if (wFailFlagBridge & FAILFLAG_STARTUP)	// Operatioon failure (=�⵿ ����?)
	{
		wFailFlagBridge &= ~FAILFLAG_STARTUP;
		if ((eMcCtrlMode != MTR_MODE_STOP_BRAKE)&&(eMcCtrlMode != MTR_MODE_STOP_OPEN))
		{
			DevSetMotorDrive(MTR_MODE_STOP_BRAKE, 0, 0);
			eMcFailEvent = MTR_FAIL_STARTUP;
		}
	}
	else if (wFailFlagBridge & FAILFLAG_OVERLOAD)	// Motor restraint (=���� ����?)
	{
		wFailFlagBridge &= ~FAILFLAG_OVERLOAD;
		if ((eMcCtrlMode != MTR_MODE_STOP_BRAKE)&&(eMcCtrlMode != MTR_MODE_STOP_OPEN))
		{
			DevSetMotorDrive(MTR_MODE_STOP_BRAKE, 0, 0);
			eMcFailEvent = MTR_FAIL_OVERLOAD;
		}
	}	
	else if (wFailFlagBridge & FAILFLAG_SPEED)		// Speed failure (=�ӵ��� ����?)
	{
		wFailFlagBridge &= ~FAILFLAG_SPEED;
		if ((eMcCtrlMode != MTR_MODE_STOP_BRAKE)&&(eMcCtrlMode != MTR_MODE_STOP_OPEN))
		{
			DevSetMotorDrive(MTR_MODE_STOP_BRAKE, 0, 0);
			eMcFailEvent = MTR_FAIL_SPEED;
		}
	}	
	else if (wFailFlagBridge & FAILFLAG_CURRENT)	// Current control failure (=�������� ���� ?)
	{
		wFailFlagBridge &= ~FAILFLAG_CURRENT;
		if ((eMcCtrlMode != MTR_MODE_STOP_BRAKE)&&(eMcCtrlMode != MTR_MODE_STOP_OPEN))
		{
			DevSetMotorDrive(MTR_MODE_STOP_BRAKE, 0, 0);
			eMcFailEvent = MTR_FAIL_OVERCURRENT;
		}
	}
	else if (wFailFlagBridge & FAILFLAG_OFFSET)		// Current OFFSET failure (=���� OFFSET ���� ?)
	{
		wFailFlagBridge &= ~FAILFLAG_OFFSET;
		if ((eMcCtrlMode != MTR_MODE_STOP_BRAKE)&&(eMcCtrlMode != MTR_MODE_STOP_OPEN))
		{
			DevSetMotorDrive(MTR_MODE_STOP_BRAKE, 0, 0);
			eMcFailEvent = MTR_FAIL_ADCOFFSET;
		}
	}
	// IPM FAULT OUT?
	if ((wFailFlagBridge & FAILFLAG_IPM_FO)&&(eMcFailEvent == MTR_FAIL_NON))
	{
		if ((wFailFlagLoop & FAILFLAG_IPM_FO) == 0)
		{
			if ((eMcCtrlMode != MTR_MODE_STOP_BRAKE)&&(eMcCtrlMode != MTR_MODE_STOP_OPEN))
			{
				eMcFailEvent = MTR_FAIL_IPMFAULT;
				DevSetMotorDrive(MTR_MODE_STOP_BRAKE, 0, 0);
			}
			wFailFlagLoop |= FAILFLAG_IPM_FO;
		}
		else if ((wFailFlagInt & FAILFLAG_IPM_FO) == 0)	//(PORT_IPM_FAULT == HI)
		{
			wFailFlagBridge &= ~FAILFLAG_IPM_FO;
			wFailFlagLoop &= ~FAILFLAG_IPM_FO;
		}
	}
	//  Hall sensor error control (=Ȧ���� ���� ����)  
#if !defined(_EST_SENSORLESS_)	
	if ((wFailFlagBridge & FAILFLAG_HALLSENSOR)&&(eMcFailEvent == MTR_FAIL_NON))
	{
		if ((wFailFlagLoop & FAILFLAG_HALLSENSOR) == 0)
		{
			if ((eMcCtrlMode != MTR_MODE_STOP_BRAKE)&&(eMcCtrlMode != MTR_MODE_STOP_OPEN))
			{
				eMcFailEvent = MTR_FAIL_HALLSENSOR;
				DevSetMotorDrive(MTR_MODE_STOP_BRAKE, 0, 0);
			}
			wFailFlagLoop |= FAILFLAG_HALLSENSOR;
		}
		else if ((wFailFlagInt & FAILFLAG_HALLSENSOR) == 0)	
		{
			wFailFlagBridge &= ~FAILFLAG_HALLSENSOR;
			wFailFlagLoop &= ~FAILFLAG_HALLSENSOR;
		}
	}
#endif
	// Overcurrent detection (=������ ����?)
	if ((wFailFlagBridge & FAILFLAG_OVER_CURRENT)&&(eMcFailEvent == MTR_FAIL_NON))
	{
		if ((wFailFlagLoop & FAILFLAG_OVER_CURRENT) == 0)
		{
			// if ((eMcCtrlMode != MTR_MODE_STOP_BRAKE)&&(eMcCtrlMode != MTR_MODE_STOP_OPEN))
			{
				eMcFailEvent = MTR_FAIL_OVERCURRENT;
				DevSetMotorDrive(MTR_MODE_STOP_BRAKE, 0, 0);
			}
			wFailFlagLoop |= FAILFLAG_OVER_CURRENT;
		}
		else if ((wFailFlagInt & FAILFLAG_OVER_CURRENT) == 0)
		{
			wFailFlagBridge &= ~FAILFLAG_OVER_CURRENT;
			wFailFlagLoop &= ~FAILFLAG_OVER_CURRENT;
		}
	}	
#ifndef _MTR_VOLT_FAIL_CHECK_MASK_
	// High voltage/low voltage, power outage detection (������/������,���� ����?)
	if ((wFailFlagBridge & (FAILFLAG_UNDER_VOLT | FAILFLAG_OVER_VOLT))&&(eMcFailEvent == MTR_FAIL_NON))
	{
		if ((wFailFlagLoop & (FAILFLAG_UNDER_VOLT | FAILFLAG_OVER_VOLT)) == 0)
		{
			if (wFailFlagBridge & FAILFLAG_UNDER_VOLT) eMcFailEvent = MTR_FAIL_UNDERVOLT;
			else eMcFailEvent = MTR_FAIL_OVERVOLT;
			DevSetMotorDrive(MTR_MODE_STOP_BRAKE, 0, 0);
			wFailFlagLoop |= (wFailFlagBridge & (FAILFLAG_UNDER_VOLT | FAILFLAG_OVER_VOLT));
		}
		else if ((wFailFlagInt & (FAILFLAG_UNDER_VOLT | FAILFLAG_OVER_VOLT)) == 0)
		{
			wFailFlagBridge &= ~(FAILFLAG_UNDER_VOLT | FAILFLAG_OVER_VOLT);
			wFailFlagLoop &= ~(FAILFLAG_UNDER_VOLT | FAILFLAG_OVER_VOLT);
		}
	}
#endif
	HAL_ENABLE_INTERRUPT();	
}

/****************************************************************************
Name:        DevMainLoopMotor
Description:
****************************************************************************/
PUBLIC void DevMainLoopMotor(void)
{
	DoCheckFailLoop();
	/*DevGetMotorSensData(MTR_SENS_UNBALREAL);*/
}

/***************************************************************************************************
Name:        EntryStateStop  
Description: Entering stop state (���� ���� ����)
***************************************************************************************************/
void EntryStateStop(void)
{
	if (fpDoTsState != DoStateStop)
	{
		fpDoTsState = DoStateStop;
		wStateIndex = 0x10;
		wAlignFailCnt = 0;
		SetRampTarget(&oRampWrpm, Q_OMEGA_RPM(0.0), -1);
	
		oIdqseRef.qD = Q_CURRENT(0.0);
		oIdqseRef.qQ = Q_CURRENT(0.0);
		oVdqse.qD = Q_VOLT(0.0);
		SetEstFlag(&oEstimate, SLFLAG_WREF_NON);
	}

	DoStateStop();
}

void DoStateStop(void)
{
	Q15 tqVqse = Q_VOLT(0.0);	
	wMcFlag = 0;
#if defined(_EST_SENSORLESS_)
	if (abs(oEstimate.qWrpmLpf) < Q_OMEGA_RPM_DRUM(4.0 * QMAX_OMEGA_RPM_DRUM / 1800.0)) qWrpmActual = Q_OMEGA_RPM_DRUM(0.0);
#endif	
	if (wFailFlagInt)
	{
		if (wFailFlagInt & (FAILFLAG_IPM_FO | FAILFLAG_UNDER_VOLT)) wMcFlag |= MCFLAG_PWM_OFF;
		else wMcFlag |= MCFLAG_PWM_ZERO;
	}
	else 
	{
		if (qWrpmActual == 0) wStateIndex &= ~0x10;
#if !defined(_EST_SENSORLESS_)
		else wStateIndex |= 0x10;
#else
		if ((wStateIndex & 0x10) == 0x00) qWrpmActual = 0;
#endif

	  	if (wStateIndex == 0x00)		// Motor stop && Offset not completed (=���� ���� && Offset �̿Ϸ�) 
		{
			if (wReturnADC & ADC_OFFSET_FLAG_END) wStateIndex |= 0x01;
			else wMcFlag |= MCFLAG_AD_OFFSET;
			wMcFlag |= MCFLAG_PWM_OFF;
		}
		else if (eMcCtrlMode == MTR_MODE_STOP_OPEN) wMcFlag |= MCFLAG_PWM_OFF;
		else
		{
			if (wStateIndex & 0x10)
			{
#if defined(_ONE_SHUNT_)
				Q15 tqVlim = MUL_SS(Q15_FRAC(1.0 / SQRT3), qVdcBus) >> 15;
				tqVqse = MUL_SS(qWrpmActual, (Q15)(-0.2 * Q_VEQ_KE)) >> SCALE_VEQ;
				if (tqVqse > tqVlim) tqVqse = tqVlim;			
				else if (tqVqse < -tqVlim) tqVqse = -tqVlim;
#else
				wMcFlag |= MCFLAG_PWM_ZERO;
#endif
			}
			else wMcFlag |= MCFLAG_PWM_ZERO;
		}
	}
	oVdqse.qQ = tqVqse;
}

/***************************************************************************************************
Name:        EntryStateReady  
Description: Enter ready state (=�غ� ���� ����)
***************************************************************************************************/
void EntryStateReady(void)
{
	if (fpDoTsState != DoStateReady)
	{
		wStateIndex = 0x10;
		fpDoTsState = DoStateReady;
		wStateTmBuf = (WORD)(T_READY / TS_CC);
		qdStateInteg = (long)(4.0 / TS_CC) + (MUL_SS(abs(qWrpmActual), (Q15)(60.0 / (TS_CC * (1L << 6)))) >> (15 -6));
		SetRampRef(&oRampWrpm, Q_OMEGA_RPM(0.0));
		
		oIdqseRef.qD = Q_CURRENT(0.0);
		oIdqseRef.qQ = Q_CURRENT(0.0);
		oVdqse.qD = Q_VOLT(0.0);
		oVdqse.qQ = Q_VOLT(0.0);	
		wMcFlag = 0;		
#if !defined(_NON_CC_) || defined(_EST_SENSORLESS_)		// Offset error check during sensorless estimation or current control (=�������� ���� �Ǵ� ��������� Offset ���� üũ)
		/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Current detection error (=������������) */ 
		if (wReturnADC & ADC_OFFSET_NG)
		{
			wFailFlagBridge |= FAILFLAG_OFFSET;
			EntryStateStop();
			return;			
		}
		/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Current detection error (=������������) */ 
#endif
		SetEstFlag(&oEstimate, SLFLAG_WREF_ZERO);
	}
	SetRampTarget(&oRampWrpm, qCtrlParam1, qCtrlParam2);
}

void DoStateReady(void)
{
#if defined(_EST_SENSORLESS_)
	if ((abs(oEstimate.qWrpmLpf) < Q_OMEGA_RPM_DRUM(4.0 * QMAX_OMEGA_RPM_DRUM / 1800.0)) || ((wStateIndex & 0x10) == 0x00))
	{
		qWrpmActual = Q_OMEGA_RPM_DRUM(0.0);
	}
	if (qWrpmActual == 0) wStateIndex &= ~0x10;
#endif

	if (qWrpmActual != Q_OMEGA_RPM_DRUM(0.0))
	{
		if (wStateTmBuf) wStateTmBuf--;
		if (qdStateInteg-- == 0)
		{
			wFailFlagBridge |= FAILFLAG_ALIGN;
			EntryStateStop();
		}
	}
	else{	 

		if (wStateTmBuf-- == 0)
		{
			// EntryStateInject();
			EntryStateAlign();
		}
	}
}

/***************************************************************************************************
Name:        EntryStateAlign  
Description: Enter align state (=����� ���� ����)
***************************************************************************************************/
#if defined(_MEASURE_RS_)
long qdVoltSum;
long qdCurrentSum;
Q15 qRsK1 = Q_VEQ_RS;
Q15 qRsVeq = Q_VEQ_RS;

#define I1_RS_MEASURE		(0.34 * I_ALIGN)

#define TPWM_GET_RS_MASURE		(WORD)MIN((0.08 * T_ALIGN / TS_CC), 128)
#define TPWM_STEPUP_RS_MASURE	(WORD)((0.3 + 0.5 *(I_ALIGN - I1_RS_MEASURE) / I_ALIGN) * T_ALIGN / TS_CC)
// Need to be modified as the initial value of align current (I_ALIGN *0.1) (=����� ���� �ʱⰪ (I_ALIGN *0.1)�� ���� ���� �ʿ�) 
#endif

void EntryStateAlign(void)
{
	SetRampTarget(&oRampWrpm, qCtrlParam1, qCtrlParam2);
	
	if (fpDoTsState != DoStateAlign)
	{
		// TAngle toAngle;
		// Q15 tqIdRef;
		
		fpDoTsState = DoStateAlign;
		wStateTmBuf = (WORD)(motor_parameter[parameter_index_id].T_ALIGN_id / TS_CC);						// Align time setting (=����� �ð� ����) 		
		wStateCnt = 0;
		wStateIndex = 0;

#if defined(_EST_SENSORLESS_) || (defined(_EST_ENCODER_) && defined(_NON_ENCODER_Z_))
		qStateTheta = oAngle.qTheta + Q_THETA_R(PI / 6.);			// Align angle setting (=����� ���� ����) 
		qStateTheta = MUL_UU((MUL_UU((qStateTheta & 0x7FFF), 3) >> 15), Q_THETA_R(PI / 3.)) | (qStateTheta & 0x8000);
#else // if defined(_EST_HALL_OBS_) || defined(_EST_HALL_MT_)
		qStateTheta = oAngle.qTheta;
#endif

#if defined(_ACT_SENSORLESS_)
		InitEstimator(&oEstimate, qStateTheta);		// 		
		SetEstFlag(&oEstimate, SLFLAG_WREF_ZERO);
#if defined(_EST_SENSORLESS_)
		oAngle.qTheta = oEstimate.oAngle.qTheta;
		oAngle.qCos = oEstimate.oAngle.qCos;
		oAngle.qSin = oEstimate.oAngle.qSin;
		qWrpmActual = Q_OMEGA_RPM(0.0);
		ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	
#endif
#endif

#if defined(_MEASURE_RS_)
		SetRampTarget(&oStateRamp1, Q_CURRENT(MIN(I1_RS_MEASURE, I_ALIGN *0.1)), -1);	// Align current initial value (=����� ���� �ʱⰪ) //RS
		SetRampTarget(&oStateRamp1, Q_CURRENT(I1_RS_MEASURE), Q_SLOPE_CURRENT(I_ALIGN / (T_ALIGN *0.45)));
		qdVoltSum = 0;
		qdCurrentSum = 0;
#else
		SetRampTarget(&oStateRamp1, Q_CURRENT(0.25 * motor_parameter[parameter_index_id].I_ALIGN_id), -1);							// Align current initial value (=����� ���� �ʱⰪ)
		SetRampTarget(&oStateRamp1, Q_CURRENT(motor_parameter[parameter_index_id].I_ALIGN_id), Q_SLOPE_CURRENT(motor_parameter[parameter_index_id].I_ALIGN_id /motor_parameter[parameter_index_id].T_ALIGN_id));	// Align current target/slope setting (=����� ���� ��ǥ/ ���� ����)
#endif

#if defined(_MEASURE_RS_)		
		INIT_LPF1(oLpfState, Q_CURRENT(0.0), (2.0 * PI * 16.0 / T_ALIGN), TS_CC); 	// Align current filter settings (=����� ���� ���� ����) //RS	
#else
		INIT_LPF1(oLpfState, Q_CURRENT(0.0), (2.0 * PI * 5.0 / motor_parameter[parameter_index_id].T_ALIGN_id), TS_CC); 	// Align current filter settings (=����� ���� ���� ����) 
#endif

		// DO_LPF1(oLpfState, oStateRamp1.qRef, tqIdRef);		
			
		oIdqseRef.qD = Q_CURRENT(0.0); 
		oIdqseRef.qQ = Q_CURRENT(0.0); 

		wMcFlag = (MCFLAG_CC_ON | MCFLAG_CC_VFF);
		
		// Current controller initialization 
		SET_PIA1_GAIN(oCtrlPiIde, KP_CC_D, KI_CC_D, KA_CC_D, TS_CC, QMAX_CURRENT, QMAX_VOLT);
		oCtrlPiIde.qdInteg = 0;
		oCtrlPiIde.qVref = 0;
		SET_PIA1_GAIN(oCtrlPiIqe, KP_CC_Q, KI_CC_Q, KA_CC_Q, TS_CC, QMAX_CURRENT, QMAX_VOLT);
		oCtrlPiIqe.qdInteg = 0;
		oCtrlPiIqe.qVref = 0;

		INIT_LPF1(oLpfIerr, Q_CURRENT(0.0), 5.0 *2.0 *PI, TS_CC);
	}
}

void DoStateAlign(void)
{
	SetAngle(&oAngle, qStateTheta);	
	ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	
#if defined(_EST_SENSORLESS_)
	qWrpmActual = Q_OMEGA_RPM(0.0);
#endif
	DO_LPF1(oLpfState, oStateRamp1.qRef, oIdqseRef.qD);
	oIdqseRef.qQ = Q_CURRENT(0.0);
	
	DoRampRef(&oStateRamp1);				// Align current command rises 
	if (wStateTmBuf > 0) wStateTmBuf--;	
#if defined(_MEASURE_RS_)
	{
		Q15 tqVoltRs;
		Q15 tqCurrentRs;

		tqVoltRs = SqrtS32(MUL_SS(oVdqss.qD, oVdqss.qD) + MUL_SS(oVdqss.qQ, oVdqss.qQ)); // oVdqse.qD; // 
		tqCurrentRs = SqrtS32(MUL_SS(oIdqss.qD, oIdqss.qD) + MUL_SS(oIdqss.qQ, oIdqss.qQ)); // oIdqse.qD; // 

		if ((wStateTmBuf >= TPWM_STEPUP_RS_MASURE) && (wStateTmBuf < (TPWM_STEPUP_RS_MASURE + TPWM_GET_RS_MASURE)))
		{
			qdVoltSum -= tqVoltRs;
			qdCurrentSum -= tqCurrentRs; 
			qdCurrentSum += ((Q_CURRENT(I1_RS_MEASURE) - oIdqse.qD) *32);		// X-Man
			if (wStateTmBuf == TPWM_STEPUP_RS_MASURE)
			{
				SetRampTarget(&oStateRamp1, Q_CURRENT(I_ALIGN), Q_SLOPE_CURRENT(I_ALIGN / (T_ALIGN *0.45)));	// Align current target/slope setting
			}
		}
		else if ((wStateTmBuf >= 1) && (wStateTmBuf < (1 + TPWM_GET_RS_MASURE)))
		{
			qdVoltSum += tqVoltRs;
			qdCurrentSum += tqCurrentRs; 
			qdCurrentSum += ((Q_CURRENT(I_ALIGN) - oIdqse.qD) *32);		// X-Man
			if (wStateTmBuf == 1)
			{
				tqVoltRs = DIV_SD((qdVoltSum << (SCALE_VEQ - 3)), (qdCurrentSum >> 3));
				//	SetRsEst(&oEstimate, tqVoltRs); 		
			#if defined(_DEAD_TIME_COMPEN_T_) 
			#elif defined(_DEAD_TIME_COMPEN_V_)
			//	tqVoltRs = (MUL_SS(Q15_FRAC(1.05), tqVoltRs) >> 15);
			#else
				tqVoltRs = (MUL_SS(Q15_FRAC(0.88), tqVoltRs) >> 15);	// (0.875)
			#endif
				if (tqVoltRs < (Q15)(Q_VEQ_RS * 0.95)) tqVoltRs = (Q15)(Q_VEQ_RS * 0.95);
				else if (tqVoltRs > (Q15)(Q_VEQ_RS * 1.45)) tqVoltRs = (Q15)(Q_VEQ_RS * 1.45);
				qRsVeq = (qRsK1 + tqVoltRs) >>1;
				qRsK1 = tqVoltRs;
				SetRsEst(&oEstimate, qRsVeq); // 
			}
		}
	}
#endif

#if defined(_EST_SENSORLESS_)
#if !defined(_NON_CC_)
	if ((oStateRamp1.qRef == oStateRamp1.qTar)
		&&((abs(oIdqse.qQ) >= Q_CURRENT(I_ALIGN * 0.2))||(abs(oIdqse.qD - oIdqseRef.qD) >= Q_CURRENT(I_ALIGN * 0.2))))
#else
	if ((oStateRamp1.qRef == oStateRamp1.qTar)&&(oIdqse.qD < Q_CURRENT(0.0)))
#endif
	{
		if (++wStateCnt >= 24)
		{
			wFailFlagBridge |= FAILFLAG_ALIGN;
			EntryStateStop();
			return;
		}
	}
#else // if defined(_EST_HALL_OBS_) || defined(_EST_HALL_MT_) || defined(_EST_ENCODER_)

#if defined(_EST_HALL_OBS_) || defined(_EST_HALL_MT_)
	if (qWrpmActual != 0)
#else // defined(_EST_ENCODER_)
	if (abs(qWrpmActual) >= Q_OMEGA_RPM_DRUM(10.0 * QMAX_OMEGA_RPM_DRUM / 1800.0))/* If it is 10rpm or more than 8 times (=10rpm�̻��� ���� 8ȸ �̻��̸�) */
#endif
	{
#if defined(_EST_HALL_OBS_) || defined(_EST_HALL_MT_) || !defined(_NON_ENCODER_Z_) 
		if (++wAlignFailCnt >= 8)			// Align failed (=����� ����)
		{
			wFailFlagBridge |= FAILFLAG_ALIGN;
			EntryStateStop();
		}
		else EntryStateReady();
#endif
	}
#endif
	else 
	{
		wStateCnt = 0;
		if (wStateTmBuf == 0)			// Align state timeout (=����� ���� �ð� ����)
		{
#if !defined(_EST_SENSORLESS_) && !defined(_EST_HALL_OBS_) && !defined(_EST_HALL_MT_) && defined(_EST_ENCODER_)
#if defined(_NON_ENCODER_Z_)
			if (abs(qWrpmActual) < Q_OMEGA_RPM_DRUM(4.0 * QMAX_OMEGA_RPM_DRUM / 1800.0)) // (qWrpmActual == Q_OMEGA_RPM_DRUM(0.0))
			{
				wStateIndex = 0;				// Change the encoder initial angle to the alignment angle (=����� ������ ���ڴ� �ʱⰢ ����) 
				qThetaOffsetEncoder = (short)(oAngle.qTheta - oObsEncoder.oAngle.qTheta + qThetaOffsetEncoder);
				InitObserver(&oObsEncoder, oAngle.qTheta);
				qWrpmActual = Q_OMEGA_RPM_DRUM(0.0);
			}
			else if (++wStateIndex > 100)
			{
				wStateIndex = 0;
				if (++wAlignFailCnt >= 8)			// Align failed (=����� ����)  
				{
					wFailFlagBridge |= FAILFLAG_ALIGN;
					EntryStateStop();
				}
				else EntryStateReady();
				return;
			}
#else	// Error when Align Theta is exceeded when controlling an encoder using Z Pulse (=Z Pulse ��� ���ڴ� ����� Align Theta �ʰ��� ����)
			if (abs(oObsEncoder.oAngle.qTheta - oAngle.qTheta) > Q_THETA_R(PI * 30. / 180.))
			{
				wStateIndex = 0;
				if (++wAlignFailCnt >= 8)			// Align failed (=����� ����)
				{
					wFailFlagBridge |= FAILFLAG_ALIGN;
					EntryStateStop();
				}
				else EntryStateReady();
				return;
			}
#endif
#elif defined(_EST_ENCODER_) && defined(_NON_ENCODER_Z_)	// Sensorless or Hall sensor control, initial angle change when using encoder (=�������� or Ȧ���� ����, ���ڴ� ���� �ʱⰢ ����)
			qThetaOffsetEncoder = (short)(oAngle.qTheta - oObsEncoder.oAngle.qTheta + qThetaOffsetEncoder);
			InitObserver(&oObsEncoder, oAngle.qTheta);	
#endif

			if (wStateIndex == 0)
			{
#if defined(_ACT_SENSORLESS_)							
				InitEstimator(&oEstimate, qStateTheta);		
	  #if defined(_SL_MATSUI_)
				oEstimate.oIeEst.qD = oIdqse.qD;
				oEstimate.oIeEst.qQ = oIdqse.qQ;
	  #endif
#endif

#if defined(_EST_SENSORLESS_) && defined(_EST_ENCODER_) && defined(_MOTOR_PARAM_)
				if (wFlagEstEnc != 0)
				{
					EntryStateRun();
					return;
				}
#endif

#if defined(_NON_STARTUP_)
				EntryStateRun();
#else
				EntryStateStartup();
#endif
			}
		}
	}
}


#if defined(_EST_SENSORLESS_) && defined(_SYNC_ACCEL_)
/***************************************************************************************************
Name:        EntryStateStartup  
Description: Enters synchronous acceleration start state (=���Ⱑ�� �⵿ ���� ����)
Synchronous acceleration by changing the alignment current to the Q-axis current command, (=����������� Q������ �������� �����Ͽ� ���Ⱑ��,)  
***************************************************************************************************/
#if defined(_SYNC_ACCEL_WM_)	

void EntryStateStartup(void)
{
	if (fpDoTsState != DoStateStartup)
	{
		Q15 tqTheta;
		Q30 tqdInteg;
		
		fpDoTsState = DoStateStartup;		
		wStateTmBuf = (WORD)(T_MAX_STARTUP / TS_CC);		// Startup time limit setting (=�⵿ ���� �ð� ����) 	
		wStateCnt = 0;
		wStateIndex = 0;
		nStateGain = 0;

		SetRampTarget(&oRampWrpm, qCtrlParam1, qCtrlParam2);	
		
		SetRampTarget(&oStateRamp1, SIGN(oRampWrpm.qTar) * Q_CURRENT(I_STARTUP_INIT), -1);	// Starting current initial/target value setting (=�⵿ ���� �ʱ�/��ǥ�� ����) 
		SetRampTarget(&oStateRamp1, SIGN(oRampWrpm.qTar) * Q_CURRENT(I_STARTUP_MAX), Q_SLOPE_CURRENT(SLOPE_I_SYNC));

		SetRampTarget(&oStateRamp2, Q_OMEGA_RPM_DRUM(0.), -1);								// Starting synchronous acceleration setting (=�⵿ ���� ���ӵ� ����) 
		SetRampTarget(&oStateRamp2, SIGN(oRampWrpm.qTar) * Q_OMEGA_RPM_DRUM(RPMD_SYNC_MAX), Q_SLOPE_RPM_DRUM(SLOPE_RPMD_SYNC));
		INIT_LPF1(oLpfState, Q_OMEGA_RPM_DRUM(0.0), (2.0 * PI * 10.0 / T_MAX_STARTUP), TS_CC);
		qWrpmActual = Q_OMEGA_RPM_DRUM(0.0);

//		nCntDbg1 =0;

		qdStateInteg = (long)(qStateTheta + (SIGN(oRampWrpm.qTar) * Q_THETA_R(-PI /2.))) << SCALE_OMEGAINTEG;	// Initial start-up electrical angle setting (=�⵿ �ʱ� ���Ⱒ ����) 
		tqTheta = (Q15)(qdStateInteg >> SCALE_OMEGAINTEG);
		SetAngle(&oAngle, tqTheta);
		// DO_LPF1(oLpfState, oStateRamp2.qRef, qWrpmActual);
		//qWrpmActual = oStateRamp2.qRef;
		ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	

		oIdqseRef.qD = Q_CURRENT(0.0);
		oIdqseRef.qQ = oStateRamp1.qRef;

		tqdInteg = oCtrlPiIde.qdInteg;
		oCtrlPiIde.qdInteg = oCtrlPiIqe.qdInteg;
		oCtrlPiIqe.qdInteg = (SIGN(oRampWrpm.qTar) * tqdInteg);

		if (oRampWrpm.qTar >= 0) SetEstFlag(&oEstimate, SLFLAG_WREF_POSITIVE);
		else SetEstFlag(&oEstimate, SLFLAG_WREF_NEGATIVE);

	// 	SET_LPF1_GAIN(oEstimate.oLpfWr, (WC_LPF_WR_FLUX * 1.5), TS_CC); 
		
		wMcFlag = (MCFLAG_CC_ON | MCFLAG_CC_VFF);	
	}
	else if (SIGN(qCtrlParam1) != SIGN(oRampWrpm.qTar))
	{
		EntryStateReady();
		return;
	}
	else SetRampTarget(&oRampWrpm, qCtrlParam1, qCtrlParam2);
}

void DoStateStartup(void)
{
	Q15 tqTheta;
	Q15 tqWrpmSync;
	Q15 tqThetaErr;
	Q15 tqWrpmErr;
	
	Q15 tqThetaEst;
	Q15 tqWrpmEst;

	tqThetaEst = oEstimate.oAngle.qTheta;;
	tqWrpmEst = oEstimate.qWrpmLpf;

	
	tqTheta = (qdStateInteg >> SCALE_OMEGAINTEG);			// Synchronous acceleration magnetic flux angle calculation (=���� ���� �ڼӰ� ���)  
	DO_LPF1(oLpfState, oStateRamp2.qRef, tqWrpmSync);		// Synchronous angular velocity filter processing (=���� ���ӵ� ���� ó��) 

	tqThetaErr = (short)(tqThetaEst - tqTheta);
	tqWrpmErr = tqWrpmEst - tqWrpmSync;
	// nCntDbg1 = tqThetaErr;	
/**/	
	if (wStateTmBuf < (WORD)(T_MAX_STARTUP / 4.0 / TS_CC))
	{
		
		/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Startup error (=�⵿����) */
		if ((((oRampWrpm.qTar > 0)&&(tqWrpmEst <= 0))||((oRampWrpm.qTar < 0)&&(tqWrpmEst >= 0)))/* When it is judged to be in reverse rotation or when it is not possible to escape the starting mode within the standard time (=��ȸ�� ���� �Ǵ� �ɶ� �Ǵ� ���ؽð� ���� �⵿ ��带 Ż�� ���� ��) */
			||(wStateTmBuf == 0))			// Start failure (reverse rotation sensorless detection, start time exceeded) (=�⵿����(��ȸ�� �������� ����, �⵿�ð� �ʰ���))
		{
//			nCntDbg1 |= 0x0200;
			wFailFlagBridge |= FAILFLAG_STARTUP;
			EntryStateStop();
			return;
		}
		/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Startup error (=�⵿����) */
	}
	if (wStateIndex == 0)
	{
		wStateTmBuf--;		// Decrease startup error check time counter (=�⵿���� üũ �ð�ī���� ����) 
		if (/*(MUL_SS(oRampWrpm.qTar, oIdqse.qQ) > 0) && (oIdqse.qD >= Q_CURRENT(0.0)) &&*/ /*(abs(tqThetaErr) <= Q_THETA_R(60. * PI/180.)) && */
			(abs(tqWrpmErr) <= Q_OMEGA_RPM_DRUM(RPMD_SYNC_MAX * 0.25))) // 0.1)))
		{	// If the sensorless magnetic flux angle rotation direction is within 90 degrees and the sensorless speed deviation is within a certain amount, the command current reduction status changes. (=�������� �ڼӰ� ȸ������ 90�� �̳�, �������� �ӵ� ���� �����̳� �̸� ���� ���� ���� ���� ����)
			if (++wStateCnt >= (WORD)(T_MAX_STARTUP * 0.005 / TS_CC)) // 0.010 // 0.008
			{
				if ((abs(tqWrpmSync) >= Q_OMEGA_RPM_DRUM(RPMD_SYNC_EXIT)) && (abs(tqThetaErr) <= Q_THETA_R(45. * PI/180.)) /**/ 
						|| (abs(tqWrpmSync) >= Q_OMEGA_RPM_DRUM((RPMD_SYNC_EXIT + RPMD_SYNC_MAX) /2.))
					/* && (oIdqse.qD >= Q_CURRENT(0.0)) && (MUL_SS(oRampWrpm.qTar, oIdqse.qQ) > 0)*/) 
				{
					// SetRampTarget(&oStateRamp1, Q_CURRENT(0.0), Q_SLOPE_CURRENT(I_STARTUP_MAX * 8. / T_MAX_STARTUP));	// Command current reduction (=�������� ����)
					wStateCnt = 0;
					nStateGain = 32768;
					qStateTheta = tqWrpmEst;
					wStateIndex = 1;
					// EntryStateRun(); return; 
					// wFailFlagBridge |= FAILFLAG_STARTUP;	EntryStateStop();	return;
				}
				else
				{
					if ((((oRampWrpm.qTar)>=0)? tqThetaErr :-tqThetaErr) >= Q_THETA_R(60. * PI/180.)) wStateIndex |= 0x8000;
					else if ((((oRampWrpm.qTar)>=0)? tqThetaErr :-tqThetaErr) <= Q_THETA_R(-60. * PI/180.)) DoRampRef(&oStateRamp1);
				}
			}
		//	if (wStateTmBuf == 0) wStateTmBuf = 1;	// startup time exceeding errors prohibit (=�⵿�ð� �ʰ� ���� ����)
		}
		else 
		{
			wStateCnt = 0;							// In case of sensorless failure, status change check counter initialization (=�������� �ҷ��� ������ȯ üũ ī���� �ʱ�ȭ)
		}
		DoRampRef(&oStateRamp2);										// Synchronous angular velocity increases (=���� ���ӵ� ����) 	
	}
	else if (wStateIndex == 1)
	{
		Q15 tqWrpmOld;
		if (wStateTmBuf > 0) wStateTmBuf--;

		tqWrpmOld = tqWrpmSync;
		qStateTheta = (((oRampWrpm.qTar)>=0)? (tqWrpmEst - qStateTheta) : (qStateTheta - tqWrpmEst));

			if ((abs(tqWrpmErr) <= Q_OMEGA_RPM_DRUM(RPMD_SYNC_MAX * 0.25)) && (qStateTheta > Q_OMEGA_RPM_DRUM(0)))
			{
				if ((((oRampWrpm.qTar)>=0)? tqThetaErr :-tqThetaErr) >= Q_THETA_R(60. * PI/180.)) wStateIndex |= 0x8000;
				else if ((((oRampWrpm.qTar)>=0)? tqThetaErr :-tqThetaErr) <= Q_THETA_R(-0. * PI/180.)) DoRampRef(&oStateRamp1); 
			}  /* */
			
		if (qStateTheta <= Q_OMEGA_RPM_DRUM(0))								// Additional increase in starting current when estimated speed decreases (=�����ӵ� ���ҽ� �⵿���� �߰� ����)
		{
//			nCntDbg1 |= 0x0001;
			DoRampRef(&oStateRamp1);							
		}

		nStateGain -= (int)(32768. *TS_CC / (T_MAX_STARTUP * 0.035));	// Startup Theta, speed Mix
		if (nStateGain < 0) nStateGain = 0;		
		tqTheta = (short)(tqThetaEst - (short)(MUL_SS(nStateGain, tqThetaErr) >> 15));
		tqWrpmSync = tqWrpmEst - (MUL_SS(nStateGain, tqWrpmErr) >> 15);
		
		if ((((oRampWrpm.qTar)>=0)? (tqWrpmSync - tqWrpmOld) : (tqWrpmOld - tqWrpmSync)) < Q_OMEGA_RPM_DRUM(0))
		{
//			nStateGain += (int)(32768. *TS_CC / (T_MAX_STARTUP * 0.035));
		//	if (nStateGain > 32768) nStateGain = 32768;
/*			if ((((oRampWrpm.qTar)>=0)? tqWrpmEst :-tqWrpmEst) < Q_OMEGA_RPM_DRUM(RPMD_SYNC_EXIT * 0.2))
			{
				nStateGain = 32768;
				nCntDbg1 |= 0x0004;
			}*/
//			nCntDbg1 |= 0x0002;
			DoRampRef(&oStateRamp1);									// Additional increase in starting current (=�⵿���� �߰� ����)
		}
		else SetRampRef(&oStateRamp2, tqWrpmSync); //

		if (/*(oIdqse.qD <= Q_CURRENT(0.0)) ||*/ (abs((short)(tqThetaEst - tqTheta)) <= Q_THETA_R(5. * PI/180.)) // Changes to operating status upon completion of D-axis current reduction (=D������ ���� �Ϸ�� �������·� ����)
			/* && (abs(tqWrpmErr) <= Q_OMEGA_RPM_DRUM(RPMD_SYNC_EXIT * 0.2))*/ 
			&&(qStateTheta > Q_OMEGA_RPM_DRUM(0)) || ((((oRampWrpm.qTar)>=0)? tqWrpmSync :-tqWrpmSync) >= Q_OMEGA_RPM_DRUM(RPMD_SYNC_MAX)))
////////		if (nStateGain == 0)	
		{
			wStateCnt = 0;
			if ((((oRampWrpm.qTar)>=0)? tqWrpmSync :-tqWrpmSync) >= Q_OMEGA_RPM_DRUM(RPMD_SYNC_EXIT * 1.4))
			{			
				tqTheta = tqThetaEst;
				tqWrpmSync = tqWrpmEst;
				wStateIndex = 2;
			// 	SET_LPF1_GAIN(oEstimate.oLpfWr, WC_LPF_WR_FLUX, TS_CC); 
				EntryStateRun(); return;  // 
			}
		}
/*		else if (abs(tqThetaErr) > Q_THETA_R(90. * PI/180.))
		{
			if ((tqThetaErr * oRampWrpm.qTar) >= 0) nCntDbg1 |= 0x0010;
			else nCntDbg1 |= 0x0020;
			if (++wStateCnt > (WORD)(T_MAX_STARTUP * 0.045 / TS_CC))
			{
				nCntDbg1 |= 0x0100;
				wFailFlagBridge |= FAILFLAG_STARTUP;
				EntryStateStop();
				return;
			}
		}*/
		qStateTheta = tqWrpmEst;
		DoRampRef(&oStateRamp2);										// Synchronous angular velocity increases (=���� ���ӵ� ����) 
		if ((((oRampWrpm.qTar)>=0)? tqThetaErr :-tqThetaErr) < Q_THETA_R(0. * PI/180.)) tqTheta -= (tqThetaErr >>1);
	}
/*	else
	{
		if (wStateTmBuf > 0) wStateTmBuf--;
		tqTheta = tqThetaEst;
		tqWrpmSync = tqWrpmEst;

		if (((((oRampWrpm.qTar)>=0)? tqWrpmSync :-tqWrpmSync) >= Q_OMEGA_RPM_DRUM(RPMD_SYNC_MAX)) || (wStateTmBuf == 0))
		{
			EntryStateRun();
			return;
		}
		if ((((oRampWrpm.qTar)>=0)? (tqWrpmEst - qStateTheta) : (qStateTheta - tqWrpmEst)) <= Q_OMEGA_RPM_DRUM(0))
		{
			DoRampRef(&oStateRamp1);									// Additional increase in starting current when estimated speed decreases (=�����ӵ� ���ҽ� �⵿���� �߰� ����)
		}
		qStateTheta = tqWrpmEst;
		
		SetRampRef(&oStateRamp2, tqWrpmSync);							// Set synchronous angular velocity measurement speed (=���� ���ӵ� �����ӵ��� ����)  
	} */
	if ((((oRampWrpm.qTar)>=0)? tqWrpmSync :-tqWrpmSync) >= abs(oRampWrpm.qTar))
	{
		EntryStateRun();
		return;
	} 

	SetAngle(&oAngle, tqTheta);										// Change current control magnetic flux angle to synchronous acceleration magnetic flux angle (=���Ⱑ�� �ڼӰ����� �������� �ڼӰ� ����) 
	ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	
	CtrlMTPA(oStateRamp1.qRef, &oIdqseRef);							//MTPA control (=MTPA ����) 

	if (wStateIndex & 0x8000) wStateIndex &= ~0x8000;
	else DoRampRef(&oStateRamp1);										// Increase/decrease starting current (=�⵿���� ���� / ����) 	
//	DoRampRef(&oStateRamp2);										// Synchronous angular velocity increases (=���� ���ӵ� ����) 
	
	qdStateInteg +=  MUL_SS(tqWrpmSync, Q_OMEGAINTEG_TS);			//  Synchronous acceleration magnetic flux angle increases (=���Ⱑ�� �ڼӰ� ����) 	
	
	SetRampRef(&oRampWrpm, tqWrpmSync);
	qWrpmActual = tqWrpmSync;
}

#else

void EntryStateStartup(void)
{
	if (fpDoTsState != DoStateStartup)
	{
		Q15 tqTheta;
		Q30 tqdInteg;
		
		fpDoTsState = DoStateStartup;		
		wStateTmBuf = (WORD)(T_MAX_STARTUP / TS_CC);		// Startup time limit setting (=�⵿ ���� �ð� ����)  	
		wStateCnt = 0;
		wStateIndex = 0;
		nStateGain = 0;

		SetRampTarget(&oRampWrpm, qCtrlParam1, qCtrlParam2);	
		
		SetRampTarget(&oStateRamp1, SIGN(oRampWrpm.qTar) * Q_CURRENT(I_STARTUP_INIT), -1);	//  Starting current initial/target value setting (=�⵿ ���� �ʱ�/��ǥ�� ����)   
		SetRampTarget(&oStateRamp1, SIGN(oRampWrpm.qTar) * Q_CURRENT(I_STARTUP_MAX), Q_SLOPE_CURRENT(SLOPE_I_SYNC));

		SetRampTarget(&oStateRamp2, Q_OMEGA_RPM_DRUM(0.), -1);								// Starting synchronous acceleration setting (=�⵿ ���� ���ӵ� ����) 
		SetRampTarget(&oStateRamp2, SIGN(oRampWrpm.qTar) * Q_OMEGA_RPM_DRUM(RPMD_SYNC_MAX), 
										Q_SLOPE_RPM_DRUM(SLOPE_RPMD_SYNC));
		INIT_LPF1(oLpfState, Q_OMEGA_RPM_DRUM(0.0), (2.0 * PI * 5.0 / T_MAX_STARTUP), TS_CC);
		qWrpmActual = Q_OMEGA_RPM_DRUM(0.0);
		
		qdStateInteg = (long)(qStateTheta + (SIGN(oRampWrpm.qTar) * Q_THETA_R(-PI /2.))) << SCALE_OMEGAINTEG;	// Initial start-up electrical angle setting (=�⵿ �ʱ� ���Ⱒ ����) 
		tqTheta = (Q15)(qdStateInteg >> SCALE_OMEGAINTEG);
		SetAngle(&oAngle, tqTheta);
		// DO_LPF1(oLpfState, oStateRamp2.qRef, qWrpmActual);
		//qWrpmActual = oStateRamp2.qRef;
		ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	

		oIdqseRef.qD = Q_CURRENT(0.0);
		oIdqseRef.qQ = oStateRamp1.qRef;

		tqdInteg = oCtrlPiIde.qdInteg;
		oCtrlPiIde.qdInteg = oCtrlPiIqe.qdInteg;
		oCtrlPiIqe.qdInteg = (SIGN(oRampWrpm.qTar) * tqdInteg);

		if (oRampWrpm.qTar >= 0) SetEstFlag(&oEstimate, SLFLAG_WREF_POSITIVE);
		else SetEstFlag(&oEstimate, SLFLAG_WREF_NEGATIVE);
		
		wMcFlag = (MCFLAG_CC_ON | MCFLAG_CC_VFF);	
	}
	else if (SIGN(qCtrlParam1) != SIGN(oRampWrpm.qTar))
	{
		EntryStateReady();
		return;
	}
	else SetRampTarget(&oRampWrpm, qCtrlParam1, qCtrlParam2);
}


void DoStateStartup(void)
{
	Q15 tqTheta;
	tqTheta = (qdStateInteg >> SCALE_OMEGAINTEG);			// Synchronous acceleration magnetic flux angle calculation (=���� ���� �ڼӰ� ���)  
	DO_LPF1(oLpfState, oStateRamp2.qRef, qWrpmActual);		// Change current speed with synchronous angular speed filter value (=���� ���ӵ� ���Ͱ����� ����ӵ� ����) 
	
	if (wStateTmBuf < (WORD)(T_MAX_STARTUP / 3.0 / TS_CC))
	{
		if ((((oRampWrpm.qTar > 0)&&(oEstimate.qWrpmLpf <= 0))||((oRampWrpm.qTar < 0)&&(oEstimate.qWrpmLpf >= 0)))
			||(wStateTmBuf == 0))			// Start failure (reverse rotation sensorless detection, start time exceeded) (=�⵿����(��ȸ�� �������� ����, �⵿�ð� �ʰ���))
		{
			wFailFlagBridge |= FAILFLAG_STARTUP;
			EntryStateStop();
			return;
		}
	}
/**/
	if (wStateIndex == 0)
	{
		wStateTmBuf--;		// Decrease startup error check time counter (=�⵿���� üũ �ð�ī���� ����) 
		if ((MUL_SS(oRampWrpm.qTar, oIdqse.qQ) > 0) && /*(oIdqse.qD >= Q_CURRENT(0.0)) &&*/
			(abs(qWrpmActual - oEstimate.qWrpmLpf) <= Q_OMEGA_RPM_DRUM(RPMD_SYNC_MAX * 0.2))) // 0.1)))
		{	// If the sensorless magnetic flux angle rotation direction is within 90 degrees and the sensorless speed deviation is within a certain amount, the command current reduction status changes. (=�������� �ڼӰ� ȸ������ 90�� �̳�, �������� �ӵ� ���� �����̳� �̸� ���� ���� ���� ���� ����)
			if (++wStateCnt >= (WORD)(T_MAX_STARTUP * 0.01 / TS_CC)) // 0.008
			{
				if (abs(qWrpmActual) >= abs(oRampWrpm.qTar))
				{
					SetRampTarget(&oStateRamp2, oRampWrpm.qTar, -1);		// When the synchronous angular speed reaches the target speed, the synchronous angular speed changes to the target speed. (=���Ⱒ�ӵ��� ��ǥ�ӵ� ���޽� ���Ⱒ�ӵ� ��ǥ�ӵ��� ����) 
					SetRampTarget(&oStateRamp1, Q_CURRENT(0.0), Q_SLOPE_CURRENT(I_STARTUP_MAX * 2. / T_MAX_STARTUP));	// Command current reduction (=�������� ����) 
					wStateCnt = 0;
					wStateIndex = 1;
				}
				else if ((abs(qWrpmActual) >= Q_OMEGA_RPM_DRUM(RPMD_SYNC_EXIT)) && (oIdqse.qD >= Q_CURRENT(0.0))) /**/ 
				{
					SetRampTarget(&oStateRamp1, Q_CURRENT(0.0), Q_SLOPE_CURRENT(I_STARTUP_MAX * 8. / T_MAX_STARTUP));	// Command current reduction (=�������� ����) 
					wStateCnt = 0;
					wStateIndex = 1;
				}
			}
			if ((wStateTmBuf == 0) && (wStateCnt < (WORD)(T_MAX_STARTUP * 0.02 / TS_CC)))
			{
				wStateTmBuf = 1;	// Startup time exceeding errors prohibit (=�⵿�ð� �ʰ� ���� ����)
			}
		}
		else 
		{
			wStateCnt = 0;		// In case of sensorless failure, status change check counter initialization (=�������� �ҷ��� ������ȯ üũ ī���� �ʱ�ȭ)  
		}
	}
	else
	{
		if (MUL_SS(oRampWrpm.qTar, oIdqse.qQ) < 0)			// Starting failure when moving beyond the sensorless magnetic flux angle rotation direction while D-axis current is decreasing (=D������ ������ �������� �ڼӰ� ȸ������ ���ķ� �̵��� �⵿����)
		{
			if (++wStateCnt >= (WORD)(T_MAX_STARTUP * 0.01 / TS_CC))
			{
				wFailFlagBridge |= FAILFLAG_STARTUP;
				EntryStateStop();
				return;
			}
		}
		else 
		{
			wStateCnt = 0;	
			if ((oIdqse.qD <= Q_CURRENT(0.0)) || (abs((short)(tqTheta - oEstimate.oAngle.qTheta)) <= Q_THETA_R(5. * PI/180.)) // Changes to operating status upon completion of D-axis current reduction (=D������ ���� �Ϸ�� �������·� ����)  
				|| ((Q_CURRENT(IS_MIN) != 0) && (abs(oIdqseRef.qQ) <= Q_CURRENT(IS_MIN))))
			{			
				EntryStateRun();
				return;
			}
		}/**/ 

		nStateGain += (int)(32768. / (T_MAX_STARTUP * I_STARTUP_INIT / I_STARTUP_MAX / 16.0 / TS_CC));	// Startup Theta Mix
		if (nStateGain > 32768) nStateGain = 32768;
		tqTheta = tqTheta + (MUL_SS(nStateGain, (short)(oEstimate.oAngle.qTheta - tqTheta)) >> 15);
	}

//	tqTheta = (qdStateInteg >> SCALE_OMEGAINTEG);
	SetAngle(&oAngle, tqTheta);										// Change current control magnetic flux angle to synchronous acceleration magnetic flux angle (=���Ⱑ�� �ڼӰ����� �������� �ڼӰ� ����) 
//	DO_LPF1(oLpfState, oStateRamp2.qRef, qWrpmActual);
//	qWrpmActual = oStateRamp2.qRef;
	ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	

//	oIdqseRef.qD = Q_CURRENT(0.0);
//	oIdqseRef.qQ = oStateRamp1.qRef;
	CtrlMTPA(oStateRamp1.qRef, &oIdqseRef);							// MTPA control

	DoRampRef(&oStateRamp1);										// Increase/decrease starting current (=�⵿���� ���� / ����) 	
	DoRampRef(&oStateRamp2);										// Synchronous angular velocity increases (=���� ���ӵ� ����)	
//	qdStateInteg +=  MUL_SS(oStateRamp2.qRef, Q_OMEGAINTEG_TS);		// Increased starting electric angle (=�⵿���Ⱒ ����) 		
	qdStateInteg +=  MUL_SS(qWrpmActual, Q_OMEGAINTEG_TS);			//  Synchronous acceleration magnetic flux angle increases (=���Ⱑ�� �ڼӰ� ����) 	
	
	SetRampRef(&oRampWrpm, qWrpmActual);
}
#endif

#else // #if defined(_EST_SENSORLESS_) && defined(_SYNC_ACCEL_)

/***************************************************************************************************
Name:        EntryStateStartup  
Description: Enters Q-axis current increase start state (=Q������ ���� �⵿ ���� ����)
***************************************************************************************************/
void EntryStateStartup(void)
{

	if (fpDoTsState != DoStateStartup)
	{
		fpDoTsState = DoStateStartup;		
		wStateTmBuf = (WORD)(motor_parameter[parameter_index_id].T_MAX_STARTUP_id / TS_CC);		// Startup time limit setting (=�⵿ ���� �ð� ����) 

		SetRampTarget(&oRampWrpm, qCtrlParam1, qCtrlParam2);	
		
		SetRampTarget(&oStateRamp1, SIGN(oRampWrpm.qTar) * Q_CURRENT(motor_parameter[parameter_index_id].I_STARTUP_INIT_id), -1);		// Starting current initial value (=�⵿ ���� �ʱⰪ) 
		SetRampTarget(&oStateRamp1, SIGN(oRampWrpm.qTar) * Q_CURRENT(motor_parameter[parameter_index_id].I_STARTUP_MAX_id), 			// Starting current target/slope setting (=�⵿ ���� ��ǥ/ ���� ����)
										Q_SLOPE_CURRENT((motor_parameter[parameter_index_id].I_STARTUP_MAX_id - motor_parameter[parameter_index_id].I_STARTUP_INIT_id) / motor_parameter[parameter_index_id].T_MAX_STARTUP_id));

		oIdqseRef.qD = Q_CURRENT(0.0);
		oIdqseRef.qQ = oStateRamp1.qRef;

		if (oRampWrpm.qTar >= 0) SetEstFlag(&oEstimate, SLFLAG_WREF_POSITIVE);
		else SetEstFlag(&oEstimate, SLFLAG_WREF_NEGATIVE);

		wMcFlag = (MCFLAG_CC_ON | MCFLAG_CC_VFF);	
	}
	else if (SIGN(qCtrlParam1) != SIGN(oRampWrpm.qTar))
	{
		EntryStateReady();
		return;
	}
	else SetRampTarget(&oRampWrpm, qCtrlParam1, qCtrlParam2);
}

void DoStateStartup(void)
{
	if (wStateTmBuf < (WORD)(motor_parameter[parameter_index_id].T_MAX_STARTUP_id / 3.0 / TS_CC))
	{
		if ((((oRampWrpm.qTar > 0)&&(qWrpmActual <= 0))||((oRampWrpm.qTar < 0)&&(qWrpmActual >= 0)))
			||(wStateTmBuf == 0))			// Start-up failure (=�⵿����) 
		{
			wFailFlagBridge |= FAILFLAG_STARTUP;
			EntryStateStop();
			return;
		}
	}
	wStateTmBuf--;
	
//	oIdqseRef.qQ = oStateRamp1.qRef;
	CtrlMTPA(oStateRamp1.qRef, &oIdqseRef);	// MTPA control  
	DoRampRef(&oStateRamp1);				// Increase starting current (=�⵿���� ����) 	
	
	SetRampRef(&oRampWrpm, qWrpmActual);

	if (SIGN(qWrpmActual) == SIGN(oRampWrpm.qTar))
	{
		if (abs(qWrpmActual) >= abs(oRampWrpm.qTar))
		{
			SetRampRef(&oRampWrpm, oRampWrpm.qTar);
			EntryStateRun();
		}
		else if (abs(qWrpmActual) >= Q_OMEGA_RPM_DRUM(RPMD_STARTUP_EXIT))/* @@@@@ */
		{
			EntryStateRun();
		}
	}
}

#endif

/***************************************************************************************************
Name:        EntryStateRun  
Description: Enter driving state (=���� ���� ����)
***************************************************************************************************/
void EntryStateRun(void)
{

	void (*tfpStateOld)(void);
	tfpStateOld = fpDoTsState;

	if (fpDoTsState != DoStateRun)
	{
		fpDoTsState = DoStateRun;	
		wSpeedFailCnt = 0;
		wOverLoadFailCnt = 0;

		// Field weakening controller initialization (=����� ����� �ʱ�ȭ) 
		SET_PIL1_GAIN(oCtrlPiFw, KP_FWC, KI_FWC, TS_SC, QMAX_VOLT, QMAX_CURRENT);
		oCtrlPiFw.qLimP = 0;
		oCtrlPiFw.qLimN = - Q_CURRENT(ID_FW_LIMIT);
		oCtrlPiFw.qdInteg = 0;
		// Speed controller initialization (=�ӵ� ����� �ʱ�ȭ) 
		oCtrlPiSpd.qVff = 0;
		if (tfpStateOld == DoStateStartup) oCtrlPiSpd.qdInteg = (Q30)oIdqseRef.qQ << SCALE_PIA1_KICT;
		else oCtrlPiSpd.qdInteg = 0;

#if !defined(_NON_CC_) || defined(_EST_SENSORLESS_)	// Offset error check during sensorless estimation or current control (=�������� ���� �Ǵ� ��������� Offset ���� üũ) 
		if (wReturnADC & ADC_OFFSET_NG)
		{
			wFailFlagBridge |= FAILFLAG_OFFSET;
			EntryStateStop();
			return;			
		}
#endif
	}
	if ((wMcFlag & MCFLAG_CC_ON) == 0) 	// Current controller initialization (=���� ����� �ʱ�ȭ) 
	{
		SET_PIA1_GAIN(oCtrlPiIde, KP_CC_D, KI_CC_D, KA_CC_D, TS_CC, QMAX_CURRENT, QMAX_VOLT);
		oCtrlPiIde.qdInteg = 0;
		oCtrlPiIde.qVref = 0;
		SET_PIA1_GAIN(oCtrlPiIqe, KP_CC_Q, KI_CC_Q, KA_CC_Q, TS_CC, QMAX_CURRENT, QMAX_VOLT);
		oCtrlPiIqe.qdInteg = 0;
		oCtrlPiIqe.qVref = 0;
		INIT_LPF1(oLpfIerr, Q_CURRENT(0.0), 5.0 *2.0 *PI, TS_CC);
	}
	if (eMcCtrlMode == MTR_MODE_CLOSEDLOOP_HALL)
	{
		if ((qCtrlParam2 >= 0)&&(((qCtrlParam1 > 0)&&(oRampWrpm.qTar < 0))||((qCtrlParam1 < 0)&&(oRampWrpm.qTar > 0))))
		{								// When commanding reverse rotation (=��ȸ�� ���ɽ�) 
			EntryStateReady();
			return;
		}
		SetRampTarget(&oRampWrpm, qCtrlParam1, qCtrlParam2);
		if (tfpStateOld == DoStateRun)
		{
			if ((wMcFlag & MCFLAG_FWC_ON) == 0) 
			{
				if (wMcFlag & MCFLAG_CC_ON) oCtrlPiFw.qdInteg = (Q30)oIdqseRef.qD << SCALE_PIL1_KPIT;
				else oCtrlPiFw.qdInteg = (Q30)oIdqse.qD << SCALE_PIL1_KPIT;
			}
			if ((wMcFlag & MCFLAG_SC_ON) == 0) 
			{
				if (wMcFlag & MCFLAG_CC_ON) oCtrlPiSpd.qdInteg = (Q30)oIdqseRef.qQ << SCALE_PIA1_KICT;
				else oCtrlPiSpd.qdInteg = (Q30)oIdqse.qQ << SCALE_PIA1_KICT;
			}
		}
		wMcFlag = (MCFLAG_CC_ON | MCFLAG_CC_VFF | MCFLAG_FWC_ON | MCFLAG_SC_ON);	//
		// Speed controller gain initialization (=�ӵ� ����� ���� �ʱ�ȭ) 
		if (abs(oRampWrpm.qRef) < Q_OMEGA_RPM_DRUM(RPMD_ROLL_EXIT))
		{
			SET_PIA1_GAIN(oCtrlPiSpd, (motor_parameter[parameter_index_id].KP_SC_ROLL_id), (motor_parameter[parameter_index_id].KI_SC_ROLL_id), (motor_parameter[parameter_index_id].KA_SC_ROLL_id), TS_SC, QMAX_OMEGA_RD, QMAX_TORQUE);
			wMcFlag |= MCFLAG_SC_ROLL_GAIN;
		}
		else
		{
			SET_PIA1_GAIN(oCtrlPiSpd, (motor_parameter[parameter_index_id].KP_SC_id), (motor_parameter[parameter_index_id].KI_SC_id), (motor_parameter[parameter_index_id].KA_SC_id), TS_SC, QMAX_OMEGA_RD, QMAX_TORQUE);
		}
		if (oRampWrpm.qTar >= 0) SetEstFlag(&oEstimate, SLFLAG_WREF_POSITIVE);
		else SetEstFlag(&oEstimate, SLFLAG_WREF_NEGATIVE);
	}
	else
	{
		if (eMcCtrlMode == MTR_MODE_OPENLOOP_LANGLEVOLT)
		{
			TAngle	toAngle;
			Q15	tqVolt;
			
			SetAngle(&toAngle, qCtrlParam1);
			
			tqVolt = MUL_SS(Q15_FRAC(1.0 / SQRT3), qVdcBus) >> 15;
			if (qCtrlParam2 > tqVolt) tqVolt = tqVolt;			
			else if (qCtrlParam2 < -tqVolt) tqVolt = -tqVolt;
			else tqVolt = qCtrlParam2;
			
			oVdqse.qD = MUL_SS(toAngle.qCos, tqVolt) >> 15;
			oVdqse.qQ = MUL_SS(toAngle.qSin, tqVolt) >> 15;
			
			oIdqseRef.qQ = oIdqse.qQ;
			oIdqseRef.qD = oIdqse.qD;		
			
			wMcFlag = 0;	//
		}
		else if (eMcCtrlMode == MTR_MODE_OPENLOOP_TORQUE)
		{
			oIdqseRef.qQ = qCtrlParam1;
			if (tfpStateOld == DoStateRun)
			{
				if ((wMcFlag & MCFLAG_FWC_ON) == 0) 
				{
					if (wMcFlag & MCFLAG_CC_ON) oCtrlPiFw.qdInteg = (Q30)oIdqseRef.qD << SCALE_PIL1_KPIT;
					else oCtrlPiFw.qdInteg = (Q30)oIdqse.qD << SCALE_PIL1_KPIT;
				}
			}			
			wMcFlag = (MCFLAG_CC_ON | MCFLAG_CC_VFF | MCFLAG_FWC_ON);	//
		}
		else if (eMcCtrlMode == MTR_MODE_OPENLOOP_CURRENT)
		{
			oIdqseRef.qD = qCtrlParam1;
			oIdqseRef.qQ = qCtrlParam2;
			
			wMcFlag = (MCFLAG_CC_ON | MCFLAG_CC_VFF);	//
		}

		wSpeedFailCnt = 0;
		SetRampTarget(&oRampWrpm, qWrpmActual, -1);
		SetEstFlag(&oEstimate, SLFLAG_WREF_NON);
	}
}

void DoStateRun(void)
{
	if (eMcCtrlMode == MTR_MODE_CLOSEDLOOP_HALL)
	{
		if ((oRampWrpm.qTar == Q_OMEGA_RPM(0.0)) && (abs(qWrpmActual) <= Q_OMEGA_RPM_DRUM(12.0)))
		{	// If the target speed is Zero and the measured speed is below a certain level, it switches to Stop status. (=��ǥ�ӵ��� Zero�̸� �����ӵ��� ���� �����̸� Stop���·� ��ȯ)
			eMcCtrlMode = MTR_MODE_STOP_BRAKE;
			EntryStateStop();
			return;
		}
/*		if (abs(oRampWrpm.qRef - qWrpmActual) > Q_OMEGA_RPM_DRUM(RPMD_SPEED_FAIL))
		{
			wSpeedFailCnt += 2;
			if (wSpeedFailCnt >= (WORD)(0.16 * 2 / TS_CC))	// Failure due to excessive speed difference of 0.16 seconds (=0.16�� �ӵ��� ���� ����) 
			{
				wFailFlagBridge |= FAILFLAG_SPEED;
				EntryStateStop();
				return;
			}
		}
		else if (wSpeedFailCnt) wSpeedFailCnt--;
*/
		if (wMcFlag & MCFLAG_SC_ROLL_GAIN)
		{
			if (abs(oRampWrpm.qRef) >= Q_OMEGA_RPM_DRUM(RPMD_ROLL_EXIT * 1.04))
			{
				SET_PIA1_GAIN(oCtrlPiSpd, (motor_parameter[parameter_index_id].KP_SC_id), (motor_parameter[parameter_index_id].KI_SC_id), (motor_parameter[parameter_index_id].KA_SC_id), TS_SC, QMAX_OMEGA_RD, QMAX_TORQUE);

				wMcFlag &= ~MCFLAG_SC_ROLL_GAIN;
			}
		}
		else
		{
			if (abs(oRampWrpm.qRef) < Q_OMEGA_RPM_DRUM(RPMD_ROLL_EXIT * 0.96))
			{
				SET_PIA1_GAIN(oCtrlPiSpd, (motor_parameter[parameter_index_id].KP_SC_ROLL_id), (motor_parameter[parameter_index_id].KI_SC_ROLL_id), (motor_parameter[parameter_index_id].KA_SC_ROLL_id), TS_SC, QMAX_OMEGA_RD, QMAX_TORQUE);
				wMcFlag |= MCFLAG_SC_ROLL_GAIN;
			}
		}
	}
	else
	{
		if (eMcCtrlMode == MTR_MODE_OPENLOOP_LANGLEVOLT)
		{
			TAngle	toAngle;
			Q15	tqVolt;
			
			SetAngle(&toAngle, qCtrlParam1);
			
			tqVolt = MUL_SS(Q15_FRAC(1.0 / SQRT3), qVdcBus) >> 15;
			if (qCtrlParam2 > tqVolt) tqVolt = tqVolt;			
			else if (qCtrlParam2 < -tqVolt) tqVolt = -tqVolt;
			else tqVolt = qCtrlParam2;
			
			oVdqse.qD = MUL_SS(toAngle.qCos, tqVolt) >> 15;
			oVdqse.qQ = MUL_SS(toAngle.qSin, tqVolt) >> 15;
			
			oIdqseRef.qQ = oIdqse.qQ;
			oIdqseRef.qD = oIdqse.qD;
		}
		SetRampTarget(&oRampWrpm, qWrpmActual, -1);
	}
	// Check operation failure due to motor restraint (=���� ���ӿ� ���� ���� ���� üũ)
/*	if (MUL_SS(oRampWrpm.qRef, qWrpmActual) <= 0)
	{
		wOverLoadFailCnt += 6;
		if (wOverLoadFailCnt >= (WORD)(T_OVERLOAD_FAIL * 6 / TS_CC))	// Restraint failure when motor stops and reverses (=���� ���� �� ��ȸ���� ���� ����)
		{
			wFailFlagBridge |= FAILFLAG_OVERLOAD;
			EntryStateStop();
			return;
		}
	}
	else if (wOverLoadFailCnt) wOverLoadFailCnt--;
*/
}

/****************************************************************************
Name:        EntryStateTest
Description: Enter test state (=Test���� ����)
****************************************************************************/
void EntryStateTest(void)
{

	if (fpDoTsState != DoStateTest)
	{
		fpDoTsState = DoStateTest;

		qdStateInteg = (long)Q_THETA_R(0.0) << SCALE_OMEGAINTEG;		// Initial start-up electrical angle setting (=�⵿ �ʱ� ���Ⱒ ����) 
		SetAngle(&oAngle, (Q15)(qdStateInteg >> SCALE_OMEGAINTEG));
		ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	
		SetEstFlag(&oEstimate, SLFLAG_WREF_NON);
	}
	
	if (eMcCtrlMode == MTR_MODE_TEST_FREQUENCYVOLT)
	{
		Q15	tqVolt;

		tqVolt = MUL_SS(Q15_FRAC(1.0 / SQRT3), qVdcBus) >> 15;
		if (qCtrlParam2 > tqVolt) tqVolt = tqVolt;			
		else if (qCtrlParam2 < -tqVolt) tqVolt = -tqVolt;
		else tqVolt = qCtrlParam2;

		oVdqse.qD = tqVolt;
		oVdqse.qQ = Q_VOLT(0.0);

		oIdqseRef.qD = Q_CURRENT(0.0);
		oIdqseRef.qQ = Q_CURRENT(0.0);
		
		SetRampTarget(&oRampWrpm, qCtrlParam1, -1);
		qWrpmActual = qCtrlParam1;
		
		wMcFlag = 0;	//	
	}
	else if (eMcCtrlMode == MTR_MODE_TEST_FREQUENCYCURRENT)
	{
		if ((wMcFlag & MCFLAG_CC_ON) == 0) 						// Current controller initialization (=���� ����� �ʱ�ȭ) 
		{
			SET_PIA1_GAIN(oCtrlPiIde, KP_CC_D, KI_CC_D, KA_CC_D, TS_CC, QMAX_CURRENT, QMAX_VOLT);
			oCtrlPiIde.qdInteg = 0;
			oCtrlPiIde.qVref = 0;
			SET_PIA1_GAIN(oCtrlPiIqe, KP_CC_Q, KI_CC_Q, KA_CC_Q, TS_CC, QMAX_CURRENT, QMAX_VOLT);
			oCtrlPiIqe.qdInteg = 0;
			oCtrlPiIqe.qVref = 0;
			INIT_LPF1(oLpfIerr, Q_CURRENT(0.0), 5.0 *2.0 *PI, TS_CC);
		}
		oIdqseRef.qD = qCtrlParam2;
		oIdqseRef.qQ = Q_CURRENT(0.0);

		SetRampTarget(&oRampWrpm, qCtrlParam1, -1);
		qWrpmActual = qCtrlParam1;
		
		wMcFlag = (MCFLAG_CC_ON | MCFLAG_CC_VFF);	//
	}
	else
	{
		oVdqse.qD = Q_VOLT(0.0);
		oVdqse.qQ = Q_VOLT(0.0);	

		oIdqseRef.qD = Q_CURRENT(0.0);
		oIdqseRef.qQ = Q_CURRENT(0.0);
		
		wMcFlag = 0;	//
	}
}

void DoStateTest(void)
{
	Q15 tqTheta;
	Q15	tqVolt;

	if (eMcCtrlMode == MTR_MODE_TEST_FREQUENCYVOLT)
	{
		tqTheta = (Q15)(qdStateInteg >> SCALE_OMEGAINTEG);
		qdStateInteg +=  MUL_SS(qCtrlParam1, Q_OMEGAINTEG_TS);
		
		SetAngle(&oAngle, (short)tqTheta);
		ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	
		qWrpmActual = qCtrlParam1;

		tqVolt = MUL_SS(Q15_FRAC(1.0 / SQRT3), qVdcBus) >> 15;
		if (qCtrlParam2 > tqVolt) tqVolt = tqVolt;			
		else if (qCtrlParam2 < -tqVolt) tqVolt = -tqVolt;
		else tqVolt = qCtrlParam2;

		oVdqse.qD = tqVolt;
		oVdqse.qQ = Q_VOLT(0.0);

	}
	else if (eMcCtrlMode == MTR_MODE_TEST_FREQUENCYCURRENT)
	{
		tqTheta = (Q15)(qdStateInteg >> SCALE_OMEGAINTEG);
		qdStateInteg +=  MUL_SS(qCtrlParam1, Q_OMEGAINTEG_TS);
		
		SetAngle(&oAngle, (short)tqTheta);
		ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	
		qWrpmActual = qCtrlParam1;

		oIdqseRef.qD = qCtrlParam2;
		oIdqseRef.qQ = Q_CURRENT(0.0);
	}
}


#if defined(_EST_ENCODER_) && !defined(_NON_ENCODER_Z_)
#if 0
#ifndef I_ENCODER_OFFSET
#define I_ENCODER_OFFSET		(3.5)
#endif
/****************************************************************************
Name:        EntryStateEncoderOffset
Description: Encoder Offset angle acquisition (current angle control) (=Encoder Offset ���� ȹ�� (���� ���� ����))
****************************************************************************/
void EntryStateEncoderOffset(void)
{
	if (fpDoTsState != DoStateEncoderOffset)
	{
		fpDoTsState = DoStateEncoderOffset;

		qdStateInteg = (long)Q_THETA_R(0.0) << SCALE_OMEGAINTEG;					// Initial start-up electrical angle setting (=�⵿ �ʱ� ���Ⱒ ����) 
		SetAngle(&oAngle, (Q15)(qdStateInteg >> SCALE_OMEGAINTEG));
		ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	

		SET_PIA1_GAIN(oCtrlPiIde, KP_CC_D, KI_CC_D, KA_CC_D, TS_CC, QMAX_CURRENT, QMAX_VOLT);	// Current controller initialization (=���� ����� �ʱ�ȭ) 
		oCtrlPiIde.qdInteg = 0;
		oCtrlPiIde.qVref = 0;
		SET_PIA1_GAIN(oCtrlPiIqe, KP_CC_Q, KI_CC_Q, KA_CC_Q, TS_CC, QMAX_CURRENT, QMAX_VOLT);
		oCtrlPiIqe.qdInteg = 0;
		oCtrlPiIqe.qVref = 0;
		INIT_LPF1(oLpfIerr, Q_CURRENT(0.0), 5.0 *2.0 *PI, TS_CC);

		SetRampTarget(&oStateRamp1, Q_CURRENT(0.0), -1);							// Synchronous rotation current target/slope setting (=����ȸ�� ���� ��ǥ/ ���� ����)
		SetRampTarget(&oStateRamp1, Q_CURRENT(I_ENCODER_OFFSET), Q_SLOPE_CURRENT(I_ENCODER_OFFSET / 0.5)); 	
		oIdqseRef.qD = Q_CURRENT(0.0);
		oIdqseRef.qQ = Q_CURRENT(0.0);
		oVdqse.qD = Q_VOLT(0.0);
		oVdqse.qQ = Q_VOLT(0.0);

		SetRampTarget(&oRampWrpm, Q_OMEGA_RPM_DRUM(0.0), -1);
		qWrpmActual = Q_OMEGA_RPM_DRUM(0.0);

		nStateGain = 0;
		SetEstFlag(&oEstimate, SLFLAG_WREF_NON);

		wMcFlag = (MCFLAG_PWM_OFF | MCFLAG_AD_OFFSET);	//
	}
}

Q15 aqEncOffset[150];
void DoStateEncoderOffset(void)
{
	Q15 tqThetaOld;
	Q15 tqThetaTemp;
	long tdqSumTheta;

	if (wReturnADC & ADC_OFFSET_OK)
	{
		wMcFlag = (MCFLAG_CC_ON | MCFLAG_CC_VFF);	//
		oIdqseRef.qD = oStateRamp1.qRef;
		oIdqseRef.qQ = Q_CURRENT(0.0);
		DoRampRef(&oStateRamp1);

		tqThetaOld = qdStateInteg >> SCALE_OMEGAINTEG;
		if (oStateRamp1.qRef != oStateRamp1.qTar)									// Current rising section before synchronous rotation (=����ȸ�� ���� ���� ��� ����?) 
		{
			qWrpmActual = oRampWrpm.qRef;

			SetAngle(&oAngle, (Q15)(qdStateInteg >> SCALE_OMEGAINTEG));
			ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	
		}
		else																		// During synchronous rotation after current rise (=��������� ����ȸ�� �� ?) 
		{
			SetRampTarget(&oRampWrpm, Q_OMEGA_RPM_DRUM(6.0), -1);					// Synchronous rotation speed setting (=����ȸ�� �ӵ� ����)
			qWrpmActual = oRampWrpm.qRef;
			
			qdStateInteg +=  MUL_SS(qWrpmActual, Q_OMEGAINTEG_TS);
			SetAngle(&oAngle, (short)(qdStateInteg >> SCALE_OMEGAINTEG));
			ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	
		}

		tdqSumTheta = nStateGain + (short)(oAngle.qTheta - tqThetaOld);
		
		if (tdqSumTheta < (long)(POLE_PAIR * 1.5 * (1L << 16)))					// Motor less than 1.5 rotations (=���� 1.5ȸ�� �̸� ?)
		{
			tqThetaTemp = (Q15)(POLE_PAIR * 1.5 * (1L << 16));
			qStateTheta = MUL_UU((MUL_UU((tqThetaTemp & 0x7FFF), 3) >> 15), Q_THETA_R(PI / 3.)) | (tqThetaTemp & 0x8000);
			wStateTmBuf = 0;
			wStateCnt = 0;
			lStateSum = 0;
			qThetaOffsetEncoder = 0;
		}
		else																	// Motor 1.5 rotations or more (=���� 1.5ȸ�� �̻� ?)
		{
			tqThetaTemp = MUL_UU((MUL_UU((oAngle.qTheta & 0x7FFF), 3) >> 15), Q_THETA_R(PI / 3.)) | (oAngle.qTheta & 0x8000);
			if (qStateTheta == tqThetaTemp)										// Electric angle rotation of 60 degrees (=���Ⱒ 60�� ȸ�� ?)
			{
				SetRampTarget(&oRampWrpm, Q_OMEGA_RPM_DRUM(0.0), -1);			// Motor stopping speed setting (=���� ���� �ӵ� ����)
				qWrpmActual = oRampWrpm.qRef;
			
				qdStateInteg = (long)tqThetaTemp << SCALE_OMEGAINTEG;
				SetAngle(&oAngle, tqThetaTemp);
				ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	
				if (oObsEncoder.qWrpmLpf == Q_OMEGA_RPM(0.0))					// Encorder speed stop (=Encorder �ӵ� ���� ?)
				{
					if (++wStateTmBuf >= 50)
					{
						if (wStateCnt == 0)
						{														// When calculating the offset for the first time, save the default offset value (=���� Offet ����� Offset �⺻�� ����) 
							qStateVal = (short)(oAngle.qTheta - oObsEncoder.oAngle.qTheta);
							aqEncOffset[wStateCnt] = qStateVal;
							lStateSum = 0;
						}
						else 
						{
							Q15 qOffset = (short)(oAngle.qTheta - oObsEncoder.oAngle.qTheta - qStateVal);
							aqEncOffset[wStateCnt] = qOffset;
							lStateSum += qOffset;
						//	lStateSum += (short)(oAngle.qTheta - oObsEncoder.oAngle.qTheta - qStateVal);
						}
						wStateCnt++;
						tqThetaTemp = qStateTheta + Q_THETA_R(90.0 * PI / 180.);	// Start of electric 60 degree rotation (=���Ⱒ 60�� ȸ�� ����) 
						qStateTheta = MUL_UU((MUL_UU((tqThetaTemp & 0x7FFF), 3) >> 15), Q_THETA_R(PI / 3.)) | (tqThetaTemp & 0x8000);
						wStateTmBuf = 0;
					}
				}
				else wStateTmBuf = 0;
			}
			
			if (tdqSumTheta > (long)(POLE_PAIR * 2.5 * (1L << 16)))				// Motor 2.5 rotations (offset of 1 rotation completed) (=���� 2.5ȸ��(1ȸ�� ������ �ջ� �Ϸ�)?)
			{			
				qThetaOffsetEncoder = (short)(DIV_SD(lStateSum, wStateCnt) + qStateVal);
				qCtrlParam1 = 0;
				qCtrlParam2 = 0;
				eMcCtrlMode = MTR_MODE_STOP_BRAKE;
				SetRampTarget(&oRampWrpm, Q_OMEGA_RPM(0.0), -1);
				EntryStateStop();
			}
		}
		nStateGain += (short)(oAngle.qTheta - tqThetaOld);
	}

	//qStateTheta = MUL_UU((MUL_UU((qStateTheta & 0x7FFF), 3) >> 15), Q_THETA_R(PI / 3.)) | (qStateTheta & 0x8000);

}
#endif

/****************************************************************************
Name:        EntryStateEncoderOffset
Description: Encoder Offset angle acquisition (voltage angle control) (=Encoder Offset ���� ȹ�� (���� ���� ����))
****************************************************************************/
void EntryStateEncoderOffset(void)
{
	if (fpDoTsState != DoStateEncoderOffset)
	{
		fpDoTsState = DoStateEncoderOffset;

		qdStateInteg = (long)Q_THETA_R(0.0) << SCALE_OMEGAINTEG;					// Initial start-up electrical angle setting (=�⵿ �ʱ� ���Ⱒ ����) 
		SetAngle(&oAngle, (Q15)(qdStateInteg >> SCALE_OMEGAINTEG));
		ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	

		SetRampTarget(&oStateRamp1, Q_VOLT(0.0), -1);							// Synchronous rotation voltage target/slope setting (=����ȸ�� ���� ��ǥ/ ���� ����)
		SetRampTarget(&oStateRamp1, Q_VOLT(V_ENCODER_OFFSET), Q_VOLT(V_ENCODER_OFFSET / 0.5)); 	
		oIdqseRef.qD = Q_CURRENT(0.0);
		oIdqseRef.qQ = Q_CURRENT(0.0);
		oVdqse.qD = Q_VOLT(0.0);
		oVdqse.qQ = Q_VOLT(0.0);

		SetRampTarget(&oRampWrpm, Q_OMEGA_RPM_DRUM(0.0), -1);
		qWrpmActual = Q_OMEGA_RPM_DRUM(0.0);

		nStateGain = 0;
		SetEstFlag(&oEstimate, SLFLAG_WREF_NON);

		wMcFlag = (MCFLAG_PWM_OFF | MCFLAG_AD_OFFSET);	//
	}
}

void DoStateEncoderOffset(void)
{
	Q15 tqThetaOld;
	Q15 tqThetaTemp;
	long tdqSumTheta;

	if (wReturnADC & ADC_OFFSET_OK)
	{
		wMcFlag = 0;	//
		oVdqse.qD = oStateRamp1.qRef;
		oVdqse.qQ = Q_VOLT(0.0);
		DoRampRef(&oStateRamp1);

		tqThetaOld = qdStateInteg >> SCALE_OMEGAINTEG;
		if (oStateRamp1.qRef != oStateRamp1.qTar)								// Current rising section before synchronous rotation (=����ȸ�� ���� ���� ��� ����?) 
		{
			qWrpmActual = oRampWrpm.qRef;

			SetAngle(&oAngle, (Q15)(qdStateInteg >> SCALE_OMEGAINTEG));
			ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	
		}
		else																	// During synchronous rotation after current rise (=��������� ����ȸ�� �� ?) 
		{
			SetRampTarget(&oRampWrpm, Q_OMEGA_RPM_DRUM(6.0), -1);				// Synchronous rotation speed setting (=����ȸ�� �ӵ� ����)
			qWrpmActual = oRampWrpm.qRef;
			
			qdStateInteg +=  MUL_SS(qWrpmActual, Q_OMEGAINTEG_TS);
			SetAngle(&oAngle, (short)(qdStateInteg >> SCALE_OMEGAINTEG));
			ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	
		}

		tdqSumTheta = nStateGain + (short)(oAngle.qTheta - tqThetaOld);
		
		if (tdqSumTheta < (long)(POLE_PAIR * 1.5 * (1L << 16)))					// Less than 1.5 motor revolutions (=���� 1.5ȸ�� �̸� ?)
		{
			tqThetaTemp = (Q15)(POLE_PAIR * 1.5 * (1L << 16));
			qStateTheta = MUL_UU((MUL_UU((tqThetaTemp & 0x7FFF), 3) >> 15), Q_THETA_R(PI / 3.)) | (tqThetaTemp & 0x8000);
			wStateTmBuf = 0;
			wStateCnt = 0;
			lStateSum = 0;
			qThetaOffsetEncoder = 0;
		}
		else																	// Motor 1.5 rotations or more (=���� 1.5ȸ�� �̻� ?)
		{
			tqThetaTemp = MUL_UU((MUL_UU((oAngle.qTheta & 0x7FFF), 3) >> 15), Q_THETA_R(PI / 3.)) | (oAngle.qTheta & 0x8000);
			if (qStateTheta == tqThetaTemp)										// Electric angle rotation of 60 degrees (=���Ⱒ 60�� ȸ�� ?)
			{
				SetRampTarget(&oRampWrpm, Q_OMEGA_RPM_DRUM(0.0), -1);			// Motor stopping speed setting (=���� ���� �ӵ� ����)
				qWrpmActual = oRampWrpm.qRef;
			
				qdStateInteg = (long)tqThetaTemp << SCALE_OMEGAINTEG;
				SetAngle(&oAngle, tqThetaTemp);
				ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	
				if (oObsEncoder.qWrpmLpf == Q_OMEGA_RPM(0.0))					// Encorder speed stop (=Encorder �ӵ� ���� ?)
				{
					if (++wStateTmBuf >= 50)
					{
						if (wStateCnt == 0)
						{														// When calculating the offset for the first time, save the default offset value (=���� Offet ����� Offset �⺻�� ����) 
							qStateVal = (short)(oAngle.qTheta - oObsEncoder.oAngle.qTheta);
							lStateSum = 0;
						}
						else 
						{
							lStateSum += (short)(oAngle.qTheta - oObsEncoder.oAngle.qTheta - qStateVal);
						}
						wStateCnt++;
						tqThetaTemp = qStateTheta + Q_THETA_R(90.0 * PI / 180.);	// Start of electric 60 degree rotation (=���Ⱒ 60�� ȸ�� ����) 
						qStateTheta = MUL_UU((MUL_UU((tqThetaTemp & 0x7FFF), 3) >> 15), Q_THETA_R(PI / 3.)) | (tqThetaTemp & 0x8000);
						wStateTmBuf = 0;
					}
				}
				else wStateTmBuf = 0;
			}
			
			if (tdqSumTheta > (long)(POLE_PAIR * 2.5 * (1L << 16)))				// Motor 2.5 rotations (offset of 1 rotation completed) (=���� 2.5ȸ��(1ȸ�� ������ �ջ� �Ϸ�)?)
			{			
				qThetaOffsetEncoder = (short)(DIV_SD(lStateSum, wStateCnt) + qStateVal);
				qCtrlParam1 = 0;
				qCtrlParam2 = 0;
				eMcCtrlMode = MTR_MODE_STOP_BRAKE;
				SetRampTarget(&oRampWrpm, Q_OMEGA_RPM(0.0), -1);
				EntryStateStop();
			}
		}
		nStateGain += (short)(oAngle.qTheta - tqThetaOld);
	}
}
#endif // #if defined(_EST_ENCODER_)
#if 0	// D-axis inductance saturation detection (=D�� �����Ͻ� ��ȭ ����) 
/****************************************************************************
Name:        EntryStateInitPosit
Description: Entering initial position detection state (=�ʱ� ������ ���� ���� ����)
****************************************************************************/
void EntryStateInitPosit(void)
{
	if (fpDoTsState != DoStateInitPosit)
	{
		fpDoTsState = DoStateInitPosit;

		oIdqseRef.qD = Q_CURRENT(0.0);
		oIdqseRef.qQ = Q_CURRENT(0.0);
		oVdqse.qD = Q_VOLT(0.0);
		oVdqse.qQ = Q_VOLT(0.0);

		SetRampTarget(&oRampWrpm, Q_OMEGA_RPM_DRUM(0.0), -1);
		qWrpmActual = Q_OMEGA_RPM_DRUM(0.0);

		nStateGain = 0;
		wStateIndex = 0;
		wStateTmBuf = 0;

		SetEstFlag(&oEstimate, SLFLAG_WREF_NON);

		wMcFlag = (MCFLAG_PWM_OFF | MCFLAG_AD_OFFSET);	//
	}
}


void DoStateInitPosit(void)
{
	static Q15 qaInducBuf[6];
	if (wReturnADC & ADC_OFFSET_OK)
	{
		wMcFlag = 0;	//
		if (wStateIndex == 0) 		SetAngle(&oAngle, Q_THETA_R(   0. * PI / 180.));
		else if (wStateIndex == 1) 	SetAngle(&oAngle, Q_THETA_R(-180. * PI / 180.));
		else if (wStateIndex == 2) 	SetAngle(&oAngle, Q_THETA_R(  60. * PI / 180.));
		else if (wStateIndex == 3) 	SetAngle(&oAngle, Q_THETA_R(-120. * PI / 180.));
		else if (wStateIndex == 4) 	SetAngle(&oAngle, Q_THETA_R( 120. * PI / 180.));
		else if (wStateIndex == 5) 	SetAngle(&oAngle, Q_THETA_R( -60. * PI / 180.));
		ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	

		oVdqse.qD = Q_VOLT(0.0);
		oVdqse.qQ = Q_VOLT(0.0);
		wStateTmBuf++;
		if (wStateTmBuf >= 250) 
		{
			oVdqse.qD = MUL_SS(Q15_FRAC(1 / SQRT3), qVdcBus) >> 15;

			if (wStateTmBuf == 252) 
			{
				Q15 tqIs;
				// tqIs = SqrtS32(MUL_SS(oIdqse.qD, oIdqse.qD) + MUL_SS(oIdqse.qQ, oIdqse.qQ));
				tqIs = oIdqse.qD;
				qaInducBuf[wStateIndex] = DIV_UD(((long)qVdcBus <<8), tqIs);
				PWM_CON(OFF);

				wStateTmBuf = 0;
				if (++wStateIndex >= 6)
				{
					int tnCnt;
					Q15 tqInducMin = qaInducBuf[0];
					wStateIndex = 0;
					for (tnCnt = 1; tnCnt < 6; tnCnt++)
					{
						if (tqInducMin > qaInducBuf[tnCnt]) 
						{
							tqInducMin = qaInducBuf[tnCnt];
							wStateIndex = tnCnt;
						}
					}

					if (wStateIndex == 0) 		qStateTheta = Q_THETA_R(   0. * PI / 180.);
					else if (wStateIndex == 1) 	qStateTheta = Q_THETA_R(-180. * PI / 180.);
					else if (wStateIndex == 2) 	qStateTheta = Q_THETA_R(  60. * PI / 180.);
					else if (wStateIndex == 3) 	qStateTheta = Q_THETA_R(-120. * PI / 180.);
					else if (wStateIndex == 4) 	qStateTheta = Q_THETA_R( 120. * PI / 180.);
					else if (wStateIndex == 5) 	qStateTheta = Q_THETA_R( -60. * PI / 180.);
					
					eMcCtrlMode = MTR_MODE_STOP_BRAKE;
					EntryStateStop();				
				}
			}
		}
	}
}
#endif

#if 0	
/****************************************************************************
Name:        EntryStateInitPosit
Description: Entering initial position detection state (=�ʱ� ������ ���� ���� ����)
****************************************************************************/
void EntryStateInitPosit(void)
{
	if (fpDoTsState != DoStateInitPosit)
	{
		fpDoTsState = DoStateInitPosit;

		oIdqseRef.qQ = Q_CURRENT(0.0);
		oIdqseRef.qD = Q_CURRENT(0.0);
		
		if (eMcCtrlMode == MTR_MODE_TEST_FREQUENCYVOLT)
		{
			Q15	tqVolt;

			qdStateInteg = (long)Q_THETA_R(0.0) << SCALE_OMEGAINTEG;						// Initial start-up electrical angle setting (=�⵿ �ʱ� ���Ⱒ ����) 
			SetAngle(&oAngle, Q_THETA_R(0.0));
			ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	

			tqVolt = MUL_SS(Q15_FRAC(1.0 / SQRT3), qVdcBus) >> 15;
			if (qCtrlParam2 > tqVolt) tqVolt = tqVolt;			
			else if (qCtrlParam2 < -tqVolt) tqVolt = -tqVolt;
			else tqVolt = qCtrlParam2;

			oVdqse.qD = tqVolt;
			oVdqse.qQ = Q_VOLT(0.0);
		}
		else
		{
			oVdqse.qD = Q_VOLT(0.0);
			oVdqse.qQ = Q_VOLT(0.0);
		}
		SetEstFlag(&oEstimate, SLFLAG_WREF_NON);
		
		wMcFlag = 0;	//
	}
	
	SetRampTarget(&oRampWrpm, Q_OMEGA_RPM(0.0), -1);
	qWrpmActual = Q_OMEGA_RPM(0.0);
}

Q30 qdIsMax;
short qThetaMax;
Q15 qIsEst;
short qThetaEst;
short qThetaBuf;
		
void DoStateInitPosit(void)
{
	Q15 tqTheta;
	Q15	tqVolt;

	if (eMcCtrlMode == MTR_MODE_TEST_FREQUENCYVOLT)
	{
		TAngle toAngle;
		tqTheta = (Q15)(qdStateInteg >> SCALE_OMEGAINTEG);
		qdStateInteg +=  MUL_SS(qCtrlParam1, Q_OMEGAINTEG_TS);
		SetAngle(&toAngle, (short)tqTheta);
		
		SetAngle(&oAngle, Q_THETA_R(0.0));
		ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	
		qWrpmActual = Q_OMEGA_RPM(0.0);

		tqVolt = MUL_SS(Q15_FRAC(1.0 / SQRT3), qVdcBus) >> 15;
		if (qCtrlParam2 > tqVolt) tqVolt = tqVolt;			
		else if (qCtrlParam2 < -tqVolt) tqVolt = -tqVolt;
		else tqVolt = qCtrlParam2;

		oVdqse.qD = MUL_SS(tqVolt, toAngle.qCos) >> 15;
		oVdqse.qQ = MUL_SS(tqVolt, toAngle.qSin) >> 15; // Q_VOLT(0.0); // 

		{	
			Q30 tqdIs;
			tqdIs = MUL_SS(oIdqss.qD, oIdqss.qD) + MUL_SS(oIdqss.qQ, oIdqss.qQ);
			if (qdIsMax < tqdIs) 
			{
				qdIsMax = tqdIs;
				qThetaMax = ATan2(oIdqss.qD, oIdqss.qQ);
			}
			if ((qThetaBuf < 0) && (toAngle.qTheta >= 0))
			{
				qThetaEst = qThetaMax;
				qIsEst = SqrtS32(qdIsMax);
				qdIsMax = 0;
			}
			qThetaBuf = toAngle.qTheta;
		}
	}
}
#endif

/****************************************************************************
Name:        StartMotorControl
Description: Motor control interface functions (=���� ��Ʈ�� �������̽� �Լ�)
****************************************************************************/

BYTE counter=0;
PUBLIC void DevSetMotorDrive(EMcCtrlMode xeMode, Q15 xqParam1, Q15 xqParam2)
{
	TCpuSR cpuSR = 0;
	CPU_ENTER_CRITICAL();	
	eMcCtrlMode = xeMode;
	qCtrlParam1 = xqParam1;
	qCtrlParam2 = xqParam2;
	if (wFailFlagInt)
	{
		wFailFlagLoop &= ~wFailFlagInt;
		EntryStateStop();
	}
	else if (eMcCtrlMode == MTR_MODE_CLOSEDLOOP_HALL)
	{
		if (qCtrlParam2 < 0)
		{
			EntryStateRun();
		}
		else if (qCtrlParam1 == Q_OMEGA_RPM(0.0))
		{
			if (fpDoTsState != DoStateRun)
			{
				eMcCtrlMode = MTR_MODE_STOP_BRAKE;
				EntryStateStop();
			}
			else EntryStateRun();
		}
		else if ((fpDoTsState == DoStateStop)||(fpDoTsState == DoStateReady)||(fpDoTsState == DoStateTest))
		{
			EntryStateReady();
		}
		else if (fpDoTsState == DoStateAlign)
		{
			EntryStateAlign();
		}
		else if (fpDoTsState == DoStateStartup)
		{
			EntryStateStartup();
		}
		else if (fpDoTsState == DoStateRun)
		{
			EntryStateRun();
		}
	}
	else if (eMcCtrlMode == MTR_MODE_OPENLOOP_LANGLEVOLT)
	{
		EntryStateRun();
	}
	else if (eMcCtrlMode == MTR_MODE_OPENLOOP_TORQUE)
	{
		EntryStateRun();
	}
	else if (eMcCtrlMode == MTR_MODE_OPENLOOP_CURRENT)
	{
		EntryStateRun();
	}	
	else if ((eMcCtrlMode == MTR_MODE_TEST_FREQUENCYVOLT) || (eMcCtrlMode == MTR_MODE_TEST_FREQUENCYCURRENT))
	{
		EntryStateTest();
	}
	else // if ((eMcCtrlMode == MTR_MODE_STOP_OPEN)||(eMcCtrlMode == MTR_MODE_STOP_BRAKE))
	{
		if (wMcSensId) DevSetMotorSensing(0, 0);
		EntryStateStop();
	}
	CPU_EXIT_CRITICAL();
}


/****************************************************************************
Name:        ChangeMcVariable
Description: Change motor control variables (=�������� ���� ����)  
****************************************************************************/
PUBLIC void ChangeMcVariable(EMcVariable xeVar, Q15 *xpqVar)
{
	TCpuSR cpuSR = 0;
	CPU_ENTER_CRITICAL();	
	if (xeVar == MTR_VAR_THETA_TEST)	//TEST state electrical angle setting (8) (=TEST���� ���Ⱒ ���� (8))
	{
		if (fpDoTsState == DoStateTest)
		{
			qdStateInteg = (long)xpqVar[0] << SCALE_OMEGAINTEG;			
			SetAngle(&oAngle, (Q15)(qdStateInteg >> SCALE_OMEGAINTEG));
			ConvDQS2DQE(&oIdqss, &oIdqse, &oAngle);	
		}
	}
#if defined(_EST_ENCODER_)
#if !defined(_NON_ENCODER_Z_)
	else if (xeVar == MTR_VAR_AUTO_ENCODER_OFFSET)	// (9)
	{
//		qThetaOffsetEncoder = (short)(oAngle.qTheta - oObsEncoder.oAngle.qTheta + qThetaOffsetEncoder);
		EntryStateEncoderOffset();
	}
#endif
	else if (xeVar == MTR_VAR_MANU_ENCODER_OFFSET)	// (10)
	{
		qThetaOffsetEncoder = (short)xpqVar[0];
	}
#endif	
	else if (xeVar == MTR_VAR_INIT_POSIT)	// (11)
	{
//		EntryStateInitPosit();
	}
		
	CPU_EXIT_CRITICAL();
}

/****************************************************************************
Name:        DoMcSensingISR
Description: motor sensing function (=���� ���� �Լ�)
****************************************************************************/
long nSumUBRealTorq;
int nCntUBRealTorq;
TQ_IIR2 oBpfUBReal;
TQ15_IIR1 oApfUBReal;
TLpf1_Q15 oLpfUBReal;

long nSumTorqSect;
int nCntTorqSect;
S32 slTorqueLoadSum;
S32 slTorqueSlopeDelta;
S16 asnTorqueSlopeBuf[40];
S16 asnTorqueLoadBuf[30];
U08 ubTorqueSlopeIdx;
U08 ubTorqueLoadIdx;


long nSumAppSens;
int nCntAppSens;
int nTmrAppSens;
U16 aunAppSensBuf[4];
U16 unAppSensStat;

void DoMcSensingISR(void)
{
	short tqPosit;

	tqPosit = (oAngle.qTheta + Q_THETA_R(PI / 4.0)) & 0xC000;

	if (tqPosit - qRevolPosit)
	{
		nRevolCnt += ((short)(tqPosit - qRevolPosit) >> 14);
		qRevolPosit = tqPosit;
		tqPosit = 1;
	}
	else tqPosit = 0;

	if (wMcSensId & MTR_SENS_UNBALREAL)
	{
		nSumUBRealTorq += oIdqse.qQ;
		nCntUBRealTorq++;
		if ((tqPosit == 1) || (nCntUBRealTorq >= (int)(0.2 / TS_PWM)))
		{
			Q15 tqTorqAvg;
			Q15 tqTorqApf;
			tqTorqAvg = DIV_SD(nSumUBRealTorq, nCntUBRealTorq);
			nSumUBRealTorq = 0;
			nCntUBRealTorq = 0;

			DO_IIR2(oBpfUBReal, tqTorqAvg, tqTorqAvg);
			DO_IIR1(oApfUBReal, tqTorqAvg, tqTorqApf);
			tqTorqAvg = SqrtS32(MUL_SS(tqTorqAvg, tqTorqAvg) + MUL_SS(tqTorqApf, tqTorqApf));
			DO_LPF1(oLpfUBReal, tqTorqAvg, tqTorqAvg);

			if (tqTorqAvg < 0) snMtrSensUnbalReal = 0;
			else snMtrSensUnbalReal = tqTorqAvg;
			wMcSensEvent |= MTR_SENS_UNBALREAL;

			if (wMcSensId & MTR_SENS_UNBALAVG)			// Average unbalance sensor (=��� ��߶��� ����) 
			{
				if (oRampWrpm.qTar != oRampWrpm.qRef) 
				{
					nCntAppSens = (int)(-2 *4 * POLE_PAIR * RATIO_PULLEY);
					nSumAppSens = 0;
				}
				else if (++nCntAppSens > 0)
				{
					nSumAppSens += snMtrSensUnbalReal;
					if (nCntAppSens >= qAppSensMem)				// Unbalance average calculation time elapsed (=��߶��� ��� ��� �ð� ��� ?)
					{
						snMtrSensAppData = nSumAppSens / nCntAppSens;
						wMcSensEvent |= MTR_SENS_UNBALAVG;
						nCntAppSens = (int)(-2 *4 * POLE_PAIR * RATIO_PULLEY);
						nSumAppSens = 0;
					}
				}
			}
			else if (wMcSensId & MTR_SENS_WEIGHT)		// weight sensor (=���԰��� ����) 
			{
				if (++nCntAppSens >= (int)(4 * POLE_PAIR * RATIO_PULLEY))
				{
					nSumAppSens = dwPwmTime - (DWORD)nSumAppSens;
					snMtrSensAppData = (nSumAppSens << 5) / (abs(qWrpmActual) - abs(qAppSensMem));
					DevSetMotorDrive(MTR_MODE_CLOSEDLOOP_HALL, qWrpmActual, CONVERT_ACCEL_MAX);
					wMcSensEvent |= MTR_SENS_WEIGHT;
					nCntAppSens = 0;						
				}
			}
			else if (wMcSensId & MTR_SENS_FINDACCEL)		// Acceleration point detection (=���ӽ��� Ž��)  
			{
				nSumAppSens += snMtrSensUnbalReal;
				if (++nCntAppSens >= (int)(4 * POLE_PAIR * RATIO_PULLEY))
				{
					U16 tunRevolUnbal;
					tunRevolUnbal = DIV_SD(nSumAppSens, nCntAppSens);
					nCntAppSens = 0;
					nSumAppSens = 0;
					
					tunRevolUnbal = (tunRevolUnbal + aunAppSensBuf[0] * 7) >> 3;
					if (nTmrAppSens >= 0)
					{
						if (tunRevolUnbal >= aunAppSensBuf[3]) nTmrAppSens++;
						else 
						{
							if (nTmrAppSens >= +4) unAppSensStat |= 0x01;	// Highest point confirmation flag set (=�ְ��� Ȯ���÷��� ��)
							nTmrAppSens = -1;
						}
					}
					else
					{
						if (tunRevolUnbal <= aunAppSensBuf[3]) nTmrAppSens--;
						else 
						{
							if (nTmrAppSens <= -4) unAppSensStat |= 0x10;	// Lowest point confirmation flag set (=������ Ȯ���÷��� ��)
							nTmrAppSens = +1;
						}
					}
					aunAppSensBuf[3] = aunAppSensBuf[2];
					aunAppSensBuf[2] = aunAppSensBuf[1];
					aunAppSensBuf[1] = aunAppSensBuf[0];
					aunAppSensBuf[0] = tunRevolUnbal;
				}
				if ((unAppSensStat & 0x01)&&(nTmrAppSens <= -qAppSensMem))	// Decay time elapsed after detection of unbalanced peak (=��ٶ��� �ְ��� ���� �� ���� �ð� ��� ?)
				{
					snMtrSensAppData = TRUE;
					wMcSensEvent |= MTR_SENS_FINDACCEL;
					nTmrAppSens = 0;
					unAppSensStat = 0;	
					nCntAppSens = 0;
					nSumAppSens = 0;
				}						
			}
		}
	}
	if (wMcSensId & (MTR_SENS_TORQSLOPE | MTR_SENS_TORQLOAD))
	{
		nSumTorqSect += oIdqse.qQ;
		nCntTorqSect++;
		if (nCntTorqSect >= (int)(0.1 / TS_PWM))
		{
			nSumTorqSect = nSumTorqSect / nCntTorqSect;

			if (wMcSensId & MTR_SENS_TORQSLOPE)
			{
				unsigned int tubIndex;
				tubIndex = (((ubTorqueSlopeIdx + 20) >= 40) ? (ubTorqueSlopeIdx - 20) : (ubTorqueSlopeIdx + 20));

				slTorqueSlopeDelta = slTorqueSlopeDelta + nSumTorqSect 
								- asnTorqueSlopeBuf[tubIndex] * 2 + asnTorqueSlopeBuf[ubTorqueSlopeIdx];
				asnTorqueSlopeBuf[ubTorqueSlopeIdx] = nSumTorqSect;

				if (SIGN(oRampWrpm.qRef) == SIGN(slTorqueSlopeDelta))
				{
					snMtrSensTorqSlope = abs(slTorqueSlopeDelta) / 20;
				}
				else snMtrSensTorqSlope = 0;
				
				if (++ubTorqueSlopeIdx >= 40) ubTorqueSlopeIdx = 0;
				wMcSensEvent |= MTR_SENS_TORQSLOPE;			// Torque increment sensor data update flag set (=��ũ���� ���� ������ ���� �÷��� ��)
			}

			if (wMcSensId & MTR_SENS_TORQLOAD)
			{
				slTorqueLoadSum = slTorqueLoadSum - asnTorqueLoadBuf[ubTorqueLoadIdx] + nSumTorqSect;
				asnTorqueLoadBuf[ubTorqueLoadIdx] = nSumTorqSect;

				if (SIGN(oRampWrpm.qRef) == SIGN(slTorqueLoadSum))
				{
					snMtrSensTorqLoad = abs(slTorqueLoadSum) / 30;
				}
				else snMtrSensTorqLoad = 0;

				if (++ubTorqueLoadIdx >= 30) ubTorqueLoadIdx = 0;
				wMcSensEvent |= MTR_SENS_TORQLOAD;			// Torque load sensor data update flag set (=��ũ���� ���� ������ ���� �÷��� ��)
			}

			nSumTorqSect = 0;
			nCntTorqSect = 0;
		}
	}
}

/****************************************************************************
Name:		 DevSetMotorSensing
Description: motor sensing function (=���� ���� �Լ�)
****************************************************************************/
PUBLIC WORD DevSetMotorSensing(WORD xwSensId, Q15 xqParamSens)
{
	WORD twSensId;
	WORD twTransit;

	wSensUserId = xwSensId;
	if ((eMcCtrlMode == MTR_MODE_STOP_OPEN)||(eMcCtrlMode == MTR_MODE_STOP_BRAKE))
	{
		twSensId = 0;
	}
	else
	{
		twSensId = xwSensId & (MTR_SENS_UNBALREAL | MTR_SENS_TORQSLOPE | MTR_SENS_TORQLOAD);

		if (xwSensId & MTR_SENS_WEIGHT)				twSensId |= (MTR_SENS_UNBALREAL | MTR_SENS_WEIGHT);
		else if (xwSensId & MTR_SENS_UNBALAVG) 		twSensId |= (MTR_SENS_UNBALREAL | MTR_SENS_UNBALAVG);
		else if (xwSensId & MTR_SENS_FINDACCEL)		twSensId |= (MTR_SENS_UNBALREAL | MTR_SENS_FINDACCEL);
	}

	twTransit = twSensId ^ wMcSensId;

	if (twTransit & MTR_SENS_UNBALREAL) SetSensUnbalReal(twSensId & MTR_SENS_UNBALREAL);
	if (twTransit & MTR_SENS_TORQSLOPE) SetSensTorqueSlope(twSensId & MTR_SENS_TORQSLOPE);
	if (twTransit & MTR_SENS_TORQLOAD) SetSensTorqueLoad(twSensId & MTR_SENS_TORQLOAD);

	SetSensApplied(twSensId & (MTR_SENS_WEIGHT | MTR_SENS_UNBALAVG | MTR_SENS_FINDACCEL), xqParamSens);

	return wMcSensId;
}

/****************************************************************************
Name:		 DevGetMotorSensData
Description: Motor sensing data acquisition function (=���� ���� ������ ��� �Լ�)
****************************************************************************/
PUBLIC S16 DevGetMotorSensData(WORD xuwSensId)
{
	if (xuwSensId == MTR_SENS_UNBALREAL)
	{
		wMcSensEvent &= ~MTR_SENS_UNBALREAL;
		return snMtrSensUnbalReal;
	}
	else if (xuwSensId == MTR_SENS_TORQSLOPE)
	{
		wMcSensEvent &= ~MTR_SENS_TORQSLOPE;
		return snMtrSensTorqSlope;
	}
	else if (xuwSensId == MTR_SENS_TORQLOAD)
	{
		wMcSensEvent &= ~MTR_SENS_TORQLOAD;
		return snMtrSensTorqLoad;
	}
	else if (xuwSensId & (MTR_SENS_WEIGHT | MTR_SENS_UNBALAVG | MTR_SENS_FINDACCEL))
	{
		S16 tnSenAppData;
		tnSenAppData = snMtrSensAppData;
		if (wMcSensEvent & (MTR_SENS_WEIGHT | MTR_SENS_UNBALAVG | MTR_SENS_FINDACCEL))
		{
			DevSetMotorSensing(wSensUserId & ~(MTR_SENS_WEIGHT | MTR_SENS_UNBALAVG | MTR_SENS_FINDACCEL), 0);	
		}
		return tnSenAppData;
	}
	return 0;
}

/****************************************************************************
Name:		 SetSensUnbalReal
Description: Instantaneous unbalance sensing start function (=���� ��߶��� ���� ���� �Լ�)
****************************************************************************/
void SetSensUnbalReal(BOOLEAN xOnOff)
{
	wMcSensId &= ~MTR_SENS_UNBALREAL;
	if (xOnOff)
	{
		nSumUBRealTorq = 0;
		nCntUBRealTorq = 0;

		INIT_IIR2_BPF(oBpfUBReal, oIdqse.qQ, (1. * 2. * PI), (1.0 / 4 / POLE_PAIR / RATIO_PULLEY), 0.707);
		INIT_IIR1_ALLPASS(oApfUBReal, oIdqse.qQ, (1. * 2. * PI), (1.0 / 4 / POLE_PAIR / RATIO_PULLEY));
		INIT_LPF1(oLpfUBReal, 0, (0.25 * 2. * PI), (1.0 / 4 / POLE_PAIR / RATIO_PULLEY));
		
		wMcSensId |= MTR_SENS_UNBALREAL;
	}
	snMtrSensUnbalReal = 0;
	wMcSensEvent &= ~MTR_SENS_UNBALREAL;
}

/****************************************************************************
Name:		 SetSensTorqueSlope
Description: Torque incremental sensing start function (=��ũ ���� ���� ���� �Լ�)
****************************************************************************/
void SetSensTorqueSlope(BOOLEAN xOnOff)
{
	wMcSensId &= ~MTR_SENS_TORQSLOPE;
	if (xOnOff)
	{
		int tubCnt;
		
		ubTorqueSlopeIdx = 0;
		slTorqueSlopeDelta = 0;
		snMtrSensTorqSlope = 0;
		
		for (tubCnt = 0; tubCnt < 40; tubCnt++)
		{
			asnTorqueSlopeBuf[tubCnt] = oIdqse.qQ;
		}		

		if ((wMcSensId & (MTR_SENS_TORQSLOPE | MTR_SENS_TORQLOAD)) == 0)
		{
			nSumTorqSect = 0;
			nCntTorqSect = 0;
		}
		
		wMcSensId |= MTR_SENS_TORQSLOPE;
	}
	wMcSensEvent &= ~MTR_SENS_TORQSLOPE;
}

/****************************************************************************
Name:		 SetSensTorqueLoad
Description: Torque load sensing start function (=��ũ ���� ���� ���� �Լ�)
****************************************************************************/
void SetSensTorqueLoad(BOOLEAN xOnOff)
{
	wMcSensId &= ~MTR_SENS_TORQLOAD;
	if (xOnOff)
	{
		int tubCnt;
		
		slTorqueLoadSum = oIdqse.qQ * 30;
		ubTorqueLoadIdx = 0;
		snMtrSensTorqLoad = oIdqse.qQ;
		
		for (tubCnt = 0; tubCnt < 30; tubCnt++)
		{
			asnTorqueLoadBuf[tubCnt] = oIdqse.qQ;
		}		

		if ((wMcSensId & (MTR_SENS_TORQSLOPE | MTR_SENS_TORQLOAD)) == 0)
		{
			nSumTorqSect = 0;
			nCntTorqSect = 0;
		}		
		wMcSensId |= MTR_SENS_TORQLOAD;
	}
	wMcSensEvent &= ~MTR_SENS_TORQLOAD;
}

/****************************************************************************
Name:		 SetSensApplied
Description: Motor application sensing start function (=���� ���� ���� ���� �Լ�)
****************************************************************************/
void SetSensApplied(WORD xwSensId, Q15 xqParamSens)
{
	wMcSensId &= ~(MTR_SENS_WEIGHT | MTR_SENS_UNBALAVG | MTR_SENS_FINDACCEL);
	wMcSensEvent &= ~(MTR_SENS_WEIGHT | MTR_SENS_UNBALAVG | MTR_SENS_FINDACCEL);
	if (xwSensId)
	{
		nSumAppSens = 0;
		nCntAppSens = 0;
		nTmrAppSens = 0;
		unAppSensStat = 0;			

		switch (xwSensId)
		{
			case MTR_SENS_WEIGHT:										// Weight sensing holding torque (=���԰��� ���� ��ũ) 
				qAppSensMem = qWrpmActual;
				nSumAppSens = dwPwmTime;
				xqParamSens = abs(xqParamSens) * SIGN(oRampWrpm.qTar);					
				DevSetMotorDrive(MTR_MODE_OPENLOOP_CURRENT, 0, xqParamSens);
				break;
			case MTR_SENS_UNBALAVG:										// Average time = xqParamSens(0.1 seconds)  (=��� �ð� = xqParamSens(0.1��)) 
				qAppSensMem = abs((S32)xqParamSens * INVERT_RPM(oRampWrpm.qTar) * (int)(4 * POLE_PAIR * RATIO_PULLEY)) / 600; 
				break;	
			case MTR_SENS_FINDACCEL:									// Acceleration time after detecting the peak = xParamSens (0.1 seconds) (=�ְ��� ������ ���ӽð� = xParamSens(0.1��)) 
				qAppSensMem = -1 * (abs((S32)xqParamSens * INVERT_RPM(oRampWrpm.qTar)) / 600);			
				aunAppSensBuf[3] = snMtrSensUnbalReal;
				aunAppSensBuf[2] = snMtrSensUnbalReal;
				aunAppSensBuf[1] = snMtrSensUnbalReal;
				aunAppSensBuf[0] = snMtrSensUnbalReal;
				break;
			default:
				break;
		}
		
		wMcSensId |= xwSensId;
	}
}



Q15 qVdcBus_get (void)
{
	return qVdcBus;
}
Q15 qWrpmActual_get (void)
{
	return qWrpmActual;
}

Q15 qThetaVsLead_get (void)
{
	return qThetaVsLead;
}
Q15 qVsRefCC_get (void)
{
	return qVsRefCC;
}
Q15 qIs_get (void)
{
	return qIs;
}		
Q15 qPowerIn_get (void)
{
	return qPowerIn;
}
DWORD dwPwmTime_get (void)
{
	return dwPwmTime;
}
short nRevolCnt_get (void)
{
	return nRevolCnt;
}
WORD wMcSensId_get (void)
{
	return wMcSensId;
}
WORD wMcSensEvent_get (void)
{
	return wMcSensEvent;
}
S16 snMtrSensUnbalReal_get (void)
{
	return snMtrSensUnbalReal;
}
S16 snMtrSensTorqSlope_get (void)
{
	return snMtrSensTorqSlope;
}
S16 snMtrSensTorqLoad_get (void)
{
	return snMtrSensTorqLoad;
}
S16 snMtrSensAppData_get (void)
{
	return snMtrSensAppData;
}

void oObsHall_get(TObserver *oObsHall_cpy)
{
   memmove((TObserver*)oObsHall_cpy,(TObserver *)&oObsHall,sizeof(TObserver));
}
 
 void oAngle_get(TAngle *oAngle_cpy)
{
   memmove((TAngle*)oAngle_cpy,(TAngle *)&oAngle,sizeof(TAngle));
}
 void oAngle_set(TAngle *oAngle_cpy)
{
   memmove((TAngle*)&oAngle,(TAngle *)oAngle_cpy,sizeof(TAngle));
}
 
 void oIdqse_get(TAxisDQ *oIdqse_cpy)
{
   memmove((TAxisDQ*)oIdqse_cpy,(TAxisDQ *)&oIdqse,sizeof(TAxisDQ));
}

 void oRampWrpm_get(TRampRef *oRampWrpm_cpy)
{
   memmove((TRampRef*)oRampWrpm_cpy,(TRampRef *)&oRampWrpm,sizeof(TRampRef));
}

 void eMcCtrlMode_get(EMcCtrlMode *eMcCtrlMode_cpy)
{
   memmove((EMcCtrlMode*)eMcCtrlMode_cpy,(EMcCtrlMode *)&eMcCtrlMode,sizeof(EMcCtrlMode));
}
 void eMcFailEvent_get(EMcFailEvent *eMcFailEvent_cpy)
{
   memmove((EMcFailEvent*)eMcFailEvent_cpy,(EMcFailEvent *)&eMcFailEvent,sizeof(EMcFailEvent));
}
void motor_parameter_set(void)
{
  motor_parameter[PULSATOR_PARAMETER_INDEX].T_ALIGN_id =T_ALIGN;
  motor_parameter[PULSATOR_PARAMETER_INDEX].T_MAX_STARTUP_id=T_MAX_STARTUP;
  motor_parameter[PULSATOR_PARAMETER_INDEX].I_ALIGN_id =I_ALIGN;
  motor_parameter[PULSATOR_PARAMETER_INDEX].I_STARTUP_INIT_id=I_STARTUP_INIT;
  motor_parameter[PULSATOR_PARAMETER_INDEX].I_STARTUP_MAX_id=I_STARTUP_MAX;
  motor_parameter[PULSATOR_PARAMETER_INDEX].JM_SC_id = JM_SC;
  motor_parameter[PULSATOR_PARAMETER_INDEX].KP_SC_ROLL_id = KP_SC_ROLL_PULSATOR;
  motor_parameter[PULSATOR_PARAMETER_INDEX].KI_SC_ROLL_id = KI_SC_ROLL_PULSATOR;
  motor_parameter[PULSATOR_PARAMETER_INDEX].KA_SC_ROLL_id = KA_SC_ROLL_PULSATOR;
  motor_parameter[PULSATOR_PARAMETER_INDEX].KP_SC_id = KP_SC_PULSATOR;
  motor_parameter[PULSATOR_PARAMETER_INDEX].KI_SC_id = KI_SC_PULSATOR;
  motor_parameter[PULSATOR_PARAMETER_INDEX].KA_SC_id = KA_SC_PULSATOR;

  motor_parameter[BASKET_PARAMETER_INDEX].T_ALIGN_id =T_ALIGN_BASKET;
  motor_parameter[BASKET_PARAMETER_INDEX].T_MAX_STARTUP_id=T_MAX_STARTUP_BASKET;
  motor_parameter[BASKET_PARAMETER_INDEX].I_ALIGN_id =I_ALIGN_BASKET;
  motor_parameter[BASKET_PARAMETER_INDEX].I_STARTUP_INIT_id=I_STARTUP_INIT_BASKET;
  motor_parameter[BASKET_PARAMETER_INDEX].I_STARTUP_MAX_id=I_STARTUP_MAX_BASKET;
  motor_parameter[BASKET_PARAMETER_INDEX].JM_SC_id = JM_SC_BASKET;
  motor_parameter[BASKET_PARAMETER_INDEX].KP_SC_ROLL_id = KP_SC_ROLL_BASKET;
  motor_parameter[BASKET_PARAMETER_INDEX].KI_SC_ROLL_id = KI_SC_ROLL_BASKET;
  motor_parameter[BASKET_PARAMETER_INDEX].KA_SC_ROLL_id = KA_SC_ROLL_BASKET;
  motor_parameter[BASKET_PARAMETER_INDEX].KP_SC_id = KP_SC_BASKET;
  motor_parameter[BASKET_PARAMETER_INDEX].KI_SC_id = KI_SC_BASKET;
  motor_parameter[BASKET_PARAMETER_INDEX].KA_SC_id = KA_SC_BASKET;
}
void parameter_index_set(parameter_en_type_def parameter_index)
{
  parameter_index_id=parameter_index;
}
/***************************************************************************************************
 *	[End Of Source File]
 **************************************************************************************************/