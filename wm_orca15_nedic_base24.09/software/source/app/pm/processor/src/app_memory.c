#include "os.h"
#include "component_config.h"

#include "app_processor.h"

#include "app_memory.h"

#include "app_nm_log.h"

#include "app_startup_memory.h"
#include "app_setup_memory.h"
#include "app_standby_memory.h"
#include "app_nm_memory.h"
#include "app_tm_memory.h"
#include "app_fault_memory.h"
#include "app_finish_memory.h"

#include "handlers/input/inc/handler_memory.h"
#include "handlers/input/inc/handler_water_level.h"
#include "handlers/input/inc/handler_acin.h"

#define MEMORY_SEQUENTIAL_WRITE_BYTES_NUM (1U)
#define MEMORY_SEQUENTIAL_READ_BYTES_NUM  (16U)

#define USER_DATA_OFFSET                 (0U)
#define LOG_OFFSET                       (USER_DATA_OFFSET + MEMORY_LAYOUT_TOTAL_NUM)
#define LVOLTAGE_OFFSET                  (LOG_OFFSET + (APP_NORMAL_MODE_LOG_TOTAL_NUM * 2U))
#define WEIGHT_OFFSET                    (LVOLTAGE_OFFSET + 2)
#define WATERLEVEL_OFFSET                (WEIGHT_OFFSET + 2U)
#define ENDURANCE_CNT_OFFSET             (WATERLEVEL_OFFSET + 5U)
#define CONTENDER_COURSE_OFFSET          (ENDURANCE_CNT_OFFSET + 2U)
#define PREFERED_COURSE_OFFSET           (CONTENDER_COURSE_OFFSET + 1U)
#define PREFERED_COURSE_COUNT_OFFSET     (PREFERED_COURSE_OFFSET + 1U)
#define AUTO_DETERGENT_FIRST_TIME_OFFSET (PREFERED_COURSE_COUNT_OFFSET + 1U)
#define AUTO_SOFTENER_FIRST_TIME_OFFSET  (AUTO_DETERGENT_FIRST_TIME_OFFSET + 1U)
#define MEMORY_END_MARKER_OFFSET         (AUTO_SOFTENER_FIRST_TIME_OFFSET + 1U)
#define MEMORY_USED_SIZE                 (MEMORY_END_MARKER_OFFSET + 2U)

static app_memory_init_state_t state = MEMORY_INIT_READ;

static uint8_t gu8_app_memory_buffer[MEMORY_USED_SIZE];
static uint16_t gu16_app_memory_logBuffer[APP_NORMAL_MODE_LOG_TOTAL_NUM];

static uint32_t gu32_app_memory_waterLevelCalibrationValue = 0;
static uint32_t gu32_app_memory_lVoltageCalibrationValue = 0;
static uint32_t gu32_app_memory_weightCalibrationValue = 0;
static uint32_t gu32_app_memory_enduranceCnt = 0;
static uint16_t gu16_app_memory_powerOnCnt = 0;
static uint8_t gu8_app_memory_preferedCourse = 0;
static uint8_t gu8_app_memory_autoDetergentFirstTime = 0;
static uint8_t gu8_app_memory_autoSoftenerFirstTime = 0;

static app_memory_storingState_t gx_memory_storingState = MEMORY_STORING_IN_PROGRESS;

static app_memory_shutdownState_t commandValue = MEMORY_SHUTDOWN_NONE;

void app_memory_update(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        app_startup_memory_process,
        app_setup_memory_process,
        app_standby_memory_process,
        app_nm_memory_process,
        app_tm_memory_process,
        app_fault_memory_process,
        app_finish_memory_process,
        NULL};

    if (app_processor_entryFlag_get() == 0)
    {
        return;
    }

    if (handlers[app_processor_state_get()] != NULL)
    {
        handlers[app_processor_state_get()](period);
    }

    LOG_CB('l', 'v', gu8_app_memory_buffer, MEMORY_USED_SIZE);

    LOG('Q', 'v', "command : %d    storingState : %d    shutdownState : %d    waterLevelCalib : %d    weightCalib : %d    enduranceCnt : %d    powerOnCnt : %d", commandValue, gx_memory_storingState, gu8_app_memory_buffer[USER_DATA_OFFSET + MEMORY_LAYOUT_SHUT_DOWN_STATE_LOCATION], gu32_app_memory_waterLevelCalibrationValue, gu32_app_memory_weightCalibrationValue, gu32_app_memory_enduranceCnt, gu16_app_memory_powerOnCnt);
}

