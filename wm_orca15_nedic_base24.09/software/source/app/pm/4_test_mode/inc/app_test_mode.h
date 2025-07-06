#ifndef APP_TEST_MODE_H
#define APP_TEST_MODE_H

#include "def.h"

#include "app_processor.h"

#define APP_TEST_MODE_FUNCTION_TEST_MAX_NUM (25)

extern uint8_t *fnSW[APP_TEST_MODE_FUNCTION_TEST_MAX_NUM];

typedef enum app_tm_state
{
    APP_TEST_MODE_INIT,
    APP_TEST_MODE_OPERATION,
    APP_TEST_MODE_DONE
} app_tm_state_t;

typedef struct app_tm_keyConfig
{
    void *fnSW[APP_TEST_MODE_FUNCTION_TEST_MAX_NUM];
    uint8_t fnNum;
} app_tm_keyConfig_t;

typedef enum app_tm_type
{
    APP_TM_NONE,
    APP_TM_WATER_LEVEL_CALIBRATION,
    APP_TM_WEIGHT_CALIBRATION,
    APP_TM_LV_CALIBRATION,
    APP_TM_ELECTRICAL_PARTS,
    APP_TM_UNBALANCE_SPIN,
    APP_TM_WATER_LEVEL_FREQ,
    APP_TM_PCB,
    APP_TM_MIDDLE_INSPECTION,
    APP_TM_FINAL_INSPECTION,
    APP_TM_ERROR_COUNT_DISPLAY,
    APP_TM_CYCLE_COUNT_DISPLAY
} app_tm_type_t;

typedef enum app_tm_weightCalibrationEnum
{
    TM_WEIGHTDETECTION_CALIBRATION_STATE_INIT,
    TM_WEIGHTDETECTION_CALIBRATION_STATE_CALIBRATION,
    TM_WEIGHTDETECTION_CALIBRATION_STATE_ERROR,
    TM_WEIGHTDETECTION_CALIBRATION_STATE_SAVING,
    TM_WEIGHTDETECTION_CALIBRATION_STATE_FINISH,
    TM_WEIGHTDETECTION_CALIBRATION_STATE_DONE
} app_tm_weightCalibrationEnum_t;

typedef enum app_tm_electricalParts_state
{
    TM_ELECTRICAL_PARTS_VERSION_CHECK,
    TM_ELECTRICAL_PARTS_DISPLAY_CHECK,
    TM_ELECTRICAL_PARTS_WATERLEVEL_CHECK,
    TM_ELECTRICAL_PARTS_MOTORCW_CHECK,
    TM_ELECTRICAL_PARTS_MOTORCWSTOP_CHECK,
    TM_ELECTRICAL_PARTS_MOTORCCW_CHECK,
    TM_ELECTRICAL_PARTS_MOTORCCWSTOP_CHECK,
    TM_ELECTRICAL_PARTS_DRAINVALVE_CHECK,
    TM_ELECTRICAL_PARTS_HOTVALVE_CHECK,
    TM_ELECTRICAL_PARTS_COLDVALVE_CHECK,
    TM_ELECTRICAL_PARTS_SOFTNERVALVE_CHECK,
#if defined(WM_SHOWER_VALVE_ENABLED)
    TM_ELECTRICAL_PARTS_SHOWERVALVE_CHECK,
#endif
#ifdef WM_CONDENSER_VALVE_ENABLED
    TM_ELECTRICAL_PARTS_CONDENSERVALVE_CHECK,
#endif
    TM_ELECTRICAL_PARTS_ENDURANCECOUNT_CHECK,
    TM_ELECTRICAL_PARTS_STATE_NUM
} app_tm_electricalParts_state_t;

typedef enum app_tm_unbalanceSpin_state
{
    TM_UNBALANCE_SPIN_STANDBY,
    TM_UNBALANCE_SPIN_SPIN,
    TM_UNBALANCE_SPIN_BRAKE
} app_tm_unbalanceSpin_state_t;

typedef enum app_tm_waterLevelFreq_state
{
    TM_WATER_LEVEL_FREQ_FILLING,
    TM_WATER_LEVEL_FREQ_WASH,
    TM_WATER_LEVEL_FREQ_DRAIN,
    TM_WATER_LEVEL_FREQ_STATE_NUM
} app_tm_waterLevelFreq_state_t;

