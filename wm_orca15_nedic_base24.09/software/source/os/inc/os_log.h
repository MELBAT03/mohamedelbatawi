#ifndef OS_LOG_H
#define OS_LOG_H

// :TODO: add append functionality
// :TODO: add log display levels

#include "components/uart/uart.h"
#include "def.h"
#include "os_tick.h"
#include <stdarg.h>
#include <stdio.h>
#include "component_config.h"

#define OS_LOG_TRAILING_KEEP  (0)
#define OS_LOG_TRAILING_RESET (1)

#define COLOR_NONE    0
#define COLOR_ERR     31
#define COLOR_WRN     33
#define CLR_INF       32
#define CLR_MIN       36
#define COLOR_DEBUG   0
#define COLOR_VERBOSE 0

#define CLR_HD        "\033[0;%dm"
#define LOG_BOLD_HEAD "\033[1;%dm"
#define CLR_E         "\033[0m"

extern uint8_t gu8_os_log_groupID;
extern uint8_t gu8_os_log_ccCounter;
extern uint8_t gu8_os_log_icCounter;
// extern uint8_t gu8_os_log_dispBuf[];
typedef uint8_t os_logID_t;

#ifdef LOG_ENABLE
#define printf(format, ...) os_log_printf(format, ##__VA_ARGS__)
#else
#define printf(format, ...)
#endif

extern int os_log_sprintf(uint8_t *buff, const char *fmt, ...);
extern int os_log_printf(const char *fmt, ...);

