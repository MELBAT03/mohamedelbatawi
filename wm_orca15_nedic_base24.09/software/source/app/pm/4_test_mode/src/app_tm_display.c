#include "def.h"

#include "app_tm_display.h"
#include "app_display.h"

typedef enum
{
    VERSION_BOARD,
    VERSION_DRIVER,
    VERSION_STATE,
    VERSION_MODEL,
    VERSION_DISPLAY
} electricalParts_versionState_t;

typedef enum
{
    DISPLAY_ON,
    DISPLAY_OFF
} electricalParts_displayState_t;

// static uint32_t gu32_finalInspection_totalTimeMins, gu32_finalInspection_totalTimeSecs;
// static electricalParts_versionState_t versionState = VERSION_BOARD;
// static electricalParts_displayState_t displayState = DISPLAY_ON;

void app_tm_display_process(uint32_t period)
{
    // do nothing
}

#if 0
if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_WATER_LEVEL_CALIBRATION)
        {
            handler_display_clear();
            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_NULL_SYMBOL);
            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_C_SYMBOL);
            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_L_SYMBOL);
            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_TRIPLE_DASH_SYMBOL);
        }
        else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_WEIGHT_CALIBRATION)
        {
            handler_display_clear();
            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_NULL_SYMBOL);
            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_C_SYMBOL);
            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_TRIPLE_DASH_SYMBOL);
        }
        else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_ERROR_COUNT_DISPLAY)
        {
            switch (app_testMode_errorCntState_get())
            {
                case ERROR_COUNT_STATE_IDLE:
                {
                    handler_display_clear();
                    versionDisplayCnt += period;

                    switch (versionState)
                    {
                        case VERSION_BOARD:
                            handler_display_ssd_set(SSD_DIGIT_0_ID, WM_SW_VERSION_MINOR);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, WM_SW_VERSION_MAJOR);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_U_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_MODEL;
                            }
                            break;

                        case VERSION_MODEL:
                        {
#if defined(WM_MODEL_11KG)
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_5_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_1_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_h_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_S_SYMBOL);
#elif defined(WM_MODEL_13KG)
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_7_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_1_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_h_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_S_SYMBOL);
#endif

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_STATE;
                            }
                        }
                        break;

                        case VERSION_STATE:
                        {
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_d_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_E_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_DRIVER;
                            }
                        }
                        break;

                        case VERSION_DRIVER:
                            tmp = handler_motor_version_get();
                            for (uint8_t k = 0; k < 4; k++)
                            {
                                tmpBuff[k] = tmp % 16;
                                tmp /= 16;
                            }

                            handler_display_ssd_set(SSD_DIGIT_0_ID, tmpBuff[0]);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, tmpBuff[1]);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_d_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_DISPLAY;
                            }
                            break;

                        case VERSION_DISPLAY:
                        {
                            handler_display_ssd_set(SSD_DIGIT_0_ID, handler_display_version_get() % 10);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, handler_display_version_get() / 10);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_P_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_BOARD;
                            }
                        }
                        break;

                        default:
                            // do nothing
                            break;
                    }
                }
                break;

                case ERROR_COUNT_STATE_ERROR_TRAVERSE:
                {
                    display_currentError_put(app_testMode_errorDisplayedData_get().currentErrorDisplay);
                }
                break;

                case ERROR_COUNT_STATE_ERROR_DISPLAY:
                {
                    app_display_ssd_put(app_testMode_errorDisplayedData_get().currentErrorCnt, FORMAT_NUMBERS_TEST);
                }
                break;

                default:
                {
                    // do nothing
                }
                break;
            }
        }
        else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_CYCLE_COUNT_DISPLAY)
        {
            switch (app_testMode_cycleCntState_get())
            {
                case CYCLE_COUNT_STATE_IDLE:
                {
                    handler_display_clear();
                    versionDisplayCnt += period;

                    switch (versionState)
                    {
                        case VERSION_BOARD:
                            handler_display_ssd_set(SSD_DIGIT_0_ID, WM_SW_VERSION_MINOR);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, WM_SW_VERSION_MAJOR);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_U_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_MODEL;
                            }
                            break;

                        case VERSION_MODEL:
                        {
#if defined(WM_MODEL_11KG)
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_5_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_1_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_h_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_S_SYMBOL);
#elif defined(WM_MODEL_13KG)
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_7_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_1_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_h_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_S_SYMBOL);
#endif

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_STATE;
                            }
                        }
                        break;

                        case VERSION_STATE:
                        {
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_d_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_C_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_DRIVER;
                            }
                        }
                        break;

                        case VERSION_DRIVER:
                            tmp = handler_motor_version_get();
                            for (uint8_t k = 0; k < 4; k++)
                            {
                                tmpBuff[k] = tmp % 16;
                                tmp /= 16;
                            }

                            handler_display_ssd_set(SSD_DIGIT_0_ID, tmpBuff[0]);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, tmpBuff[1]);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_d_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_DISPLAY;
                            }
                            break;

                        case VERSION_DISPLAY:
                        {
                            handler_display_ssd_set(SSD_DIGIT_0_ID, handler_display_version_get() % 10);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, handler_display_version_get() / 10);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_P_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_BOARD;
                            }
                        }
                        break;

                        default:
                            // do nothing
                            break;
                    }
                }
                break;

                case CYCLE_COUNT_STATE_CYCLE_TRAVERSE:
                {
                    display_currentCourse_put(app_testMode_cycleDisplayedData_get().currentCycleDisplay - APP_NORMAL_MODE_LOG_COURSE_CYCLE_NUM_START);
                }
                break;

                case CYCLE_COUNT_STATE_CYCLE_DISPLAY:
                {
                    app_display_ssd_put(app_testMode_cycleDisplayedData_get().currentCycleCnt, FORMAT_NUMBERS_TEST);
                }
                break;

                default:
                {
                    // do nothing
                }
                break;
            }
        }
        else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_ELECTRICAL_PARTS)
        {
            switch (app_testMode_electricalPartsState_get())
            {
                case ELECTRICAL_PARTS_VERSION_CHECK:
                    handler_display_clear();
                    versionDisplayCnt += period;

                    switch (versionState)
                    {
                        case VERSION_BOARD:
                            handler_display_ssd_set(SSD_DIGIT_0_ID, WM_SW_VERSION_MINOR);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, WM_SW_VERSION_MAJOR);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_U_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_MODEL;
                            }
                            break;

                        case VERSION_MODEL:
                        {
#if defined(WM_MODEL_11KG)
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_5_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_1_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_h_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_S_SYMBOL);
#elif defined(WM_MODEL_13KG)
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_7_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_1_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_h_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_S_SYMBOL);
#endif

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_STATE;
                            }
                        }
                        break;

                        case VERSION_STATE:
                        {
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_P_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_E_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_DRIVER;
                            }
                        }
                        break;

                        case VERSION_DRIVER:
                            tmp = handler_motor_version_get();
                            for (uint8_t k = 0; k < 4; k++)
                            {
                                tmpBuff[k] = tmp % 16;
                                tmp /= 16;
                            }

                            handler_display_ssd_set(SSD_DIGIT_0_ID, tmpBuff[0]);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, tmpBuff[1]);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_d_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_DISPLAY;
                            }
                            break;

                        case VERSION_DISPLAY:
                        {
                            handler_display_ssd_set(SSD_DIGIT_0_ID, handler_display_version_get() % 10);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, handler_display_version_get() / 10);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_P_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_BOARD;
                            }
                        }
                        break;

                        default:
                            // do nothing
                            break;
                    }
                    break;

                case ELECTRICAL_PARTS_DISPLAY_CHECK:
                    handler_display_clear();
                    displayToggleCnt += period;

                    switch (displayState)
                    {
                        case DISPLAY_ON:
                            handler_display_fire();
                            if (displayToggleCnt >= 1000)
                            {
                                displayToggleCnt = 0;
                                displayState = DISPLAY_OFF;
                            }
                            break;

                        case DISPLAY_OFF:
                            handler_display_clear();
                            if (displayToggleCnt >= 1000)
                            {
                                displayToggleCnt = 0;
                                displayState = DISPLAY_ON;
                            }
                            break;

                        default:
                            // do nothing
                            break;
                    }
                    break;

                case ELECTRICAL_PARTS_WATERLEVEL_CHECK:
                    handler_display_clear();
                    app_display_ssd_put(handler_waterLevel_currentFreq_get() / 10, FORMAT_NUMBERS_TEST);
                    break;

                case ELECTRICAL_PARTS_MOTORCW_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_r_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, WATER_LEVEL_3);
                    break;

                case ELECTRICAL_PARTS_MOTORCWSTOP_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_G_SEG_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_G_SEG_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_G_SEG_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_G_SEG_SYMBOL);
                    break;

                case ELECTRICAL_PARTS_MOTORCCW_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_L_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, WATER_LEVEL_1);
                    break;

                case ELECTRICAL_PARTS_MOTORCCWSTOP_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_G_SEG_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_G_SEG_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_G_SEG_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_G_SEG_SYMBOL);
                    break;

                case ELECTRICAL_PARTS_DRAINVALVE_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_r_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_d_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, WATER_LEVEL_4);
                    break;

                case ELECTRICAL_PARTS_DRAINPUMP_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_P_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_d_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, WATER_LEVEL_2);
                    break;

                case ELECTRICAL_PARTS_HOTVALVE_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_H_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_LED_GROUP, WATER_TEMP_HOT);
                    break;

                case ELECTRICAL_PARTS_COLDVALVE_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_C_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_LED_GROUP, WATER_TEMP_COLD);
                    break;

                case ELECTRICAL_PARTS_SOFTNERVALVE_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_0_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_S_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_LED_GROUP, WATER_TEMP_WARM);
                    break;

