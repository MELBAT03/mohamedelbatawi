#include "def.h"

#include "app_display.h"
#include "app_nm_display.h"

#include "app_nm.h"
#include "app_nm_init.h"

#include "handlers/output/inc/handler_lock_motor.h"

void app_nm_init_display_process(uint32_t period)
{
    static app_display_ledBlink_t courseBlinking = {.ledID = HANDLER_DISPLAY_COURSE_LED_GROUP, .status = LED_STATUS_ON, .timeInState = 0};
    static app_display_ledBlink_t delayBlinking = {.ledID = HANDLER_DISPLAY_DELAY_START_LED_GROUP, .status = LED_STATUS_ON, .timeInState = 0};
    uint32_t totalRemainingTimeForCourse;

    totalRemainingTimeForCourse = app_nm_data_get()->totalTimeMSec / 60000U;

    handler_display_dimState_set(0);

    handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);

    if (app_nm_init_state_get() == APP_NORMAL_MODE_INIT_LINT_FILTER_CHECK)
    {
        app_display_ssdSymbols_put(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_c_SYMBOL, SSD_F_SYMBOL);
    }
    else
    {
        if (app_userInterface_data_get()->normalMode.common.delayStartHourSelection != APP_NORMAL_MODE_UI_DELAY_START_TIME_0_HOUR)
        {
            app_display_remainingTime_put(period, app_nm_data_get()->delayStart, 1);

            handler_display_led_clear();
            app_display_led_blink(&delayBlinking, period, 700, 700, 1);
        }
        else
        {
            if (app_userInterface_data_get()->normalMode.wash.waterLevelSelection != APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0)
            {
                app_display_remainingTime_put(period, totalRemainingTimeForCourse, 1);
            }
            else
            {
                app_display_dashSymbols_put();
            }

            handler_display_ledGroup_set(HANDLER_DISPLAY_CHILD_LOCK_LED_GROUP, app_userInterface_data_get()->normalMode.common.childLockSelection);
            handler_display_ledGroup_set(HANDLER_DISPLAY_LID_LOCK_LED_GROUP, handler_lockMotor_state_get());

            app_display_led_blink(&courseBlinking, period, 1000, 500, app_userInterface_data_get()->normalMode.common.courseSelection);

            handler_display_ledGroup_set(HANDLER_DISPLAY_SOFTNER_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_LED_GROUP, app_userInterface_data_get()->normalMode.wash.steamTechSelection);

            handler_display_ledGroup_set(HANDLER_DISPLAY_ON_OFF_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_START_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STAIN_LEVEL_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_BUTTON_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_TUBCLEAN_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_BUTTON_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_BUTTON_LED_GROUP, 1);
            handler_display_ledGroup_set(HANDLER_DISPLAY_DELAY_START_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_BUTTON_LED_GROUP, 0);
            handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_BUTTON_LED_GROUP, 0);
        }
    }
}
