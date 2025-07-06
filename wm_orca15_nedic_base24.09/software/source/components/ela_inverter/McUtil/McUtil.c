/**===========================================================================
 * @file		McUtil.c
==============================================================================*/

#define _MC_UTIL_C_

/***************************************************************************************************
 *	[Include File]
 **************************************************************************************************/
#include "../McApp/McProject.h"
#include "McUtil.h"
#include "../core/tmpm370/inc/tmpm370_adc.h"
#include "../core/tmpm370/inc/tmpm370_pmd.h"


/***************************************************************************************************
 *	[Define]
 **************************************************************************************************/

/***************************************************************************************************
 *	[Enumeration & TypeDeclarations]
 **************************************************************************************************/



/* [3 Shunt current detection]) */
#define SAVE_PWM_REAR(xRearA, xRearB, xRearC)		// PWM Rear Time ������� ���� 
#define SET_I_ORDER(xMax, xMed, xMin)		{ }


/***************************************************************************************************
 *	[Constant & Variable Declarations]
 **************************************************************************************************/
const Q15 caqSinTblD[512 +1] =
{
	0,		402,	804,	1206,	1608,	2009,	2411,	2811,
	3212,	3612,	4011,	4410,	4808,	5205,	5602,	5998,
	6393,	6787,	7180,	7571,	7962,	8351,	8740,	9127,
	9512,	9896,	10279,	10660,	11039,	11417,	11793,	12167,
	12540,	12910,	13279,	13646,	14010,	14373,	14733,	15091,
	15447,	15800,	16151,	16500,	16846,	17190,	17531,	17869,
	18205,	18538,	18868,	19195,	19520,	19841,	20160,	20475,
	20788,	21097,	21403,	21706,	22006,	22302,	22595,	22884,
	23170,	23453,	23732,	24008,	24279,	24548,	24812,	25073,
	25330,	25583,	25833,	26078,	26320,	26557,	26791,	27020,
	27246,	27467,	27684,	27897,	28106,	28311,	28511,	28707,
	28899,	29086,	29269,	29448,	29622,	29792,	29957,	30118,
	30274,	30425,	30572,	30715,	30853,	30986,	31114,	31238,
	31357,	31471,	31581,	31686,	31786,	31881,	31972,	32058,
	32138,	32214,	32286,	32352,	32413,	32470,	32522,	32568,
	32610,	32647,	32679,	32706,	32729,	32746,	32758,	32766,
	32767,	32766,	32758,	32746,	32729,	32706,	32679,	32647,
	32610,	32568,	32522,	32470,	32413,	32352,	32286,	32214,
	32138,	32058,	31972,	31881,	31786,	31686,	31581,	31471,
	31357,	31238,	31114,	30986,	30853,	30715,	30572,	30425,
	30274,	30118,	29957,	29792,	29622,	29448,	29269,	29086,
	28899,	28707,	28511,	28311,	28106,	27897,	27684,	27467,
	27246,	27020,	26791,	26557,	26320,	26078,	25833,	25583,
	25330,	25073,	24812,	24548,	24279,	24008,	23732,	23453,
	23170,	22884,	22595,	22302,	22006,	21706,	21403,	21097,
	20788,	20475,	20160,	19841,	19520,	19195,	18868,	18538,
	18205,	17869,	17531,	17190,	16846,	16500,	16151,	15800,
	15447,	15091,	14733,	14373,	14010,	13646,	13279,	12910,
	12540,	12167,	11793,	11417,	11039,	10660,	10279,	9896,
	9512,	9127,	8740,	8351,	7962,	7571,	7180,	6787,
	6393,	5998,	5602,	5205,	4808,	4410,	4011,	3612,
	3212,	2811,	2411,	2009,	1608,	1206,	804,	402,
	0,		-402,	-804,	-1206,	-1608,	-2009,	-2411,	-2811,
	-3212,	-3612,	-4011,	-4410,	-4808,	-5205,	-5602,	-5998,
	-6393,	-6787,	-7180,	-7571,	-7962,	-8351,	-8740,	-9127,
	-9512,	-9896,	-10279,	-10660,	-11039,	-11417,	-11793,	-12167,
	-12540,	-12910,	-13279,	-13646,	-14010,	-14373,	-14733,	-15091,
	-15447,	-15800,	-16151,	-16500,	-16846,	-17190,	-17531,	-17869,
	-18205,	-18538,	-18868,	-19195,	-19520,	-19841,	-20160,	-20475,
	-20788,	-21097,	-21403,	-21706,	-22006,	-22302,	-22595,	-22884,
	-23170,	-23453,	-23732,	-24008,	-24279,	-24548,	-24812,	-25073,
	-25330,	-25583,	-25833,	-26078,	-26320,	-26557,	-26791,	-27020,
	-27246,	-27467,	-27684,	-27897,	-28106,	-28311,	-28511,	-28707,
	-28899,	-29086,	-29269,	-29448,	-29622,	-29792,	-29957,	-30118,
	-30274,	-30425,	-30572,	-30715,	-30853,	-30986,	-31114,	-31238,
	-31357,	-31471,	-31581,	-31686,	-31786,	-31881,	-31972,	-32058,
	-32138,	-32214,	-32286,	-32352,	-32413,	-32470,	-32522,	-32568,
	-32610,	-32647,	-32679,	-32706,	-32729,	-32746,	-32758,	-32766,
	-32767,	-32766,	-32758,	-32746,	-32729,	-32706,	-32679,	-32647,
	-32610,	-32568,	-32522,	-32470,	-32413,	-32352,	-32286,	-32214,
	-32138,	-32058,	-31972,	-31881,	-31786,	-31686,	-31581,	-31471,
	-31357,	-31238,	-31114,	-30986,	-30853,	-30715,	-30572,	-30425,
	-30274,	-30118,	-29957,	-29792,	-29622,	-29448,	-29269,	-29086,
	-28899,	-28707,	-28511,	-28311,	-28106,	-27897,	-27684,	-27467,
	-27246,	-27020,	-26791,	-26557,	-26320,	-26078,	-25833,	-25583,
	-25330,	-25073,	-24812,	-24548,	-24279,	-24008,	-23732,	-23453,
	-23170,	-22884,	-22595,	-22302,	-22006,	-21706,	-21403,	-21097,
	-20788,	-20475,	-20160,	-19841,	-19520,	-19195,	-18868,	-18538,
	-18205,	-17869,	-17531,	-17190,	-16846,	-16500,	-16151,	-15800,
	-15447,	-15091,	-14733,	-14373,	-14010,	-13646,	-13279,	-12910,
	-12540,	-12167,	-11793,	-11417,	-11039,	-10660,	-10279,	-9896,
	-9512,	-9127,	-8740,	-8351,	-7962,	-7571,	-7180,	-6787,
	-6393,	-5998,	-5602,	-5205,	-4808,	-4410,	-4011,	-3612,
	-3212,	-2811,	-2411,	-2009,	-1608,	-1206,	-804,	-402,
	0
};

