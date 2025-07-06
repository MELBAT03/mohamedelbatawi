#include "def.h"

#include "app_setup_display.h"
#include "app_display.h"
#include "app_user_interface.h"

#include "handlers/output/inc/handler_display.h"

#define SETUP_COURSE_APPERANCE_RATE (200U)

static void display_setup_handle(uint32_t period);

void app_setup_display_process(uint32_t period)
{
    display_setup_handle(period);
}

static void display_setup_handle(uint32_t period)
{
    static uint32_t counterMSec = 0;
    static uint8_t course = 0;

    handler_display_ssd_set(0, SSD_NULL_SYMBOL);
    handler_display_ssd_set(1, SSD_NULL_SYMBOL);
    handler_display_ssd_set(2, SSD_NULL_SYMBOL);
    handler_display_ssd_set(3, SSD_NULL_SYMBOL);

    counterMSec += period;
    if (counterMSec >= SETUP_COURSE_APPERANCE_RATE)
    {
        counterMSec = 0;

        if (course < 12)
        {
            course++;
        }
    }

    handler_display_dimState_set(0);
    handler_display_ssdDP_set(SSD_DP_OFF);
    handler_display_courseLedSequence_traverse(course);

    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_LID_LOCK_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_CHILD_LOCK_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_DELAY_START_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_HEATER_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_POWDER_DETERGENT_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_SOFTNER_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_TIMES_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_MIN_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_TUBCLEAN_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_STAIN_LEVEL_LED_GROUP, 0);

    handler_display_ledGroup_set(HANDLER_DISPLAY_EXTRA_SPIN_LED_GROUP, 0);
    handler_display_ledGroup_set(HANDLER_DISPLAY_SOAK_LED_GROUP, 0);

    handler_display_ledGroup_set(HANDLER_DISPLAY_ON_OFF_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_START_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_STAIN_LEVEL_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_STEAM_TECH_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_SPIN_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_TUBCLEAN_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_GEL_DETERGENT_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_RINSE_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_WASH_TIME_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_DELAY_START_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_TEMP_BUTTON_LED_GROUP, 1);
    handler_display_ledGroup_set(HANDLER_DISPLAY_WATER_LEVEL_BUTTON_LED_GROUP, 1);
}
