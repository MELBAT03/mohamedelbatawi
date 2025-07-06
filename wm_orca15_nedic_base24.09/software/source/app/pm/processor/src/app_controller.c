#include "component_config.h"
#include "def.h"
#include "utils.h"
#include "app_controller.h"
#include "app_processor.h"
#include "app_unbalance.h"
#include "components/uart/uart.h"

#include "app_user_interface.h"

#ifdef COOLING_FAN_ENABLED
#include "handlers/output/inc/handler_cooling_fan.h"
#endif

#include "handlers/input/inc/handler_mems.h"
#include "handlers/output/inc/handler_water_valve.h"
#include "handlers/output/inc/handler_clutch.h"
#include "handlers/output/inc/handler_heater.h"
#include "handlers/output/inc/handler_lock_motor.h"
#include "handlers/output/inc/handler_drain_valve.h"
#include "handlers/output/inc/handler_drain_pump.h"
#include "handlers/input/inc/handler_power_detection.h"
#include "handlers/input/inc/handler_keypad.h"
#include "handlers/input/inc/handler_off_sw.h"
#include "handlers/output/inc/handler_detergent_pump.h"
#include "handlers/output/inc/handler_softener_pump.h"
#ifdef LINT_FILTER_ENABLED
#include "handlers/input/inc/handler_lint_filter_sensor.h"
#endif

#ifdef BLOWER_ENABLED
#include "handlers/output/inc/handler_blower.h"
#endif

#include "os.h"

typedef enum app_controller_mode
{
    APP_CONTROLLER_NONE,
    APP_CONTROLLER_USER_MODE,
    APP_CONTROLLER_OVERRIDE_MODE
} app_controller_mode_t;

static void controller_overrideMode_process(void);
static void controller_userMode_process(void);

static uint8_t controller_string_compare(char *gbuffer, uint8_t size, const char *string);

static void keypad_keyState_register(keyID_t key, uint8_t state, uint32_t timeout);
static void keypad_state_update(uint32_t period);

static uint32_t keyState_internalTimeoutMSec[KEYPAD_KEY_NUM];
static uint8_t keyState_state[KEYPAD_KEY_NUM];
static keyID_t keyState_keyID[KEYPAD_KEY_NUM];

static char gbuffer[15] = {0};

static app_controller_mode_t gx_app_controller_mode = APP_CONTROLLER_NONE;

