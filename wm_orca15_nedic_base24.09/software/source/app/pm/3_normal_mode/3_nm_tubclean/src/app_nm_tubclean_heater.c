#include "def.h"
#include "app_heater.h"
#include "app_nm_tubclean_heater.h"
#include "app_nm_tubclean.h"

static void heater_steamTech_handle(uint32_t period);

static app_nm_tubclean_steamTech_heaterState_t heaterState = TUBCLEAN_HEATER_STEAM_TECH_REQUIRED_ON;

void app_nm_tubclean_heater_process(uint32_t period)
{
    if ((app_nm_tubclean_state_get() == APP_NORMAL_MODE_TUBCLEAN_MH) || (app_nm_tubclean_state_get() == APP_NORMAL_MODE_TUBCLEAN_MD))
    {
        heater_steamTech_handle(period);
    }
    else
    {
        app_heater_plateState_set(APP_HEATER_OFF);
        heaterState = TUBCLEAN_HEATER_STEAM_TECH_REQUIRED_ON;
    }
}

app_nm_tubclean_steamTech_heaterState_t app_nm_tubclean_heater_steamTechHeaterState_get(void)
{
    return heaterState;
}

void app_nm_tubclean_heater_steamTechHeaterState_set(app_nm_tubclean_steamTech_heaterState_t state)
{
    heaterState = state;
}

static void heater_steamTech_handle(uint32_t period)
{
    switch (heaterState)
    {
        case TUBCLEAN_HEATER_STEAM_TECH_REQUIRED_ON:
        {
            app_heater_plateState_set(APP_HEATER_TWO_PLATES);
            if (app_nm_tubclean_heater_steamTechTargetONTemp_get() <= app_heater_temp_get())
            {
                heaterState = TUBCLEAN_HEATER_STEAM_TECH_REQUIRED_OFF;
            }
        }
        break;

        case TUBCLEAN_HEATER_STEAM_TECH_REQUIRED_OFF:
        {
            app_heater_plateState_set(APP_HEATER_OFF);
            if (app_nm_tubclean_heater_steamTechTargetOFFTemp_get() >= app_heater_temp_get())
            {
                heaterState = TUBCLEAN_HEATER_STEAM_TECH_REQUIRED_ON;
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
