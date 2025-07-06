#include "def.h"
#include "acin.h"
#include "os_log.h"
#include "component_config.h"

#include "components/ainput_module/ainput_module.h"

#define ACIN_STARTUP_TIMEOUT_MSEC (5000U)

static uint8_t (*relation)(uint32_t measurement, uint32_t ref) = NULL;

static uint32_t gu32_acin_underVoltageRefValue;
static uint32_t gu32_acin_underVoltageTimeOutRefMSec;

static acin_state_t gx_acin_acState = ACIN_STARTUP_TIMEOUT;
static uint32_t gu32_acValue = 0;

#if ACIN_TYPE == ACIN_METHOD_ADC
static ainputModule_handle gx_acin;
#elif ACIN_TYPE == ACIN_METHOD_TIME
static mcal_timer_t *gx_tm;
static uint32_t result;
#endif

void acin_init(acinConfig_t *config)
{
#if ACIN_TYPE == ACIN_METHOD_ADC
    gx_acin = ainputModule_init(&(config->ain));
#elif ACIN_TYPE == ACIN_METHOD_TIME
    mcal_timer_timerModeUS_init(config->tm, 1500);
    gx_tm = config->tm;
#endif

    gu32_acin_underVoltageRefValue = config->underVoltageValue;
    gu32_acin_underVoltageTimeOutRefMSec = config->underVoltageErrorTimeoutMSec;

    relation = config->relation;
}

void acin_update(uint32_t period)
{
    static uint32_t timeoutMSec = 0;

#if ACIN_TYPE == ACIN_METHOD_ADC
    gu32_acValue = (gu32_acValue >> 1) + (ainputModule_value_get(gx_acin) >> 1);
#elif ACIN_TYPE == ACIN_METHOD_TIME
    gu32_acValue = (gu32_acValue >> 1) + (result >> 1);
#endif

    switch (gx_acin_acState)
    {
        case ACIN_STARTUP_TIMEOUT:
        {
            timeoutMSec += period;
            if (timeoutMSec >= ACIN_STARTUP_TIMEOUT_MSEC)
            {
                timeoutMSec = 0;
                gx_acin_acState = ACIN_NORMAL_VOLTAGE;
            }
        }
        break;

        case ACIN_NORMAL_VOLTAGE:
        {
            if ((relation != NULL) && (0U != relation(gu32_acValue, gu32_acin_underVoltageRefValue)))
            {
                gx_acin_acState = ACIN_MAYBE_POWER_DIPPING;
            }
        }
        break;

        case ACIN_MAYBE_POWER_DIPPING:
        case ACIN_UNDER_VOLTAGE:
        {
            if ((relation != NULL) && (0U != relation(gu32_acValue, gu32_acin_underVoltageRefValue)))
            {
                timeoutMSec += period;
                if (timeoutMSec >= gu32_acin_underVoltageTimeOutRefMSec)
                {
                    timeoutMSec = 0;
                    gx_acin_acState++;
                }
            }
            else
            {
                timeoutMSec = 0;
                gx_acin_acState = ACIN_NORMAL_VOLTAGE;
            }
        }
        break;

        case ACIN_DANGEROUS_UNDER_VOLTAGE:
        default:
        {
            // do nothing
        }
        break;
    }

    LOG('a', 'v', "acValue:%d  state:%d", gu32_acValue, gx_acin_acState);
}

uint32_t acin_value_get(void)
{
    return gu32_acValue;
}

acin_state_t acin_state_get(void)
{
    return gx_acin_acState;
}

void acin_calculation_start(void)
{
#if ACIN_TYPE == ACIN_METHOD_TIME
    mcal_timer_timerState_set(gx_tm, MCAL_TIMER_STOP);
    mcal_timer_timerFlag_clear(gx_tm);
    mcal_timer_timerState_set(gx_tm, MCAL_TIMER_START);
#endif
}

void acin_calculation_record(void)
{
#if ACIN_TYPE == ACIN_METHOD_TIME
    result = mcal_timer_timerCounter_get(gx_tm) / mcal_timer_timerModeUS_conversionFactor_get(gx_tm);
    mcal_timer_timerState_set(gx_tm, MCAL_TIMER_STOP);
#endif
}
