#ifndef HANDLER_MEMS_H
#define HANDLER_MEMS_H 1

#include "def.h"

#ifndef TESTING
#include "components/kx022/kx022.h"

typedef enum handler_mems_faultDirection
{
    APP_MEMS_FAULT_X = KX022_FAULT_X,
    APP_MEMS_FAULT_Y = KX022_FAULT_Y,
    APP_MEMS_FAULT_Z = KX022_FAULT_Z
} handler_mems_faultDirection_t;

#else

typedef enum handler_mems_faultDirection
{
    APP_MEMS_FAULT_X = 0,
    APP_MEMS_FAULT_Y = 1,
    APP_MEMS_FAULT_Z = 2
} handler_mems_faultDirection_t;

#endif

typedef enum handler_mems_unbalanceState
{
    UNBALANCE_NOT_DETECTED = 0,
    UNBALANCE_DETECTED = 1
} handler_mems_unbalanceState_t;

void handler_mems_init(void);

handler_mems_unbalanceState_t handler_mems_unbalanceDetectionFlag_get(void);
uint8_t handler_mems_error_get(void);

uint32_t handler_mems_xThresholdValue_get(void);
uint32_t handler_mems_yThresholdValue_get(void);
uint32_t handler_mems_zThresholdValue_get(void);

void handler_mems_xThreshold_set(uint32_t value);
void handler_mems_yThreshold_set(uint32_t value);
void handler_mems_zThreshold_set(uint32_t value);

uint32_t handler_mems_xThresholdTestModeValue_get(void);
uint32_t handler_mems_yThresholdTestModeValue_get(void);
uint32_t handler_mems_zThresholdTestModeValue_get(void);

void handler_mems_hitCnt_set(uint8_t value);
void handler_mems_critHitCnt_set(uint8_t value);

uint8_t handler_mems_hitCntTestModeValue_get(void);
uint8_t handler_mems_critHitCntTestModeValue_get(void);
uint8_t handler_mems_hitCntNormalModeValue_get(void);
uint8_t handler_mems_critHitCntNormalModeValue_get(void);

int16_t handler_mems_faultValue_get(handler_mems_faultDirection_t direction);

void handler_mems_unbalanceDetectionFlag_set(uint8_t value);
void handler_mems_unbalanceCnt_reset(void);
void handler_mems_unbalance_reset(void);

uint8_t handler_mems_hitCnt_get(void);
uint8_t handler_mems_critHitCnt_get(void);

int16_t handler_mems_xValue_get(void);
int16_t handler_mems_yValue_get(void);
int16_t handler_mems_zValue_get(void);
int16_t handler_mems_xCalibratedValue_get(void);
int16_t handler_mems_yCalibratedValue_get(void);
int16_t handler_mems_zCalibratedValue_get(void);

uint8_t handler_mems_currentHitCnt_get(void);
uint8_t handler_mems_currentCritHitCnt_get(void);

#endif
