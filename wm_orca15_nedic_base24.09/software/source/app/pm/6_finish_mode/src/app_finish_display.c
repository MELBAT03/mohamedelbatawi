#include "def.h"

#include "app_finish_display.h"
#include "app_display.h"

#include "handlers/output/inc/handler_lock_motor.h"

static void display_finish_handle(void);

void app_finish_display_process(uint32_t period)
{
    display_finish_handle();
}

static void display_finish_handle(void)
{
    handler_display_led_clear();

    handler_display_ssd_set(SSD_DIGIT_0_ID, SSD_NULL_SYMBOL);
    handler_display_ssd_set(SSD_DIGIT_1_ID, SSD_NULL_SYMBOL);
    handler_display_ssd_set(SSD_DIGIT_2_ID, SSD_NULL_SYMBOL);
    handler_display_ssd_set(SSD_DIGIT_3_ID, SSD_NULL_SYMBOL);

    handler_display_ledGroup_set(HANDLER_DISPLAY_LID_LOCK_LED_GROUP, handler_lockMotor_state_get());

    handler_display_ssdDP_set(SSD_DP_OFF);

    handler_display_ledGroup_set(HANDLER_DISPLAY_ON_OFF_LED_GROUP, 1);

    handler_display_dimState_set(0);
    handler_display_end();
}
