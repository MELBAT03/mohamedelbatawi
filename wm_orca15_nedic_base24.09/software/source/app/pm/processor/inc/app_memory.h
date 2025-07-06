#ifndef APP_MEMORY_H
#define APP_MEMORY_H 1

#include "def.h"

typedef enum app_memory_init_state
{
    MEMORY_INIT_READ,
    MEMORY_INIT_PROCESS,
    MEMORY_INIT_DONE
} app_memory_init_state_t;

typedef enum app_memory_storingState
{
    MEMORY_STORING_IN_PROGRESS = 0,
    MEMORY_STORING_DONE = 1
} app_memory_storingState_t;

typedef enum app_memory_shutdownState
{
    MEMORY_SHUTDOWN_NONE = 0x00,
    MEMORY_SHUTDOWN_NORMAL = 0xaa,
    MEMORY_SHUTDOWN_ABNORMAL = 0xee,
    MEMORY_SHUTDOWN_SKIP = 0xbb
} app_memory_shutdownState_t;

typedef enum app_memory_storeType
{
    MEMORY_STORE_TYPE_CONTINUOUS = 0,
    MEMORY_STORE_TYPE_PERIODIC = 1
} app_memory_storeType_t;

typedef enum app_memory_autoDetergentFirstTime
{
    MEMORY_AUTO_DETERGENT_FIRST_TIME_NOT_RECORDED = 0,
    MEMORY_AUTO_DETERGENT_FIRST_TIME_RECORDED = 0xee
} app_memory_autoDetergentFirstTime_t;

typedef enum app_memory_layout
{
    MEMORY_LAYOUT_SHUT_DOWN_STATE_LOCATION,

    MEMORY_LAYOUT_COURSE_LOCATION,
    MEMORY_LAYOUT_OPERATION_LOCATION,
    MEMORY_LAYOUT_DLEAY_START_LOCATION,
    MEMORY_LAYOUT_CHILD_LOCK_LOCATION,
    MEMORY_LAYOUT_CYCLE_TYPE_LOCATION,
    MEMORY_LAYOUT_TUBCLEAN_LOCATION,

    MEMORY_LAYOUT_WATER_TEMP_LOCATION,
    MEMORY_LAYOUT_WATER_LEVEL_LOCATION,
    MEMORY_LAYOUT_SOAK_LOCATION,
    MEMORY_LAYOUT_WASH_LOCATION,
    MEMORY_LAYOUT_STEAM_LOCATION,
    MEMORY_LAYOUT_RINSE_LOCATION,
    MEMORY_LAYOUT_ERINSE_LOCATION,
    MEMORY_LAYOUT_SPIN_LOCATION,
    MEMORY_LAYOUT_SOIL_LEVEL_LOCATION,
    MEMORY_LAYOUT_SUPER_SPIN_LOCATION,
    MEMORY_LAYOUT_ANTIWRINKLE_LOCATION,
    MEMORY_LAYOUT_WATER_HEATING_LOCATION,
    MEMORY_LAYOUT_STEAM_TECH_LOCATION,
    MEMORY_LAYOUT_GEL_DETERGENT_LOCATION,

    MEMORY_LAYOUT_DRY_TIME_LOCATION,
    MEMORY_LAYTOUT_DRY_INTENSITY_LOCATION,
    MEMORY_LAYTOUT_DRY_WATER_LEVEL_LOCATION,

    MEMORY_LAYOUT_NM_STATE_LOCATION,
    MEMORY_LAYOUT_NM_INIT_STATE_LOCATION,
    MEMORY_LAYOUT_NM_WASH_STATE_LOCATION,
    MEMORY_LAYOUT_WASH_SETUP_STATE_LOCATION,
    MEMORY_LAYOUT_WASH_SOAK_STATE_LOCATION,
    MEMORY_LAYOUT_WASH_WASH_STATE_LOCATION,
    MEMORY_LAYOUT_WASH_STEAM_STATE_LOCATION,
    MEMORY_LAYOUT_WASH_RINSE_STATE_LOCATION,
    MEMORY_LAYOUT_WASH_SPIN_STATE_LOCATION,
    MEMORY_LAYOUT_NM_DRY_STATE_LOCATION,
    MEMORY_LAYOUT_DRY_PRESETUP_STATE_LOCATION,
    MEMORY_LAYOUT_DRY_SETUP_STATE_LOCATION,
    MEMORY_LAYOUT_DRY_OPERATION_STATE_LOCATION,

    MEMORY_LAYOUT_TUBCLEAN_STATE_LOCATION,
    MEMORY_LAYOUT_TUBCLEAN_NUM_LOCATION,

    MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_HIGH_LOCATION,
    MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_LOW_LOCATION,

    MEMORY_LAYOUT_END_MARKER_HIGH_LOCATION,
    MEMORY_LAYOUT_END_MARKER_LOW_LOCATION,

    MEMORY_LAYOUT_TOTAL_NUM
} app_memory_layout_t;

void app_memory_init(void);

void app_memory_update(uint32_t period);

app_memory_init_state_t app_memory_initState_get(void);
void app_memory_initState_set(app_memory_init_state_t value);

void app_memory_data_store(app_memory_storeType_t condition);

app_memory_storingState_t app_memory_storingState_get(void);
void app_memory_storingState_set(app_memory_storingState_t value);

void app_memory_command_set(app_memory_shutdownState_t value);

void app_memory_data_set(app_memory_layout_t addr, uint8_t data);
uint8_t app_memory_data_get(app_memory_layout_t addr);

app_memory_shutdownState_t app_memory_shutdownState_get(void);
void app_memory_shutdownState_set(app_memory_shutdownState_t value);

void app_memory_logError_set(uint8_t addr, uint16_t data);
void app_memory_logCycle_set(uint8_t addr, uint16_t data);

uint16_t *app_memory_logBuffer_get(void);

void app_memory_resetWaterLevel_set(uint32_t data);
uint32_t app_memory_restWaterLevel_get(void);

void app_memory_resetWeight_set(uint16_t data);
uint32_t app_memory_weightData_get(void);

void app_memory_enduranceCnt_set(uint32_t data);
uint32_t app_memory_enduranceCnt_get(void);

void app_memory_lVoltage_set(uint16_t data);
uint32_t app_memory_lVoltageData_get(void);
uint8_t app_memory_lVoltageCalibration_get(void);

void app_memory_preferedCourse_set(uint8_t course);
uint8_t app_memory_preferedCourse_get(void);

uint8_t app_memory_autoDetergentFirstTime_get(void);
uint8_t app_memory_autoSoftenerFirstTime_get(void);
void app_memory_autoDetergentFirstTime_set(app_memory_autoDetergentFirstTime_t value);
void app_memory_autoSoftenerFirstTime_set(app_memory_autoDetergentFirstTime_t value);

uint8_t app_memory_reset(void);

uint32_t app_memory_totalMemoryLayoutCount_get(void);

#endif
