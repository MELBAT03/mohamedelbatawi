#ifndef BOARD_TMPM370_H
#define BOARD_TMPM370_H

#include "component_config.h"

#include "gpio/gpio.h"
#include "keypad/keypad.h"

#define MODEL_MARKER (1500)
#define MODEL_NUMBER (0x15)

#define BOARD_CHECKSUM (BOARD_TYPE + MODEL_MARKER)

#if DDM_MODEL == DDM_EXTERNAL

/*********************************************************************/
// heartbeat
#define HEARTBEAT_LED_PORT       ()
#define HEARTBEAT_LED_PIN        ()
#define HEARTBEAT_LED_INIT_STATE ()

/*********************************************************************/
// dinput
#define POWER_OFF_SW_PORT ()
#define POWER_OFF_SW_PIN  ()

#define LOCK_MOTOR_SENSOR_PORT (GPIO_PORTC)
#define LOCK_MOTOR_SENSOR_PIN  (GPIO_PIN6)

#define LID_SENSOR_PORT (GPIO_PORTJ)
#define LID_SENSOR_PIN  (GPIO_PIN3)

#define LINT_FILTER_PORT ()
#define LINT_FILTER_PIN  ()

#define DETERGENT_LIMIT_PORT (GPIO_PORTH)
#define DETERGENT_LIMIT_PIN  (GPIO_PIN4)

#define SOFTNER_LIMIT_PORT (GPIO_PORTH)
#define SOFTNER_LIMIT_PIN  (GPIO_PIN3)

// external interrupt
#define ZCD_PORT (GPIO_PORTH)
#define ZCD_PIN  (GPIO_PIN1)
#define ZCD_IRQ  (MCAL_EXTI_IRQ1)

/*********************************************************************/
// doutput
#define DRAIN_PUMP_PORT (GPIO_PORTG)
#define DRAIN_PUMP_PIN  (GPIO_PIN4)

#define DETERGENT_PUMP_PORT (GPIO_PORTD)
#define DETERGENT_PUMP_PIN  (GPIO_PIN1)

#define SOFTNER_PUMP_PORT (GPIO_PORTC)
#define SOFTNER_PUMP_PIN  (GPIO_PIN5)

#define DRAIN_VALVE_PORT ()
#define DRAIN_VALVE_PIN  ()

#define CLUTCH_PORT (GPIO_PORTC)
#define CLUTCH_PIN  (GPIO_PIN4)

#define LOCK_MOTOR_PORT (GPIO_PORTF)
#define LOCK_MOTOR_PIN  (GPIO_PIN2)

#define COLD_WATER_VALVE_PORT (GPIO_PORTE)
#define COLD_WATER_VALVE_PIN  (GPIO_PIN7)

#define HOT_WATER_VALVE_PORT (GPIO_PORTC)
#define HOT_WATER_VALVE_PIN  (GPIO_PIN0)

#define SOFTENER_WATER_VALVE_PORT (GPIO_PORTE)
#define SOFTENER_WATER_VALVE_PIN  (GPIO_PIN5)

#define SHOWER_WATER_VALVE_PORT (GPIO_PORTE)
#define SHOWER_WATER_VALVE_PIN  (GPIO_PIN6)

#define CONDENSER_VALVE_PORT ()
#define CONDENSER_VALVE_PIN  ()

#define RELAY_PORT (GPIO_PORTC)
#define RELAY_PIN  (GPIO_PIN2)

#define DDM_RELAY_PORT ()
#define DDM_RELAY_PIN  ()

// NOTE: not used from the doutputmodule
#define BUZZER_ENABLE_PORT (GPIO_PORTA)
#define BUZZER_ENABLE_PIN  (GPIO_PIN2)

#define DISPLAY_RESET_SIGNAL_PORT (GPIO_PORTA)
#define DISPLAY_RESET_SIGNAL_PIN  (GPIO_PIN4)

#define HEATER_PLATE_1_PORT (GPIO_PORTC)
#define HEATER_PLATE_1_PIN  (GPIO_PIN3)