void app_controller_update(uint32_t period)
{
    static uint8_t rxCounter = 0;

    keypad_state_update(period);

    // NOTE: format /O : Override Mode   xx:yy : override setting
    // NOTE: format /U : User Mode       xx:yy : user selections
    // NOTE: format OFF                       : off
    // NOTE: format SP                        : start/pause
    // NOTE: format A                         : LOG
    if (uart_recv_check(LOG_CHANNEL_ID) == TRUE)
    {
        gbuffer[rxCounter] = uart_byte_get(LOG_CHANNEL_ID);

        if (gbuffer[rxCounter] == '\n' || gbuffer[rxCounter] == '\r' || gbuffer[rxCounter] == '\0')
        {
            // printf("\n\rReceived Count %d -->   ", rxCounter);
            // for (uint8_t i = 0; i < rxCounter; i++)
            // {
            //     printf("%c ", gbuffer[i]);
            // }
            LOG_CC(0, 'i', gbuffer, rxCounter);

            if (rxCounter == 1)
            {
                os_log_groupID_set(gbuffer[0]);
            }
            else if (gbuffer[0] == 'P' && gbuffer[1] == 'I')
            {
                handler_powerDetection_state_set(HANDLER_POWER_DETECTION_NOK);
            }
            else if (gbuffer[0] == 'O' && gbuffer[1] == 'N')
            {
                // keypad_keyState_register(KEY_ON_OFF_ID, 1, 100);
            }
            else if (gbuffer[0] == 'S' && gbuffer[1] == 'P')
            {
                handler_keypad_edgeState_set(KEY_START_PAUSE_ID, 1);
            }
            else if (gbuffer[0] == '0' && gbuffer[1] == '0')
            {
                NVIC_SystemReset();   // used for quick system reset instead of powering off during development
            }
            else if (gbuffer[0] == 'O' && gbuffer[1] == 'F')
            {
                // handler_keypad_state_set(KEY_ON_OFF_ID, 1);
                handler_offSW_state_set((handler_offSW_state_t)1);
            }
            else if (gbuffer[0] == '/' && gbuffer[1] == 'O')
            {
                gx_app_controller_mode = APP_CONTROLLER_OVERRIDE_MODE;
                LOG('0', 'i', "Override Mode!!");
            }
            else if (0 != controller_string_compare(&gbuffer[0], 3, "MX:"))
            {
                gbuffer[rxCounter] = 0;   // to replace /n with /0 for atoi to work correctly
                handler_mems_xThreshold_set(utils_atoi(&gbuffer[3]));
            }
            else if (0 != controller_string_compare(&gbuffer[0], 3, "MY:"))
            {
                gbuffer[rxCounter] = 0;   // to replace /n with /0 for atoi to work correctly
                handler_mems_yThreshold_set(utils_atoi(&gbuffer[3]));
            }
            else if (0 != controller_string_compare(&gbuffer[0], 3, "MZ:"))
            {
                gbuffer[rxCounter] = 0;   // to replace /n with /0 for atoi to work correctly
                handler_mems_zThreshold_set(utils_atoi(&gbuffer[3]));
            }
            else if (0 != controller_string_compare(&gbuffer[0], 3, "MN:"))
            {
                gbuffer[rxCounter] = 0;   // to replace /n with /0 for atoi to work correctly
                handler_mems_hitCnt_set(utils_atoi(&gbuffer[3]));
            }
            else if (0 != controller_string_compare(&gbuffer[0], 3, "MC:"))
            {
                gbuffer[rxCounter] = 0;   // to replace /n with /0 for atoi to work correctly
                handler_mems_critHitCnt_set(utils_atoi(&gbuffer[3]));
            }
            else if (0 != controller_string_compare(&gbuffer[0], 4, "HET:"))
            {
                if (gbuffer[4] == '0')
                {
                    handler_heater_plate_set(HANDLER_HEATER_OFF);
                }
                else
                {
                    handler_heater_plate_set(HANDLER_HEATER_PLATE_2);
                }
            }
            else if (0 != controller_string_compare(&gbuffer[0], 4, "DRP:"))
            {
                if (gbuffer[4] == '0')
                {
                    handler_drainPump_state_set(HANDLER_DRAIN_PUMP_INACTIVE);
                }
                else
                {
                    handler_drainPump_state_set(HANDLER_DRAIN_PUMP_ACTIVE);
                }
            }
            else if (0 != controller_string_compare(&gbuffer[0], 4, "COV:"))
            {
                if (gbuffer[4] == '0')
                {
                    handler_coldWaterValve_state_set(WATER_VALVE_INACTIVE);
                }
                else
                {
                    handler_coldWaterValve_state_set(WATER_VALVE_ACTIVE);
                }
            }
            else if (0 != controller_string_compare(&gbuffer[0], 4, "RIV:"))
            {
                if (gbuffer[4] == '0')
                {
                    handler_softenerWaterValve_state_set(WATER_VALVE_INACTIVE);
                }
                else
                {
                    handler_softenerWaterValve_state_set(WATER_VALVE_ACTIVE);
                }
            }
            else if (0 != controller_string_compare(&gbuffer[0], 4, "HOV:"))
            {
                if (gbuffer[4] == '0')
                {
                    handler_hotWaterValve_state_set(WATER_VALVE_INACTIVE);
                }
                else
                {
                    handler_hotWaterValve_state_set(WATER_VALVE_ACTIVE);
                }
            }
            else if (0 != controller_string_compare(&gbuffer[0], 4, "GEM:"))
            {
                if (gbuffer[4] == '0')
                {
                    handler_clutch_switch(HANDLER_CLUTCH_WITHOUT_BASKET);
                }
                else
                {
                    handler_clutch_switch(HANDLER_CLUTCH_WITH_BASKET);
                }
            }
            else if (0 != controller_string_compare(&gbuffer[0], 4, "LIL:"))
            {
                if (gbuffer[4] == '0')
                {
                    handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_INACTIVE);
                }
                else
                {
                    handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_ACTIVE);
                }
            }
            else if (0 != controller_string_compare(&gbuffer[0], 4, "DEP:"))
            {
                if (gbuffer[4] == '0')
                {
                    handler_detergentPump_state_set(HANDLER_DETERGENT_PUMP_INACTIVE);
                }
                else
                {
                    handler_detergentPump_state_set(HANDLER_DETERGENT_PUMP_ACTIVE);
                }
            }
            else if (0 != controller_string_compare(&gbuffer[0], 4, "SOP:"))
            {
                if (gbuffer[4] == '0')
                {
                    handler_softenerPump_state_set(HANDLER_SOFTENER_PUMP_INACTIVE);
                }
                else
                {
                    handler_softenerPump_state_set(HANDLER_SOFTENER_PUMP_ACTIVE);
                }
            }
            else if (gbuffer[0] == '/' && gbuffer[1] == 'U')
            {
                gx_app_controller_mode = APP_CONTROLLER_USER_MODE;
                LOG('0', 'i', "User Mode!!");
            }
            else if (gbuffer[0] == '/' && gbuffer[1] == '/')
            {
                gx_app_controller_mode = APP_CONTROLLER_NONE;
                LOG('0', 'i', "Exited All Modes!!");
            }
            else
            {
                // do nothing
            }

            switch (gx_app_controller_mode)
            {
                case APP_CONTROLLER_OVERRIDE_MODE:
                {
                    controller_overrideMode_process();
                }
                break;

                case APP_CONTROLLER_USER_MODE:
                {
                    controller_userMode_process();
                }
                break;

                case APP_CONTROLLER_NONE:
                default:
                {
                    // do nothing
                }
                break;
            }

            for (uint8_t i = 0; i < 10; i++)
            {
                gbuffer[i] = 0;
            }
            rxCounter = 0;
        }
        else
        {
            rxCounter++;
            if (rxCounter >= 10)
            {
                rxCounter = 0;
            }
        }
    }
}

