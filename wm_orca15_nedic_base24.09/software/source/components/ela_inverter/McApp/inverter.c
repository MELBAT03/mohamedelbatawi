
/******************************************************************************* * 
* Header: inverter 
*  Author: mohamed ibrahim elbatawi
* Description: configer the movement of the WM inverter 
*  mohaemd elbatawi  4/5/2025
* *******************************************************************************/
#include "MC_config.h"
#include "inverter.h"
#include "../McDev/DevMotor.h"
#include "AppWashingMachine.h"
#include "MC_interface.h"
#include "utils.h"

static unsigned int 	vs_Time_motor;
static TYPE_BYTE		_vs_Cnt_motor_wash;
#define	vs_Cnt_motor_wash	_vs_Cnt_motor_wash.byte
#define	FG_MOTOR_ON_OFF		_vs_Cnt_motor_wash.bit.b0
#define	FG_MOTOR_CW_CCW		_vs_Cnt_motor_wash.bit.b1
uint32_t spin_counts =0;

static unsigned int	vs_Cnt_load_data;
static unsigned int	vs_Load_data_buf[20];
static unsigned int	vs_Load_data;
static unsigned char 	load_detection_level;

#if defined (MC_SPIN_INTERNAL_ALG)
static unsigned char 	vs_Cnt_spin;
static unsigned int  	vs_Time_spin_step;
static unsigned char 	vs_Spin= 5;
static EMcCtrlMode	vs_Motor_mode_sub;
static signed int		vs_Speed_sub;
static signed int		vs_Accel_sub;

/*........................................................*/
const motor_type TABLE_RINSE_SPIN_NORMAL[] =
{MOTOR_RUN,  MOTOR_RPM2SPD(45),   CONVERT_SPDACCEL(6),	 8,	/* 4min intermediate spin (=4?? ???Z??) */		
MOTOR_RUN,  MOTOR_RPM2SPD(100),  CONVERT_SPDACCEL(5),	30,                   
MOTOR_RUN,  MOTOR_RPM2SPD(250),  CONVERT_SPDACCEL(8),	30,                 
MOTOR_RUN,  MOTOR_RPM2SPD(300),  CONVERT_SPDACCEL(2),   25,                   
MOTOR_RUN,  MOTOR_RPM2SPD(400),  CONVERT_SPDACCEL(3),   34,                  
MOTOR_RUN,  MOTOR_RPM2SPD(500),  CONVERT_SPDACCEL(3),	34, 
MOTOR_RUN,  MOTOR_RPM2SPD(700),  CONVERT_SPDACCEL(3),	79,                 
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,            
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0};

const motor_type TABLE_RINSE_SPIN_LOW[] =   
{MOTOR_RUN,  MOTOR_RPM2SPD(45),   CONVERT_SPDACCEL(6),	 8,	/* 4min intermediate spin (=4?? ???Z??) */		
MOTOR_RUN,  MOTOR_RPM2SPD(100),  CONVERT_SPDACCEL(5),	30,                   
MOTOR_RUN,  MOTOR_RPM2SPD(250),  CONVERT_SPDACCEL(8),	30,                  
MOTOR_RUN,  MOTOR_RPM2SPD(300),  CONVERT_SPDACCEL(2),   60,                   
MOTOR_RUN,  MOTOR_RPM2SPD(400),  CONVERT_SPDACCEL(2),  112,                  
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,                  
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,                   
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,                   
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,                   
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0};

/*........................................................*/						 		    	
const motor_type TABLE_SPIN_NORMAL1[] ={MOTOR_RUN,  MOTOR_RPM2SPD(45),   CONVERT_SPDACCEL(6),	 8,	/* Spin 1min */		
MOTOR_RUN,  MOTOR_RPM2SPD(100),  CONVERT_SPDACCEL(5),	30,                   
MOTOR_RUN,  MOTOR_RPM2SPD(250),  CONVERT_SPDACCEL(8),	30,                 
MOTOR_RUN,  MOTOR_RPM2SPD(300),  CONVERT_SPDACCEL(2),   25,                   
MOTOR_RUN,  MOTOR_RPM2SPD(400),  CONVERT_SPDACCEL(3),   34,                  
MOTOR_RUN,  MOTOR_RPM2SPD(500),  CONVERT_SPDACCEL(3),	34, 
MOTOR_RUN,  MOTOR_RPM2SPD(600),  CONVERT_SPDACCEL(3),	79,                 
MOTOR_RUN,  MOTOR_RPM2SPD(700),  CONVERT_SPDACCEL(3),   60,			            
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0};