#if WM_SHOWER_VALVE_ENABLED == 1
                case ELECTRICAL_PARTS_SHOWERVALVE_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_h_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_S_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                    handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 250);
                    break;
#endif

                case ELECTRICAL_PARTS_ENDURANCECOUNT_CHECK:
                    handler_display_clear();
                    app_display_ssd_put(app_memory_enduranceCnt_get(), FORMAT_NUMBERS);
                    break;

                default:
                    // do nothing
                    break;
            }
        }
        else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_UNBALANCE_SPIN)
        {
            switch (app_testMode_unbalanceSpinState_get())
            {
                case UNBALANCE_SPIN_STANDBY:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_t_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_b_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                    handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 250);
                    break;

                case UNBALANCE_SPIN_SPIN:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_t_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_b_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                    app_display_led_blink(HANDLER_DISPLAY_SPIN_LED_GROUP, period, 250, 700, 700);
                    break;

                case UNBALANCE_SPIN_BRAKE:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_t_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_b_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                    handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 250);
                    break;

                default:
                    // do nothing
                    break;
            }
        }
        else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_WATER_LEVEL_FREQ)
        {
            switch (app_testMode_waterLevelFreqState_get())
            {
                case WATER_LEVEL_FREQ_FILLING:
                    handler_display_clear();
                    app_display_ssd_put(handler_waterLevel_currentFreq_get() / 10, FORMAT_NUMBERS_TEST);

                    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_LED_GROUP, WATER_TEMP_COLD);
                    break;

                case WATER_LEVEL_FREQ_WASH:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_1_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_d_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    break;

                case WATER_LEVEL_FREQ_DRAIN:
                    handler_display_clear();
                    app_display_ssd_put(handler_waterLevel_currentFreq_get() / 10, FORMAT_NUMBERS_TEST);
                    break;

                default:
                    // do nothing
                    break;
            }
        }
        else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_PCB)
        {
            switch (app_testMode_pcbState_get())
            {
                case PCB_VERSION_CHECK:
                    handler_display_clear();
                    versionDisplayCnt += period;

                    switch (versionState)
                    {
                        case VERSION_BOARD:
                            handler_display_ssd_set(SSD_DIGIT_0_ID, WM_SW_VERSION_MINOR);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, WM_SW_VERSION_MAJOR);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_U_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_MODEL;
                            }
                            break;

                        case VERSION_MODEL:
                        {
#if defined(WM_MODEL_11KG)
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_5_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_1_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_h_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_S_SYMBOL);
#elif defined(WM_MODEL_13KG)
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_7_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_1_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_h_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_S_SYMBOL);
#endif

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_STATE;
                            }
                        }
                        break;

                        case VERSION_STATE:
                        {
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_b_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_C_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_P_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_DRIVER;
                            }
                        }
                        break;

                        case VERSION_DRIVER:
                            tmp = handler_motor_version_get();
                            for (uint8_t k = 0; k < 4; k++)
                            {
                                tmpBuff[k] = tmp % 16;
                                tmp /= 16;
                            }

                            handler_display_ssd_set(SSD_DIGIT_0_ID, tmpBuff[0]);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, tmpBuff[1]);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_d_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_DISPLAY;
                            }
                            break;

                        case VERSION_DISPLAY:
                        {
                            handler_display_ssd_set(SSD_DIGIT_0_ID, handler_display_version_get() % 10);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, handler_display_version_get() / 10);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_P_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_BOARD;
                            }
                        }
                        break;

                        default:
                            // do nothing
                            break;
                    }
                    break;

                case PCB_DISPLAY_CHECK:
                    handler_display_clear();
                    displayToggleCnt += period;

                    switch (displayState)
                    {
                        case DISPLAY_ON:
                            handler_display_fire();
                            if (displayToggleCnt >= 1000)
                            {
                                displayToggleCnt = 0;
                                displayState = DISPLAY_OFF;
                            }
                            break;

                        case DISPLAY_OFF:
                            handler_display_clear();
                            if (displayToggleCnt >= 1000)
                            {
                                displayToggleCnt = 0;
                                displayState = DISPLAY_ON;
                            }
                            break;

                        default:
                            // do nothing
                            break;
                    }
                    break;

                case PCB_WATER_LEVEL_SENSOR_CHECK:
                    handler_display_clear();
                    if (utils_thresholdValidity_check(handler_waterLevel_currentFreq_get(), HANDLER_WATER_LEVEL_DEFAULT_UPPER_THRESHOLD, HANDLER_WATER_LEVEL_DEFAULT_LOWER_THRESHOLD) == 1)
                    {
                        app_display_ssd_put(handler_waterLevel_currentFreq_get() / 10, FORMAT_NUMBERS_TEST);
                    }
                    else
                    {
                        handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_0_SYMBOL);
                        handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
                        handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                        handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    }

                    break;

                case PCB_DRAIN_PUMP_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_P_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_d_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    break;

                case PCB_HOT_VALVE_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_H_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    break;

                case PCB_SOFTENER_VALVE_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_0_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_S_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    break;

                case PCB_DRAIN_VALVE_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_r_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_d_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    break;

                case PCB_COLD_VALVE_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_C_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    break;

