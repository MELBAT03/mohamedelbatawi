/**===========================================================================
 * @file		McUtil.h
==============================================================================*/

#ifndef	_MC_UTIL_H_
#define	_MC_UTIL_H_

/***************************************************************************************************
 *	[Include File]
 **************************************************************************************************/
#include "McTarget.h"


/***************************************************************************************************
 *	[Define]
 **************************************************************************************************/

//=====[ Math constatns (=���� ���) ]=================================================
#define SQRT3					1.732050808
#define SQRT2					1.414213562
#define PI						3.141592654

//=====[ Built-in function (=��Ʈ�� �Լ�) ]=================================================
#define Q15_FRAC(xFloat)	(((xFloat) < 0.0) ? (Q15)(32768 * (xFloat) - 0.5) : (Q15)(32768 * (xFloat) + 0.5))

#define MUL_SS(qa, qb)		(long)((long)(qa) * (qb))					// signed long MUL_SS(const signed int p0, const signed int p1);
#define MUL_US(qa, qb)		(long)((long)((unsigned long)(qa)) * (qb))					// signed long MUL_US(const unsigned int p0, const signed int p1);
#define MUL_UU(qa, qb)		(unsigned long)((unsigned long)(qa) * (unsigned long)(qb))		// unsigned long MUL_UU(const unsigned int p0, const unsigned int p1);

#if defined(_CPU_UPD70F3454_IG3_)
#define MUL_LL(la, lb, scale)	(long)(__mul32(((la) << (16 - scale)), ((lb) << 16)))
#else
#define MUL_LL(la, lb, scale)	(long)(((long long)(la) * (lb)) >> scale)		// signed long MUL_LL(const signed long p0, const signed long p1, usigned char scale);
#endif

#define DIV_SD(qda, qb)		(int)((long)(qda) / (qb))						// int DIV_SD(const long num, const int den);
#define DIV_UD(qda, qb)		(unsigned int)((unsigned long)(qda) / (qb))		// unsigned int DIV_UD(const unsigned long num, const unsigned int den);

#define LIMIT(x,s,l)		(((x)>(l))?(l):((x)<(s))?((s)):(x))
#define SIGN(x)				(((x)>=0)? 1 :-1)


// PORT Level Noise Reduction Read (=������ ���� �б�) ---------------------------------------------
#define PORT_DEBOUNCE_READER(xPortScan, xResult, xFilterCnt)	{ BYTE tCnt; \
								xResult = (xPortScan); tCnt = (xFilterCnt); \
								while(xFilterCnt) { WORD tReader = (xPortScan); \
									if (xResult == tReader) { if (--tCnt == 0) break; } \
									else {xResult = tReader; tCnt = (xFilterCnt); } }}


#define MOTOR_SPD2RPM(xRpm)			(int)((xRpm) * QMAX_OMEGA_RPM_DRUM/ 0x8000L) 		/* xRpm*10/182 */

//=====[ Control constant (=���� ���) ]=================================================
#ifndef	FREQ_CLK
#define FREQ_CLK				80000000.0
#endif

#ifndef	FREQ_PWM
#define FREQ_PWM	        	16000.0
#endif

#ifndef	T_AD_CONV
#define T_AD_CONV				0.000002
#endif

#ifndef	T_IPM_DEAD
#define T_IPM_DEAD				0.0000015
#endif

#ifndef	T_DEAD_COMPEN
#define T_DEAD_COMPEN			(1.0 * T_IPM_DEAD)
#endif

#ifndef	T_AD_NOISE
#define T_AD_NOISE				0.000002
#endif

#define TS_PWM					(1.0 / FREQ_PWM)

#define TS_CC					TS_PWM

#ifndef	TS_SC
#define TS_SC					TS_CC
#endif

#ifndef	AD_BIT_NO
#define AD_BIT_NO				12
#endif

#ifndef	RATIO_PULLEY
#define RATIO_PULLEY			1.0
#endif


#define TCLK_PWM_PERIOD			(int)(FREQ_CLK / FREQ_PWM)
#define TCLK_SAMP_IREAD			(int)((T_AD_NOISE + T_IPM_DEAD + T_AD_CONV) * FREQ_CLK)	//(int)((0.0000015 + T_IPM_DEAD + T_AD_CONV) * FREQ_CLK)
#define TCLK_AD_CONV			(int)(T_AD_CONV * FREQ_CLK)
#define TCLK_IPM_DEAD			(int)(T_IPM_DEAD * FREQ_CLK)

#if defined(_DEAD_TIME_COMPEN_T_)
#define TCLK_DEAD_COMPEN		(int)(T_DEAD_COMPEN * FREQ_CLK)
#else
#define TCLK_DEAD_COMPEN		(int)(0)
#endif

// controller scale constant (=����� ������ ���) ---------------------------------
#ifndef	SCALE_PIL1_KPIT
#define SCALE_PIL1_KPIT			15
#endif

#ifndef	SCALE_PIA1_KP
#define SCALE_PIA1_KP			8
#endif

#ifndef	SCALE_PIA1_KICT
#define SCALE_PIA1_KICT			15
#endif

#ifndef	SCALE_SLOPE_INTEG
#define SCALE_SLOPE_INTEG		13
#endif

#ifndef	SCALE_POWER
#define SCALE_POWER				12
#endif

// Q15 Format maximum value (=Q15 Format �ִ밪) --------------------------------------
#define ADMAX_V_DC				(((R_V_DC_HIGH + R_V_DC_LOW) * V_ADC_REF / R_V_DC_LOW)*(1L <<AD_BIT_NO)/((1L <<AD_BIT_NO) -1.))
#define ADMAX_I_IPM				((V_ADC_REF / (R_SHUNT_IPM * GAIN_OPAMP_IPM))*(1L <<AD_BIT_NO)/((1L <<AD_BIT_NO) -1.))
//#define ADMAX_V_DC				((R_V_DC_HIGH + R_V_DC_LOW) * V_ADC_REF / R_V_DC_LOW)
//#define ADMAX_I_IPM				(V_ADC_REF / (R_SHUNT_IPM * GAIN_OPAMP_IPM))

#ifndef QMAX_VOLT
#define QMAX_VOLT				(480.0)
#endif
#ifndef QMAX_CURRENT
#define QMAX_CURRENT			(40.0)
#endif

#define QMAX_TORQUE				(3.0 / 2.0 * QMAX_CURRENT * RATIO_PULLEY * POLE_PAIR * KE)	// Drum torque maximum value (=�巳 ���ũ �ִ밪) 
#define QMAX_POWER				(3.0 / 2.0 * QMAX_VOLT * QMAX_CURRENT * (1L <<SCALE_POWER) / (1L <<15))

#define QMAX_THETA_R			(PI)										// electrical angle maximum (=���Ⱒ �ִ밪)
#define QMAX_THETA_RM			(PI / POLE_PAIR)							// Motor angle maximum value (=���Ͱ� �ִ밪) 
#define QMAX_THETA_RD			(PI / POLE_PAIR / RATIO_PULLEY)				// Drum angle maximum (=�巳�� �ִ밪) 

