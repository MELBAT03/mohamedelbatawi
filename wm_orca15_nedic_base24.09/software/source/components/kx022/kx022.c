#include "component_config.h"
#include "kx022.h"
#include "components/gpio/gpio.h"
#include "math.h"
#include "os.h"

typedef enum kx022_state
{
    KX022_STATE_INIT,
    KX022_STATE_PRE_STARTUP,
    KX022_STATE_STARTUP_0,
    KX022_STATE_STARTUP_1,
    KX022_STATE_CALIBRATION_STABILIZATION,
    KX022_STATE_FETCH_X_DATA,
    KX022_STATE_FETCH_Y_DATA,
    KX022_STATE_FETCH_Z_DATA,
    KX022_STATE_PROCESS_DATA,
    KX022_STATE_HEALTH_CHECK,
    KX022_STATE_ERROR
} kx022_state_t;

typedef enum kx022_error
{
    KX022_AK = 0,
    KX022_NAK = 1
} kx022_error_t;

typedef uint8_t kx022_readWrite_t;
#define KX022_READ  0x80U
#define KX022_WRITE 0x00U

typedef uint8_t kx022_internalReg_t;
#define KX022_ACCEL_BASE   0x06U
#define KX022_X_OUT_L_REG  0x06U
#define KX022_X_OUT_H_REG  0x07U
#define KX022_Y_OUT_L_REG  0x08U
#define KX022_Y_OUT_H_REG  0x09U
#define KX022_Z_OUT_L_REG  0x0AU
#define KX022_Z_OUT_H_REG  0x0BU
#define KX022_COTR_REG     0x0CU
#define KX022_WHO_AM_I_REG 0x0FU
#define KX022_CNTL1_REG    0x18U
#define KX022_CNTL2_REG    0x19U
#define KX022_ODCNTL_REG   0x1BU

#define KX022_COTR_RESPONSE               (0x55U)
#define KX022_WHO_AM_I_RESPONSE           (0x14U)
#define KX022_CNTL1_CONFIG_CHANGE_ENABLE  (0x40U)
#define KX022_CNTL2_RAM_RESET             (0x80U)
#define KX022_CNTL1_OPERATION_2G_START    (0xC0U)
#define KX022_ODCNTL_OUTPUT_DATA_RATE_SET (0x02U)

static void kx022_configuration_start(void);
static kx022_error_t kx022_configuration_continue_0(void);
static void kx022_configuration_continue_1(void);

static void kx022_accelRawX_get(int16_t *buff);
static void kx022_accelRawY_get(int16_t *buff);
static void kx022_accelRawZ_get(int16_t *buff);
static uint8_t kx022_transmit(kx022_internalReg_t reg, kx022_readWrite_t readWrite, uint8_t value);
static void kx022_dummy_delay(uint32_t del);

kx022_state_t kx022State = KX022_STATE_INIT;

static uint8_t kx022_detectionEnable_flag = 1;
static uint32_t kx022_errorCnt = 0, stateCnt = 0;
static uint8_t gu8_unbalanceXHitCnt = 0, gu8_unbalanceYHitCnt = 0, gu8_unbalanceZHitCnt = 0, gu16_unbalanceSumHitCnt = 0, gu8_unbalanceCriticalHitCnt = 0;
static uint8_t gu8_kx022_hitThreshold, gu8_kx022_criticalHitThreshold;
static uint8_t gu8_kx022_unbalance_flag = 0;
static uint8_t gu8_kx022_error_flag = 0;
static int16_t mems_xyz[4] = {0}, mems_prevXYZ[4] = {0};
static int16_t mems_max[4] = {-32000, -32000, -32000};
static int32_t mems_xyzCalibrated[4] = {0};
static kx022_csLevel_t csONLevel;

static int32_t xThreshold = 0, yThreshold = 0, zThreshold = 0;
static uint8_t kx022_normalHitDone_flag = 0;

static gpio_t control;

void kx022_init(kx022_config_t *config)
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
    gu8_kx022_hitThreshold = config->hitRepeatCount;
    gu8_kx022_criticalHitThreshold = config->criticalHitRepeatCount;
    xThreshold = config->xThreshold;
    yThreshold = config->yThreshold;
    zThreshold = config->zThreshold;

    kx022State = KX022_STATE_PRE_STARTUP;
}

