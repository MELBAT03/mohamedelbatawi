#include "components/stwi/stwi.h"
#include "components/rtc/rtc.h"
#include <time.h>

#define UNIX_OFFSET 946684800

// #define RTC_DS3231
#define RTC_DS1307

#ifdef RTC_DS3231

#define RTC_ADDR       0b11010000
#define RTC_WRITE_ADDR 0xD0u
#define RTC_READ_ADDR  0xD1u
#define RTC_REG_PTR_ADDR
#define RTC_SECONDS_ADDR      0x00
#define RTC_MINUTES_ADDR      0x01
#define RTC_HOURS_ADDR        0x02
#define RTC_DAY_OF_WEEK_ADDR  0x03
#define RTC_DAY_OF_MONTH_ADDR 0x04
#define RTC_MONTH_ADDR        0x05
#define RTC_YEAR_ADDR         0x06
#define RTC_CTRL_REG_ADDR     0x0E

#elif defined(RTC_DS1307)

#define RTC_ADDR       0b11010000
#define RTC_WRITE_ADDR 0xD0u
#define RTC_READ_ADDR  0xD1u
#define RTC_REG_PTR_ADDR
#define RTC_SECONDS_ADDR      0x00
#define RTC_MINUTES_ADDR      0x01
#define RTC_HOURS_ADDR        0x02
#define RTC_DAY_OF_WEEK_ADDR  0x03
#define RTC_DAY_OF_MONTH_ADDR 0x04
#define RTC_MONTH_ADDR        0x05
#define RTC_YEAR_ADDR         0x06
#define RTC_CTRL_REG_ADDR     0x07

#endif

void rtc_init(twi_pinConfig_t *pins)
{
    twi_pinConfig_t pinConfig;

    pinConfig.dataPort = pins->dataPort;
    pinConfig.dataPin = pins->dataPin;
    pinConfig.clockPort = pins->clockPort;
    pinConfig.clockPin = pins->clockPin;

    twi_init(&pinConfig, 100000);

#ifdef RTC_DS1307
    twi_start();
    twi_write(TWI_WRITE_SET(RTC_ADDR));
    twi_write(RTC_CTRL_REG_ADDR);
    twi_write(0x00);
    twi_stop();
#endif
}

// set time
//  time is provided as : (10, 30, 00) --> must be converted to packed bcd or (0x10, 0x30, 0x00) --> can be used as packed bcd
//  set date, year 0 : 99
void rtc_time_set(rtc_t *time)
{
    time->hour &= ~(1 << 6);     // set to 24-hour mode
    time->second &= ~(1 << 7);   // enable the osc in ds1307
    // time->year &= 99;

    twi_start();
    twi_write(TWI_WRITE_SET(RTC_ADDR));
    twi_write(RTC_SECONDS_ADDR);
    twi_write(util_dToPbcd(time->second));
    twi_write(util_dToPbcd(time->minute));
    twi_write(util_dToPbcd(time->hour));
    twi_write(util_dToPbcd(time->dayOfWeek));
    twi_write(util_dToPbcd(time->dayOfMonth));
    twi_write(util_dToPbcd(time->month));
    // :NOTE: year is represented as "year - 1900" to get to 2019 we need to add "119 + 1900" thus the year value cant be stored as a packed bcd value. so set it as a bin value and retrieve it as a bin value
    twi_write(time->year);
    twi_stop();
}

// get time & date
void rtc_time_get(rtc_t *time)
{
    twi_start();
    twi_write(TWI_WRITE_SET(RTC_ADDR));
    twi_write(RTC_SECONDS_ADDR);
    twi_stop();

    twi_start();
    twi_write(TWI_READ_SET(RTC_ADDR));
    time->second = util_PbcdToDec(twi_read(TWI_ACK_SET) & 0x7f);
    time->minute = util_PbcdToDec(twi_read(TWI_ACK_SET) & 0x7f);
    time->hour = util_PbcdToDec(twi_read(TWI_ACK_SET) & 0x3f);
    time->dayOfWeek = util_PbcdToDec(twi_read(TWI_ACK_SET) & 0x07);
    time->dayOfMonth = util_PbcdToDec(twi_read(TWI_ACK_SET) & 0x3f);
    time->month = util_PbcdToDec(twi_read(TWI_ACK_SET) & 0x1f);
    time->year = twi_read(TWI_ACK_CLR);
    twi_stop();
}

void rtc_epochToHuman(rtc_t *tp, time_t time)
{
    long day, mins, secs, year, leap;
    day = time / (24L * 60 * 60);
    secs = time % (24L * 60 * 60);
    mins = secs / 60;
    year = (((day * 4) + 2) / 1461);

    tp->second = secs % 60;
    tp->hour = mins / 60;
    tp->minute = mins % 60;
    tp->dayOfWeek = (day + 4) % 7;
    tp->year = year + 70;
    leap = !(tp->year & 3);
    day -= ((year * 1461) + 1) / 4;
    // tp->tm_yday = day;
    day += (day > 58 + leap) ? ((leap) ? 1 : 2) : 0;
    tp->month = ((day * 12) + 6) / 367;
    tp->dayOfMonth = day + 1 - ((tp->month * 367) + 5) / 12;
    // tp->tm_isdst = 0;
}

void rtc_humanToEpoch(rtc_t *tp, time_t *time)
{
    struct tm t;
    t.tm_year = tp->year - 1900;
    t.tm_mon = tp->month - 1;
    t.tm_mday = tp->dayOfMonth;
    t.tm_wday = tp->dayOfWeek - 1;
    t.tm_hour = tp->hour;
    t.tm_min = tp->minute;
    t.tm_sec = tp->second;
    t.tm_isdst = 0;
    *time = mktime(&t) + UNIX_OFFSET;
}