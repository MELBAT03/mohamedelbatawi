#include "def.h"
#include "app_heater.h"
#include "app_nm_wash_heater.h"
#include "app_nm_wash.h"

#define STEAM_HEATER_TARGET_TEMP          (90UL)
#define STEAM_ONE_PLATE_STEP_TIMEOUT_MSEC (300000U)

typedef enum wash_steam_heater_state
{
    HEATER_STEAM_INIT,
    HEATER_STEAM_TWO_PLATES,
    HEATER_STEAM_ONE_PLATE
} wash_steam_heater_state_t;

static void heater_steam_handle(uint32_t period);
static void heater_steamTech_handle(uint32_t period);
static void heater_heating_handle(uint32_t period);

static app_wash_steamTech_heaterState_t heaterState = HEATER_STEAM_TECH_REQUIRED_ON;

void app_nm_wash_heater_process(uint32_t period)
{
    switch (app_nm_wash_state_get())
    {
        case APP_NORMAL_MODE_WASH_WASH:
        {
            if ((app_userInterface_data_get()->normalMode.wash.steamTechSelection == APP_NORMAL_MODE_UI_STEAM_TECH_ON) && ((app_nm_wash_wash_state_get() == APP_NORMAL_MODE_WASH_WASH_MH) || (app_nm_wash_wash_state_get() == APP_NORMAL_MODE_WASH_WASH_MD)))
            {
                heater_steamTech_handle(period);
            }
            else if ((app_userInterface_data_get()->normalMode.wash.waterHeatingSelection == APP_NORMAL_MODE_UI_WATER_HEATING_ON) && (app_nm_wash_wash_state_get() == APP_NORMAL_MODE_WASH_WASH_HEATING))
            {
                heater_heating_handle(period);
            }
            else
            {
                app_heater_plateState_set(APP_HEATER_OFF);
                heaterState = HEATER_STEAM_TECH_REQUIRED_ON;
            }
        }
        break;

        case APP_NORMAL_MODE_WASH_STEAM:
        {
            heater_steam_handle(period);
        }
        break;

        default:
        {
            app_heater_plateState_set(APP_HEATER_OFF);
        }
        break;
    }
}

app_wash_steamTech_heaterState_t app_nm_wash_heater_steamTechHeaterState_get(void)
{
    return heaterState;
}

void app_nm_wash_heater_steamTechHeaterState_set(app_wash_steamTech_heaterState_t state)
{
    heaterState = state;
}

static void heater_steamTech_handle(uint32_t period)
{
    switch (heaterState)
    {
        case HEATER_STEAM_TECH_REQUIRED_ON:
        {
            app_heater_plateState_set(APP_HEATER_TWO_PLATES);
            if (app_nm_wash_heater_steamTechONTemp_get() <= app_heater_temp_get())
            {
                heaterState = HEATER_STEAM_TECH_REQUIRED_OFF;
            }
        }
        break;

        case HEATER_STEAM_TECH_REQUIRED_OFF:
        {
            app_heater_plateState_set(APP_HEATER_OFF);
            if (app_nm_wash_heater_steamTechOFFTemp_get() >= app_heater_temp_get())
            {
                heaterState = HEATER_STEAM_TECH_REQUIRED_ON;
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

static void heater_heating_handle(uint32_t period)
{
    switch (heaterState)
    {
        case HEATER_STEAM_TECH_REQUIRED_ON:
        {
            app_heater_plateState_set(APP_HEATER_TWO_PLATES);
            if (app_nm_wash_heater_steamTechTargetONTemp_get(app_userInterface_data_get()->normalMode.common.courseSelection) <= app_heater_temp_get())
            {
                heaterState = HEATER_STEAM_TECH_REQUIRED_OFF;
            }
        }
        break;

        case HEATER_STEAM_TECH_REQUIRED_OFF:
        {
            app_heater_plateState_set(APP_HEATER_OFF);
            if (app_nm_wash_heater_steamTechTargetOFFTemp_get(app_userInterface_data_get()->normalMode.common.courseSelection) >= app_heater_temp_get())
            {
                heaterState = HEATER_STEAM_TECH_REQUIRED_ON;
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

static void heater_steam_handle(uint32_t period)
{
    static uint32_t timeoutMSec = 0;
    static wash_steam_heater_state_t heaterPattern = HEATER_STEAM_INIT;

    switch (heaterPattern)
    {
        case HEATER_STEAM_INIT:
        {
            heaterPattern = HEATER_STEAM_TWO_PLATES;
            timeoutMSec = 0;
        }
        break;

        case HEATER_STEAM_TWO_PLATES:
        {
            app_heater_plateState_set(APP_HEATER_TWO_PLATES);
            if (STEAM_HEATER_TARGET_TEMP <= app_heater_temp_get())
            {
                heaterPattern = HEATER_STEAM_ONE_PLATE;
                timeoutMSec = 0;
            }
        }
        break;

        case HEATER_STEAM_ONE_PLATE:
        {
            app_heater_plateState_set(APP_HEATER_ONE_PLATE);
            timeoutMSec += period;
            if (timeoutMSec >= STEAM_ONE_PLATE_STEP_TIMEOUT_MSEC)
            {
                timeoutMSec = 0;
                heaterPattern = HEATER_STEAM_TWO_PLATES;
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