const WORD cawATanTbl[128+2] =
{
	0,		81,		163,	244,	326,	407,	489,	570,
	651,	732,	813,	894,	975,	1056,	1136,	1217,
	1297,	1377,	1457,	1537,	1617,	1696,	1775,	1854,
	1933,	2012,	2090,	2168,	2246,	2324,	2401,	2478,
	2555,	2632,	2708,	2784,	2860,	2935,	3010,	3085,
	3159,	3233,	3307,	3380,	3453,	3526,	3599,	3670,
	3742,	3813,	3884,	3955,	4025,	4095,	4164,	4233,
	4302,	4370,	4438,	4505,	4572,	4639,	4705,	4771,
	4836,	4901,	4966,	5030,	5094,	5157,	5220,	5282,
	5344,	5406,	5467,	5528,	5589,	5649,	5708,	5768,
	5826,	5885,	5943,	6000,	6058,	6114,	6171,	6227,
	6282,	6337,	6392,	6446,	6500,	6554,	6607,	6660,
	6712,	6764,	6815,	6867,	6917,	6968,	7018,	7068,
	7117,	7166,	7214,	7262,	7310,	7358,	7405,	7451,
	7498,	7544,	7589,	7635,	7679,	7724,	7768,	7812,
	7856,	7899,	7942,	7984,	8026,	8068,	8110,	8151,
	8192,	8192
};

/***************************************************************************************************
 *	[Function Declaration]
 **************************************************************************************************/
void CalcPWM(TPwmDuty *xpPwmDuty, Q15 xqVmax, Q15 xqVmed, Q15 xqVmin, Q15 xqVdc);

/***************************************************************************************************
Name:        SqrtS32   
Description: Square root calculation function (=������ ��� �Լ�) 
***************************************************************************************************/

const WORD cawSqrtTbl[128+1] =
{
	0,		2896,	4096,	5017,	5793,	6476,	7094,	7663,
	8192,	8689,	9159,	9606,	10033,	10443,	10837,	11217,
	11585,	11942,	12288,	12625,	12953,	13273,	13585,	13890,
	14189,	14482,	14768,	15050,	15326,	15597,	15864,	16126,
	16384,	16638,	16888,	17135,	17378,	17618,	17854,	18087,
	18318,	18545,	18770,	18992,	19212,	19429,	19644,	19856,
	20066,	20274,	20480,	20684,	20886,	21085,	21283,	21480,
	21674,	21867,	22058,	22247,	22435,	22621,	22806,	22989,
	23170,	23351,	23530,	23707,	23884,	24059,	24232,	24405,
	24576,	24746,	24915,	25083,	25249,	25415,	25580,	25743,
	25905,	26067,	26227,	26387,	26545,	26703,	26859,	27015,
	27170,	27324,	27477,	27629,	27780,	27931,	28081,	28230,
	28378,	28525,	28672,	28818,	28963,	29108,	29251,	29394,
	29537,	29678,	29819,	29960,	30099,	30238,	30377,	30515,
	30652,	30788,	30924,	31059,	31194,	31328,	31462,	31595,
	31727,	31859,	31991,	32122,	32252,	32382,	32511,	32640,
	32768
};

