#include "def.h"
#include "os.h"
#include "app_nm.h"
#include "app_drain.h"
#include "app_motor.h"
#include "app_fill.h"

#include "app_nm_tubclean.h"
#include "app_nm_tubclean_table.h"
#include "app_nm_tubclean_heater.h"

#include "app_buzzer.h"
#include "app_nm.h"
#include "app_memory.h"

#define TUBCLEAN_WATER_SUPPLY_AND_STIRRING_TIMES (12U)

#define TUBCLEAN_FILL_MINIMUM_ALLOWED_VARIANCE (0U)
#define TUBCLEAN_FILL_MAXIMUM_ALLOWED_VARIANCE (200U)

static app_nm_state_t tubclean_pauseRequest_handle(app_nm_state_t state);
static uint8_t tubclean_cycleEnd_check(void);

static void init_handle(uint32_t period);
static void fill_handle(uint32_t period);
static void mh_handle(uint32_t period);
static void md_handle(uint32_t period);
static void fillStir_handle(uint32_t period);
static void soak_handle(uint32_t period);
static void wash_handle(uint32_t period);
static void drain_handle(uint32_t period);
static void spin_handle(uint32_t period);
static void regroup_handle(uint32_t period);
static void finish_handle(uint32_t period);

static app_nm_tubclean_state_t gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_INIT;

static uint8_t gu8_fillingAndStirring_counter = 0U;

app_nm_state_t app_nm_tubclean_process(uint32_t period)
{
    app_nm_state_t ret = APP_NORMAL_MODE_TUBCLEAN;

    static void (*const handlers[])(uint32_t period) = {
        init_handle,
        fill_handle,
        mh_handle,
        md_handle,
        fillStir_handle,
        soak_handle,
        wash_handle,
        drain_handle,
        spin_handle,
        regroup_handle,
        finish_handle,
        NULL};

    if (APP_NORMAL_MODE_TUBCLEAN == app_nm_state_get())
    {
        if (handlers[gx_app_tubclean_state] != NULL)
        {
            handlers[gx_app_tubclean_state](period);
        }
    }

    ret = tubclean_pauseRequest_handle(ret);

    if (APP_NORMAL_MODE_TUBCLEAN_DONE == gx_app_tubclean_state)
    {
        ret = APP_NORMAL_MODE_FINISH;
    }

    return ret;
}

app_nm_tubclean_state_t app_nm_tubclean_state_get(void)
{
    return gx_app_tubclean_state;
}

void app_nm_tubclean_state_set(app_nm_tubclean_state_t state)
{
    gx_app_tubclean_state = state;
}

uint8_t app_nm_tubclean_fillCounter_get(void)
{
    return gu8_fillingAndStirring_counter;
}

