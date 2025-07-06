/*
 * handler_temp_sensor.c
 *
 *  Created on: Dec 17, 2023
 *      Author: EKHAIR01
 */

#include "board.h"
#include "handler_temp_sensor.h"
#include "ntc_temp_sensor.h"

#include "handler_heater.h"

#define INPUT_AIR_TEMP_SIZE                  150U
#define INPUT_AIR_TEMP_FAULT_LOWER_THRESHOLD 10U
#define INPUT_AIR_TEMP_FAULT_UPPER_THRESHOLD 4096U
#define INPUT_AIR_TEMP_STEP                  1U

#define AMBIENT_TEMP_SIZE                  70U
#define AMBIENT_TEMP_FAULT_LOWER_THRESHOLD 500U
#define AMBIENT_TEMP_FAULT_UPPER_THRESHOLD 4096U
#define AMBIENT_TEMP_STEP                  1U

#define WATER_TEMP_SIZE                  100U
#define WATER_TEMP_FAULT_LOWER_THRESHOLD 500U
#define WATER_TEMP_FAULT_UPPER_THRESHOLD 4096U
#define WATER_TEMP_STEP                  1U

#if 0
static const uint16_t airSensorTempLookupTable[] = {
    111,
    117,
    123,
    129,
    136,
    143,
    150,
    158,
    165,
    173,
    182,
    191,
    200,
    209,
    219,
    229,
    239,
    250,
    262,
    273,
    285,
    298,
    310,
    324,
    337,
    351,
    366,
    381,
    396,
    412,
    428,
    445,
    462,
    480,
    498,
    517,
    536,
    555,
    575,
    596,
    616,
    638,
    660,
    682,
    704,
    728,
    751,
    775,
    800,
    825,
    850,
    875,
    902,
    928,
    955,
    982,
    1010,
    1038,
    1066,
    1094,
    1123,
    1152,
    1182,
    1212,
    1242,
    1272,
    1302,
    1333,
    1364,
    1394,
    1426,
    1457,
    1488,
    1520,
    1551,
    1583,
    1615,
    1646,
    1678,
    1710,
    1741,
    1773,
    1805,
    1836,
    1868,
    1899,
    1931,
    1962,
    1993,
    2024,
    2055,
    2085,
    2116,
    2146,
    2176,
    2205,
    2235,
    2264,
    2293,
    2322,
    2351,
    2379,
    2407,
    2435,
    2462,
    2489,
    2516,
    2543,
    2569,
    2595,
    2620,
    2645,
    2670,
    2695,
    2719,
    2743,
    2767,
    2790,
    2813,
    2835,
    2858,
    2880,
    2901,
    2922,
    2943,
    2964,
    2984,
    3004,
    3024,
    3043,
    3062,
    3080,
    3099,
    3117,
    3134,
    3152,
    3169,
    3186,
    3202,
    3218,
    3234,
    3250,
    3265,
    3280,
    3295,
    3309,
    3324,
    3338,
    3351,
    3365,
    3378};
static const uint16_t ambientSensorTempLookupTable[] = {
    1095,
    1137,
    1180,
    1224,
    1268,
    1313,
    1358,
    1404,
    1450,
    1497,
    1543,
    1590,
    1638,
    1685,
    1732,
    1780,
    1827,
    1874,
    1921,
    1968,
    2015,
    2061,
    2108,
    2153,
    2199,
    2243,
    2288,
    2332,
    2375,
    2418,
    2460,
    2501,
    2542,
    2582,
    2622,
    2660,
    2698,
    2736,
    2772,
    2808,
    2843,
    2877,
    2911,
    2944,
    2976,
    3007,
    3037,
    3067,
    3096,
    3124,
    3152,
    3179,
    3205,
    3230,
    3255,
    3279,
    3302,
    3325,
    3347,
    3369,
    3390,
    3410,
    3430,
    3449,
    3467,
    3485,
    3503,
    3520,
    3536,
    3552,
    3568};
#endif
static const uint16_t waterSensorTempLookupTable[] = {
    527,
    552,
    578,
    604,
    632,
    660,
    688,
    718,
    748,
    779,
    811,
    844,
    877,
    911,
    946,
    981,
    1017,
    1054,
    1091,
    1129,
    1167,
    1206,
    1245,
    1285,
    1325,
    1366,
    1407,
    1448,
    1489,
    1531,
    1572,
    1614,
    1656,
    1698,
    1741,
    1783,
    1825,
    1867,
    1909,
    1950,
    1992,
    2033,
    2074,
    2115,
    2155,
    2196,
    2235,
    2275,
    2314,
    2352,
    2390,
    2428,
    2465,
    2502,
    2538,
    2573,
    2608,
    2643,
    2677,
    2710,
    2743,
    2775,
    2806,
    2837,
    2867,
    2897,
    2926,
    2955,
    2983,
    3010,
    3037,
    3063,
    3089,
    3114,
    3138,
    3162,
    3185,
    3208,
    3230,
    3252,
    3273,
    3294,
    3314,
    3334,
    3353,
    3372,
    3390,
    3408,
    3425,
    3442,
    3458,
    3474,
    3490,
    3505,
    3520,
    3534,
    3548,
    3562,
    3575,
    3588,
    3600};

static ntcSensor_handle waterSensor;

void handler_tempSensor_init(void)
{
    ntc_config_t ntcConfig;

    ntcConfig.adcLookupTableRef = waterSensorTempLookupTable;
    ntcConfig.lookupTableSize = WATER_TEMP_SIZE;
    ntcConfig.tempStep = WATER_TEMP_STEP;
    ntcConfig.adcFaultUpperThreshold = WATER_TEMP_FAULT_UPPER_THRESHOLD;
    ntcConfig.adcFaultLowerThreshold = WATER_TEMP_FAULT_LOWER_THRESHOLD;
    ntcConfig.ainput.gpio.pin = WATER_TEMP_SENSOR_PIN;
    ntcConfig.ainput.gpio.port = WATER_TEMP_SENSOR_PORT;
    ntcConfig.ainput.adcBlock = WATER_TEMP_SENSOR_ADC_BLK;
    ntcConfig.ainput.adcCh = WATER_TEMP_SENSOR_ADC_CH;
    waterSensor = ntc_init(&ntcConfig);
}

uint16_t handler_waterTempSensor_temp_get(void)
{
    return ntc_temp_get(waterSensor);
}
uint16_t handler_ambientTempSensor_temp_get(void)
{
    return 30;
}
uint32_t handler_heaterTempSensor_temp_get(void)
{
    return (uint32_t)handler_heater_feedback_get();
}
uint16_t handler_airBTempSensor_temp_get(void)
{
    return 30;
}

uint8_t handler_waterTempSensor_error_get(void)
{
    return ntc_error_get(waterSensor);
}
uint8_t handler_ambientTempSensor_error_get(void)
{
    return 0;
}
uint8_t handler_airATempSensor_error_get(void)
{
    return 0;
}
uint8_t handler_airBTempSensor_error_get(void)
{
    return 0;
}