#ifndef	QMAX_OMEGA_RPM_DRUM
#define QMAX_OMEGA_RPM_DRUM		(1800.0)									// Maximum drum revolutions per minute (=�巳 �д� ȸ���� �ִ밪) 
#endif
#define QMAX_OMEGA_RPM			(QMAX_OMEGA_RPM_DRUM * RATIO_PULLEY)		// Maximum value of motor revolutions per minute (=���� �д� ȸ���� �ִ밪) 
#define QMAX_OMEGA_RPS			(QMAX_OMEGA_RPM / 60.0)						// Maximum value of motor revolutions per second (=���� �ʴ� ȸ���� �ִ밪) 
#define QMAX_OMEGA_RM			(2.0 * PI * QMAX_OMEGA_RPS)					// Motor angular velocity maximum value (=���� ���ӵ� �ִ밪) 
#define QMAX_OMEGA_R			(POLE_PAIR * QMAX_OMEGA_RM)					// Electrical angular velocity maximum value (=���� ���ӵ� �ִ밪) 
#define QMAX_OMEGA_RD			(2.0 * PI * QMAX_OMEGA_RPM_DRUM / 60.0)		// Drum angular velocity maximum value (=�巳 ���ӵ� �ִ밪) 
#define QMAX_FREQUENCY			(POLE_PAIR * QMAX_OMEGA_RPS)				// electrical frequency maximum value ���� ���ļ� �ִ밪 

#define QMAX_SLOPE_RPM_DRUM		(QMAX_OMEGA_RPM_DRUM / (TS_SC * (1L <<SCALE_SLOPE_INTEG)))
#define QMAX_SLOPE_RPM			(QMAX_OMEGA_RPM / (TS_SC * (1L <<SCALE_SLOPE_INTEG)))
#define QMAX_SLOPE_CRRENT		(QMAX_CURRENT / (TS_CC * (1L <<SCALE_SLOPE_INTEG)))

#ifndef	QMAX_FULX
#define QMAX_FULX				(4.0 * QMAX_VOLT / QMAX_OMEGA_R)			// Flux maximum value 
#endif

// Q15 Format conversion function (=��ȯ �Լ�) --------------------------------------
#define Q_VOLT(xVolt)			Q15_FRAC((xVolt) / QMAX_VOLT)
#define Q_CURRENT(xCurr)		Q15_FRAC((xCurr) / QMAX_CURRENT) 
#define Q_TORQUE(xTor)			Q15_FRAC((xTor) / QMAX_TORQUE)
#define Q_POWER(xPwr)			Q15_FRAC((xPwr) / QMAX_POWER)

#define Q_THETA_R(xTheta)		Q15_FRAC((xTheta) / QMAX_THETA_R)
#define Q_OMEGA_RPM_DRUM(xRpm)	Q15_FRAC((xRpm) / QMAX_OMEGA_RPM_DRUM)
#define Q_OMEGA_RPM(xRpm)		Q15_FRAC((xRpm) / QMAX_OMEGA_RPM)
#define Q_OMEGA_RPS(xRps)		Q15_FRAC((xRps) / QMAX_OMEGA_RPS)
#define Q_OMEGA_R(xWr)			Q15_FRAC((xWr) / QMAX_OMEGA_R)
#define Q_FREQUENCY(xFreq)		Q15_FRAC((xFreq) / QMAX_FREQUENCY)

#define Q_SLOPE(xSlope, Ts, QMax)	Q15_FRAC((xSlope) * (Ts) * (1L <<SCALE_SLOPE_INTEG) / (QMax))

#define Q_SLOPE_RPM_DRUM(xRpmPerS)	Q_SLOPE(xRpmPerS, TS_SC, QMAX_OMEGA_RPM_DRUM)
#define Q_SLOPE_RPM(xRpmPerS)		Q_SLOPE(xRpmPerS, TS_SC, QMAX_OMEGA_RPM)
#define Q_SLOPE_CURRENT(xAPerS)		Q_SLOPE(xAPerS, TS_CC, QMAX_CURRENT)


// Motor parameter constants (=���� �Ķ���� ���) -----------------------------------------

#ifndef SCALE_VEQ
#define SCALE_VEQ				10
#endif

#define EQ_VEQ_RS				(Q15)(RS * (1L <<SCALE_VEQ) * QMAX_CURRENT / QMAX_VOLT + 0.5)
#define EQ_VEQ_LD				(Q15)(LD * (1L <<SCALE_VEQ) * QMAX_CURRENT * QMAX_OMEGA_R / QMAX_VOLT + 0.5)
#define EQ_VEQ_LQ				(Q15)(LQ * (1L <<SCALE_VEQ) * QMAX_CURRENT * QMAX_OMEGA_R / QMAX_VOLT + 0.5)
#define EQ_VEQ_KE				(Q15)(KE * (1L <<SCALE_VEQ) * QMAX_OMEGA_R / QMAX_VOLT + 0.5)

#ifndef	SCALE_INV_KE
#define SCALE_INV_KE			13
#endif

#define EQ_INV_KE				(Q15)(1.0 / KE * (1L <<SCALE_INV_KE) * QMAX_VOLT / QMAX_OMEGA_R + 0.5)

// _MOTOR_PARAM_ -----------------------------------
#if defined(_MOTOR_PARAM_)
GLOBAL Q15 Q_VEQ_RS;
GLOBAL Q15 Q_VEQ_LD;
GLOBAL Q15 Q_VEQ_LQ;
GLOBAL Q15 Q_VEQ_KE;
GLOBAL Q15 Q_INV_KE;
#define UPDATE_GAIN_PARAM()		do {	\
	Q_VEQ_RS = EQ_VEQ_RS;	Q_VEQ_LD = EQ_VEQ_LD;	Q_VEQ_LQ = EQ_VEQ_LQ;	Q_VEQ_KE = EQ_VEQ_KE;	Q_INV_KE = EQ_INV_KE; \
} while (0)

#else
#define Q_VEQ_RS				EQ_VEQ_RS
#define Q_VEQ_LD				EQ_VEQ_LD
#define Q_VEQ_LQ				EQ_VEQ_LQ
#define Q_VEQ_KE				EQ_VEQ_KE
#define Q_INV_KE				EQ_INV_KE
#endif


// Velocity Integral Theta Transformation Constant (=�ӵ� ���� Theta ��ȯ ���) -----------------------------------
#define SCALE_OMEGAINTEG		15
#define Q_OMEGAINTEG_TS			(Q15)(TS_CC * (1L <<SCALE_OMEGAINTEG) * QMAX_OMEGA_R / QMAX_THETA_R + 0.5)

#ifndef T_PWMCMP				// PWM output delay compensation time (=PWM ������� ���� �ð�) 
#define T_PWMCMP				(0.0 * TS_CC)
#endif
#define Q_OMEGAINTEG_T_PWMCOMP	(Q15)(T_PWMCMP * (1L <<SCALE_OMEGAINTEG) * QMAX_OMEGA_R / QMAX_THETA_R + 0.5)

#ifndef T_OBSCMP				// Observer flux angle estimation delay compensation time (=������ �ڼӰ� ���� ���� ���� �ð�) 
#define T_OBSCMP				(0.0 * TS_CC)
#endif
#define Q_OMEGAINTEG_T_OBSCOMP	(Q15)(T_OBSCMP * (1L <<SCALE_OMEGAINTEG) * QMAX_OMEGA_R / QMAX_THETA_R + 0.5)



//======[ ADC Read constants setting ]======================================================
#define OFFSET_DELAY			4
#define Q_OFFSET_MAX			Q15_FRAC(0.05)