#define HEATER_PLATE_2_PORT (GPIO_PORTC)
#define HEATER_PLATE_2_PIN  (GPIO_PIN1)

#define COOLING_FAN_LOW_PORT ()
#define COOLING_FAN_LOW_PIN  ()

#define COOLING_FAN_HIGH_PORT ()
#define COOLING_FAN_HIGH_PIN  ()

/*********************************************************************/
// internal timers
#define ACIN_TIME_TIMER (MCAL_TIMER_6)

/*********************************************************************/
// analog inputs
// #define ACIN_ADC_PORT (GPIO_PORTI)
// #define ACIN_ADC_PIN  (GPIO_PIN0)
// #define ACIN_ADC_BLK  (MCAL_ADC_BLK_A)
// #define ACIN_ADC_CH   (MCAL_ADC_CH8)

#define AMBIENT_TEMP_SENSOR_PORT    ()
#define AMBIENT_TEMP_SENSOR_PIN     ()
#define AMBIENT_TEMP_SENSOR_ADC_BLK ()
#define AMBIENT_TEMP_SENSOR_ADC_CH  ()

#define WATER_TEMP_SENSOR_PORT    (GPIO_PORTJ)
#define WATER_TEMP_SENSOR_PIN     (GPIO_PIN5)
#define WATER_TEMP_SENSOR_ADC_BLK (MCAL_ADC_BLK_B)
#define WATER_TEMP_SENSOR_ADC_CH  (MCAL_ADC_CH8)

#define AIR_TEMP_SENSOR_A_PORT    ()
#define AIR_TEMP_SENSOR_A_PIN     ()
#define AIR_TEMP_SENSOR_A_ADC_BLK ()
#define AIR_TEMP_SENSOR_A_ADC_CH  ()

#define AIR_TEMP_SENSOR_B_PORT    ()
#define AIR_TEMP_SENSOR_B_PIN     ()
#define AIR_TEMP_SENSOR_B_ADC_BLK ()
#define AIR_TEMP_SENSOR_B_ADC_CH  ()

#define HEATER_SENSOR_PORT    (GPIO_PORTJ)
#define HEATER_SENSOR_PIN     (GPIO_PIN4)
#define HEATER_SENSOR_ADC_BLK (MCAL_ADC_BLK_B)
#define HEATER_SENSOR_ADC_CH  (MCAL_ADC_CH7)

/*********************************************************************/
// freq inputs
#define WATER_LEVEL_PORT           (GPIO_PORTA)
#define WATER_LEVEL_PIN            (GPIO_PIN0)
#define WATER_LEVEL_CHANNEL        (FINPUT_MODULE_0)
#define WATER_LEVEL_DETECTION_EDGE (FINPUT_MODULE_EDGE_RISING)

#define DRAIN_VALVE_SENSOR_PORT           ()
#define DRAIN_VALVE_SENSOR_PIN            ()
#define DRAIN_VALVE_SENSOR_CHANNEL        ()
#define DRAIN_VALVE_SENSOR_DETECTION_EDGE ()

#define CLUTCH_SENSOR_PORT           (GPIO_PORTD)
#define CLUTCH_SENSOR_PIN            (GPIO_PIN0)
#define CLUTCH_SENSOR_CHANNEL        (FINPUT_MODULE_5)
#define CLUTCH_SENSOR_DETECTION_EDGE (FINPUT_MODULE_EDGE_RISING)

#define BLOWER_FEEDBACK_PORT           ()
#define BLOWER_FEEDBACK_PIN            ()
#define BLOWER_FEEDBACK_CHANNEL        ()
#define BLOWER_FEEDBACK_DETECTION_EDGE ()

#define COOLING_FAN_FEEDBACK_PORT           ()
#define COOLING_FAN_FEEDBACK_PIN            ()
#define COOLING_FAN_FEEDBACK_CHANNEL        ()
#define COOLING_FAN_FEEDBACK_DETECTION_EDGE ()

