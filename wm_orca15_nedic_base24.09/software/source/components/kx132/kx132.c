#include "component_config.h"
#include "kx132.h"
#include "components/gpio/gpio.h"
#include "math.h"
#include "os.h"

typedef enum kx132_state
{
    KX132_STATE_INIT,
    KX132_STATE_PRE_STARTUP,
    KX132_STATE_STARTUP_0,
    KX132_STATE_STARTUP_1,
    KX132_STATE_CALIBRATION_STABILIZATION,
    KX132_STATE_FETCH_X_DATA,
    KX132_STATE_FETCH_Y_DATA,
    KX132_STATE_FETCH_Z_DATA,
    KX132_STATE_PROCESS_DATA,
    KX132_STATE_HEALTH_CHECK,
    KX132_STATE_ERROR
} kx132_state_t;

typedef enum kx132_error
{
    KX132_AK = 0,
    KX132_NAK = 1
} kx132_error_t;

typedef uint8_t kx132_readWrite_t;
#define KX132_READ  0x80U
#define KX132_WRITE 0x00U

typedef uint8_t kx132_internalReg_t;
#define KX132_ACCEL_BASE   0x08U
#define KX132_X_OUT_L_REG  0x08U
#define KX132_X_OUT_H_REG  0x09U
#define KX132_Y_OUT_L_REG  0x0AU
#define KX132_Y_OUT_H_REG  0x0BU
#define KX132_Z_OUT_L_REG  0x0CU
#define KX132_Z_OUT_H_REG  0x0DU
#define KX132_COTR_REG     0x12U
#define KX132_WHO_AM_I_REG 0x13U
#define KX132_CNTL1_REG    0x1BU
#define KX132_CNTL2_REG    0x1CU
#define KX132_ODCNTL_REG   0x21U

#define KX132_COTR_RESPONSE               (0x55U)
#define KX132_WHO_AM_I_RESPONSE           (0x3DU)
#define KX132_CNTL1_CONFIG_CHANGE_ENABLE  (0x00U)
#define KX132_CNTL2_RAM_RESET             (0x80U)
#define KX132_CNTL1_OPERATION_2G_START    (0xC0U)
#define KX132_ODCNTL_OUTPUT_DATA_RATE_SET (0x06U)

static void kx132_configuration_start(void);
static kx132_error_t kx132_configuration_continue_0(void);
static void kx132_configuration_continue_1(void);

static void kx132_accelRawX_get(int16_t *buff);
static void kx132_accelRawY_get(int16_t *buff);
static void kx132_accelRawZ_get(int16_t *buff);
static uint8_t kx132_transmit(kx132_internalReg_t reg, kx132_readWrite_t readWrite, uint8_t value);
static void kx132_dummy_delay(uint32_t del);

kx132_state_t kx132State = KX132_STATE_INIT;

static uint8_t kx132_detectionEnable_flag = 1;
static uint32_t kx132_errorCnt = 0, stateCnt = 0;
static uint8_t gu8_unbalanceXHitCnt = 0, gu8_unbalanceYHitCnt = 0, gu8_unbalanceZHitCnt = 0, gu16_unbalanceSumHitCnt = 0, gu8_unbalanceCriticalHitCnt = 0;
static uint8_t gu8_kx132_hitThreshold, gu8_kx132_criticalHitThreshold;
static uint8_t gu8_kx132_unbalance_flag = 0;
static uint8_t gu8_kx132_error_flag = 0;
static int16_t mems_xyz[4] = {0}, mems_prevXYZ[4] = {0};
static int16_t mems_max[4] = {-32000, -32000, -32000};
static int32_t mems_xyzCalibrated[4] = {0};
static kx132_csLevel_t csONLevel;

static int32_t xThreshold = 0, yThreshold = 0, zThreshold = 0;
static uint8_t kx132_normalHitDone_flag = 0;

static gpio_t control;

void kx132_init(kx132_config_t *config)
{
    sspi_config_t sspi;

    sspi.bitRate = config->config.bitRate;
    sspi.mosiPort = config->config.mosiPort;
    sspi.mosiPin = config->config.mosiPin;
    sspi.misoPort = config->config.misoPort;
    sspi.misoPin = config->config.misoPin;
    sspi.csPort = config->config.csPort;
    sspi.csPin = config->config.csPin;
    sspi.sclPort = config->config.sclPort;
    sspi.sclPin = config->config.sclPin;
    sspi.dataOrder = config->config.dataOrder;
    sspi.mode = config->config.mode;
    sspi_init(&sspi);

    control.ioState = config->controlPin.ioState;
    control.port = config->controlPin.port;
    control.pin = config->controlPin.pin;
    control.state = config->controlPin.state;
    gpio_pin_init(&control);

    csONLevel = config->csLevel;
    gu8_kx132_hitThreshold = config->hitRepeatCount;
    gu8_kx132_criticalHitThreshold = config->criticalHitRepeatCount;
    xThreshold = config->xThreshold;
    yThreshold = config->yThreshold;
    zThreshold = config->zThreshold;

    kx132State = KX132_STATE_PRE_STARTUP;
}