#ifndef	AD15_OFFSET
#define AD15_OFFSET				(0x01 << 14)
#endif

#if defined(_OPAMP_INVERTING_)
#define AD_NEGATIVE(qAd)		+(qAd)
#define AD_POSITIVE(qAd)		-(qAd)
#else
#define AD_NEGATIVE(qAd)		-(qAd)
#define AD_POSITIVE(qAd)		+(qAd)
#endif


// ADC Offset FLAG -----------------------------------------
#define ADC_OFFSET_FLAG_DO			0x0100
#define ADC_OFFSET_FLAG_END			0x0200
#define ADC_OFFSET_NOT				0x0000
#define ADC_OFFSET_OK				0x0001
#define ADC_OFFSET_NG				0x0002


/***************************************************************************************************
 *	[Enumeration & TypeDeclarations]
 **************************************************************************************************/
// coordinate axis structure (=��ǥ �� ����ü) ------------------------------------------
typedef struct 
{
	Q15 qA;
	Q15 qB;
	Q15 qC;
} TAxisABC;

typedef struct 
{
	Q15 qD;
	Q15 qQ;
} TAxisDQ;


// Angle structure -------------------------------------
typedef struct 
{
	Q15 qTheta;
	Q15 qSin;
	Q15 qCos;
} TAngle;

// Ramp structure ---------------------------------------
typedef struct 
{
	Q15 qTar;
	Q15 qRef;
	Q15 qSlope;
	Q30 qdInteg;
} TRampRef;

// First-order low-pass filter (=1�� ���� ��� ����) -----------------------------------
typedef struct 
{
	Q15 qX1;
	Q15 qKx;
	Q30 qdInteg;
} TLpf1_Q15;

// PWM Duty Ton structure -----------------------------------
typedef struct 
{
	int nMajorMax;		// Major : Full Period or On Sequence Ton Duty
	int nMajorMed;
	int nMajorMin;
#if	defined(_ONE_SHUNT_)
	int nRearMax;		// Rear : Off Sequence Ton Duty
	int nRearMed;
	int nRearMin;
#endif
} TPwmDuty;

// _FLT_TRAPEZOIDAL_
#define SET_LPF_TRAP_GAIN(lpf,Wc,Ts) {	lpf.qKx = Q15_FRAC((Ts) * (Wc) / (2 + (Ts) * (Wc))); }
#define INIT_LPF_TRAP(lpf,xVal,Wc,Ts) {	SET_LPF_TRAP_GAIN(lpf,Wc,Ts); \
										lpf.qX1 = (xVal); lpf.qdInteg = ((Q30)(xVal) <<15);}

#define DO_LPF_TRAP(lpf, x, y) 	do { 	lpf.qdInteg += MUL_SS(lpf.qKx, lpf.qX1 + (x) - (lpf.qdInteg >>14));\
										lpf.qX1 = (x); (y) = lpf.qdInteg >> 15;} while(0)
// _FLT_FORWARD_
#define SET_LPF_FWD_GAIN(lpf,Wc,Ts) {	lpf.qKx = Q15_FRAC((Ts) * (Wc)); }
#define INIT_LPF_FWD(lpf,xVal,Wc,Ts) {	SET_LPF_FWD_GAIN(lpf,Wc,Ts); \
										lpf.qX1 = (xVal); lpf.qdInteg = ((Q30)(xVal) <<15);}

#define DO_LPF_FWD(lpf, x, y) 	do {	lpf.qdInteg += MUL_SS(lpf.qKx, (lpf.qX1) - (lpf.qdInteg >>15));\
										lpf.qX1 = (x); (y) = lpf.qdInteg >> 15;} while(0)
// _FLT_BACKWARD_
#define SET_LPF_BWD_GAIN(lpf,Wc,Ts) {	lpf.qKx = Q15_FRAC((Ts) * (Wc) / (1 + (Ts) * (Wc))); }
#define INIT_LPF_BWD(lpf,xVal,Wc,Ts) { 	SET_LPF_BWD_GAIN(lpf,Wc,Ts); \
										lpf.qX1 = (xVal); lpf.qdInteg = ((Q30)(xVal) <<15);}

#define DO_LPF_BWD(lpf, x, y) 	do {	lpf.qdInteg += MUL_SS(lpf.qKx, (x) - (lpf.qdInteg >>15));\
										lpf.qX1 = (x); (y) = lpf.qdInteg >> 15;} while(0)

#if defined(_FLT_TRAPEZOIDAL_)
#define SET_LPF1_GAIN(lpf,Wc,Ts)		SET_LPF_TRAP_GAIN(lpf,Wc,Ts)
#define INIT_LPF1(lpf,xVal,Wc,Ts)		INIT_LPF_TRAP(lpf,xVal,Wc,Ts)
#define DO_LPF1(lpf, x, y)				DO_LPF_TRAP(lpf, x, y)

#elif defined(_FLT_FORWARD_)
#define SET_LPF1_GAIN(lpf,Wc,Ts)		SET_LPF_FWD_GAIN(lpf,Wc,Ts)
#define INIT_LPF1(lpf,xVal,Wc,Ts)		INIT_LPF_FWD(lpf,xVal,Wc,Ts)
#define DO_LPF1(lpf, x, y)				DO_LPF_FWD(lpf, x, y)

#else	// #elif defined(_FLT_BACKWARD_)
#define SET_LPF1_GAIN(lpf,Wc,Ts)		SET_LPF_BWD_GAIN(lpf,Wc,Ts)
#define INIT_LPF1(lpf,xVal,Wc,Ts)		INIT_LPF_BWD(lpf,xVal,Wc,Ts)
#define DO_LPF1(lpf, x, y)				DO_LPF_BWD(lpf, x, y)
#endif

// First-order high-pass filter (=1�� ���� ��� ����) -----------------------------------
typedef struct 
{
	Q15 qX1;
	Q15 qKx;
	Q30 qdInteg;
} THpf1_Q15;

#define SET_HPF1_GAIN(hpf,Wc,Ts) {		hpf.qKx = Q15_FRAC(2 / (2 + (Ts) * (Wc))); }
#define INIT_HPF1(hpf,xVal,Wc,Ts) {		SET_HPF1_GAIN(hpf,Wc,Ts); \
										hpf.qX1 = (xVal); hpf.qdInteg = 0;}

#define DO_HPF1(hpf, x, y) {			hpf.qdInteg += MUL_SS(hpf.qKx, (x) - hpf.qX1 - (hpf.qdInteg >>15)); \
										hpf.qX1 = (x);	(y) = hpf.qdInteg >> 15;}


