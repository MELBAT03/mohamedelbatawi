#ifndef APP_DISPLAY_H
#define APP_DISPLAY_H

#include "def.h"

#include "app_user_interface.h"

#include "handlers/output/inc/handler_display.h"
#include "ssd/ssd.h"

#define SSD_CENTER_DASH       SSD_G_SEG_SYMBOL
#define ONE_OR_TWO_DIGIT(NUM) ((NUM / 10) % 10)

typedef struct app_display_ledBlink
{
    uint32_t timeInState;
    handler_display_led_state_t status;
    uint16_t ledID;
} app_display_ledBlink_t;

void app_display_update(uint32_t period);

void app_display_dashSymbols_put(void);
void app_display_ssdSymbols_put(ssd_symbolEnum_t ssd1, ssd_symbolEnum_t ssd2, ssd_symbolEnum_t ssd3, ssd_symbolEnum_t ssd4);
void app_display_remainingTime_put(uint32_t period, uint32_t reminingTime, uint8_t dpState);
void app_display_ssd_charBlink(uint32_t period, uint8_t *buf, uint8_t bufCount, uint8_t blinkingNumber, uint16_t onTimeMS, uint16_t OffTimeMS);
void app_display_ssd_blink(uint32_t period, uint16_t Disp_number, uint8_t blinkingNumber, uint16_t OnTimeMS, uint16_t OffTimeMS, handler_display_ssdFormat_t FormatSel);
void app_display_ssd_put(uint16_t Disp_number, handler_display_ssdFormat_t FormatSel);
void app_display_led_blink(app_display_ledBlink_t *led, uint32_t period, uint16_t OnTimeMS, uint16_t OffTimeMS, uint32_t value);
void app_display_weightDetectionPattern_put(uint32_t period);

void app_display_fire(void);
void app_display_clear(void);

#endif
