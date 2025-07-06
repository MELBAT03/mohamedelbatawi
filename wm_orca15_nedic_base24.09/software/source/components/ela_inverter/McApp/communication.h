/*
 * communication.h
 *
 *  Created on: May 14, 2023
 *      Author: MELBAT03
 */

#ifndef COMMUNICATION_COMMUNICATION_H_
#define COMMUNICATION_COMMUNICATION_H_

#include "system_TMPM370.h"

#include "MC_interface.h"
#include "../os/utils/inc/utils.h"
#include "../McUtil/DevPeripheral.h"
#include "config.h"

#define monitor_of_communication      0
#define request_timeout_duration   6000   // micro s

#define MAX_SPEED							(80*9.55)

#define IPM_MODEL			20

#define communication_with_toshipa      0
#define communication_with_PC           1


typedef enum {
	comm_state_idle=0,
	comm_state_send=1,
	comm_state_receive=2,
	no_of_states,
}comm_state_en;

typedef enum {
	comm_TX_state_normal=0,
	comm_TX_state_fault_communication,
	no_of_TX_states,
}comm_TX_state_t;

// typedef enum {
//	NO_ERROR = 0,
// 	General_fault = 0x01,
// 	IPM_over_temperature = 0x02,
// 	DC_Bus_voltage_fail =0x04,
// 	Bus_over_voltage = 0x04,
// 	Motor_stall = 0x08,
// 	Hardware_over_current = 0x10,
// 	Bus_under_voltage = 0x20,
// 	Lost_phase =0x40,
// 	Motor_over_temperature = 0x80
// }motor_fault;

typedef enum
{
	TX_HEADER_INDEX_BYTE_0,
	TX_MODE_INDEX_BYTE_1,
	TX_BYTE_2,
	TX_BYTE_3,
	TX_BYTE_4,
	TX_BYTE_5,
	TX_CRC_INDEX_BYTE_6=6,
	TX_FRAME_SIZE
}TX_frame_componant;

  typedef enum
  {
	TX_FCT_HEADER_INDEX_BYTE_0,
	TX_FCT_MODE_INDEX_BYTE_1,
	TX_FCT_BYTE_2,
	TX_FCT_BYTE_3,
	TX_FCT_BYTE_4,
	TX_FCT_BYTE_5,
	TX_FCT_BYTE_6,
	TX_FCT_BYTE_7,
	TX_FCT_BYTE_8,
	TX_FCT_BYTE_9,
	TX_FCT_BYTE_10,
	TX_FCT_BYTE_11,
	TX_FCT_CRC_INDEX_BYTE_12,
	TX_FCT_FRAME_SIZE
}TX_frame_FCT;

typedef enum
{
	RX_HEADER_INDEX_BYTE_0,
	RX_MODE_INDEX_BYTE_1,
	RX_INDEX_BYTE_2,
	RX_INDEX_BYTE_3,
	RX_INDEX_BYTE_4,
	RX_INDEX_BYTE_5,
	RX_INDEX_BYTE_6,
	RX_INDEX_BYTE_7,
	RX_INDEX_BYTE_8,
	RX_INDEX_BYTE_9,
	RX_CRC_INDEX_BYTE_10,
	RX_FRAME_SIZE
}RX_frame_componant;

typedef enum{
	TX_RX_FRAME_HEADER_DATA = 0xA5,
	TX_FRAME_HEADER_DATA_2=0x5A
}TX_RX_FRAME_HEADER;

typedef enum {
	Ping = 0x02,
	Request= 0x05,
	CW_rotation = 0x09 ,
	CCW_rotation = 0x0B ,
	Agitation=0x0C,
	Spin=0X0D,
	Stop =0X27,
	Brake =0X29,
	Voltage_detection = 0X2B,
	Weight_detection_enable =  0X2C,
	Fabric_detection_enable =  0x2D,
	Clutching_declutching = 0x19,
	Enable_Disable_Forced_brake =  0x1B ,
	Motor_free_shaft_spin = 0x1D ,
	FCT_test = 0x66,
}TX_RX_FRAME_MODE;

