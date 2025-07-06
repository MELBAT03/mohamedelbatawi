#include "def.h"
#include "os.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "components/uart/uart.h"
#include "component_config.h"

typedef struct
{
    uint16_t startAddr;
    uint16_t size;
    os_logID_t id;
    uint8_t : 8;
} os_log_t;

// uint8_t gu8_os_log_dispBuf[OS_LOG_DISPLAY_SIZE] = {0};
#ifdef OS_LOG_QUEUE_ENABLE
static uint8_t gu8_os_log_mainBuffer[OS_LOG_MAIN_BUFFER_SIZE] = {0};
static os_log_t gx_os_log_internalHandler[OS_LOG_USER_LOGS_NUM];
static uint8_t gu8_os_log_internalCounter = 0;
#endif
uint8_t gu8_os_log_groupID = 0;
uint8_t gu8_os_log_ccCounter = 0;
uint8_t gu8_os_log_icCounter = 0;

#define FILL_0       0x01
#define FILL_LEFT    0x02
#define POINTOR      0x04
#define ALTERNATE    0x08
#define OUPUT_INT    0x10
#define START        0x20
#define VINT_STR_MAX 10

typedef union _val_cache
{
    uint8_t val8;
    int32_t val32;
    uint32_t val32u;
    const char *valcp;
} val_cache_t;

typedef struct _val_attr
{
    int8_t type;
    uint8_t state;
    uint8_t fillbytes;
    uint8_t precision;
    val_cache_t value;
} val_attr_t;

#define isdigit(_c)        ((_c <= '9') && (_c >= '0'))
#define fill_num(_attr)    ((attr)->fillbytes)
#define isfill_0(_attr)    (fill_num(_attr) && ((_attr)->state & FILL_0))
#define isfill_left(_attr) (fill_num(_attr) && ((_attr)->state & FILL_LEFT))
#define isstart(_attr)     ((_attr)->state & START)

static int os_log_putc(uint8_t *buff, int c, int8_t printable);
static inline void os_log_printf_ch_mutlti(uint8_t **buff, uint32_t c, uint32_t len, int8_t printable);
static inline void os_log_printf_buf(uint8_t **buff, const char *s, uint32_t len, int8_t printable);
static int os_log_printf_str(uint8_t **buff, const val_attr_t *const attr, int8_t printable);
static int os_log_printf_int(uint8_t **buff, val_attr_t *const attr, uint8_t hex, int8_t printable);
static int os_log_vprintf(uint8_t *buff, const char *fmt, va_list va, int8_t printable);

int8_t os_log_add(os_logID_t logID, uint16_t size)
{
#ifdef OS_LOG_QUEUE_ENABLE
    uint16_t i = 0;
    uint16_t start, end;

    if (gu8_os_log_internalCounter == 0)
    {
        if (size > OS_LOG_MAIN_BUFFER_SIZE)
        {
            return -1;
        }

        gx_os_log_internalHandler[gu8_os_log_internalCounter].id = logID;
        gx_os_log_internalHandler[gu8_os_log_internalCounter].startAddr = 0;
        gx_os_log_internalHandler[gu8_os_log_internalCounter].size = size;
    }
    else
    {
        if (size + gx_os_log_internalHandler[gu8_os_log_internalCounter - 1].startAddr + gx_os_log_internalHandler[gu8_os_log_internalCounter - 1].size > OS_LOG_MAIN_BUFFER_SIZE)
        {
            return -1;
        }

        gx_os_log_internalHandler[gu8_os_log_internalCounter].id = logID;
        gx_os_log_internalHandler[gu8_os_log_internalCounter].startAddr = gx_os_log_internalHandler[gu8_os_log_internalCounter - 1].startAddr + gx_os_log_internalHandler[gu8_os_log_internalCounter - 1].size;
        gx_os_log_internalHandler[gu8_os_log_internalCounter].size = size;
    }

    start = gx_os_log_internalHandler[gu8_os_log_internalCounter].startAddr;
    end = start + gx_os_log_internalHandler[gu8_os_log_internalCounter].size;

    for (i = start; i < end; i++)
    {
        gu8_os_log_mainBuffer[i] = 0;
    }

    gu8_os_log_internalCounter++;
#endif

    return 0;
}