int SqrtS32(long xlVal)	// Calculation error : 0 ~ -23
{
	WORD twVal;
	WORD twRoot;
	WORD twDelta;
	
/*	if ((DWORD)xlVal < (1L << 16)) 
	{
		twVal = xlVal >> 0;

		twRoot = cawSqrtTbl[twVal >> 9];
		twDelta = cawSqrtTbl[(twVal >> 9) +1] - twRoot;
		twDelta = MUL_UU(twDelta, twVal & 0x01FF) >> 9;
		twRoot = (twRoot + twDelta) >> 7;
	}	*/
/*	if ((DWORD)xlVal < (1L << 18)) 
	{
		twVal = xlVal >> 2;

		twRoot = cawSqrtTbl[twVal >> 9];
		twDelta = cawSqrtTbl[(twVal >> 9) +1] - twRoot;
		twDelta = MUL_UU(twDelta, twVal & 0x01FF) >> 9;
		twRoot = (twRoot + twDelta) >> 6;
	}	*/
	if ((DWORD)xlVal < (1L << 20)) 
	{
		twVal = xlVal >> 4;

		twRoot = cawSqrtTbl[twVal >> 9];
		twDelta = cawSqrtTbl[(twVal >> 9) +1] - twRoot;
		twDelta = MUL_UU(twDelta, twVal & 0x01FF) >> 9;
		twRoot = (twRoot + twDelta) >> 5;
	}
	else if ((DWORD)xlVal < (1L << 26)) 
	{
		twVal = xlVal >> 10;
		
		twRoot = cawSqrtTbl[twVal >> 9];
		twDelta = cawSqrtTbl[(twVal >> 9) +1] - twRoot;
		twDelta = MUL_UU(twDelta, twVal & 0x01FF) >> 9;
		twRoot = (twRoot + twDelta) >> 2;
	}
	else if ((DWORD)xlVal < (1L << 30)) 
	{
		twVal = xlVal >> 14;
		
		twRoot = cawSqrtTbl[twVal >> 9];
		twDelta = cawSqrtTbl[(twVal >> 9) +1] - twRoot;
		twDelta = MUL_UU(twDelta, twVal & 0x01FF) >> 9;
		twRoot = (twRoot + twDelta) >> 0;
	}
	else twRoot = 32767;
	return (int)twRoot;
}

/***************************************************************************************************
Name:        ATan2   
Description:  
***************************************************************************************************/
Q15 ATan2(int xnX, int xnY)
{
	WORD twQuadTheta;
	WORD twTmpDelta;
	WORD twTmpTheta;
	WORD twTmpPara;

	if ((xnX | xnY) == 0) return 0;
	if (xnY > 0)
	{
		if (xnX <= 0)
		{
			xnX = -xnX;
			twQuadTheta = (1U * 0x4000);
		}
		else twQuadTheta = (0U * 0x4000);
	}
	else
	{
		xnY = -xnY;
		if (xnX > 0) twQuadTheta = (3U * 0x4000);
		else
		{
			xnX = -xnX;
			twQuadTheta = (2U * 0x4000);
		}
	}

	if (xnX > xnY)
	{
		if (twQuadTheta & 0x4000) twQuadTheta += 0x2000;
		twTmpPara = DIV_UD(MUL_UU(0x4000 ,xnY), xnX);
	}
	else
	{
		if ((twQuadTheta & 0x4000) == 0) twQuadTheta += 0x2000;
		twTmpPara = DIV_UD(MUL_UU(0x4000 ,xnX), xnY);
	}
	
	twTmpTheta = cawATanTbl[twTmpPara >> 7];
	twTmpDelta = cawATanTbl[(twTmpPara >> 7) +1] - twTmpTheta;
	twTmpDelta = MUL_UU(twTmpDelta, twTmpPara & 0x007F) >> 7;
	twTmpTheta += twTmpDelta;

	if (twQuadTheta & 0x2000) twTmpTheta = 0x2000 - twTmpTheta;
	return (Q15)((short)(twQuadTheta + twTmpTheta));
}

/***************************************************************************************************
Name:        DoCtrlPIL1   
Description: PI controller (including Only Limiter) (=PI ����� (Only Limitter ����))
***************************************************************************************************/
Q15 DoCtrlPIL1(TCtrlPIL *xpParamPI, Q15 xqErr)
{
	long tdqOut;

	xpParamPI->qdInteg += MUL_SS(xqErr, xpParamPI->qKiT);
	
	tdqOut = (xpParamPI->qdInteg + MUL_SS(xqErr, xpParamPI->qKp)) >> SCALE_PIL1_KPIT;
	
	if (tdqOut > xpParamPI->qLimP)
	{
		tdqOut = xpParamPI->qLimP;
		if (xpParamPI->qdInteg > (tdqOut << SCALE_PIL1_KPIT))
		{
			xpParamPI->qdInteg = tdqOut << SCALE_PIL1_KPIT;
		}
	}
	else if (tdqOut < xpParamPI->qLimN)
	{
		tdqOut = xpParamPI->qLimN;
		if (xpParamPI->qdInteg < (tdqOut << SCALE_PIL1_KPIT))
		{
		  xpParamPI->qdInteg = tdqOut << SCALE_PIL1_KPIT;
		}
	}
	
	return (Q15)tdqOut;
}

