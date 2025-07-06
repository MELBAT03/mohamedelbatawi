/* // @suppress("Lack of copyright information")
* Transmitting_frame.c
*
*  Created on: May 14, 2023
*      Author: MELBAT03
*/


#include "communication.h"
#include "../os/inc/os.h"
#include "../core/tmpm370/inc/tmpm370_uart.h"
#include "MC_config.h"
#include "../McDev/DevMotor.h"

uint16_t ipm_temp = 100; // from FOC file
uint16_t Phase_Current = 0; // from foc file

/* flag that refers that the ack buffer is done */
bool_t u8_flag_nedicDDM_intrnalRXbuffer= FALSE;

Fault_status_code Fault_status_code_value;
uint8_t RXdata;
static TX_FRAME_information ACK_FRAME ={0};
static RX_FRAME_information RECEVING_FRAME;
uint16_t VERSION_number = 0x0C;
static volatile uint8_t correct_header=0;
static uint8_t frcount=0;


static comm_state_en comm_state=comm_state_idle;
static comm_TX_state_t comm_TX_state=comm_TX_state_fault_communication;

static uint32_t       fault_timeout_communication=0;
uint32_t time_state;
wm_mode_typedef wm_mode;

uint16_t current_faults= NO_FAULT;
Fault_status_code current_fault_now =NO_FAULT;
/*static function */

static uint8_t crc_check(uint8_t *TX_frame , uint8_t size);
static int combine (uint8_t b1 , uint8_t b2);

// receiving frame
uint8_t ACK_frame_check_flag=0;
uint8_t ACK_frame_check_counter=0;
uint32_t no_of_enters=0;
uint8_t frist_time=0;


