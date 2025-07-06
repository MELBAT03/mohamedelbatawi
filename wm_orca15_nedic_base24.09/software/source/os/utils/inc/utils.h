#ifndef __UTIL_H_
#define __UTIL_H_

#include "mcal.h"
#include "def.h"

#define MASK_SHIFT_BYTE(value, byte) ((((uint32_t)value) >> ((uint32_t)byte * 8)) & 0xff)

#define BIT_CTRL(reg, index, command) ((command) == (0) ? (BIT_CLR(reg, index)) : (BIT_SET(reg, index)))
#define BIT_SET(reg, index)           ((reg) |= (1 << (index)))
#define BIT_CLR(reg, index)           ((reg) &= (~(1 << (index))))
#define BIT_TOG(reg, index)           ((reg) ^= (1 << (index)))
#define BIT_GET(reg, index)           (((reg) >> (index)) & 0x01)
#define REG_CONFIG(reg, value)        ((reg) |= (value))
#define REG_SET(reg, value)           ((reg) = (value))
#define REG_GET(reg)                  (reg)

#define CONTROL_BIT(reg, index, command) ((command) == (0) ? (BIT_CLR(reg, index)) : (BIT_SET(reg, index)))

#define MOD8(dividend, divisor)  ((uint8_t)(dividend - (divisor * (uint8_t)(dividend / divisor))))
#define MOD16(dividend, divisor) ((uint16_t)(dividend - (divisor * (uint16_t)(dividend / divisor))))
#define MOD32(dividend, divisor) ((uint32_t)(dividend - (divisor * (uint32_t)(dividend / divisor))))

#define D2A(decDigit) ((decDigit) | 0x30)
#define A2D(ascDigit) ((ascDigit) & 0x0f)
#define H2A(hexDigit) (((hexDigit) > 0x09) ? ((hexDigit) + 0x37) : ((hexDigit) | 0x30))
#define A2H(ascDigit) (((ascDigit) > '9') ? ((ascDigit)-0x37) : ((ascDigit)-0x30))

#define MAX(num1, num2) (((num1) > (num2)) ? (num1) : (num2))
#define MIN(num1, num2) (((num1) < (num2)) ? (num1) : (num2))

#define SWAP(x, y) ((x) ^= (y) ^= (x) ^= (y))
#define ABS(x)     (((x) < 0) ? -(x) : (x))

#define UPDATE_COUNTER_CHECK(counter, interval) \
    do                                          \
    {                                           \
        counter += OS_TICK_PERIOD_MS;           \
        if ((counter) < (interval))             \
            return;                             \
        counter = 0;                            \
    } while (0)

void utils_ftoa(float x, char *a);
void utils_itoa(int32_t n, char s[]);
void utils_uitoa(uint32_t n, char s[]);
void utils_reverse(char *str);
int utils_strlen(char *s);
int32_t utils_atoi(char s[]);
void utils_itoahex(uint8_t x, char *a);
uint8_t utils_dataMatch_check(uint8_t *data, uint8_t *ref, uint16_t size);
void utils_sw_delay(unsigned long x);
uint8_t utils_dToPbcd(uint8_t val);
uint8_t utils_PbcdToDec(uint8_t val);
void utils_arrayToUint32(uint8_t *array, uint32_t *bytes);
void utils_arrayToFloat(uint8_t *array, float *bytes);
uint8_t utils_convergance_check(int32_t value1, int32_t value2, int32_t error);
uint8_t utils_convergenceValidity_check(uint32_t value, uint32_t threshold, uint32_t range);
uint8_t utils_thresholdValidity_check(uint32_t value, uint32_t highThreshold, uint32_t lowThreshold);
int8_t utils_binarySearch(uint32_t arr[], uint16_t start, uint16_t end, uint32_t element);


unsigned int getAbs(int n);
unsigned char numtostr(unsigned char str[],unsigned char start_index, unsigned char end_type, int num );
#endif
