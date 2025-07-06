#include "def.h"
#include "mcal.h"
#include "os.h"
#include "component_config.h"

#include "components/zcd/zcd.h"

#define ONE_SEC_IN_MSEC            (1000U)
#define FREQ_50_HZ_LOWER_THRESHOLD (190U)
#define FREQ_50_HZ_UPPER_THRESHOLD (210U)
#define FREQ_60_HZ_LOWER_THRESHOLD (215U)
#define FREQ_60_HZ_UPPER_THRESHOLD (235U)

#define FREQ_50_HZ (50U)
#define FREQ_60_HZ (60U)

static void zcd_isr(void);

static zcd_sate_t gx_zcd_state = ZCD_DETECTION_NOK;

static uint8_t gu8_zcd_minDetectionCycleCnt;

static uint32_t gu32_zcd_isrCnt = 0;
static uint32_t gu32_zcd_updateCnt = 0;
static uint8_t gu32_zcd_freq = 0;
static uint32_t zcdDetectedFreqCnt = 0;

static uint8_t gu8_initDone_flag = 0;
static mcal_exti_irqNumber_t gx_irqNumber;

void (*zcd_init(zcd_config_t *config))(void)
{
    static void (*isr)(void) = NULL;
    mcal_exti_t irqconfig;

    mcal_gpio_pin_init(&(config->pin));

    gu8_zcd_minDetectionCycleCnt = config->minDetectionCycleCnt;
    gu8_initDone_flag = 1;

    gx_irqNumber = config->irqReq;

    irqconfig.port = config->pin.port;
    irqconfig.pin = config->pin.pin;
    irqconfig.detectionType = config->edge;
    irqconfig.intNumber = config->irqPinNumber;

    mcal_exti_init(&irqconfig);

    isr = zcd_isr;

    return isr;
}

// NOTE: this is called from within the interrupt
static void zcd_isr(void)
{
    static volatile uint32_t gu32_zcd_waitCnt = 0;

    if (gu32_zcd_waitCnt > gu8_zcd_minDetectionCycleCnt)
    {
        gx_zcd_state = ZCD_DETECTION_OK;
        gu32_zcd_isrCnt++;
    }
    else
    {
        gu32_zcd_waitCnt++;
    }

    zcdDetectedFreqCnt++;

    mcal_exti_requestFlag_clear(gx_irqNumber);
}

void zcd_update(uint32_t period)
{
    static uint32_t zcdDetectedFreqPeriodMSec = 0;
    static uint8_t zcd_faultCnt = 0;

    if (0 != gu8_initDone_flag)
    {
        gu32_zcd_updateCnt++;

        LOG('z', 'v', "freqCnt : %d  freq : %d    update Cnt : %d    zcd Cnt : %d    sync_flag : %d", zcdDetectedFreqCnt, gu32_zcd_freq, gu32_zcd_updateCnt, gu32_zcd_isrCnt, gx_zcd_state);

        if ((ZCD_DETECTION_NOK != gx_zcd_state) && (gu32_zcd_isrCnt >= gu32_zcd_updateCnt))
        {
            gu32_zcd_isrCnt = 0;
            gu32_zcd_updateCnt = 0;
        }
        else if (gu32_zcd_updateCnt > gu32_zcd_isrCnt)
        {
            zcd_faultCnt++;
            gu32_zcd_updateCnt = 0;
            gu32_zcd_isrCnt = 0;

            if (zcd_faultCnt > 2)
            {
                zcd_faultCnt = 0;
                gx_zcd_state = ZCD_DETECTION_NOK;
                gu32_zcd_isrCnt = 0;
                gu32_zcd_updateCnt = 0;
            }
        }
        else
        {
            // do nothing
        }

        zcdDetectedFreqPeriodMSec += period;
        if (zcdDetectedFreqPeriodMSec >= ONE_SEC_IN_MSEC)
        {
            zcdDetectedFreqPeriodMSec = 0;
            if (zcdDetectedFreqCnt > FREQ_50_HZ_LOWER_THRESHOLD && zcdDetectedFreqCnt <= FREQ_50_HZ_UPPER_THRESHOLD)
            {
                gu32_zcd_freq = FREQ_50_HZ;
            }
            else if (zcdDetectedFreqCnt > FREQ_60_HZ_LOWER_THRESHOLD && zcdDetectedFreqCnt <= FREQ_60_HZ_UPPER_THRESHOLD)
            {
                gu32_zcd_freq = FREQ_60_HZ;
            }
            else
            {
                gu32_zcd_freq = 0;
            }
            zcdDetectedFreqCnt = 0;
        }
    }
}

zcd_sate_t zcd_state_get(void)
{
    return gx_zcd_state;
}

uint8_t zcd_freq_get(void)
{
    return gu32_zcd_freq;
}
