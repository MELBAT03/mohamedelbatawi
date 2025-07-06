#include "os_config.h"
#include "os_tick.h"
#include "os_int_handlers.h"

#include "os_log.h"
#include "os_scheduler.h"

#define OS_SCHEDULER_MAX_TIME_DEPENDENT_TAKS_NUM   (CONFIG_OS_TIME_DEPENDENT_TASKS_NUM + CONFIG_OS_SYSTEM_TASKS_NUM)
#define OS_SCHEDULER_MAX_EVENT_DEPENDENT_TASKS_NUM (CONFIG_OS_EVENT_DEPENDENT_TASKS_NUM)

#define OS_SCHDULER_ALLOWED_OVERRUN_TICKS (3U)

#define OS_SCHDULER_DUMMUY_TASK_ID (0xFFU)

static void os_scheduler_dispatch(void);
static void os_scheduler_processor_sleep(void);

static os_sch_taskHandle_t gx_os_scheduler_inetrnalUserTimeTaskHandler[OS_SCHEDULER_MAX_TIME_DEPENDENT_TAKS_NUM];
static uint32_t gu32_os_schduler_internalUserTimeTaskCnt = 0;
static os_sch_taskHandle_t gx_os_scheduler_inetrnalUserEventTaskHandler[OS_SCHEDULER_MAX_EVENT_DEPENDENT_TASKS_NUM];
static uint8_t gu8_os_scheduler_taskRelease_flag = 0;
static uint8_t gu8_os_scheduler_bufferedTaskOverRunTaskID = OS_SCHDULER_DUMMUY_TASK_ID;
static char *gu8_os_scheduler_bufferedTaskOverRunTaskName;
static uint8_t gu8_os_scheduler_taskOverRunTaskID = OS_SCHDULER_DUMMUY_TASK_ID;
static char *gu8_os_scheduler_taskOverRunTaskName;
static uint8_t gu8_os_scheduler_schRunCnt = 0;
static uint32_t gu32_os_scheduler_overRunCnt = 0;
static os_sch_fault_t gu8_os_scheduler_fault = OS_SCH_NO_FAULT;
static int8_t g8_os_scheduler_shutdown_flag = 0;
static int8_t ng8_os_scheduler_shutdown_flag = 1;

static uint32_t gu32_os_scheduler_tickPeriodMSec = 0;

static uint8_t gu8_os_scheduler_taskCnt = 0;

#ifdef OS_TICK_LIST_MONITOR_ENABLE
static os_scheduler_tickListData_t os_scheduler_tickList;
#endif

static uint8_t (*gx_os_scheduler_shutdownCondition)(void);
static uint8_t os_scheduler_dummyShutdownCondition(void);
static void (*gv_os_scheduler_shutdown)(void);

#if OS_MEASUREMENT == OS_MEASUREMENT_TICK_JITTER
static os_sch_tickJitterMeasurement_t gx_tickJitterMeasurement;
#endif

os_sch_taskHandle_t os_scheduler_internalEventTaskHandler_get(uint8_t index)
{
    return gx_os_scheduler_inetrnalUserEventTaskHandler[index];
}

os_sch_taskHandle_t os_scheduler_internalTimeTaskHandler_get(uint8_t index)
{
    return gx_os_scheduler_inetrnalUserTimeTaskHandler[index];
}

