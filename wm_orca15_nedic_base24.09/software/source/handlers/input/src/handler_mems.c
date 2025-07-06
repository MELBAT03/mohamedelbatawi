#include "def.h"
#include "board.h"
#include "components/kx022/kx022.h"
#include "handlers/input/inc/handler_mems.h"

static kx022_config_t mems;

static uint32_t mems_normalModeCalibratedValues[] = {1300, 2300, 2000};
static uint32_t mems_testModeCalibratedValues[] = {1800, 2600, 2300};

static uint8_t mems_normalModeHitCnt = 30, mems_normalModeCritHitCnt = 1;
static uint8_t mems_testModeHitCnt = 160, mems_testModeCritHitCnt = 2;

void handler_mems_init(void)
{
    mems.config.bitRate = 0;
    mems.config.mosiPort = SSPI_MOSI_PORT;
    mems.config.mosiPin = SSPI_MOSI_PIN;
    mems.config.misoPort = SSPI_MISO_PORT;
    mems.config.misoPin = SSPI_MISO_PIN;
    mems.config.csPort = SSPI_CS_PORT;
    mems.config.csPin = SSPI_CS_PIN;
    mems.config.sclPort = SSPI_SCL_PORT;
    mems.config.sclPin = SSPI_SCL_PIN;
    mems.config.dataOrder = SSPI_MSB_FIRST;
    mems.config.mode = SPI_MODE_2;
    mems.csLevel = KX022_CS_ON_LEVEL_HIGH;

    mems.controlPin.ioState = GPIO_OUTPUT;
    mems.controlPin.port = SSPI_ON_OFF_PORT;
    mems.controlPin.pin = SSPI_ON_OFF_PIN;
    mems.controlPin.state = GPIO_HIGH;

    mems.criticalHitRepeatCount = mems_normalModeCritHitCnt;
    mems.hitRepeatCount = mems_normalModeHitCnt;

    mems.xThreshold = mems_normalModeCalibratedValues[0];
    mems.yThreshold = mems_normalModeCalibratedValues[1];
    mems.zThreshold = mems_normalModeCalibratedValues[2];

    kx022_init(&mems);
}

handler_mems_unbalanceState_t handler_mems_unbalanceDetectionFlag_get(void)
{
    handler_mems_unbalanceState_t ret = UNBALANCE_NOT_DETECTED;

    ret = (handler_mems_unbalanceState_t)kx022_unbalanceFlag_get();

    return ret;
}

void handler_mems_xThreshold_set(uint32_t value)
{
    kx022_xThreshold_set(value);
    mems.xThreshold = value;
}

void handler_mems_yThreshold_set(uint32_t value)
{
    kx022_yThreshold_set(value);
    mems.yThreshold = value;
}

void handler_mems_zThreshold_set(uint32_t value)
{
    kx022_zThreshold_set(value);
    mems.zThreshold = value;
}

void handler_mems_hitCnt_set(uint8_t value)
{
    kx022_hitCnt_set(value);
    mems.hitRepeatCount = value;
}

void handler_mems_critHitCnt_set(uint8_t value)
{
    kx022_critHitCnt_set(value);
    mems.criticalHitRepeatCount = value;
}

int16_t handler_mems_faultValue_get(handler_mems_faultDirection_t direction)
{
    return kx022_faultValue_get((kx022_faultDirection_t)direction);
}

void handler_mems_unbalanceDetectionFlag_set(uint8_t value)
{
    kx022_unbalanceFlag_set(value);
}

void handler_mems_unbalanceCnt_reset(void)
{
    kx022_unbalanceCnt_reset();
}

void handler_mems_unbalance_reset(void)
{
    kx022_unbalanceFlag_set(0);
    kx022_unbalanceCnt_reset();
}

uint8_t handler_mems_error_get(void)
{
    return kx022_error_get();
}

uint32_t handler_mems_xThresholdValue_get(void)
{
    return mems.xThreshold;
}

uint32_t handler_mems_yThresholdValue_get(void)
{
    return mems.yThreshold;
}

uint32_t handler_mems_zThresholdValue_get(void)
{
    return mems.zThreshold;
}

uint32_t handler_mems_xThresholdTestModeValue_get(void)
{
    return mems_testModeCalibratedValues[0];
}

uint32_t handler_mems_yThresholdTestModeValue_get(void)
{
    return mems_testModeCalibratedValues[1];
}

uint32_t handler_mems_zThresholdTestModeValue_get(void)
{
    return mems_testModeCalibratedValues[2];
}

uint8_t handler_mems_hitCntTestModeValue_get(void)
{
    return mems_testModeHitCnt;
}

uint8_t handler_mems_critHitCntTestModeValue_get(void)
{
    return mems_testModeCritHitCnt;
}

uint8_t handler_mems_hitCntNormalModeValue_get(void)
{
    return mems_normalModeHitCnt;
}

uint8_t handler_mems_critHitCntNormalModeValue_get(void)
{
    return mems_normalModeCritHitCnt;
}

uint8_t handler_mems_hitCnt_get(void)
{
    return mems.hitRepeatCount;
}

uint8_t handler_mems_critHitCnt_get(void)
{
    return mems.criticalHitRepeatCount;
}

int16_t handler_mems_xValue_get(void)
{
    return kx022_xValue_get();
}

int16_t handler_mems_yValue_get(void)
{
    return kx022_yValue_get();
}

int16_t handler_mems_zValue_get(void)
{
    return kx022_zValue_get();
}

int16_t handler_mems_xCalibratedValue_get(void)
{
    return kx022_xCalibratedValue_get();
}

int16_t handler_mems_yCalibratedValue_get(void)
{
    return kx022_yCalibratedValue_get();
}

int16_t handler_mems_zCalibratedValue_get(void)
{
    return kx022_zCalibratedValue_get();
}

uint8_t handler_mems_currentHitCnt_get(void)
{
    return kx022_currentHitCnt_get();
}
uint8_t handler_mems_currentCritHitCnt_get(void)
{
    return kx022_currentCritHitCnt_get();
}