/***************************************************************************************************
Name:        DoCtrlPIA1   
Description: PI controller (Anti Windup, Feed Forword function)
***************************************************************************************************/
Q15 DoCtrlPIA1(TCtrlPIA *xpParamPI, Q15 xqErr)
{
	Q15 tqRef;
	Q15 tqOut;
	Q30 tqdInteg;
	
	tqdInteg = xpParamPI->qdInteg + MUL_SS(xqErr, xpParamPI->qKiT) ;
	tqRef = tqdInteg >> SCALE_PIA1_KICT;
	
	tqRef += MUL_SS(xqErr, xpParamPI->qKp) >> SCALE_PIA1_KP;
	tqRef += xpParamPI->qVff;

	tqOut = tqRef;
	
	if (tqOut > xpParamPI->qLimP)
	{
		tqOut = xpParamPI->qLimP;
	}
	else if (tqOut < xpParamPI->qLimN)
	{
		tqOut = xpParamPI->qLimN;
	}
	xpParamPI->qVref = tqRef;
	xpParamPI->qdInteg = tqdInteg - MUL_SS((tqRef - tqOut), xpParamPI->qKcT);
	
	return tqOut;
}

Q15 DoCtrlPIA1_Ref(TCtrlPIA *xpParamPI, Q15 xqErr)
{
	Q15 tqRef;
	
	xpParamPI->qdInteg = xpParamPI->qdInteg + MUL_SS(xqErr, xpParamPI->qKiT) ;
	tqRef = xpParamPI->qdInteg >> SCALE_PIA1_KICT;
	
	tqRef += MUL_SS(xqErr, xpParamPI->qKp) >> SCALE_PIA1_KP;
	tqRef += xpParamPI->qVff;

	xpParamPI->qVref = tqRef;
	return tqRef;
}

Q15 DoCtrlPIA1_Lim(TCtrlPIA *xpParamPI)
{
	Q15 tqOut;

	tqOut = xpParamPI->qVref;
	if (tqOut > xpParamPI->qLimP)
	{
		tqOut = xpParamPI->qLimP;
	}
	else if (tqOut < xpParamPI->qLimN)
	{
		tqOut = xpParamPI->qLimN;
	}
	xpParamPI->qdInteg = xpParamPI->qdInteg - MUL_SS((xpParamPI->qVref - tqOut), xpParamPI->qKcT);
	
	return tqOut;
}



/***************************************************************************************************
Name:        SetAngle   
Description: Calculation about Sin, Cos value of Theta 
***************************************************************************************************/
void SetAngle(TAngle *xpAngle, Q15 xTheta)
{
	Q15 tqSinCosB;	//
	Q15 tqSinCosD;	//
	WORD 	twRem;		//
	WORD	twIndex;	//
	
	xpAngle->qTheta = (short)xTheta;
	twRem = (WORD)xTheta & 0x007F;
	twIndex = (WORD)xTheta >> 7;

	tqSinCosB = caqSinTblD[twIndex];
	tqSinCosD = caqSinTblD[twIndex +1] - tqSinCosB;
	tqSinCosD = MUL_SS(twRem, tqSinCosD) >> 7;
	xpAngle->qSin = tqSinCosB + tqSinCosD;

	twIndex = (twIndex + 0x80) & 0x01FF;
	tqSinCosB = caqSinTblD[twIndex];
	tqSinCosD = caqSinTblD[twIndex +1] - tqSinCosB;
	tqSinCosD = MUL_SS(twRem, tqSinCosD) >> 7;
	xpAngle->qCos = tqSinCosB + tqSinCosD;
}

#if !defined(_INLINE_)	//ndef _INLINE_
/***************************************************************************************************
Name:        ConvABC2DQS   
Description: 3-phase stationary coordinate => d-q stationary coordinate conversion (=3�� ������ǥ => d-q ������ǥ ��ȯ)
***************************************************************************************************/
void ConvABC2DQS(TAxisABC *xpABC, TAxisDQ *xpDQS)
{
	xpDQS->qD = xpABC->qA;
	xpDQS->qQ = MUL_SS(Q15_FRAC(1.0 / SQRT3), xpABC->qB - xpABC->qC) >> 15;
/*
	xpDQS->qD = (MUL_SS(Q15_FRAC(2.0 / 3.0), xpABC->qA) - MUL_SS(Q15_FRAC(1.0 / 3.0), xpABC->qB + xpABC->qC)) >> 15;
	xpDQS->qQ = (MUL_SS(Q15_FRAC(1.0 / SQRT3), xpABC->qB - xpABC->qC)) >> 15;
*/
}

