#include "def.h"
#include "os.h"

#include "app_heater.h"
#ifdef BLOWER_ENABLED
#include "app_blower.h"
#endif
#include "app_processor.h"

#include "app_nm_log.h"

#include "app_startup_heater.h"
#include "app_setup_heater.h"
#include "app_standby_heater.h"
#include "app_nm_heater.h"
#include "app_tm_heater.h"
#include "app_fault_heater.h"
#include "app_finish_heater.h"

#include "handlers/input/inc/handler_temp_sensor.h"
#include "handlers/input/inc/handler_water_level.h"

#define HEATER_ON_BLOWER_OFF_TIMEOUT_MSEC (10000U)

#define HEATER_OVERCURRENT_VALUE        (100.0F)
#define HEATER_OVERCURRENT_TIMEOUT_MSEC (10000U)

#define HEATER_NOT_OPERATING_TIMEOUT_MSEC (60000U)

#define HEATER_TEMP_SENSOR_FAULT_JUDGE_VALUE  (30U)
#define HEATER_TEMP_SENSOR_FAULT_TIMEOUT_MSEC (900000U)

#ifdef BLOWER_ENABLED
static uint8_t heater_heaterOnWhileBlowerOff_check(uint32_t period);
#endif

static uint8_t heater_heaterOnWhileNoWater_check(uint32_t period);
static uint8_t heater_overCurrent_check(uint32_t period);
static uint8_t heater_notOperating_check(uint32_t period);
static uint8_t heater_shouldntOperating_check(uint32_t period);
static uint8_t heater_tempSensorFault_check(uint32_t period);

static app_heater_plateState_t gx_heater_plateState;
static uint32_t gu32_heater_temp;
static float gf_heater_currentFeedback;

#ifdef BLOWER_ENABLED
static uint8_t gu8_heater_blowerFault_flag = 0;
#endif
static uint8_t gu8_heater_operatingWithoutWater_flag = 0;
static uint8_t gu8_heater_overCurrentFault_flag = 0;
static uint8_t gu8_heater_notOperatingFault_flag = 0;
static uint8_t gu8_heater_shouldntOperatingFault_flag = 0;
static uint8_t gu8_heater_tempSensorFault_flag = 0;

void app_heater_update(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        app_startup_heater_process,
        app_setup_heater_process,
        app_standby_heater_process,
        app_nm_heater_process,
        app_tm_heater_process,
        app_fault_heater_process,
        app_finish_heater_process,
        NULL};

    if (app_processor_entryFlag_get() == 0)
    {
        return;
    }

    if (handlers[app_processor_state_get()] != NULL)
    {
        handlers[app_processor_state_get()](period);
    }

#ifdef BLOWER_ENABLED
    gu8_heater_blowerFault_flag = heater_heaterOnWhileBlowerOff_check(period);
#endif

    gu8_heater_operatingWithoutWater_flag = heater_heaterOnWhileNoWater_check(period);
    gu8_heater_overCurrentFault_flag = heater_overCurrent_check(period);
    gu8_heater_notOperatingFault_flag = heater_notOperating_check(period);
    gu8_heater_shouldntOperatingFault_flag = heater_shouldntOperating_check(period);
    gu8_heater_tempSensorFault_flag = heater_tempSensorFault_check(period);

    if (gu8_heater_tempSensorFault_flag || gu8_heater_shouldntOperatingFault_flag || gu8_heater_notOperatingFault_flag || gu8_heater_overCurrentFault_flag || gu8_heater_operatingWithoutWater_flag)
    {
        app_heater_plateState_set(APP_HEATER_OFF);
    }

    LOG('h', 'v', "heater %d  fb %d %f  fault[NoWater:%d  OVCurr:%d  NotOP:%d  shouldn'tOP:%d  TempSnr:%d]", gx_heater_plateState, handler_heaterTempSensor_temp_get(), app_heater_currentFeedback_get(), gu8_heater_operatingWithoutWater_flag, gu8_heater_overCurrentFault_flag, gu8_heater_notOperatingFault_flag, gu8_heater_shouldntOperatingFault_flag, gu8_heater_tempSensorFault_flag);
}

#ifdef BLOWER_ENABLED
uint8_t app_heater_heaterOnWhileBlowerOffFault_get(void)
{
    return gu8_heater_blowerFault_flag;
}
#endif

uint8_t app_heater_overCurrentFault_get(void)
{
    return gu8_heater_overCurrentFault_flag;
}

