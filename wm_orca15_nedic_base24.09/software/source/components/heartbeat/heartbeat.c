#include "app/config/config.h"
#include "gpio/gpio.h"
#include "os.h"

#include "os_scheduler.h"

#include "core_cm3.h"

#ifdef HEARTBEAT_LED_ENABLE
static gpio_t gx_heartbeatLED;
#endif

extern uint8_t gu8_os_log_groupID;

void heartbeat_init(gpio_t *px_led)
{
#ifdef HEARTBEAT_LED_ENABLE
    px_led->ioState = GPIO_OUTPUT;

    gx_heartbeatLED.port = px_led->port;
    gx_heartbeatLED.pin = px_led->pin;
    gx_heartbeatLED.ioState = px_led->ioState;
    gx_heartbeatLED.state = px_led->state;

    gpio_pin_init(&gx_heartbeatLED);
    gpio_pinState_set(gx_heartbeatLED.port, gx_heartbeatLED.pin, gx_heartbeatLED.state);
#endif
}

void heartbeat_update(uint32_t period)
{
    CG_ResetFlag resetFlags = CG_GetResetFlag();

    if (resetFlags.All)
    {
        LOG(0, 'w', "(RST) {POR: %d   EXT: %d   WDT: %d   LVD: %d   DBG: %d   OFD: %d}", resetFlags.Bit.PowerOn, resetFlags.Bit.ResetPin, resetFlags.Bit.WDTReset, resetFlags.Bit.VLTDReset, resetFlags.Bit.DebugReset, resetFlags.Bit.OFDReset);
    }

#ifdef HEARTBEAT_LED_ENABLE
    gx_heartbeatLED.state = ~gx_heartbeatLED.state;
    gpio_pinState_set(gx_heartbeatLED.port, gx_heartbeatLED.pin, gx_heartbeatLED.state);
#endif

    if (os_scheduler_taskOverRunTicksCnt_get() > 1U)
    {
        LOG(0, 'w', "OR[%d  TID:%d  TNAME:%s] - sch[Fault:%d  TCount:%d] - FaultRegister:%d", os_scheduler_taskOverRunTicksCnt_get(), os_scheduler_taskOverRunTaskID_get(), os_scheduler_taskOverRunTaskName_get(), os_scheduler_fault_get(), os_scheduler_taskCnt_get(), SCB->HFSR);
    }

    LOG('l', 'v', "OR[%d  TID:%d  TNAME:%s] - sch[Fault:%d  TCount:%d]", os_scheduler_taskOverRunTicksCnt_get(), os_scheduler_taskOverRunTaskID_get(), os_scheduler_taskOverRunTaskName_get(), os_scheduler_fault_get(), os_scheduler_taskCnt_get());

#if OS_MEASUREMENT == OS_MEASUREMENT_TASK_EXECUTION_TIME
    if (gu8_os_log_groupID == 'E')
    {
        printf("\n\r");
        for (uint8_t i = 0; i < os_scheduler_taskCnt_get() && os_scheduler_executionMeasurements_get(i) != NULL; i++)
        {
            printf("%s:%d ", os_scheduler_executionMeasurements_get(i)->taskName, os_scheduler_executionMeasurements_get(i)->taskExecutionTimeUSec);
        }
    }
    else if (gu8_os_log_groupID == 'C')
    {
        printf("\n\r");
        for (uint8_t i = 0; i < os_scheduler_taskCnt_get() && os_scheduler_executionMeasurements_get(i) != NULL; i++)
        {
            printf("%s:%d ", os_scheduler_executionMeasurements_get(i)->taskName, os_scheduler_executionMeasurements_get(i)->taskWCETUSec);
        }
    }
    else
    {
        // do nothing
    }
#elif OS_MEASUREMENT == OS_MEASUREMENT_TICK_JITTER
    LOG('J', 'v', "current Jitter : %d    Worst Jitter : %d", os_scheduler_tickJitterMeasurements_get()->tickJitterTimeUSec, os_scheduler_tickJitterMeasurements_get()->tickWorstJitterTimeUSec);
#endif

    // for (uint8_t i = 0; i < OS_HYPERPERIOD_TICKS && os_schuduler_tickList_get()->tickList[i] != NULL; i++)
    // {
    //     printf("%s  ", os_schuduler_tickList_get()->tickList[i]);
    // }
    // printf("\n\r");
    // printf("\n\r");
}