const motor_type TABLE_SPIN_NORMAL5[] ={/*MOTOR_RUN,  MOTOR_RPM2SPD(45),   CONVERT_SPDACCEL(6),	 8,*/	/* Spin 5min */				
  MOTOR_RUN,  MOTOR_RPM2SPD(100),  CONVERT_SPDACCEL(5),	30,                   
  MOTOR_RUN,  MOTOR_RPM2SPD(250),  CONVERT_SPDACCEL(8),	30,                 
  MOTOR_RUN,  MOTOR_RPM2SPD(300),  CONVERT_SPDACCEL(2),   25,                   
  MOTOR_RUN,  MOTOR_RPM2SPD(400),  CONVERT_SPDACCEL(3),   34,                  
  MOTOR_RUN,  MOTOR_RPM2SPD(500),  CONVERT_SPDACCEL(3),	34, 
  MOTOR_RUN,  MOTOR_RPM2SPD(600),  CONVERT_SPDACCEL(3),	79,                 
  MOTOR_RUN,  MOTOR_RPM2SPD(700),  CONVERT_SPDACCEL(3),  300,			            
  MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	0,
  MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	0};

const motor_type TABLE_SPIN_NORMAL9[] ={MOTOR_RUN,  MOTOR_RPM2SPD(45),   CONVERT_SPDACCEL(6),	 8,	/* Spin 1min */			
MOTOR_RUN,  MOTOR_RPM2SPD(100),  CONVERT_SPDACCEL(5),	30,                   
MOTOR_RUN,  MOTOR_RPM2SPD(250),  CONVERT_SPDACCEL(8),	30,                 
MOTOR_RUN,  MOTOR_RPM2SPD(300),  CONVERT_SPDACCEL(2),   25,                   
MOTOR_RUN,  MOTOR_RPM2SPD(400),  CONVERT_SPDACCEL(3),   34,                  
MOTOR_RUN,  MOTOR_RPM2SPD(500),  CONVERT_SPDACCEL(3),	34, 
MOTOR_RUN,  MOTOR_RPM2SPD(600),  CONVERT_SPDACCEL(3),	79,                 
MOTOR_RUN,  MOTOR_RPM2SPD(700),  CONVERT_SPDACCEL(3),  540,	 		            
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0};						 		    							 		    	

const motor_type TABLE_SPIN_LOW1[] =   {MOTOR_RUN,  MOTOR_RPM2SPD(45),   CONVERT_SPDACCEL(6),	 8,	/* Spin 1min */				
MOTOR_RUN,  MOTOR_RPM2SPD(100),  CONVERT_SPDACCEL(5),	30,                   
MOTOR_RUN,  MOTOR_RPM2SPD(250),  CONVERT_SPDACCEL(8),	30,                  
MOTOR_RUN,  MOTOR_RPM2SPD(300),  CONVERT_SPDACCEL(2),   60,                   
MOTOR_RUN,  MOTOR_RPM2SPD(400),  CONVERT_SPDACCEL(2),  112,                  
MOTOR_RUN,  MOTOR_RPM2SPD(400),  CONVERT_SPDACCEL(2),   60, 	             
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,                   
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,                   
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,                   
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0};

const motor_type TABLE_SPIN_LOW5[] =   {MOTOR_RUN,  MOTOR_RPM2SPD(45),   CONVERT_SPDACCEL(6),	 8,	/* Spin 5min */				
MOTOR_RUN,  MOTOR_RPM2SPD(100),  CONVERT_SPDACCEL(5),	30,                   
MOTOR_RUN,  MOTOR_RPM2SPD(250),  CONVERT_SPDACCEL(8),	30,                  
MOTOR_RUN,  MOTOR_RPM2SPD(300),  CONVERT_SPDACCEL(2),   60,                   
MOTOR_RUN,  MOTOR_RPM2SPD(400),  CONVERT_SPDACCEL(2),  112,                  
MOTOR_RUN,  MOTOR_RPM2SPD(400),  CONVERT_SPDACCEL(2),  300, 	                 
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,                   
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,                   
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,                   
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0};

