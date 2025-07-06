#include "def.h"

#include "app_fault_display.h"
#include "app_display.h"
#include "app_nm_log.h"

#include "ssd/ssd.h"

static void display_faultState_process(uint32_t period);

static void display_currentError_put(uint8_t error);
static void display_ssd_blink(uint32_t period);
static void display_error_put(ssd_symbolEnum_t ssd1, ssd_symbolEnum_t ssd2, ssd_symbolEnum_t ssd3, ssd_symbolEnum_t ssd4);

static ssd_symbolEnum_t u8_ssd0;
static ssd_symbolEnum_t u8_ssd1;
static ssd_symbolEnum_t u8_ssd2;
static ssd_symbolEnum_t u8_ssd3;

void app_fault_display_process(uint32_t period)
{
    display_faultState_process(period);
}

static void display_faultState_process(uint32_t period)
{
    display_currentError_put(app_nm_log_error_get());
    display_ssd_blink(period);
}

static void display_currentError_put(uint8_t error)
{
    switch (error)
    {
        case APP_NORMAL_MODE_LOG_ERROR_DRAIN_FAILURE:
        {
            display_error_put(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_E_SYMBOL, SSD_1_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_DRAIN_FILTER_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_1_SYMBOL, SSD_CENTER_DASH, SSD_1_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_LID_OPENING_FAILURE:
        {
            display_error_put(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_E_SYMBOL, SSD_2_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_LID_LOCK_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_2_SYMBOL, SSD_CENTER_DASH, SSD_1_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_MEMS_SENSOR_FAILURE:
        {
            display_error_put(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_E_SYMBOL, SSD_3_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_DRAIN_VALVE_MOTOR_FAILURE:
        {
            display_error_put(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_E_SYMBOL, SSD_4_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_ABNORMAL_WATER_LEVEL_SENSOR_VALUE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_5_SYMBOL, SSD_CENTER_DASH, SSD_1_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_MOTOR_CW_ROTATION:
        {
            display_error_put(SSD_E_SYMBOL, SSD_7_SYMBOL, SSD_CENTER_DASH, SSD_1_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_MOTOR_CCW_ROTATION:
        {
            display_error_put(SSD_E_SYMBOL, SSD_7_SYMBOL, SSD_CENTER_DASH, SSD_2_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_MOTOR_SPIN_ROTATION:
        {
            display_error_put(SSD_E_SYMBOL, SSD_7_SYMBOL, SSD_CENTER_DASH, SSD_3_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_MOTOR_CW_CCW_ROTATION:
        {
            display_error_put(SSD_E_SYMBOL, SSD_7_SYMBOL, SSD_CENTER_DASH, SSD_4_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_MOTOR_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_d_SYMBOL, SSD_CENTER_DASH, SSD_1_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_DRIVER_IPM_FAILRUE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_d_SYMBOL, SSD_CENTER_DASH, SSD_2_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_MOTOR_VOLTAGE_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_d_SYMBOL, SSD_CENTER_DASH, SSD_3_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_DRIVER_NO_COMMUNICATION:
        {
            display_error_put(SSD_E_SYMBOL, SSD_d_SYMBOL, SSD_CENTER_DASH, SSD_c_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_CRC_MISMATCH_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_c_SYMBOL, SSD_r_SYMBOL, SSD_c_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_ABNORMAL_WATER_WHEN_DRY:
        {
            display_error_put(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_E_SYMBOL, SSD_A_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_DISPLAY_DISCONNECTED_FAILURE:
        {
            display_error_put(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_E_SYMBOL, SSD_P_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_LOW_POWER_FAILURE:
        {
            display_error_put(SSD_NULL_SYMBOL, SSD_E_SYMBOL, SSD_L_SYMBOL, SSD_P_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_ABNORMAL_THERMISTOR_VALUE:
        {
            display_error_put(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_E_SYMBOL, SSD_t_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_LINT_FILTER_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_u_SYMBOL, SSD_CENTER_DASH, SSD_1_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_LOG_ERROR_OVER_LOADING_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_u_SYMBOL, SSD_CENTER_DASH, SSD_2_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_WASH_LOG_ERROR_WATER_SUPPLY_FAILURE:
        {
            display_error_put(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_E_SYMBOL, SSD_5_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_WASH_LOG_ERROR_UNBALANCE_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_3_SYMBOL, SSD_CENTER_DASH, SSD_2_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_WASH_LOG_ERROR_ABNORMAL_WATER_SUPPLY_OVERFLOW:
        {
            display_error_put(SSD_E_SYMBOL, SSD_6_SYMBOL, SSD_CENTER_DASH, SSD_1_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_WASH_LOG_ERROR_WATER_SUPPLY_CONNECTION_FAILURE:
        {
            display_error_put(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_E_SYMBOL, SSD_H_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_WASH_LOG_ERROR_ABNORMAL_WATER_LEAKAGE:
        {
            display_error_put(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_E_SYMBOL, SSD_9_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_WASH_LOG_ERROR_FILL_HIGH_TEMP_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_F_SYMBOL, SSD_CENTER_DASH, SSD_1_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_DRY_LOG_ERROR_BLOWER_NOT_OPERATING_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_u_SYMBOL, SSD_CENTER_DASH, SSD_2_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_DRY_LOG_ERROR_BLOWER_OFF_HEATER_ON_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_u_SYMBOL, SSD_CENTER_DASH, SSD_3_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_DRY_LOG_ERROR_HEATER_OVERCURRENT_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_u_SYMBOL, SSD_CENTER_DASH, SSD_4_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_DRY_LOG_ERROR_HEATER_NOT_OPERATING_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_u_SYMBOL, SSD_CENTER_DASH, SSD_5_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_DRY_LOG_ERROR_HEATER_SHOULDNOT_OPERATING_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_u_SYMBOL, SSD_CENTER_DASH, SSD_6_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_DRY_LOG_ERROR_HEATER_TEMP_SENSOR_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_u_SYMBOL, SSD_CENTER_DASH, SSD_7_SYMBOL);
        }
        break;

        case APP_NORMAL_MODE_DRY_LOG_ERROR_LINT_FILTER_NOT_FOUND_FAILURE:
        {
            display_error_put(SSD_E_SYMBOL, SSD_u_SYMBOL, SSD_CENTER_DASH, SSD_8_SYMBOL);
        }
        break;

        default:
        {
            display_error_put(SSD_E_SYMBOL, SSD_r_SYMBOL, SSD_r_SYMBOL, SSD_r_SYMBOL);
        }
        break;
    }
}

static void display_ssd_blink(uint32_t period)
{
    static uint32_t u16_errorTimeout = 0;
    static uint8_t u8_ssdCurrentState = 0;

    u16_errorTimeout += period;
    if (u16_errorTimeout >= 500U)
    {
        u16_errorTimeout = 0;
        if (u8_ssdCurrentState == 0U)
        {
            u8_ssdCurrentState = 1;
        }
        else
        {
            u8_ssdCurrentState = 0;
        }
    }

    switch (u8_ssdCurrentState)
    {
        case 0:
            handler_display_ssd_set(SSD_DIGIT_0_ID, u8_ssd0);
            handler_display_ssd_set(SSD_DIGIT_1_ID, u8_ssd1);
            handler_display_ssd_set(SSD_DIGIT_2_ID, u8_ssd2);
            handler_display_ssd_set(SSD_DIGIT_3_ID, u8_ssd3);
            break;

        case 1:
            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_NULL_SYMBOL);
            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
            break;

        default:
            break;
    }
}

static void display_error_put(ssd_symbolEnum_t ssd1, ssd_symbolEnum_t ssd2, ssd_symbolEnum_t ssd3, ssd_symbolEnum_t ssd4)
{
    u8_ssd0 = ssd4;
    u8_ssd1 = ssd3;
    u8_ssd2 = ssd2;
    u8_ssd3 = ssd1;

    handler_display_ssd_clear();
    handler_display_clear();

    handler_display_ssd_set(SSD_DIGIT_0_ID, u8_ssd0);
    handler_display_ssd_set(SSD_DIGIT_1_ID, u8_ssd1);
    handler_display_ssd_set(SSD_DIGIT_2_ID, u8_ssd2);
    handler_display_ssd_set(SSD_DIGIT_3_ID, u8_ssd3);
}