static void controller_overrideMode_process(void)
{
    if (0 != controller_string_compare(&gbuffer[0], 6, "TM:WLC"))
    {
        handler_keypad_edgeState_set(KEY_WATER_LEVEL_ID, 1);
        handler_keypad_state_set(KEY_WATER_LEVEL_ID, 1);
        handler_keypad_edgePressedKeyCount_set(1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 6, "TM:WDC"))
    {
        handler_keypad_edgeState_set(KEY_WATER_LEVEL_ID, 1);
        handler_keypad_state_set(KEY_WATER_LEVEL_ID, 1);
        handler_keypad_edgePressedKeyCount_set(1);
    }
#ifdef LINT_FILTER_ENABLED
    else if (0 != controller_string_compare(&gbuffer[0], 5, "LF:ON"))
    {
        handler_lintFilterSensor_state_set(1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 6, "LF:OFF"))
    {
        handler_lintFilterSensor_state_set(0);
    }
#endif
#ifdef BLOWER_ENABLED
    else if (0 != controller_string_compare(&gbuffer[0], 6, "BLR:ON"))
    {
        handler_blower_output_set(90);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 6, "BLR:OFF"))
    {
        handler_blower_output_set(HANDLER_BLOWER_OFF);
    }
#endif
    else if (0 != controller_string_compare(&gbuffer[0], 4, "DL:0"))
    {
        app_userInterface_data_set()->normalMode.dry.dryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "DL:1"))
    {
        app_userInterface_data_set()->normalMode.dry.dryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_1;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "DL:2"))
    {
        app_userInterface_data_set()->normalMode.dry.dryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_2;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "DL:3"))
    {
        app_userInterface_data_set()->normalMode.dry.dryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_3;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "DL:4"))
    {
        app_userInterface_data_set()->normalMode.dry.dryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_4;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "DL:5"))
    {
        app_userInterface_data_set()->normalMode.dry.dryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_5;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "DL:6"))
    {
        app_userInterface_data_set()->normalMode.dry.dryWaterLevel = APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_6;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "WL:0"))
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "WL:1"))
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "WL:2"))
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_2;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "WL:3"))
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_3;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "WL:4"))
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_4;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "WL:5"))
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_5;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "WL:6"))
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_6;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "WL:7"))
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_7;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "WL:8"))
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_8;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "WL:9"))
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_9;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 5, "WL:10"))
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_10;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "M:ON"))
    {
        app_unbalance_detection_set(1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "M:OF"))
    {
        app_unbalance_detection_set(0);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "C:0"))
    {
        app_userInterface_data_set()->normalMode.common.courseSelection = APP_NORMAL_MODE_UI_COURSE_COTTON;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "O:0"))
    {
        app_userInterface_data_set()->normalMode.common.operationSelection = APP_NORMAL_MODE_UI_OPERATION_NONE;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "O:1"))
    {
        app_userInterface_data_set()->normalMode.common.operationSelection = APP_NORMAL_MODE_UI_OPERATION_WASH_ONLY;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "O:2"))
    {
        app_userInterface_data_set()->normalMode.common.operationSelection = APP_NORMAL_MODE_UI_OPERATION_WASH_DRY;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "O:3"))
    {
        app_userInterface_data_set()->normalMode.common.operationSelection = APP_NORMAL_MODE_UI_OPERATION_DRY_ONLY;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "T:0"))
    {
        app_userInterface_data_set()->normalMode.wash.waterTempSelection = APP_NORMAL_MODE_UI_WATER_TEMP_NONE;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "T:1"))
    {
        app_userInterface_data_set()->normalMode.wash.waterTempSelection = APP_NORMAL_MODE_UI_WATER_TEMP_COLD;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "T:2"))
    {
        app_userInterface_data_set()->normalMode.wash.waterTempSelection = APP_NORMAL_MODE_UI_WATER_TEMP_WARM;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "T:3"))
    {
        app_userInterface_data_set()->normalMode.wash.waterTempSelection = APP_NORMAL_MODE_UI_WATER_TEMP_HOT;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "K:0"))
    {
        app_userInterface_data_set()->normalMode.wash.soakSelection = APP_NORMAL_MODE_UI_SOAK_TIME_0_HOUR;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "K:1"))
    {
        app_userInterface_data_set()->normalMode.wash.soakSelection = APP_NORMAL_MODE_UI_SOAK_TIME_1_HOUR;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "K:2"))
    {
        app_userInterface_data_set()->normalMode.wash.soakSelection = APP_NORMAL_MODE_UI_SOAK_TIME_2_HOUR;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "K:3"))
    {
        app_userInterface_data_set()->normalMode.wash.soakSelection = APP_NORMAL_MODE_UI_SOAK_TIME_3_HOUR;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "K:4"))
    {
        app_userInterface_data_set()->normalMode.wash.soakSelection = APP_NORMAL_MODE_UI_SOAK_TIME_4_HOUR;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "W:0"))
    {
        app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_0_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "W:1"))
    {
        app_userInterface_data_set()->normalMode.wash.washSelection = (app_nm_wash_userInterface_washTimeSelection_t)1;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "W:3"))
    {
        app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_3_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "W:5"))
    {
        app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_5_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "W:9"))
    {
        app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_9_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "W:12"))
    {
        app_userInterface_data_set()->normalMode.wash.washSelection = APP_NORMAL_MODE_UI_WASH_TIME_12_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "R:0"))
    {
        app_userInterface_data_set()->normalMode.wash.rinseSelection = APP_NORMAL_MODE_UI_RINSE_0_TIMES;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "R:1"))
    {
        app_userInterface_data_set()->normalMode.wash.rinseSelection = APP_NORMAL_MODE_UI_RINSE_1_TIMES;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "R:2"))
    {
        app_userInterface_data_set()->normalMode.wash.rinseSelection = APP_NORMAL_MODE_UI_RINSE_2_TIMES;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "R:3"))
    {
        app_userInterface_data_set()->normalMode.wash.rinseSelection = APP_NORMAL_MODE_UI_RINSE_3_TIMES;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "ER:0"))
    {
        app_userInterface_data_set()->normalMode.wash.extraRinseSelection = APP_NORMAL_MODE_UI_RINSE_OVERFLOW_OFF_SHOWER_OFF;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "ER:1"))
    {
        app_userInterface_data_set()->normalMode.wash.extraRinseSelection = APP_NORMAL_MODE_UI_RINSE_OVERFLOW_ON_SHOWER_OFF;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "ER:2"))
    {
        app_userInterface_data_set()->normalMode.wash.extraRinseSelection = APP_NORMAL_MODE_UI_RINSE_OVERFLOW_OFF_SHOWER_ON;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "ER:3"))
    {
        app_userInterface_data_set()->normalMode.wash.extraRinseSelection = APP_NORMAL_MODE_UI_RINSE_OVERFLOW_ON_SHOWER_ON;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "S:0"))
    {
        app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "S:1"))
    {
        app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_1_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "S:5"))
    {
        app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_5_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "S:9"))
    {
        app_userInterface_data_set()->normalMode.wash.spinSelection = APP_NORMAL_MODE_UI_SPIN_TIME_9_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "SL:0"))
    {
        app_userInterface_data_set()->normalMode.wash.soilLevelSelection = APP_NORMAL_MODE_UI_SOIL_LEVEL_NONE;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "SL:1"))
    {
        app_userInterface_data_set()->normalMode.wash.soilLevelSelection = APP_NORMAL_MODE_UI_SOIL_LEVEL_LIGHT;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "SL:2"))
    {
        app_userInterface_data_set()->normalMode.wash.soilLevelSelection = APP_NORMAL_MODE_UI_SOIL_LEVEL_NORMAL;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "SL:3"))
    {
        app_userInterface_data_set()->normalMode.wash.soilLevelSelection = APP_NORMAL_MODE_UI_SOIL_LEVEL_HEAVY;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "SS:0"))
    {
        app_userInterface_data_set()->normalMode.wash.superSpinSelection = APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_0_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "SS:1"))
    {
        // app_userInterface_data_set()->normalMode.wash.superSpinSelection = APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_10_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "SS:2"))
    {
        app_userInterface_data_set()->normalMode.wash.superSpinSelection = APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_20_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "SS:3"))
    {
        // app_userInterface_data_set()->normalMode.wash.superSpinSelection = APP_NORMAL_MODE_UI_SUPER_SPIN_TIME_30_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "A:0"))
    {
        app_userInterface_data_set()->normalMode.wash.antiWrinkleSelection = APP_NORMAL_MODE_UI_ANTI_WRINKLE_TIME_0_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "A:1"))
    {
        app_userInterface_data_set()->normalMode.wash.antiWrinkleSelection = APP_NORMAL_MODE_UI_ANTI_WRINKLE_TIME_1_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "A:2"))
    {
        app_userInterface_data_set()->normalMode.wash.antiWrinkleSelection = APP_NORMAL_MODE_UI_ANTI_WRINKLE_TIME_2_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "D:0"))
    {
        app_userInterface_data_set()->normalMode.dry.dryTimeSelection = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "D:1"))
    {
        app_userInterface_data_set()->normalMode.dry.dryTimeSelection = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "D:2"))
    {
        app_userInterface_data_set()->normalMode.dry.dryTimeSelection = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_60_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "D:3"))
    {
        app_userInterface_data_set()->normalMode.dry.dryTimeSelection = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_90_MIN;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "D:A"))
    {
        app_userInterface_data_set()->normalMode.dry.dryTimeSelection = APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "DI:0"))
    {
        app_userInterface_data_set()->normalMode.dry.dryIntensitySelection = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NONE;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "DI:1"))
    {
        app_userInterface_data_set()->normalMode.dry.dryIntensitySelection = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL;
    }
    else if (0 != controller_string_compare(&gbuffer[0], 4, "DI:2"))
    {
        app_userInterface_data_set()->normalMode.dry.dryIntensitySelection = APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_HEAVY;
    }
    else
    {
        // do nothing
    }
}