void interface_function_ACK(TX_FRAME_information *TX_frame)
{ // Set up washer type
  int mechanical_angular_speed_rpm = getAbs(MC_get_actual_speed());
  uint8_t Weight_Value = 10*MC_get_Water_level();   // uint 100g
  int  DC_bus =0 ;
  Fault_status_code system_errors =MC_get_error();
  TX_frame->TX_frame_componant[TX_HEADER_INDEX_BYTE_0] = TX_RX_FRAME_HEADER_DATA;
  TX_frame->TX_frame_componant[TX_MODE_INDEX_BYTE_1] = TX_frame->FRAME_MODE;
  TX_frame->TX_frame_componant[TX_BYTE_2] = system_errors;
  TX_frame->TX_frame_componant[TX_BYTE_3] =(uint8_t)(mechanical_angular_speed_rpm>>8) ;
  TX_frame->TX_frame_componant[TX_BYTE_4] = (uint8_t)(mechanical_angular_speed_rpm);
  TX_frame->TX_frame_componant[TX_BYTE_5]=0;
  
  
  switch (TX_frame->FRAME_MODE) {
  case Ping:
    TX_frame->TX_frame_componant[TX_BYTE_2] = TX_frame->PLATFORM;//0x15; // determine with load senseing
    TX_frame->TX_frame_componant[TX_BYTE_3] = (uint8_t)(TX_frame->VERSION_NUMBER>>8) ;//0x00;
    TX_frame->TX_frame_componant[TX_BYTE_4] = (uint8_t)(TX_frame->VERSION_NUMBER);//0x0C;
    break;
  case Request:
    TX_frame->TX_frame_componant[TX_BYTE_5] =(uint8_t) (Weight_Value);
    break;
  case CW_rotation:
  case CCW_rotation:
  case Agitation:
  case Spin:
  case Stop:
  case Brake:
  case Motor_free_shaft_spin:
    break;
  case Voltage_detection:
    DC_bus= MC_get_DC_BUS();
    TX_frame->TX_frame_componant[TX_BYTE_3] =(uint8_t)(DC_bus>>8) ;
    TX_frame->TX_frame_componant[TX_BYTE_4] = (uint8_t)(DC_bus);
    break;
  case Weight_detection_enable:
    RECEVING_FRAME.Empty_raw_weight = Weight_Value*1;
    TX_frame->TX_frame_componant[TX_BYTE_3] =(uint8_t)(RECEVING_FRAME.Empty_raw_weight>>8) ;
    TX_frame->TX_frame_componant[TX_BYTE_4] = (uint8_t)(RECEVING_FRAME.Empty_raw_weight);
    TX_frame->TX_frame_componant[TX_BYTE_5] =(uint8_t)(Weight_Value);
    break;
  case Clutching_declutching:
    TX_frame->TX_frame_componant[TX_BYTE_3] =(uint8_t)(mechanical_angular_speed_rpm>>8) ;
    TX_frame->TX_frame_componant[TX_BYTE_4] = (uint8_t)(mechanical_angular_speed_rpm);
    TX_frame->TX_frame_componant[TX_BYTE_5] =(uint8_t)(Movement_not_done);
    break;
  case  Enable_Disable_Forced_brake:
    TX_frame->TX_frame_componant[TX_BYTE_5] =(uint8_t)(Forced_brake_enable);
    break;
  case FCT_test:
    
    TX_frame->TX_frame_componant[TX_HEADER_INDEX_BYTE_0] = 0;
    TX_frame->TX_frame_componant[TX_MODE_INDEX_BYTE_1] = 0;
    
    
    TX_frame->TX_frame_FCT[TX_FCT_HEADER_INDEX_BYTE_0] =TX_RX_FRAME_HEADER_DATA;
    TX_frame->TX_frame_FCT[TX_FCT_MODE_INDEX_BYTE_1] = TX_frame->FRAME_MODE;
    TX_frame->TX_frame_FCT[TX_FCT_BYTE_2] = (uint8_t)(Fault_status_code_value >>8);//(uint8_t) ((uint8_t)(get_Fault_status_code()) >> 8);
    TX_frame->TX_frame_FCT[TX_FCT_BYTE_3] =(uint8_t) Fault_status_code_value;//(get_Fault_status_code());
    TX_frame->TX_frame_FCT[TX_FCT_BYTE_4] = (uint8_t)(mechanical_angular_speed_rpm>>8);
    TX_frame->TX_frame_FCT[TX_FCT_BYTE_5] =(uint8_t)(mechanical_angular_speed_rpm);
    TX_frame->TX_frame_FCT[TX_FCT_BYTE_6] = (uint8_t)(ipm_temp>>8);
    TX_frame->TX_frame_FCT[TX_FCT_BYTE_7] =(uint8_t)(ipm_temp);
    TX_frame->TX_frame_FCT[TX_FCT_BYTE_8] = (uint8_t)(IPM_MODEL +(uint8_t)(DC_bus >> 8));
    TX_frame->TX_frame_FCT[TX_FCT_BYTE_9] =(uint8_t)(DC_bus);
    TX_frame->TX_frame_FCT[TX_FCT_BYTE_10] = (uint8_t)(Phase_Current>>8);
    TX_frame->TX_frame_FCT[TX_FCT_BYTE_11] =(uint8_t)(Phase_Current);
    TX_frame->TX_frame_FCT[TX_FCT_CRC_INDEX_BYTE_12] = crc_check((TX_frame->TX_frame_FCT) ,TX_FCT_FRAME_SIZE );
    
    break;
  default:
    TX_frame->TX_frame_componant[TX_HEADER_INDEX_BYTE_0] = TX_NACK_FRAME_DATA_HEADER;
    TX_frame->TX_frame_componant[TX_MODE_INDEX_BYTE_1] = TX_NACK_FRAME_DATA_BYTE_2;
    TX_frame->TX_frame_componant[TX_BYTE_2] = TX_NACK_FRAME_DATA_BYTE_3;
    TX_frame->TX_frame_componant[TX_BYTE_3] =TX_NACK_FRAME_DATA_BYTE_4 ;
    TX_frame->TX_frame_componant[TX_BYTE_4] = TX_NACK_FRAME_DATA_BYTE_5;
    TX_frame->TX_frame_componant[TX_BYTE_5] =TX_NACK_FRAME_DATA_BYTE_6;
    break;
  }
}
void tx_frame_sending(TX_FRAME_information *TX_frame)
{
  // doing the frame
  // CRC check
  switch (TX_frame->FRAME_MODE) {
  case FCT_test:
    // check sum
    TX_frame->TX_frame_FCT[TX_FCT_CRC_INDEX_BYTE_12] = 0;
    TX_frame->TX_frame_FCT[TX_FCT_CRC_INDEX_BYTE_12] = crc_check((TX_frame->TX_frame_FCT) ,TX_FCT_FRAME_SIZE );
    // transmit
#if defined(COMMAND_WITH_TTL)
    SendUart1Tx((uint8_t *) TX_frame->TX_frame_FCT,TX_FCT_FRAME_SIZE);
#endif
    
    break;
  default:
    // check sum
    TX_frame->TX_frame_componant[TX_CRC_INDEX_BYTE_6] = 0;
    TX_frame->TX_frame_componant[TX_CRC_INDEX_BYTE_6] = crc_check((TX_frame->TX_frame_componant) ,TX_FRAME_SIZE );
    u8_flag_nedicDDM_intrnalRXbuffer= TRUE;
    // transmit
#if defined(COMMAND_WITH_TTL)
    SendUart1Tx((uint8_t *) TX_frame->TX_frame_componant,TX_FRAME_SIZE);
#endif
    break;
  }
}