void os_scheduler_init(uint32_t tickPeriodMSec)
{
    for (uint8_t i = 0; i < OS_SCHEDULER_MAX_TIME_DEPENDENT_TAKS_NUM; i++)
    {
        gx_os_scheduler_inetrnalUserTimeTaskHandler[i].delay = 0;
        gx_os_scheduler_inetrnalUserTimeTaskHandler[i].period = 0;
        gx_os_scheduler_inetrnalUserTimeTaskHandler[i].run = 0;
        gx_os_scheduler_inetrnalUserTimeTaskHandler[i].task = NULL;
        gx_os_scheduler_inetrnalUserTimeTaskHandler[i].periodArg = 0;
    }

    for (uint8_t i = 0; i < OS_SCHEDULER_MAX_EVENT_DEPENDENT_TASKS_NUM; i++)
    {
        gx_os_scheduler_inetrnalUserEventTaskHandler[i].delay = 0;
        gx_os_scheduler_inetrnalUserEventTaskHandler[i].period = 0;
        gx_os_scheduler_inetrnalUserEventTaskHandler[i].run = 0;
        gx_os_scheduler_inetrnalUserEventTaskHandler[i].task = NULL;
        gx_os_scheduler_inetrnalUserEventTaskHandler[i].periodArg = 0;
    }

    gu32_os_scheduler_tickPeriodMSec = tickPeriodMSec;

#if OS_MEASUREMENT == OS_MEASUREMENT_TASK_EXECUTION_TIME
    mcal_timer_timerModeUS_init(OS_MESUREMENTS_TIMER, 1600);
    mcal_timer_timerState_set(OS_MESUREMENTS_TIMER, MCAL_TIMER_STOP);
    mcal_timer_timerFlag_clear(OS_MESUREMENTS_TIMER);
#elif OS_MEASUREMENT == OS_MEASUREMENT_TICK_JITTER
    mcal_timer_timerModeUS_init(OS_MESUREMENTS_TIMER, 6000);
    mcal_timer_timerState_set(OS_MESUREMENTS_TIMER, MCAL_TIMER_STOP);
    mcal_timer_timerFlag_clear(OS_MESUREMENTS_TIMER);
#endif
}

os_sch_taskHandle_t *os_scheduler_executionMeasurements_get(uint8_t index)
{
#if OS_MEASUREMENT == OS_MEASUREMENT_TASK_EXECUTION_TIME
    return &gx_os_scheduler_inetrnalUserTimeTaskHandler[index];
#else
    return NULL;
#endif
}

os_sch_tickJitterMeasurement_t *os_scheduler_tickJitterMeasurements_get(void)
{
#if OS_MEASUREMENT == OS_MEASUREMENT_TICK_JITTER
    return &gx_tickJitterMeasurement;
#else
    return NULL;
#endif
}

void os_scheduler_onExit_run(void (*shutdown)(void))
{
    gv_os_scheduler_shutdown = shutdown;
}

uint8_t os_scheduler_startUpSync_check(uint32_t (*startupSyncFn)(void))
{
    uint8_t ret = 1;

    if ((startupSyncFn != NULL) && (startupSyncFn() == 0UL))
    {
        ret = 0;
    }

    return ret;
}

void os_scheduler_process(void)
{
    os_tick_init(gu32_os_scheduler_tickPeriodMSec);

    os_scheduler_time_update();

    for (;;)
    {
        os_scheduler_dispatch();
        if ((g8_os_scheduler_shutdown_flag == (int8_t)1) && (ng8_os_scheduler_shutdown_flag == 0) && (gx_os_scheduler_shutdownCondition() != 0))
        {
            if (gv_os_scheduler_shutdown == NULL)
            {
                break;
            }
            else
            {
                gv_os_scheduler_shutdown();
            }
        }
    }
}

void os_scheduler_shutdown_request(void (*shutdownCondition)(void))
{
    g8_os_scheduler_shutdown_flag = 1;
    ng8_os_scheduler_shutdown_flag = 0;

    if (shutdownCondition != NULL)
    {
        gx_os_scheduler_shutdownCondition = (uint8_t(*)(void))shutdownCondition;
    }
    else
    {
        gx_os_scheduler_shutdownCondition = os_scheduler_dummyShutdownCondition;
    }
}

