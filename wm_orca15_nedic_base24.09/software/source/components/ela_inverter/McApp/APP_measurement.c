/******************************************************************************* * 
 * Header: app_measurment  
 *  Author: mohamed ibrahim elbatawi
 * Description: measuring (speed - tourque - unbalance - power - voltage ) 
 *  mohaemd elbatawi  4/5/2025
 * *******************************************************************************/
#include "MC_config.h"
#include "APP_measurement.h"
#include "../McDev/DevMotor.h"
#include "utils.h"
typedef struct {
  unsigned int	vs_Torque_avg;
  unsigned int	vs_Unbal_avg;
  unsigned int	vs_Power_avg;
  unsigned int	vs_Rpm_avg;
  unsigned int	vs_Voltage_avg; 
}APP_measurment_t;
APP_measurment_t APP_measurment={0};

static unsigned int	vs_Cnt_torque; 								/* Qaxis current (=Q?? ????) */
static unsigned int	vs_Torque_buf[32];
//unsigned int	vs_Torque_avg;

static unsigned int	vs_Cnt_unbal; 
static unsigned int	vs_Unbal_buf[32];
//unsigned int	vs_Unbal_avg;
 
static unsigned int	vs_Cnt_power; 
static unsigned int	vs_Power_buf[32];
//unsigned int	vs_Power_avg;

static unsigned int	vs_Cnt_rpm;
static unsigned int	vs_Rpm_buf[16];
//unsigned int	vs_Rpm_avg;

static unsigned int	vs_Cnt_voltage;
static unsigned int	vs_Voltage_buf[10];
//unsigned int	vs_Voltage_avg; 

static U08 Tourque_unbalance_Vdc_flg=0;
static U08 RPM_POWER_flg=0;

static void average_torque_measurment(void);
static void average_unbalance_measurment(void);
static void average_power_measurment(void);
static void average_rpm_measurment(void);
static void average_voltage_measurment(void);

void APP_measurment_init(void)
{
}
void APP_measurment_Update(uint32_t period)
{
  static uint8_t intervalTimeoutCnt_unbalance_Vdc_ID = 0;
  intervalTimeoutCnt_unbalance_Vdc_ID+=period; 
  
   static uint8_t intervalTimeoutCnt__RPM_POWER_ID = 0;
  intervalTimeoutCnt__RPM_POWER_ID+=period; 
  if (intervalTimeoutCnt_unbalance_Vdc_ID >= MC_OS_TOURQUE_UNBALANCE_VDC_ID)
  {
    Tourque_unbalance_Vdc_flg =1;
    intervalTimeoutCnt_unbalance_Vdc_ID=0;
  }
    if (intervalTimeoutCnt__RPM_POWER_ID >= MC_OS_RPM_POWER_ID)
  {
    RPM_POWER_flg =1;
    intervalTimeoutCnt__RPM_POWER_ID=0;
  }
  
  average_torque_measurment();
  average_unbalance_measurment();
  average_power_measurment();
  average_rpm_measurment();
  average_voltage_measurment();
  Tourque_unbalance_Vdc_flg =0;
  RPM_POWER_flg=0;
}




/*--------------------------------------------------------*/
static void average_torque_measurment(void)
{
	unsigned char	i;
	unsigned long 	j;
	TAxisDQ oIdqse_local;
	if(Tourque_unbalance_Vdc_flg)  // 10 mile sec
	{
		++vs_Cnt_torque;
		if(vs_Cnt_torque >= 32)
		{
			vs_Cnt_torque = 0;
		}
		oIdqse_get(&oIdqse_local);
		Q15 vs_Torque_ampere = 	oIdqse_local.qQ;
		vs_Torque_buf[vs_Cnt_torque] = abs(vs_Torque_ampere);
		
		for(i=0,j=0 ; i<32 ; i++)
		{
			j = j + vs_Torque_buf[i];
		}
		APP_measurment.vs_Torque_avg = (unsigned int)((j+16) >> 5);
	}
}

/*-----	Finding the average value of instantaneous unbalance (=???? ????? ???? ?????) ----------------------*/
static void average_unbalance_measurment(void)
{
	unsigned char 	i;
	unsigned long 	j;

	if(Tourque_unbalance_Vdc_flg == 1)  // 10 milesec
	{
		++vs_Cnt_unbal;
		if(vs_Cnt_unbal >= 32)
		{
			vs_Cnt_unbal = 0;
		}	
		vs_Unbal_buf[vs_Cnt_unbal] = abs(snMtrSensUnbalReal_get());
		
		for(i=0,j=0 ; i<32 ; i++)
		{
			j = j + vs_Unbal_buf[i];
		}
		APP_measurment.vs_Unbal_avg = (unsigned int)((j+16) >> 5);
	}	
}

/*-----	Finding the POWER average value (=POWER ???? ?????) ------------------------------*/
static void average_power_measurment(void)
{
	unsigned char 	i;
	unsigned long 	j;

	if(RPM_POWER_flg == 1)   // 100 milsec
	{
		++vs_Cnt_power;
		if(vs_Cnt_power >= 32)
		{
			vs_Cnt_power = 0;
		}	
		vs_Power_buf[vs_Cnt_power] = abs(qPowerIn_get());
		
		for(i=0,j=0 ; i<32 ; i++)
		{
			j = j + vs_Power_buf[i];
		}
		APP_measurment.vs_Power_avg = (unsigned int)((j+16) >> 5);
	}	
}

/*-----	Finding the RPM average value (=RPM ???? ?????) --------------------------------*/
static void average_rpm_measurment(void)
{
	unsigned char 	i;
	unsigned long	j;

	if(RPM_POWER_flg == 1)          //100 milsec
	{
		++vs_Cnt_rpm;
		if(vs_Cnt_rpm >= 16)
		{
			vs_Cnt_rpm = 0;
		}	
		vs_Rpm_buf[vs_Cnt_rpm] = abs(qWrpmActual_get());
		
		for(i=0,j=0 ; i<16 ; i++)
		{
			j = j + vs_Rpm_buf[i];
		}
		APP_measurment.vs_Rpm_avg = (unsigned int)((j+8) >> 4);
                
               APP_measurment.vs_Rpm_avg =  MOTOR_SPD2RPM(APP_measurment.vs_Rpm_avg);
	}	
}

/*-----	Finding the DC Voltage average value (=DC VOLTAGE ???? ?????) -------------------------*/
static void average_voltage_measurment(void)
{
	unsigned char	i;
	unsigned long	j;

	if(Tourque_unbalance_Vdc_flg == 1)   // 10 milesec
	{
		++vs_Cnt_voltage;
		if(vs_Cnt_voltage >= 10)
		{
			vs_Cnt_voltage = 0;
		}	
		vs_Voltage_buf[vs_Cnt_voltage] = abs(qVdcBus_get());
		
		for(i=0,j=0 ; i<10 ; i++)
		{
			j = j + vs_Voltage_buf[i];
		}
		j = (j+5)/10;
		
		j = j*480L;
		APP_measurment.vs_Voltage_avg = (unsigned int)(j >> 15);
					
	}	
}

unsigned int avg_speed_rpm_get(void)
{
  return APP_measurment.vs_Rpm_avg;
}

unsigned int avg_DC_bus_get(void)
{
  return APP_measurment.vs_Voltage_avg;
}