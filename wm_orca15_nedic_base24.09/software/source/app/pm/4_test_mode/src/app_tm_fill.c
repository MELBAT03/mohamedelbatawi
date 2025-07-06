#include "def.h"
#include "app_test_mode.h"

#include "app_fill.h"

#include "handlers/output/inc/handler_water_valve.h"

void app_tm_fill_process(uint32_t period)
{
    switch (app_tm_type_get())
    {
        case APP_TM_WATER_LEVEL_CALIBRATION:
        {
            app_fill_stop();
        }
        break;

        case APP_TM_WEIGHT_CALIBRATION:
        {
            app_fill_stop();
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
                case TM_ELECTRICAL_PARTS_MOTORCCWSTOP_CHECK:
                case TM_ELECTRICAL_PARTS_DRAINVALVE_CHECK:
                case TM_ELECTRICAL_PARTS_ENDURANCECOUNT_CHECK:
                {
                    app_fill_stop();
                }
                break;

                case TM_ELECTRICAL_PARTS_HOTVALVE_CHECK:
                {
                    handler_coldWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_hotWaterValve_state_set(WATER_VALVE_ACTIVE);
                    handler_softenerWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_showerWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_condenserWaterValve_state_set(WATER_VALVE_INACTIVE);
                }
                break;

                case TM_ELECTRICAL_PARTS_COLDVALVE_CHECK:
                {
                    handler_coldWaterValve_state_set(WATER_VALVE_ACTIVE);
                    handler_hotWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_softenerWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_showerWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_condenserWaterValve_state_set(WATER_VALVE_INACTIVE);
                }
                break;

                case TM_ELECTRICAL_PARTS_SOFTNERVALVE_CHECK:
                {
                    handler_coldWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_hotWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_softenerWaterValve_state_set(WATER_VALVE_ACTIVE);
                    handler_showerWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_condenserWaterValve_state_set(WATER_VALVE_INACTIVE);
                }
                break;

#ifdef WM_SHOWER_VALVE_ENABLED
                case TM_ELECTRICAL_PARTS_SHOWERVALVE_CHECK:
                {
                    handler_coldWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_hotWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_softenerWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_showerWaterValve_state_set(WATER_VALVE_ACTIVE);
                    handler_condenserWaterValve_state_set(WATER_VALVE_INACTIVE);
                }
                break;
#endif

#ifdef WM_CONDENSER_VALVE_ENABLED
                case TM_ELECTRICAL_PARTS_CONDENSERVALVE_CHECK:
                {
                    handler_coldWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_hotWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_softenerWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_showerWaterValve_state_set(WATER_VALVE_INACTIVE);
                    handler_condenserWaterValve_state_set(WATER_VALVE_ACTIVE);
                }
                break;
#endif

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
    //                 handler_waterValve_state_set(COLD_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(HOT_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(SOFTENER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(SHOWER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
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
    //                 handler_waterValve_state_set(COLD_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(HOT_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(SOFTENER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(SHOWER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
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
    //                 fill_stop();
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
    //             case WATER_LEVEL_FREQ_DRAIN:
    //             case WATER_LEVEL_FREQ_WASH:
    //                 fill_stop();
    //                 break;

    //             case WATER_LEVEL_FREQ_FILLING:
    //                 handler_waterValve_state_set(COLD_WATER_VALVE_ID, WATER_VALVE_ACTIVE);
    //                 handler_waterValve_state_set(HOT_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(SOFTENER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(SHOWER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
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
    //             case PCB_DRAIN_VALVE_CHECK:
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
    //                 fill_stop();
    //                 break;

    //             case PCB_HOT_VALVE_CHECK:
    //                 handler_waterValve_state_set(COLD_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(HOT_WATER_VALVE_ID, WATER_VALVE_ACTIVE);
    //                 handler_waterValve_state_set(SOFTENER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(SHOWER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 break;

    //             case PCB_COLD_VALVE_CHECK:
    //                 handler_waterValve_state_set(COLD_WATER_VALVE_ID, WATER_VALVE_ACTIVE);
    //                 handler_waterValve_state_set(HOT_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(SOFTENER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(SHOWER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 break;

    //             case PCB_SOFTENER_VALVE_CHECK:
    //                 handler_waterValve_state_set(COLD_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(HOT_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(SOFTENER_WATER_VALVE_ID, WATER_VALVE_ACTIVE);
    //                 handler_waterValve_state_set(SHOWER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 break;

    // #if defined(WM_SHOWER_VALVE_ENABLED)
    //             case PCB_SHOWER_VALVE_CHECK:
    //                 handler_waterValve_state_set(COLD_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(HOT_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(SOFTENER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                 handler_waterValve_state_set(SHOWER_WATER_VALVE_ID, WATER_VALVE_ACTIVE);
    //                 break;
    // #endif

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
    //             case MIDDLE_INSPECTION_LOAD_SENSE:
    //             case MIDDLE_INSPECTION_WASHING:
    //             case MIDDLE_INSPECTION_DRAIN:
    //             case MIDDLE_INSPECTION_DRAIN_HOLD:
    //             case MIDDLE_INSPECTION_SPIN:
    //             case MIDDLE_INSPECTION_ERROR:
    //                 fill_stop();
    //                 break;

    //             case MIDDLE_INSPECTION_DONE:
    //                 // do nothing
    //                 break;

    //             case MIDDLE_INSPECTION_WATER_VALVE_CHECK:
    //                 switch (app_testMode_middleInspectionWaterValveState_get())
    //                 {
    //                     case MIDDLE_INSPECTION_NONE_VALVE_CHECK:
    //                         fill_stop();
    //                         break;

    //                     case MIDDLE_INSPECTION_COLD_VALVE_CHECK:
    //                         handler_waterValve_state_set(COLD_WATER_VALVE_ID, WATER_VALVE_ACTIVE);
    //                         handler_waterValve_state_set(HOT_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                         handler_waterValve_state_set(SOFTENER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                         handler_waterValve_state_set(SHOWER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                         break;

    //                     case MIDDLE_INSPECTION_HOT_VALVE_CHECK:
    //                         handler_waterValve_state_set(COLD_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                         handler_waterValve_state_set(HOT_WATER_VALVE_ID, WATER_VALVE_ACTIVE);
    //                         handler_waterValve_state_set(SOFTENER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                         handler_waterValve_state_set(SHOWER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                         break;

    //                     case MIDDLE_INSPECTION_SOFTENER_VALVE_CHECK:
    //                         handler_waterValve_state_set(COLD_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                         handler_waterValve_state_set(HOT_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                         handler_waterValve_state_set(SOFTENER_WATER_VALVE_ID, WATER_VALVE_ACTIVE);
    //                         handler_waterValve_state_set(SHOWER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                         break;

    // #if defined(WM_SHOWER_VALVE_ENABLED)
    //                     case MIDDLE_INSPECTION_SHOWER_VALVE_CHECK:
    //                         handler_waterValve_state_set(COLD_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                         handler_waterValve_state_set(HOT_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                         handler_waterValve_state_set(SOFTENER_WATER_VALVE_ID, WATER_VALVE_INACTIVE);
    //                         handler_waterValve_state_set(SHOWER_WATER_VALVE_ID, WATER_VALVE_ACTIVE);
    //                         break;
    // #endif

    //                     default:
    //                         // do nothing
    //                         break;
    //                 }
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
    //             case FINAL_INSPECTION_SPIN:
    //             case FINAL_INSPECTION_LED_FLASH:
    //             case FINAL_INSPECTION_MECH_RANKING_WATER_LEVEL_CALIBRATION:
    //             case FINAL_INSPECTION_WEIGHT_SENSOR_CHECK:
    //             case FINAL_INSPECTION_TOTAL_TIME_CHECK:
    //             case FINAL_INSPECTION_ERROR:
    //                 fill_stop();
    //                 break;

    //             case FINAL_INSPECTION_DONE:
    //                 // do nothing
    //                 break;

    //             default:
    //                 // do nothing
    //                 break;
    //         }
    //     }
}
