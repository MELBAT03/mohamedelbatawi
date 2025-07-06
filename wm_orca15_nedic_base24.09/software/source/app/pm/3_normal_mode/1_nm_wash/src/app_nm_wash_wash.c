#include "def.h"

#include "app_motor.h"
#include "app_nm_wash.h"
#include "app_fill.h"

#include "app_nm.h"

#include "app_nm_wash_heater.h"

#define WASH_LEAKAGE_LEVEL_LITER (10U)

typedef struct wash_wash_internalDataRead
{
    const uint32_t *stateTimeMSec[APP_NORMAL_MODE_WASH_STATES_WITH_DATA];
    const uint8_t *stateWaterLevelLiter[APP_NORMAL_MODE_WASH_STATES_WITH_DATA];
} wash_wash_internalDataRead_t;

typedef struct wash_wash_internalDataWrite
{
    uint32_t *stateTimeMSec[APP_NORMAL_MODE_WASH_STATES_WITH_DATA];
    uint8_t *stateWaterLevelLiter[APP_NORMAL_MODE_WASH_STATES_WITH_DATA];
} wash_wash_internalDataWrite_t;

static void init_handle(uint32_t period);
static void refill_handle(uint32_t period);
static void setup_handle(uint32_t period);
static void m0_handle(uint32_t period);
static void mh_handle(uint32_t period);
static void md_handle(uint32_t period);
static void m1_handle(uint32_t period);
static void m2_handle(uint32_t period);
static void m3_handle(uint32_t period);
static void m4_handle(uint32_t period);
static void heating_handle(uint32_t period);
static void stir_handle(uint32_t period);
static void untangle_handle(uint32_t period);
static void finish_handle(uint32_t period);
static void soak_handle(uint32_t period);
static void rinseTransition_handle(uint32_t period);

static uint8_t wash_cycleEnd_check(void);

static app_nm_wash_wash_state_t gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_INIT;
static app_nm_wash_wash_state_t gx_wash_wash_prevState = APP_NORMAL_MODE_WASH_WASH_INIT;

static const app_nm_wash_data_t *dataRead;

static wash_wash_internalDataRead_t internalDataRead;
static wash_wash_internalDataWrite_t internalDataWrite;

static app_nm_wash_wash_waterLeakage_state_t leakageState = APP_NORMAL_MODE_WASH_WASH_NO_LEAKAGE;

void app_nm_wash_wash_process(uint32_t period)
{
    static app_nm_wash_data_t *dataWrite;

    static void (*const handlers[])(uint32_t period) = {
        init_handle,
        refill_handle,
        setup_handle,
        m0_handle,
        mh_handle,
        md_handle,
        m1_handle,
        m2_handle,
        m3_handle,
        m4_handle,
        heating_handle,
        stir_handle,
        untangle_handle,
        finish_handle,
        soak_handle,
        rinseTransition_handle,
        NULL};

    dataRead = &(app_nm_data_get()->wash);
    dataWrite = &(app_nm_data_set()->wash);

    if (0U != dataRead->soakTimeMSec)
    {
        for (uint8_t i = 0; i < APP_NORMAL_MODE_WASH_STATES_WITH_DATA; i++)
        {
            internalDataRead.stateTimeMSec[i] = &(dataRead->soakStateTimeMSec[i]);
            internalDataRead.stateWaterLevelLiter[i] = &(dataRead->soakStateWaterLevelLiter[i]);

            internalDataWrite.stateTimeMSec[i] = &(dataWrite->soakStateTimeMSec[i]);
            internalDataWrite.stateWaterLevelLiter[i] = &(dataWrite->soakStateWaterLevelLiter[i]);
        }
    }
    else
    {
        for (uint8_t i = 0; i < APP_NORMAL_MODE_WASH_STATES_WITH_DATA; i++)
        {
            internalDataRead.stateTimeMSec[i] = &(dataRead->washStateTimeMSec[i]);
            internalDataRead.stateWaterLevelLiter[i] = &(dataRead->washStateWaterLevelLiter[i]);

            internalDataWrite.stateTimeMSec[i] = &(dataWrite->washStateTimeMSec[i]);
            internalDataWrite.stateWaterLevelLiter[i] = &(dataWrite->washStateWaterLevelLiter[i]);
        }
    }

    if (handlers[gx_wash_wash_state] != NULL)
    {
        handlers[gx_wash_wash_state](period);
    }
}

app_nm_wash_wash_state_t app_nm_wash_wash_state_get(void)
{
    return gx_wash_wash_state;
}