static void controller_userMode_process(void)
{
    if (0 != controller_string_compare(&gbuffer[0], 2, "DD"))
    {
        keypad_keyState_register(KEY_WATER_LEVEL_ID, 1, 3000);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "ED"))
    {
        keypad_keyState_register(KEY_SPIN_ID, 1, 5000);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "CO"))
    {
        handler_keypad_edgeState_set(KEY_COTTON_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "WH"))
    {
        handler_keypad_edgeState_set(KEY_WHITE_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "BE"))
    {
        handler_keypad_edgeState_set(KEY_BEDDING_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "MI"))
    {
        handler_keypad_edgeState_set(KEY_MIX_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "DA"))
    {
        handler_keypad_edgeState_set(KEY_DARKS_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "BA"))
    {
        handler_keypad_edgeState_set(KEY_BABYCARE_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "SE"))
    {
        handler_keypad_edgeState_set(KEY_SENSITIVE_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "LI"))
    {
        handler_keypad_edgeState_set(KEY_LIGHT_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "EC"))
    {
        handler_keypad_edgeState_set(KEY_ECO_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "PO"))
    {
        handler_keypad_edgeState_set(KEY_SPORTS_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "JE"))
    {
        handler_keypad_edgeState_set(KEY_JEANS_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "AL"))
    {
        handler_keypad_edgeState_set(KEY_ALERGY_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "TU"))
    {
        handler_keypad_edgeState_set(KEY_TUBCLEAN_ID, 1);
    }
