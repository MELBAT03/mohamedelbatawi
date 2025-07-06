
#include "MC_interface.h"
#include "inverter.h"
#include "APP_measurement.h"
#include "MC_config.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "app_drain.h"
#include "app_motor.h"
#include "communication.h"
static run_paramter Vs_run_parameter;

extern RX_FRAME_information RECEVING_FRAME;
uint8_t drain_flag =APP_DRAIN_CLOSE;  //global var
uint8_t clutch_flag =HANDLER_CLUTCH_WITHOUT_BASKET;  //global var
uint8_t run_state_ready =0;
#define clutch_flag_done              55

uint32_t frame_accel =12000;
uint32_t frame_deccel =0;
int32_t frame_rpm =150;
uint32_t frame_time_on =0;
uint32_t frame_time_off =0;

void MC_Set_mode_parameter(run_paramter Vs_data)
{
  Vs_run_parameter.Vs_mode = Vs_data.Vs_mode;
  Vs_run_parameter.rpm     = Vs_data.rpm;
  Vs_run_parameter.accel   =Vs_data.accel;
  Vs_run_parameter.decel   =Vs_data.decel;
  Vs_run_parameter.time_off=Vs_data.time_off;
  Vs_run_parameter.time_on =Vs_data.time_on;
}



void MC_Get_mode_parameter(run_paramter *Vs_data)
{
  memmove((run_paramter*)Vs_data,(run_paramter *)&Vs_run_parameter,sizeof(run_paramter));
}

unsigned char MC_get_Water_level(void)
{
  return load_detection_level_get();
  
}



Fault_status_code MC_get_error(void)
{
  Fault_status_code fault_status=NO_FAULT;
  EMcFailEvent eMcFailEvent_local;
  eMcFailEvent_get(&eMcFailEvent_local);
  switch (eMcFailEvent_local)
  {
  case MTR_FAIL_NON:
    fault_status=NO_FAULT;
    break;
  case MTR_FAIL_STARTUP:
    fault_status=Motor_stall;
    break;
  case MTR_FAIL_OVERLOAD:
    fault_status=Lost_phase;
    break;
  case MTR_FAIL_OVERCURRENT:
    fault_status= over_current;
    break;
  case MTR_FAIL_IPMFAULT:
    fault_status= IPM_over_temperature;
    break;
  case MTR_FAIL_OVERVOLT:
    fault_status= Bus_over_voltage;
    break;
  case MTR_FAIL_UNDERVOLT:
    fault_status= Bus_under_voltage;
    break;
  case MTR_FAIL_HALLSENSOR:
  case MTR_FAIL_ALIGN:
  case MTR_FAIL_SPEED:
  case MTR_FAIL_ADCOFFSET:
    fault_status=General_fault;
    break;
  default :
    fault_status=NO_FAULT;
    break;
  }
  
  
  return fault_status;
}
unsigned int MC_get_actual_speed(void)
{
  return avg_speed_rpm_get();
}
unsigned int MC_get_DC_BUS(void)
{
  return avg_DC_bus_get();
}

static U16 vs_Time_dummy_software=0;
static U08 steps_dummy=2;
void dummy_test_software (uint32_t period)
{
//  if (run_state_ready ==clutch_flag_done)
  {
  switch (steps_dummy)
  {
  case 0:
    {
      vs_Time_dummy_software = 10;
      steps_dummy=1;
      break;
    }
      case 1:
    {
      run_paramter Vs_run_parameter;
      Vs_run_parameter.Vs_mode = WM_LOAD_SENSING;   //,   WM_Spin ,, WM_Washing
      Vs_run_parameter.rpm     = 0;
      Vs_run_parameter.accel   = 0;
      Vs_run_parameter.time_off= 0;
      Vs_run_parameter.time_on = 0;
      Vs_run_parameter.decel   = 0;
      MC_Set_mode_parameter(Vs_run_parameter);
//      steps_dummy = 5;
      if(vs_Time_dummy_software != 0)
      {
        --vs_Time_dummy_software;
      }
      if(vs_Time_dummy_software == 0)					 	
      {
        steps_dummy=2;
        vs_Time_dummy_software = 60;
      }
      break;
    }
  case 2:
    {
      
      run_paramter Vs_run_parameter;
      Vs_run_parameter.Vs_mode = WM_Spin;   //,   WM_Spin ,, WM_WashingWM_CONSTAT_SPEED
      Vs_run_parameter.rpm     = frame_rpm;
      Vs_run_parameter.accel   = SLOPE_TIME(ABS(frame_rpm),10,frame_accel);//(1000*(frame_rpm-SPEED_INIT))/frame_accel;
      Vs_run_parameter.time_off= (frame_time_off+frame_deccel)/MC_APP_WM_task_OS_time;
      Vs_run_parameter.time_on = (frame_time_on+frame_accel)/MC_APP_WM_task_OS_time;
      Vs_run_parameter.decel   = SLOPE_TIME(frame_rpm,0,frame_deccel);
      MC_Set_mode_parameter(Vs_run_parameter);
      steps_dummy = 5;
      if(vs_Time_dummy_software != 0)
      {
        --vs_Time_dummy_software;
      }
      if(vs_Time_dummy_software == 0)					 	
      {
        steps_dummy=2;
      }
      break;
    }
    
  case 3:
    
    //      run_paramter Vs_run_parameter;
    Vs_run_parameter.Vs_mode = WM_FREE_WHEEL;
    Vs_run_parameter.rpm     = 600;
    Vs_run_parameter.accel   =600;
    Vs_run_parameter.time_off=240;
    Vs_run_parameter.time_on =120;
    MC_Set_mode_parameter(Vs_run_parameter);
    break;
    
  }
  }
}
//
//void dummy_drain_test (uint32_t period)
//{
//  /* drain handle */
//  if(drain_flag == APP_DRAIN_OPEN)
//  {
////    app_drain_switch(APP_DRAIN_OPEN);
//    GPIO_WriteDataBit(GPIO_PORTG,GPIO_PIN0,0);
//  }
//  else
//  {
////    app_drain_switch(APP_DRAIN_CLOSE);
//        GPIO_WriteDataBit(GPIO_PORTG,GPIO_PIN0,1);
//
//  }
//}
//void dummy_clutch_test (uint32_t period)
//{
////  /* clutch handle */
//  static uint8_t clutch_counter=0;
////  
////  switch ()
////  {
////    
////  }
//  if(clutch_flag == HANDLER_CLUTCH_WITHOUT_BASKET)
//  {
//    run_state_ready=0;
////    motor_free_wheel();
//    if (MC_get_actual_speed() == 0)
//    {
//      if (clutch_counter++ == 3)
//      {
//        clutch_counter=0;
//        // chage the clutch state
//        if ( clutch_flag == app_motor_clutch_switch(HANDLER_CLUTCH_WITHOUT_BASKET))
//        {
//          clutch_flag = clutch_flag_done;
//        }       
//      }
//    }
//    else 
//    {
//      clutch_counter=0;
//    }
//             
//  }
//  else if(clutch_flag == HANDLER_CLUTCH_WITH_BASKET)
//  {
//    run_state_ready=0;
//  }
//  else if (clutch_flag == clutch_flag_done)
//  {
//    run_state_ready =clutch_flag_done;
//  }
//  else 
//  {
//  }
//}
//             