/*********************************************************************/
// freq outputs
#define BUZZER_FREQ_PORT (GPIO_PORTA)
#define BUZZER_FREQ_PIN  (GPIO_PIN3)
#define BUZZER_CHANNEL   (FOUTPUT_MODULE_1)

#define BLOWER_CONTROL_PORT    ()
#define BLOWER_CONTROL_PIN     ()
#define BLOWER_CONTROL_CHANNEL ()

/*********************************************************************/
// stwi
#define EEPROM_ADDRESS      (0xA0)
#define EEPROM_TIMER_MODULE (HW_TIMER_3)

#define TWI_DATA_PORT (GPIO_PORTD)
#define TWI_DATA_PIN  (GPIO_PIN3)

#define TWI_CLOCK_PORT (GPIO_PORTD)
#define TWI_CLOCK_PIN  (GPIO_PIN2)

/*********************************************************************/
// mems
#define SSPI_MOSI_PORT (GPIO_PORTJ)
#define SSPI_MOSI_PIN  (GPIO_PIN0)
#define SSPI_MISO_PORT (GPIO_PORTI)
#define SSPI_MISO_PIN  (GPIO_PIN0)
#define SSPI_SCL_PORT  (GPIO_PORTH)
#define SSPI_SCL_PIN   (GPIO_PIN7)
#define SSPI_CS_PORT   (GPIO_PORTH)
#define SSPI_CS_PIN    (GPIO_PIN6)

#define SSPI_ON_OFF_PORT (GPIO_PORTH)
#define SSPI_ON_OFF_PIN  (GPIO_PIN5)

/*********************************************************************/
// uart
#define UART_0_TX_PORT (GPIO_PORTE)
#define UART_0_TX_PIN  (GPIO_PIN0)
#define UART_0_RX_PORT (GPIO_PORTE)
#define UART_0_RX_PIN  (GPIO_PIN1)

#define UART_1_TX_PORT (GPIO_PORTA)
#define UART_1_TX_PIN  (GPIO_PIN5)
#define UART_1_RX_PORT (GPIO_PORTA)
#define UART_1_RX_PIN  (GPIO_PIN6)

#define UART_2_TX_PORT (GPIO_PORTD)
#define UART_2_TX_PIN  (GPIO_PIN5)
#define UART_2_RX_PORT (GPIO_PORTD)
#define UART_2_RX_PIN  (GPIO_PIN6)

#define UART_3_TX_PORT (GPIO_PORTF)
#define UART_3_TX_PIN  (GPIO_PIN3)
#define UART_3_RX_PORT (GPIO_PORTF)
#define UART_3_RX_PIN  (GPIO_PIN4)

/*********************************************************************/
// keypad pins
#define KEYPAD_COL_0_PORT ()
#define KEYPAD_COL_0_PIN  ()
#define KEYPAD_COL_1_PORT ()
#define KEYPAD_COL_1_PIN  ()
#define KEYPAD_COL_2_PORT ()
#define KEYPAD_COL_2_PIN  ()
#define KEYPAD_COL_3_PORT ()
#define KEYPAD_COL_3_PIN  ()

#define KEYPAD_ROW_0_PORT ()
#define KEYPAD_ROW_0_PIN  ()
#define KEYPAD_ROW_1_PORT ()
#define KEYPAD_ROW_1_PIN  ()
#define KEYPAD_ROW_2_PORT ()
#define KEYPAD_ROW_2_PIN  ()
#define KEYPAD_ROW_3_PORT
#define KEYPAD_ROW_3_PIN

/*********************************************************************/
// led matrix
#define LED_MATRIX_COL_0_PORT ()
#define LED_MATRIX_COL_0_PIN  ()
#define LED_MATRIX_COL_1_PORT ()
#define LED_MATRIX_COL_1_PIN  ()
#define LED_MATRIX_COL_2_PORT ()
#define LED_MATRIX_COL_2_PIN  ()
#define LED_MATRIX_COL_3_PORT ()
#define LED_MATRIX_COL_3_PIN  ()