void os_scheduler_task_add(void (*task)(uint32_t), const char *taskName, os_scheduler_taskType_t taskType, os_scheduler_taskTickMultiplier_t tickMultiplier, uint32_t delay, uint32_t period)
{
    uint8_t taskAddedAck = 0;

    switch (taskType)
    {
        case TIME_BASED_TASK:
            for (uint8_t i = 0; i < OS_SCHEDULER_MAX_TIME_DEPENDENT_TAKS_NUM; i++)
            {
                if (gx_os_scheduler_inetrnalUserTimeTaskHandler[i].task == NULL)
                {
                    for (uint8_t j = 0; j < OS_SCHEDULER_TASK_NAME_SIZE_LIMIT && taskName[j] != 0; j++)
                    {
                        gx_os_scheduler_inetrnalUserTimeTaskHandler[i].taskName[j] = taskName[j];
                    }

                    gx_os_scheduler_inetrnalUserTimeTaskHandler[i].delay = delay;
                    gx_os_scheduler_inetrnalUserTimeTaskHandler[i].period = period;
                    gx_os_scheduler_inetrnalUserTimeTaskHandler[i].run = 0;
                    gx_os_scheduler_inetrnalUserTimeTaskHandler[i].task = task;
                    gx_os_scheduler_inetrnalUserTimeTaskHandler[i].periodArg = tickMultiplier * period;
                    gu32_os_schduler_internalUserTimeTaskCnt++;
                    taskAddedAck = 1;
                    gu8_os_scheduler_taskCnt++;
                    break;
                }
            }
            break;

        case EVENT_BASED_TASK:
            for (uint8_t i = 0; i < OS_SCHEDULER_MAX_EVENT_DEPENDENT_TASKS_NUM; i++)
            {
                if (gx_os_scheduler_inetrnalUserEventTaskHandler[i].task == NULL)
                {
                    for (uint8_t j = 0; j < OS_SCHEDULER_TASK_NAME_SIZE_LIMIT && taskName[j] != 0; j++)
                    {
                        gx_os_scheduler_inetrnalUserEventTaskHandler[i].taskName[j] = taskName[j];
                    }

                    gx_os_scheduler_inetrnalUserEventTaskHandler[i].delay = delay;
                    gx_os_scheduler_inetrnalUserEventTaskHandler[i].period = period;
                    gx_os_scheduler_inetrnalUserEventTaskHandler[i].run = 0;
                    gx_os_scheduler_inetrnalUserEventTaskHandler[i].task = task;
                    gx_os_scheduler_inetrnalUserEventTaskHandler[i].periodArg = period;
                    taskAddedAck = 1;
                    gu8_os_scheduler_taskCnt++;
                    break;
                }
            }
            break;

        default:
            // do nothing
            break;
    }

    if (!taskAddedAck)
    {
        gu8_os_scheduler_fault = OS_SCH_TASK_OVERFLOW_FAULT;
    }
}

os_scheduler_tickListData_t *os_schuduler_tickList_get(void)
{
#ifdef OS_TICK_LIST_MONITOR_ENABLE
    return &os_scheduler_tickList;
#else
    return NULL;
#endif
}

