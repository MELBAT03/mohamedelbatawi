#include "def.h"
#include "os.h"

#include "app_fill.h"
#include "app_processor.h"

#include "app_nm_log.h"

#include "app_fault.h"

#include "app_startup_fill.h"
#include "app_setup_fill.h"
#include "app_standby_fill.h"
#include "app_nm_fill.h"
#include "app_tm_fill.h"
#include "app_fault_fill.h"
#include "app_finish_fill.h"
#include "app_nm_wash.h"

#include "app_nm_wash_fill.h"

#include "handlers/output/inc/handler_water_valve.h"
#include "handlers/input/inc/handler_water_level.h"
#include "handlers/input/inc/handler_temp_sensor.h"
#include "handlers/input/inc/handler_detergent_sensor.h"
#include "handlers/input/inc/handler_softener_sensor.h"
#include "handlers/output/inc/handler_softener_pump.h"
#include "handlers/output/inc/handler_detergent_pump.h"

#define WATER_SUPPLY_FAILIUR_20MIN_TIME_MS     (1200000U)
#define FILL_OVER_FLOW_DANGEROUS_LEVEL_TIMEOUT (300000U)

#define FILL_DELAY_MS (3000U)

#define FILL_MAX_TEMP                               (63U)
#define FILL_COLD_WATER_HIGHTEMP                    (50U)
#define FILL_WRONG_VALVE_CONNECTION_TIMEOUT_MSEC    (10000U)
#define FILL_WATER_TEMP_DANGEROUS_TEMP_TIMEOUT_MSEC (30000U)
#define FILL_WATER_TEMP_FAILURE_TIMEOUT_MSEC        (150000U)

typedef enum app_fill_state
{
    FILL_PREWASH,
    FILL_POWDER,
    FILL_GEL,
    FILL_PREWASH_POWDER,
    FILL_RINSE
} app_fill_state_t;

static void fill_error_check(uint32_t period);
static void fill_overFlow_check(uint32_t period);
static void fill_waterTemp_fault_check(uint32_t period);
static void fill_detergentPumpState_set(uint8_t state, uint32_t period);
static void fill_softenerPumpState_set(uint8_t state, uint32_t period);
static void fill_washFillingSource_select(app_fill_stage_t stage, uint32_t period, app_nm_wash_userInterface_waterTempSelection_t temp);
static void fill_valves_control(uint32_t period, app_nm_wash_userInterface_waterTempSelection_t temp);

static app_fill_state_t fillState = FILL_PREWASH;

static uint32_t fillErrorHandling_internalTimer = 0;
static uint32_t fillDelayMSec = 0;
static uint8_t fill_error_flag = 0;

static app_fill_overFlowState_t gx_fill_overFLowState = OVERFLOW_NONE;
static app_fill_waterValveSafetyState_t gx_fill_waterValveFaultState = WATER_VALVE_NORMAL;

void app_fill_update(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        app_startup_fill_process,
        app_setup_fill_process,
        app_standby_fill_process,
        app_nm_fill_process,
        app_tm_fill_process,
        app_fault_fill_process,
        app_finish_fill_process,
        NULL};

    if (app_processor_entryFlag_get() == 0)
    {
        return;
    }

    if (handlers[app_processor_state_get()] != NULL)
    {
        handlers[app_processor_state_get()](period);
    }

    fill_overFlow_check(period);
    fill_waterTemp_fault_check(period);
}

app_fill_overFlowState_t app_fill_overFlowState_get(void)
{
    return gx_fill_overFLowState;
}

app_fill_waterValveSafetyState_t app_fill_waterTempFault_get(void)
{
    return gx_fill_waterValveFaultState;
}

app_fill_waterLevelState_t app_fill_waterLevelState_get(uint32_t reqWaterLevelLiter, uint32_t variance)
{
    app_fill_waterLevelState_t ret = APP_FILL_WATER_LEVEL_NOK;

    uint32_t currentLevelFreqHz = handler_waterLevel_currentFreq_get();
    uint32_t requiredLevelFreqHz = handler_waterLevel_freqValue_get(reqWaterLevelLiter);

    if (handler_waterLevel_compare(currentLevelFreqHz, requiredLevelFreqHz, variance))
    {
        ret = APP_FILL_WATER_LEVEL_OK;
    }

    LOG('F', 'v', "RequiredLiter:%d  currentFreq:%d  requiredFreq:%d  ret:%d", reqWaterLevelLiter, currentLevelFreqHz, requiredLevelFreqHz, ret);

    return ret;
}

