#include "def.h"

#include "app_test_mode.h"
#include "app_blower.h"

void app_tm_blower_process(uint32_t period)
{
    switch (app_tm_type_get())
    {
        case APP_TM_WATER_LEVEL_CALIBRATION:
        case APP_TM_WEIGHT_CALIBRATION:
        case APP_TM_UNBALANCE_SPIN:
        case APP_TM_WATER_LEVEL_FREQ:
        case APP_TM_ERROR_COUNT_DISPLAY:
        case APP_TM_CYCLE_COUNT_DISPLAY:
        {
            app_blower_speedControl_set(APP_BLOWER_OFF);
        }
        break;

        case APP_TM_ELECTRICAL_PARTS:
        {
            // TODO:
        }
        break;

        case APP_TM_PCB:
        {
            // TODO:
        }
        break;

        case APP_TM_MIDDLE_INSPECTION:
        {
            // TODO:
        }
        break;

        case APP_TM_FINAL_INSPECTION:
        {
            // TODO:
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}
