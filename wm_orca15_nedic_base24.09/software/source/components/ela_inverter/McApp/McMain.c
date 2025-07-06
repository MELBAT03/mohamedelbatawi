/******************************************************************************* * 
* Header: inverter 
*  Author: mohamed ibrahim elbatawi
* Description: configer the movement of the WM inverter 
*  mohaemd elbatawi  4/5/2025
* *******************************************************************************/

/*========================================================*/
/*  		main.c                           			  */
/*========================================================*/

/*========================================================*/
/*			Include File     							  */
/*========================================================*/
#include "McProject.h"
#include "../McDev/DevMotor.h"
#include "../McUtil/DevPeripheral.h"
#include "app_startup.h"
#include "os.h"
#include "MC_interface.h"
#include "power_relay.h"
#include "HEART_BEAT.h"



// motor control task
#include "APP_measurement.h"
#include "communication.h"
#include "HEART_BEAT.h"
#include "inverter.h"
#include "MC_interface.h"
#include "power_relay.h"
#include "MC_config.h"
void mc_app_entry_startup_init(void)
{
  
//  HEARTBEAT_Init();
  DevInitMotor();
//  Power_relay_init();
   
}

void mc_app_init(void)
{
      // motor tasks
//    os_scheduler_task_add(Power_relay_Update,"MCC",TIME_BASED_TASK,OS_SCH_MULTIPLIER_NONE,0,1);    // shared 
      os_scheduler_task_add(APP_comm_update,"MCC",TIME_BASED_TASK,OS_SCH_MULTIPLIER_NONE,0,MC_APP_comm_task_os_time);     // motor control communication
    os_scheduler_task_add(APP_measurment_Update,"MCC",TIME_BASED_TASK,OS_SCH_MULTIPLIER_NONE,0,MC_APP_measurment_os_time);
    os_scheduler_task_add(inverter_fail_Update,"MCC",TIME_BASED_TASK,OS_SCH_MULTIPLIER_NONE,0,MC_APP_inverter_fail_Update);
    os_scheduler_task_add(WM_Update,"MCC",TIME_BASED_TASK,OS_SCH_MULTIPLIER_NONE,0,MC_APP_WM_task_OS_time);
//   os_scheduler_task_add(dummy_test_software,"MCC",TIME_BASED_TASK,OS_SCH_MULTIPLIER_NONE,5,1000);
      
  //    os_scheduler_task_add(HEARTBEAT_Update,"MCC",TIME_BASED_TASK,OS_SCH_MULTIPLIER_NONE,5,MC_HEARTBEAT_UPDATE_PERIOD_MS);
#if defined(MC_STAND_ALONE_DUMMYS)
    os_scheduler_task_add(dummy_test_software,"MCC",TIME_BASED_TASK,OS_SCH_MULTIPLIER_NONE,5,1000);
    // os_scheduler_task_add(dummy_drain_test,"MCD",TIME_BASED_TASK,OS_SCH_MULTIPLIER_NONE,3,10);
    // os_scheduler_task_add(dummy_clutch_test,"MCD",TIME_BASED_TASK,OS_SCH_MULTIPLIER_NONE,2,10);


#endif
////    os_scheduler_task_add(MC_Perf_GetTOTAL_CPU_Load_task   ,   "MCC",TIME_BASED_TASK,OS_SCH_MULTIPLIER_NONE, 0  , OS_TIMER_UPDATE_PERIOD_MS       );
}
/*--------------------------------------------------------*/
/*		[End Of Source File]                              */
/*--------------------------------------------------------*/
