#include "def.h"
#include "app_nm_wash_fill.h"
#include "app_nm.h"
#include "app_fill.h"

#include "app_nm_wash_rinse.h"
#include "app_nm_wash_fill.h"

#include "app_nm_wash.h"

#include "handlers/input/inc/handler_temp_sensor.h"
#include "handlers/output/inc/handler_detergent_pump.h"

#define SHOWER_OPERATION_TIMEOUT_MSEC (30000U)

typedef enum fill_showerState
{
    SHOWER_OFF = 0,
    SHOWER_ON = 1
} fill_showerState_t;

static void fill_wash_wash_process(uint32_t period);
static void fill_wash_rinse_process(uint32_t period);
static void fill_wash_rinse_unbalance_handle(uint32_t period);

static void fill_wash_spin_process(uint32_t period);
static void fill_wash_spin_unbalance_handle(uint32_t period);

static void fill_showerOperation_handle(uint32_t period);

static uint16_t gu8_app_nm_wash_fill_waterTemp = 0;

void app_nm_wash_fill_process(uint32_t period)
{
    switch (app_nm_wash_state_get())
    {
        case APP_NORMAL_MODE_WASH_INIT:
        case APP_NORMAL_MODE_WASH_SOAK:
        case APP_NORMAL_MODE_WASH_STEAM:
        case APP_NORMAL_MODE_WASH_FINISH:
        {
            app_fill_stop();
        }
        break;

        case APP_NORMAL_MODE_WASH_SETUP:
        {
            app_fill_stop();
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH:
        {
            fill_wash_wash_process(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE:
        {
            fill_wash_rinse_process(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN:
        {
            fill_wash_spin_process(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

uint16_t app_nm_wash_fill_waterTempDuringWashFilling_get(void)
{
    return gu8_app_nm_wash_fill_waterTemp;
}

static void fill_wash_wash_process(uint32_t period)
{
    uint8_t waterLevel = 0;

    switch (app_nm_wash_wash_state_get())
    {
        case APP_NORMAL_MODE_WASH_WASH_INIT:
        case APP_NORMAL_MODE_WASH_WASH_MH:
        case APP_NORMAL_MODE_WASH_WASH_MD:
        case APP_NORMAL_MODE_WASH_WASH_M2:
        case APP_NORMAL_MODE_WASH_WASH_M3:
        case APP_NORMAL_MODE_WASH_WASH_M4:
        case APP_NORMAL_MODE_WASH_WASH_HEATING:
        case APP_NORMAL_MODE_WASH_WASH_STIR:
        case APP_NORMAL_MODE_WASH_WASH_UNTANGLE:
        case APP_NORMAL_MODE_WASH_WASH_FINISH:
        case APP_NORMAL_MODE_WASH_WASH_SOAK_TRANSITION:
        case APP_NORMAL_MODE_WASH_WASH_RINSE_TRANSITION:
        {
            app_fill_stop();
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_SETUP:
        {
            if (0 != app_nm_data_get()->wash.soakTimeMSec)
            {
                waterLevel = app_nm_data_get()->wash.soakStateWaterLevelLiter[app_nm_wash_wash_state_get()];
            }
            else
            {
                waterLevel = app_nm_data_get()->wash.washStateWaterLevelLiter[app_nm_wash_wash_state_get()];
            }

            if (APP_FILL_WATER_LEVEL_OK == app_fill_waterLevelState_get(waterLevel, WASH_FILL_MINIMUM_ALLOWED_VARIANCE))
            {
                app_fill_stop();
            }
            else
            {
                app_fill_handle(period, app_userInterface_data_get()->normalMode.wash.waterTempSelection, APP_FILL_PRE_WASH);
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_M0:
        case APP_NORMAL_MODE_WASH_WASH_M1:
        {
            app_fill_handle(period, app_userInterface_data_get()->normalMode.wash.waterTempSelection, APP_FILL_PRE_WASH);
            gu8_app_nm_wash_fill_waterTemp = handler_waterTempSensor_temp_get();
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_REFILL:
        {
            app_fill_handle(period, app_userInterface_data_get()->normalMode.wash.waterTempSelection, APP_FILL_MAIN_WASH);
            gu8_app_nm_wash_fill_waterTemp = handler_waterTempSensor_temp_get();
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

static void fill_wash_rinse_process(uint32_t period)
{
    uint8_t softenerWaterLevel = 0;
    app_fill_stage_t softenerState;

    switch (app_nm_wash_rinse_state_get())
    {
        case APP_NORMAL_MODE_WASH_RINSE_INIT:
        case APP_NORMAL_MODE_WASH_RINSE_DRAIN:
        case APP_NORMAL_MODE_WASH_RINSE_SPIN:
        case APP_NORMAL_MODE_WASH_RINSE_SPIN_BRAKE:
        case APP_NORMAL_MODE_WASH_RINSE_UNTANGLE:
        case APP_NORMAL_MODE_WASH_RINSE_REGROUP:
        case APP_NORMAL_MODE_WASH_RINSE_FINISH:
        case APP_NORMAL_MODE_WASH_RINSE_SPIN_TRANSITION:
        {
            app_fill_stop();
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_FILL:
        {
            softenerState = APP_FILL_RINSE;

            if (APP_NORMAL_MODE_UI_RINSE_1_TIMES == app_nm_data_get()->wash.rinseNum)
            {
                softenerWaterLevel = app_nm_wash_fill_softnerWaterLevel_get(app_userInterface_data_get()->normalMode.common.courseSelection, app_userInterface_data_get()->normalMode.wash.waterLevelSelection)->softnerWaterLevelLiter;

                if (APP_FILL_WATER_LEVEL_OK == app_fill_waterLevelState_get(softenerWaterLevel, RINSE_FILL_MAXIMUM_ALLOWED_VARIANCE))
                {
                    softenerState = APP_FILL_SOFTENER;
                }
            }

            app_fill_handle(period, app_userInterface_data_get()->normalMode.wash.waterTempSelection, softenerState);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_STIR:
        {
            if (APP_NORMAL_MODE_UI_RINSE_OVERFLOW_OFF_SHOWER_OFF == app_userInterface_data_get()->normalMode.wash.extraRinseSelection)
            {
                app_fill_stop();
            }
            else if (APP_NORMAL_MODE_UI_RINSE_OVERFLOW_ON_SHOWER_OFF == app_userInterface_data_get()->normalMode.wash.extraRinseSelection)
            {
                // NOTE: accessing the cold valve directly as here we don't want to log any fill errors
                app_fill_coldWaterValve_state_set(APP_FILL_WATER_VALVE_ON);
            }
            else if (APP_NORMAL_MODE_UI_RINSE_OVERFLOW_OFF_SHOWER_ON == app_userInterface_data_get()->normalMode.wash.extraRinseSelection)
            {
                fill_showerOperation_handle(period);
            }
            else if (APP_NORMAL_MODE_UI_RINSE_OVERFLOW_ON_SHOWER_ON == app_userInterface_data_get()->normalMode.wash.extraRinseSelection)
            {
                // NOTE: accessing the cold valve directly as here we don't want to log any fill errors
                fill_showerOperation_handle(period);
                app_fill_coldWaterValve_state_set(APP_FILL_WATER_VALVE_ON);
            }
            else
            {
                // do nothing
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE:
        {
            fill_wash_rinse_unbalance_handle(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}
static void fill_wash_rinse_unbalance_handle(uint32_t period)
{
    switch (app_nm_wash_rinse_unbalanceState_get())
    {
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_INIT:
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_SETUP:
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_STIR:
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_DRAIN:
        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_FINISH:
        {
            app_fill_stop();
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_FILL:
        {
            app_fill_handle(period, app_userInterface_data_get()->normalMode.wash.waterTempSelection, APP_FILL_MAIN_WASH);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE_UNBALANCE_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

static void fill_wash_spin_process(uint32_t period)
{
    switch (app_nm_wash_spin_state_get())
    {
        case APP_NORMAL_MODE_WASH_SPIN_INIT:
        case APP_NORMAL_MODE_WASH_SPIN_DRAIN:
        case APP_NORMAL_MODE_WASH_SPIN_SPIN:
        case APP_NORMAL_MODE_WASH_SPIN_SUPER_SPIN:
        case APP_NORMAL_MODE_WASH_SPIN_BRAKE:
        case APP_NORMAL_MODE_WASH_SPIN_ANTI_WRINKLE:
        case APP_NORMAL_MODE_WASH_SPIN_FINISH:
        {
            app_fill_stop();
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE:
        {
            fill_wash_spin_unbalance_handle(period);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_END_TRANSITION:
        case APP_NORMAL_MODE_WASH_SPIN_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

static void fill_wash_spin_unbalance_handle(uint32_t period)
{
    switch (app_nm_wash_spin_unbalanceState_get())
    {
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_INIT:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_SETUP:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_STIR:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_DRAIN:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_SUPER_SPIN_STIR:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_FINISH:
        {
            app_fill_stop();
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_FILL:
        {
            app_fill_handle(period, app_userInterface_data_get()->normalMode.wash.waterTempSelection, APP_FILL_MAIN_WASH);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_DONE:
        case APP_NORMAL_MODE_WASH_SPIN_UNBALANCE_ERROR:
        default:
        {
            // do nothing
        }
        break;
    }
}

static void fill_showerOperation_handle(uint32_t period)
{
    static fill_showerState_t showerState = SHOWER_ON;
    static uint32_t timeoutMSec = 0;

    timeoutMSec += period;

    switch (showerState)
    {
        case SHOWER_OFF:
        {
            app_fill_showerWaterValve_state_set(APP_FILL_WATER_VALVE_OFF);

            if (timeoutMSec >= SHOWER_OPERATION_TIMEOUT_MSEC)
            {
                timeoutMSec = 0;
                showerState = SHOWER_ON;
            }
        }
        break;

        case SHOWER_ON:
        {
            app_fill_showerWaterValve_state_set(APP_FILL_WATER_VALVE_ON);

            if (timeoutMSec >= SHOWER_OPERATION_TIMEOUT_MSEC)
            {
                timeoutMSec = 0;
                showerState = SHOWER_OFF;
            }
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}
