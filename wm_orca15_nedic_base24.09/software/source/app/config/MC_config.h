/******************************************************************************* * 
 * Header: MC_config.h
 *  Author: mohamed ibrahim elbatawi
 * Description: configuration of os task aperiodicity and OS tick time  header file
 *  mohaemd elbatawi  Initial Version of the MCconfig.h 
 * *******************************************************************************/
#ifndef CONFIG_H_
#define CONFIG_H_

/******************************************************************************/
/*                               INCLUDES                                     */
/******************************************************************************/


//#include "../app/timer.h"

/******************************************************************************/
/*                                MACROS                                      */
/******************************************************************************/

/* Features To be added To the System */
//t for test and p for production 24 year 32 week number 0 is version number in the same week
#define version_numver                          M5I5B96

// #define MC_STAND_ALONE_DUMMYS           
//#define COMMAND_WITH_TTL              // Run the motor from ttl with the frame of WM with uart  
#define MC_DEBUG_UART_ENABLED           // enable  the debug port
//#define MC_SPIN_INTERNAL_ALG                  // if we use the spin algorithm and communication wit tll                

/****************MC_os task configuration******************/
#define MC_APP_comm_task_os_time                         (50)
#define MC_APP_measurment_os_time                       (10)
#define MC_APP_inverter_fail_Update                     (10)
#define MC_APP_WM_task_OS_time                          (10)
#define MC_HEARTBEAT_UPDATE_PERIOD_MS                    (1000)
/*********************************software timer id ********************************/
   
#define MC_OS_MOTOR_SPIN_ID                                (1000)
#define MC_OS_LOAD_SENSING_ID                              (10)
#define MC_OS_MOTOR_LOAD_SENSING_STORING_DATA_ID           (100)
#define MC_OS_TOURQUE_UNBALANCE_VDC_ID                     (10)
#define MC_OS_RPM_POWER_ID                                 (100)
/*********************************mc_debug port config********************************/

#define MC_DEBUG_UART                        UART0
#define MC_DEBUG_UART_RX_IRG               INTRX0_IRQn
#define MC_DEBUG_UART_TX_IRG               INTTX0_IRQn


/*********************************mc_communication uints********************************/
#define FRAME_TIME_UNIT                 (100)
#define WEIGHT_TIME_UNIT                (100)

#endif   // CONFIG_H_
