#include "def.h"

#include "app_test_mode.h"
#include "app_drain.h"

void app_tm_drain_process(uint32_t period)
{
    switch (app_tm_type_get())
    {
        case APP_TM_WATER_LEVEL_CALIBRATION:
        {
            app_drain_switch(APP_DRAIN_CLOSE);
        }
        break;

        case APP_TM_WEIGHT_CALIBRATION:
        {
            app_drain_switch(APP_DRAIN_CLOSE);
        }
        break;

        case APP_TM_ELECTRICAL_PARTS:
        {
            switch (app_tm_electricalPartsState_get())
            {
                case TM_ELECTRICAL_PARTS_VERSION_CHECK:
                case TM_ELECTRICAL_PARTS_DISPLAY_CHECK:
                case TM_ELECTRICAL_PARTS_WATERLEVEL_CHECK:
                case TM_ELECTRICAL_PARTS_MOTORCW_CHECK:
                case TM_ELECTRICAL_PARTS_MOTORCWSTOP_CHECK:
                case TM_ELECTRICAL_PARTS_MOTORCCW_CHECK:
                case TM_ELECTRICAL_PARTS_ENDURANCECOUNT_CHECK:
                case TM_ELECTRICAL_PARTS_MOTORCCWSTOP_CHECK:
                case TM_ELECTRICAL_PARTS_HOTVALVE_CHECK:
                case TM_ELECTRICAL_PARTS_COLDVALVE_CHECK:
                case TM_ELECTRICAL_PARTS_SOFTNERVALVE_CHECK:
#if defined(WM_SHOWER_VALVE_ENABLED)
                case TM_ELECTRICAL_PARTS_SHOWERVALVE_CHECK:
#endif
#ifdef WM_CONDENSER_VALVE_ENABLED
                case TM_ELECTRICAL_PARTS_CONDENSERVALVE_CHECK:
#endif
                {
                    app_drain_switch(APP_DRAIN_CLOSE);
                }
                break;

                case TM_ELECTRICAL_PARTS_DRAINVALVE_CHECK:
                {
                    app_drain_switch(APP_DRAIN_OPEN);
                }
                break;

                default:
                {
                    // do nothing
                }
                break;
            }
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }

    //     else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_ERROR_COUNT_DISPLAY)
    //     {
    //         switch (app_testMode_errorCntState_get())
    //         {
    //             case ERROR_COUNT_STATE_IDLE:
    //             case ERROR_COUNT_STATE_ERROR_TRAVERSE:
    //             case ERROR_COUNT_STATE_ERROR_DISPLAY:
    //             {
    //                 drain_clutch_switch(CLUTCH_CLOSE);
    //                 app_drainPump_state_set(DRAIN_PUMP_INACTIVE);
    //                 drainPumpState = DRAIN_PUMP_INACTIVE;
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
    //                 drain_clutch_switch(CLUTCH_CLOSE);
    //                 app_drainPump_state_set(DRAIN_PUMP_INACTIVE);
    //                 drainPumpState = DRAIN_PUMP_INACTIVE;
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
    //             case UNBALANCE_SPIN_SPIN:
    //             case UNBALANCE_SPIN_BRAKE:
    //                 drain_clutch_switch(CLUTCH_OPEN);
    //                 app_drainPump_state_set(DRAIN_PUMP_INACTIVE);
    //                 drainPumpState = DRAIN_PUMP_INACTIVE;
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
    //             case WATER_LEVEL_FREQ_WASH:
    //                 drain_clutch_switch(CLUTCH_CLOSE);
    //                 app_drainPump_state_set(DRAIN_PUMP_INACTIVE);
    //                 drainPumpState = DRAIN_PUMP_INACTIVE;
    //                 break;

    //             case WATER_LEVEL_FREQ_DRAIN:
    //                 if (app_pulsator_clutchSwitchingSafteyFlag_get() == PULSATOR_CLUTCH_SWITCHING_ACK)
    //                 {
    //                     drain_clutch_switch(CLUTCH_OPEN);
    //                 }
    //                 app_drainPump_state_set(DRAIN_PUMP_ACTIVE);
    //                 drainPumpState = DRAIN_PUMP_ACTIVE;
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
    //             case PCB_HOT_VALVE_CHECK:
    //             case PCB_SOFTENER_VALVE_CHECK:
    //             case PCB_COLD_VALVE_CHECK:
    // #if defined(WM_SHOWER_VALVE_ENABLED)
    //             case PCB_SHOWER_VALVE_CHECK:
    // #endif
    //             case PCB_MOTOR_CCW_CHECK:
    //             case PCB_MOTOR_CCW_STOP_CHECK:
    //             case PCB_MOTOR_CW_CHECK:
    //             case PCB_MOTOR_CW_STOP_CHECK:
    //             case PCB_HEAVY_LED_CHECK:
    //             case PCB_COTTON_LED_CHECK:
    //             case PCB_REGULAR_LED_CHECK:
    //             case PCB_QUICK_WASH_LED_CHECK:
    //             case PCB_JEANS_LED_CHECK:
    //             case PCB_BLANKET_LED_CHECK:
    //             case PCB_MEMS_CHECK:
    //             case PCB_LID_LOCK_ON_CHECK:
    //             case PCB_LID_LOCK_OFF_CHECK:
    //             case PCB_LID_SENSOR_CHECK:
    //                 drain_clutch_switch(CLUTCH_CLOSE);
    //                 app_drainPump_state_set(DRAIN_PUMP_INACTIVE);
    //                 drainPumpState = DRAIN_PUMP_INACTIVE;
    //                 break;

    //             case PCB_DRAIN_VALVE_CHECK:
    //                 drain_clutch_switch(CLUTCH_OPEN);
    //                 app_drainPump_state_set(DRAIN_PUMP_INACTIVE);
    //                 drainPumpState = DRAIN_PUMP_INACTIVE;
    //                 break;

    //             case PCB_DRAIN_PUMP_CHECK:
    //                 drain_clutch_switch(CLUTCH_CLOSE);
    //                 app_drainPump_state_set(DRAIN_PUMP_ACTIVE);
    //                 drainPumpState = DRAIN_PUMP_ACTIVE;
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
    //             case MIDDLE_INSPECTION_WATER_VALVE_CHECK:
    //             case MIDDLE_INSPECTION_WATER_LEVEL_CHECK:
    //             case MIDDLE_INSPECTION_LOAD_SENSE:
    //             case MIDDLE_INSPECTION_WASHING:
    //             case MIDDLE_INSPECTION_ERROR:
    //                 drain_clutch_switch(CLUTCH_CLOSE);
    //                 app_drainPump_state_set(DRAIN_PUMP_INACTIVE);
    //                 drainPumpState = DRAIN_PUMP_INACTIVE;
    //                 break;

    //             case MIDDLE_INSPECTION_DRAIN:
    //             case MIDDLE_INSPECTION_DRAIN_HOLD:
    //             case MIDDLE_INSPECTION_SPIN:
    //                 drain_clutch_switch(CLUTCH_OPEN);
    //                 app_drainPump_state_set(DRAIN_PUMP_ACTIVE);
    //                 drainPumpState = DRAIN_PUMP_ACTIVE;
    //                 break;

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
    //             case FINAL_INSPECTION_LED_FLASH:
    //             case FINAL_INSPECTION_MECH_RANKING_WATER_LEVEL_CALIBRATION:
    //             case FINAL_INSPECTION_WEIGHT_SENSOR_CHECK:
    //             case FINAL_INSPECTION_TOTAL_TIME_CHECK:
    //             case FINAL_INSPECTION_ERROR:
    //                 drain_clutch_switch(CLUTCH_CLOSE);
    //                 app_drainPump_state_set(DRAIN_PUMP_ACTIVE);
    //                 drainPumpState = DRAIN_PUMP_ACTIVE;
    //                 break;

    //             case FINAL_INSPECTION_LID_CLOSE_CHECK:
    //             case FINAL_INSPECTION_DRAIN:
    //             case FINAL_INSPECTION_SPIN:
    //                 drain_clutch_switch(CLUTCH_OPEN);
    //                 app_drainPump_state_set(DRAIN_PUMP_ACTIVE);
    //                 drainPumpState = DRAIN_PUMP_ACTIVE;
    //                 break;

    //             case FINAL_INSPECTION_DONE:
    //                 // do nothing
    //                 break;

    //             default:
    //                 // do nothing
    //                 break;
    //         }
    // }
}