const motor_type TABLE_SPIN_LOW9[] =   {MOTOR_RUN,  MOTOR_RPM2SPD(45),   CONVERT_SPDACCEL(6),	 8,	/* Spin 9min */				
MOTOR_RUN,  MOTOR_RPM2SPD(100),  CONVERT_SPDACCEL(5),	30,                   
MOTOR_RUN,  MOTOR_RPM2SPD(250),  CONVERT_SPDACCEL(8),	30,                  
MOTOR_RUN,  MOTOR_RPM2SPD(300),  CONVERT_SPDACCEL(2),   60,                   
MOTOR_RUN,  MOTOR_RPM2SPD(400),  CONVERT_SPDACCEL(2),  112,                  
MOTOR_RUN,  MOTOR_RPM2SPD(400),  CONVERT_SPDACCEL(2),  540, 	                 
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,                   
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,                   
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0,                   
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0};

/*........................................................*/						 		    	
const motor_type TABLE_SUPER_SPIN[] =  {MOTOR_RUN,  MOTOR_RPM2SPD(45),   CONVERT_SPDACCEL(6),	8,	
MOTOR_RUN,  MOTOR_RPM2SPD(100),  CONVERT_SPDACCEL(5),	30,                    
MOTOR_RUN,  MOTOR_RPM2SPD(250),  CONVERT_SPDACCEL(8),	30,                  
MOTOR_RUN,  MOTOR_RPM2SPD(300),  CONVERT_SPDACCEL(2),   60,                   
MOTOR_RUN,  MOTOR_RPM2SPD(400),  CONVERT_SPDACCEL(2),   60,                  
MOTOR_RUN,  MOTOR_RPM2SPD(500),  CONVERT_SPDACCEL(2),	60,  
MOTOR_RUN,  MOTOR_RPM2SPD(600),  CONVERT_SPDACCEL(2),	60,                  
MOTOR_RUN,  MOTOR_RPM2SPD(600),  CONVERT_SPDACCEL(2),	90,
MOTOR_RUN,  MOTOR_RPM2SPD(700),  CONVERT_SPDACCEL(2), 7200,	
MOTOR_STOP, MOTOR_RPM2SPD(0),    CONVERT_SPDACCEL(0),	 0};						 		    								 		    								 		    							 		    	



static void motor_spin(uint32_t period);
#else 
static void motor_spin(uint32_t period);

#endif

static void motor_break(void);
static void motor_free_wheel(void);
static void motor_washing_mode(unsigned int time_on, unsigned int time_off, unsigned int rpm, unsigned int accel, unsigned int decel, uint32_t period_ms);
static uint8_t motor_load_sensing_mode(uint32_t period);
static void load_detection_result(void);



static run_paramter Vs_run_parameter_inverter;
static APP_load_detection_states load_state=start_load_detection_CW;

void inverter_Init(void)
{
}
void inverter_fail_Update(uint32_t period)
{
  DevMainLoopMotor();
}

