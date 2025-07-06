#include "components/hm14n/hm14n.h"
#include "def.h"
#include "os.h"
#include "component_config.h"

typedef struct hm14n_internalHandler
{
    doutputModule_handle hm14nHandle;

    hm_14n_state_t commandedState;
    hm_14n_state_t actualState;
} hm14n_internalHandler_t;

static hm14n_internalHandler_t gx_hm14n_internalHandler[HM14N_MODULE_DEVICE_NUM];

static uint8_t gu8_handlerItemsCounter = 0;

hm14n_handle hm14n_init(hm14n_config_t *hm)
{
    hm14n_handle handle = NULL;
    doutputModule_t hm14n;

    if (hm != NULL && gu8_handlerItemsCounter < HM14N_MODULE_DEVICE_NUM)
    {
        gx_hm14n_internalHandler[gu8_handlerItemsCounter].commandedState = HM14N_IDLE;
        gx_hm14n_internalHandler[gu8_handlerItemsCounter].actualState = HM14N_IDLE;

        hm14n.gpio.port = hm->hm14n.gpio.port;
        hm14n.gpio.pin = hm->hm14n.gpio.pin;
        hm14n.gpio.state = hm->hm14n.gpio.state;
        hm14n.gpio.ioState = GPIO_OUTPUT;

        gx_hm14n_internalHandler[gu8_handlerItemsCounter].hm14nHandle = doutputModule_init(&hm14n);

        handle = &gx_hm14n_internalHandler[gu8_handlerItemsCounter];
        gu8_handlerItemsCounter++;
    }

    return handle;
}

void hm14n_update(uint32_t period)
{
    static uint32_t onDelay = 0;

    for (uint8_t i = 0; i < gu8_handlerItemsCounter; i++)
    {
        switch (gx_hm14n_internalHandler[i].actualState)
        {
            case HM14N_IDLE:
            {
                if (gx_hm14n_internalHandler[i].commandedState == HM14N_ACTIVE)
                {
                    gx_hm14n_internalHandler[i].actualState = HM14N_PROCESSING;
                }
                else if (gx_hm14n_internalHandler[i].commandedState == HM14N_INACTIVE)
                {
                    gx_hm14n_internalHandler[i].actualState = HM14N_PROCESSING;
                }
            }
            break;

            case HM14N_ACTIVE:
            {
                if (gx_hm14n_internalHandler[i].commandedState == HM14N_INACTIVE)
                {
                    gx_hm14n_internalHandler[i].actualState = HM14N_PROCESSING;
                }
            }
            break;

            case HM14N_INACTIVE:
            {
                if (gx_hm14n_internalHandler[i].commandedState == HM14N_ACTIVE)
                {
                    gx_hm14n_internalHandler[i].actualState = HM14N_PROCESSING;
                }
            }
            break;

            case HM14N_PROCESSING:
            {
                if (gx_hm14n_internalHandler[i].commandedState == HM14N_INACTIVE)
                {
                    doutputModule_state_set(gx_hm14n_internalHandler[i].hm14nHandle, HM14N_INACTIVE);
                    gx_hm14n_internalHandler[i].actualState = HM14N_INACTIVE;
                }
                else if (gx_hm14n_internalHandler[i].commandedState == HM14N_ACTIVE)
                {
                    doutputModule_state_set(gx_hm14n_internalHandler[i].hm14nHandle, HM14N_ACTIVE);
                    if (onDelay >= 8000)
                    {
                        gx_hm14n_internalHandler[i].actualState = HM14N_ACTIVE;
                        onDelay = 0;
                    }
                    else
                    {
                        onDelay += period;
                    }
                }
            }
            break;

            case HM14N_RESET:
            {
                // do nothing
                // kept for compitability with hm15n
            }
            break;

            default:
            {
                // do nothing
            }
            break;
        }
    }
}

void hm14n_state_set(hm14n_handle hm14n, hm_14n_state_t state)
{
    if (hm14n != NULL)
    {
        hm14n->commandedState = state;
    }
}

hm_14n_state_t hm14n_state_get(hm14n_handle hm14n)
{
    hm_14n_state_t ret = HM14N_INACTIVE;

    if (hm14n != NULL)
    {
        ret = hm14n->actualState;
    }

    return ret;
}

void hm14n_currentState_set(hm14n_handle hm14n, hm_14n_state_t value)
{
    if (hm14n != NULL)
    {
        hm14n->actualState = value;
    }
}

uint8_t hm14n_error_get(hm14n_handle hm14n)
{
    // do nothing
    // kept for compitability with hm15n
    return 0;
}

void hm14n_error_set(hm14n_handle hm14n, uint8_t state)
{
    // do nothing
    // kept for compitability with hm15n
}