typedef enum { //a washer volume data
	PLATFORM_volume_6Kg   =0x6,
	PLATFORM_volume_7kg	 =0x7,
	PLATFORM_volume_8kg	=0x8
}TX_RX_PLATFORM;





typedef enum{ // unknown
	Raw_weight_data_1 = (uint16_t)50,
	Raw_weight_data_2 = (uint16_t)52,
	Raw_weight_data_3 = (uint16_t)58,

}Raw_weight_data;

typedef enum{
	Movement_not_done =0,
	Movement_done =1,

}Movement_status;
typedef enum {
	Forced_brake_disable ,
	Forced_brake_enable
}Forced_brake_status;

typedef struct {
	uint8_t TX_frame_componant[TX_FRAME_SIZE];
	uint8_t TX_frame_FCT[TX_FCT_FRAME_SIZE];
	TX_RX_FRAME_MODE FRAME_MODE;
	uint16_t VERSION_NUMBER;
	uint8_t PLATFORM;
}TX_FRAME_information;

typedef struct {
	TX_RX_FRAME_MODE FRAME_MODE;
	uint16_t VERSION_NUMBER;
	uint8_t PLATFORM;
	uint16_t Empty_raw_weight;
	int16_t Motor_Speed;
	uint16_t Time_Accel;
	uint16_t Time_decel;
	uint16_t Time_CW_on;
	uint16_t Time_CW_off;
	uint16_t Time_CCW_on;
	uint16_t Time_CCW_off;
	uint8_t Forced_brake;
	uint8_t Wash2spin_OR_Spin2wash;
	uint8_t flag;
	uint8_t RX_frame_complete[RX_FRAME_SIZE];
}RX_FRAME_information;


typedef enum {
	TX_NACK_FRAME_DATA_HEADER = TX_FRAME_HEADER_DATA_2,
	TX_NACK_FRAME_DATA_BYTE_2 = 0x00,
	TX_NACK_FRAME_DATA_BYTE_3 = 0xFF,
	TX_NACK_FRAME_DATA_BYTE_4 = 0x00,
	TX_NACK_FRAME_DATA_BYTE_5 = 0xFF,
	TX_NACK_FRAME_DATA_BYTE_6 = 0x00
}TX_NACK_FRAME_DATA;




void comm_send_mode_check(void);
void comm_idle_mode_check(void);

void interface_function_ACK(TX_FRAME_information *TX_frame);
//void uart_handle_rx_isr(UART_HandleTypeDef *huart);
void uart_handle_rx_isr(uint8_t RXdata);

void tx_frame_sending(TX_FRAME_information *TX_frame);
//RX_FRAME_information evacuation_receving_frame(void);
 void evacuation_receving_frame(void);
void coping_data_from_rx_frame(void);
RX_FRAME_information* get_rx_frame_information(void);
void updat_mode(void);

void increment_fault_timeout_communication(void);
uint32_t get_fault_timeout_communication(void);
void  set_fault_timeout_communication(uint32_t value);
void timer_Handle_RX_ISR(void);
//void APP_comm_update(void *arg);
void APP_comm_update(uint32_t period);

RX_FRAME_information* get_rx_frame_information(void);
TX_FRAME_information* get_ack_frame_information(void);

void prepare_communication(void);
uint32_t get_motor_speed(void);
uint32_t get_time_CCW_on(void);
uint32_t get_time_CW_on(void);
void fault_clear( void);
void fault_time_report_to_main_update(void) ;
void HAL_UART_RxCpltCallback(void);
// void clear_frame(TX_FRAME_information *TX_frame);

//motor_fault get_motor_fault(void);
//void set_motor_fault(motor_fault fault_type);
//void motor_fault_detection(UART_HandleTypeDef *huart);

bool_t mc_get_integ_RX_buffer_flag (void);
uint8_t *mc_get_integ_RX_buffer (uint8_t* ptru8_nedicDDM_intrnalTXbuffer);
void mc_set_integ_TX_buffer( uint8_t u8_nedicDDM_intrnalTXbuffer);  //replace with" uart_byte_put "function in the for loop

#endif /* COMMUNICATION_COMMUNICATION_H_ */