//static uint32_t constant_sspeed_counter=0;
//static bool_t one_shoot_constant_speed=FALSE;
void WM_Update(uint32_t period)
{
 // static wm_mode_typedef prev_mode ;
 // prev_mode = Vs_run_parameter_inverter.Vs_mode;
  MC_Get_mode_parameter(&Vs_run_parameter_inverter);
  switch (Vs_run_parameter_inverter.Vs_mode)
  {
  case WM_IDEAL_STAT:
    {
      //one_shoot_constant_speed=FALSE;
      
      break;
    }
  case WM_Washing:
    {
      motor_washing_mode(Vs_run_parameter_inverter.time_on
                         ,Vs_run_parameter_inverter.time_off
                           ,ABS(Vs_run_parameter_inverter.rpm)
                             ,Vs_run_parameter_inverter.accel
                               ,Vs_run_parameter_inverter.decel
                                 ,period);
    }
    break;
    
  case WM_BREAK:
    {
      motor_break();
      run_paramter Vs_run_parameter;
      Vs_run_parameter.Vs_mode = WM_IDEAL_STAT;
      Vs_run_parameter.rpm     =0;
      Vs_run_parameter.accel   =0;
      Vs_run_parameter.time_off=0;
      Vs_run_parameter.time_on =0;
      MC_Set_mode_parameter(Vs_run_parameter);
    }
    break;
    
  case WM_Spin:
    {
      motor_spin(period);
    }
    break;
    
  case WM_FREE_WHEEL:
    {
      motor_free_wheel();
      run_paramter Vs_run_parameter;
      Vs_run_parameter.Vs_mode = WM_IDEAL_STAT;
      Vs_run_parameter.rpm     =0;
      Vs_run_parameter.accel   =0;
      Vs_run_parameter.time_off=0;
      Vs_run_parameter.time_on =0;
      MC_Set_mode_parameter(Vs_run_parameter);
      
    }
    break;
  case WM_CONSTAT_SPEED:
    {
//      if (one_shoot_constant_speed == FALSE){
//        //              constant_sspeed_counter++;
//        one_shoot_constant_speed=TRUE;
        //      motor_start(MOTOR_RUN, MOTOR_RPM2SPD(-45.0), -1);	/* Motor CW */
        motor_start(MOTOR_RUN, MOTOR_RPM2SPD(Vs_run_parameter_inverter.rpm), CONVERT_SPDACCEL(Vs_run_parameter_inverter.accel));
//        motor_start(MOTOR_RUN, MOTOR_RPM2SPD(90), CONVERT_SPDACCEL(Vs_run_parameter_inverter.accel));

        //      motor_start(MOTOR_RUN, -MOTOR_RPM2SPD(40.0), -1);
        //      motor_start(MOTOR_RUN, -MOTOR_RPM2SPD(40.0), CONVERT_SPDACCEL(40.0));
//      }
      //            motor_washing_mode(13000
      //                         ,13000
      //                           ,ABS(100)
      //                             ,500
      //                               ,500
      //                                ,period);
    }
    break;
  case WM_LOAD_SENSING:
    {
      //      if(prev_mode != WM_LOAD_SENSING)
      //      {
      //        load_state = start_load_detection_CW;
      //        
      //      }
      
      if( motor_load_sensing_mode(period))
      {
        
        run_paramter Vs_run_parameter;
        Vs_run_parameter.Vs_mode = WM_FREE_WHEEL;
        Vs_run_parameter.rpm     =0;
        Vs_run_parameter.accel   =0;
        Vs_run_parameter.time_off=0;
        Vs_run_parameter.time_on =0;
        MC_Set_mode_parameter(Vs_run_parameter);
      }
      
    }
    break;
    
    
    
  default:
    {
    }
    break; 
    
    
  }
  
}
void motor_washing_mode(unsigned int time_on, unsigned int time_off, unsigned int rpm, unsigned int accel, unsigned int decel, uint32_t period_ms)
{
  unsigned long 	i;
  signed int		vc_Speed_temp;
  signed int		vc_Accel_temp;
  
  if(vs_Time_motor != 0)
  {
    --vs_Time_motor;
  }
  
  /*....................................................*/
  i = ((unsigned long)rpm*182)/10;                        /* When you release the macro, it is as follows. (=??u??? ??? ????? ????) */
  vc_Speed_temp = (signed int)i;
  
  i = ((unsigned long)accel*932)/100;
  vc_Accel_temp = (signed int)i;
  
  /*....................................................*/
  if(vs_Time_motor == 0)
  {
    if(FG_MOTOR_ON_OFF == 0)							
    {
      vs_Time_motor = time_on;  						/* Motor stop->opeartion (=???? ????->????) */
      
      if(FG_MOTOR_CW_CCW == 0)
      {
        motor_start(MOTOR_RUN, -MOTOR_RPM2SPD(100.0), -1);  	/* Motor CCW */
        motor_start(MOTOR_RUN, -vc_Speed_temp, vc_Accel_temp);
      }
      else
      {
        //        vs_Time_motor = time_on;				
        
        motor_start(MOTOR_RUN, MOTOR_RPM2SPD(100.0), -1);	/* Motor CW */
        motor_start(MOTOR_RUN, vc_Speed_temp, vc_Accel_temp);
      }
    }
    else
    {
      vs_Time_motor = time_off;						/* Motor stop */
      //motor_start(MOTOR_OPEN,0,0);
      //motor_open();									/* No change in DC-LINK voltage at all (=DC-LINK???? ???? ???????) */
      //      motor_start(MOTOR_STOP,0,0);
      //      motor_start(MOTOR_RUN, MOTOR_RPM2SPD(0), CONVERT_SPDACCEL(1400));	/* High pressure error occurred(wash->only water load, spin all) (=??????????(wash->only water load, spin all)) */
      //motor_start(MOTOR_RUN, MOTOR_RPM2SPD(0), CONVERT_SPDACCEL(1120));	/* 0.8decel */
      //      motor_start(MOTOR_RUN, MOTOR_RPM2SPD(0), CONVERT_SPDACCEL(700));	/* 0.5decel */
      //       motor_start(MOTOR_RUN, MOTOR_RPM2SPD(0), CONVERT_SPDACCEL(vc_Accel_temp));	/* 0.5decel */
      motor_start(MOTOR_RUN, MOTOR_RPM2SPD(0), CONVERT_SPDACCEL(decel));	/* vc_Accel_temp */
      
    }
    ++vs_Cnt_motor_wash;
  }
}
#if defined (MC_SPIN_INTERNAL_ALG)
void motor_spin(uint32_t period)
{
  static uint32_t intervalTimeoutCnt_spin_id = 0;
  intervalTimeoutCnt_spin_id+=period;
  if((intervalTimeoutCnt_spin_id) >= MC_OS_MOTOR_SPIN_ID)
  {
    intervalTimeoutCnt_spin_id=0;
    if(vs_Time_spin_step != 0)
    {
      --vs_Time_spin_step;
    }
  }
  if(vs_Time_spin_step == 0)					 	
  {
    
    switch(vs_Spin)
    {
    case 1:
      vs_Motor_mode_sub =(EMcCtrlMode) TABLE_SPIN_LOW1[vs_Cnt_spin].mode;
      vs_Speed_sub = 		TABLE_SPIN_LOW1[vs_Cnt_spin].speed;
      vs_Accel_sub = 		TABLE_SPIN_LOW1[vs_Cnt_spin].accel;
      vs_Time_spin_step =	TABLE_SPIN_LOW1[vs_Cnt_spin].time;
      break;
    case 2: vs_Motor_mode_sub = (EMcCtrlMode) TABLE_SPIN_LOW5[vs_Cnt_spin].mode;
    vs_Speed_sub = 		TABLE_SPIN_LOW5[vs_Cnt_spin].speed;
    vs_Accel_sub = 		TABLE_SPIN_LOW5[vs_Cnt_spin].accel;
    vs_Time_spin_step =	TABLE_SPIN_LOW5[vs_Cnt_spin].time;
    break;
    case 3: vs_Motor_mode_sub = (EMcCtrlMode) TABLE_SPIN_LOW9[vs_Cnt_spin].mode;
    vs_Speed_sub = 		TABLE_SPIN_LOW9[vs_Cnt_spin].speed;
    vs_Accel_sub = 		TABLE_SPIN_LOW9[vs_Cnt_spin].accel;
    vs_Time_spin_step =	TABLE_SPIN_LOW9[vs_Cnt_spin].time;
    break;
    
    case 4: vs_Motor_mode_sub =(EMcCtrlMode)  TABLE_SPIN_NORMAL1[vs_Cnt_spin].mode;
    vs_Speed_sub = 		TABLE_SPIN_NORMAL1[vs_Cnt_spin].speed;
    vs_Accel_sub = 		TABLE_SPIN_NORMAL1[vs_Cnt_spin].accel;
    vs_Time_spin_step =	TABLE_SPIN_NORMAL1[vs_Cnt_spin].time;
    break;
    case 5: vs_Motor_mode_sub = (EMcCtrlMode) TABLE_SPIN_NORMAL5[vs_Cnt_spin].mode;
    vs_Speed_sub = 		TABLE_SPIN_NORMAL5[vs_Cnt_spin].speed;
    vs_Accel_sub = 		TABLE_SPIN_NORMAL5[vs_Cnt_spin].accel;
    vs_Time_spin_step =	TABLE_SPIN_NORMAL5[vs_Cnt_spin].time;
    break;
    case 6: vs_Motor_mode_sub = (EMcCtrlMode) TABLE_SPIN_NORMAL9[vs_Cnt_spin].mode;
    vs_Speed_sub = 		TABLE_SPIN_NORMAL9[vs_Cnt_spin].speed;
    vs_Accel_sub = 		TABLE_SPIN_NORMAL9[vs_Cnt_spin].accel;
    vs_Time_spin_step =	TABLE_SPIN_NORMAL9[vs_Cnt_spin].time;
    break;
    }
    motor_start(vs_Motor_mode_sub, -vs_Speed_sub, vs_Accel_sub);
    ++vs_Cnt_spin;
  }
  
  if (vs_Cnt_spin >= 10)
  {
    Vs_run_parameter_inverter.Vs_mode=WM_FREE_WHEEL;
  }
  
}
#else
static void motor_spin(uint32_t period)
{
  //  if (spin_counts> 30 ){
  //
  motor_start(MOTOR_RUN, MOTOR_RPM2SPD(Vs_run_parameter_inverter.rpm), CONVERT_SPDACCEL(Vs_run_parameter_inverter.accel));
  //   spin_counts++;
  //  }
  //  else
  //  {
  //    motor_start(MOTOR_RUN, -MOTOR_RPM2SPD(100.0), -1);
  ////        motor_start(MOTOR_RUN, MOTOR_RPM2SPD(Vs_run_parameter_inverter.rpm), CONVERT_SPDACCEL(Vs_run_parameter_inverter.accel));
  //           spin_counts++;
  //
  //  }
  
  
}
#endif

