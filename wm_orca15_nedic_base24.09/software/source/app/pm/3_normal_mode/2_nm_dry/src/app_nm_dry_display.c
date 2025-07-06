#include "def.h"

#include "app_display.h"
#include "app_nm_display.h"
#include "app_nm.h"
#include "app_nm_dry.h"

#include "handlers/output/inc/handler_lock_motor.h"

void app_nm_dry_display_process(uint32_t period)
{
    static app_display_ledBlink_t blinking = {.ledID = HANDLER_DISPLAY_DRY_TIME_LED_GROUP, .status = LED_STATUS_ON, .timeInState = 0};
    uint32_t totalRemainingTimeForCourse;

    totalRemainingTimeForCourse = app_nm_data_get()->totalTimeMSec / 60000U;

    if (app_nm_dry_operation_state_get() != APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED_RESET)
    {
        app_display_remainingTime_put(period, totalRemainingTimeForCourse, 1);

        handler_display_ledGroup_set(HANDLER_DISPLAY_COURSE_LED_GROUP, app_userInterface_data_get()->normalMode.common.courseSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, app_userInterface_data_get()->normalMode.wash.washSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterTempSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_EXTRA_SPIN_LED_GROUP, app_userInterface_data_get()->normalMode.wash.superSpinSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_DELAY_START_LED_GROUP, app_userInterface_data_get()->normalMode.common.delayStartHourSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_CHILD_LOCK_LED_GROUP, app_userInterface_data_get()->normalMode.common.childLockSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, app_userInterface_data_get()->normalMode.wash.spinSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, app_userInterface_data_get()->normalMode.wash.rinseSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_ANTI_WRINKLE_LED_GROUP, app_userInterface_data_get()->normalMode.wash.antiWrinkleSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_SOAK_LED_GROUP, app_userInterface_data_get()->normalMode.wash.soakSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_STAIN_LEVEL_LED_GROUP, app_userInterface_data_get()->normalMode.wash.soilLevelSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_LID_LOCK_LED_GROUP, handler_lockMotor_state_get());

        handler_display_ledGroup_set(HANDLER_DISPLAY_OPERATION_LED_GROUP, app_userInterface_data_get()->normalMode.common.operationSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_LED_GROUP, app_userInterface_data_get()->normalMode.wash.steamSelection);
        handler_display_ledGroup_set(HANDLER_DISPLAY_DRY_TIME_LED_GROUP, app_userInterface_data_get()->normalMode.dry.dryTimeSelection);

        app_display_led_blink(&blinking, period, 700, 700, app_userInterface_data_get()->normalMode.dry.dryTimeSelection);
    }
    else
    {
        app_display_ssdSymbols_put(SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_c_SYMBOL, SSD_F_SYMBOL);
    }
}
