#include "components/hm15n/hm15n.h"
#include "def.h"
#include "os.h"
#include "component_config.h"

#include "components/finput_module/finput_module.h"

typedef struct hm15n_internalHandler
{
    doutputModule_handle doutputHandle;

    hm_15n_state_t commandedState;
    hm_15n_state_t actualState;

    uint32_t errorInterval;
    uint8_t error_flag;
} hm15n_internalHandler_t;

static hm15n_sensor_state_t hm15n_sensorState_get(finputModule_handle finputModule);

static hm15n_internalHandler_t gx_hm15n_internalHandler[HM15N_MODULE_DEVICE_NUM];

static finputModule_handle gx_finput;

hm15n_handle hm15n_init(hm15n_config_t *hm)
{
    hm15n_handle handle = NULL;
    doutputModule_t hm15n;
    finputModule_t sensor;
    static uint8_t handlerItemsCounter = 0;

    if (hm != NULL && handlerItemsCounter < HM15N_MODULE_DEVICE_NUM)
    {
        gx_hm15n_internalHandler[handlerItemsCounter].actualState = HM15N_IDLE;
        gx_hm15n_internalHandler[handlerItemsCounter].commandedState = HM15N_IDLE;
        gx_hm15n_internalHandler[handlerItemsCounter].errorInterval = hm->errorInterval;
        gx_hm15n_internalHandler[handlerItemsCounter].error_flag = 0;

        hm15n.gpio.port = hm->hm15n.gpio.port;
        hm15n.gpio.pin = hm->hm15n.gpio.pin;
        hm15n.gpio.state = hm->hm15n.gpio.state;
        gx_hm15n_internalHandler[handlerItemsCounter].doutputHandle = doutputModule_init(&hm15n);

        sensor.timerModule = hm->sensor.timerModule;
        sensor.gpio.port = hm->sensor.gpio.port;
        sensor.gpio.pin = hm->sensor.gpio.pin;
        sensor.edge = hm->sensor.edge;
        sensor.updatePeriod = hm->sensor.updatePeriod;

        gx_finput = finputModule_init(&sensor);

        handle = &gx_hm15n_internalHandler[handlerItemsCounter];
        handlerItemsCounter++;
    }

    return handle;
}