typedef enum app_tm_pcb_state
{
    TM_PCB_VERSION_CHECK,
    TM_PCB_DISPLAY_CHECK,
    TM_PCB_WATER_LEVEL_SENSOR_CHECK,
    TM_PCB_DRAIN_PUMP_CHECK,
    TM_PCB_HOT_VALVE_CHECK,
    TM_PCB_SOFTENER_VALVE_CHECK,
    TM_PCB_DRAIN_VALVE_CHECK,
    TM_PCB_COLD_VALVE_CHECK,
#if defined(WM_SHOWER_VALVE_ENABLED)
    TM_PCB_SHOWER_VALVE_CHECK,
#endif
    TM_PCB_MOTOR_CCW_CHECK,
    TM_PCB_MOTOR_CCW_STOP_CHECK,
    TM_PCB_MOTOR_CW_CHECK,
    TM_PCB_MOTOR_CW_STOP_CHECK,
    TM_PCB_MEMS_CHECK,
    TM_PCB_LID_SENSOR_CHECK,
    TM_PCB_LID_LOCK_ON_CHECK,
    TM_PCB_LID_LOCK_OFF_CHECK
} app_tm_pcb_state_t;

typedef enum app_tm_middleInspection_state
{
    TM_MIDDLE_INSPECTION_STANDBY,
    TM_MIDDLE_INSPECTION_WATER_VALVE_CHECK,
    TM_MIDDLE_INSPECTION_WATER_LEVEL_CHECK,
    TM_MIDDLE_INSPECTION_LOAD_SENSE,
    TM_MIDDLE_INSPECTION_WASHING,
    TM_MIDDLE_INSPECTION_DRAIN,
    TM_MIDDLE_INSPECTION_DRAIN_HOLD,
    TM_MIDDLE_INSPECTION_SPIN,
    TM_MIDDLE_INSPECTION_ERROR,
    TM_MIDDLE_INSPECTION_DONE
} app_tm_middleInspection_state_t;

typedef enum app_tm_middleInspection_valveCheckState
{
    TM_MIDDLE_INSPECTION_NONE_VALVE_CHECK,
    TM_MIDDLE_INSPECTION_COLD_VALVE_CHECK,
    TM_MIDDLE_INSPECTION_HOT_VALVE_CHECK,
    TM_MIDDLE_INSPECTION_SOFTENER_VALVE_CHECK,
#if defined(WM_SHOWER_VALVE_ENABLED)
    TM_MIDDLE_INSPECTION_SHOWER_VALVE_CHECK,
#endif
    TM_MIDDLE_INSPECTION_LID_LOCK_CHECK
} app_tm_middleInspection_valveCheckState_t;

typedef enum app_tm_middleInspectionError
{
    TM_MIDDLE_INSPECTION_ERROR_NONE,
    TM_MIDDLE_INSPECTION_ERROR_WATER_LEVEL,      // E5
    TM_MIDDLE_INSPECTION_ERROR_MOTOR_ROTATION,   // E7
    TM_MIDDLE_INSPECTION_ERROR_DRAIN,            // E1
    TM_MIDDLE_INSPECTION_ERROR_LID,              // E2
} app_tm_middleInspectionError_t;

typedef enum app_tm_finalInspection_state
{
    TM_FINAL_INSPECTION_STANDBY,
    TM_FINAL_INSPECTION_DRAIN,
    TM_FINAL_INSPECTION_LID_CLOSE_CHECK,
    TM_FINAL_INSPECTION_SPIN,
    TM_FINAL_INSPECTION_BRAKING,
    TM_FINAL_INSPECTION_LED_FLASH,
    TM_FINAL_INSPECTION_MECH_RANKING_WATER_LEVEL_CALIBRATION,
    TM_FINAL_INSPECTION_WEIGHT_SENSOR_CHECK,
    TM_FINAL_INSPECTION_TOTAL_TIME_CHECK,
    TM_FINAL_INSPECTION_ERROR,
    TM_FINAL_INSPECTION_DONE
} app_tm_finalInspection_state_t;

typedef enum app_tm_finalInspectionSpinDisplay
{
    TM_FINAL_INSPECTION_SPIN_DISPLAY_NORMAL,
    TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_X,
    TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_Y,
    TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_Z,
    TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_NORMAL_HIT,
    TM_FINAL_INSPECTION_SPIN_DISPLAY_MEMS_CRITICAL_HIT
} app_tm_finalInspectionSpinDisplay_t;

typedef enum app_tm_finalInspectionError
{
    TM_FINAL_INSPECTION_ERROR_NONE,
    TM_FINAL_INSPECTION_ERROR_UNBALANCE,   // E3
} app_tm_finalInspectionError_t;