#define LED_MATRIX_ROW_0_PORT ()
#define LED_MATRIX_ROW_0_PIN  ()
#define LED_MATRIX_ROW_1_PORT ()
#define LED_MATRIX_ROW_1_PIN  ()
#define LED_MATRIX_ROW_2_PORT ()
#define LED_MATRIX_ROW_2_PIN  ()
#define LED_MATRIX_ROW_3_PORT ()
#define LED_MATRIX_ROW_3_PIN  ()
#define LED_MATRIX_ROW_4_PORT ()
#define LED_MATRIX_ROW_4_PIN  ()
#define LED_MATRIX_ROW_5_PORT ()
#define LED_MATRIX_ROW_5_PIN  ()
#define LED_MATRIX_ROW_6_PORT ()
#define LED_MATRIX_ROW_6_PIN  ()
#define LED_MATRIX_ROW_7_PORT ()
#define LED_MATRIX_ROW_7_PIN  ()

/*********************************************************************/
// ssd
#define SSD_DIGIT_1_PORT ()
#define SSD_DIGIT_1_PIN  ()
#define SSD_DIGIT_0_PORT ()
#define SSD_DIGIT_0_PIN  ()
#define SSD_DIGIT_3_PORT ()
#define SSD_DIGIT_3_PIN  ()
#define SSD_DIGIT_2_PORT ()
#define SSD_DIGIT_2_PIN  ()

#define SSD_DATA_A_PORT  ()
#define SSD_DATA_A_PIN   ()
#define SSD_DATA_B_PORT  ()
#define SSD_DATA_B_PIN   ()
#define SSD_DATA_C_PORT  ()
#define SSD_DATA_C_PIN   ()
#define SSD_DATA_D_PORT  ()
#define SSD_DATA_D_PIN   ()
#define SSD_DATA_E_PORT  ()
#define SSD_DATA_E_PIN   ()
#define SSD_DATA_F_PORT  ()
#define SSD_DATA_F_PIN   ()
#define SSD_DATA_G_PORT  ()
#define SSD_DATA_G_PIN   ()
#define SSD_DATA_DP_PORT ()
#define SSD_DATA_DP_PIN  ()

/*********************************************************************/

#elif DDM_MODEL == DDM_INTERNAL

/*********************************************************************/
// heartbeat
#define HEARTBEAT_LED_PORT       ()
#define HEARTBEAT_LED_PIN        ()
#define HEARTBEAT_LED_INIT_STATE ()

/*********************************************************************/
// dinput
#define POWER_OFF_SW_PORT ()
#define POWER_OFF_SW_PIN  ()

#define LOCK_MOTOR_SENSOR_PORT (GPIO_PORTE) 
#define LOCK_MOTOR_SENSOR_PIN  (GPIO_PIN7)

#define LID_SENSOR_PORT (GPIO_PORTJ)
#define LID_SENSOR_PIN  (GPIO_PIN3)

#define LINT_FILTER_PORT ()
#define LINT_FILTER_PIN  ()

#define DETERGENT_LIMIT_PORT (GPIO_PORTH)
#define DETERGENT_LIMIT_PIN  (GPIO_PIN4)

#define SOFTNER_LIMIT_PORT (GPIO_PORTH)
#define SOFTNER_LIMIT_PIN  (GPIO_PIN3)

// external interrupt
#define ZCD_PORT (GPIO_PORTH)
#define ZCD_PIN  (GPIO_PIN1)
#define ZCD_IRQ  (MCAL_EXTI_IRQ1)

/*********************************************************************/
// doutput
#define DRAIN_PUMP_PORT (GPIO_PORTG)
#define DRAIN_PUMP_PIN  (GPIO_PIN0)

#define DETERGENT_PUMP_PORT (GPIO_PORTG) 
#define DETERGENT_PUMP_PIN  (GPIO_PIN4)

#define SOFTNER_PUMP_PORT (GPIO_PORTE)  
#define SOFTNER_PUMP_PIN  (GPIO_PIN6)


