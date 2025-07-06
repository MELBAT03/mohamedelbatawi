#include "def.h"
#include "os.h"

#include "app_memory.h"
#include "app_nm_log.h"

#define MAX_LOGS_VALUE 9999

static app_nm_log_error_t gx_app_log_currentError = (app_nm_log_error_t)0;

void app_nm_log_error_set(app_nm_log_error_t errorNum)
{
#ifndef ERROR_DISABLE
    uint16_t *gu16Arr_LogsFreqArray;

    gu16Arr_LogsFreqArray = app_memory_logBuffer_get();

    if (errorNum != APP_NORMAL_MODE_LOG_ERROR_NO_ERROR)
    {
        if (errorNum < APP_NORMAL_MODE_LOG_ERRORS_TOTAL_NUM)
        {
            gx_app_log_currentError = errorNum;

            if (gu16Arr_LogsFreqArray[errorNum] < MAX_LOGS_VALUE)
            {
                gu16Arr_LogsFreqArray[errorNum]++;
                app_memory_logError_set(errorNum, gu16Arr_LogsFreqArray[errorNum]);
            }
        }
    }
    else if (errorNum == APP_NORMAL_MODE_LOG_ERROR_NO_ERROR)
    {
        gx_app_log_currentError = errorNum;
    }
    else
    {
        // do nothing
    }
#endif
}

app_nm_log_error_t app_nm_log_error_get(void)
{
    return gx_app_log_currentError;
}

void app_nm_log_errorCnt_clear(void)
{
    for (uint8_t i = 0; i < APP_NORMAL_MODE_LOG_ERRORS_TOTAL_NUM; i++)
    {
        app_memory_logError_set(i, 0);
    }
}

void app_nm_log_courseCnt_clear(void)
{
    for (uint8_t i = APP_NORMAL_MODE_LOG_COURSE_CYCLE_NUM_START; i < APP_NORMAL_MODE_LOG_CYCLE_RECORD_LENGTH; i++)
    {
        app_memory_logError_set(i, 0);
    }
}

uint16_t app_nm_log_errorCount_get(app_nm_log_error_t errorName)
{
    uint16_t *gu16Arr_LogsFreqArray;

    gu16Arr_LogsFreqArray = app_memory_logBuffer_get();

    return gu16Arr_LogsFreqArray[errorName];
}

void app_nm_log_completedCycleCount_set(app_nm_userInterface_courseSelection_t course)
{
    uint16_t *gu16Arr_LogsFreqArray;

    gu16Arr_LogsFreqArray = app_memory_logBuffer_get();

    if (course < APP_NORMAL_MODE_UI_COURSE_NUM)
    {
        if (gu16Arr_LogsFreqArray[(uint16_t)course + APP_NORMAL_MODE_LOG_COURSE_ADDR_OFFSET] < MAX_LOGS_VALUE)
        {
            gu16Arr_LogsFreqArray[(uint16_t)course + APP_NORMAL_MODE_LOG_COURSE_ADDR_OFFSET]++;
            app_memory_logCycle_set((uint16_t)course, gu16Arr_LogsFreqArray[(uint16_t)course + APP_NORMAL_MODE_LOG_COURSE_ADDR_OFFSET]);
        }
    }
}

uint16_t app_nm_log_completedCycleCount_get(app_nm_userInterface_courseSelection_t course)
{
    uint16_t *gu16Arr_LogsFreqArray;

    gu16Arr_LogsFreqArray = app_memory_logBuffer_get();
    return gu16Arr_LogsFreqArray[(uint16_t)course + APP_NORMAL_MODE_LOG_COURSE_ADDR_OFFSET];
}