/***************************************************************************************************
Name:        ConvDQS2DQE   
Description: d-q stationary coordinate => d-q synchronous coordinate conversion (=d-q ������ǥ => d-q ������ǥ ��ȯ)
***************************************************************************************************/
void ConvDQS2DQE(TAxisDQ *xpDQS, TAxisDQ *xpDQE, TAngle *xpAngle)
{
	xpDQE->qD = (MUL_SS(xpDQS->qD, xpAngle->qCos) + MUL_SS(xpDQS->qQ, xpAngle->qSin)) >> 15;
	xpDQE->qQ = (MUL_SS(-xpDQS->qD, xpAngle->qSin) + MUL_SS(xpDQS->qQ, xpAngle->qCos)) >> 15;
}

/***************************************************************************************************
Name:        ConvDQE2DQS   
Description: d-q synchronous coordinate => d-q stationary coordinate conversion (=d-q ������ǥ => d-q ������ǥ ��ȯ)
***************************************************************************************************/
void ConvDQE2DQS(TAxisDQ *xpDQE, TAxisDQ *xpDQS, TAngle *xpAngle)
{
	xpDQS->qD = (MUL_SS(xpDQE->qD, xpAngle->qCos) - MUL_SS(xpDQE->qQ, xpAngle->qSin)) >> 15;
	xpDQS->qQ = (MUL_SS(xpDQE->qD, xpAngle->qSin) + MUL_SS(xpDQE->qQ, xpAngle->qCos)) >> 15;
}

/***************************************************************************************************
Name:        ConvDQS2ABC   
Description: d-q stationary coordinate => 3-phase stationary coordinate conversion (=d-q ������ǥ => 3�� ������ǥ ��ȯ)
***************************************************************************************************/
void ConvDQS2ABC(TAxisDQ *xpDQS, TAxisABC *xpABC)
{
	xpABC->qA = xpDQS->qD;
	xpABC->qB = (MUL_SS(Q15_FRAC(-0.5), xpDQS->qD) + MUL_SS(Q15_FRAC(SQRT3 / 2.0), xpDQS->qQ)) >> 15;
	xpABC->qC = -(xpABC->qA + xpABC->qB);
}
#endif // #ifndef _INLINE_


/***************************************************************************************************
Name:        SetRampTarget   
Description: Setting target speed and acceleration (=��ǥ�ӵ� �� ���ӵ� ����)
***************************************************************************************************/
void SetRampTarget(TRampRef *xpRamp, Q15 xqTarget, Q15 xqSlope)
{
	xpRamp->qTar = xqTarget;
	if (xqSlope < 0) 
	{
		xpRamp->qRef = xqTarget;
		xpRamp->qdInteg = (long)xqTarget << SCALE_SLOPE_INTEG;
	}
	else xpRamp->qSlope = xqSlope;
}

/***************************************************************************************************
Name:        SetRampRef   
Description: Command speed setting (=���ɼӵ� ����) 
***************************************************************************************************/
void SetRampRef(TRampRef *xpRamp, Q15 xqRef)
{
	xpRamp->qRef = xqRef;
	xpRamp->qdInteg = (long)xqRef << SCALE_SLOPE_INTEG;
}

/***************************************************************************************************
Name:        DoRampRef   
Description: Command speed acceleration/deceleration control (=���ɼӵ� ������ ����) 
***************************************************************************************************/
void DoRampRef(TRampRef *xpRamp)
{
	Q15 tqReference;
 	
	if (xpRamp->qRef != xpRamp->qTar)
	{
		if (xpRamp->qSlope < 0)
		{
			xpRamp->qRef = xpRamp->qTar;
			xpRamp->qdInteg = (long)xpRamp->qTar << SCALE_SLOPE_INTEG;
			return;
		}
		else if (xpRamp->qRef < xpRamp->qTar)
		{
			xpRamp->qdInteg += xpRamp->qSlope;
			tqReference = xpRamp->qdInteg >> SCALE_SLOPE_INTEG;
			
			if (tqReference >= xpRamp->qTar)
			{
				xpRamp->qRef = xpRamp->qTar;
				xpRamp->qdInteg = (long)xpRamp->qTar << SCALE_SLOPE_INTEG;
				return;
			}
		}
		else
		{
			xpRamp->qdInteg -= xpRamp->qSlope;
			tqReference = xpRamp->qdInteg >> SCALE_SLOPE_INTEG;
			
			if (tqReference <= xpRamp->qTar)
			{
				xpRamp->qRef = xpRamp->qTar;
				xpRamp->qdInteg = (long)xpRamp->qTar << SCALE_SLOPE_INTEG;
				return;
			}
		}
		xpRamp->qRef = tqReference;
	}
}


/***************************************************************************************************
Name:        CtrlMTPA   
Description: Maximum torque per ampere control (MTPA)
***************************************************************************************************/
void CtrlMTPA(Q15 xqIt, TAxisDQ *xpIdqRef)
{
#if !defined(_NON_MTPA_)
	if (Q15_ID_MTPA_BASE < 32767)
	{
		Q30 tdqIt;
		tdqIt = MUL_SS(xqIt, xqIt); 

		xpIdqRef->qD = Q15_ID_MTPA_BASE - SqrtS32(MUL_SS(Q15_ID_MTPA_BASE, Q15_ID_MTPA_BASE) + (tdqIt >> 1));
		if (xpIdqRef->qD > Q_CURRENT(0.0)) xpIdqRef->qD = Q_CURRENT(0.0);	// SqrtS32 ���� ���� 
		xpIdqRef->qQ = SIGN(xqIt) * SqrtS32(tdqIt - MUL_SS(xpIdqRef->qD, xpIdqRef->qD));
	}
	else
#endif
	{
		xpIdqRef->qD = 0;
		xpIdqRef->qQ = xqIt;
	}
}