#define DRAIN_VALVE_PORT ()
#define DRAIN_VALVE_PIN  ()

#define CLUTCH_PORT (GPIO_PORTA)   
#define CLUTCH_PIN  (GPIO_PIN3)

#define LOCK_MOTOR_PORT (GPIO_PORTG)  
#define LOCK_MOTOR_PIN  (GPIO_PIN3)

#define COLD_WATER_VALVE_PORT (GPIO_PORTA)  
#define COLD_WATER_VALVE_PIN  (GPIO_PIN6)

#define HOT_WATER_VALVE_PORT (GPIO_PORTA)  
#define HOT_WATER_VALVE_PIN  (GPIO_PIN7)

#define SOFTENER_WATER_VALVE_PORT (GPIO_PORTA) 
#define SOFTENER_WATER_VALVE_PIN  (GPIO_PIN4)

#define SHOWER_WATER_VALVE_PORT (GPIO_PORTA)  
#define SHOWER_WATER_VALVE_PIN  (GPIO_PIN5)

#define CONDENSER_VALVE_PORT ()
#define CONDENSER_VALVE_PIN  ()

#define RELAY_PORT (GPIO_PORTE)  
#define RELAY_PIN  (GPIO_PIN3)

#define NEDIC_RELAY_PORT ()
#define NEDIC_RELAY_PIN  ()

// NOTE: not used from the doutputmodule
#define BUZZER_ENABLE_PORT (GPIO_PORTF)
#define BUZZER_ENABLE_PIN  (GPIO_PIN2)

#define DISPLAY_RESET_SIGNAL_PORT (GPIO_PORTA)
#define DISPLAY_RESET_SIGNAL_PIN  (GPIO_PIN1)   

#define HEATER_PLATE_1_PORT (GPIO_PORTE)
#define HEATER_PLATE_1_PIN  (GPIO_PIN4)

#define HEATER_PLATE_2_PORT (GPIO_PORTE) 
#define HEATER_PLATE_2_PIN  (GPIO_PIN2)

#define COOLING_FAN_LOW_PORT ()
#define COOLING_FAN_LOW_PIN  ()

#define COOLING_FAN_HIGH_PORT ()
#define COOLING_FAN_HIGH_PIN  ()

/*********************************************************************/
// internal timers
#define ACIN_TIME_TIMER (MCAL_TIMER_6)

/*********************************************************************/
// analog inputs
// #define ACIN_ADC_PORT (GPIO_PORTI)
// #define ACIN_ADC_PIN  (GPIO_PIN0)
// #define ACIN_ADC_BLK  (MCAL_ADC_BLK_A)
// #define ACIN_ADC_CH   (MCAL_ADC_CH8)

#define AMBIENT_TEMP_SENSOR_PORT    ()
#define AMBIENT_TEMP_SENSOR_PIN     ()
#define AMBIENT_TEMP_SENSOR_ADC_BLK ()
#define AMBIENT_TEMP_SENSOR_ADC_CH  ()

#define WATER_TEMP_SENSOR_PORT    (GPIO_PORTJ)
#define WATER_TEMP_SENSOR_PIN     (GPIO_PIN5)
#define WATER_TEMP_SENSOR_ADC_BLK (MCAL_ADC_BLK_B)
#define WATER_TEMP_SENSOR_ADC_CH  (MCAL_ADC_CH8)

#define AIR_TEMP_SENSOR_A_PORT    ()
#define AIR_TEMP_SENSOR_A_PIN     ()
#define AIR_TEMP_SENSOR_A_ADC_BLK ()
#define AIR_TEMP_SENSOR_A_ADC_CH  ()

#define AIR_TEMP_SENSOR_B_PORT    ()
#define AIR_TEMP_SENSOR_B_PIN     ()
#define AIR_TEMP_SENSOR_B_ADC_BLK ()
#define AIR_TEMP_SENSOR_B_ADC_CH  ()

