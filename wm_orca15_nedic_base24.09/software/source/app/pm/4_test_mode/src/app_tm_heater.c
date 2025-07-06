#include "def.h"

#include "app_test_mode.h"

#include "app_heater.h"

void app_tm_heater_process(uint32_t period)
{
    switch (app_tm_type_get())
    {
        case APP_TM_WATER_LEVEL_CALIBRATION:
        {
            app_heater_plateState_set(HANDLER_HEATER_OFF);
        }
        break;

        case APP_TM_WEIGHT_CALIBRATION:
        {
            app_heater_plateState_set(HANDLER_HEATER_OFF);
        }
        break;

        case APP_TM_ELECTRICAL_PARTS:
        {
        }
        break;

        case APP_TM_UNBALANCE_SPIN:
        {
        }
        break;

        case APP_TM_WATER_LEVEL_FREQ:
        {
        }
        break;

        case APP_TM_PCB:
        {
        }
        break;

        case APP_TM_MIDDLE_INSPECTION:
        {
        }
        break;

        case APP_TM_FINAL_INSPECTION:
        {
        }
        break;

        case APP_TM_ERROR_COUNT_DISPLAY:
        {
        }
        break;

        case APP_TM_CYCLE_COUNT_DISPLAY:
        {
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}