int8_t os_log_write(os_logID_t logID, uint8_t *data, uint8_t size, uint8_t clearTrailing)
{
#ifdef OS_LOG_QUEUE_ENABLE
    for (uint16_t i = 0; i < gu8_os_log_internalCounter; i++)
    {
        if (logID == gx_os_log_internalHandler[i].id)
        {
            if (size > gx_os_log_internalHandler[i].size)
            {
                return -1;
            }
            for (uint16_t j = 0; j < size; j++)
            {
                gu8_os_log_mainBuffer[j] = data[j];
            }
            if (clearTrailing == 1)
            {
                for (; size < gx_os_log_internalHandler[i].size; size++)
                {
                    gu8_os_log_mainBuffer[size] = 0;
                }
            }
            return 0;
        }
    }
#endif

    return -1;
}

int8_t os_log_read(os_logID_t logID, uint8_t *data, uint8_t size)
{
#ifdef OS_LOG_QUEUE_ENABLE
    for (uint16_t i = 0; i < gu8_os_log_internalCounter; i++)
    {
        if (logID == gx_os_log_internalHandler[i].id)
        {
            if (size > gx_os_log_internalHandler[i].size)
            {
                return -1;
            }
            for (uint16_t j = 0; j < size; j++)
            {
                data[j] = gu8_os_log_mainBuffer[j];
            }
            for (; size < gx_os_log_internalHandler[i].size; size++)
            {
                data[size] = 0;
            }
            return 0;
        }
    }
#endif

    return -1;
}

void os_log_groupID_set(uint8_t value)
{
    gu8_os_log_groupID = value;
}

static int os_log_putc(uint8_t *buff, int c, int8_t printable)
{
    if (printable == 1)
    {
        uart_byte_put((uart_t)LOG_CHANNEL_ID, c);
    }

    if (buff != NULL)
        *buff = c;
    return c;
}

/*
 * Using "os_log_vprintf" costs stack without alignment and accuracy:
 *                      just "fmt": 136 Bytes
 *                            "%s": 172 Bytes
 *      "%p", "%d, "%i, "%u", "%x": 215 Bytes
 */
int os_log_printf(const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = os_log_vprintf(NULL, fmt, ap, 1);
    va_end(ap);

    return ret;
}

int os_log_sprintf(uint8_t *buff, const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = os_log_vprintf(buff, fmt, ap, -1);
    va_end(ap);

    return ret;
}

static inline void os_log_printf_ch_mutlti(uint8_t **buff, uint32_t c, uint32_t len, int8_t printable)
{
    while (len--)
    {
        if (*buff != NULL)
        {
            os_log_putc(*buff, c, printable);
            (*buff)++;
        }
        else
        {
            os_log_putc(NULL, c, printable);
        }
    }
}

static inline void os_log_printf_buf(uint8_t **buff, const char *s, uint32_t len, int8_t printable)
{
    while (len--)
    {
        if (*buff != NULL)
        {
            os_log_putc(*buff, *s++, printable);
            (*buff)++;
        }
        else
        {
            os_log_putc(NULL, *s++, printable);
        }
    }
}

static int os_log_printf_str(uint8_t **buff, const val_attr_t *const attr, int8_t printable)
{
    const char *s = attr->value.valcp;
    s = s == NULL ? "<null>" : s;

    if (fill_num(attr))
    {
        unsigned char left;
        unsigned char len;

        while (*s != '\0')
            s++;
        len = s - attr->value.valcp;
        left = fill_num(attr) > len ? fill_num(attr) - len : 0;

        if (!isfill_left(attr))
        {
            os_log_printf_ch_mutlti(buff, ' ', left, printable);
        }

        os_log_printf_buf(buff, attr->value.valcp, len, printable);

        if (isfill_left(attr))
        {
            os_log_printf_ch_mutlti(buff, ' ', left, printable);
        }
    }
    else
    {
        while (*s != '\0')
        {
            if (*buff != NULL)
            {
                os_log_putc(*buff, *s++, printable);
                (*buff)++;
            }
            else
            {
                os_log_putc(NULL, *s++, printable);
            }
        }
    }

    return 0;
}