/* // 2nd Butter worth filter -----------------------------------
typedef struct 
{
	Q15 qX1;
	Q15 qX2;
	Q15 qY1;
	Q15 qY2;
	Q15 qKb0;
	Q15 qKb1;
	Q15 qKb2;
	Q15 qKma1;
	Q15 qKa2;
	Q30 qdInteg;
} TButter2_Q15;

#define SET_BUTTER2_GAIN(flt, b0, b1, b2, a1, a2)	{	\
							flt.qKb0 = Q15_FRAC(b0);	flt.qKb1 = Q15_FRAC(b1);	flt.qKb2 = Q15_FRAC(b2);	\
							flt.qKma1 = Q15_FRAC(a1 +1.0);	flt.qKa2 = Q15_FRAC(a2);	}

#define DO_BUTTER2(flt, x, y)	{ flt.qdInteg = flt.qdInteg \
										+ MUL_SS((x), flt.qKb0) + MUL_SS(flt.qX1, flt.qKb1) + MUL_SS(flt.qX2, flt.qKb2) \
										- MUL_SS(flt.qY2, flt.qKa2) - MUL_SS(flt.qY1, flt.qKma1); \
										flt.qX2 = flt.qX1; flt.qX1 = (x); flt.qY2 = flt.qY1; \
										flt.qY1 = flt.qdInteg >> 15; (y) = flt.qY1; }

// 2nd IIR filter -----------------------------------
typedef struct 
{
	Q15 qZ1;
	Q15 qZ2;
	Q15 qKb0;
	Q15 qKb1;
	Q15 qKb2;
	Q15 qKma1;
	Q15 qKa2;
	Q30 qdInteg;
} TIIR2_Q15;

#define SET_IIR2_GAIN(flt, b0, b1, b2, a1, a2)	{	\
							flt.qKb0 = Q15_FRAC(b0 *0.5);	flt.qKb1 = Q15_FRAC(b1 *0.5);	flt.qKb2 = Q15_FRAC(b2 *0.5);	\
							flt.qKma1 = Q15_FRAC((a1 +1.0) *0.5);	flt.qKa2 = Q15_FRAC(a2 *0.5);	}

#define DO_IIR2(flt, x, y)	{ flt.qdInteg = flt.qdInteg \
								+ ((x) <<14) - MUL_SS(flt.qZ1, flt.qKma1) - MUL_SS(flt.qZ2, flt.qKa2); \
								(y) = (MUL_SS((flt.qdInteg  >>14), flt.qKb0) + MUL_SS(flt.qZ1, flt.qKb1) + MUL_SS(flt.qZ1, flt.qKb1)) >>14; \
								flt.qZ2 = flt.qZ1;	flt.qZ1 = (flt.qdInteg  >>14); } */


// 1st IIR filter -----------------------------------
typedef struct 
{
	long mR1;
	Q15 qKb0;
	Q15 qKb1;
	Q15 qKa1;
} TQ15_IIR1;

#define SET_IIR1_LPF_GAIN(flt,Wc,Ts) {	\
								flt.qKb0 = Q15_FRAC((Ts) * (Wc) / (2. + (Ts) * (Wc))); flt.qKb1 = flt.qKb0; \
								flt.qKa1 = Q15_FRAC((2. - (Ts) * (Wc)) / (2. + (Ts) * (Wc)));	}

#define SET_IIR1_HPF_GAIN(flt,Wc,Ts) {	\
								flt.qKb0 = Q15_FRAC(2. / (2. + (Ts) * (Wc))); flt.qKb1 = -flt.qKb0; \
								flt.qKa1 = Q15_FRAC((2. - (Ts) * (Wc)) / (2. + (Ts) * (Wc)));	}

#define SET_IIR1_ALLPASS_GAIN(flt,Wc,Ts) {	\
								flt.qKb0 = Q15_FRAC((2. - (Ts) * (Wc)) / (2. + (Ts) * (Wc))); \
								flt.qKb1 = Q15_FRAC((-2. - (Ts) * (Wc)) / (2. + (Ts) * (Wc))); \
								flt.qKa1 = flt.qKb0;	}

#define INIT_IIR1_LPF(flt,xVal,Wc,Ts) {	SET_IIR1_LPF_GAIN(flt,Wc,Ts); \
								flt.mR1 = MUL_SS((xVal), (flt.qKb1 + flt.qKa1));	}

#define INIT_IIR1_HPF(flt,xVal,Wc,Ts) {	SET_IIR1_HPF_GAIN(flt,Wc,Ts) \
								flt.mR1 = MUL_SS((xVal), flt.qKb1);	}

#define INIT_IIR1_ALLPASS(flt,xVal,Wc,Ts) {	SET_IIR1_ALLPASS_GAIN(flt,Wc,Ts) \
								flt.mR1 = MUL_SS((xVal), (flt.qKb1 - flt.qKa1));	}

#define DO_IIR1(flt, x, y) { 	long mY = (flt.mR1 + MUL_SS(flt.qKb0, (x)));  \
								flt.mR1 = (MUL_SS(flt.qKb1, (x)) + MUL_LL(mY, flt.qKa1, 15));	\
								(y) = (Q15)(mY >> 15);	}

// 2nd IIR filter -----------------------------------
#ifndef SCALE_IIR2
#define SCALE_IIR2				14
#endif
#define QC_IIR2					(2.0 / (1L << (16 - SCALE_IIR2)))

typedef struct 
{
	long mR1;
	long mR2;
	Q15 qKb0;
	Q15 qKb1;
	Q15 qKb2;
	Q15 qKa1;
	Q15 qKa2;
} TQ_IIR2;

#define SET_IIR2_GAIN(flt, b0, b1, b2, a1, a2)	{	\
							flt.qKb0 = Q15_FRAC(QC_IIR2* b0);	flt.qKb1 = Q15_FRAC(QC_IIR2* b1);	flt.qKb2 = Q15_FRAC(QC_IIR2* b2);	\
							flt.qKa1 = Q15_FRAC(QC_IIR2* a1);	flt.qKa2 = Q15_FRAC(QC_IIR2* a2);	}

