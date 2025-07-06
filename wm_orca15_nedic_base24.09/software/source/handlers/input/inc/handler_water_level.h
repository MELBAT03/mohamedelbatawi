#ifndef HANDLER_WATER_LEVEL_H
#define HANDLER_WATER_LEVEL_H 1

#include "def.h"

typedef enum handler_waterLevel_resetCheckType
{
    WATER_LEVEL_RESET_NORMAL_CHECK,
    WATER_LEVEL_RESET_ERROR_CHECK,
    WATER_LEVEL_RESET_SPIN_ISSUE_CHECK,
    WATER_LEVEL_RESET_TEST_MODE_CHECK,
    WATER_LEVEL_RESET_CALIBRATION_CHECK,
    WATER_LEVEL_LEAKAGE_CHECK
} handler_waterLevel_resetCheckType_t;

#define HANDLER_WATER_LEVEL_OUT_OF_RANGE_THRESHOLD                   (0)
#define HANDLER_WATER_LEVEL_DEFAULT_UPPER_THRESHOLD                  (28000)
#define HANDLER_WATER_LEVEL_DEFAULT_LOWER_THRESHOLD                  (20800)
#define HANDLER_WATER_LEVEL_OVERFLOW_LOWER_THRESHOLD_OFFSET          (5000)
#define HANDLER_WATER_LEVEL_OVERFLOW_UPPER_THRESHOLD_OFFSET          (5100)
#define HANDLER_WATER_LEVEL_DANGEROUS_OVERFLOW_THRESHOLD_OFFSET      (5200)
#define HANDLER_WATER_LEVEL_RESET_VALUE                              (27000)
#define HANDLER_WATER_LEVEL_DEFAULT_RESET_RANGE                      (5000)
#define HANDLER_WATER_LEVEL_READINGS_ACCEPTED_DIFF                   (50)
#define HANDLER_WATER_LEVEL_ZERO_LEVEL_ACCEPTED_DIFF                 (1000)
#define HANDLER_WATER_LEVEL_ZERO_LEVEL_ERROR_ACCEPTED_DIFF           (500)
#define HANDLER_WATER_LEVEL_ZERO_LEVEL_SPIN_ERROR_ACCEPTED_DIFF      (HANDLER_WATER_LEVEL_ZERO_LEVEL_ACCEPTED_DIFF + 100)
#define HANDLER_WATER_LEVEL_ZERO_LEVEL_LEAKAGE_ERROR_ACCEPTED_DIFF   (600)
#define HANDLER_WATER_LEVEL_ZERO_LEVEL_TEST_MODE_ERROR_ACCEPTED_DIFF (400)
#define HANDLER_WATER_LEVEL_ZERO_CALIBRATION_ACCEPTED_DIFF           (1000)

#define HANDLER_WATERLEVEL_EEPROM_CORRECTION_FACTOR_FLAG_MAGIC_VALUE (0xAAU)

void handler_waterLevel_init(void);

uint32_t handler_waterLevel_currentFreq_get(void);
uint32_t handler_waterLevel_samplesPerSec_get(void);
uint32_t handler_waterLevel_freqValue_get(uint32_t litter);
uint8_t handler_waterLevel_resetLevel_check(handler_waterLevel_resetCheckType_t type);
uint8_t handler_waterLevel_error_get(void);
uint32_t handler_waterLevel_resetLevel_get(void);
uint32_t handler_waterLevel_upperDrainFilterThreshold_get(void);
uint32_t handler_waterLevel_lowerDrainFilterThreshold_get(void);
uint8_t handler_waterLevel_calibrationFlag_get(void);

void handler_waterLevel_resetLevel_set(uint32_t value);
void handler_waterLevel_currentLevelFreq_set(uint32_t value);
void handler_waterLevel_resetLevel_mark(void);
void handler_waterLevel_calibrationFlag_set(void);

uint8_t handler_waterLevel_compare(uint32_t current, uint32_t userSetting, uint32_t diff);

void handler_waterLevel_currentFreq_set(uint32_t value);

#endif
