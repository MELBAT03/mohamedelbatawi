#include "handlers/output/inc/handler_clutch_hm15n.h"
#include "board.h"

#include "def.h"
#include "components/doutput_module/doutput_module.h"
#include "os.h"
#include "components/hm15n/hm15n.h"

#define CLUTCH_MOTOR_ERROR_TIME_MS         (20 * 1000u)
#define FINPUT_CLUTCH_SENSOR_UPDATE_PERIOD (100)

static hm15n_handle clutchHandle;

#ifdef COMP_MOCKER_ENABLE
static uint8_t gu8_mock_clutchError;
#endif

void handler_clutch_init(void)
{
    hm15n_config_t clutch;

    clutch.hm15n.gpio.port = CLUTCH_PORT;
    clutch.hm15n.gpio.pin = CLUTCH_PIN;
    clutch.hm15n.gpio.state = HM15N_INACTIVE;
    clutch.errorInterval = CLUTCH_MOTOR_ERROR_TIME_MS;

    clutch.sensor.timerModule = CLUTCH_SENSOR_CHANNEL;
    clutch.sensor.gpio.port = CLUTCH_SENSOR_PORT;
    clutch.sensor.gpio.pin = CLUTCH_SENSOR_PIN;
    clutch.sensor.edge = CLUTCH_SENSOR_DETECTION_EDGE;
    clutch.sensor.updatePeriod = FINPUT_CLUTCH_SENSOR_UPDATE_PERIOD;

    clutchHandle = hm15n_init(&clutch);
}

handler_clutch_state_t handler_clutch_switch(handler_clutch_state_t state)
{
#ifdef COMP_MOCKER_ENABLE
    hm15n_state_set(clutchHandle, (hm_15n_state_t)state);
    (void)hm15n_state_get(clutchHandle);
    return state;
#else
    handler_clutch_state_t ret = HANDLER_CLUTCH_PROCESSING;

    hm15n_state_set(clutchHandle, (hm_15n_state_t)state);

    ret = (handler_clutch_state_t)hm15n_state_get(clutchHandle);

    return ret;
#endif
}

uint8_t handler_clutch_error_get(void)
{
#ifdef COMP_MOCKER_ENABLE
    (void)hm15n_error_get(clutchHandle);
    return gu8_mock_clutchError;
#else
    return hm15n_error_get(clutchHandle);
#endif
}

void handler_clutch_error_set(uint8_t state)
{
#ifdef COMP_MOCKER_ENABLE
    hm15n_error_set(clutchHandle, state);
    gu8_mock_clutchError = state;
#else
    hm15n_error_set(clutchHandle, state);
#endif
}