#ifdef DRYER_ENABLED
    else if (0 != controller_string_compare(&gbuffer[0], 2, "WD"))
    {
        handler_keypad_edgeState_set(KEY_WASH_N_DRY_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "DO"))
    {
        handler_keypad_edgeState_set(KEY_DRY_ONLY_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "WO"))
    {
        handler_keypad_edgeState_set(KEY_WASH_ONLY_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "DT"))
    {
        handler_keypad_edgeState_set(KEY_DRY_TIME_ID, 1);
    }
#endif
#ifdef STEAM_ENABLED
    else if (0 != controller_string_compare(&gbuffer[0], 2, "TT"))
    {
        handler_keypad_edgeState_set(KEY_STEAM_ID, 1);
    }
#endif
    else if (0 != controller_string_compare(&gbuffer[0], 2, "DS"))
    {
        handler_keypad_edgeState_set(KEY_DELAY_START_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "TP"))
    {
        handler_keypad_edgeState_set(KEY_WATER_TEMP_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "KT"))
    {
        handler_keypad_edgeState_set(KEY_SOAK_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "WT"))
    {
        handler_keypad_edgeState_set(KEY_WASH_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "RT"))
    {
        handler_keypad_edgeState_set(KEY_RINSE_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "ST"))
    {
        handler_keypad_edgeState_set(KEY_SPIN_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 3, "SST"))
    {
        handler_keypad_edgeState_set(KEY_EXTRA_SPIN_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "SL"))
    {
        handler_keypad_edgeState_set(KEY_STAIN_LEVEL_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "WL"))
    {
        handler_keypad_edgeState_set(KEY_WATER_LEVEL_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "SM"))
    {
        handler_keypad_edgeState_set(KEY_STEAM_TECH_ID, 1);
    }
    else if (0 != controller_string_compare(&gbuffer[0], 2, "GD"))
    {
        handler_keypad_edgeState_set(KEY_GEL_DETERGENT_ID, 1);
    }
    else
    {
        // do nothing
    }
}

