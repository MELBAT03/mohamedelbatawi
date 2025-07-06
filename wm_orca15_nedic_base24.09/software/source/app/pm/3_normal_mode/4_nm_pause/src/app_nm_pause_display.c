#include "def.h"

#include "app_display.h"
#include "app_nm_display.h"
#include "app_nm.h"
#include "app_nm_wash.h"

#include "app_user_interface.h"

#include "handlers/output/inc/handler_display.h"
#include "handlers/output/inc/handler_lock_motor.h"

void app_nm_pause_display_process(uint32_t period)
{
    uint32_t totalRemainingTimeForCourse;

    totalRemainingTimeForCourse = app_nm_data_get()->totalTimeMSec / 60000U;
    ssd_symbolEnum_t data[3];

    handler_display_dimState_set(0);

    switch (app_userInterface_state_get())
    {
        case UI_ALTERNATE_NONE:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 0);

            if ((app_userInterface_data_get()->normalMode.wash.waterLevelSelection != APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0) || (app_userInterface_data_get()->normalMode.dry.dryWaterLevel != APP_NORMAL_MODE_DRY_UI_WATER_LEVEL_0))
            {
                app_display_remainingTime_put(period, totalRemainingTimeForCourse, 0);
            }
            else
            {
                app_display_dashSymbols_put();
            }
        }
        break;

        case UI_ALTERNATE_DELAY_START:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 0);

            app_display_ssd_blink(period, (app_userInterface_data_get()->normalMode.common.delayStartHourSelection), 5, 500, 500, FORMAT_HOURS);
        }
        break;

        case UI_ALTERNATE_SOAK:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 0);

            app_display_ssd_blink(period, app_userInterface_data_get()->normalMode.wash.soakSelection, 5, 500, 500, FORMAT_HOURS);
        }
        break;

        case UI_ALTERNATE_ANTI_WRINKLE:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 0);

            app_display_ssd_blink(period, (app_userInterface_data_get()->normalMode.wash.antiWrinkleSelection), 5, 500, 500, FORMAT_NUMBERS);
        }
        break;

        case UI_ALTERNATE_EXTRA_SPIN:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 0);

            app_display_ssd_blink(period, (app_userInterface_data_get()->normalMode.wash.superSpinSelection), 5, 500, 500, FORMAT_NUMBERS);
        }
        break;

        case UI_ALTERNATE_WASH:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 0);

            app_display_ssd_blink(period, (app_userInterface_data_get()->normalMode.wash.washSelection), 5, 500, 500, FORMAT_NUMBERS);
        }
        break;

        case UI_ALTERNATE_RINSE:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 1);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 0);

            if (app_userInterface_data_get()->normalMode.wash.extraRinseSelection == APP_NORMAL_MODE_UI_RINSE_EXTENDED)
            {
                data[2] = SSD_P_SYMBOL;
                data[1] = SSD_r_SYMBOL;
                data[0] = SSD_o_SYMBOL;
                app_display_ssd_charBlink(period, (uint8_t *)data, 3, 50, 500, 500);
            }
            else
            {
                app_display_ssd_blink(period, (app_userInterface_data_get()->normalMode.wash.rinseSelection), 50, 500, 500, FORMAT_NUMBERS);
            }
        }
        break;

        case UI_ALTERNATE_SPIN:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);

            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 1);

            app_display_ssd_blink(period, (app_userInterface_data_get()->normalMode.wash.spinSelection), 5, 500, 500, FORMAT_NUMBERS);
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }

    switch (app_nm_wash_state_get())
    {
        case APP_NORMAL_MODE_WASH_INIT:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterTempSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterTempSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_LED_GROUP, app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_POWDER_DETERGENT_LED_GROUP, !app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_LED_GROUP, app_userInterface_data_get()->normalMode.wash.steamTechSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.steamTechSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.spinSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.washSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.rinseSelection);
        }
        break;

        case APP_NORMAL_MODE_WASH_SETUP:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterTempSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterTempSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_LED_GROUP, app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_POWDER_DETERGENT_LED_GROUP, !app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_LED_GROUP, app_userInterface_data_get()->normalMode.wash.steamTechSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.steamTechSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.spinSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.washSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.rinseSelection);
        }
        break;

        case APP_NORMAL_MODE_WASH_SOAK:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterTempSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterTempSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_LED_GROUP, app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_POWDER_DETERGENT_LED_GROUP, !app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_LED_GROUP, app_userInterface_data_get()->normalMode.wash.steamTechSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.steamTechSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.spinSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.washSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.rinseSelection);
        }
        break;

        case APP_NORMAL_MODE_WASH_WASH:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterTempSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterTempSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_LED_GROUP, app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_POWDER_DETERGENT_LED_GROUP, !app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_LED_GROUP, app_userInterface_data_get()->normalMode.wash.steamTechSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.steamTechSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.spinSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_BUTTON_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.rinseSelection);
        }
        break;

        case APP_NORMAL_MODE_WASH_RINSE:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterTempSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_POWDER_DETERGENT_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.spinSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_BUTTON_LED_GROUP, 1);
        }
        break;

        case APP_NORMAL_MODE_WASH_SPIN:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_POWDER_DETERGENT_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_BUTTON_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_BUTTON_LED_GROUP, 0);
        }
        break;

        case APP_NORMAL_MODE_WASH_FINISH:
        {
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_POWDER_DETERGENT_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_BUTTON_LED_GROUP, 0);
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }

    handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, app_userInterface_data_get()->normalMode.common.courseSelection);
    handler_display_ledGroup_set(HANDLER_DISPLAY_LID_LOCK_LED_GROUP, handler_lockMotor_state_get());
    handler_display_ledGroup_set(HANDLER_DISPLAY_CHILD_LOCK_LED_GROUP, app_userInterface_data_get()->normalMode.common.childLockSelection);
    handler_display_ledGroup_set(HANDLER_DISPLAY_DELAY_START_LED_GROUP, app_userInterface_data_get()->normalMode.common.delayStartHourSelection);
    handler_display_ledGroup_set(HANDLER_DISPLAY_HEATER_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterHeatingSelection);
    handler_display_ledGroup_set(HANDLER_DISPLAY_SOFTNER_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_TUBCLEAN_LED_GROUP, app_userInterface_data_get()->normalMode.common.tubcleanSelection);

    handler_display_ledGroup_set(HANDLER_DISPLAY_STAIN_LEVEL_LED_GROUP, app_userInterface_data_get()->normalMode.wash.soilLevelSelection);

    handler_display_ledGroup_set(HANDLER_DISPLAY_EXTRA_SPIN_LED_GROUP, app_userInterface_data_get()->normalMode.wash.superSpinSelection);
    handler_display_ledGroup_set(HANDLER_DISPLAY_SOAK_LED_GROUP, app_userInterface_data_get()->normalMode.wash.soakSelection);

    handler_display_ledGroup_set(HANDLER_DISPLAY_ON_OFF_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_START_LED_GROUP, 1);

    handler_display_ledGroup_set(HANDLER_DISPLAY_STAIN_LEVEL_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.wash.soilLevelSelection);

    handler_display_ledGroup_set(HANDLER_DISPLAY_TUBCLEAN_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.common.tubcleanSelection);

    handler_display_ledGroup_set(HANDLER_DISPLAY_DELAY_START_BUTTON_LED_GROUP, app_userInterface_data_get()->normalMode.common.delayStartHourSelection);
}
