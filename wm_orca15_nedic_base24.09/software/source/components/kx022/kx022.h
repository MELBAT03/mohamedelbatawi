#ifndef KX022_H
#define KX022_H

#include "def.h"
#include "components/sspi/sspi.h"

typedef enum
{
    KX022_CS_ON_LEVEL_HIGH = 1,
    KX022_CS_ON_LEVEL_LOW = 0
} kx022_csLevel_t;

typedef enum
{
    KX022_FAULT_X = 0,
    KX022_FAULT_Y = 1,
    KX022_FAULT_Z = 2
} kx022_faultDirection_t;

typedef struct
{
    sspi_config_t config;
    kx022_csLevel_t csLevel;
    gpio_t controlPin;
    uint8_t criticalHitRepeatCount;
    uint8_t hitRepeatCount;
    uint32_t xThreshold;
    uint32_t yThreshold;
    uint32_t zThreshold;
} kx022_config_t;

void kx022_init(kx022_config_t *config);

void kx022_update(uint32_t period);
int32_t kx022_xThreshold_get(void);
void kx022_xThreshold_set(int32_t value);
void kx022_yThreshold_set(int32_t value);
void kx022_zThreshold_set(int32_t value);

void kx022_hitCnt_set(uint8_t value);
void kx022_critHitCnt_set(uint8_t value);

uint8_t kx022_unbalanceFlag_get(void);
uint8_t kx022_error_get(void);
int16_t kx022_xValue_get(void);
int16_t kx022_yValue_get(void);
int16_t kx022_zValue_get(void);
int16_t kx022_xCalibratedValue_get(void);
int16_t kx022_yCalibratedValue_get(void);
int16_t kx022_zCalibratedValue_get(void);

uint8_t kx022_currentHitCnt_get(void);
uint8_t kx022_currentCritHitCnt_get(void);

int16_t kx022_faultValue_get(kx022_faultDirection_t direction);

void kx022_unbalanceFlag_set(uint8_t value);
void kx022_unbalanceDetection_set(uint8_t value);
void kx022_unbalanceCnt_reset(void);

#endif