uint8_t motor_load_sensing_mode(uint32_t period)
{
  unsigned char i;
  unsigned long j;
  static uint8_t intervalTimeoutCnt_load_sensing_id =0;
  static uint8_t intervalTimeoutCnt_load_sensing_storing_data_id =0;
  intervalTimeoutCnt_load_sensing_id+=period ;
  intervalTimeoutCnt_load_sensing_storing_data_id+=period;
  if((intervalTimeoutCnt_load_sensing_id)>=MC_OS_LOAD_SENSING_ID)
  {
    intervalTimeoutCnt_load_sensing_id=0;
    if(vs_Time_motor != 0)
    {
      --vs_Time_motor;
    }		
  }		
  /*....................................................*/
  switch(load_state)
  {
  case start_load_detection_CW:
    if(qWrpmActual_get() == 0)
    {
      load_state = measuring_the_power_CW;
      vs_Time_motor = 400;
      vs_Cnt_load_data = 0;
      vs_Load_data = 0;
      
      motor_start(MOTOR_RUN, -MOTOR_RPM2SPD(100.0), -1);
      motor_start(MOTOR_RUN, -MOTOR_RPM2SPD(150.0), CONVERT_SPDACCEL(150.0));
    }
    else
    {
      motor_break();	
    }
    break;
    
  case measuring_the_power_CW:
    
    if(vs_Time_motor <= 250)
    {	
      if(vs_Cnt_load_data < 20)
      {
        if(intervalTimeoutCnt_load_sensing_storing_data_id >= MC_OS_MOTOR_LOAD_SENSING_STORING_DATA_ID)
        {
          intervalTimeoutCnt_load_sensing_storing_data_id=0;
          vs_Load_data_buf[vs_Cnt_load_data] = abs(qPowerIn_get());
          ++vs_Cnt_load_data;
        }	
      }
      else
      {
        load_state = start_load_detection_CCW;
        vs_Time_motor = 100;
        motor_start(MOTOR_STOP,0,0);
        
        for(i=0,j=0 ; i<20 ; i++)
        {
          j = j + (unsigned long)vs_Load_data_buf[i];
        }
        vs_Load_data = (j+10)/20;
      }	
    }	
    break;
    
  case start_load_detection_CCW:
    if(vs_Time_motor == 0)
    {
      if(qWrpmActual_get() == 0)
      {
        load_state = measuring_the_power_CCW;
        vs_Time_motor = 400;
        vs_Cnt_load_data = 0;
        
        motor_start(MOTOR_RUN, MOTOR_RPM2SPD(100.0), -1);
        motor_start(MOTOR_RUN, MOTOR_RPM2SPD(150.0), CONVERT_SPDACCEL(150.0));
      }
      else
      {
        motor_break();	
      }
    }	
    break;			
    
  case measuring_the_power_CCW:
    
    if(vs_Time_motor <= 250)
    {	
      if(vs_Cnt_load_data < 20)
      {
        if(intervalTimeoutCnt_load_sensing_storing_data_id >= MC_OS_MOTOR_LOAD_SENSING_STORING_DATA_ID)
        {
          intervalTimeoutCnt_load_sensing_storing_data_id=0;
          vs_Load_data_buf[vs_Cnt_load_data] = abs(qPowerIn_get());
          ++vs_Cnt_load_data;
        }	
      }
      else
      {
        load_state = load_display;
        vs_Time_motor = 50;
        motor_start(MOTOR_STOP,0,0);
        
        for(i=0,j=0 ; i<20 ; i++)
        {
          j = j + (unsigned long)vs_Load_data_buf[i];
        }
        vs_Load_data = vs_Load_data + (j+10)/20;
      }	
    }	
    break;
    
  case load_display:
    if(vs_Time_motor == 0)
    {
      motor_break();
      load_detection_result();
//      load_state =start_load_detection_CW;
      return load_detection_level ;
      
      break;
    }
    break;
  }
  return 0 ;
}

