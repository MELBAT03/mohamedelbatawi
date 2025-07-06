#include "components/dls35s/dls35s.h"
#include "components/dinput_module/dinput_module.h"
#include "def.h"
#include "os.h"

static doutputModule_t dls35s;
static dinputModule_t sensor;
static dinputModule_handle gx_dlsSensor;

static dls35s_state_t gx_dls35s_commandedState = DLS35S_IDLE;
static dls35s_state_t gx_dls35s_actualState = DLS35S_INIT;
static uint8_t gu8_dls35s_error_flag = 0;
static uint32_t gu32_dls35s_sensorState;
static uint32_t gu32_dls35s_errorInterval = 0;

static doutputModule_handle gx_dls35s;

void dls35s_init(dls35s_config_t *dls)
{
    dls35s.gpio.port = dls->dls35s.gpio.port;
    dls35s.gpio.pin = dls->dls35s.gpio.pin;
    dls35s.gpio.state = dls->dls35s.gpio.state;

    gu32_dls35s_errorInterval = dls->errorInterval;

    gx_dls35s = doutputModule_init(&dls35s);

    sensor.type = dls->sensor.type;
    sensor.activeLevel = dls->sensor.activeLevel;
    sensor.gpio.port = dls->sensor.gpio.port;
    sensor.gpio.pin = dls->sensor.gpio.pin;
    sensor.shortPressMultiplier = dls->sensor.shortPressMultiplier;
    sensor.longPressMultiplier = dls->sensor.longPressMultiplier;

    gx_dlsSensor = dinputModule_init(&sensor);

    gx_dls35s_actualState = DLS35S_IDLE;
}

void dls35s_update(uint32_t period)
{
    static uint32_t onDelay = 0, offDelay = 0;
    static uint16_t u16_dls35s_errorTimer = 0;
    static uint32_t resetTimeout = 0;

    gu32_dls35s_sensorState = dls35s_sensorCurrentState_get();

    switch (gx_dls35s_actualState)
    {
        case DLS35S_INIT:
        {
            // do nothing
            // this is to confirm that init has been called before
        }
        break;

        case DLS35S_IDLE:
            gx_dls35s_actualState = DLS35S_RESET;
            break;

        case DLS35S_ACTIVE:
            if (gx_dls35s_commandedState == DLS35S_INACTIVE)
            {
                gx_dls35s_actualState = DLS35S_PROCESSING;
            }
            break;

        case DLS35S_INACTIVE:
            if (gx_dls35s_commandedState == DLS35S_ACTIVE)
            {
                gx_dls35s_actualState = DLS35S_PROCESSING;
            }
            break;

        case DLS35S_PROCESSING:
            doutputModule_state_set(gx_dls35s, DLS35S_ACTIVE);

            if (gx_dls35s_commandedState == DLS35S_ACTIVE)
            {
                if (gu32_dls35s_sensorState == sensor.activeLevel)
                {
                    u16_dls35s_errorTimer = 0;
                    onDelay += period;
                    if (onDelay >= 250)
                    {
                        doutputModule_state_set(gx_dls35s, DLS35S_INACTIVE);
                        gx_dls35s_actualState = DLS35S_ACTIVE;
                        onDelay = 0;
                    }
                }
                else if (gu32_dls35s_sensorState != sensor.activeLevel)
                {
                    u16_dls35s_errorTimer += period;
                    if (u16_dls35s_errorTimer >= gu32_dls35s_errorInterval)
                    {
                        u16_dls35s_errorTimer = 0;
                        gu8_dls35s_error_flag = 1;
                    }
                }
            }
            else if (gx_dls35s_commandedState == DLS35S_INACTIVE)
            {
                if (gu32_dls35s_sensorState != sensor.activeLevel)
                {
                    u16_dls35s_errorTimer = 0;
                    offDelay += period;
                    if (offDelay >= 800)
                    {
                        doutputModule_state_set(gx_dls35s, DLS35S_INACTIVE);
                        gx_dls35s_actualState = DLS35S_INACTIVE;
                        offDelay = 0;
                    }
                }
                else if (gu32_dls35s_sensorState == sensor.activeLevel)
                {
                    u16_dls35s_errorTimer += period;
                    if (u16_dls35s_errorTimer >= gu32_dls35s_errorInterval)
                    {
                        u16_dls35s_errorTimer = 0;
                        gu8_dls35s_error_flag = 1;
                    }
                }
            }
            break;

        case DLS35S_RESET:
            resetTimeout += period;
            doutputModule_state_set(gx_dls35s, DLS35S_ACTIVE);

            if (gu32_dls35s_sensorState == sensor.activeLevel)
            {
                offDelay = period;
            }
            else if (gu32_dls35s_sensorState != sensor.activeLevel && offDelay != 0)
            {
                offDelay += period;
                if (offDelay >= 800)
                {
                    doutputModule_state_set(gx_dls35s, DLS35S_INACTIVE);
                    gx_dls35s_actualState = DLS35S_INACTIVE;
                    offDelay = 0;
                }
            }

            // :NOTE: the mech part is in the way meaning the lid lock is inactive
            if (resetTimeout >= 3000 && gu32_dls35s_sensorState != sensor.activeLevel)
            {
                doutputModule_state_set(gx_dls35s, DLS35S_INACTIVE);
                gx_dls35s_actualState = DLS35S_INACTIVE;
            }
            break;

        default:
            // do nothing
            break;
    }
    // }
}

void dls35s_state_set(dls35s_state_t u8_state)
{
    gx_dls35s_commandedState = u8_state;
}

dls35s_state_t dls35s_currentState_get(void)
{
    return gx_dls35s_actualState;
}

void dls35s_currentState_set(dls35s_state_t value)
{
    gx_dls35s_actualState = value;
}

uint8_t dls35s_sensorCurrentState_get(void)
{
    uint32_t tmp;

    tmp = dinputModule_state_get(gx_dlsSensor);

    return tmp;
}

uint8_t dls35s_error_get(void)
{
    return gu8_dls35s_error_flag;
}

void dls35s_error_set(uint8_t state)
{
    gu8_dls35s_error_flag = state;
}