#ifdef LOG_ENABLE
#define LOG(groupID, level, format, ...)                                                                                \
    do                                                                                                                  \
    {                                                                                                                   \
        if ((char)(level) == (char)'i')                                                                                 \
        {                                                                                                               \
            printf("\n\r" CLR_HD "[%c] [(%7d) %s - %5u]:  ", CLR_INF, 'I', gu32_os_tick_timestamp, __func__, __LINE__); \
            printf((format), ##__VA_ARGS__);                                                                            \
            printf(CLR_E);                                                                                              \
        }                                                                                                               \
        else if ((char)(level) == (char)'e')                                                                            \
        {                                                                                                               \
            printf("\n\r" CLR_HD "[%c] - [%s - %-4u] : ", COLOR_ERR, 'E', __func__, __LINE__);                          \
            printf((format), ##__VA_ARGS__);                                                                            \
            printf(CLR_E);                                                                                              \
        }                                                                                                               \
        else if ((char)(level) == (char)'w')                                                                            \
        {                                                                                                               \
            printf("\n\r" CLR_HD "[%c] - [%s - %-4u] : ", COLOR_WRN, 'W', __func__, __LINE__);                          \
            printf((format), ##__VA_ARGS__);                                                                            \
            printf(CLR_E);                                                                                              \
        }                                                                                                               \
        else if (((uint8_t)(groupID) == 'V') && ((uint8_t)(groupID) == gu8_os_log_groupID) &&                           \
                 (uart_dataSent_check(LOG_CHANNEL_ID) == (int8_t)1))                                                    \
        {                                                                                                               \
            printf((format), ##__VA_ARGS__);                                                                            \
        }                                                                                                               \
        else if (((uint8_t)(groupID) == gu8_os_log_groupID) && (uart_dataSent_check(LOG_CHANNEL_ID) == (int8_t)1))      \
        {                                                                                                               \
            printf("\n\r" CLR_HD "[%7d]: ", CLR_MIN, gu32_os_tick_timestamp);                                           \
            printf((format), ##__VA_ARGS__);                                                                            \
            printf(CLR_E);                                                                                              \
        }                                                                                                               \
        else                                                                                                            \
        {                                                                                                               \
            /* do nothing */                                                                                            \
        }                                                                                                               \
    } while (0)

#define LOG_CB(groupID, level, buffer, count)                                                                           \
    do                                                                                                                  \
    {                                                                                                                   \
        if ((char)(level) == 'i')                                                                                       \
        {                                                                                                               \
            printf("\n\r" CLR_HD "[%c] [(%7d) %s - %5u]:  ", CLR_INF, 'I', gu32_os_tick_timestamp, __func__, __LINE__); \
            for (gu8_os_log_ccCounter = 0; gu8_os_log_ccCounter < (count); gu8_os_log_ccCounter++)                      \
            {                                                                                                           \
                printf("%x ", (buffer[gu8_os_log_ccCounter]));                                                          \
            }                                                                                                           \
            printf(CLR_E);                                                                                              \
        }                                                                                                               \
        else if ((char)(level) == 'e')                                                                                  \
        {                                                                                                               \
            printf("\n\r" CLR_HD "[%c] - [%s - %-4u] : ", COLOR_ERR, 'E', __func__, __LINE__);                          \
            for (gu8_os_log_ccCounter = 0; gu8_os_log_ccCounter < (count); gu8_os_log_ccCounter++)                      \
            {                                                                                                           \
                printf("%x ", (buffer[gu8_os_log_ccCounter]));                                                          \
            }                                                                                                           \
            printf(CLR_E);                                                                                              \
        }                                                                                                               \
        else if ((char)(level) == 'w')                                                                                  \
        {                                                                                                               \
            printf("\n\r" CLR_HD "[%c] - [%s - %-4u] : ", COLOR_WRN, 'W', __func__, __LINE__);                          \
            for (gu8_os_log_ccCounter = 0; gu8_os_log_ccCounter < (count); gu8_os_log_ccCounter++)                      \
            {                                                                                                           \
                printf("%x ", (buffer[gu8_os_log_ccCounter]));                                                          \
            }                                                                                                           \
            printf(CLR_E);                                                                                              \
        }                                                                                                               \
        else if ((groupID) == gu8_os_log_groupID && uart_dataSent_check(LOG_CHANNEL_ID) == 1)                           \
        {                                                                                                               \
            printf("\n\r" CLR_HD "[CC] ", CLR_MIN);                                                                     \
            for (gu8_os_log_ccCounter = 0; gu8_os_log_ccCounter < (count); gu8_os_log_ccCounter++)                      \
            {                                                                                                           \
                printf("%x ", (buffer[gu8_os_log_ccCounter]));                                                          \
            }                                                                                                           \
            printf(CLR_E);                                                                                              \
        }                                                                                                               \
        else                                                                                                            \
        {                                                                                                               \
            /* do nothing */                                                                                            \
        }                                                                                                               \
    } while (0)

#define LOG_CC(groupID, level, buffer, count)                                                                           \
    do                                                                                                                  \
    {                                                                                                                   \
        if ((char)(level) == 'i')                                                                                       \
        {                                                                                                               \
            printf("\n\r" CLR_HD "[%c] [(%7d) %s - %5u]:  ", CLR_INF, 'I', gu32_os_tick_timestamp, __func__, __LINE__); \
            for (gu8_os_log_ccCounter = 0; gu8_os_log_ccCounter < (count); gu8_os_log_ccCounter++)                      \
            {                                                                                                           \
                printf("%c ", (buffer[gu8_os_log_ccCounter]));                                                          \
            }                                                                                                           \
            printf(CLR_E);                                                                                              \
        }                                                                                                               \
        else if ((char)(level) == 'e')                                                                                  \
        {                                                                                                               \
            printf("\n\r" CLR_HD "[%c] - [%s - %-4u] : ", COLOR_ERR, 'E', __func__, __LINE__);                          \
            for (gu8_os_log_ccCounter = 0; gu8_os_log_ccCounter < (count); gu8_os_log_ccCounter++)                      \
            {                                                                                                           \
                printf("%c ", (buffer[gu8_os_log_ccCounter]));                                                          \
            }                                                                                                           \
            printf(CLR_E);                                                                                              \
        }                                                                                                               \
        else if ((char)(level) == 'w')                                                                                  \
        {                                                                                                               \
            printf("\n\r" CLR_HD "[%c] - [%s - %-4u] : ", COLOR_WRN, 'W', __func__, __LINE__);                          \
            for (gu8_os_log_ccCounter = 0; gu8_os_log_ccCounter < (count); gu8_os_log_ccCounter++)                      \
            {                                                                                                           \
                printf("%c ", (buffer[gu8_os_log_ccCounter]));                                                          \
            }                                                                                                           \
            printf(CLR_E);                                                                                              \
        }                                                                                                               \
        else if ((groupID) == gu8_os_log_groupID && uart_dataSent_check(LOG_CHANNEL_ID) == 1)                           \
        {                                                                                                               \
            printf("\n\r" CLR_HD "[CC] ", CLR_MIN);                                                                     \
            for (gu8_os_log_ccCounter = 0; gu8_os_log_ccCounter < (count); gu8_os_log_ccCounter++)                      \
            {                                                                                                           \
                printf("%c ", (buffer[gu8_os_log_ccCounter]));                                                          \
            }                                                                                                           \
            printf(CLR_E);                                                                                              \
        }                                                                                                               \
        else                                                                                                            \
        {                                                                                                               \
            /* do nothing */                                                                                            \
        }                                                                                                               \
    } while (0)
#else
#define LOG(groupID, level, format, ...)
#define LOG_CB(groupID, level, buffer, count)
#define LOG_CC(groupID, level, buffer, count)
#endif

int8_t os_log_add(os_logID_t logID, uint16_t size);
int8_t os_log_write(os_logID_t logID, uint8_t *data, uint8_t size, uint8_t clearTrailing);
int8_t os_log_read(os_logID_t logID, uint8_t *data, uint8_t size);

void os_log_groupID_set(uint8_t value);

#endif
