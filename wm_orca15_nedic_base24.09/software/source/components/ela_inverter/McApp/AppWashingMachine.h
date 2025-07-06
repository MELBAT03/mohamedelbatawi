/*========================================================*/
/*  		AppWashingMachine.c                           */
/*========================================================*/
#ifndef	_DEV_WASHING_MACHINE_H_
#define	_DEV_WASHING_MACHINE_H_

/*========================================================*/
/*			Include File     							  */
/*========================================================*/
#include "../McUtil/McUtil.h"
#include "../McDev/DevMotor.h"
#include "../core/tmpm370/inc/tmpm370_gpio.h"
#include "../app/config/def.h"

/*mabrouk edit portH data*/
#define	PORT_RELAY_POWER_read	GPIO_ReadDataBit(GPIO_PH,GPIO_BIT_3)
#define	PORT_RELAY_POWER_write(x)	GPIO_WriteDataBit(GPIO_PH,GPIO_BIT_3,x)
#define	PORT_RELAY_POWER	TSB_PH_DATA_PH3
//#define	PORT_RELAY_POWER	PORTH_bit.PH3

/*========================================================*/
/*		MOTOR DEFINE									  */
/*========================================================*/
#define DRUM_RPM_MAX				1800.0
#define DRUM_ACCEL_MAX				3515.625
#define CURRENT_MAX					40.0
#define VOLTAGE_MAX					480.0
#define POWER_MAX					3600

#define MOTOR_RUN					MTR_MODE_CLOSEDLOOP_HALL
#define MOTOR_STOP					MTR_MODE_STOP_BRAKE
#define MOTOR_OPEN					MTR_MODE_STOP_OPEN
#define MOTOR_SENSING				MTR_SENS_UNBALREAL
#define MTR_MODE_TEST_FREQUENCYVOLT	MTR_MODE_TEST_FREQUENCYVOLT

/*----- Parameter variable conversion macro (=�Ķ���� ���� ������ ��ũ��) ----------------------*/
#define MOTOR_RPM2SPD(xRpm)			(int)((xRpm) * 0x8000L / QMAX_OMEGA_RPM_DRUM) 		/* xRpm*182/10 */
//#define MOTOR_SPD2RPM(xRpm)			(int)((xRpm) * QMAX_OMEGA_RPM_DRUM/ 0x8000L) 		/* xRpm*10/182 */
#define CONVERT_SPDACCEL(xRpmPerS)	(int)((xRpmPerS) * 0x8000L / QMAX_SLOPE_RPM_DRUM) 	/* xRpmPerS*932/100 */
//#define CONVERT_CURRENT(xCurrent)	(int)((xCurrent) * 0x8000L / QMAX_CURRENT)
//#define CONVERT_VOLT(xVolt)			(int)((xVolt) * 0x8000L / QMAX_VOLT)
#define CONVERT_POWER(xPower)		(int)((xPower) * 0x8000L / QMAX_POWER)
#define RPM2VS_SPEED_SUB(xRpm2)		    (int)((xRpm2) * 182/10) 		// xRpm * 182 / 10 = actual speed (rpm)
#define RPMPERS2VS_ACCEL_SUB(xRpmPerS2)	(unsigned int)((xRpmPerS2)*932/100) //xRpmPerS * 932/100 = actual acceleration (rpm/sce)

#define motor_start(pass1, pass2, pass3)	do{DevSetMotorDrive(pass1, pass2, pass3); DevSetMotorSensing(MTR_SENS_UNBALREAL, 0);}while(0)
#define ipm_fault_disable_time(pass1)		do{DevSetMotorIPMFaultDisableTime(pass1);}while(0)

// #define vs_Motor_mode				(eMcCtrlMode)
#define vs_Temp_heatsink

/*========================================================*/
/*		DEFINE									  		  */
/*========================================================*/
#define ON     	1
#define OFF    	0

#define HIGH	1	
#define LOW		0

#define OPEN	1	
#define CLOSE	0

//#define TRUE	1
//#define FALSE	0

/*========================================================*/
/*		TYPE DECLARATION								  */
/*========================================================*/
 typedef struct {
	unsigned char b0:1;
	unsigned char b1:1;
	unsigned char b2:1;
	unsigned char b3:1;
	unsigned char b4:1;
	unsigned char b5:1;
	unsigned char b6:1;
	unsigned char b7:1;
} BYTE_FIELD;

typedef struct {
	unsigned char b0_1:2;
	unsigned char b2_3:2;
	unsigned char b4_5:2;
	unsigned char b6_7:2;
} BYTE_FIELD2;

typedef struct {
	unsigned char b0_2:3;
	unsigned char b3_5:3;
	unsigned char b6_7:2;
} BYTE_FIELD3;

typedef struct {
	unsigned char b0_3:4;
	unsigned char b4_7:4;
} BYTE_FIELD4;

typedef struct {
	unsigned int b0:1;
	unsigned int b1:1;
	unsigned int b2:1;
	unsigned int b3:1;
	unsigned int b4:1;
	unsigned int b5:1;
	unsigned int b6:1;
	unsigned int b7:1;
	unsigned int b8:1;
	unsigned int b9:1;
	unsigned int b10:1;
	unsigned int b11:1;
	unsigned int b12:1;
	unsigned int b13:1;
	unsigned int b14:1;
	unsigned int b15:1;
} WORD_FIELD;

typedef struct {
	unsigned int b0_1:2;
	unsigned int b2_3:2;
	unsigned int b4_5:2;
	unsigned int b6_7:2;
	unsigned int b8_9:2;
	unsigned int b10_11:2;
	unsigned int b12_13:2;
	unsigned int b14_15:2;
} WORD_FIELD2;

/*-------  BYTE IO ---------------------------------------*/
typedef union {
	unsigned char 	byte;
	BYTE_FIELD 		bit; 
	BYTE_FIELD2 	bit2;
	BYTE_FIELD3 	bit3;
	BYTE_FIELD4 	bit4;
} TYPE_BYTE;

/*------- WORD IO ----------------------------------------*/
typedef union {
	unsigned int	word;
	unsigned char	byte[2];
	WORD_FIELD 		bit;
	WORD_FIELD2		bit2;
} TYPE_WORD;

/*------ DOUBLE IO ---------------------------------------*/
typedef union {
	unsigned long 	wword;
	unsigned int	word[2];
	unsigned char	byte[4];
} TYPE_DOUBLE;

typedef union {
	unsigned int	word[3];
	unsigned char	byte[6];
} TYPE_3WORD;

/*--------------------------------------------------------*/
typedef	struct tag_motor_type{
	unsigned int	mode;
	signed int		speed;
	signed int		accel;
	unsigned int	time;
}motor_type;


/***************************************************************************************************
 *	[End Of Header File]
 **************************************************************************************************/
#endif
