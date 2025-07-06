#ifndef RTC_H
#define RTC_H

#include "components/stwi/stwi.h"
#include "time.h"

// :NOTE: the time and date should be set only once, otherwise each time the controller resets it will reset the time.

typedef struct
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t dayOfWeek;
    uint8_t dayOfMonth;
    uint8_t month;
    uint16_t year;
} rtc_t;

void rtc_init(twi_pinConfig_t *pins);
void rtc_time_set(rtc_t *time);
void rtc_time_get(rtc_t *time);
void rtc_epochToHuman(rtc_t *tp, time_t time);
void rtc_humanToEpoch(rtc_t *tp, time_t *time);

#endif