void kx132_update(uint32_t period)
{
    static uint32_t calibrationCnt = 0;

    switch (kx132State)
    {
        case KX132_STATE_INIT:
        {
            // do nothing
            // this is to make sure that the init has been called
        }
        break;

        case KX132_STATE_PRE_STARTUP:
        {
            gpio_pinState_set(control.port, control.pin, GPIO_HIGH);

            stateCnt += period;
            if (stateCnt >= 30)
            {
                kx132State = KX132_STATE_STARTUP_0;
                kx132_configuration_start();
            }
        }
        break;

        case KX132_STATE_STARTUP_0:
        {
            if (kx132_configuration_continue_0() == KX132_AK)
            {
                kx132State = KX132_STATE_STARTUP_1;
            }
            else
            {
                kx132State = KX132_STATE_ERROR;
            }
        }
        break;

        case KX132_STATE_STARTUP_1:
        {
            kx132_configuration_continue_1();
            kx132State = KX132_STATE_CALIBRATION_STABILIZATION;
        }
        break;

        case KX132_STATE_CALIBRATION_STABILIZATION:
        {
            if (calibrationCnt < 20U)
            {
                // NOTE: ignore the first 20 readings as they sometimes are not stable due to power supply startup issues
                calibrationCnt++;
            }
            else
            {
                calibrationCnt = 0;
                kx132State = KX132_STATE_FETCH_X_DATA;
            }
        }
        break;

        case KX132_STATE_FETCH_X_DATA:
        {
            kx132_accelRawX_get(mems_xyz);
            kx132State = KX132_STATE_FETCH_Y_DATA;
        }
        break;

        case KX132_STATE_FETCH_Y_DATA:
        {
            kx132_accelRawY_get(mems_xyz);
            kx132State = KX132_STATE_FETCH_Z_DATA;
        }
        break;

        case KX132_STATE_FETCH_Z_DATA:
        {
            kx132_accelRawZ_get(mems_xyz);
            kx132State = KX132_STATE_PROCESS_DATA;
        }
        break;

        case KX132_STATE_PROCESS_DATA:
        {
            if (calibrationCnt < 16U)
            {
                calibrationCnt++;

                mems_xyzCalibrated[0] += mems_xyz[0];
                mems_xyzCalibrated[1] += mems_xyz[1];
                mems_xyzCalibrated[2] += mems_xyz[2];

                if (calibrationCnt == 16U)
                {
                    mems_xyzCalibrated[0] >>= 4U;
                    mems_xyzCalibrated[1] >>= 4U;
                    mems_xyzCalibrated[2] >>= 4U;

                    mems_prevXYZ[0] = (int16_t)mems_xyzCalibrated[0];
                    mems_prevXYZ[1] = (int16_t)mems_xyzCalibrated[1];
                    mems_prevXYZ[2] = (int16_t)mems_xyzCalibrated[2];
                }

                kx132State = KX132_STATE_FETCH_X_DATA;
            }
            else
            {
                if (mems_xyz[0] > mems_max[0])
                {
                    mems_max[0] = mems_xyz[0];
                }
                if (mems_xyz[1] > mems_max[1])
                {
                    mems_max[1] = mems_xyz[1];
                }
                if (mems_xyz[2] > mems_max[2])
                {
                    mems_max[2] = mems_xyz[2];
                }

                if (((mems_xyz[0] > (mems_xyzCalibrated[0] + xThreshold)) || (mems_xyz[1] > (mems_xyzCalibrated[1] + yThreshold)) || (mems_xyz[2] > (mems_xyzCalibrated[2] + zThreshold))) && (kx132_normalHitDone_flag != 0U))
                {
                    gu8_unbalanceCriticalHitCnt++;
                    if ((gu8_unbalanceCriticalHitCnt >= gu8_kx132_criticalHitThreshold) && (kx132_detectionEnable_flag != 0U))
                    {
                        gu8_unbalanceXHitCnt = 0;
                        gu8_unbalanceYHitCnt = 0;
                        gu8_unbalanceZHitCnt = 0;
                        gu8_unbalanceCriticalHitCnt = 0;
                        gu8_kx132_unbalance_flag = 1;
                        kx132_normalHitDone_flag = 0;
                    }
                }
                else if (mems_xyz[0] > mems_prevXYZ[0])
                {
                    mems_prevXYZ[0] = mems_xyz[0];
                    gu8_unbalanceXHitCnt++;
                }
                else if (mems_xyz[0] > (mems_xyzCalibrated[0] + xThreshold))
                {
                    gu8_unbalanceXHitCnt += 2U;
                }
                else if (mems_xyz[1] > mems_prevXYZ[1])
                {
                    mems_prevXYZ[1] = mems_xyz[1];
                    gu8_unbalanceYHitCnt++;
                }
                else if (mems_xyz[1] > (mems_xyzCalibrated[1] + yThreshold))
                {
                    gu8_unbalanceYHitCnt += 2U;
                }
                else if (mems_xyz[2] > mems_prevXYZ[2])
                {
                    mems_prevXYZ[2] = mems_xyz[2];
                    gu8_unbalanceZHitCnt++;
                }
                else if (mems_xyz[2] > (mems_xyzCalibrated[2] + zThreshold))
                {
                    gu8_unbalanceZHitCnt += 2U;
                }
                else
                {
                    // do nothing
                }

                gu16_unbalanceSumHitCnt = gu8_unbalanceXHitCnt + gu8_unbalanceYHitCnt + gu8_unbalanceZHitCnt;
                if (gu16_unbalanceSumHitCnt > gu8_kx132_hitThreshold)
                {
                    kx132_normalHitDone_flag = 1U;
                }

                kx132State = KX132_STATE_HEALTH_CHECK;
            }
        }
        break;

        case KX132_STATE_HEALTH_CHECK:
        {
            if (kx132_transmit(KX132_COTR_REG, KX132_READ, KX132_COTR_RESPONSE) != KX132_COTR_RESPONSE)
            {
                kx132State = KX132_STATE_ERROR;
            }
            else
            {
                kx132State = KX132_STATE_FETCH_X_DATA;
            }
        }
        break;

        case KX132_STATE_ERROR:
        {
            kx132_errorCnt++;
            if (kx132_errorCnt < 5U)
            {
                kx132State = KX132_STATE_PRE_STARTUP;
                stateCnt = 0;
                gpio_pinState_set(control.port, control.pin, GPIO_LOW);
            }
            else
            {
                gu8_kx132_error_flag = 1;
            }
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }

    LOG('m', 'm', "[%d] mems: %-6d  %-6d  %-6d   calib: %-6d  %-6d  %-6d   max: %-6d  %-6d  %-6d   hitsThr:%d  hitsX:%d  hitsY:%d  hitsZ:%d  cHitsThr:%d  cHits:%d", kx132State, mems_xyz[0], mems_xyz[1], mems_xyz[2], mems_xyzCalibrated[0], mems_xyzCalibrated[1], mems_xyzCalibrated[2], mems_max[0], mems_max[1], mems_max[2], gu8_kx132_hitThreshold, gu8_unbalanceXHitCnt, gu8_unbalanceYHitCnt, gu8_unbalanceZHitCnt, gu8_kx132_criticalHitThreshold, gu8_unbalanceCriticalHitCnt);
}

uint8_t kx132_unbalanceFlag_get(void)
{
    return gu8_kx132_unbalance_flag;
}

void kx132_xThreshold_set(int32_t value)
{
    xThreshold = value;
}

void kx132_yThreshold_set(int32_t value)
{
    yThreshold = value;
}

void kx132_zThreshold_set(int32_t value)
{
    zThreshold = value;
}

int16_t kx132_faultValue_get(kx132_faultDirection_t direction)
{
    return mems_max[direction];
}

void kx132_hitCnt_set(uint8_t value)
{
    gu8_kx132_hitThreshold = value;
}

void kx132_critHitCnt_set(uint8_t value)
{
    gu8_kx132_criticalHitThreshold = value;
}

void kx132_unbalanceFlag_set(uint8_t value)
{
    gu8_kx132_unbalance_flag = value;
}

void kx132_unbalanceDetection_set(uint8_t value)
{
    kx132_detectionEnable_flag = value;
}

void kx132_unbalanceCnt_reset(void)
{
    gu8_unbalanceXHitCnt = 0;
    gu8_unbalanceYHitCnt = 0;
    gu8_unbalanceZHitCnt = 0;
    gu8_unbalanceCriticalHitCnt = 0;
    kx132_normalHitDone_flag = 0;
    mems_prevXYZ[0] = (int16_t)mems_xyzCalibrated[0];
    mems_prevXYZ[1] = (int16_t)mems_xyzCalibrated[1];
    mems_prevXYZ[2] = (int16_t)mems_xyzCalibrated[2];
}

uint8_t kx132_error_get(void)
{
    return gu8_kx132_error_flag;
}

int16_t kx132_xValue_get(void)
{
    return mems_xyz[0];
}

int16_t kx132_yValue_get(void)
{
    return mems_xyz[1];
}

int16_t kx132_zValue_get(void)
{
    return mems_xyz[2];
}

int16_t kx132_xCalibratedValue_get(void)
{
    return (int16_t)mems_xyzCalibrated[0];
}

int16_t kx132_yCalibratedValue_get(void)
{
    return (int16_t)mems_xyzCalibrated[1];
}

int16_t kx132_zCalibratedValue_get(void)
{
    return (int16_t)mems_xyzCalibrated[2];
}

uint8_t kx132_currentHitCnt_get(void)
{
    return gu16_unbalanceSumHitCnt;
}

uint8_t kx132_currentCritHitCnt_get(void)
{
    return gu8_unbalanceCriticalHitCnt;
}

static void kx132_configuration_start(void)
{
    (void)kx132_transmit(KX132_CNTL1_REG, KX132_WRITE, KX132_CNTL1_CONFIG_CHANGE_ENABLE);
    (void)kx132_transmit(KX132_CNTL2_REG, KX132_WRITE, KX132_CNTL2_RAM_RESET);
}

static kx132_error_t kx132_configuration_continue_0(void)
{
    kx132_error_t ret = KX132_AK;

    if (kx132_transmit(KX132_WHO_AM_I_REG, KX132_READ, KX132_WHO_AM_I_RESPONSE) != KX132_WHO_AM_I_RESPONSE)
    {
        ret = KX132_NAK;
    }

    if (kx132_transmit(KX132_COTR_REG, KX132_READ, KX132_COTR_RESPONSE) != KX132_COTR_RESPONSE)
    {
        ret = KX132_NAK;
    }

    return ret;
}
static void kx132_configuration_continue_1(void)
{
    (void)kx132_transmit(KX132_ODCNTL_REG, KX132_WRITE, KX132_ODCNTL_OUTPUT_DATA_RATE_SET);
    (void)kx132_transmit(KX132_CNTL1_REG, KX132_WRITE, KX132_CNTL1_OPERATION_2G_START);
}

static void kx132_accelRawX_get(int16_t *buff)
{
    sspi_csState_set(csONLevel);
    (void)sspi_data_transmit((uint8_t)(~(uint8_t)((uint8_t)KX132_X_OUT_L_REG | (uint8_t)KX132_READ)));
    buff[0] = abs((int16_t)(((int16_t)sspi_data_transmit(0xff)) | ((int16_t)sspi_data_transmit(0xff) << 8)));
    sspi_csState_set(!csONLevel);
}
static void kx132_accelRawY_get(int16_t *buff)
{
    sspi_csState_set(csONLevel);
    (void)sspi_data_transmit((uint8_t)(~(uint8_t)((uint8_t)KX132_Y_OUT_L_REG | (uint8_t)KX132_READ)));
    buff[1] = abs((int16_t)(((int16_t)sspi_data_transmit(0xff)) | ((int16_t)sspi_data_transmit(0xff) << 8)));
    sspi_csState_set(!csONLevel);
}
static void kx132_accelRawZ_get(int16_t *buff)
{
    sspi_csState_set(csONLevel);
    (void)sspi_data_transmit((uint8_t)(~(uint8_t)((uint8_t)KX132_Z_OUT_L_REG | (uint8_t)KX132_READ)));
    buff[2] = abs((int16_t)(((int16_t)sspi_data_transmit(0xff)) | ((int16_t)sspi_data_transmit(0xff) << 8)));
    sspi_csState_set(!csONLevel);
}

static uint8_t kx132_transmit(kx132_internalReg_t reg, kx132_readWrite_t readWrite, uint8_t value)
{
    uint8_t ret = 0;

    sspi_csState_set(csONLevel);
    (void)sspi_data_transmit(~(reg | readWrite));

    if (readWrite == KX132_WRITE)
    {
        ret = ~(sspi_data_transmit(~(value)));
    }
    else if (readWrite == KX132_READ)
    {
        ret = ~(sspi_data_transmit(0xff));
    }
    else
    {
        // do nothing
    }

    sspi_csState_set(!csONLevel);

    kx132_dummy_delay(200);

    return ret;
}

static void kx132_dummy_delay(uint32_t del)
{
    for (volatile uint32_t i = 0; i < del; i++)
    {
        // do nothing
    }
}