void app_memory_init(void)
{
    uint32_t u32_tempBuffer = 0;
    uint32_t eepromBuffer = 0;

    static uint16_t index = 0;
    static uint16_t numBytesRead = 0;
    static uint16_t numBytesToRead = MEMORY_USED_SIZE;

    switch (state)
    {
        case MEMORY_INIT_READ:
        {
            while (numBytesToRead > 0)
            {
                if (numBytesToRead > MEMORY_SEQUENTIAL_READ_BYTES_NUM)
                {
                    numBytesRead = MEMORY_SEQUENTIAL_READ_BYTES_NUM;
                    numBytesToRead -= MEMORY_SEQUENTIAL_READ_BYTES_NUM;
                }
                else
                {
                    numBytesRead = numBytesToRead;
                    numBytesToRead = 0;
                }

                handler_memory_data_get(&gu8_app_memory_buffer[index], index, numBytesRead);
                index += numBytesRead;

                for (volatile uint8_t i = 0; i < 10U; i++)
                {
                    // dummy delay between sequential reads
                }
            }

            state = MEMORY_INIT_PROCESS;
        }
        break;

        case MEMORY_INIT_PROCESS:
        {
            if (gu8_app_memory_buffer[USER_DATA_OFFSET] != MEMORY_SHUTDOWN_NORMAL && gu8_app_memory_buffer[USER_DATA_OFFSET] != MEMORY_SHUTDOWN_ABNORMAL)
            {
                // first time run
                for (uint16_t k = 0; k < MEMORY_USED_SIZE; k++)
                {
                    gu8_app_memory_buffer[k] = 0;
                }

                for (uint8_t j = 0; j < APP_NORMAL_MODE_LOG_TOTAL_NUM; j++)
                {
                    gu16_app_memory_logBuffer[j] = 0;
                }
                gu8_app_memory_buffer[USER_DATA_OFFSET + MEMORY_LAYOUT_SHUT_DOWN_STATE_LOCATION] = MEMORY_SHUTDOWN_NORMAL;
            }
            else
            {
                for (uint8_t k = LOG_OFFSET, j = 0; j < APP_NORMAL_MODE_LOG_TOTAL_NUM; j++, k += 2)
                {
                    gu16_app_memory_logBuffer[j] = (uint16_t)gu8_app_memory_buffer[k] | (uint16_t)((uint16_t)gu8_app_memory_buffer[k + 1] << 8);
                }

                gu32_app_memory_lVoltageCalibrationValue |= ((uint32_t)gu8_app_memory_buffer[LVOLTAGE_OFFSET] << 0);
                gu32_app_memory_lVoltageCalibrationValue |= ((uint32_t)gu8_app_memory_buffer[LVOLTAGE_OFFSET + 1] << 8);

                gu32_app_memory_weightCalibrationValue |= ((uint32_t)gu8_app_memory_buffer[WEIGHT_OFFSET] << 0);
                gu32_app_memory_weightCalibrationValue |= ((uint32_t)gu8_app_memory_buffer[WEIGHT_OFFSET + 1] << 8);

                gu32_app_memory_waterLevelCalibrationValue |= ((uint32_t)gu8_app_memory_buffer[WATERLEVEL_OFFSET] << 0);
                gu32_app_memory_waterLevelCalibrationValue |= ((uint32_t)gu8_app_memory_buffer[WATERLEVEL_OFFSET + 1] << 8);
                gu32_app_memory_waterLevelCalibrationValue |= ((uint32_t)gu8_app_memory_buffer[WATERLEVEL_OFFSET + 2] << 16);
                gu32_app_memory_waterLevelCalibrationValue |= ((uint32_t)gu8_app_memory_buffer[WATERLEVEL_OFFSET + 3] << 24);

                gu32_app_memory_enduranceCnt |= ((uint32_t)gu8_app_memory_buffer[ENDURANCE_CNT_OFFSET] << 0);
                gu32_app_memory_enduranceCnt |= ((uint32_t)gu8_app_memory_buffer[ENDURANCE_CNT_OFFSET + 1] << 8);

                gu8_app_memory_preferedCourse = gu8_app_memory_buffer[PREFERED_COURSE_OFFSET];

                gu8_app_memory_autoDetergentFirstTime = gu8_app_memory_buffer[AUTO_DETERGENT_FIRST_TIME_OFFSET];
                gu8_app_memory_autoSoftenerFirstTime = gu8_app_memory_buffer[AUTO_SOFTENER_FIRST_TIME_OFFSET];

                gu16_app_memory_powerOnCnt |= gu8_app_memory_buffer[MEMORY_END_MARKER_OFFSET];
                gu16_app_memory_powerOnCnt |= ((uint16_t)gu8_app_memory_buffer[MEMORY_END_MARKER_OFFSET + 1] << 8);
                gu16_app_memory_powerOnCnt++;
                gu8_app_memory_buffer[MEMORY_END_MARKER_OFFSET] = gu16_app_memory_powerOnCnt & 0x00ffU;
                gu8_app_memory_buffer[MEMORY_END_MARKER_OFFSET + 1U] = (gu16_app_memory_powerOnCnt >> 8U) & 0x00ffU;

                eepromBuffer = gu32_app_memory_waterLevelCalibrationValue;
                u32_tempBuffer = eepromBuffer & 0x00ffffffU;

                if ((eepromBuffer >> 24U) == HANDLER_WATERLEVEL_EEPROM_CORRECTION_FACTOR_FLAG_MAGIC_VALUE)
                {
                    handler_waterLevel_resetLevel_set(u32_tempBuffer);
                    handler_waterLevel_calibrationFlag_set();
                }
            }

            state = MEMORY_INIT_DONE;

            LOG(0, 'w', "powerOnCount : %d    enduranceCount : %d    waterLevelCalibration : %d    WeightCalibration : %d", gu16_app_memory_powerOnCnt, gu32_app_memory_enduranceCnt, u32_tempBuffer, gu32_app_memory_weightCalibrationValue);
        }
        break;

        case MEMORY_INIT_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

app_memory_init_state_t app_memory_initState_get(void)
{
    return state;
}

void app_memory_initState_set(app_memory_init_state_t value)
{
    state = value;
}

void app_memory_data_store(app_memory_storeType_t condition)
{
    static uint16_t index = 0;
    static uint16_t numBytesWritten = 0;
    static uint16_t numBytesToWrite = MEMORY_USED_SIZE;

    if (commandValue == MEMORY_SHUTDOWN_SKIP)
    {
        gu8_app_memory_buffer[USER_DATA_OFFSET] = MEMORY_SHUTDOWN_NORMAL;
        (void)handler_memory_data_set(&gu8_app_memory_buffer[USER_DATA_OFFSET], USER_DATA_OFFSET, 1);
        gx_memory_storingState = MEMORY_STORING_DONE;
    }
    else if ((MEMORY_STORING_IN_PROGRESS == gx_memory_storingState) && (commandValue != MEMORY_SHUTDOWN_NONE) && (handler_memory_resetState_get() == 0U))
    {
        // NOTE: to reset the variables if the periodic task has run before lvd detection
        if (condition == MEMORY_STORE_TYPE_CONTINUOUS)
        {
            index = 0;
            numBytesWritten = 0;
            numBytesToWrite = MEMORY_USED_SIZE;
        }

        do
        {
            if (numBytesToWrite > MEMORY_SEQUENTIAL_WRITE_BYTES_NUM)
            {
                numBytesWritten = MEMORY_SEQUENTIAL_WRITE_BYTES_NUM;
                numBytesToWrite -= MEMORY_SEQUENTIAL_WRITE_BYTES_NUM;
            }
            else
            {
                numBytesWritten = numBytesToWrite;
                numBytesToWrite = 0;
            }

            if (numBytesWritten > 0U)
            {
                (void)handler_memory_data_set(&gu8_app_memory_buffer[index], index, numBytesWritten);
                index += numBytesWritten;
            }
            else if (numBytesWritten == 0U)
            {
                gx_memory_storingState = MEMORY_STORING_DONE;
            }
            else
            {
                // do nothing
            }
        } while (condition == MEMORY_STORE_TYPE_CONTINUOUS && MEMORY_STORING_DONE != gx_memory_storingState);
    }
    else
    {
        // do nothing
    }
}

app_memory_storingState_t app_memory_storingState_get(void)
{
    return gx_memory_storingState;
}

void app_memory_storingState_set(app_memory_storingState_t value)
{
    gx_memory_storingState = value;
}

void app_memory_command_set(app_memory_shutdownState_t shut_down_value)
{
    commandValue = shut_down_value;
    gu8_app_memory_buffer[USER_DATA_OFFSET + MEMORY_LAYOUT_SHUT_DOWN_STATE_LOCATION] = (uint8_t)shut_down_value;
}

uint32_t app_memory_totalMemoryLayoutCount_get(void)
{
    return MEMORY_USED_SIZE;
}

void app_memory_data_set(app_memory_layout_t addr, uint8_t data)
{
    gu8_app_memory_buffer[addr + USER_DATA_OFFSET] = data;
}
uint8_t app_memory_data_get(app_memory_layout_t addr)
{
    return gu8_app_memory_buffer[addr + USER_DATA_OFFSET];
}

app_memory_shutdownState_t app_memory_shutdownState_get(void)
{
    app_memory_shutdownState_t ret = (app_memory_shutdownState_t)gu8_app_memory_buffer[USER_DATA_OFFSET + MEMORY_LAYOUT_SHUT_DOWN_STATE_LOCATION];
    return ret;
}
void app_memory_shutdownState_set(app_memory_shutdownState_t value)
{
    gu8_app_memory_buffer[USER_DATA_OFFSET + MEMORY_LAYOUT_SHUT_DOWN_STATE_LOCATION] = (uint8_t)value;
}

void app_memory_logError_set(uint8_t addr, uint16_t data)
{
    uint16_t gu16_app_memory_logUpdatedAddr = 0;

    gu16_app_memory_logUpdatedAddr = LOG_OFFSET + (2 * addr);
    gu8_app_memory_buffer[gu16_app_memory_logUpdatedAddr] = (uint8_t)data;
    gu8_app_memory_buffer[gu16_app_memory_logUpdatedAddr + 1] = (uint8_t)(data >> 8);
    gu16_app_memory_logBuffer[addr] = data;
}

void app_memory_logCycle_set(uint8_t addr, uint16_t data)
{
    uint16_t gu16_app_memory_logCycleUpdatedAddr = 0;

    gu16_app_memory_logCycleUpdatedAddr = LOG_OFFSET + (2 * (APP_NORMAL_MODE_LOG_COURSE_ADDR_OFFSET + addr));
    gu8_app_memory_buffer[gu16_app_memory_logCycleUpdatedAddr] = (uint8_t)data;
    gu8_app_memory_buffer[gu16_app_memory_logCycleUpdatedAddr + 1] = (uint8_t)(data >> 8);
    gu16_app_memory_logBuffer[addr + APP_NORMAL_MODE_LOG_COURSE_ADDR_OFFSET] = data;
}

uint16_t *app_memory_logBuffer_get(void)
{
    return gu16_app_memory_logBuffer;
}

void app_memory_resetWaterLevel_set(uint32_t data)
{
    gu8_app_memory_buffer[WATERLEVEL_OFFSET] = (uint8_t)data;
    gu8_app_memory_buffer[WATERLEVEL_OFFSET + 1] = (uint8_t)(data >> 8);
    gu8_app_memory_buffer[WATERLEVEL_OFFSET + 2] = (uint8_t)(data >> 16);
    gu8_app_memory_buffer[WATERLEVEL_OFFSET + 3] = (uint8_t)(HANDLER_WATERLEVEL_EEPROM_CORRECTION_FACTOR_FLAG_MAGIC_VALUE);   // :NOTE: memory marker

    // LOG_I("%d  %d  %d  %d", gu8_app_memory_buffer[WATERLEVEL_OFFSET], gu8_app_memory_buffer[WATERLEVEL_OFFSET + 1], gu8_app_memory_buffer[WATERLEVEL_OFFSET + 2], gu8_app_memory_buffer[WATERLEVEL_OFFSET + 3]);
}

void app_memory_resetWeight_set(uint16_t data)
{
    gu8_app_memory_buffer[WEIGHT_OFFSET] = (uint8_t)data;
    gu8_app_memory_buffer[WEIGHT_OFFSET + 1] = (uint8_t)(data >> 8);

    // LOG_I("memory weight %d  %d", gu8_app_memory_buffer[WEIGHT_OFFSET], gu8_app_memory_buffer[WEIGHT_OFFSET + 1]);
}

void app_memory_lVoltage_set(uint16_t data)
{
    gu8_app_memory_buffer[LVOLTAGE_OFFSET] = (uint8_t)data;
    gu8_app_memory_buffer[LVOLTAGE_OFFSET + 1] = (uint8_t)(data >> 8);

    // LOG_I("memory weight %d  %d", gu8_app_memory_buffer[LVOLTAGE_OFFSET], gu8_app_memory_buffer[LVOLTAGE_OFFSET + 1]);
}

void app_memory_enduranceCnt_set(uint32_t data)
{
    gu8_app_memory_buffer[ENDURANCE_CNT_OFFSET] = (uint8_t)data;
    gu8_app_memory_buffer[ENDURANCE_CNT_OFFSET + 1] = (uint8_t)(data >> 8);

    // LOG_I("%d  %d", gu8_app_memory_buffer[ENDURANCE_CNT_OFFSET], gu8_app_memory_buffer[ENDURANCE_CNT_OFFSET + 1]);
}

void app_memory_preferedCourse_set(uint8_t course)
{
    static uint8_t incrementGuard = 0;

    if (gu8_app_memory_buffer[CONTENDER_COURSE_OFFSET] != course)
    {
        gu8_app_memory_buffer[PREFERED_COURSE_COUNT_OFFSET] = 0;
        gu8_app_memory_buffer[CONTENDER_COURSE_OFFSET] = course;
    }
    else if (incrementGuard == 0)
    {
        incrementGuard = 1;
        gu8_app_memory_buffer[CONTENDER_COURSE_OFFSET] = course;
        gu8_app_memory_buffer[PREFERED_COURSE_COUNT_OFFSET]++;
        if (gu8_app_memory_buffer[PREFERED_COURSE_COUNT_OFFSET] >= 5)
        {
            gu8_app_memory_buffer[PREFERED_COURSE_OFFSET] = course;
            gu8_app_memory_buffer[PREFERED_COURSE_COUNT_OFFSET] = 0;
        }
    }
    else
    {
        // do nothing
    }
}

uint32_t app_memory_restWaterLevel_get(void)
{
    return gu32_app_memory_waterLevelCalibrationValue;
}

uint32_t app_memory_weightData_get(void)
{
    return gu32_app_memory_weightCalibrationValue;
}

uint32_t app_memory_lVoltageData_get(void)
{
    if (gu32_app_memory_lVoltageCalibrationValue == 0 || gu32_app_memory_lVoltageCalibrationValue == 0xffff)
    {
        return handler_acin_defaultUnderVoltageValue_get();
    }
    else
    {
        return gu32_app_memory_lVoltageCalibrationValue;
    }
}

uint8_t app_memory_lVoltageCalibration_get(void)
{
    uint8_t ret = 1;

    if (gu32_app_memory_lVoltageCalibrationValue == 0 || gu32_app_memory_lVoltageCalibrationValue == 0xffff)
    {
        ret = 0;
    }

    return ret;
}

uint32_t app_memory_enduranceCnt_get(void)
{
    return gu32_app_memory_enduranceCnt;
}

uint8_t app_memory_preferedCourse_get(void)
{
    return gu8_app_memory_preferedCourse;
}

uint8_t app_memory_autoDetergentFirstTime_get(void)
{
    return gu8_app_memory_autoDetergentFirstTime;
}
uint8_t app_memory_autoSoftenerFirstTime_get(void)
{
    return gu8_app_memory_autoSoftenerFirstTime;
}

void app_memory_autoDetergentFirstTime_set(app_memory_autoDetergentFirstTime_t value)
{
    gu8_app_memory_buffer[AUTO_DETERGENT_FIRST_TIME_OFFSET] = (uint8_t)value;
}
void app_memory_autoSoftenerFirstTime_set(app_memory_autoDetergentFirstTime_t value)
{
    gu8_app_memory_buffer[AUTO_SOFTENER_FIRST_TIME_OFFSET] = (uint8_t)value;
}

uint8_t app_memory_reset(void)
{
    for (uint8_t i = 0; i < MEMORY_USED_SIZE; i++)
    {
        gu8_app_memory_buffer[i] = 0;
    }

    for (uint8_t i = 0; i < APP_NORMAL_MODE_LOG_TOTAL_NUM; i++)
    {
        gu16_app_memory_logBuffer[i] = 0;
    }

    gx_memory_storingState = MEMORY_STORING_IN_PROGRESS;

    return handler_memory_reset();
}
