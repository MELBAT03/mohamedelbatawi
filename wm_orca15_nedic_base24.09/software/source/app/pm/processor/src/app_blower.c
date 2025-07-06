#include "def.h"

#include "app_blower.h"
#include "app_processor.h"

#include "app_startup_blower.h"
#include "app_setup_blower.h"
#include "app_standby_blower.h"
#include "app_nm_blower.h"
#include "app_tm_blower.h"
#include "app_fault_blower.h"
#include "app_finish_blower.h"

#include "handlers/output/inc/handler_blower.h"

#define BLOWER_ERROR_INTERVAL_MSEC (10000UL)

#define BLOEWR_OVER_SPEED_ERROR_VALUE    (6000U)
#define BLOEWR_OVER_SPEED_ERROR_INTERVAL (60000U)

static uint8_t app_blower_error_monitor(uint32_t period);

static uint32_t gu32_blower_speed;
static app_blower_power_t gx_blower_power;
static uint8_t gu8_blower_fault_flag = 0;

void app_blower_update(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        app_startup_blower_process,
        app_setup_blower_process,
        app_standby_blower_process,
        app_nm_blower_process,
        app_tm_blower_process,
        app_fault_blower_process,
        app_finish_blower_process,
        NULL};

    if (app_processor_entryFlag_get() == 0)
    {
        return;
    }

    if (handlers[app_processor_state_get()] != NULL)
    {
        handlers[app_processor_state_get()](period);
    }

    gu8_blower_fault_flag = app_blower_error_monitor(period);
}

void app_blower_speedFeedback_set(uint32_t value)
{
    gu32_blower_speed = value;
}

uint32_t app_blower_speedFeedback_get(void)
{
#ifdef TESTING
    return gu32_blower_speed;
#else
    return handler_blower_speed_get();
#endif
}

void app_blower_speedControl_set(app_blower_power_t power)
{
    gx_blower_power = power;
    handler_blower_output_set(power);
}

static uint8_t app_blower_error_monitor(uint32_t period)
{
    uint8_t ret = 0;
    static uint32_t errorCnt = 0;

    if ((handler_blower_speed_get() == 0U) && (gx_blower_power != APP_BLOWER_OFF))
    {
        errorCnt += period;
        if (errorCnt >= BLOWER_ERROR_INTERVAL_MSEC)
        {
            errorCnt = 0;
            ret = 1;
        }
    }
    else
    {
        errorCnt = 0;
    }

    return ret;
}

uint8_t app_blower_error_get(void)
{
    return gu8_blower_fault_flag;
}

void app_blower_error_set(uint8_t value)
{
    gu8_blower_fault_flag = value;
}