typedef enum app_tm_finalInspectionMechRankDisplayState
{
    TM_FINAL_INSPECTION_WEIGHT_DISPLAY,
    TM_FINAL_INSPECTION_MECH_RANK_DISPLAY,
    TM_FINAL_INSPECTION_STATE_DISPLAY
} app_tm_finalInspectionMechRankDisplayState_t;

typedef enum app_tm_finalInspectionWeightSensorState
{
    TM_FINAL_INSPECTION_WEIGHT_SENSOR_OK,
    TM_FINAL_INSPECTION_WEIGHT_SENSOR_NOK
} app_tm_finalInspectionWeightSensorState_t;

typedef enum app_tm_errorCount_state
{
    TM_ERROR_COUNT_STATE_IDLE,
    TM_ERROR_COUNT_STATE_ERROR_TRAVERSE,
    TM_ERROR_COUNT_STATE_ERROR_DISPLAY,
    TM_ERROR_COUNT_STATE_NUM
} app_tm_errorCount_state_t;

typedef struct app_tm_errorCntData
{
    uint8_t currentErrorDisplay;
    uint16_t currentErrorCnt;
} app_tm_errorCntData_t;

typedef enum app_tm_cycleCount_state
{
    TM_CYCLE_COUNT_STATE_IDLE,
    TM_CYCLE_COUNT_STATE_CYCLE_TRAVERSE,
    TM_CYCLE_COUNT_STATE_CYCLE_DISPLAY,
    TM_CYCLE_COUNT_STATE_NUM
} app_tm_cycleCount_state_t;
typedef struct app_tm_cycleCntData
{
    uint8_t currentCycleDisplay;
    uint16_t currentCycleCnt;
} app_tm_cycleCntData_t;

app_processor_state_t app_testMode_process(uint32_t period);

uint8_t app_tm_entry_check(uint32_t period);

app_tm_type_t app_tm_type_get(void);
void app_tm_type_set(app_tm_type_t type);

app_tm_state_t app_tm_state_get(void);
void app_tm_state_set(app_tm_state_t testModeState);

void app_tm_waterLevelCalibration_process(uint32_t period);

void app_tm_lvCalibrationTest_process(uint32_t period);

void app_tm_weightDetectionCalibration_process(uint32_t period);
app_tm_weightCalibrationEnum_t app_tm_weightCalibrationState_get(void);

void app_tm_electricalParts_process(uint32_t period);
app_tm_electricalParts_state_t app_tm_electricalPartsState_get(void);

void app_tm_unbalanceSpin_process(uint32_t period);
app_tm_unbalanceSpin_state_t app_tm_unbalanceSpinState_get(void);

void app_tm_waterLevelFreq_process(uint32_t period);
app_tm_waterLevelFreq_state_t app_tm_waterLevelFreqState_get(void);

void app_tm_pcb_process(uint32_t period);
app_tm_pcb_state_t app_tm_pcbState_get(void);

void app_tm_middleInspection_process(uint32_t period);
app_tm_middleInspection_state_t app_tm_middleInspectionState_get(void);
app_tm_middleInspectionError_t app_tm_middleInspectionError_get(void);
app_tm_middleInspection_valveCheckState_t app_tm_middleInspectionWaterValveState_get(void);

void app_tm_finalInspection_process(uint32_t period);
app_tm_finalInspection_state_t app_tm_finalInspectionState_get(void);
app_tm_finalInspectionError_t app_tm_finalInspectionError_get(void);
uint32_t app_tm_finalInspectionBrakingTimeMSec_get(void);
uint32_t app_tm_finalInspectionMechRank_get(void);
app_tm_finalInspectionMechRankDisplayState_t app_tm_finalInspectionDisplayState_get(void);
app_tm_finalInspectionWeightSensorState_t app_tm_finalInspectionWeightSensorState_get(void);
uint32_t app_tm_finalInspectionTotalTimeMSec_get(void);
app_tm_finalInspectionSpinDisplay_t app_tm_finalInspectionDataDisplayType_get(void);

void app_tm_errorCnt_process(uint32_t period);
app_tm_errorCount_state_t app_tm_errorCntState_get(void);
app_tm_errorCntData_t app_tm_errorDisplayedData_get(void);

void app_tm_cycleCnt_process(uint32_t period);
app_tm_cycleCount_state_t app_tm_cycleCntState_get(void);
app_tm_cycleCntData_t app_tm_cycleDisplayedData_get(void);

#endif