void os_scheduler_time_update(void)
{
    static uint8_t taskListIndex = 0;
    static uint32_t taskSequenceInitializationTimeoutMSec = 10000U;

#if OS_MEASUREMENT == OS_MEASUREMENT_TICK_JITTER
    gx_tickJitterMeasurement.tickJitterTimeUSec = mcal_timer_timerCounter_get(OS_MESUREMENTS_TIMER) / mcal_timer_timerModeUS_conversionFactor_get(OS_MESUREMENTS_TIMER);
    if (gx_tickJitterMeasurement.tickJitterTimeUSec > gx_tickJitterMeasurement.tickWorstJitterTimeUSec)
    {
        gx_tickJitterMeasurement.tickWorstJitterTimeUSec = gx_tickJitterMeasurement.tickJitterTimeUSec;
    }
    mcal_timer_timerState_set(OS_MESUREMENTS_TIMER, MCAL_TIMER_STOP);
    mcal_timer_timerFlag_clear(OS_MESUREMENTS_TIMER);
    mcal_timer_timerState_set(OS_MESUREMENTS_TIMER, MCAL_TIMER_START);
#endif

    gu8_os_scheduler_schRunCnt++;
#ifdef OS_TICK_LIST_MONITOR_ENABLE
    os_scheduler_tickList.tickCount++;
#endif
    taskSequenceInitializationTimeoutMSec = 0;

    // if (taskSequenceInitializationTimeoutMSec > gu32_os_scheduler_tickPeriodMSec)
    // {
    //     taskSequenceInitializationTimeoutMSec -= gu32_os_scheduler_tickPeriodMSec;
    // }
    // else
    // {
    //     taskSequenceInitializationTimeoutMSec = 0;
    //     hyperPeriodTimeoutMsec += gu32_os_scheduler_tickPeriodMSec;
    // }

    for (uint8_t i = 0; ((i < gu32_os_schduler_internalUserTimeTaskCnt) && (i < OS_SCHEDULER_MAX_TIME_DEPENDENT_TAKS_NUM)); i++)
    {
        if (gx_os_scheduler_inetrnalUserTimeTaskHandler[i].task != NULL)
        {
            if (gx_os_scheduler_inetrnalUserTimeTaskHandler[i].delay == 0UL)
            {
                if (taskSequenceInitializationTimeoutMSec == 0U)
                {
#ifdef OS_TICK_LIST_MONITOR_ENABLE
                    os_scheduler_tickList.tickList[taskListIndex++] = gx_os_scheduler_inetrnalUserTimeTaskHandler[i].taskName;
                    // if (os_scheduler_tickList.tickCount >= OS_HYPERPERIOD_TICKS)
                    // {
                    //     os_scheduler_tickList.tickCount = 0;
                    // }
#endif
                    if (taskListIndex >= OS_HYPERPERIOD_TICKS)
                    {
                        taskListIndex = 0;
                    }
                }
                gx_os_scheduler_inetrnalUserTimeTaskHandler[i].delay = gx_os_scheduler_inetrnalUserTimeTaskHandler[i].period;
                gx_os_scheduler_inetrnalUserTimeTaskHandler[i].run++;
            }

            if (gx_os_scheduler_inetrnalUserTimeTaskHandler[i].delay > 0UL)
            {
                gx_os_scheduler_inetrnalUserTimeTaskHandler[i].delay -= gu32_os_scheduler_tickPeriodMSec;
            }
        }
    }

    if (gu8_os_scheduler_taskRelease_flag != 0UL)
    {
        gu8_os_scheduler_taskOverRunTaskID = gu8_os_scheduler_bufferedTaskOverRunTaskID;
        gu8_os_scheduler_taskOverRunTaskName = gu8_os_scheduler_bufferedTaskOverRunTaskName;
        gu32_os_scheduler_overRunCnt++;
    }
    else
    {
        gu8_os_scheduler_taskOverRunTaskID = OS_SCHDULER_DUMMUY_TASK_ID;
        gu8_os_scheduler_taskOverRunTaskName = NULL;
        gu32_os_scheduler_overRunCnt = 0;
    }

    if (gu32_os_scheduler_overRunCnt > OS_SCHDULER_ALLOWED_OVERRUN_TICKS)
    {
        // TODO: enter FAIL-SAFE
        LOG(0, 'e', "Overrun Ticks:%d TID:%d TName:%s\n\r", gu32_os_scheduler_overRunCnt, gu8_os_scheduler_taskOverRunTaskID, gu8_os_scheduler_taskOverRunTaskName);
    }
}

// :NOTE: this needs to be called in zcd isr
void os_scheduler_event_update(void)
{
    for (uint8_t i = 0; i < OS_SCHEDULER_MAX_EVENT_DEPENDENT_TASKS_NUM; i++)
    {
        if (gx_os_scheduler_inetrnalUserEventTaskHandler[i].task != NULL)
        {
            gx_os_scheduler_inetrnalUserEventTaskHandler[i].task(gx_os_scheduler_inetrnalUserEventTaskHandler[i].periodArg);
        }
    }
}

uint8_t os_scheduler_taskCnt_get(void)
{
    return gu8_os_scheduler_taskCnt;
}

uint32_t os_scheduler_taskOverRunTicksCnt_get(void)
{
    return gu32_os_scheduler_overRunCnt;
}