#define SET_IIR2_LPF_GAIN(flt,Wc,Ts,Z) {	\
							flt.qKb0 = Q15_FRAC(QC_IIR2* ((Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKb1 = Q15_FRAC(QC_IIR2* (2.*(Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKb2 = Q15_FRAC(QC_IIR2* ((Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKa1 = Q15_FRAC(QC_IIR2* (8.-2.*(Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKa2 = Q15_FRAC(QC_IIR2* (4.*(Ts)*(Z)*(Wc)-4.-(Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); }

#define SET_IIR2_HPF_GAIN(flt,Wc,Ts,Z) {	\
							flt.qKb0 = Q15_FRAC(QC_IIR2* (4.) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKb1 = Q15_FRAC(QC_IIR2* (-8.) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKb2 = Q15_FRAC(QC_IIR2* (4.) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKa1 = Q15_FRAC(QC_IIR2* (8.-2.*(Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKa2 = Q15_FRAC(QC_IIR2* (4.*(Ts)*(Z)*(Wc)-4.-(Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); }

#define SET_IIR2_ALLPASS_GAIN(flt,Wc,Ts,Z) {	\
							flt.qKb0 = Q15_FRAC(QC_IIR2* (4.-4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKb1 = Q15_FRAC(QC_IIR2* (2.*(Ts)*(Ts)*(Wc)*(Wc)-8.) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKb2 = Q15_FRAC(QC_IIR2* (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKa1 = Q15_FRAC(QC_IIR2* (8.-2.*(Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKa2 = Q15_FRAC(QC_IIR2* (4.*(Ts)*(Z)*(Wc)-4.-(Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); }

#define SET_IIR2_BPF_GAIN(flt,Wc,Ts,Z) {	\
							flt.qKb0 = Q15_FRAC(QC_IIR2* (4.*(Ts)*(Z)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKb1 = Q15_FRAC(QC_IIR2* (0.) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKb2 = Q15_FRAC(QC_IIR2* (-4.*(Ts)*(Z)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKa1 = Q15_FRAC(QC_IIR2* (8.-2.*(Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKa2 = Q15_FRAC(QC_IIR2* (4.*(Ts)*(Z)*(Wc)-4.-(Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); }

#define SET_IIR2_NOTCH_GAIN(flt,Wc,Ts,Z) {	\
							flt.qKb0 = Q15_FRAC(QC_IIR2* (4.+(Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKb1 = Q15_FRAC(QC_IIR2* (2.*(Ts)*(Ts)*(Wc)*(Wc)-8.) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKb2 = Q15_FRAC(QC_IIR2* (4.+(Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKa1 = Q15_FRAC(QC_IIR2* (8.-2.*(Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); \
							flt.qKa2 = Q15_FRAC(QC_IIR2* (4.*(Ts)*(Z)*(Wc)-4.-(Ts)*(Ts)*(Wc)*(Wc)) / (4.+4.*(Ts)*(Z)*(Wc)+(Ts)*(Ts)*(Wc)*(Wc))); }


#define INIT_IIR2_LPF(flt,xVal,Wc,Ts,Z) {	SET_IIR2_LPF_GAIN(flt,Wc,Ts,Z); \
							flt.mR2 = MUL_SS((xVal), (flt.qKb2 + flt.qKa2)); flt.mR1 = flt.mR2 + MUL_SS((xVal), (flt.qKb1 + flt.qKa1));	}

#define INIT_IIR2_HPF(flt,xVal,Wc,Ts,Z) {	SET_IIR2_HPF_GAIN(flt,Wc,Ts,Z); \
							flt.mR2 = MUL_SS((xVal), flt.qKb2); flt.mR1 = flt.mR2 + MUL_SS((xVal), flt.qKb1);	}

#define INIT_IIR2_ALLPASS(flt,xVal,Wc,Ts,Z) {	SET_IIR2_ALLPASS_GAIN(flt,Wc,Ts,Z) \
							flt.mR2 = MUL_SS((xVal), (flt.qKb2 + flt.qKa2)); flt.mR1 = flt.mR2 + MUL_SS((xVal), (flt.qKb1 + flt.qKa1));	}

#define INIT_IIR2_BPF(flt,xVal,Wc,Ts,Z) {	SET_IIR2_BPF_GAIN(flt,Wc,Ts,Z) \
							flt.mR2 = MUL_SS((xVal), (flt.qKb2 + flt.qKa2)); flt.mR1 = flt.mR2 + MUL_SS((xVal), (flt.qKb1 + flt.qKa1));	}

#define INIT_IIR2_NOTCH(flt,xVal,Wc,Ts,Z) {	SET_IIR2_NOTCH_GAIN(flt,Wc,Ts,Z) \
							flt.mR2 = MUL_SS((xVal), (flt.qKb2 + flt.qKa2)); flt.mR1 = flt.mR2 + MUL_SS((xVal), (flt.qKb1 + flt.qKa1));	}

#define DO_IIR2(flt, x, y) {	long mY = (flt.mR1 + MUL_SS(flt.qKb0, (x)));  \
								flt.mR1 = (flt.mR2 + MUL_SS(flt.qKb1, (x)) + MUL_LL(mY, flt.qKa1, SCALE_IIR2)); \
								flt.mR2 = (MUL_SS(flt.qKb2, (x)) + MUL_LL(mY, flt.qKa2, SCALE_IIR2));	\
								(y) = (Q15)(mY >> SCALE_IIR2);	}

// PI controller ---------------------------------------------
typedef struct 
{
	Q15 qKp;
	Q15 qKiT;
	Q15 qLimP;
	Q15 qLimN;
	Q30 qdInteg;
} TCtrlPIL;

#define SET_PIL1_GAIN(ctrlPI, xKp, xKi, Ts, QMaxIn, QMaxOut)	do { \
							ctrlPI.qKp = (Q15)(xKp * (1L <<SCALE_PIL1_KPIT) * QMaxIn / QMaxOut + 0.5);\
							ctrlPI.qKiT = (Q15)(xKi * Ts * (1L <<SCALE_PIL1_KPIT) * QMaxIn / QMaxOut + 0.5); }while(0)

typedef struct 
{
	Q15 qKp;
	Q15 qKiT;
	Q15 qKcT;
	Q15 qVff;
	Q15 qVref;
	Q15 qLimP;
	Q15 qLimN;
	Q30 qdInteg;
} TCtrlPIA;

#define SET_PIA1_GAIN(ctrlPI, xKp, xKi, xKa, Ts, QMaxIn, QMaxOut)	do { \
							ctrlPI.qKp = (Q15)(xKp * (1L <<SCALE_PIA1_KP) * QMaxIn / QMaxOut + 0.5);\
							ctrlPI.qKiT = (Q15)(xKi * Ts * (1L <<SCALE_PIA1_KICT) * QMaxIn / QMaxOut + 0.5);\
							ctrlPI.qKcT = (Q15)(xKa * xKi * Ts * (1L <<SCALE_PIA1_KICT) + 0.5);	}while(0)

// MTPA controller ---------------------------------------------

#ifndef COEF_MTPA
#define COEF_MTPA		1.0
#endif

#if defined(_MOTOR_PARAM_) || defined(_TUNING_MTPA_)
#ifdef	_MC_UTIL_C_
GLOBAL float fCoefMtpa = COEF_MTPA;
#else
GLOBAL float fCoefMtpa;
#endif
GLOBAL Q15 Q15_ID_MTPA_BASE;
#define UPDATE_GAIN_MTPA()		do {	\
	Q15_ID_MTPA_BASE = (((LQ > LD) && (fCoefMtpa > 0.)) ? Q_CURRENT(KE / (fCoefMtpa * 4. * (LQ - LD))) : 32767);	\
} while (0)

#else
#define Q15_ID_MTPA_BASE		((LQ > LD) ? Q_CURRENT(KE / (COEF_MTPA * 4. * (LQ - LD))) : 32767)
#define UPDATE_GAIN_MTPA()
#endif


#define SLFLAG_WREF_NON				(0x00030000)
#define SLFLAG_WREF_NEGATIVE		(0x00030001)
#define SLFLAG_WREF_POSITIVE		(0x00030002)
#define SLFLAG_WREF_ZERO			(0x00030003)

#if defined(_ACT_SENSORLESS_)
#if !defined(_INLINE_)	//ndef _INLINE_
void SetEstFlag(TEstimate *xpEst, DWORD xdwFlag);

#else
#define SetEstFlag(xpEst, xdwFlag)		do { \
	(xpEst)->wFlag = (((xpEst)->wFlag & ~((xdwFlag) >> 16)) | (WORD)(xdwFlag));	} while(0)
	
#endif

#else	// #if defined(_ACT_SENSORLESS_)
#define SetEstFlag(xpEst, xdwFlag)
#define IsFailEst(xpEst)		FALSE
#define SetRsEst(xpEst, xqVs)	

#endif 

//---------------------------------------------------------
#if defined(_SL_MATSUI_)
#ifndef COEF_K_THETA
#define COEF_K_THETA			10.0
#endif
#ifndef SCALE_K_THETA
#define SCALE_K_THETA			8
#endif
#define GAIN_K_THETA			(Q15)(COEF_K_THETA * LD / TS_CC * (1L <<SCALE_K_THETA) * QMAX_CURRENT / QMAX_OMEGA_R + 0.5)

#ifndef COEF_K_EST
#define COEF_K_EST				0.025
#endif
#ifndef SCALE_K_EST
#define SCALE_K_EST				15
#endif
#define GAIN_K_EST				(Q15)(COEF_K_EST * LQ / TS_CC * (1L <<SCALE_K_EST) * QMAX_CURRENT / QMAX_VOLT + 0.5)

#define SCALE_TUNING_K_EST		13
#define QMAX_TUNING_K_EST		((1L <<15) / (1L <<SCALE_TUNING_K_EST))
#define LIMLT_N_TUNING_K_EST	(Q15)(0.3 * (1L <<SCALE_TUNING_K_EST) + 0.5)
#define LIMLT_P_TUNING_K_EST	(Q15)(1.8 * (1L <<SCALE_TUNING_K_EST) + 0.5)

#define KP_TUNING_K_EST			0.00003 //0.0002 //0.0	//  
#define KI_TUNING_K_EST			0.02 // 0.0	//

#ifndef VD_EST_OFFSET
#define VD_EST_OFFSET			Q_VOLT(0.) // 16.0)
#endif

#ifndef WC_LPF_EEST
#define WC_LPF_EEST				(500.0 * 2. * PI)
#endif
#ifndef WC_LPF_THETAC
#define WC_LPF_THETAC			(200.0 * 2. * PI)
#endif
#ifndef WC_LPF_WR_EST
#define WC_LPF_WR_EST			(20.0 * 2. * PI)
#endif

#ifndef SCALE_MA
#define SCALE_MA			13
#endif

#define Q_MA_LQTS_LD		(Q15)(LQ * TS_CC / LD * (1L <<SCALE_MA) * QMAX_OMEGA_R * QMAX_CURRENT / QMAX_CURRENT + 0.5)
#define Q_MA_RSTS_LD		(Q15)(RS * TS_CC / LD * (1L <<SCALE_MA) * QMAX_CURRENT / QMAX_CURRENT + 0.5)
#define Q_MA_TS_LD			(Q15)(TS_CC / LD * (1L <<SCALE_MA) * QMAX_VOLT / QMAX_CURRENT + 0.5)

#define Q_MA_LDTS_LQ		(Q15)(LD * TS_CC / LQ * (1L <<SCALE_MA) * QMAX_OMEGA_R * QMAX_CURRENT / QMAX_CURRENT + 0.5)
#define Q_MA_RSTS_LQ		(Q15)(RS * TS_CC / LQ * (1L <<SCALE_MA) * QMAX_CURRENT / QMAX_CURRENT + 0.5)
#define Q_MA_TS_LQ			(Q15)(TS_CC / LQ * (1L <<SCALE_MA) * QMAX_VOLT / QMAX_CURRENT + 0.5)
//0.5* 0.5* 0.5* 0.5* 1.6* 1.6* 1.6* 1.6* 
typedef struct 
{
	TAngle	oAngle;
	Q15 qWrpmLpf;
	WORD wFlag;

	Q15 qWrhat;
	TAxisDQ oIeEst;
	
	Q15 qKtheta;
	Q15 qKest;
	
	long	qdEhatInteg;
	long	qdThetaInteg;

	TLpf1_Q15 oLpfEhat;
	TLpf1_Q15 oLpfThataC;
	TLpf1_Q15 oLpfWr;
	TCtrlPIL oTuningKest;
} TEstimate;

#define IsFailEst(xpEst)		FALSE
#define SetRsEst(xpEst, xqVs)	

//---------------------------------------------------------
#elif defined(_SL_EX_EMF_)
#define WC_EST_EMF			(2.0 * PI * 60)

#define SCALE_WCLD			14
#define GAIN_WCLD			(Q15)(WC_EST_EMF * LD * (1L <<SCALE_WCLD) * QMAX_CURRENT / (QMAX_VOLT) + 0.5)

#define KP_EST_EMF_PLL			(50.0 * 6.0)
#define KI_EST_EMF_PLL			(50.0 * 50.0 * 6.0)

typedef struct 
{
	TAngle	oAngle;
	Q15 qWrpmLpf;
	WORD wFlag;

	TLpf1_Q15 oLpfVd;
	TLpf1_Q15 oLpfVq;

	THpf1_Q15 oHpfVd;
	THpf1_Q15 oHpfVq;

	TLpf1_Q15 oLpfWr;

	TCtrlPIL oCtrlPll;
	long	qdThetaInteg;
	
} TEstimate;

#define IsFailEst(xpEst)			FALSE
#define SetRsEst(xpEst, xqVs)

//---------------------------------------------------------
#elif defined(_SL_MO_EMF_)
#ifndef SCALE_MO
#define SCALE_MO			13
#endif

#define Q_MO_LQTS_LD		(Q15)(LQ * TS_CC / LD * (1L <<SCALE_MO) * QMAX_OMEGA_R * QMAX_CURRENT / QMAX_CURRENT + 0.5)
#define Q_MO_RSTS_LD		(Q15)(RS * TS_CC / LD * (1L <<SCALE_MO) * QMAX_CURRENT / QMAX_CURRENT + 0.5)
#define Q_MO_TS_LD			(Q15)(TS_CC / LD * (1L <<SCALE_MO) * QMAX_VOLT / QMAX_CURRENT + 0.5)

#define WC_LPF_VDQ_MO			(500.0 * 2.0* PI)
// #define WC_LPF_WR_MO			(500.0 * 2.0* PI)
#define WC_LPF_WR_MO			(100.0 * 2.0* PI)

#ifndef WC_ESTCC_MO
#define WC_ESTCC_MO				(2.0 * PI * FREQ_PWM / 32.0)
#endif
#define KP_ESTCC_MO				(LD * WC_ESTCC_MO)
#define KI_ESTCC_MO				(RS * WC_ESTCC_MO)

#ifndef WC_PLL_MO
#define WC_PLL_MO				(5.0 * 2.0 * PI)
#endif
#define KP_PLL_MO				(0.707 * 1.0 * WC_PLL_MO) 
#define KI_PLL_MO				(WC_PLL_MO * WC_PLL_MO) 

typedef struct 
{
	TAngle	oAngle;
	Q15 qWrpmLpf;
	WORD wFlag;

	TLpf1_Q15 oLpfVd;
	TLpf1_Q15 oLpfVq;

	long	qdIDestInteg;
	long	qdIQestInteg;

	TAxisDQ oIdqest;
	TAxisDQ oVdqest;
	TAxisDQ oEdqest;

	TCtrlPIL oCtrlEgam;
	TCtrlPIL oCtrlEdel;

	TLpf1_Q15 oLpfWr;

	TCtrlPIL oCtrlPll;
	long	qdThetaInteg;
	
} TEstimate;

#define IsFailEst(xpEst)			FALSE
#define SetRsEst(xpEst, xqVs)

//---------------------------------------------------------
#elif defined(_SL_FLUX_)

#ifndef WB_HPF_FLUX
#define WB_HPF_FLUX				(QMAX_OMEGA_R * 0.004167) // (5.0 * 2.0 * PI)
#endif
#ifndef SCALE_WB_HPF
#define SCALE_WB_HPF			15
#endif
#define GAIN_WB_HPF				(Q15)(WB_HPF_FLUX * (1L << SCALE_WB_HPF) * QMAX_FULX / QMAX_VOLT + 0.5)

#ifndef SCALE_VOLT_INTEG
#define SCALE_VOLT_INTEG		15
#endif
#define GAIN_VOLT_INTEG			(Q15)(TS_CC * (1L << SCALE_VOLT_INTEG) * QMAX_VOLT / QMAX_FULX + 0.5)

#ifndef SCALE_L01_FLUX
#define SCALE_L01_FLUX			13
#endif
#define EGAIN_L0_FLUX			(Q15)(((LQ + LD) / 2.) * (1L <<SCALE_L01_FLUX) * QMAX_CURRENT / QMAX_FULX + 0.5)
#define EGAIN_L1_FLUX			(Q15)(((LQ - LD) / 2.) * (1L <<SCALE_L01_FLUX) * QMAX_CURRENT / QMAX_FULX + 0.5)

#if defined(_MOTOR_PARAM_)
GLOBAL Q15 GAIN_L0_FLUX;
GLOBAL Q15 GAIN_L1_FLUX;
#define UPDATE_GAIN_SL()		do {	\
	GAIN_L0_FLUX = EGAIN_L0_FLUX;	GAIN_L1_FLUX = EGAIN_L1_FLUX;	\
} while (0)
#else
#define GAIN_L0_FLUX			EGAIN_L0_FLUX
#define GAIN_L1_FLUX			EGAIN_L1_FLUX
#define UPDATE_GAIN_SL()
#endif

#ifndef WC_LPF_WR_FLUX
#define WC_LPF_WR_FLUX			(20.0 * 2.0* PI)
#endif

#ifndef WC_PLL_FLUX
#define WC_PLL_FLUX				(47.0 * 2.0 * PI)
#endif
#ifndef ZETA_PLL_FLUX
#define ZETA_PLL_FLUX			1.0 
#endif
#define KP_PLL_FLUX				(WC_PLL_FLUX * ZETA_PLL_FLUX * 2.0) // (W_PLL * 6.0)
#define KI_PLL_FLUX				(WC_PLL_FLUX * WC_PLL_FLUX) //  (W_PLL * W_PLL * 6.0)

typedef struct 
{
	TAngle	oAngle;
	Q15 qWrpmLpf;
	WORD wFlag;

	Q15 qRsVeq;
	
	Q15 qFluxD;
	Q15 qFluxQ;

//	Q15 qThetaPll;
	
	long	qdFluxDInteg;
	long	qdFluxQInteg;
	long	qdThetaInteg;

//	TLpf1_Q15 oLpfLamD;
//	TLpf1_Q15 oLpfLamQ;

//	THpf1_Q15 oHpfLamD;
//	THpf1_Q15 oHpfLamQ;

/*	TCtrlPIL oCtrlLamD;
	TCtrlPIL oCtrlLamQ;
*/	
	TLpf1_Q15 oLpfWr;
	TCtrlPIL oCtrlPll;

} TEstimate;

#ifndef NMIN_FLUX
#define NMIN_FLUX			(800)
#endif

// #define IsFailEst(xpEst)			((abs((xpEst)->qFluxD) + abs((xpEst)->qFluxQ)) <= NMIN_FLUX)
#define IsFailEst(xpEst)			((MUL_SS((xpEst)->qFluxD, (xpEst)->qFluxD) + MUL_SS((xpEst)->qFluxQ, (xpEst)->qFluxQ)) <= (long)(NMIN_FLUX * NMIN_FLUX)) 
#define SetRsEst(xpEst, xqVs)		do { (xpEst)->qRsVeq = xqVs; }while(0)

//---------------------------------------------------------
#elif defined(_SL_INTEGRAL_)

#define KP_PLL_INTEG			(50.0 * 6.0)
#define KI_PLL_INTEG			(50.0 * 50.0 * 6.0)

#define SCALE_LS_FLUX			12
#define GAIN_LD_FLUX			(Q15)(LD * QMAX_CURRENT * (1L <<SCALE_LS_FLUX) / QMAX_FULX + 0.5)
#define GAIN_LQ_FLUX			(Q15)(LQ * QMAX_CURRENT * (1L <<SCALE_LS_FLUX) / QMAX_FULX + 0.5)

typedef struct 
{
	TAngle	oAngle;
	Q15 qWrpmLpf;
	WORD wFlag;

	TLpf1_Q15 oLpfRamD;
	TLpf1_Q15 oLpfRamQ;

	TLpf1_Q15 oLpfWr;

	TCtrlPIL oCtrlPll;
	long	qdThetaInteg;
	
} TEstimate;

#define IsFailEst(xpEst)			FALSE
#define SetRsEst(xpEst, xqVs)

//---------------------------------------------------------
#elif defined(_SL_QUASI_)

#define KP_PLL_QUASI			(50.0 * 6.0)
#define KI_PLL_QUASI			(50.0 * 50.0 * 6.0)

typedef struct 
{
	TAngle	oAngle;
	Q15 qWrpmLpf;
	WORD wFlag;

	TLpf1_Q15 oLpfWr;

	TCtrlPIL oCtrlPll;
	long	qdThetaInteg;
	
} TEstimate;

#define IsFailEst(xpEst)			FALSE
#define SetRsEst(xpEst, xqVs)	

#endif


// Hall Sensor Observer speed and magnetic flux position estimator (=�ӵ� �� �ڼ���ġ ������) ///////////////////////////////////
#if defined(_EST_HALL_OBS_) || defined(_EST_ENCODER_)
#ifndef QMAX_TI_OBS
#define QMAX_TI_OBS				700.0
#endif
#ifndef JM_OBS
#define JM_OBS					(0.5)
#endif
#ifndef B_OBS
#define B_OBS					(0.0001)
#endif
#ifndef X_OBS
#define X_OBS					32.0	// 48.0 // 100.0	// 
#endif

#define L1_OBS					(3.0 * X_OBS - B_OBS / JM_OBS)
#define L2_OBS					(3.0 * X_OBS * X_OBS - L1_OBS * B_OBS / JM_OBS)
#define L3_OBS					(X_OBS * X_OBS * X_OBS * JM_OBS)

#define Q15_OBS_L3TS			Q15_FRAC(L3_OBS * TS_CC * QMAX_THETA_RD / QMAX_TI_OBS)
#define Q15_OBS_BTS_JM			Q15_FRAC(B_OBS * TS_CC / JM_OBS)
#define Q15_OBS_TS_JM			Q15_FRAC(TS_CC / JM_OBS * QMAX_TI_OBS / QMAX_OMEGA_RD)
#define Q15_OBS_L2TS			Q15_FRAC(L2_OBS * TS_CC * QMAX_THETA_RD / QMAX_OMEGA_RD)
#define Q15_OBS_TS				Q15_FRAC(TS_CC * QMAX_OMEGA_RD / QMAX_THETA_RD)
#define Q15_OBS_L1TS			Q15_FRAC(L1_OBS * TS_CC)

typedef struct 
{
	TAngle	oAngle;
	Q15 	qWrpmLpf;
	Q30	qdTiInteg;	
	Q30	qdWrInteg;
	Q30	qdThetaInteg;
	TLpf1_Q15 oLpfWr;	
	Q15 	qThetaErr;
	Q15 	qThetaObs;
	TLpf1_Q15 oLpfThetaErr;
} TObserver;

void InitObserver(TObserver *xpObs, Q15 xqTheta);
void DoObserver(TObserver *xpObs, Q15 xqThetaHall);

#endif	// #if defined(_EST_HALL_OBS_) || defined(_EST_ENCODER_)


// Hall INT MT Measure speed and magnetic flux position estimator (=�ӵ� �� �ڼ���ġ ������) ////////////////////////////////////////
#if defined(_EST_HALL_MT_)
#define TS_MT						0.01

#define FREQ_MT_FR_TMR				(FREQ_CLK / (1 <<7))
#define GAIN_MT_CONST_FR			(S32)((60.0 * FREQ_MT_FR_TMR * 0x8000) /(4.0 * POLE_PAIR * QMAX_OMEGA_RPM))
#define GAIN_MT_CONST_PWM			(S32)((60.0 * FREQ_PWM * 0x8000) /(4.0 * POLE_PAIR * QMAX_OMEGA_RPM))

#define T_MT_SPEED_ZERO				0.4							// Hall signal interval that determines the speed as ��0�� (=�ӵ��� "0"�� �Ǵ��ϴ� Hall��ȣ ����) 
#define T_MT_FR_OF					(30000 / FREQ_MT_FR_TMR)

#define Q15_MT_KP_PLL				(Q30)(100.0 * (1L<<15) * QMAX_THETA_R / QMAX_OMEGA_R + 0.5)

typedef struct 
{
	TAngle	oAngle;
	Q15 	qWrpmLpf;
	Q15 	qWrpmPll;
	WORD	wHall;
	BOOLEAN	bPllFlag;
	U16		unPwmTime;
	int		nIntervalTmr;
	S16 	nWiseCnt;	
	U16		unHallFrTime;
	U16		unHallPwmTime;
	U16 	unHallFrCap;				// Free-run timer value of recent hall signal detection during speed measurement cycle (=�ӵ����� �ֱ�� �ֱ� Ȧ��ȣ ������ ������ Ÿ�̸Ӱ�)
	U16 	unHallPwmCap;				// PWM timer value of recent hall signal detection during speed measurement cycle (=�ӵ����� �ֱ�� �ֱ� Ȧ��ȣ ������ PWM Ÿ��Ӱ�)
	S16 	nWiseCntCap;				// Hall count value of recent Hall signal detection during speed measurement cycle (=�ӵ����� �ֱ�� �ֱ� Ȧ��ȣ ������ Ȧ ī��Ʈ��)
	Q30		qdThetaInteg;
	TLpf1_Q15 oLpfWr;	
	TLpf1_Q15 oLpfThetaErr;
} TMeasureMT;

typedef struct 
{
	int		nWise;
	Q15 	qThetaEadge;
	Q15 	qThetaCenter;
} THallTheta;

void InitMeasureMT(TMeasureMT *xpMT, WORD xwHall);
void IntHallMeasureMT(TMeasureMT *xpMT, WORD xwHall);
int DoMeasureMT(TMeasureMT *xpMT);

#endif	// #if defined(_EST_HALL_MT_)

/***************************************************************************************************
 *	[Constant & Variable Declarations]
 **************************************************************************************************/

#if defined(_MOTOR_PARAM_)
#define UPDATE_UTIL_GAIN()		do {	\
	UPDATE_GAIN_PARAM();	UPDATE_GAIN_SL();	UPDATE_GAIN_MTPA();	\
} while (0)
#else
#define UPDATE_UTIL_GAIN()
#endif

/***************************************************************************************************
 *	[Function Declaration]
 **************************************************************************************************/
int SqrtS32(long xlVal);
Q15 ATan2(int xnX, int xnY);

Q15 DoCtrlPIL1(TCtrlPIL *xpParamPI, Q15 xqErr);
Q15 DoCtrlPIA1(TCtrlPIA *xpParamPI, Q15 xqErr);
Q15 DoCtrlPIA1_Ref(TCtrlPIA *xpParamPI, Q15 xqErr);
Q15 DoCtrlPIA1_Lim(TCtrlPIA *xpParamPI);

void SetAngle(TAngle *xpAngle, Q15 xTheta);

#if !defined(_INLINE_)	//ndef _INLINE_
void ConvABC2DQS(TAxisABC *xpABC, TAxisDQ *xpDQS);
void ConvDQS2DQE(TAxisDQ *xpDQS, TAxisDQ *xpDQE, TAngle *xpAngle);
void ConvDQE2DQS(TAxisDQ *xpDQE, TAxisDQ *xpDQS, TAngle *xpAngle);
void ConvDQS2ABC(TAxisDQ *xpDQS, TAxisABC *xpABC);

#else

#define ConvABC2DQS(xpABC, xpDQS)		do { \
	(xpDQS)->qD = (xpABC)->qA;	\
	(xpDQS)->qQ = MUL_SS(Q15_FRAC(1.0 / SQRT3), (xpABC)->qB - (xpABC)->qC) >> 15;	} while(0)

#define ConvDQS2DQE(xpDQS, xpDQE, xpAngle)		do { \
	(xpDQE)->qD = (MUL_SS((xpDQS)->qD, (xpAngle)->qCos) + MUL_SS((xpDQS)->qQ, (xpAngle)->qSin)) >> 15;	\
	(xpDQE)->qQ = (MUL_SS(-(xpDQS)->qD, (xpAngle)->qSin) + MUL_SS((xpDQS)->qQ, (xpAngle)->qCos)) >> 15;	} while(0)

#define ConvDQE2DQS(xpDQE, xpDQS, xpAngle)		do { \
	(xpDQS)->qD = (MUL_SS((xpDQE)->qD, (xpAngle)->qCos) - MUL_SS((xpDQE)->qQ, (xpAngle)->qSin)) >> 15;	\
	(xpDQS)->qQ = (MUL_SS((xpDQE)->qD, (xpAngle)->qSin) + MUL_SS((xpDQE)->qQ, (xpAngle)->qCos)) >> 15;	} while(0)

#define ConvDQS2ABC(xpDQS, xpABC)		do { \
	(xpABC)->qA = (xpDQS)->qD;	\
	(xpABC)->qB = -((xpDQS)->qD >>1) + ((MUL_SS(Q15_FRAC(SQRT3 / 2.0), (xpDQS)->qQ)) >> 15);	\
	(xpABC)->qC = -((xpABC)->qA + (xpABC)->qB);	} while(0)
	
#endif

void SetRampTarget(TRampRef *xpRamp, Q15 xqTarget, Q15 xqSlope);
void SetRampRef(TRampRef *xpRamp, Q15 xqRef);
void DoRampRef(TRampRef *xpRamp);

void CtrlMTPA(Q15 xqIt, TAxisDQ *xpIdqRef);

#if defined(_ACT_SENSORLESS_)
void InitEstimator(TEstimate *xpEst, Q15 xqTheta);
void DoEstimate(TEstimate *xpEst, TAxisDQ *xpIdqss, TAxisDQ *xpVdqss);
#endif

WORD ReadADC(WORD xwOffSetSector,TAxisABC *xpIabcs, Q15 *xpqVdc);
WORD GenPWM(TAxisABC *xpVabcs, Q15 xqVdc);


/***************************************************************************************************
 *	[End Of Header File]
 **************************************************************************************************/
#endif