void kx022_update(uint32_t period)
{
    static uint32_t calibrationCnt = 0;

    switch (kx022State)
    {
        case KX022_STATE_INIT:
        {
            // do nothing
            // this is to make sure that the init has been called
        }
        break;

        case KX022_STATE_PRE_STARTUP:
        {
            gpio_pinState_set(control.port, control.pin, GPIO_HIGH);

            stateCnt += period;
            if (stateCnt >= 30)
            {
                kx022State = KX022_STATE_STARTUP_0;
                kx022_configuration_start();
            }
        }
        break;

        case KX022_STATE_STARTUP_0:
        {
            if (kx022_configuration_continue_0() == KX022_AK)
            {
                kx022State = KX022_STATE_STARTUP_1;
            }
            else
            {
                kx022State = KX022_STATE_ERROR;
            }
        }
        break;

        case KX022_STATE_STARTUP_1:
        {
            kx022_configuration_continue_1();
            kx022State = KX022_STATE_CALIBRATION_STABILIZATION;
        }
        break;

        case KX022_STATE_CALIBRATION_STABILIZATION:
        {
            if (calibrationCnt < 20U)
            {
                // NOTE: ignore the first 20 readings as they sometimes are not stable due to power supply startup issues
                calibrationCnt++;
            }
            else
            {
                calibrationCnt = 0;
                kx022State = KX022_STATE_FETCH_X_DATA;
            }
        }
        break;

        case KX022_STATE_FETCH_X_DATA:
        {
            kx022_accelRawX_get(mems_xyz);
            kx022State = KX022_STATE_FETCH_Y_DATA;
        }
        break;

        case KX022_STATE_FETCH_Y_DATA:
        {
            kx022_accelRawY_get(mems_xyz);
            kx022State = KX022_STATE_FETCH_Z_DATA;
        }
        break;

        case KX022_STATE_FETCH_Z_DATA:
        {
            kx022_accelRawZ_get(mems_xyz);
            kx022State = KX022_STATE_PROCESS_DATA;
        }
        break;

        case KX022_STATE_PROCESS_DATA:
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

                kx022State = KX022_STATE_FETCH_X_DATA;
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

                if (((mems_xyz[0] > (mems_xyzCalibrated[0] + xThreshold)) || (mems_xyz[1] > (mems_xyzCalibrated[1] + yThreshold)) || (mems_xyz[2] > (mems_xyzCalibrated[2] + zThreshold))) && (kx022_normalHitDone_flag != 0U))
                {
                    gu8_unbalanceCriticalHitCnt++;
                    if ((gu8_unbalanceCriticalHitCnt >= gu8_kx022_criticalHitThreshold) && (kx022_detectionEnable_flag != 0U))
                    {
                        gu8_unbalanceXHitCnt = 0;
                        gu8_unbalanceYHitCnt = 0;
                        gu8_unbalanceZHitCnt = 0;
                        gu8_unbalanceCriticalHitCnt = 0;
                        gu8_kx022_unbalance_flag = 1;
                        kx022_normalHitDone_flag = 0;
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
                if (gu16_unbalanceSumHitCnt > gu8_kx022_hitThreshold)
                {
                    kx022_normalHitDone_flag = 1U;
                }

                kx022State = KX022_STATE_HEALTH_CHECK;
            }
        }
        break;

        case KX022_STATE_HEALTH_CHECK:
        {
            if (kx022_transmit(KX022_COTR_REG, KX022_READ, KX022_COTR_RESPONSE) != KX022_COTR_RESPONSE)
            {
                kx022State = KX022_STATE_ERROR;
            }
            else
            {
                kx022State = KX022_STATE_FETCH_X_DATA;
            }
        }
        break;

        case KX022_STATE_ERROR:
        {
            kx022_errorCnt++;
            if (kx022_errorCnt < 5U)
            {
                kx022State = KX022_STATE_PRE_STARTUP;
                stateCnt = 0;
                gpio_pinState_set(control.port, control.pin, GPIO_LOW);
            }
            else
            {
                gu8_kx022_error_flag = 1;
            }
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }

    LOG('m', 'm', "[%d] mems: %-6d  %-6d  %-6d   calib: %-6d  %-6d  %-6d   max: %-6d  %-6d  %-6d   hitsThr:%d  hitsX:%d  hitsY:%d  hitsZ:%d  cHitsThr:%d  cHits:%d  %d  %d  %d", kx022State, mems_xyz[0], mems_xyz[1], mems_xyz[2], mems_xyzCalibrated[0], mems_xyzCalibrated[1], mems_xyzCalibrated[2], mems_max[0], mems_max[1], mems_max[2], gu8_kx022_hitThreshold, gu8_unbalanceXHitCnt, gu8_unbalanceYHitCnt, gu8_unbalanceZHitCnt, gu8_kx022_criticalHitThreshold, gu8_unbalanceCriticalHitCnt, xThreshold, yThreshold, zThreshold);
}

uint8_t kx022_unbalanceFlag_get(void)
{
    return gu8_kx022_unbalance_flag;
}

void kx022_xThreshold_set(int32_t value)
{
    xThreshold = value;
}

int32_t kx022_xThreshold_get(void)
{
    return xThreshold;
}

void kx022_yThreshold_set(int32_t value)
{
    yThreshold = value;
}

void kx022_zThreshold_set(int32_t value)
{
    zThreshold = value;
}

int16_t kx022_faultValue_get(kx022_faultDirection_t direction)
{
    return mems_max[direction];
}

void kx022_hitCnt_set(uint8_t value)
{
    gu8_kx022_hitThreshold = value;
}

void kx022_critHitCnt_set(uint8_t value)
{
    gu8_kx022_criticalHitThreshold = value;
}

void kx022_unbalanceFlag_set(uint8_t value)
{
    gu8_kx022_unbalance_flag = value;
}

void kx022_unbalanceDetection_set(uint8_t value)
{
    kx022_detectionEnable_flag = value;
}

void kx022_unbalanceCnt_reset(void)
{
    gu8_unbalanceXHitCnt = 0;
    gu8_unbalanceYHitCnt = 0;
    gu8_unbalanceZHitCnt = 0;
    gu8_unbalanceCriticalHitCnt = 0;
    kx022_normalHitDone_flag = 0;
    mems_prevXYZ[0] = (int16_t)mems_xyzCalibrated[0];
    mems_prevXYZ[1] = (int16_t)mems_xyzCalibrated[1];
    mems_prevXYZ[2] = (int16_t)mems_xyzCalibrated[2];
}

uint8_t kx022_error_get(void)
{
    return gu8_kx022_error_flag;
}

int16_t kx022_xValue_get(void)
{
    return mems_xyz[0];
}

int16_t kx022_yValue_get(void)
{
    return mems_xyz[1];
}

int16_t kx022_zValue_get(void)
{
    return mems_xyz[2];
}

int16_t kx022_xCalibratedValue_get(void)
{
    return (int16_t)mems_xyzCalibrated[0];
}

int16_t kx022_yCalibratedValue_get(void)
{
    return (int16_t)mems_xyzCalibrated[1];
}

int16_t kx022_zCalibratedValue_get(void)
{
    return (int16_t)mems_xyzCalibrated[2];
}

uint8_t kx022_currentHitCnt_get(void)
{
    return gu16_unbalanceSumHitCnt;
}

uint8_t kx022_currentCritHitCnt_get(void)
{
    return gu8_unbalanceCriticalHitCnt;
}

static void kx022_configuration_start(void)
{
    (void)kx022_transmit(KX022_CNTL1_REG, KX022_WRITE, KX022_CNTL1_CONFIG_CHANGE_ENABLE);
    (void)kx022_transmit(KX022_CNTL2_REG, KX022_WRITE, KX022_CNTL2_RAM_RESET);
}

static kx022_error_t kx022_configuration_continue_0(void)
{
    kx022_error_t ret = KX022_AK;

    if (kx022_transmit(KX022_WHO_AM_I_REG, KX022_READ, KX022_WHO_AM_I_RESPONSE) != KX022_WHO_AM_I_RESPONSE)
    {
        ret = KX022_NAK;
    }

    if (kx022_transmit(KX022_COTR_REG, KX022_READ, KX022_COTR_RESPONSE) != KX022_COTR_RESPONSE)
    {
        ret = KX022_NAK;
    }

    return ret;
}
static void kx022_configuration_continue_1(void)
{
    (void)kx022_transmit(KX022_ODCNTL_REG, KX022_WRITE, KX022_ODCNTL_OUTPUT_DATA_RATE_SET);
    (void)kx022_transmit(KX022_CNTL1_REG, KX022_WRITE, KX022_CNTL1_OPERATION_2G_START);
}

static void kx022_accelRawX_get(int16_t *buff)
{
    sspi_csState_set(csONLevel);
    (void)sspi_data_transmit((uint8_t)(~(uint8_t)((uint8_t)KX022_X_OUT_L_REG | (uint8_t)KX022_READ)));
    buff[0] = abs((int16_t)(((int16_t)sspi_data_transmit(0xff)) | ((int16_t)sspi_data_transmit(0xff) << 8)));
    sspi_csState_set(!csONLevel);
}
static void kx022_accelRawY_get(int16_t *buff)
{
    sspi_csState_set(csONLevel);
    (void)sspi_data_transmit((uint8_t)(~(uint8_t)((uint8_t)KX022_Y_OUT_L_REG | (uint8_t)KX022_READ)));
    buff[1] = abs((int16_t)(((int16_t)sspi_data_transmit(0xff)) | ((int16_t)sspi_data_transmit(0xff) << 8)));
    sspi_csState_set(!csONLevel);
}
static void kx022_accelRawZ_get(int16_t *buff)
{
    sspi_csState_set(csONLevel);
    (void)sspi_data_transmit((uint8_t)(~(uint8_t)((uint8_t)KX022_Z_OUT_L_REG | (uint8_t)KX022_READ)));
    buff[2] = abs((int16_t)(((int16_t)sspi_data_transmit(0xff)) | ((int16_t)sspi_data_transmit(0xff) << 8)));
    sspi_csState_set(!csONLevel);
}

static uint8_t kx022_transmit(kx022_internalReg_t reg, kx022_readWrite_t readWrite, uint8_t value)
{
    uint8_t ret = 0;

    sspi_csState_set(csONLevel);
    (void)sspi_data_transmit(~(reg | readWrite));

    if (readWrite == KX022_WRITE)
    {
        ret = ~(sspi_data_transmit(~(value)));
    }
    else if (readWrite == KX022_READ)
    {
        ret = ~(sspi_data_transmit(0xff));
    }
    else
    {
        // do nothing
    }

    sspi_csState_set(!csONLevel);

    kx022_dummy_delay(200);

    return ret;
}

static void kx022_dummy_delay(uint32_t del)
{
    for (volatile uint32_t i = 0; i < del; i++)
    {
        // do nothing
    }
}