/***************************************************************************************************
Name:        InitObserver   
Description: Hall Sensor velocity and position estimator initialization (=Hall Senser �ӵ� �� ��ġ ������ �ʱ�ȭ)  
***************************************************************************************************/
void InitObserver(TObserver *xpObs, Q15 xqTheta)
{
	xpObs->qThetaObs = xqTheta;
	SetAngle(&xpObs->oAngle, xqTheta);
	xpObs->qdThetaInteg = xqTheta << 15;
	
	xpObs->qdTiInteg = 0;
	xpObs->qdWrInteg = 0;
		
	INIT_LPF1(xpObs->oLpfWr, 0, 50.0 * 2.0* PI, TS_CC);

	xpObs->qThetaErr = Q_THETA_R(0.0);
	INIT_LPF1(xpObs->oLpfThetaErr, 0, 10.0 * 2.0* PI, TS_CC);
}

/***************************************************************************************************
Name:        DoObserver   
Description: Speed and position estimation from Hall Sensor Theta (=Hall Senser Theta ���� �ӵ� �� ��ġ ����) 
***************************************************************************************************/
void DoObserver(TObserver *xpObs, Q15 xqThetaHall)
{
	Q15 tqTemp;
	short tqTheta;
	long tqdThetaComp;

	tqTheta = xqThetaHall - xpObs->qThetaObs;
	DO_LPF1(xpObs->oLpfThetaErr, abs(tqTheta), xpObs->qThetaErr);

	xpObs->qdTiInteg -= MUL_SS(tqTheta, Q15_OBS_L3TS);
	tqTemp = xpObs->qdTiInteg >> 15;

	xpObs->qdWrInteg = xpObs->qdWrInteg - MUL_SS(xpObs->oLpfWr.qX1, Q15_OBS_BTS_JM)		
					- MUL_SS(tqTemp, Q15_OBS_TS_JM) + MUL_SS(tqTheta, Q15_OBS_L2TS);
	
	tqTemp = xpObs->qdWrInteg >> 15;
	DO_LPF1(xpObs->oLpfWr, tqTemp, xpObs->qWrpmLpf);
	xpObs->qdThetaInteg = xpObs->qdThetaInteg + MUL_SS(tqTemp, Q15_OBS_TS) + MUL_SS(tqTheta, Q15_OBS_L1TS);

	xpObs->qThetaObs = (short)(xpObs->qdThetaInteg >> 15);
	tqdThetaComp = xpObs->qdThetaInteg + MUL_SS(tqTemp, Q_OMEGAINTEG_T_OBSCOMP);
	SetAngle(&xpObs->oAngle, (short)(tqdThetaComp >> SCALE_OMEGAINTEG));
}

