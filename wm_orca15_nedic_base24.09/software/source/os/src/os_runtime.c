#include "def.h"
#include "os.h"
#include "mcal.h"

typedef struct
{
    void (*osTaskPtr1)(void);
    uint8_t repeated;
    uint32_t period;
    uint32_t cnt;
} os_runTime_task_t;

os_runTime_task_t gx_os_runTime_internalHandler[CONFIG_OS_RUN_TIME_USER_HOOKS_NUM];
static uint8_t gu8_os_runTime_internalCounter;

void os_runTime_init(void)
{
    for (uint8_t i = 0; i < CONFIG_OS_RUN_TIME_USER_HOOKS_NUM; i++)
    {
        gx_os_runTime_internalHandler[i].osTaskPtr1 = NULL;
        gx_os_runTime_internalHandler[i].repeated = 0;
    }
}

void os_runTime_userTask_register(void (*p)(void), uint8_t repeated, uint32_t period)
{
    if (gu8_os_runTime_internalCounter < CONFIG_OS_RUN_TIME_USER_HOOKS_NUM)
    {
        gx_os_runTime_internalHandler[gu8_os_runTime_internalCounter].osTaskPtr1 = p;
        gx_os_runTime_internalHandler[gu8_os_runTime_internalCounter].repeated = repeated;
        gx_os_runTime_internalHandler[gu8_os_runTime_internalCounter].period = period;
        gx_os_runTime_internalHandler[gu8_os_runTime_internalCounter].cnt = 0;

        gu8_os_runTime_internalCounter++;
    }
}

void os_runTime_update(uint32_t period)
{
    for (uint8_t i = 0; i < gu8_os_runTime_internalCounter; i++)
    {
        if (gx_os_runTime_internalHandler[i].osTaskPtr1 != NULL)
        {
            gx_os_runTime_internalHandler[i].cnt += period;
            if (gx_os_runTime_internalHandler[i].cnt >= gx_os_runTime_internalHandler[i].period)
            {
                gx_os_runTime_internalHandler[i].cnt = 0;
                gx_os_runTime_internalHandler[i].osTaskPtr1();
                if (gx_os_runTime_internalHandler[i].repeated == 0U)
                {
                    gx_os_runTime_internalHandler[i].osTaskPtr1 = NULL;
                }
            }
        }
    }
}