static uint8_t crc_check(uint8_t *TX_frame , uint8_t size){
  uint8_t CHECK_SUM_RESULT=0;
  for (uint8_t indx=0; indx < size-1 ; indx ++){
    CHECK_SUM_RESULT^= TX_frame[indx];
  }
  return (CHECK_SUM_RESULT);
}





#if communication_with_toshipa
void uart_handle_rx_isr(uint8_t RXdata)
{
  //receiving my message
  if (ACK_frame_check_flag == 1)
  {
    if (ACK_frame_check_counter < (TX_FRAME_SIZE-1))
    {
      if (RXdata == ACK_FRAME.TX_frame_componant[ACK_frame_check_counter++])
      {
      }
    }
    else{
      ACK_frame_check_flag = 0;
      ACK_frame_check_counter=0;
      correct_header= 0; frcount= 0;      
    }
  }
  
  else{
    /* to check the spend time of receving complete frame */
    if (frcount == 0){
      
    }
    if (correct_header== 0x01)
    {
      RECEVING_FRAME.RX_frame_complete[frcount]=RXdata;
      
      if (frcount== (RX_FRAME_SIZE-1))
      {
        if (crc_check(RECEVING_FRAME.RX_frame_complete, RX_FRAME_SIZE) == RECEVING_FRAME.RX_frame_complete[RX_FRAME_SIZE-1])
        {
          RECEVING_FRAME.flag = 1;
          
          switch(frist_time){
          case 0:
            {
              frist_time++;
              break;
            }
          case 1:
            {
              frist_time--;
              break;
            }
          }
          
        }
        else
        {
          ACK_FRAME.FRAME_MODE =0;
          interface_function_ACK(&ACK_FRAME);
          tx_frame_sending(huart, &ACK_FRAME);
        }
        correct_header= 0; frcount= 0;ACK_frame_check_flag =1;
      }
      else
      {
        frcount++;
      }
    }
    else
    {
      if ((correct_header== 0x00)&&(RXdata== TX_RX_FRAME_HEADER_DATA))
      {
        correct_header= 0x01;
        RECEVING_FRAME.RX_frame_complete[frcount++]=RXdata;
      }
    }
  }
  
  
}

#elif communication_with_PC

void uart_handle_rx_isr(uint8_t RXdata)
{
  //		HAL_GPIO_WritePin(LED_4_PORT, LED_4_PIN,GPIO_PIN_SET);
  //receiving my message
  if (correct_header== 0x01)
  {
    RECEVING_FRAME.RX_frame_complete[frcount]=RXdata;
    
    if (frcount== (RX_FRAME_SIZE-1))
    {
      if (crc_check(RECEVING_FRAME.RX_frame_complete, RX_FRAME_SIZE) == RECEVING_FRAME.RX_frame_complete[RX_FRAME_SIZE-1])
      {
        RECEVING_FRAME.flag = 1;
      }
      else
      {
        ACK_FRAME.FRAME_MODE = (TX_RX_FRAME_MODE)0;
        interface_function_ACK(&ACK_FRAME);
        tx_frame_sending( &ACK_FRAME);
      }
      correct_header= 0; frcount= 0;
    }
    else
    {
      frcount++;
    }
  }
  else
  {
    if ((correct_header== 0x00)&&(RXdata== TX_RX_FRAME_HEADER_DATA))
    {
      correct_header= 0x01;
      RECEVING_FRAME.RX_frame_complete[frcount++]=RXdata;
    }
  }
}
#else
"error should select communication"