static int os_log_printf_int(uint8_t **buff, val_attr_t *const attr, uint8_t hex, int8_t printable)
{
    char buf[VINT_STR_MAX];
    unsigned char offset = VINT_STR_MAX;

    if (attr->value.val32u != 0)
    {
        for (; attr->value.val32u > 0; attr->value.val32u /= hex)
        {
            unsigned char c = attr->value.val32u % hex;
            if (c < 10)
                buf[--offset] = c + '0';
            else
                buf[--offset] = c + 'a' - 10;
        }
    }
    else
        buf[--offset] = '0';

    if (fill_num(attr))
    {
        char fill_data = isfill_0(attr) ? '0' : ' ';
        unsigned char len = fill_num(attr) - (VINT_STR_MAX - offset);
        unsigned char left = fill_num(attr) > (VINT_STR_MAX - offset) ? len : 0;

        if (!isfill_left(attr))
        {
            os_log_printf_ch_mutlti(buff, fill_data, left, printable);
        }

        os_log_printf_buf(buff, &buf[offset], VINT_STR_MAX - offset, printable);

        if (isfill_left(attr))
        {
            fill_data = ' ';
            os_log_printf_ch_mutlti(buff, fill_data, left, printable);
        }
    }
    else
    {
        os_log_printf_buf(buff, &buf[offset], VINT_STR_MAX - offset, printable);
    }

    return 0;
}

static int os_log_vprintf(uint8_t *buff, const char *fmt, va_list va, int8_t printable)
{
    for (;;)
    {
        const char *ps = fmt;
        val_attr_t attr;

        while (*ps != '\0' && *ps != '%')
        {
            if (buff != NULL)
            {
                os_log_putc(buff++, *ps++, printable);
            }
            else
            {
                os_log_putc(NULL, *ps++, printable);
            }
        }

        if (*ps == '\0')
        {
            *buff = 0;
            break;
        }

        fmt = ps;

        attr.state = 0;
        attr.type = -1;
        attr.fillbytes = 0;
        attr.precision = 0;

        for (;;)
        {
            switch (*++ps)
            {
                case 'd':
                case 'i':
                case 'u':
                case 'x':
                case 'c':
                case 's':
                case 'p':
                case '\0':
                    attr.type = *ps++;
                    break;
                case '#':
                    attr.state |= ALTERNATE;
                    ps++;
                    break;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    if ((!isstart(&attr) || *(ps - 1) == '-') && *ps == '0')
                    {
                        attr.state |= FILL_0;
                    }
                    else
                    {
                        if (attr.state & POINTOR)
                            attr.precision = attr.precision * 10 + *ps - '0';
                        else
                            attr.fillbytes = attr.fillbytes * 10 + *ps - '0';
                    }
                    break;
                case '.':
                    attr.state |= POINTOR;
                    break;
                case '-':
                    attr.state |= FILL_LEFT;
                    break;
                default:
                    attr.type = -2;
                    break;
            }

            attr.state |= START;

            if (attr.type != -1)
                break;
        }

        switch (attr.type)
        {
            case 'c':
                attr.value.val8 = (char)va_arg(va, int);
                if (buff != NULL)
                {
                    os_log_putc(buff++, attr.value.val8, printable);
                }
                else
                {
                    os_log_putc(NULL, attr.value.val8, printable);
                }

                break;
            case 's':
                attr.value.valcp = va_arg(va, const char *);
                os_log_printf_str(&buff, &attr, printable);
                break;
            case 'i':
            case 'd':
                attr.value.val32 = va_arg(va, int);
                if (attr.value.val32 < 0)
                {
                    if (buff != NULL)
                    {
                        os_log_putc(buff++, '-', printable);
                    }
                    else
                    {
                        os_log_putc(NULL, '-', printable);
                    }
                    attr.value.val32 = -attr.value.val32;
                }
                os_log_printf_int(&buff, &attr, 10, printable);
                break;
            case 'u':
                attr.value.val32u = va_arg(va, unsigned int);
                os_log_printf_int(&buff, &attr, 10, printable);
                break;
            case 'x':
                attr.value.val32u = va_arg(va, unsigned int);
                os_log_printf_int(&buff, &attr, 16, printable);
                break;
            case 'p':
                os_log_printf_buf(&buff, "0x", 2, printable);
                attr.value.valcp = va_arg(va, const char *);
                os_log_printf_int(&buff, &attr, 16, printable);
            default:
                break;
        }

        fmt = ps;
    }

    return 0;
}