void app_fill_handle(uint32_t period, app_nm_wash_userInterface_waterTempSelection_t userTempSelection, app_fill_stage_t stage)
{
    // NOTE: to remove the spray effect
    if (fillDelayMSec < FILL_DELAY_MS)
    {
        fillDelayMSec += period;
        return;
    }

#ifdef AUTO_DETERGENT_HEATER_MODEL
    switch (stage)
    {
        case APP_FILL_PRE_WASH:
        case APP_FILL_MAIN_WASH:
        {
            fill_washFillingSource_select(stage, period, userTempSelection);
        }
        break;

        case APP_FILL_RINSE:
        {
            fillState = FILL_RINSE;
            fill_softenerPumpState_set(0, period);
            fill_detergentPumpState_set(0, period);
        }
        break;

        case APP_FILL_SOFTENER:
        {
            fillState = FILL_RINSE;
            fill_softenerPumpState_set(1, period);
            fill_detergentPumpState_set(0, period);
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
#else
    switch (userTempSelection)
    {
        case APP_NORMAL_MODE_UI_WATER_TEMP_HOT:
        {
            if ((WATER_VALVE_COLD_FORCED_ON == gx_fill_waterValveFaultState) || (WATER_VALVE_COLD_FORCED_ON_HOT_FORCED_OFF == gx_fill_waterValveFaultState))
            {
                handler_coldWaterValve_state_set(WATER_VALVE_ACTIVE);
            }
            else
            {
                handler_coldWaterValve_state_set(WATER_VALVE_INACTIVE);
            }

            if (WATER_VALVE_COLD_FORCED_ON_HOT_FORCED_OFF == gx_fill_waterValveFaultState)
            {
                handler_hotWaterValve_state_set(WATER_VALVE_INACTIVE);
            }
            else
            {
                handler_hotWaterValve_state_set(WATER_VALVE_ACTIVE);
            }
            handler_softenerWaterValve_state_set(WATER_VALVE_INACTIVE);
        }
        break;

        case APP_NORMAL_MODE_UI_WATER_TEMP_WARM:
        {
            handler_coldWaterValve_state_set(WATER_VALVE_ACTIVE);
            if (WATER_VALVE_COLD_FORCED_ON_HOT_FORCED_OFF == gx_fill_waterValveFaultState)
            {
                handler_hotWaterValve_state_set(WATER_VALVE_INACTIVE);
            }
            else
            {
                handler_hotWaterValve_state_set(WATER_VALVE_ACTIVE);
            }
            handler_softenerWaterValve_state_set(WATER_VALVE_INACTIVE);
        }
        break;

        case APP_NORMAL_MODE_UI_WATER_TEMP_COLD:
        {
            handler_coldWaterValve_state_set(WATER_VALVE_ACTIVE);
            handler_hotWaterValve_state_set(WATER_VALVE_INACTIVE);

            if (stage == APP_FILL_SOFTENER)
            {
                handler_softenerWaterValve_state_set(WATER_VALVE_ACTIVE);
            }
            else
            {
                handler_softenerWaterValve_state_set(WATER_VALVE_INACTIVE);
            }
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
#endif

    fill_error_check(period);
}

void app_fill_coldWaterValve_state_set(app_fill_waterValve_state_t state)
{
    handler_coldWaterValve_state_set(state);
}
void app_fill_hotWaterValve_state_set(app_fill_waterValve_state_t state)
{
    handler_hotWaterValve_state_set(state);
}
void app_fill_softenerWaterValve_state_set(app_fill_waterValve_state_t state)
{
    handler_softenerWaterValve_state_set(state);
}
void app_fill_showerWaterValve_state_set(app_fill_waterValve_state_t state)
{
    handler_showerWaterValve_state_set(state);
}

void app_fill_stop(void)
{
    fillDelayMSec = 0;
    handler_coldWaterValve_state_set(WATER_VALVE_INACTIVE);
    handler_hotWaterValve_state_set(WATER_VALVE_INACTIVE);
    handler_softenerWaterValve_state_set(WATER_VALVE_INACTIVE);
    handler_showerWaterValve_state_set(WATER_VALVE_INACTIVE);
#ifdef AUTO_DETERGENT_HEATER_MODEL
    handler_softenerPump_state_set(HANDLER_SOFTENER_PUMP_INACTIVE);
#endif
    fillErrorHandling_internalTimer = 0;
}

uint8_t app_fill_error_get(void)
{
    return fill_error_flag;
}

app_fill_autoDetergentState_t app_fill_autoDetergentState_get(void)
{
#ifdef AUTO_DETERGENT_HEATER_MODEL
    app_fill_autoDetergentState_t ret = APP_FILL_AUTO_DETERGENT_NORMAL;

    if ((handler_detergentSensor_state_get() == HANDLER_DETERGENT_LOW) && (app_memory_autoDetergentFirstTime_get() == MEMORY_AUTO_DETERGENT_FIRST_TIME_RECORDED))
    {
        ret = APP_FILL_AUTO_DETERGENT_LOW;
    }
    else if ((handler_detergentSensor_state_get() == HANDLER_DETERGENT_LOW) && (app_memory_autoDetergentFirstTime_get() != MEMORY_AUTO_DETERGENT_FIRST_TIME_RECORDED))
    {
        ret = APP_FILL_AUTO_DETERGENT_MAYBE_LOW;
    }
    else
    {
        // do nothing
    }

    return ret;
#else
    return APP_FILL_AUTO_DETERGENT_NORMAL;
#endif
}

app_fill_autoSoftenerState_t app_fill_autoSoftenerState_get(void)
{
#ifdef AUTO_DETERGENT_HEATER_MODEL
    app_fill_autoSoftenerState_t ret = APP_FILL_AUTO_SOFTENER_NORMAL;

    if ((handler_softenerSensor_state_get() == HANDLER_SOFTENER_LOW) && (app_memory_autoSoftenerFirstTime_get() == MEMORY_AUTO_DETERGENT_FIRST_TIME_RECORDED))
    {
        ret = APP_FILL_AUTO_SOFTENER_LOW;
    }
    else if ((handler_softenerSensor_state_get() == HANDLER_SOFTENER_LOW) && (app_memory_autoSoftenerFirstTime_get() != MEMORY_AUTO_DETERGENT_FIRST_TIME_RECORDED))
    {
        ret = APP_FILL_AUTO_SOFTENER_MAYBE_LOW;
    }
    else
    {
        // do nothing
    }

    return ret;
#else
    return APP_FILL_AUTO_SOFTENER_NORMAL;
#endif
}

static void fill_error_check(uint32_t period)
{
    fillErrorHandling_internalTimer += period;
    if (fillErrorHandling_internalTimer >= WATER_SUPPLY_FAILIUR_20MIN_TIME_MS)
    {
        fillErrorHandling_internalTimer = 0;
        fill_error_flag = 1;
    }
}

static void fill_overFlow_check(uint32_t period)
{
    static uint32_t dangerousOverFlowTimeOut = 0;

    switch (gx_fill_overFLowState)
    {
        case OVERFLOW_NONE:
        {
            if (handler_waterLevel_resetLevel_get() - handler_waterLevel_currentFreq_get() >= HANDLER_WATER_LEVEL_OVERFLOW_UPPER_THRESHOLD_OFFSET)
            {
                gx_fill_overFLowState = OVERFLOW_NORMAL;
                dangerousOverFlowTimeOut = 0;
            }
        }
        break;

        case OVERFLOW_NORMAL:
        {
            if (handler_waterLevel_resetLevel_get() - handler_waterLevel_currentFreq_get() >= HANDLER_WATER_LEVEL_DANGEROUS_OVERFLOW_THRESHOLD_OFFSET)
            {
                gx_fill_overFLowState = OVERFLOW_DANGEROUS;
                dangerousOverFlowTimeOut = 0;
            }
            else if (handler_waterLevel_resetLevel_get() - handler_waterLevel_currentFreq_get() < HANDLER_WATER_LEVEL_OVERFLOW_LOWER_THRESHOLD_OFFSET)
            {
                gx_fill_overFLowState = OVERFLOW_NONE;
                dangerousOverFlowTimeOut = 0;
            }
            else
            {
                dangerousOverFlowTimeOut += period;
                if (dangerousOverFlowTimeOut >= FILL_OVER_FLOW_DANGEROUS_LEVEL_TIMEOUT)
                {
                    dangerousOverFlowTimeOut = 0;
                    gx_fill_overFLowState = OVERFLOW_DANGEROUS;
                }
            }
        }
        break;

        case OVERFLOW_DANGEROUS:
        default:
            // do nothing
            break;
    }
}

static void fill_waterTemp_fault_check(uint32_t period)
{
    static uint32_t wrongValveConnection = 0;
    static uint32_t highTempCnt = 0;
    uint16_t tempSensorValue = 20;

    tempSensorValue = handler_waterTempSensor_temp_get();

    switch (app_userInterface_data_get()->normalMode.wash.waterTempSelection)
    {
        case APP_NORMAL_MODE_UI_WATER_TEMP_COLD:
        {
            if (tempSensorValue >= FILL_COLD_WATER_HIGHTEMP)
            {
                wrongValveConnection += period;
                if (wrongValveConnection >= FILL_WRONG_VALVE_CONNECTION_TIMEOUT_MSEC)
                {
                    gx_fill_waterValveFaultState = WATER_VALVE_WRONG_CONNECTION;
                }
            }
            else
            {
                wrongValveConnection = 0;
            }
        }
        break;

        case APP_NORMAL_MODE_UI_WATER_TEMP_WARM:
        case APP_NORMAL_MODE_UI_WATER_TEMP_HOT:
        {
            wrongValveConnection = 0;

            if (tempSensorValue >= FILL_MAX_TEMP)
            {
                gx_fill_waterValveFaultState = WATER_VALVE_COLD_FORCED_ON;
            }

            if ((WATER_VALVE_COLD_FORCED_ON == gx_fill_waterValveFaultState) && (tempSensorValue <= (FILL_MAX_TEMP - 3U)))
            {
                gx_fill_waterValveFaultState = WATER_VALVE_NORMAL;
                highTempCnt = 0;
            }
            else if ((WATER_VALVE_COLD_FORCED_ON == gx_fill_waterValveFaultState) && (tempSensorValue >= FILL_MAX_TEMP))
            {
                highTempCnt += period;
                if (highTempCnt >= FILL_WATER_TEMP_DANGEROUS_TEMP_TIMEOUT_MSEC)
                {
                    gx_fill_waterValveFaultState = WATER_VALVE_COLD_FORCED_ON_HOT_FORCED_OFF;
                    if (highTempCnt >= FILL_WATER_TEMP_FAILURE_TIMEOUT_MSEC)
                    {
                        highTempCnt = 0;
                        gx_fill_waterValveFaultState = WATER_VALVE_TEMP_FAILURE;
                    }
                }
            }
            else
            {
                // do nothing
            }
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}

static void fill_detergentPumpState_set(uint8_t state, uint32_t period)
{
    static uint32_t detergentTimeout = 0;

    if (detergentTimeout < app_nm_wash_wash_detergent_time_get(app_userInterface_data_get()->normalMode.wash.waterLevelSelection))
    {
        detergentTimeout += period;
        handler_detergentPump_state_set(HANDLER_DETERGENT_PUMP_ACTIVE);
    }
    else
    {
        detergentTimeout = 0;
        handler_detergentPump_state_set(HANDLER_DETERGENT_PUMP_INACTIVE);
    }
}

static void fill_softenerPumpState_set(uint8_t state, uint32_t period)
{
    static uint32_t softenerTimeout = 0;

    if (app_fill_autoSoftenerState_get() != APP_FILL_AUTO_SOFTENER_LOW)
    {
        if ((softenerTimeout < app_nm_wash_fill_autoSoftenerTimeout_get()) && (FILL_SOFTNER_ON == state))
        {
            softenerTimeout += period;
            handler_softenerPump_state_set(HANDLER_SOFTENER_PUMP_ACTIVE);
        }
        else
        {
            softenerTimeout = 0;
            handler_softenerPump_state_set(HANDLER_SOFTENER_PUMP_INACTIVE);
        }
    }
    else
    {
        softenerTimeout = 0;
        handler_softenerPump_state_set(HANDLER_SOFTENER_PUMP_INACTIVE);
    }
}

static void fill_washFillingSource_select(app_fill_stage_t stage, uint32_t period, app_nm_wash_userInterface_waterTempSelection_t temp)
{
    if ((app_userInterface_data_get()->normalMode.wash.gelDetergentSelection == APP_NORMAL_MODE_UI_GEL_DETERGENT_ON) && (APP_FILL_AUTO_DETERGENT_LOW != app_fill_autoDetergentState_get()))
    {
        if (app_userInterface_data_get()->normalMode.wash.waterLevelDisplaySelection < APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_5)
        {
            if (stage == APP_FILL_PRE_WASH)
            {
                fillState = FILL_PREWASH;
                fill_detergentPumpState_set(0, period);
                fill_softenerPumpState_set(0, period);
            }
            else
            {
                fillState = FILL_GEL;
                fill_detergentPumpState_set(1, period);
                fill_softenerPumpState_set(0, period);
            }
        }
        else
        {
            fillState = FILL_GEL;
            fill_detergentPumpState_set(1, period);
            fill_softenerPumpState_set(0, period);
        }
    }
    else
    {
        if (app_userInterface_data_get()->normalMode.wash.waterLevelDisplaySelection < APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_5)
        {
            if (stage == APP_FILL_PRE_WASH)
            {
                fillState = FILL_PREWASH;
                fill_detergentPumpState_set(0, period);
                fill_softenerPumpState_set(0, period);
            }
            else
            {
                fillState = FILL_POWDER;
                fill_detergentPumpState_set(0, period);
                fill_softenerPumpState_set(0, period);
            }
        }
        else
        {
            fillState = FILL_PREWASH_POWDER;
            fill_detergentPumpState_set(0, period);
            fill_softenerPumpState_set(0, period);
        }
    }

    fill_valves_control(period, temp);
}

static void fill_valves_control(uint32_t period, app_nm_wash_userInterface_waterTempSelection_t temp)
{
    static uint32_t timeoutMSec = 0;
    static uint32_t hotPreWashTimeoutMSec = 0;
    static uint32_t hotPowderTimeoutMSec = 0;
    static uint32_t hotGelTimeoutMSec = 0;
    static uint32_t hotPrePowderTimeoutMSec = 0;

    if (temp != APP_NORMAL_MODE_UI_WATER_TEMP_COLD)
    {
        handler_hotWaterValve_state_set(WATER_VALVE_ACTIVE);
    }

    switch (fillState)
    {
        case FILL_PREWASH:
        {
            if (temp == APP_NORMAL_MODE_UI_WATER_TEMP_HOT)
            {
                hotPreWashTimeoutMSec += period;
                if (hotPreWashTimeoutMSec >= 60000U)
                {
                    handler_softenerWaterValve_state_set(WATER_VALVE_INACTIVE);
                }
                else
                {
                    handler_softenerWaterValve_state_set(WATER_VALVE_ACTIVE);
                }
            }
            else
            {
                handler_softenerWaterValve_state_set(WATER_VALVE_ACTIVE);
            }

            handler_coldWaterValve_state_set(WATER_VALVE_INACTIVE);
        }
        break;

        case FILL_POWDER:
        {
            if (temp == APP_NORMAL_MODE_UI_WATER_TEMP_HOT)
            {
                hotPowderTimeoutMSec += period;
                if (hotPowderTimeoutMSec >= 60000U)
                {
                    handler_coldWaterValve_state_set(WATER_VALVE_INACTIVE);
                }
                else
                {
                    handler_coldWaterValve_state_set(WATER_VALVE_ACTIVE);
                }
            }
            else
            {
                handler_coldWaterValve_state_set(WATER_VALVE_ACTIVE);
            }

            handler_softenerWaterValve_state_set(WATER_VALVE_INACTIVE);
        }
        break;

        case FILL_GEL:
        {
            if (temp == APP_NORMAL_MODE_UI_WATER_TEMP_HOT)
            {
                hotGelTimeoutMSec += period;
                if (hotGelTimeoutMSec >= 60000U)
                {
                    handler_softenerWaterValve_state_set(WATER_VALVE_INACTIVE);
                }
                else
                {
                    handler_softenerWaterValve_state_set(WATER_VALVE_ACTIVE);
                }
            }
            else
            {
                handler_softenerWaterValve_state_set(WATER_VALVE_ACTIVE);
            }

            handler_coldWaterValve_state_set(WATER_VALVE_INACTIVE);
        }
        break;

        case FILL_PREWASH_POWDER:
        {
            timeoutMSec += period;
            if (timeoutMSec >= 60000U)
            {
                // WHEN TO RESET THE COUNTER?
                handler_softenerWaterValve_state_set(WATER_VALVE_INACTIVE);
            }
            else
            {
                handler_softenerWaterValve_state_set(WATER_VALVE_ACTIVE);
            }

            if (temp == APP_NORMAL_MODE_UI_WATER_TEMP_HOT)
            {
                hotPrePowderTimeoutMSec += period;
                if (hotPrePowderTimeoutMSec >= 60000U)
                {
                    handler_coldWaterValve_state_set(WATER_VALVE_INACTIVE);
                }
                else
                {
                    handler_coldWaterValve_state_set(WATER_VALVE_ACTIVE);
                }
            }
            else
            {
                handler_coldWaterValve_state_set(WATER_VALVE_ACTIVE);
            }
        }
        break;

        case FILL_RINSE:
        {
            handler_softenerWaterValve_state_set(WATER_VALVE_ACTIVE);
            handler_coldWaterValve_state_set(WATER_VALVE_INACTIVE);
            handler_hotWaterValve_state_set(WATER_VALVE_INACTIVE);
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}