uint8_t app_heater_notOperatingFault_get(void)
{
    return gu8_heater_notOperatingFault_flag;
}
uint8_t app_heater_shouldntOperatingFault_get(void)
{
    return gu8_heater_shouldntOperatingFault_flag;
}

uint8_t app_heater_tempSensorFault_get(void)
{
    return gu8_heater_tempSensorFault_flag;
}

app_heater_plateState_t app_heater_plateState_get(void)
{
    return gx_heater_plateState;
}

void app_heater_plateState_set(app_heater_plateState_t state)
{
#ifdef TESTING
    gx_heater_plateState = state;
#else
    gx_heater_plateState = state;
    handler_heater_plate_set(state);
#endif
}

uint32_t app_heater_temp_get(void)
{
#ifndef TESTING
    gu32_heater_temp = handler_heaterTempSensor_temp_get();
#endif
    return gu32_heater_temp;
}

void app_heater_temp_set(uint32_t value)
{
    gu32_heater_temp = value;
}

double app_heater_currentFeedback_get(void)
{
#ifndef TESTING
    gf_heater_currentFeedback = handler_heater_feedback_get();
#endif

    return gf_heater_currentFeedback;
}

void app_heater_currentFeedback_set(float value)
{
    gf_heater_currentFeedback = value;
}

#ifdef BLOWER_ENABLED
static uint8_t heater_heaterOnWhileBlowerOff_check(uint32_t period)
{
    uint8_t ret = 0;
    static uint32_t timeout = 0;

    if ((0 < app_heater_currentFeedback_get()) && (0 == app_blower_speedFeedback_get()))
    {
        timeout += period;
        if (timeout >= HEATER_ON_BLOWER_OFF_TIMEOUT_MSEC)
        {
            timeout = 0;
            ret = 1;
        }
    }
    else
    {
        timeout = 0;
    }

    return ret;
}
#endif

static uint8_t heater_overCurrent_check(uint32_t period)
{
    uint8_t ret = 0;
    static uint32_t timeout = 0;

    if (HEATER_OVERCURRENT_VALUE <= app_heater_currentFeedback_get())
    {
        timeout += period;
        if (timeout >= HEATER_OVERCURRENT_TIMEOUT_MSEC)
        {
            timeout = 0;
            ret = 1;
        }
    }
    else
    {
        timeout = 0;
    }

    return ret;
}

static uint8_t heater_notOperating_check(uint32_t period)
{
    uint8_t ret = 0;
    static uint32_t timeout = 0;

    if ((APP_HEATER_OFF != gx_heater_plateState) && (0 == app_heater_currentFeedback_get()))
    {
        timeout += period;
        if (timeout >= HEATER_NOT_OPERATING_TIMEOUT_MSEC)
        {
            timeout = 0;
            ret = 1;
        }
    }
    else
    {
        timeout = 0;
    }

    return ret;
}
static uint8_t heater_shouldntOperating_check(uint32_t period)
{
    uint8_t ret = 0;
    static uint32_t timeout = 0;

    if ((APP_HEATER_OFF == gx_heater_plateState) && (0 != app_heater_currentFeedback_get()))
    {
        timeout += period;
        if (timeout >= HEATER_NOT_OPERATING_TIMEOUT_MSEC)
        {
            timeout = 0;
            ret = 1;
        }
    }
    else
    {
        timeout = 0;
    }

    return ret;
}

static uint8_t heater_tempSensorFault_check(uint32_t period)
{
    uint8_t ret = 0;
    static uint32_t timeout = 0;

    if ((APP_HEATER_OFF != gx_heater_plateState) && (0 < app_heater_currentFeedback_get()) && (HEATER_TEMP_SENSOR_FAULT_JUDGE_VALUE > app_heater_temp_get()))
    {
        timeout += period;
        if (timeout >= HEATER_TEMP_SENSOR_FAULT_TIMEOUT_MSEC)
        {
            timeout = 0;
            ret = 1;
        }
    }
    else
    {
        timeout = 0;
    }

    return ret;
}

static uint8_t heater_heaterOnWhileNoWater_check(uint32_t period)
{
#ifdef AUTO_DETERGENT_HEATER_MODEL
    uint8_t ret = 0;

    if ((APP_HEATER_OFF != gx_heater_plateState) && (0U != handler_waterLevel_resetLevel_check(WATER_LEVEL_RESET_NORMAL_CHECK)))
    {
        ret = 1;
    }

    return ret;
#else
    return 0;
#endif
}
