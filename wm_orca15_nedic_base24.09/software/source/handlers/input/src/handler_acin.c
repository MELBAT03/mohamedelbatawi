#include "def.h"
#include "os.h"
#include "board.h"

#include "app_memory.h"

#include "handler_acin.h"
#include "components/acin/acin.h"

// NOTE: calibration must be done with the power from e2 disabled
#define ACIN_UNDER_VOLTAGE_ADC_VALUE (1000U)

#define ACIN_UNDER_VOLTAGE_ERROR_TIMEOUT_MSEC (300000U)

static uint8_t handler_acin_relation_compare(uint32_t measurement, uint32_t ref);

#ifdef COMP_MOCKER_ENABLE
static uint32_t gu32_mock_acinValue;
static handler_acin_state_t gu32_mock_acinState;
#endif

void handler_acin_init(void)
{
    acinConfig_t acin;

    // acin.ain.adcBlock = ACIN_ADC_BLK;
    // acin.ain.adcCh = ACIN_ADC_CH;
    // acin.ain.gpio.port = ACIN_ADC_PORT;
    // acin.ain.gpio.pin = ACIN_ADC_PIN;
    acin.tm = ACIN_TIME_TIMER;

    acin.underVoltageValue = app_memory_lVoltageData_get();
    acin.underVoltageErrorTimeoutMSec = ACIN_UNDER_VOLTAGE_ERROR_TIMEOUT_MSEC;

    acin.relation = &handler_acin_relation_compare;

    acin_init(&acin);
}

uint32_t handler_acin_value_get(void)
{
#ifdef COMP_MOCKER_ENABLE
    (void)acin_value_get();
    return gu32_mock_acinValue;
#else
    return acin_value_get();
#endif
}

void handler_acin_value_set(uint32_t value)
{
#ifdef COMP_MOCKER_ENABLE
    gu32_mock_acinValue = value;
#endif
}

handler_acin_state_t handler_acin_state_get(void)
{
#ifdef COMP_MOCKER_ENABLE
    (void)acin_state_get();
    return gu32_mock_acinState;
#else
    return acin_state_get();
#endif
}

void handler_acin_state_set(handler_acin_state_t value)
{
#ifdef COMP_MOCKER_ENABLE
    gu32_mock_acinState = value;
#endif
}

uint32_t handler_acin_defaultUnderVoltageValue_get(void)
{
    return ACIN_UNDER_VOLTAGE_ADC_VALUE;
}

static uint8_t handler_acin_relation_compare(uint32_t measurement, uint32_t ref)
{
    uint8_t ret = 0;

    if (measurement > ref)
    {
        ret = 1;
    }

    return ret;
}