#endif
void evacuation_receving_frame(void)
{
  RECEVING_FRAME.FRAME_MODE = (TX_RX_FRAME_MODE)RECEVING_FRAME.RX_frame_complete[RX_MODE_INDEX_BYTE_1];
  ACK_FRAME.FRAME_MODE =RECEVING_FRAME.FRAME_MODE ;
  switch (RECEVING_FRAME.FRAME_MODE)
  {
  case Ping:
    ACK_FRAME.PLATFORM = RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_2];
    ACK_FRAME.VERSION_NUMBER =VERSION_number;
    RECEVING_FRAME.Empty_raw_weight = combine(RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_3], RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_4]);
    break;
  case Request:
  case Stop:
  case Brake:
  case Weight_detection_enable:
  case Voltage_detection:
    break;
  case CCW_rotation:
  case Spin:
  case Motor_free_shaft_spin:
  case FCT_test:
    RECEVING_FRAME.Motor_Speed = combine(RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_2] ,RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_3] );
    if (RECEVING_FRAME.Motor_Speed > MAX_SPEED) RECEVING_FRAME.Motor_Speed =MAX_SPEED;
    RECEVING_FRAME.Time_Accel = combine(RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_4] ,RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_5] );
  RECEVING_FRAME.Time_Accel =RECEVING_FRAME.Time_Accel*FRAME_TIME_UNIT;
    break;
  case CW_rotation:
    RECEVING_FRAME.Motor_Speed =  combine(RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_2] ,RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_3] );
    if (RECEVING_FRAME.Motor_Speed > MAX_SPEED) RECEVING_FRAME.Motor_Speed =MAX_SPEED;
    RECEVING_FRAME.Motor_Speed *=-1;
    RECEVING_FRAME.Time_Accel = combine(RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_4] ,RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_5] );
      RECEVING_FRAME.Time_Accel =RECEVING_FRAME.Time_Accel*FRAME_TIME_UNIT;
    break;
  case Agitation:
    RECEVING_FRAME.Motor_Speed =  combine(RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_2] ,RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_3] );
    RECEVING_FRAME.Time_Accel = (uint8_t)(RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_4]);
    RECEVING_FRAME.Time_decel = (uint8_t)(RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_5]);
    RECEVING_FRAME.Time_CW_on = (uint8_t)(RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_6]);
    RECEVING_FRAME.Time_CW_off = (uint8_t)(RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_7]);
    RECEVING_FRAME.Time_CCW_on = (uint8_t)(RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_8]);
    RECEVING_FRAME.Time_CCW_off = (uint8_t)(RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_9]);
      RECEVING_FRAME.Time_Accel =RECEVING_FRAME.Time_Accel*FRAME_TIME_UNIT;
  RECEVING_FRAME.Time_decel =RECEVING_FRAME.Time_decel*FRAME_TIME_UNIT;
  RECEVING_FRAME.Time_CCW_off =RECEVING_FRAME.Time_CCW_off*FRAME_TIME_UNIT;
  RECEVING_FRAME.Time_CCW_on =RECEVING_FRAME.Time_CCW_on*FRAME_TIME_UNIT;
  RECEVING_FRAME.Time_CW_off =RECEVING_FRAME.Time_CW_off*FRAME_TIME_UNIT;
  RECEVING_FRAME.Time_CW_on =RECEVING_FRAME.Time_CW_on*FRAME_TIME_UNIT;
    break;
    
  case Clutching_declutching:
    RECEVING_FRAME.Wash2spin_OR_Spin2wash = RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_2];
    break;
  case Enable_Disable_Forced_brake:
    RECEVING_FRAME.Forced_brake = RECEVING_FRAME.RX_frame_complete[RX_INDEX_BYTE_2];
    break;
  default:
    break;
  }

}



static int combine (uint8_t b1 , uint8_t b2){
  int combined = b1 << 8 |b2;
  return (combined);
}

RX_FRAME_information* get_rx_frame_information(void) // get the time value
{
  return (&RECEVING_FRAME);
}

TX_FRAME_information* get_ack_frame_information(void) // get the time value
{
  return (&ACK_FRAME);
}



////////////////////////////////