#define HEATER_SENSOR_PORT    (GPIO_PORTJ)
#define HEATER_SENSOR_PIN     (GPIO_PIN4)
#define HEATER_SENSOR_ADC_BLK (MCAL_ADC_BLK_B)
#define HEATER_SENSOR_ADC_CH  (MCAL_ADC_CH7)

/*********************************************************************/
// freq inputs
#define WATER_LEVEL_PORT           (GPIO_PORTA)
#define WATER_LEVEL_PIN            (GPIO_PIN0)
#define WATER_LEVEL_CHANNEL        (FINPUT_MODULE_0)
#define WATER_LEVEL_DETECTION_EDGE (FINPUT_MODULE_EDGE_RISING)

#define DRAIN_VALVE_SENSOR_PORT           ()
#define DRAIN_VALVE_SENSOR_PIN            ()
#define DRAIN_VALVE_SENSOR_CHANNEL        ()
#define DRAIN_VALVE_SENSOR_DETECTION_EDGE ()

#define CLUTCH_SENSOR_PORT           (GPIO_PORTA)            
#define CLUTCH_SENSOR_PIN            (GPIO_PIN2)
#define CLUTCH_SENSOR_CHANNEL        (FINPUT_MODULE_1)
#define CLUTCH_SENSOR_DETECTION_EDGE (FINPUT_MODULE_EDGE_RISING)

#define BLOWER_FEEDBACK_PORT           ()
#define BLOWER_FEEDBACK_PIN            ()
#define BLOWER_FEEDBACK_CHANNEL        ()
#define BLOWER_FEEDBACK_DETECTION_EDGE ()

#define COOLING_FAN_FEEDBACK_PORT           ()
#define COOLING_FAN_FEEDBACK_PIN            ()
#define COOLING_FAN_FEEDBACK_CHANNEL        ()
#define COOLING_FAN_FEEDBACK_DETECTION_EDGE ()

/*********************************************************************/
// freq outputs
#define BUZZER_FREQ_PORT (GPIO_PORTF)
#define BUZZER_FREQ_PIN  (GPIO_PIN1)
#define BUZZER_CHANNEL   (FOUTPUT_MODULE_7)

#define BLOWER_CONTROL_PORT    ()
#define BLOWER_CONTROL_PIN     ()
#define BLOWER_CONTROL_CHANNEL ()

/*********************************************************************/
// stwi
#define EEPROM_ADDRESS      (0xA0)
#define EEPROM_TIMER_MODULE (HW_TIMER_3)

#define TWI_DATA_PORT (GPIO_PORTD)
#define TWI_DATA_PIN  (GPIO_PIN3)

#define TWI_CLOCK_PORT (GPIO_PORTD)
#define TWI_CLOCK_PIN  (GPIO_PIN2)

/*********************************************************************/
// mems
#define SSPI_MOSI_PORT (GPIO_PORTJ)
#define SSPI_MOSI_PIN  (GPIO_PIN0)
#define SSPI_MISO_PORT (GPIO_PORTI)
#define SSPI_MISO_PIN  (GPIO_PIN0)
#define SSPI_SCL_PORT  (GPIO_PORTH)
#define SSPI_SCL_PIN   (GPIO_PIN7)
#define SSPI_CS_PORT   (GPIO_PORTH)
#define SSPI_CS_PIN    (GPIO_PIN6)

#define SSPI_ON_OFF_PORT (GPIO_PORTH)
#define SSPI_ON_OFF_PIN  (GPIO_PIN5)

/*********************************************************************/
// uart
#define UART_0_TX_PORT (GPIO_PORTE)   
#define UART_0_TX_PIN  (GPIO_PIN0)
#define UART_0_RX_PORT (GPIO_PORTE)
#define UART_0_RX_PIN  (GPIO_PIN1)

#define UART_1_TX_PORT (GPIO_PORTA)  
#define UART_1_TX_PIN  (GPIO_PIN5)
#define UART_1_RX_PORT (GPIO_PORTA)
#define UART_1_RX_PIN  (GPIO_PIN6)