#if WM_SHOWER_VALVE_ENABLED == 1
                case PCB_SHOWER_VALVE_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_h_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_S_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    break;
#endif

                case PCB_MOTOR_CCW_CHECK:
                    handler_display_clear();

                    pcbMotorToggleMSec += period;
                    if (pcbMotorToggleMSec < 800)
                    {
                        if (handler_motor_speed_get(&motorSpeed) == HANDLER_MOTOR_ERROR_NONE)
                        {
                            app_display_ssd_put((motorSpeed), FORMAT_NUMBERS_TEST);
                        }
                    }
                    else
                    {
                        if (pcbMotorToggleMSec > 1600)
                        {
                            pcbMotorToggleMSec = 0;
                        }
                        handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_L_SYMBOL);
                        handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
                        handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                        handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    }
                    break;

                case PCB_MOTOR_CCW_STOP_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_G_SEG_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_G_SEG_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_G_SEG_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_G_SEG_SYMBOL);
                    break;

                case PCB_MOTOR_CW_CHECK:
                    handler_display_clear();

                    pcbMotorToggleMSec += period;
                    if (pcbMotorToggleMSec < 800)
                    {
                        if (handler_motor_speed_get(&motorSpeed) == HANDLER_MOTOR_ERROR_NONE)
                        {
                            app_display_ssd_put((motorSpeed), FORMAT_NUMBERS_TEST);
                        }
                    }
                    else
                    {
                        if (pcbMotorToggleMSec > 1600)
                        {
                            pcbMotorToggleMSec = 0;
                        }
                        handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_r_SYMBOL);
                        handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
                        handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                        handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    }
                    break;

                case PCB_MOTOR_CW_STOP_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_G_SEG_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_G_SEG_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_G_SEG_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_G_SEG_SYMBOL);
                    break;

                case PCB_HEAVY_LED_CHECK:
                    handler_display_clear();
                    handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_HEAVY);
                    break;

                case PCB_COTTON_LED_CHECK:
                    handler_display_clear();
                    handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_COTTON);
                    break;

                case PCB_REGULAR_LED_CHECK:
                    handler_display_clear();
                    handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_REGULAR);
                    break;

                case PCB_QUICK_WASH_LED_CHECK:
                    handler_display_clear();
                    handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_QUICK_WASH);
                    break;

                case PCB_JEANS_LED_CHECK:
                    handler_display_clear();
                    handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_JEANS);
                    break;

                case PCB_BLANKET_LED_CHECK:
                    handler_display_clear();
                    handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_BLANKET);
                    break;

                case PCB_MEMS_CHECK:
                    handler_display_clear();
                    app_display_ssd_put(handler_mems_yValue_get(), FORMAT_NUMBERS_TEST);
                    break;

                case PCB_LID_SENSOR_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_d_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_I_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_L_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    if (handler_lidSensor_state_get() == 1)
                    {
                        handler_display_ledGroup_set(HANDLER_DISPLAY_CHILD_LOCK_LED_GROUP, 250);
                    }
                    else
                    {
                        handler_display_ledGroup_set(HANDLER_DISPLAY_CHILD_LOCK_LED_GROUP, CHILD_LOCK_OFF);
                    }
                    break;

                case PCB_LID_LOCK_ON_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_n_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_0_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    handler_display_ledGroup_set(HANDLER_DISPLAY_LID_LOCK_LED_GROUP, handler_lockMotor_state_get());
                    break;

                case PCB_LID_LOCK_OFF_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_F_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_F_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_0_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    handler_display_ledGroup_set(HANDLER_DISPLAY_LID_LOCK_LED_GROUP, handler_lockMotor_state_get());
                    break;

                default:
                    // do nothing
                    break;
            }
        }
        else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_MIDDLE_INSPECTION)
        {
            switch (app_testMode_middleInspectionState_get())
            {
                case MIDDLE_INSPECTION_STANDBY:
                    handler_display_clear();
                    versionDisplayCnt += period;

                    switch (versionState)
                    {
                        case VERSION_BOARD:
                            handler_display_ssd_set(SSD_DIGIT_0_ID, WM_SW_VERSION_MINOR);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, WM_SW_VERSION_MAJOR);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_U_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_MODEL;
                            }
                            break;

                        case VERSION_MODEL:
                        {
#if defined(WM_MODEL_11KG)
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_5_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_1_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_h_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_S_SYMBOL);
#elif defined(WM_MODEL_13KG)
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_7_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_1_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_h_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_S_SYMBOL);
#endif

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_STATE;
                            }
                        }
                        break;

                        case VERSION_STATE:
                        {
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_0_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_C_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_DRIVER;
                            }
                        }
                        break;

                        case VERSION_DRIVER:
                            tmp = handler_motor_version_get();
                            for (uint8_t k = 0; k < 4; k++)
                            {
                                tmpBuff[k] = tmp % 16;
                                tmp /= 16;
                            }

                            handler_display_ssd_set(SSD_DIGIT_0_ID, tmpBuff[0]);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, tmpBuff[1]);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_d_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_DISPLAY;
                            }
                            break;

                        case VERSION_DISPLAY:
                        {
                            handler_display_ssd_set(SSD_DIGIT_0_ID, handler_display_version_get() % 10);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, handler_display_version_get() / 10);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_P_SYMBOL);

                            if (versionDisplayCnt >= 800)
                            {
                                versionDisplayCnt = 0;
                                versionState = VERSION_BOARD;
                            }
                        }
                        break;

                        default:
                            // do nothing
                            break;
                    }
                    break;

                case MIDDLE_INSPECTION_WATER_VALVE_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_1_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_C_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    if (app_testMode_middleInspectionWaterValveState_get() == MIDDLE_INSPECTION_LID_LOCK_CHECK)
                    {
                        handler_display_ledGroup_set(HANDLER_DISPLAY_LID_LOCK_LED_GROUP, handler_lockMotor_state_get());
                    }
                    break;

                case MIDDLE_INSPECTION_WATER_LEVEL_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_2_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_C_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    break;

                case MIDDLE_INSPECTION_LOAD_SENSE:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_3_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_C_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    break;

                case MIDDLE_INSPECTION_WASHING:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_4_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_C_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    break;

                case MIDDLE_INSPECTION_DRAIN:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_5_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_C_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    break;

                case MIDDLE_INSPECTION_DRAIN_HOLD:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_6_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_C_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    break;

                case MIDDLE_INSPECTION_SPIN:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_7_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_C_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    break;

                case MIDDLE_INSPECTION_DONE:
                    // do nothing
                    break;

                case MIDDLE_INSPECTION_ERROR:
                    handler_display_clear();
                    switch (app_testMode_middleInspectionError_get())
                    {
                        case MIDDLE_INSPECTION_ERROR_NONE:
                            // do nothing
                            break;

                        case MIDDLE_INSPECTION_ERROR_WATER_LEVEL:
                            handler_display_clear();
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_5_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_E_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                            break;

                        case MIDDLE_INSPECTION_ERROR_MOTOR_ROTATION:
                            handler_display_clear();
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_7_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_E_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                            break;

                        case MIDDLE_INSPECTION_ERROR_DRAIN:
                            handler_display_clear();
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_1_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_E_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                            break;

                        case MIDDLE_INSPECTION_ERROR_LID:
                            handler_display_clear();
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_2_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_E_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                            break;

                        default:
                            // do nothing
                            break;
                    }
                    break;

                default:
                    // do nothing
                    break;
            }
        }
        else if (app_userSelection_data_get()->testModeSelection == TEST_MODE_SELECTION_FINAL_INSPECTION)
        {
            switch (app_testMode_finalInspectionState_get())
            {
                case FINAL_INSPECTION_STANDBY:
                {
                    switch (app_testMode_finalInspectionDataDisplayType_get())
                    {
                        case FINAL_INSPECTION_SPIN_DISPLAY_NORMAL:
                        {
                            handler_display_clear();
                            versionDisplayCnt += period;

                            switch (versionState)
                            {
                                case VERSION_BOARD:
                                    handler_display_ssd_set(SSD_DIGIT_0_ID, WM_SW_VERSION_MINOR);
                                    handler_display_ssd_set(SSD_DIGIT_1_ID, WM_SW_VERSION_MAJOR);
                                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_U_SYMBOL);

                                    if (versionDisplayCnt >= 800)
                                    {
                                        versionDisplayCnt = 0;
                                        versionState = VERSION_MODEL;
                                    }
                                    break;

                                case VERSION_MODEL:
                                {
#if defined(WM_MODEL_11KG)
                                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_5_SYMBOL);
                                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_1_SYMBOL);
                                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_h_SYMBOL);
                                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_S_SYMBOL);
#elif defined(WM_MODEL_13KG)
                                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_7_SYMBOL);
                                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_1_SYMBOL);
                                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_h_SYMBOL);
                                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_S_SYMBOL);