void APP_comm_update(uint32_t period)  // 50 msec
{
  switch (comm_state)
  {
  case comm_state_idle:
    comm_idle_mode_check();
    
    break;
  case comm_state_receive:
    evacuation_receving_frame();
    comm_state = comm_state_send;
    comm_TX_state = comm_TX_state_normal;
    break;
  case comm_state_send:
    comm_send_mode_check();
#if !defined(MC_STAND_ALONE_DUMMYS)
    prepare_communication();
#endif
    break;
  default :
    break;
  }
  
}

void comm_idle_mode_check(void){
  if (RECEVING_FRAME.flag == 1)
  {
    RECEVING_FRAME.flag = 0;
    fault_timeout_communication=0;
    comm_state = comm_state_receive;
  }
  //  	fault_timeout_communication++;
  if (fault_timeout_communication>60){
    comm_state = comm_state_send;
    comm_TX_state = comm_TX_state_fault_communication;
  }
  
}

void comm_send_mode_check(void){
  switch(comm_TX_state){
  case comm_TX_state_normal:
    interface_function_ACK(&ACK_FRAME);
    tx_frame_sending(&ACK_FRAME);
    comm_state = comm_state_idle;
    break;
  case comm_TX_state_fault_communication:
    ACK_FRAME.FRAME_MODE = FCT_test;
    interface_function_ACK(&ACK_FRAME);
    tx_frame_sending( &ACK_FRAME);
    fault_timeout_communication=0;
    comm_state = comm_state_idle;
    
    break;
  default:
    break;
  }
}


