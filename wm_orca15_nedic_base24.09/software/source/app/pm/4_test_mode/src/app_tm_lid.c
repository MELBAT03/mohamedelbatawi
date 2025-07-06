#include "def.h"
#include "app_test_mode.h"

#include "app_lid.h"

void app_tm_lid_process(uint32_t period)
{
    switch (app_tm_type_get())
    {
        case APP_TM_WATER_LEVEL_CALIBRATION:
        {
            app_lidLock_state_set(APP_LID_LOCK_OFF);
        }
        break;

        case APP_TM_WEIGHT_CALIBRATION:
        {
            app_lidLock_state_set(APP_LID_LOCK_OFF);
        }
        break;

        case APP_TM_ELECTRICAL_PARTS:
        {
            app_lidLock_state_set(APP_LID_LOCK_OFF);
        }
        break;

        case APP_TM_UNBALANCE_SPIN:
        {
        }
        break;

        case APP_TM_WATER_LEVEL_FREQ:
        {
        }
        break;

        case APP_TM_PCB:
        {
        }
        break;

        case APP_TM_MIDDLE_INSPECTION:
        {
        }
        break;

        case APP_TM_FINAL_INSPECTION:
        {
        }
        break;

        case APP_TM_ERROR_COUNT_DISPLAY:
        {
        }
        break;

        case APP_TM_CYCLE_COUNT_DISPLAY:
        {
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }   //     else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_ERROR_COUNT_DISPLAY)
    //     {
    //         switch (app_testMode_errorCntState_get())
    //         {
    //             case ERROR_COUNT_STATE_IDLE:
    //             case ERROR_COUNT_STATE_ERROR_TRAVERSE:
    //             case ERROR_COUNT_STATE_ERROR_DISPLAY:
    //             {
    //                 handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_INACTIVE);
    //             }
    //             break;

    //             default:
    //             {
    //                 // do nothing
    //             }
    //             break;
    //         }
    //     }
    //     else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_CYCLE_COUNT_DISPLAY)
    //     {
    //         switch (app_testMode_cycleCntState_get())
    //         {
    //             case CYCLE_COUNT_STATE_IDLE:
    //             case CYCLE_COUNT_STATE_CYCLE_TRAVERSE:
    //             case CYCLE_COUNT_STATE_CYCLE_DISPLAY:
    //             {
    //                 handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_INACTIVE);
    //             }
    //             break;

    //             default:
    //             {
    //                 // do nothing
    //             }
    //             break;
    //         }
    //     }
    //     else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_UNBALANCE_SPIN)
    //     {
    //         switch (app_testMode_unbalanceSpinState_get())
    //         {
    //             case UNBALANCE_SPIN_STANDBY:
    //             case UNBALANCE_SPIN_BRAKE:
    //             case UNBALANCE_SPIN_SPIN:
    //                 handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_INACTIVE);
    //                 break;

    //             default:
    //                 // do nothing
    //                 break;
    //         }
    //     }
    //     else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_WATER_LEVEL_FREQ)
    //     {
    //         switch (app_testMode_waterLevelFreqState_get())
    //         {
    //             case WATER_LEVEL_FREQ_FILLING:
    //             case WATER_LEVEL_FREQ_DRAIN:
    //             case WATER_LEVEL_FREQ_WASH:
    //                 handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_INACTIVE);
    //                 break;

    //             default:
    //                 // do nothing
    //                 break;
    //         }
    //     }
    //     else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_PCB)
    //     {
    //         switch (app_testMode_pcbState_get())
    //         {
    //             case PCB_VERSION_CHECK:
    //             case PCB_DISPLAY_CHECK:
    //             case PCB_WATER_LEVEL_SENSOR_CHECK:
    //             case PCB_DRAIN_PUMP_CHECK:
    //             case PCB_HOT_VALVE_CHECK:
    //             case PCB_SOFTENER_VALVE_CHECK:
    //             case PCB_DRAIN_VALVE_CHECK:
    //             case PCB_COLD_VALVE_CHECK:
    //             case PCB_HEAVY_LED_CHECK:
    //             case PCB_COTTON_LED_CHECK:
    //             case PCB_REGULAR_LED_CHECK:
    //             case PCB_QUICK_WASH_LED_CHECK:
    //             case PCB_JEANS_LED_CHECK:
    //             case PCB_BLANKET_LED_CHECK:
    //             case PCB_MEMS_CHECK:
    //             case PCB_LID_LOCK_OFF_CHECK:
    //             case PCB_MOTOR_CW_CHECK:
    //             case PCB_MOTOR_CW_STOP_CHECK:
    //             case PCB_MOTOR_CCW_CHECK:
    //             case PCB_MOTOR_CCW_STOP_CHECK:
    //             case PCB_LID_SENSOR_CHECK:
    // #if defined(WM_SHOWER_VALVE_ENABLED)
    //             case PCB_SHOWER_VALVE_CHECK:
    // #endif
    //                 handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_INACTIVE);
    //                 break;

    //             case PCB_LID_LOCK_ON_CHECK:
    //                 handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_ACTIVE);
    //                 break;

    //             default:
    //                 // do nothing
    //                 break;
    //         }
    //     }
    //     else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_MIDDLE_INSPECTION)
    //     {
    //         switch (app_testMode_middleInspectionState_get())
    //         {
    //             case MIDDLE_INSPECTION_STANDBY:
    //             case MIDDLE_INSPECTION_WATER_LEVEL_CHECK:
    //             case MIDDLE_INSPECTION_DRAIN:
    //             case MIDDLE_INSPECTION_SPIN:
    //             case MIDDLE_INSPECTION_DRAIN_HOLD:
    //             case MIDDLE_INSPECTION_ERROR:
    //             case MIDDLE_INSPECTION_LOAD_SENSE:
    //             case MIDDLE_INSPECTION_WASHING:
    //                 handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_INACTIVE);
    //                 break;

    //             case MIDDLE_INSPECTION_WATER_VALVE_CHECK:
    //             {
    //                 switch (app_testMode_middleInspectionWaterValveState_get())
    //                 {
    //                     case MIDDLE_INSPECTION_NONE_VALVE_CHECK:
    //                     case MIDDLE_INSPECTION_COLD_VALVE_CHECK:
    //                     case MIDDLE_INSPECTION_HOT_VALVE_CHECK:
    //                     case MIDDLE_INSPECTION_SOFTENER_VALVE_CHECK:
    // #if defined(WM_SHOWER_VALVE_ENABLED)
    //                     case MIDDLE_INSPECTION_SHOWER_VALVE_CHECK:
    //                         handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_INACTIVE);
    //                         break;
    // #endif

    //                     case MIDDLE_INSPECTION_LID_LOCK_CHECK:
    //                         handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_ACTIVE);
    //                         break;

    //                     default:
    //                         // do nothing
    //                         break;
    //                 }
    //             }
    //             break;

    //             case MIDDLE_INSPECTION_DONE:
    //                 // do nothing
    //                 break;

    //             default:
    //                 // do nothing
    //                 break;
    //         }
    //     }
    //     else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_FINAL_INSPECTION)
    //     {
    //         switch (app_testMode_finalInspectionState_get())
    //         {
    //             case FINAL_INSPECTION_STANDBY:
    //             case FINAL_INSPECTION_BRAKING:
    //             case FINAL_INSPECTION_DRAIN:
    //             case FINAL_INSPECTION_LID_CLOSE_CHECK:
    //             case FINAL_INSPECTION_TOTAL_TIME_CHECK:
    //             case FINAL_INSPECTION_ERROR:
    //             case FINAL_INSPECTION_LED_FLASH:
    //             case FINAL_INSPECTION_MECH_RANKING_WATER_LEVEL_CALIBRATION:
    //             case FINAL_INSPECTION_WEIGHT_SENSOR_CHECK:
    //                 handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_INACTIVE);
    //                 break;

    //             case FINAL_INSPECTION_SPIN:
    //             {
    //                 if (app_motorHandler_speed_get(&motorSpeed) == APP_MOTOR_HANDLER_ERROR_NONE)
    //                 {
    //                     if (motorSpeed < 600)
    //                     {
    //                         handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_ACTIVE);
    //                     }
    //                     else
    //                     {
    //                         handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_INACTIVE);
    //                     }
    //                 }
    //             }
    //             break;

    //             case FINAL_INSPECTION_DONE:
    //                 // do nothing
    //                 break;

    //             default:
    //                 // do nothing
    //                 break;
    //         }
    //     }
}
