#include "def.h"

#include "app_drain.h"
#include "app_processor.h"

#include "app_user_interface.h"
#include "app_fault.h"

#include "app_startup_drain.h"
#include "app_setup_drain.h"
#include "app_standby_drain.h"
#include "app_nm_drain.h"
#include "app_tm_drain.h"
#include "app_fault_drain.h"
#include "app_finish_drain.h"

#include "handlers/output/inc/handler_drain_pump.h"
#include "handlers/output/inc/handler_drain_valve.h"
#include "handlers/input/inc/handler_water_level.h"

#define DRAIN_PUMP_SAFETY_ON_INTERVAL_TIMEOUT_MSEC  (150000UL)
#define DRAIN_PUMP_SAFETY_OFF_INTERVAL_TIMEOUT_MSEC (10000UL)

#define DRAIN_FAILURE_PERIOD_SEC                         (900000U)
#define DRAIN_FILTER_FAILURE_RATE_SEC                    (180000U)
#define DRAIN_FAILURE_WATER_DURING_SPIN                  (10000U)
#define DRAIN_ABNORMAL_WATER_DURING_SPIN_ERROR_MAX_COUNT (5U)

static app_drain_state_t gx_app_drain_state;
static app_drain_failure_state_t gx_app_drain_failureState;

static uint8_t gu8_app_drain_shortDrain_flag;

void app_drain_update(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        app_startup_drain_process,
        app_setup_drain_process,
        app_standby_drain_process,
        app_nm_drain_process,
        app_tm_drain_process,
        app_fault_drain_process,
        app_finish_drain_process,
        NULL};

    if (app_processor_entryFlag_get() == 0)
    {
        return;
    }

    if (handlers[app_processor_state_get()] != NULL)
    {
        handlers[app_processor_state_get()](period);
    }
}

// TESTING:
void app_drain_state_set(app_drain_state_t value)
{
    gx_app_drain_state = value;
}

app_drain_state_t app_drain_state_get(void)
{
    return gx_app_drain_state;
}

void app_drain_switch(app_drain_state_t state)
{
#ifdef TESTING
    gx_app_drain_state = state;
#else
    gx_app_drain_state = handler_drainValve_switch(state);
    if (APP_DRAIN_OPEN == state)
    {
        handler_drainPump_state_set(HANDLER_DRAIN_PUMP_ACTIVE);
    }
    else
    {
        handler_drainPump_state_set(HANDLER_DRAIN_PUMP_INACTIVE);
    }
#endif
}

app_drain_tubState_t app_drain_tubState_get(void)
{
    app_drain_tubState_t ret = APP_DRAIN_TUB_NOT_EMPTY;

    if (0U != handler_waterLevel_resetLevel_check(WATER_LEVEL_RESET_NORMAL_CHECK))
    {
        ret = APP_DRAIN_TUB_EMPTY;
    }

    return ret;
}

app_drain_failure_state_t app_drain_failureState_get(void)
{
    return gx_app_drain_failureState;
}

uint32_t app_drain_waitTimeoutMSec_get(void)
{
    return APP_DRAIN_WATER_ESTIMATED_INTERVAL;
}

void app_drain_pumpSafteyOff_handle(uint32_t period)
{
    static uint32_t intervalTimeoutCnt = 0;

    if (handler_drainPump_state_get() == HANDLER_DRAIN_PUMP_ACTIVE)
    {
        intervalTimeoutCnt += period;
        if (intervalTimeoutCnt >= DRAIN_PUMP_SAFETY_ON_INTERVAL_TIMEOUT_MSEC)
        {
            handler_drainPump_state_set(HANDLER_DRAIN_PUMP_INACTIVE);
            if (intervalTimeoutCnt >= (DRAIN_PUMP_SAFETY_ON_INTERVAL_TIMEOUT_MSEC + DRAIN_PUMP_SAFETY_OFF_INTERVAL_TIMEOUT_MSEC))
            {
                intervalTimeoutCnt = 0;
                handler_drainPump_state_set(HANDLER_DRAIN_PUMP_ACTIVE);
            }
        }
    }
    else
    {
        intervalTimeoutCnt = 0;
    }
}

void app_drain_error_check(uint32_t period)
{
    static uint32_t drainErrorHandling_internalTimer = 0;
    static uint32_t drainFilterErrorHandlingInternalTimer = 0;
    static uint32_t firstShotDrainFilterFreq = 0;
    static uint32_t secondShotDrainFilterFreq = 0;
    uint32_t freq;

    if (0U != handler_waterLevel_resetLevel_check(WATER_LEVEL_RESET_NORMAL_CHECK))
    {
        freq = handler_waterLevel_currentFreq_get();

        drainErrorHandling_internalTimer += period;
        if (drainErrorHandling_internalTimer >= DRAIN_FAILURE_PERIOD_SEC)
        {
            drainErrorHandling_internalTimer = 0;
            gx_app_drain_failureState = APP_DRAIN_FAILURE_BLOCKED;
        }

        if (drainFilterErrorHandlingInternalTimer == 0U)
        {
            firstShotDrainFilterFreq = freq;
        }

        drainFilterErrorHandlingInternalTimer += period;
        if (DRAIN_FILTER_FAILURE_RATE_SEC < drainFilterErrorHandlingInternalTimer)
        {
            drainFilterErrorHandlingInternalTimer = 0;
            secondShotDrainFilterFreq = freq;

            if (secondShotDrainFilterFreq > firstShotDrainFilterFreq)
            {
                if ((secondShotDrainFilterFreq - firstShotDrainFilterFreq) > handler_waterLevel_upperDrainFilterThreshold_get() && (secondShotDrainFilterFreq - firstShotDrainFilterFreq) <= handler_waterLevel_lowerDrainFilterThreshold_get())
                {
                    gx_app_drain_failureState = APP_DRAIN_FAILURE_LOW_RATE;
                }
            }
        }
    }
    else
    {
        drainErrorHandling_internalTimer = 0;
        drainFilterErrorHandlingInternalTimer = 0;
    }
}

void app_drain_spinError_check(uint32_t period)
{
    static uint32_t spinDrainErrorCnt = 0;
    static uint32_t waterDuringSpinErrorCnt = 0;

    if (0U == handler_waterLevel_resetLevel_check(WATER_LEVEL_RESET_SPIN_ISSUE_CHECK))
    {
        spinDrainErrorCnt += period;
        if (spinDrainErrorCnt >= DRAIN_FAILURE_WATER_DURING_SPIN)
        {
            spinDrainErrorCnt = 0;
            waterDuringSpinErrorCnt++;
            gx_app_drain_failureState = APP_DRAIN_FAILURE_WATER_DURING_SPIN_NORMAL;
            if (waterDuringSpinErrorCnt >= DRAIN_ABNORMAL_WATER_DURING_SPIN_ERROR_MAX_COUNT)
            {
                waterDuringSpinErrorCnt = 0;
                gx_app_drain_failureState = APP_DRAIN_FAILURE_WATER_DURING_SPIN_DANGEROUS;
            }
        }
    }
    else
    {
        spinDrainErrorCnt = 0;
        waterDuringSpinErrorCnt = 0;
    }
}

uint8_t app_drain_shortDrainFlag_get(void)
{
    return gu8_app_drain_shortDrain_flag;
}

void app_drain_shortDrainFlag_set(uint8_t value)
{
    gu8_app_drain_shortDrain_flag = value;
}