void prepare_communication(void)
{
//    uint16_t vs_accel_temp =SLOPE_TIME(RECEVING_FRAME.Motor_Speed,SPEED_INIT,RECEVING_FRAME.Time_Accel); //RECEVING_FRAME.Motor_Speed*10 / RECEVING_FRAME.Time_Accel;
//    uint16_t vs_decel_temp =SLOPE_TIME(RECEVING_FRAME.Motor_Speed,0,RECEVING_FRAME.Time_decel); //RECEVING_FRAME.Motor_Speed*10 / RECEVING_FRAME.Time_decel;

  switch (RECEVING_FRAME.FRAME_MODE)
  {
  case Ping:
  case Request:
    break;
  case CCW_rotation:
  case CW_rotation:
//  case Spin:
  case Motor_free_shaft_spin:
    {
      run_paramter Vs_run_parameter;
      Vs_run_parameter.Vs_mode = WM_CONSTAT_SPEED;//WM_CONSTAT_SPEED;
      Vs_run_parameter.rpm     = RECEVING_FRAME.Motor_Speed;
      Vs_run_parameter.accel   =SLOPE_TIME(ABS(RECEVING_FRAME.Motor_Speed),0,RECEVING_FRAME.Time_Accel);
      Vs_run_parameter.decel   =0;//SLOPE_TIME(RECEVING_FRAME.Motor_Speed,0,RECEVING_FRAME.Time_decel);
      Vs_run_parameter.time_off=0;
      Vs_run_parameter.time_on =0;
      MC_Set_mode_parameter(Vs_run_parameter);
      parameter_index_set(BASKET_PARAMETER_INDEX);
    }
    break;
    case Spin:
      {
      run_paramter Vs_run_parameter;
      Vs_run_parameter.Vs_mode = WM_Spin;
      Vs_run_parameter.rpm     = 0;
      Vs_run_parameter.accel   =0;
      Vs_run_parameter.decel   =0;//SLOPE_TIME(RECEVING_FRAME.Motor_Speed,0,RECEVING_FRAME.Time_decel);
      Vs_run_parameter.time_off=0;
      Vs_run_parameter.time_on =0;
      MC_Set_mode_parameter(Vs_run_parameter);
      parameter_index_set(BASKET_PARAMETER_INDEX);
    }
      break;
  case Agitation:
    {
      run_paramter Vs_run_parameter;
      Vs_run_parameter.Vs_mode = WM_Washing;
      Vs_run_parameter.rpm     = RECEVING_FRAME.Motor_Speed;
      Vs_run_parameter.accel   =SLOPE_TIME(RECEVING_FRAME.Motor_Speed,SPEED_INIT,RECEVING_FRAME.Time_Accel);
      Vs_run_parameter.decel   =SLOPE_TIME(RECEVING_FRAME.Motor_Speed,0,RECEVING_FRAME.Time_decel);
      Vs_run_parameter.time_off=(RECEVING_FRAME.Time_CCW_off+RECEVING_FRAME.Time_decel)/MC_APP_WM_task_OS_time;//(RECEVING_FRAME.Time_CCW_off+RECEVING_FRAME.Time_decel)*100/MC_APP_WM_task_OS_time;
      Vs_run_parameter.time_on =(RECEVING_FRAME.Time_CCW_on+RECEVING_FRAME.Time_Accel)/MC_APP_WM_task_OS_time;//(RECEVING_FRAME.Time_CCW_on+RECEVING_FRAME.Time_Accel)*100/MC_APP_WM_task_OS_time;
      MC_Set_mode_parameter(Vs_run_parameter);
      parameter_index_set(PULSATOR_PARAMETER_INDEX);
      
    }
     break;
    //    case Spin:
    //      
    //      break;
  case Stop:
    {
      run_paramter Vs_run_parameter;
      Vs_run_parameter.Vs_mode = WM_FREE_WHEEL;
      Vs_run_parameter.rpm     = 0;
      Vs_run_parameter.accel   =0;
      Vs_run_parameter.time_off=0;
      Vs_run_parameter.time_on =0;
      MC_Set_mode_parameter(Vs_run_parameter);      
    }
     break;
  case Brake:
    {
      run_paramter Vs_run_parameter;
      Vs_run_parameter.Vs_mode = WM_BREAK;
      Vs_run_parameter.rpm     = 0;
      Vs_run_parameter.accel   =0;
      Vs_run_parameter.time_off=0;
      Vs_run_parameter.time_on =0;
      MC_Set_mode_parameter(Vs_run_parameter);
      

    }
    break;
  case Voltage_detection:
    {
      
    }
     break;
  case Weight_detection_enable:
    {
      run_paramter Vs_run_parameter;
      Vs_run_parameter.Vs_mode = WM_LOAD_SENSING;
      Vs_run_parameter.rpm     = 0;
      Vs_run_parameter.accel   =0;
      Vs_run_parameter.time_off=0;
      Vs_run_parameter.time_on =0;
      MC_Set_mode_parameter(Vs_run_parameter);
      parameter_index_set(PULSATOR_PARAMETER_INDEX);
      
    }
          break;

  case Clutching_declutching:
    //					wm_mode=WM_Clutching_declutching;
    break;
  case Enable_Disable_Forced_brake:
    {
      run_paramter Vs_run_parameter;
      Vs_run_parameter.Vs_mode = WM_BREAK;
      Vs_run_parameter.rpm     = 0;
      Vs_run_parameter.accel   =0;
      Vs_run_parameter.time_off=0;
      Vs_run_parameter.time_on =0;
      MC_Set_mode_parameter(Vs_run_parameter);
      
    }
          break;

  case FCT_test:
    break;
  default:
    break;
  }
  
  
}




void fault_set(Fault_status_code error)
{
  current_faults |=error;
  //	current_fault_now = error;
  
}

Fault_status_code fault_get(void)
{
  Fault_status_code fault=NO_FAULT;
  if (current_faults != NO_FAULT)
  {
    fault = current_fault_now;
  }
  return fault;
}
void fault_clear(void)
{
  current_faults =0;
  current_fault_now = NO_FAULT;
}



void HAL_UART_RxCpltCallback(void)
{
  RXdata =  UART_GetRxData(UART1);  //recieve the sent value at the heartbeat function 
  uart_handle_rx_isr( RXdata);
  
}
void mc_set_integ_TX_buffer( uint8_t u8_nedicDDM_intrnalTXbuffer)  //replace with" uart_byte_put "function in the for loop
{
  uart_handle_rx_isr(u8_nedicDDM_intrnalTXbuffer);
}

uint8_t *mc_get_integ_RX_buffer (uint8_t* ptru8_nedicDDM_intrnalTXbuffer)
{
  u8_flag_nedicDDM_intrnalRXbuffer = FALSE;
  
  return ACK_FRAME.TX_frame_componant;
}

bool_t mc_get_integ_RX_buffer_flag (void)
{
  return u8_flag_nedicDDM_intrnalRXbuffer;
}