/***************************************************************************************************
Name:        ReadADC   
Description: Read AD data and set phase current and DC BUS voltage values (=AD �����͸� �о� ������ �� DC BUS ���а� ����) 
***************************************************************************************************/
WORD ReadADC(WORD xwOffSetSector,TAxisABC *xpIabcs, Q15 *xpqVdc)
{
	static WORD wOffSetRet = ADC_OFFSET_NOT;
	static unsigned int unOffCnt = 0;
	/* [3 Shunt current detection] (=[ 3 ��Ʈ ���� ���� ])============================================== */
	static Q15 qIAdOffSetA = AD15_OFFSET;
	static Q15 qIAdOffSetB = AD15_OFFSET;
	static Q15 qIAdOffSetC = AD15_OFFSET;	
	static long qdOffSumA;
	static long qdOffSumB;
	static long qdOffSumC;
        
        /*mabrouk edit*/
        Q15 qIAdcA  = (Q15)((ADC_GetConvertResult(TSB_ADA, ADC_REG0).All)>>1)  ;
        Q15 qIAdcB  = (Q15)((ADC_GetConvertResult(TSB_ADA, ADC_REG2).All)>>1)  ;
        Q15 qIAdcC  = (Q15)((ADC_GetConvertResult(TSB_ADA, ADC_REG1).All)>>1)  ;

	if (xwOffSetSector & ADC_OFFSET_FLAG_DO) 
	{
		xwOffSetSector &= ~ADC_OFFSET_FLAG_DO;
		if ((wOffSetRet & ADC_OFFSET_FLAG_DO) == 0)
		{
			wOffSetRet |= ADC_OFFSET_FLAG_DO;
			unOffCnt = 0;
		}
		if (unOffCnt == OFFSET_DELAY)
		{
			qdOffSumA = 0;
			qdOffSumB = 0;
			qdOffSumC = 0;		
		}
		qdOffSumA += qIAdcA;
		qdOffSumB += qIAdcB;
		qdOffSumC += qIAdcC;
		
		if (++unOffCnt >= ((1 << 13) + OFFSET_DELAY))/* @@@@@ */
		{
			qIAdOffSetA = qdOffSumA >> 13;
			qIAdOffSetB = qdOffSumB >> 13;
			qIAdOffSetC = qdOffSumC >> 13;
			unOffCnt = 0;
			if ((abs(qIAdOffSetA - AD15_OFFSET) <= Q_OFFSET_MAX)&&(abs(qIAdOffSetB - AD15_OFFSET) <= Q_OFFSET_MAX)
				&&(abs(qIAdOffSetC - AD15_OFFSET) <= Q_OFFSET_MAX))
			{
				wOffSetRet = ADC_OFFSET_OK | ADC_OFFSET_FLAG_END;
			}
			else wOffSetRet = ADC_OFFSET_NG | ADC_OFFSET_FLAG_END;
		}
		else wOffSetRet &= ~ADC_OFFSET_FLAG_END;
	}
	else wOffSetRet &= ~(ADC_OFFSET_FLAG_DO | ADC_OFFSET_FLAG_END);

	switch (xwOffSetSector)
	{
	case 1:		case 6:				//(MaxPhase == 'A') 
		xpIabcs->qB = MUL_SS(AD_NEGATIVE(qIAdcB - qIAdOffSetB), Q15_FRAC(ADMAX_I_IPM / QMAX_CURRENT)) >> 15;
		xpIabcs->qC = MUL_SS(AD_NEGATIVE(qIAdcC - qIAdOffSetC), Q15_FRAC(ADMAX_I_IPM / QMAX_CURRENT)) >> 15;
		xpIabcs->qA = -(xpIabcs->qB + xpIabcs->qC);
		break;
	case 2:		case 3:				//(MaxPhase == 'B') 
		xpIabcs->qC = MUL_SS(AD_NEGATIVE(qIAdcC - qIAdOffSetC), Q15_FRAC(ADMAX_I_IPM / QMAX_CURRENT)) >> 15;		
		xpIabcs->qA = MUL_SS(AD_NEGATIVE(qIAdcA - qIAdOffSetA), Q15_FRAC(ADMAX_I_IPM / QMAX_CURRENT)) >> 15;
		xpIabcs->qB = -(xpIabcs->qC + xpIabcs->qA);
		break;
	default:	// case 4:	case 5:	//(MaxPhase == 'C') 
		xpIabcs->qA = MUL_SS(AD_NEGATIVE(qIAdcA - qIAdOffSetA), Q15_FRAC(ADMAX_I_IPM / QMAX_CURRENT)) >> 15;
		xpIabcs->qB = MUL_SS(AD_NEGATIVE(qIAdcB - qIAdOffSetB), Q15_FRAC(ADMAX_I_IPM / QMAX_CURRENT)) >> 15;
		xpIabcs->qC = -(xpIabcs->qA + xpIabcs->qB);
		break;
	}

/* mabrouk edit*/
	*xpqVdc = MUL_SS((Q15)((ADC_GetConvertResult(TSB_ADA, ADC_REG3).All)>>1)/*AD15_REG_V_DC*/, Q15_FRAC(ADMAX_V_DC / QMAX_VOLT)) >> 15;

	return wOffSetRet;
}


/***************************************************************************************************
Name:        GenPWM   
Description: Set PWM Duty by compensating 3-phase voltage with DC BUS voltage. (=3�� ���� �� DC BUS ���� �����Ͽ� PWM Duty ����) 
***************************************************************************************************/

/* [120 degree discontinuous modulation, 3 SHUNT detection] */
void CalcPWM(TPwmDuty *xpPwmDuty, Q15 xqVmax, Q15 xqVmed, Q15 xqVmin, Q15 xqVdc)		
{
	xpPwmDuty->nMajorMax = DIV_UD(MUL_UU((TCLK_PWM_PERIOD >>1), xqVmax - xqVmin), xqVdc) + (TCLK_DEAD_COMPEN >>1);
	xpPwmDuty->nMajorMed = DIV_UD(MUL_UU((TCLK_PWM_PERIOD >>1), xqVmed - xqVmin), xqVdc);
	xpPwmDuty->nMajorMin = 0;
	if (xpPwmDuty->nMajorMax > (TCLK_PWM_PERIOD >>1))		// In-phase overmodulation limit (=�������� ������ ����) 
	{
		xpPwmDuty->nMajorMed = DIV_UD(MUL_UU(xpPwmDuty->nMajorMed, (TCLK_PWM_PERIOD >>1)), xpPwmDuty->nMajorMax);
		xpPwmDuty->nMajorMax = (TCLK_PWM_PERIOD >>1);
	}
	if (xpPwmDuty->nMajorMed > ((TCLK_PWM_PERIOD >>1) - 2 * TCLK_AD_CONV))	// Securing shunt current measurement time (=��Ʈ ���� �����ð� Ȯ��) 
	{
		xpPwmDuty->nMajorMed = ((TCLK_PWM_PERIOD >>1) - 2 * TCLK_AD_CONV);
	}
}
	