static uint8_t controller_string_compare(char *lbuffer, uint8_t size, const char *string)
{
    uint8_t ret = 1;

    if (size > 10)
        return 0;

    for (uint8_t i = 0; i < size; i++)
    {
        if (lbuffer[i] == string[i])
        {
            continue;
        }
        else
        {
            ret = 0;
            break;
        }
    }

    return ret;
}

static void keypad_keyState_register(keyID_t key, uint8_t state, uint32_t timeout)
{
    uint32_t *keys;
    keyState_internalTimeoutMSec[key] = timeout;
    keyState_state[key] = 1;
    keyState_keyID[key] = key;

    handler_keypad_state_set(key, state);
    keys = handler_keypad_buffer_get();
    if (keys != NULL)
    {
        *keys = *keys & (0x00 << key);
        *keys = *keys | (state << key);
    }
}

static void keypad_state_update(uint32_t period)
{
    uint32_t *keys;
    for (uint8_t i = 0; i < KEYPAD_KEY_NUM; i++)
    {
        if (keyState_state[i] == 1)
        {
            if (keyState_internalTimeoutMSec[i] >= period)
            {
                keyState_internalTimeoutMSec[i] -= period;
            }
            else
            {
                keyState_internalTimeoutMSec[i] = 0;
            }

            if (keyState_internalTimeoutMSec[i] == 0)
            {
                handler_keypad_state_set(keyState_keyID[i], 0);
                keys = handler_keypad_buffer_get();
                if (keys != NULL)
                {
                    *keys = *keys & (0x00 << keyState_keyID[i]);
                }
                keyState_state[i] = 0;
            }
        }
    }
}