uint8_t os_scheduler_taskOverRunTaskID_get(void)
{
    return gu8_os_scheduler_taskOverRunTaskID;
}

char *os_scheduler_taskOverRunTaskName_get(void)
{
    return gu8_os_scheduler_taskOverRunTaskName;
}

os_sch_fault_t os_scheduler_fault_get(void)
{
    return gu8_os_scheduler_fault;
}

void os_scheduler_fault_set(os_sch_fault_t fault)
{
    gu8_os_scheduler_fault = fault;
}

static void os_scheduler_dispatch(void)
{
    uint8_t taskRunReq_flag = 0;

    OS_INT_DISABLE;
    taskRunReq_flag = (uint8_t)((uint32_t)gu8_os_scheduler_schRunCnt > 0U);
    OS_INT_ENABLE;

    while (taskRunReq_flag != 0U)
    {
        for (uint8_t i = 0; ((i < gu32_os_schduler_internalUserTimeTaskCnt) && (i < OS_SCHEDULER_MAX_TIME_DEPENDENT_TAKS_NUM)); i++)
        {
            if ((gx_os_scheduler_inetrnalUserTimeTaskHandler[i].task != NULL) && (gx_os_scheduler_inetrnalUserTimeTaskHandler[i].run > 0UL))
            {
                gu8_os_scheduler_bufferedTaskOverRunTaskID = i;
                gu8_os_scheduler_bufferedTaskOverRunTaskName = gx_os_scheduler_inetrnalUserTimeTaskHandler[i].taskName;
                gx_os_scheduler_inetrnalUserTimeTaskHandler[i].run--;

#if OS_MEASUREMENT == OS_MEASUREMENT_TASK_EXECUTION_TIME
                mcal_timer_timerState_set(OS_MESUREMENTS_TIMER, MCAL_TIMER_STOP);
                mcal_timer_timerFlag_clear(OS_MESUREMENTS_TIMER);
                mcal_timer_timerState_set(OS_MESUREMENTS_TIMER, MCAL_TIMER_START);
#endif

                gu8_os_scheduler_taskRelease_flag = 1;
                gx_os_scheduler_inetrnalUserTimeTaskHandler[i].task(gx_os_scheduler_inetrnalUserTimeTaskHandler[i].periodArg);
                gu8_os_scheduler_taskRelease_flag = 0;

#if OS_MEASUREMENT == OS_MEASUREMENT_TASK_EXECUTION_TIME
                gx_os_scheduler_inetrnalUserTimeTaskHandler[i].taskExecutionTimeUSec = mcal_timer_timerCounter_get(OS_MESUREMENTS_TIMER) / mcal_timer_timerModeUS_conversionFactor_get(OS_MESUREMENTS_TIMER);
                if (gx_os_scheduler_inetrnalUserTimeTaskHandler[i].taskExecutionTimeUSec > gx_os_scheduler_inetrnalUserTimeTaskHandler[i].taskWCETUSec)
                {
                    gx_os_scheduler_inetrnalUserTimeTaskHandler[i].taskWCETUSec = gx_os_scheduler_inetrnalUserTimeTaskHandler[i].taskExecutionTimeUSec;
                }
                mcal_timer_timerState_set(OS_MESUREMENTS_TIMER, MCAL_TIMER_STOP);
#endif

                gu8_os_scheduler_bufferedTaskOverRunTaskID = OS_SCHDULER_DUMMUY_TASK_ID;
                gu8_os_scheduler_bufferedTaskOverRunTaskName = NULL;
            }
        }

        OS_INT_DISABLE;
        gu8_os_scheduler_schRunCnt--;
        taskRunReq_flag = (uint8_t)((uint32_t)gu8_os_scheduler_schRunCnt > 0U);
        OS_INT_ENABLE;
    }

    os_scheduler_processor_sleep();
}

static void os_scheduler_processor_sleep(void)
{
    // do nothing
}

static uint8_t os_scheduler_dummyShutdownCondition(void)
{
    return 1;
}
