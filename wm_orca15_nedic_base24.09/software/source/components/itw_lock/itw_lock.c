#include "components/itw_lock/itw_lock.h"
#include "components/dinput_module/dinput_module.h"
#include "def.h"
#include "os.h"

static dinputModule_t sensor;
static dinputModule_handle gx_sensor;

static itwLock_state_t gx_itwLock_commandedState = ITW_LOCK_INIT;
static itwLock_state_t gx_itwLock_actualState = ITW_LOCK_INIT;
static uint8_t gu8_itwLock_error_flag = 0;
static uint8_t gu8_itwLock_sensorState;
static uint32_t gu32_itwLock_errorInterval = 0;

static doutputModule_handle gx_lock;

void itwLock_init(itwLock_config_t *itwLock)
{
    doutputModule_t lock;

    lock.gpio.port = itwLock->lock.gpio.port;
    lock.gpio.pin = itwLock->lock.gpio.pin;
    lock.gpio.state = itwLock->lock.gpio.state;

    gu32_itwLock_errorInterval = itwLock->errorInterval;

    gx_lock = doutputModule_init(&lock);

    sensor.type = itwLock->sensor.type;
    sensor.activeLevel = itwLock->sensor.activeLevel;
    sensor.gpio.port = itwLock->sensor.gpio.port;
    sensor.gpio.pin = itwLock->sensor.gpio.pin;
    sensor.shortPressMultiplier = itwLock->sensor.shortPressMultiplier;
    sensor.longPressMultiplier = itwLock->sensor.longPressMultiplier;

    gx_sensor = dinputModule_init(&sensor);
}

void itwLock_update(uint32_t period)
{
    static uint16_t errorCnt = 0;
    static uint32_t resetTimeout = 0;

    gu8_itwLock_sensorState = itwLock_sensorCurrentState_get();

    switch (gx_itwLock_actualState)
    {
        case ITW_LOCK_INIT:
        {
            if (gx_itwLock_commandedState != ITW_LOCK_INIT)
            {
                if (gu8_itwLock_sensorState == sensor.activeLevel)
                {
                    doutputModule_state_set(gx_lock, ITW_LOCK_ON);
                    gx_itwLock_actualState = ITW_LOCK_TRANSITION;
                }
                else
                {
                    gx_itwLock_actualState = ITW_LOCK_INACTIVE;
                }
            }
        }
        break;

        case ITW_LOCK_ACTIVE:
        {
            if ((gx_itwLock_commandedState == ITW_LOCK_INACTIVE) || (gu8_itwLock_sensorState != sensor.activeLevel))
            {
                gx_itwLock_actualState = ITW_LOCK_TRANSITION;
                doutputModule_state_set(gx_lock, ITW_LOCK_ON);
            }
        }
        break;

        case ITW_LOCK_INACTIVE:
        {
            if ((gx_itwLock_commandedState == ITW_LOCK_ACTIVE) || (gu8_itwLock_sensorState == sensor.activeLevel))
            {
                gx_itwLock_actualState = ITW_LOCK_TRANSITION;
                doutputModule_state_set(gx_lock, ITW_LOCK_ON);
            }
        }
        break;

        case ITW_LOCK_TRANSITION:
        {
            doutputModule_state_set(gx_lock, ITW_LOCK_OFF);
            gx_itwLock_actualState = ITW_LOCK_PROCESSING;
            resetTimeout = 2000U;
        }
        break;

        case ITW_LOCK_PROCESSING:
        {
            if (gu8_itwLock_sensorState != sensor.activeLevel && gx_itwLock_commandedState == ITW_LOCK_INACTIVE)
            {
                gx_itwLock_actualState = ITW_LOCK_INACTIVE;
                resetTimeout = 0;
                errorCnt = 0;
            }
            else if (gu8_itwLock_sensorState == sensor.activeLevel && gx_itwLock_commandedState == ITW_LOCK_INACTIVE)
            {
                resetTimeout -= period;
                if (resetTimeout == 0)
                {
                    gx_itwLock_actualState = ITW_LOCK_ACTIVE;
                }
                errorCnt++;
            }
            if (gu8_itwLock_sensorState == sensor.activeLevel && gx_itwLock_commandedState == ITW_LOCK_ACTIVE)
            {
                gx_itwLock_actualState = ITW_LOCK_ACTIVE;
                resetTimeout = 0;
                errorCnt = 0;
            }
            else if (gu8_itwLock_sensorState != sensor.activeLevel && gx_itwLock_commandedState == ITW_LOCK_ACTIVE)
            {
                resetTimeout -= period;
                if (resetTimeout == 0)
                {
                    gx_itwLock_actualState = ITW_LOCK_INACTIVE;
                }
                errorCnt++;
            }
            else
            {
                // do nothing
            }

            if (errorCnt >= 6U)
            {
                gu8_itwLock_error_flag = 1;
            }
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
    // }

    LOG('i', 'v', "act:%d  cmd:%d  snr:%d  err:%d", gx_itwLock_actualState, gx_itwLock_commandedState, gu8_itwLock_sensorState, gu8_itwLock_error_flag);
}

void itwLock_state_set(itwLock_state_t state)
{
    gx_itwLock_commandedState = state;
}

itwLock_state_t itwLock_currentState_get(void)
{
    return gx_itwLock_actualState;
}

void itwLock_currentState_set(itwLock_state_t value)
{
    gx_itwLock_actualState = value;
}

uint8_t itwLock_sensorCurrentState_get(void)
{
    uint8_t tmp;

    tmp = dinputModule_state_get(gx_sensor);

    return tmp;
}

uint8_t itwLock_error_get(void)
{
    return gu8_itwLock_error_flag;
}

void itwLock_error_set(uint8_t state)
{
    gu8_itwLock_error_flag = state;
}