app_nm_wash_wash_state_t app_nm_wash_wash_refillExecludedState_get(void)
{
    app_nm_wash_wash_state_t ret;
    if (APP_NORMAL_MODE_WASH_WASH_REFILL == gx_wash_wash_state)
    {
        ret = gx_wash_wash_prevState;
    }
    else
    {
        ret = gx_wash_wash_state;
    }

    return ret;
}

void app_nm_wash_wash_state_set(app_nm_wash_wash_state_t value)
{
    gx_wash_wash_state = value;
}

void app_nm_wash_wash_reset(void)
{
    gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_INIT;
    gx_wash_wash_prevState = APP_NORMAL_MODE_WASH_WASH_INIT;
}

app_nm_wash_wash_waterLeakage_state_t app_nm_wash_wash_leakageState_get(void)
{
    return leakageState;
}

static void init_handle(uint32_t period)
{
    app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_SETUP;
}
static void refill_handle(uint32_t period)
{
    uint32_t reqWaterLevelLiter;

    if (0U != wash_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_FINISH;
    }
    else if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
    {
        reqWaterLevelLiter = *(internalDataRead.stateWaterLevelLiter[gx_wash_wash_prevState]);
        if (APP_FILL_WATER_LEVEL_OK == app_fill_waterLevelState_get(reqWaterLevelLiter, WASH_FILL_MINIMUM_ALLOWED_VARIANCE))
        {
            gx_wash_wash_state = gx_wash_wash_prevState;
        }
        else if ((APP_NORMAL_MODE_WASH_WASH_STIR == gx_wash_wash_prevState) && (APP_FILL_WATER_LEVEL_OK == app_fill_waterLevelState_get(WASH_LEAKAGE_LEVEL_LITER, WASH_FILL_MINIMUM_ALLOWED_VARIANCE)))
        {
            leakageState = APP_NORMAL_MODE_WASH_WASH_LEAKAGE;
        }
        else
        {
            // do nothing
        }
    }
}
static void setup_handle(uint32_t period)
{
    if (0U != wash_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_FINISH;
    }
    else if (0U == *(internalDataRead.stateTimeMSec[gx_wash_wash_state]))
    {
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_M0;
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, internalDataWrite.stateTimeMSec[gx_wash_wash_state]);
        }
    }
}
static void m0_handle(uint32_t period)
{
    if (0U != wash_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_FINISH;
    }
    else if (APP_FILL_WATER_LEVEL_OK == app_fill_waterLevelState_get(*(internalDataRead.stateWaterLevelLiter[gx_wash_wash_state]), WASH_FILL_MINIMUM_ALLOWED_VARIANCE))
    {
        *(internalDataWrite.stateTimeMSec[gx_wash_wash_state]) = 0;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_MH;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, internalDataWrite.stateTimeMSec[gx_wash_wash_state]);
        }
    }
}
static void mh_handle(uint32_t period)
{
    if (0U != wash_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_FINISH;
    }
    else if ((app_nm_wash_heater_steamTechHeaterState_get() == HEATER_STEAM_TECH_REQUIRED_OFF) || (app_userInterface_data_get()->normalMode.wash.steamTechSelection == APP_NORMAL_MODE_UI_STEAM_TECH_OFF))
    {
        *(internalDataWrite.stateTimeMSec[gx_wash_wash_state]) = 0;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_MD;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, internalDataWrite.stateTimeMSec[gx_wash_wash_state]);
        }
    }
}
static void md_handle(uint32_t period)
{
    if (0U != wash_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_FINISH;
    }
    else if ((0U == *(internalDataRead.stateTimeMSec[gx_wash_wash_state])) || (app_userInterface_data_get()->normalMode.wash.steamTechSelection == APP_NORMAL_MODE_UI_STEAM_TECH_OFF))
    {
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_M1;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, internalDataWrite.stateTimeMSec[gx_wash_wash_state]);
        }
    }
}
static void m1_handle(uint32_t period)
{
    if (0U != wash_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_FINISH;
    }
    else if (0U == *(internalDataRead.stateTimeMSec[gx_wash_wash_state]))
    {
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_M2;
    }
    else if (APP_FILL_WATER_LEVEL_OK == app_fill_waterLevelState_get(*(internalDataRead.stateWaterLevelLiter[gx_wash_wash_state]), WASH_FILL_MINIMUM_ALLOWED_VARIANCE))
    {
        *(internalDataWrite.stateTimeMSec[gx_wash_wash_state]) = 0;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, internalDataWrite.stateTimeMSec[gx_wash_wash_state]);
        }
    }
}
static void m2_handle(uint32_t period)
{
    if (0U != wash_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_FINISH;
    }
    else if (0U == *(internalDataRead.stateTimeMSec[gx_wash_wash_state]))
    {
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_M3;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, internalDataWrite.stateTimeMSec[gx_wash_wash_state]);
        }
    }
}
static void m3_handle(uint32_t period)
{
    if (0U != wash_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_FINISH;
    }
    else if ((0U == *(internalDataRead.stateTimeMSec[gx_wash_wash_state])) && (APP_FILL_WATER_LEVEL_OK == app_fill_waterLevelState_get(*(internalDataRead.stateWaterLevelLiter[gx_wash_wash_state]), WASH_FILL_MINIMUM_ALLOWED_VARIANCE)))
    {
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_M4;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, internalDataWrite.stateTimeMSec[gx_wash_wash_state]);

            if (APP_FILL_WATER_LEVEL_NOK == app_fill_waterLevelState_get(*(internalDataRead.stateWaterLevelLiter[gx_wash_wash_state]), WASH_FILL_MAXIMUM_ALLOWED_VARIANCE))
            {
                gx_wash_wash_prevState = gx_wash_wash_state;

                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
                gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_REFILL;
            }
        }
    }
}
static void m4_handle(uint32_t period)
{
    if (0U != wash_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_FINISH;
    }
    else if ((0U == *(internalDataRead.stateTimeMSec[gx_wash_wash_state])) && (APP_FILL_WATER_LEVEL_OK == app_fill_waterLevelState_get(*(internalDataRead.stateWaterLevelLiter[gx_wash_wash_state]), WASH_FILL_MINIMUM_ALLOWED_VARIANCE)))
    {
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_HEATING;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, internalDataWrite.stateTimeMSec[gx_wash_wash_state]);

            if (APP_FILL_WATER_LEVEL_NOK == app_fill_waterLevelState_get(*(internalDataRead.stateWaterLevelLiter[gx_wash_wash_state]), WASH_FILL_MAXIMUM_ALLOWED_VARIANCE))
            {
                gx_wash_wash_prevState = gx_wash_wash_state;

                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
                gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_REFILL;
            }
        }
    }
}
static void heating_handle(uint32_t period)
{
    if (0U != wash_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_FINISH;
    }
    else if ((app_nm_wash_heater_steamTechHeaterState_get() == HEATER_STEAM_TECH_REQUIRED_OFF) || (app_userInterface_data_get()->normalMode.wash.waterHeatingSelection == APP_NORMAL_MODE_UI_WATER_HEATING_OFF))
    {
        *(internalDataWrite.stateTimeMSec[gx_wash_wash_state]) = 0;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_STIR;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, internalDataWrite.stateTimeMSec[gx_wash_wash_state]);
        }
    }
}
static void stir_handle(uint32_t period)
{
    if (0U != wash_cycleEnd_check())
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_FINISH;
    }
    else if (0U == *(internalDataRead.stateTimeMSec[gx_wash_wash_state]))
    {
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_UNTANGLE;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, internalDataWrite.stateTimeMSec[gx_wash_wash_state]);

            if (APP_FILL_WATER_LEVEL_NOK == app_fill_waterLevelState_get(*(internalDataRead.stateWaterLevelLiter[gx_wash_wash_state]), WASH_FILL_MAXIMUM_ALLOWED_VARIANCE))
            {
                gx_wash_wash_prevState = gx_wash_wash_state;

                app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
                gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_REFILL;
            }
        }
    }
}
static void untangle_handle(uint32_t period)
{
    if ((0U != wash_cycleEnd_check()) || (0U == *(internalDataRead.stateTimeMSec[gx_wash_wash_state])))
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_FINISH;
    }
    else
    {
        if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
        {
            app_nm_remainingTime_process(period, internalDataWrite.stateTimeMSec[gx_wash_wash_state]);
        }
    }
}
static void finish_handle(uint32_t period)
{
    if (APP_CLUTCH_MODE_OPERATION == app_motor_data_get()->clutchMode)
    {
        if (0U != dataRead->soakTimeMSec)
        {
            app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
            gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_SOAK_TRANSITION;
        }
        else
        {
            gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_RINSE_TRANSITION;
            app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        }
    }
}
static void soak_handle(uint32_t period)
{
    if (0U == dataRead->soakTimeMSec)
    {
        app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
        gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_STIR;
    }
}

static void rinseTransition_handle(uint32_t period)
{
    app_motor_data_get()->clutchMode = APP_CLUTCH_MODE_HALTING_MOTOR;
    gx_wash_wash_state = APP_NORMAL_MODE_WASH_WASH_DONE;
}

static uint8_t wash_cycleEnd_check(void)
{
    uint8_t ret = 0;

    if (APP_NORMAL_MODE_UI_WASH_TIME_0_MIN == app_userInterface_data_get()->normalMode.wash.washSelection)
    {
        ret = 1;
    }

    return ret;
}