void hm15n_update(uint32_t period)
{
    static uint32_t onDelay = 0, offDelay = 0;
    static uint16_t u16_hm15n_errorTimer = 0;
    hm15n_sensor_state_t sensorState;

    for (uint8_t i = 0; i < HM15N_MODULE_DEVICE_NUM; i++)
    {
        sensorState = hm15n_sensorState_get(gx_finput);

        switch (gx_hm15n_internalHandler[i].actualState)
        {
            case HM15N_IDLE:
            {
                if (gx_hm15n_internalHandler[i].commandedState == HM15N_ACTIVE)
                {
                    if (sensorState == HM15N_SENSOR_ACTIVE)
                    {
                        gx_hm15n_internalHandler[i].actualState = HM15N_RESET;
                    }
                    else
                    {
                        gx_hm15n_internalHandler[i].actualState = HM15N_PROCESSING;
                    }
                }
                else if (gx_hm15n_internalHandler[i].commandedState == HM15N_INACTIVE)
                {
                    if (sensorState == HM15N_SENSOR_ACTIVE)
                    {
                        gx_hm15n_internalHandler[i].actualState = HM15N_PROCESSING;
                    }
                    else
                    {
                        gx_hm15n_internalHandler[i].actualState = HM15N_INACTIVE;
                    }
                }
            }
            break;

            case HM15N_ACTIVE:
            {
                if (gx_hm15n_internalHandler[i].commandedState == HM15N_INACTIVE)
                {
                    gx_hm15n_internalHandler[i].actualState = HM15N_PROCESSING;
                }
            }
            break;

            case HM15N_INACTIVE:
            {
                if (gx_hm15n_internalHandler[i].commandedState == HM15N_ACTIVE)
                {
                    gx_hm15n_internalHandler[i].actualState = HM15N_PROCESSING;
                }
            }
            break;

            case HM15N_PROCESSING:
            {
                doutputModule_state_set(gx_hm15n_internalHandler[i].doutputHandle, HM15N_ACTIVE);

                if (gx_hm15n_internalHandler[i].commandedState == HM15N_ACTIVE)
                {
                    offDelay = 0;
                    if (sensorState == HM15N_SENSOR_ACTIVE)
                    {
                        u16_hm15n_errorTimer = 0;
                        onDelay += period;
                        if (onDelay >= 6500)
                        {
                            doutputModule_state_set(gx_hm15n_internalHandler[i].doutputHandle, HM15N_INACTIVE);
                            gx_hm15n_internalHandler[i].actualState = HM15N_ACTIVE;
                            onDelay = 0;
                        }
                    }
                    else if (sensorState == HM15N_SENSOR_INACTIVE)
                    {
                        u16_hm15n_errorTimer += period;
                        if (u16_hm15n_errorTimer >= gx_hm15n_internalHandler[i].errorInterval)
                        {
                            u16_hm15n_errorTimer = 0;
                            gx_hm15n_internalHandler[i].error_flag = 1;
                        }
                    }
                }
                else if (gx_hm15n_internalHandler[i].commandedState == HM15N_INACTIVE)
                {
                    onDelay = 0;
                    if (sensorState == HM15N_SENSOR_INACTIVE)
                    {
                        u16_hm15n_errorTimer = 0;
                        offDelay += period;
                        if (offDelay >= 1000)
                        {
                            doutputModule_state_set(gx_hm15n_internalHandler[i].doutputHandle, HM15N_INACTIVE);
                            gx_hm15n_internalHandler[i].actualState = HM15N_INACTIVE;
                            offDelay = 0;
                        }
                    }
                    else if (sensorState == HM15N_SENSOR_ACTIVE)
                    {
                        u16_hm15n_errorTimer += period;
                        if (u16_hm15n_errorTimer >= gx_hm15n_internalHandler[i].errorInterval)
                        {
                            u16_hm15n_errorTimer = 0;
                            gx_hm15n_internalHandler[i].error_flag = 1;
                        }
                    }
                }
            }
            break;

            case HM15N_RESET:
            {
                doutputModule_state_set(gx_hm15n_internalHandler[i].doutputHandle, HM15N_ACTIVE);
                if (sensorState == HM15N_SENSOR_INACTIVE)
                {
                    offDelay += period;
                    if (offDelay >= 1000)
                    {
                        doutputModule_state_set(gx_hm15n_internalHandler[i].doutputHandle, HM15N_INACTIVE);
                        gx_hm15n_internalHandler[i].actualState = HM15N_INACTIVE;
                    }
                }
            }
            break;

            default:
            {
                // do nothing
            }
            break;
        }

        LOG('c', 'v', "act %d  cmd %d  snr %d  err %d", gx_hm15n_internalHandler[i].actualState, gx_hm15n_internalHandler[i].commandedState, sensorState, gx_hm15n_internalHandler[i].error_flag);
    }
}

void hm15n_state_set(hm15n_handle hm15n, hm_15n_state_t state)
{
    hm15n->commandedState = state;
}

hm_15n_state_t hm15n_state_get(hm15n_handle hm15n)
{
    return hm15n->actualState;
}

void hm15n_currentState_set(hm15n_handle hm15n, hm_15n_state_t value)
{
    hm15n->actualState = value;
}

uint8_t hm15n_error_get(hm15n_handle hm15n)
{
    return hm15n->error_flag;
}

void hm15n_error_set(hm15n_handle hm15n, uint8_t state)
{
    hm15n->error_flag = state;
}

static hm15n_sensor_state_t hm15n_sensorState_get(finputModule_handle finputModule)
{
    hm15n_sensor_state_t ret;
    uint32_t tmp;

    tmp = finputModule_value_get(finputModule);
    tmp *= 10;

    if (tmp < 10)
    {
        ret = HM15N_SENSOR_INACTIVE;
    }
    else
    {
        ret = HM15N_SENSOR_ACTIVE;
    }

    return ret;
}