/*--------------------------------------------------------*/
void load_detection_result(void)
{
  
  if(vs_Load_data < _1K)
  {										
    load_detection_level = 1;	
  }
  else if(vs_Load_data < _2K)
  {									
    load_detection_level = 2;					
  }		
  else if(vs_Load_data < _3K)
  {									
    load_detection_level = 3;				
  }
  else if(vs_Load_data < _4K)
  {									
    load_detection_level = 4;				
  }
  else if(vs_Load_data < _5K)
  {										
    load_detection_level = 5;				
  }
   else if(vs_Load_data < _6K)
  {										
    load_detection_level = 6;				
  }
  else if(vs_Load_data < _7K)
  {										
    load_detection_level = 7;				
  }
  else if(vs_Load_data < _8K)
  {										
    load_detection_level = 8;				
  }
    else if(vs_Load_data < _9K)
  {										
    load_detection_level = 9;				
  }
    else if(vs_Load_data < _10K)
  {										
    load_detection_level = 10;				
  }
    else if(vs_Load_data < _11K)
  {										
    load_detection_level = 11;				
  }
      else if(vs_Load_data < _12K)
  {										
    load_detection_level = 12;				
  }
      else if(vs_Load_data < _13K)
  {										
    load_detection_level = 13;				
  }
      else if(vs_Load_data < _14K)
  {										
    load_detection_level = 14;				
  }
      else if(vs_Load_data < _15K)
  {										
    load_detection_level = 15;				
  }
   else if(vs_Load_data < _16K)
  {										
    load_detection_level = 16;				
  }
  else 
  {									
    load_detection_level = 17;
  }
  
}
unsigned char load_detection_level_get(void)
{
  return load_detection_level;
}
void motor_break(void)
{
  vs_Cnt_motor_wash = 0;
  vs_Time_motor = 0;
  motor_start(MOTOR_STOP,0,0);
}

void motor_free_wheel(void)
{
  vs_Cnt_motor_wash = 0;
  vs_Time_motor = 0;
  motor_start(MOTOR_OPEN,0,0);
}

void motor_stop(void)
{
  motor_start(MOTOR_STOP,0,0);
}