#define UART_2_TX_PORT (GPIO_PORTD)
#define UART_2_TX_PIN  (GPIO_PIN5)
#define UART_2_RX_PORT (GPIO_PORTD)
#define UART_2_RX_PIN  (GPIO_PIN6)

#define UART_3_TX_PORT (GPIO_PORTF)
#define UART_3_TX_PIN  (GPIO_PIN3)
#define UART_3_RX_PORT (GPIO_PORTF)
#define UART_3_RX_PIN  (GPIO_PIN4)

/*********************************************************************/
// keypad pins
#define KEYPAD_COL_0_PORT ()
#define KEYPAD_COL_0_PIN  ()
#define KEYPAD_COL_1_PORT ()
#define KEYPAD_COL_1_PIN  ()
#define KEYPAD_COL_2_PORT ()
#define KEYPAD_COL_2_PIN  ()
#define KEYPAD_COL_3_PORT ()
#define KEYPAD_COL_3_PIN  ()

#define KEYPAD_ROW_0_PORT ()
#define KEYPAD_ROW_0_PIN  ()
#define KEYPAD_ROW_1_PORT ()
#define KEYPAD_ROW_1_PIN  ()
#define KEYPAD_ROW_2_PORT ()
#define KEYPAD_ROW_2_PIN  ()
#define KEYPAD_ROW_3_PORT
#define KEYPAD_ROW_3_PIN

/*********************************************************************/
// led matrix
#define LED_MATRIX_COL_0_PORT ()
#define LED_MATRIX_COL_0_PIN  ()
#define LED_MATRIX_COL_1_PORT ()
#define LED_MATRIX_COL_1_PIN  ()
#define LED_MATRIX_COL_2_PORT ()
#define LED_MATRIX_COL_2_PIN  ()
#define LED_MATRIX_COL_3_PORT ()
#define LED_MATRIX_COL_3_PIN  ()

#define LED_MATRIX_ROW_0_PORT ()
#define LED_MATRIX_ROW_0_PIN  ()
#define LED_MATRIX_ROW_1_PORT ()
#define LED_MATRIX_ROW_1_PIN  ()
#define LED_MATRIX_ROW_2_PORT ()
#define LED_MATRIX_ROW_2_PIN  ()
#define LED_MATRIX_ROW_3_PORT ()
#define LED_MATRIX_ROW_3_PIN  ()
#define LED_MATRIX_ROW_4_PORT ()
#define LED_MATRIX_ROW_4_PIN  ()
#define LED_MATRIX_ROW_5_PORT ()
#define LED_MATRIX_ROW_5_PIN  ()
#define LED_MATRIX_ROW_6_PORT ()
#define LED_MATRIX_ROW_6_PIN  ()
#define LED_MATRIX_ROW_7_PORT ()
#define LED_MATRIX_ROW_7_PIN  ()

/*********************************************************************/
// ssd
#define SSD_DIGIT_1_PORT ()
#define SSD_DIGIT_1_PIN  ()
#define SSD_DIGIT_0_PORT ()
#define SSD_DIGIT_0_PIN  ()
#define SSD_DIGIT_3_PORT ()
#define SSD_DIGIT_3_PIN  ()
#define SSD_DIGIT_2_PORT ()
#define SSD_DIGIT_2_PIN  ()

#define SSD_DATA_A_PORT  ()
#define SSD_DATA_A_PIN   ()
#define SSD_DATA_B_PORT  ()
#define SSD_DATA_B_PIN   ()
#define SSD_DATA_C_PORT  ()
#define SSD_DATA_C_PIN   ()
#define SSD_DATA_D_PORT  ()
#define SSD_DATA_D_PIN   ()
#define SSD_DATA_E_PORT  ()
#define SSD_DATA_E_PIN   ()
#define SSD_DATA_F_PORT  ()
#define SSD_DATA_F_PIN   ()
#define SSD_DATA_G_PORT  ()
#define SSD_DATA_G_PIN   ()
#define SSD_DATA_DP_PORT ()
#define SSD_DATA_DP_PIN  ()

/*********************************************************************/

#endif

#endif