#endif

                                    if (versionDisplayCnt >= 800)
                                    {
                                        versionDisplayCnt = 0;
                                        versionState = VERSION_STATE;
                                    }
                                }
                                break;

                                case VERSION_STATE:
                                {
                                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_0_SYMBOL);
                                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_d_SYMBOL);
                                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

                                    if (versionDisplayCnt >= 800)
                                    {
                                        versionDisplayCnt = 0;
                                        versionState = VERSION_DRIVER;
                                    }
                                }
                                break;

                                case VERSION_DRIVER:
                                    tmp = handler_motor_version_get();
                                    for (uint8_t k = 0; k < 4; k++)
                                    {
                                        tmpBuff[k] = tmp % 16;
                                        tmp /= 16;
                                    }

                                    handler_display_ssd_set(SSD_DIGIT_0_ID, tmpBuff[0]);
                                    handler_display_ssd_set(SSD_DIGIT_1_ID, tmpBuff[1]);
                                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_d_SYMBOL);

                                    if (versionDisplayCnt >= 800)
                                    {
                                        versionDisplayCnt = 0;
                                        versionState = VERSION_DISPLAY;
                                    }
                                    break;

                                case VERSION_DISPLAY:
                                {
                                    handler_display_ssd_set(SSD_DIGIT_0_ID, handler_display_version_get() % 10);
                                    handler_display_ssd_set(SSD_DIGIT_1_ID, handler_display_version_get() / 10);
                                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_CENTER_DASH);
                                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_P_SYMBOL);

                                    if (versionDisplayCnt >= 800)
                                    {
                                        versionDisplayCnt = 0;
                                        versionState = VERSION_BOARD;
                                    }
                                }
                                break;

                                default:
                                    // do nothing
                                    break;
                            }
                        }
                        break;

                        case FINAL_INSPECTION_SPIN_DISPLAY_MEMS_X:
                        {
                            handler_display_clear();
                            app_display_ssd_put(handler_mems_xThresholdValue_get() / 10, FORMAT_NUMBERS_TEST);
                        }
                        break;

                        case FINAL_INSPECTION_SPIN_DISPLAY_MEMS_Y:
                        {
                            handler_display_clear();
                            app_display_ssd_put(handler_mems_yThresholdValue_get() / 10, FORMAT_NUMBERS_TEST);
                        }
                        break;

                        case FINAL_INSPECTION_SPIN_DISPLAY_MEMS_Z:
                        {
                            handler_display_clear();
                            app_display_ssd_put(handler_mems_zThresholdValue_get() / 10, FORMAT_NUMBERS_TEST);
                        }
                        break;

                        case FINAL_INSPECTION_SPIN_DISPLAY_MEMS_NORMAL_HIT:
                        {
                            handler_display_clear();
                            app_display_ssd_put(handler_mems_hitCnt_get(), FORMAT_NUMBERS_TEST);
                        }
                        break;

                        case FINAL_INSPECTION_SPIN_DISPLAY_MEMS_CRITICAL_HIT:
                        {
                            handler_display_clear();
                            app_display_ssd_put(handler_mems_critHitCnt_get(), FORMAT_NUMBERS_TEST);
                        }
                        break;

                        default:
                            // do nothing
                            break;
                    }
                }
                break;

                case FINAL_INSPECTION_DRAIN:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_1_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_d_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    break;

                case FINAL_INSPECTION_LID_CLOSE_CHECK:
                    handler_display_clear();
                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_2_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_d_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    break;

                case FINAL_INSPECTION_SPIN:
                    if (handler_motor_speed_get(&motorSpeed) == HANDLER_MOTOR_ERROR_NONE)
                    {
                        if (motorSpeed > 650)
                        {
                            app_display_ssd_put((motorSpeed), FORMAT_NUMBERS_TEST);
                        }
                        else
                        {
                            handler_display_clear();
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_3_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_d_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                        }
                    }
                    break;

                case FINAL_INSPECTION_BRAKING:
                    if (app_testMode_finalInspectionBrakingTimeMSec_get() >= 20000)
                    {
                        handler_display_clear();
                        handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_9_SYMBOL);
                        handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_F_SYMBOL);
                        handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                        handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    }
                    else
                    {
                        handler_display_clear();
                        app_display_ssd_put(app_testMode_finalInspectionBrakingTimeMSec_get() / 100, FORMAT_NUMBERS_TEST);
                    }
                    break;

                case FINAL_INSPECTION_MECH_RANKING_WATER_LEVEL_CALIBRATION:
                    switch (app_testMode_finalInspectionDisplayState_get())
                    {
                        case FINAL_INSPECTION_WEIGHT_DISPLAY:
                            handler_display_clear();
                            app_display_ssd_put(handler_motor_testModePulseCount_get(), FORMAT_NUMBERS_TEST);
                            break;

                        case FINAL_INSPECTION_MECH_RANK_DISPLAY:
                            if (app_testMode_finalInspectionMechRank_get() == 0)
                            {
                                handler_display_clear();
                                handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_0_SYMBOL);
                                handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_0_SYMBOL);
                                handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                            }
                            else if (app_testMode_finalInspectionMechRank_get() == 0xff)
                            {
                                handler_display_clear();
                                handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_F_SYMBOL);
                                handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_F_SYMBOL);
                                handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                                handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                            }
                            break;

                        case FINAL_INSPECTION_STATE_DISPLAY:
                            handler_display_clear();
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_8_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_d_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                            break;

                        default:
                            // do nothing
                            break;
                    }
                    break;

                case FINAL_INSPECTION_WEIGHT_SENSOR_CHECK:
                    switch (app_testMode_finalInspectionWeightSensorState_get())
                    {
                        case FINAL_INSPECTION_WEIGHT_SENSOR_OK:
                            handler_display_clear();
#if defined(WM_MODEL_11KG)
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_5_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_1_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_h_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_S_SYMBOL);
#elif defined(WM_MODEL_13KG)
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_7_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_1_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_h_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_S_SYMBOL);
#endif
                            break;

                        case FINAL_INSPECTION_WEIGHT_SENSOR_NOK:
                            handler_display_clear();
                            handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_E_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_S_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_L_SYMBOL);
                            handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                            break;

                        default:
                            // do nothing
                            break;
                    }
                    break;

                case FINAL_INSPECTION_TOTAL_TIME_CHECK:
                    gu32_finalInspection_totalTimeMins = (app_testMode_finalInspectionTotalTimeMSec_get() / 1000) / 60;
                    gu32_finalInspection_totalTimeSecs = (app_testMode_finalInspectionTotalTimeMSec_get() / 1000) % 60;

                    handler_display_ssd_set(SSD_DIGIT_0_ID, gu32_finalInspection_totalTimeSecs % 10);
                    handler_display_ssd_set(SSD_DIGIT_1_ID, gu32_finalInspection_totalTimeSecs / 10);
                    handler_display_ssd_set(SSD_DIGIT_2_ID, gu32_finalInspection_totalTimeMins);
                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                    handler_display_ssdDP_set(SSD_DP_ON);
                    break;

                case FINAL_INSPECTION_ERROR:
                    handler_display_clear();
                    switch (app_testMode_finalInspectionError_get())
                    {
                        case FINAL_INSPECTION_ERROR_NONE:
                            // do nothing
                            break;

                        case FINAL_INSPECTION_ERROR_UNBALANCE:
                            switch (app_testMode_finalInspectionDataDisplayType_get())
                            {
                                case FINAL_INSPECTION_SPIN_DISPLAY_NORMAL:
                                {
                                    handler_display_clear();
                                    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_3_SYMBOL);
                                    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_E_SYMBOL);
                                    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
                                    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);
                                }
                                break;

                                case FINAL_INSPECTION_SPIN_DISPLAY_MEMS_X:
                                {
                                    handler_display_clear();
                                    app_display_ssd_put(handler_mems_faultValue_get(APP_MEMS_FAULT_X) / 10, FORMAT_NUMBERS_TEST);
                                }
                                break;

                                case FINAL_INSPECTION_SPIN_DISPLAY_MEMS_Y:
                                {
                                    handler_display_clear();
                                    app_display_ssd_put(handler_mems_faultValue_get(APP_MEMS_FAULT_Y) / 10, FORMAT_NUMBERS_TEST);
                                }
                                break;

                                case FINAL_INSPECTION_SPIN_DISPLAY_MEMS_Z:
                                {
                                    handler_display_clear();
                                    app_display_ssd_put(handler_mems_faultValue_get(APP_MEMS_FAULT_Z) / 10, FORMAT_NUMBERS_TEST);
                                }
                                break;

                                case FINAL_INSPECTION_SPIN_DISPLAY_MEMS_NORMAL_HIT:
                                {
                                    handler_display_clear();
                                    app_display_ssd_put(handler_mems_hitCnt_get(), FORMAT_NUMBERS_TEST);
                                }
                                break;

                                case FINAL_INSPECTION_SPIN_DISPLAY_MEMS_CRITICAL_HIT:
                                {
                                    handler_display_clear();
                                    app_display_ssd_put(handler_mems_critHitCnt_get(), FORMAT_NUMBERS_TEST);
                                }
                                break;

                                default:
                                    // do nothing
                                    break;
                            }
                            break;

                        default:
                            // do nothing
                            break;
                    }
                    break;

                default:
                    // do nothing
                    break;
            }
        }

    static void display_currentCourse_put(uint8_t course)
{
    switch (course)
    {
        case COURSE_REGULAR:
            app_error_handling_displayError(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_r_SYMBOL, SSD_e_SYMBOL);
            handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_REGULAR);
            break;
        case COURSE_QUICK_WASH:
            app_error_handling_displayError(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_q_SYMBOL, SSD_TRIPLE_DASH_SYMBOL);
            handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_QUICK_WASH);
            break;
        case COURSE_BABYCARE:
            app_error_handling_displayError(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_b_SYMBOL, SSD_c_SYMBOL);
            handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_BABYCARE);
            break;
        case COURSE_DELICATE:
            app_error_handling_displayError(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_d_SYMBOL, SSD_e_SYMBOL);
            handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_DELICATE);
            break;
        case COURSE_WOOL:
            app_error_handling_displayError(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_TRIPLE_DASH_SYMBOL, SSD_o_SYMBOL);
            handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_WOOL);
            break;
        case COURSE_JEANS:
            app_error_handling_displayError(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_J_SYMBOL, SSD_e_SYMBOL);
            handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_JEANS);
            break;
        case COURSE_COTTON:
            app_error_handling_displayError(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_c_SYMBOL, SSD_o_SYMBOL);
            handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_COTTON);
            break;
        case COURSE_HEAVY:
            app_error_handling_displayError(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_h_SYMBOL, SSD_u_SYMBOL);
            handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_HEAVY);
            break;
        case COURSE_BLANKET:
            app_error_handling_displayError(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_b_SYMBOL, SSD_L_SYMBOL);
            handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_BLANKET);
            break;
        case COURSE_TUBCLEAN:
            app_error_handling_displayError(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_t_SYMBOL, SSD_c_SYMBOL);
            handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_TUBCLEAN);
            break;
        case COURSE_SPORTS_WEAR:
            app_error_handling_displayError(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_S_SYMBOL, SSD_TRIPLE_DASH_SYMBOL);
            handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_SPORTS_WEAR);
            break;
        case COURSE_QUICK_RINSE:
            app_error_handling_displayError(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_q_SYMBOL, SSD_r_SYMBOL);
            handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, COURSE_QUICK_RINSE);
            break;
        default:
            app_error_handling_displayError(SSD_E_SYMBOL, SSD_r_SYMBOL, SSD_r_SYMBOL, SSD_r_SYMBOL);
            break;
    }
}

static void app_error_handling_displayError(ssd_symbolEnum_t ssd1, ssd_symbolEnum_t ssd2, ssd_symbolEnum_t ssd3, ssd_symbolEnum_t ssd4)
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

#endif
