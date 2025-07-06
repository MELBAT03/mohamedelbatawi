#ifndef APP_FILL_H
#define APP_FILL_H

#include "def.h"

#include "app_user_interface.h"
#include "handlers/output/inc/handler_water_valve.h"

#define APP_FILL_TIME_MSEC (120000U)

typedef enum app_fill_waterLevelState
{
    APP_FILL_WATER_LEVEL_NOK = 0,
    APP_FILL_WATER_LEVEL_OK = 1
} app_fill_waterLevelState_t;

typedef enum app_fill_stage
{
    APP_FILL_PRE_WASH,
    APP_FILL_MAIN_WASH,
    APP_FILL_RINSE,
    APP_FILL_SOFTENER
} app_fill_stage_t;

typedef enum app_fill_softenerState
{
    FILL_SOFTNER_ON = 1,
    FILL_SOFTNER_OFF = 0
} app_fill_softenerState_t;

typedef enum app_fill_overFlowState
{
    OVERFLOW_NONE,
    OVERFLOW_NORMAL,
    OVERFLOW_DANGEROUS
} app_fill_overFlowState_t;

typedef enum app_fill_waterValveSafetyState
{
    WATER_VALVE_NORMAL = 0,
    WATER_VALVE_WRONG_CONNECTION = 1,
    WATER_VALVE_COLD_FORCED_ON = 2,
    WATER_VALVE_COLD_FORCED_ON_HOT_FORCED_OFF = 3,
    WATER_VALVE_TEMP_FAILURE = 4
} app_fill_waterValveSafetyState_t;

typedef enum app_fill_autoDetergentState
{
    APP_FILL_AUTO_DETERGENT_LOW,
    APP_FILL_AUTO_DETERGENT_NORMAL,
    APP_FILL_AUTO_DETERGENT_MAYBE_LOW
} app_fill_autoDetergentState_t;

typedef enum app_fill_autoSoftenerState
{
    APP_FILL_AUTO_SOFTENER_LOW,
    APP_FILL_AUTO_SOFTENER_NORMAL,
    APP_FILL_AUTO_SOFTENER_MAYBE_LOW
} app_fill_autoSoftenerState_t;

typedef handler_waterValve_state_t app_fill_waterValve_state_t;

#define APP_FILL_WATER_VALVE_ON  (WATER_VALVE_ACTIVE)
#define APP_FILL_WATER_VALVE_OFF (WATER_VALVE_INACTIVE)

void app_fill_update(uint32_t period);

app_fill_overFlowState_t app_fill_overFlowState_get(void);
app_fill_waterValveSafetyState_t app_fill_waterTempFault_get(void);

app_fill_waterLevelState_t app_fill_waterLevelState_get(uint32_t reqWaterLevelLiter, uint32_t variance);

void app_fill_handle(uint32_t period, app_nm_wash_userInterface_waterTempSelection_t userTempSelection, app_fill_stage_t stage);
void app_fill_stop(void);

app_fill_autoDetergentState_t app_fill_autoDetergentState_get(void);
app_fill_autoSoftenerState_t app_fill_autoSoftenerState_get(void);

void app_fill_coldWaterValve_state_set(app_fill_waterValve_state_t state);
void app_fill_hotWaterValve_state_set(app_fill_waterValve_state_t state);
void app_fill_softenerWaterValve_state_set(app_fill_waterValve_state_t state);
void app_fill_showerWaterValve_state_set(app_fill_waterValve_state_t state);

uint8_t app_fill_error_get(void);

#endif