WORD GenPWM(TAxisABC *xpVabcs, Q15 xqVdc)
{
	TPwmDuty tPwmDuty;
	WORD twSector;

	if (xqVdc <= 0)
	{
		if (xqVdc == 0) { PWM_CON(ON); twSector = 0; }
		else { PWM_CON(OFF); twSector = 8; }
		PMD_SetAllPhaseCompareValue(PMD0,0, 0, 0);
		SAVE_PWM_REAR(0, 0, 0);
		tPwmDuty.nMajorMin = 0;
		tPwmDuty.nMajorMed = TCLK_SAMP_IREAD;
	}
	else 
	{
		PWM_CON(ON);
		if (xpVabcs->qA > xpVabcs->qB)
		{
			if (xpVabcs->qB > xpVabcs->qC)
			{
				twSector = 1;	// 'A';	// ABC
				SET_I_ORDER(oIabcs.qA, oIabcs.qB, oIabcs.qC); 
				CalcPWM(&tPwmDuty, xpVabcs->qA, xpVabcs->qB, xpVabcs->qC, xqVdc);
				PMD_SetAllPhaseCompareValue(PMD0,tPwmDuty.nMajorMax, tPwmDuty.nMajorMed, tPwmDuty.nMajorMin);
				SAVE_PWM_REAR(tPwmDuty.nRearMax, tPwmDuty.nRearMed, tPwmDuty.nRearMin);
			}
			else
			{
				if (xpVabcs->qA > xpVabcs->qC)
				{
					twSector = 6;	// 'A';	// ACB
					SET_I_ORDER(oIabcs.qA, oIabcs.qC, oIabcs.qB); 
					CalcPWM(&tPwmDuty, xpVabcs->qA, xpVabcs->qC, xpVabcs->qB, xqVdc);
                                        PMD_SetAllPhaseCompareValue(PMD0,tPwmDuty.nMajorMax, tPwmDuty.nMajorMin, tPwmDuty.nMajorMed);
					SAVE_PWM_REAR(tPwmDuty.nRearMax, tPwmDuty.nRearMin, tPwmDuty.nRearMed);
				}
				else 
				{
					twSector = 5;	// 'C';	// CAB
					SET_I_ORDER(oIabcs.qC, oIabcs.qA, oIabcs.qB); 
					CalcPWM(&tPwmDuty, xpVabcs->qC, xpVabcs->qA, xpVabcs->qB, xqVdc);
                                        PMD_SetAllPhaseCompareValue(PMD0,tPwmDuty.nMajorMed, tPwmDuty.nMajorMin, tPwmDuty.nMajorMax);
					SAVE_PWM_REAR(tPwmDuty.nRearMed, tPwmDuty.nRearMin, tPwmDuty.nRearMax);
				}
			}
		}
		else
		{
			if (xpVabcs->qA > xpVabcs->qC)
			{
				twSector = 2;	// 'B';	// BAC
				SET_I_ORDER(oIabcs.qB, oIabcs.qA,oIabcs.qC); 
				CalcPWM(&tPwmDuty, xpVabcs->qB, xpVabcs->qA, xpVabcs->qC, xqVdc);
                                PMD_SetAllPhaseCompareValue(PMD0,tPwmDuty.nMajorMed, tPwmDuty.nMajorMax, tPwmDuty.nMajorMin);
				SAVE_PWM_REAR(tPwmDuty.nRearMed, tPwmDuty.nRearMax, tPwmDuty.nRearMin);
			}
			else
			{
				if (xpVabcs->qB > xpVabcs->qC)
				{
					twSector = 3;	// 'B';	// BCA
					SET_I_ORDER(oIabcs.qB, oIabcs.qC, oIabcs.qA); 
					CalcPWM(&tPwmDuty, xpVabcs->qB, xpVabcs->qC, xpVabcs->qA, xqVdc);
                                        PMD_SetAllPhaseCompareValue(PMD0,tPwmDuty.nMajorMin, tPwmDuty.nMajorMax, tPwmDuty.nMajorMed);
					SAVE_PWM_REAR(tPwmDuty.nRearMin, tPwmDuty.nRearMax, tPwmDuty.nRearMed);
				}
				else 
				{
					twSector = 4;	// 'C';	// CBA
					SET_I_ORDER(oIabcs.qC, oIabcs.qB, oIabcs.qA); 
					CalcPWM(&tPwmDuty, xpVabcs->qC, xpVabcs->qB, xpVabcs->qA, xqVdc);
					PMD_SetAllPhaseCompareValue(PMD0,tPwmDuty.nMajorMin, tPwmDuty.nMajorMed, tPwmDuty.nMajorMax);
					SAVE_PWM_REAR(tPwmDuty.nRearMin, tPwmDuty.nRearMed, tPwmDuty.nRearMax);
				}
			}
		}
	}
	return twSector;
}

INTERRUPT ISR_MCPWM_REAR(void)
{
  
}


#undef _MC_UTIL_C_
/***************************************************************************************************
 *	[End Of Source File]
 **************************************************************************************************/
