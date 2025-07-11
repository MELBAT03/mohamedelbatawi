#ifndef COMPONENT_CONFIG_H
#define COMPONENT_CONFIG_H

#define EXTERNAL_EEPROM_ENABLE

#define STWI_ENABLE

#define WDT_OUTPUT     (WDT_OUTPUT_RESET)
#define WDT_RESET_MSEC (WDT_TIMEOUT_LV4)

#define QE_TOUCH_VERSION_NONE (0U)
#define QE_TOUCH_VERSION_1    (1U)
#define QE_TOUCH_VERSION_3    (3U)
#define QE_TOUCH_VERSION      (QE_TOUCH_VERSION_NONE)

#define ACIN_METHOD_TIME (0U)
#define ACIN_METHOD_ADC  (1U)
#define ACIN_TYPE        ACIN_METHOD_TIME

#define MEMORY_SIZE (1024U)
// #define DMEM_RECORD_NUM (106U)
#define TWI_EEPROM_PAGE_SIZE (16U)
#define TWI_EEPROM_SPEED     (300000U)

#define DINPUT_MODULE_PRESSED_LEVEL  (0U)
#define DINPUT_MODULE_RELEASED_LEVEL (1U)
#define DINPUT_MODULE_DEVICE_NUM     (5U)

#define DOUTPUT_MODULE_LOW_LEVEL  (0U)
#define DOUTPUT_MODULE_HIGH_LEVEL (1U)
#define DOUTPUT_MODULE_DEVICE_NUM (15U)

#define FOUTPUT_MODULE_DEVCIES_NUM (0U)

#define FINPUT_MODULE_DEVICES_NUM (2U)

#define AINPUT_MODULE_DEVICES_NUM (6U)

// hm14/15n
#define HM15N_MODULE_DEVICE_NUM (1U)
// #define HM14N_MODULE_DEVICE_NUM (2U)

// ddm motor
#define DDM_INTERNAL (0U)
#define DDM_EXTERNAL (1U)
#define DDM_MODEL    (DDM_INTERNAL)

#define DDM_MOTOR_MAX_RPM     (700U)
#define DDM_MOTOR_SPEED_LIMIT (700U)
#if DDM_MODEL == DDM_EXTERNAL
#define DDM_CRC_CCITT
#elif DDM_MODEL == DDM_INTERNAL
#define DDM_CRC_XOR
#endif

// uart
#define UART_RX_INT                (1U)
#define UART_RECV_BUFFER_SIZE      (30U)
#define UART_TRANS_BUFFER_SIZE     (30U)
#define UART_LOG_RECV_BUFFER_SIZE  (30U)
#define UART_LOG_TRANS_BUFFER_SIZE (300U)
#define LOG_CHANNEL_ID             (UART_3)
#define MOTOR_COM_PORT_ID          (1U)
#define DISPLAY_PORT_ID            (2U)
#define UART_INTERFACES_NO_LOG_NUM (2U)

// led matrix
#define LED_MATRIX_COL_NUMBER  (12U)
#define LED_MATRIX_ROW_NUMBER  (13U)
#define LED_MATRIX_COLOUMN_ON  (1U)
#define LED_MATRIX_COLOUMN_OFF (0U)
#define LED_MATRIX_ON          (1U)
#define LED_MATRIX_OFF         (0U)
#define LED_MATRIX_SSD_DP_OFF  (0U)
#define LED_MATRIX_SSD_DP_ON   (1U)
#define LED_MATRIX_LED_NUM     (55U)

// keypad
#define KEYPAD_KEY_NUM         (25U)
#define KEYPAD_EMBEDDED_MATRIX (0U)
#define KEYPAD_COL_NUMBER      (1U)
#define KEYPAD_ROW_NUMBER      (KEYPAD_KEY_NUM)
#define KEYPAD_PRESSED_LEVEL   (1U)
#define KEYPAD_RELEASED_LEVEL  (0U)

// ssd
#define SSD_DATA_INVERTED (1U)
#define SSD_DATA_PINS     (7U)
#define SSD_COM_ANODE     (1U)
#define SSD_COM_CATHODE   (0U)
#define SSD_COM_TYPE      (SSD_COM_ANODE)
#define SSD_ON            (1U)
#define SSD_OFF           (0U)
#define SSD_DP_ON         (1U)
#define SSD_DP_OFF        (0U)
#define SSD_DIGITS_NUM    (4U)

// ptc
#define PTC_HEATER_PLATE_NUMBER (2U)

// ntc
#define NTC_NUMBER_OF_FILTER_SAMPLES (10U)
#define NTC_SENSOR_NUM               (1U)

// com_base
#define HEADER_VALUE    (0xA0U)
#define HEADER_SIZE     (1U)
#define SEQ_NUM_SIZE    (1U)
#define DIM_ENABLE_SIZE (1U)
#define CRC_SIZE        (1U)

#define COM_BASE_SSD_BYTE_NUM           (5U)
#define COM_BASE_SSD_NUM                (4U)
#define COM_BASE_LED_NUM                (55U)
#define COM_BASE_LED_BYTE_NUM           (7U)
#define COM_BASE_HEADER_BYTE_OFFSET     (0U)
#define COM_BASE_SEQ_NUM_BYTE_OFFSET    (1U)
#define COM_BASE_DIM_ENABLE_BYTE_OFFSET (2U)
#define COM_BASE_SSD_BYTE_OFFSET        (3U)

#define COM_BASE_LED_BYTE_OFFSET ((COM_BASE_SSD_BYTE_OFFSET) + (COM_BASE_SSD_BYTE_NUM))

#define COM_BASE_PACKET_SIZE      (HEADER_SIZE + SEQ_NUM_SIZE + DIM_ENABLE_SIZE + COM_BASE_SSD_BYTE_NUM + COM_BASE_LED_BYTE_NUM + CRC_SIZE)
#define COM_BASE_DATA_PACKET_SIZE ((COM_BASE_PACKET_SIZE) - (HEADER_SIZE) - (SEQ_NUM_SIZE) - (CRC_SIZE))

#define COM_BASE_CRC_BYTE_OFFSET ((COM_BASE_LED_BYTE_OFFSET) + (COM_BASE_LED_BYTE_NUM))

// com_display
#define COM_DISPLAY_DATA_PACKET_OFFSET  (2U)
#define COM_DISPLAY_KEY_NUM             (24U)
#define COM_DISPLAY_KEY_BYTE_NUM        (3U)
#define COM_DISPLAY_HEADER_BYTE_OFFSET  (0U)
#define COM_DISPLAY_SEQ_NUM_BYTE_OFFSET (1U)
#define COM_DISPLAY_KEYPAD_BYTE_OFFSET  (2U)

#define VERSION_SIZE (1U)

#define COM_DISPLAY_PACKET_SIZE (HEADER_SIZE + SEQ_NUM_SIZE + COM_DISPLAY_KEY_BYTE_NUM + VERSION_SIZE + CRC_SIZE)

#define COM_DISPLAY_VERSION_BYTE_OFFSET ((COM_DISPLAY_KEYPAD_BYTE_OFFSET) + (COM_DISPLAY_KEY_BYTE_NUM))

#define COM_DISPLAY_CRC_BYTE_OFFSET ((COM_DISPLAY_VERSION_BYTE_OFFSET) + 1)

#endif
