
#ifndef	_MC_interface_H_
#define	_MC_interface_H_
#include "def.h"
//#include "McUtil.h"

#define SPEED_INIT                     100              //RPM
//#define SPEED_DEF(RPM)                  ((RPM - SPEED_INIT)*(1L <<SCALE_PIL1_KPIT)*(1L<<SCALE_SLOPE_INTEG)*(1000)*(TS_CC) / (QMAX_OMEGA_RPM_DRUM))  
//#define STEP_SLOPE(accel_time_ms)          ((accel_time_ms)*(1L <<SCALE_PIL1_KPIT)*(1L<<SCALE_SLOPE_INTEG) / ((QMAX_OMEGA_RPM_DRUM)/(TS_CC)))
//#define  ACCEL_TIME(rpm,accel_time)            ((SPEED_DEF(rpm))/(STEP_SLOPE(accel_time)))
#define SLOPE_TIME(frame_rpm,speed_init,frame_accel_decel)          ((1000*(frame_rpm-speed_init))/frame_accel_decel)
typedef enum
{
  NO_FAULT=0x00,
  General_fault = 0x01,
  IPM_over_temperature = 0x02,
  Bus_over_voltage = 0x04,
  Motor_stall = 0x08,
  over_current = 0x10,
  Bus_under_voltage =0x20,
  Lost_phase =0x40,
  Motor_over_temperature=0x80,
}Fault_status_code;

typedef enum
 {
	 WM_IDEAL_STAT,
	 WM_OPEN_LOOP,
	 WM_Washing ,
	 WM_BREAK,
	 WM_Spin,
	 WM_FREE_WHEEL,
	 WM_LOAD_SENSING,
	 WM_Clutching_declutching,
	 WM_CONSTAT_SPEED,
	 WM_TOTAL_MODE
 }wm_mode_typedef;

typedef struct {
  wm_mode_typedef Vs_mode;
  unsigned int time_on;
  unsigned int time_off;
  int rpm;
  unsigned int accel;
  unsigned int decel;
}run_paramter;

void MC_Set_mode_parameter(run_paramter Vs_data);
void MC_Get_mode_parameter(run_paramter *Vs_data);
unsigned char MC_get_Water_level(void);
Fault_status_code MC_get_error(void);
unsigned int MC_get_actual_speed(void);
unsigned int MC_get_DC_BUS(void);


void dummy_test_software (uint32_t period);
void dummy_drain_test (uint32_t period);
void dummy_clutch_test (uint32_t period);
#endif