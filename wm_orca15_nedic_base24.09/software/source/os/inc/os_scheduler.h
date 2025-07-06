#if !defined(OS_SCH_H)
#define OS_SCH_H

#include "def.h"
#include "os_config.h"

#define OS_SCHEDULER_TASK_NAME_SIZE_LIMIT (4U)

typedef enum
{
    TIME_BASED_TASK,
    EVENT_BASED_TASK
} os_scheduler_taskType_t;

typedef enum os_scheduler_taskTickMultiplier
{
    OS_SCH_MULTIPLIER_NONE = 1,
    OS_SCH_MULTIPLIER_TEN = 10
} os_scheduler_taskTickMultiplier_t;

typedef struct
{
    void (*task)(uint32_t period);
    char taskName[OS_SCHEDULER_TASK_NAME_SIZE_LIMIT];
    uint32_t delay;
    uint32_t period;
    uint32_t run;
    uint32_t periodArg;
#if OS_MEASUREMENT == OS_MEASUREMENT_TASK_EXECUTION_TIME
    uint32_t taskExecutionTimeUSec;
    uint32_t taskWCETUSec;
#endif
} os_sch_taskHandle_t;

typedef struct os_sch_tickJitterMeasurement
{
    uint32_t tickJitterTimeUSec;
    uint32_t tickWorstJitterTimeUSec;
} os_sch_tickJitterMeasurement_t;

typedef enum
{
    OS_SCH_NO_FAULT,
    OS_SCH_TASK_OVERFLOW_FAULT,
    OS_SCH_SYS_CLOCK_FAULT
} os_sch_fault_t;

typedef struct os_scheduler_tickListData
{
    char *tickList[210];
    uint32_t tickCount;
} os_scheduler_tickListData_t;

os_sch_taskHandle_t os_scheduler_internalEventTaskHandler_get(uint8_t index);
os_sch_taskHandle_t os_scheduler_internalTimeTaskHandler_get(uint8_t index);

void os_scheduler_init(uint32_t tickPeriodMSec);

os_sch_taskHandle_t *os_scheduler_executionMeasurements_get(uint8_t index);
os_sch_tickJitterMeasurement_t *os_scheduler_tickJitterMeasurements_get(void);

os_scheduler_tickListData_t *os_schuduler_tickList_get(void);

void os_scheduler_onExit_run(void (*shutdown)(void));
uint8_t os_scheduler_startUpSync_check(uint32_t (*startupSyncFn)(void));
void os_scheduler_process(void);
void os_scheduler_shutdown_request(void (*shutdownCondition)(void));
void os_scheduler_task_add(void (*task)(uint32_t), const char *taskName, os_scheduler_taskType_t taskType, os_scheduler_taskTickMultiplier_t tickMultiplier, uint32_t delay, uint32_t period);
void os_scheduler_time_update(void);
void os_scheduler_event_update(void);

uint8_t os_scheduler_taskCnt_get(void);
uint32_t os_scheduler_taskOverRunTicksCnt_get(void);
uint8_t os_scheduler_taskOverRunTaskID_get(void);
char *os_scheduler_taskOverRunTaskName_get(void);
os_sch_fault_t os_scheduler_fault_get(void);

void os_scheduler_fault_set(os_sch_fault_t fault);

#endif
