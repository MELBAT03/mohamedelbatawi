#ifndef KX132_H
#define KX132_H

#include "def.h"
#include "components/sspi/sspi.h"

typedef enum
{
    KX132_CS_ON_LEVEL_HIGH = 1,
    KX132_CS_ON_LEVEL_LOW = 0
} kx132_csLevel_t;

typedef enum
{
    KX132_FAULT_X = 0,
    KX132_FAULT_Y = 1,
    KX132_FAULT_Z = 2
} kx132_faultDirection_t;

typedef struct
{
    sspi_config_t config;
    kx132_csLevel_t csLevel;
    gpio_t controlPin;
    uint8_t criticalHitRepeatCount;
    uint8_t hitRepeatCount;
    uint32_t xThreshold;
    uint32_t yThreshold;
    uint32_t zThreshold;
} kx132_config_t;

void kx132_init(kx132_config_t *config);

void kx132_update(uint32_t period);

void kx132_xThreshold_set(int32_t value);
void kx132_yThreshold_set(int32_t value);
void kx132_zThreshold_set(int32_t value);

void kx132_hitCnt_set(uint8_t value);
void kx132_critHitCnt_set(uint8_t value);

uint8_t kx132_unbalanceFlag_get(void);
uint8_t kx132_error_get(void);
int16_t kx132_xValue_get(void);
int16_t kx132_yValue_get(void);
int16_t kx132_zValue_get(void);
int16_t kx132_xCalibratedValue_get(void);
int16_t kx132_yCalibratedValue_get(void);
int16_t kx132_zCalibratedValue_get(void);

uint8_t kx132_currentHitCnt_get(void);
uint8_t kx132_currentCritHitCnt_get(void);

int16_t kx132_faultValue_get(kx132_faultDirection_t direction);

void kx132_unbalanceFlag_set(uint8_t value);
void kx132_unbalanceDetection_set(uint8_t value);
void kx132_unbalanceCnt_reset(void);

#endif