static void init_handle(uint32_t period)
{
    app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_FILL;
}
static void fill_handle(uint32_t period)
{
    if (0U != tubclean_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        app_nm_data_set()->wash.tubcleanNum = 0;
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_FINISH;
    }

    if (APP_FILL_WATER_LEVEL_OK == app_fill_waterLevelState_get(app_nm_tubclean_fillWaterLevel_get(), TUBCLEAN_FILL_MAXIMUM_ALLOWED_VARIANCE))
    {
        app_nm_data_set()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1] = 0;
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_MH;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1]));
        }
    }
}
static void mh_handle(uint32_t period)
{
    if (0U != tubclean_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        app_nm_data_set()->wash.tubcleanNum = 0;
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_FINISH;
    }
    else if (app_nm_tubclean_heater_steamTechHeaterState_get() == TUBCLEAN_HEATER_STEAM_TECH_REQUIRED_OFF)
    {
        app_nm_data_set()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1] = 0;
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_MD;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1]));
        }
    }
}
static void md_handle(uint32_t period)
{
    if (0U != tubclean_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        app_nm_data_set()->wash.tubcleanNum = 0;
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_FINISH;
    }
    else if (app_nm_data_get()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1] == 0U)
    {
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_FILL_STIR;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1]));
        }
    }
}
static void fillStir_handle(uint32_t period)
{
    static uint32_t fillingTimeoutMSec = 0;

    if (0U != tubclean_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        app_nm_data_set()->wash.tubcleanNum = 0;
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_FINISH;
    }
    else if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
    {
        app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1]));

        fillingTimeoutMSec += period;

        if ((APP_FILL_WATER_LEVEL_OK == app_fill_waterLevelState_get(app_nm_tubclean_fillStirTable_get(gu8_fillingAndStirring_counter)->waterLevelLiter, TUBCLEAN_FILL_MAXIMUM_ALLOWED_VARIANCE)) && (fillingTimeoutMSec >= app_nm_tubclean_fillStirTable_get(gu8_fillingAndStirring_counter)->timeMSec))
        {
            gu8_fillingAndStirring_counter++;
            fillingTimeoutMSec = 0;
        }

        if (gu8_fillingAndStirring_counter == TUBCLEAN_WATER_SUPPLY_AND_STIRRING_TIMES)
        {
            app_nm_data_set()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1] = 0U;
            gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_SOAK;
            gu8_fillingAndStirring_counter = 0;
        }
    }
    else
    {
        // do nothing
    }
}
static void soak_handle(uint32_t period)
{
    if (0U != tubclean_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        app_nm_data_set()->wash.tubcleanNum = 0;
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_FINISH;
    }
    else if (app_nm_data_get()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1] == 0U)
    {
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_WASH;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1]));
        }
    }
}
static void wash_handle(uint32_t period)
{
    if (0U != tubclean_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        app_nm_data_set()->wash.tubcleanNum = 0;
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_FINISH;
    }
    else if (app_nm_data_get()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1] == 0U)
    {
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_DRAIN;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1]));
        }
    }
}
static void drain_handle(uint32_t period)
{
    if (0U != tubclean_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        app_nm_data_set()->wash.tubcleanNum = 0;
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_FINISH;
    }
    else if (app_nm_data_get()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1] == 0U)
    {
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_SPIN;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            if (APP_DRAIN_TUB_EMPTY == app_drain_tubState_get())
            {
                app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1]));
            }
        }
    }
}
static void spin_handle(uint32_t period)
{
    if (0U != tubclean_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        app_nm_data_set()->wash.tubcleanNum = 0;
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_FINISH;
    }
    else if (app_nm_data_get()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1] == 0U)
    {
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_REGROUP;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, &(app_nm_data_set()->wash.tubcleanStateTimeMSec[gx_app_tubclean_state][app_nm_data_get()->wash.tubcleanNum - 1]));
        }
    }
}
static void regroup_handle(uint32_t period)
{
    app_nm_data_set()->wash.tubcleanNum--;
    if (0U == app_nm_data_get()->wash.tubcleanNum)
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_FINISH;
    }
    else
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_FILL_STIR;
    }
}
static void finish_handle(uint32_t period)
{
    if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
    {
        gx_app_tubclean_state = APP_NORMAL_MODE_TUBCLEAN_DONE;
    }
}

static app_nm_state_t tubclean_pauseRequest_handle(app_nm_state_t state)
{
    app_nm_state_t ret = state;

    if (APP_NORMAL_MODE_UI_SETTING_OK == app_nm_userInterface_startPause_check())
    {
        if (APP_NORMAL_MODE_UI_SETTING_OK == app_nm_userInterface_setting_check())
        {
            app_nm_prevState_set(ret);
            ret = APP_NORMAL_MODE_PAUSE;
            app_buzzer_tone_set(APP_BUZZER_TONE_PAUSE);
        }
        else if (APP_NORMAL_MODE_UI_SETTING_NOK == app_nm_userInterface_setting_check())
        {
            app_buzzer_tone_set(APP_BUZZER_TONE_INVALID_KEY);
        }
        else
        {
            // do nothing
        }
    }

    return ret;
}

static uint8_t tubclean_cycleEnd_check(void)
{
    uint8_t ret = 0;

    if (APP_NORMAL_MODE_UI_TUBCLEAN_OFF == app_userInterface_data_get()->normalMode.common.tubcleanSelection)
    {
        ret = 1;
    }

    return ret;
}
