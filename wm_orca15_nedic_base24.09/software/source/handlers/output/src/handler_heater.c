#include "def.h"
#include "board.h"

#include "handlers/output/inc/handler_heater.h"
#include "components/ptc_heater/ptc_heater.h"

static double ptcHeater_equation(double value);

static ptcHeater_handle heater1;
static ptcHeater_handle heater2;

#ifdef COMP_MOCKER_ENABLE
static double gd_mock_heaterFeedback;
#endif

void handler_heater_init(void)
{
    ptcHeater_config_t heater1_config;
    ptcHeater_config_t heater2_config;

    heater1_config.plate.gpio.port = HEATER_PLATE_1_PORT;
    heater1_config.plate.gpio.pin = HEATER_PLATE_1_PIN;
    heater1_config.plate.gpio.state = HEATER_STATE_OFF;

    heater1_config.feedback = PTC_HEATER_FEEDBACK_AVAILABLE;
    heater1_config.feedbackEq = &ptcHeater_equation;
    heater1_config.minADCValue = 0;
    heater1_config.maxADCValue = 2500;

    heater1_config.FB.adcBlock = HEATER_SENSOR_ADC_BLK;
    heater1_config.FB.adcCh = HEATER_SENSOR_ADC_CH;
    heater1_config.FB.gpio.port = HEATER_SENSOR_PORT;
    heater1_config.FB.gpio.pin = HEATER_SENSOR_PIN;

    heater1 = ptcHeater_init(&heater1_config);

    heater2_config.plate.gpio.port = HEATER_PLATE_2_PORT;
    heater2_config.plate.gpio.pin = HEATER_PLATE_2_PIN;
    heater2_config.plate.gpio.state = HEATER_STATE_OFF;

    heater2_config.feedback = PTC_HEATER_FEEDBACK_NOT_AVAILABLE;

    heater2 = ptcHeater_init(&heater2_config);
}
double handler_heater_feedback_get(void)
{
#ifdef COMP_MOCKER_ENABLE
    (void)ptcHeater_feedback_get(heater1);
    return gd_mock_heaterFeedback;
#else
    return ptcHeater_feedback_get(heater1);
#endif
}

void handler_heater_feedback_set(double value)
{
#ifdef COMP_MOCKER_ENABLE
    gd_mock_heaterFeedback = value;
#endif
}

void handler_heater_plate_set(handler_heater_state_t plateState)
{
    if (HANDLER_HEATER_OFF == plateState)
    {
        ptcHeater_plate_set(heater1, PTC_HEATER_OFF);
        ptcHeater_plate_set(heater2, PTC_HEATER_OFF);
    }
    else if (HANDLER_HEATER_PLATE_1 == plateState)
    {
        ptcHeater_plate_set(heater1, PTC_HEATER_ON);
        ptcHeater_plate_set(heater2, PTC_HEATER_OFF);
    }
    else if (HANDLER_HEATER_PLATE_2 == plateState)
    {
        ptcHeater_plate_set(heater1, PTC_HEATER_ON);
        ptcHeater_plate_set(heater2, PTC_HEATER_ON);
    }
    else
    {
        // do nothing
    }
}

static double ptcHeater_equation(double value)
{
    double ret = 0;

    if ((uint32_t)value <= 178U)   // in order to avoid getting negative values by the equation
    {
        ret = 0.0;
    }
    else
    {
        ret = (value * 0.0324) - 5.75;
    }

    return ret;
}
