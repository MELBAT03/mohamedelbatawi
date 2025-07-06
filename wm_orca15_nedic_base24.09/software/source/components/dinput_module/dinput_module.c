#include "component_config.h"
#include "dinput_module.h"
#include "gpio/gpio.h"

static uint8_t dinputModule_level_check(uint32_t keyNum);

typedef struct dinputModule_internalHandler_t
{
    dinputModule_t button;
    uint32_t activeStateCnt;
    dinputModule_stateEnum_t activeState;
    dinputModule_stateEnum_t edgeActiveState;
    dinputModule_stateEnum_t onReleaseState;
} dinputModule_internalHandler_t;

static dinputModule_internalHandler_t gx_dinputModule_internalHandler[DINPUT_MODULE_DEVICE_NUM];
static uint8_t gu8_handlerItemsCounter = 0;

dinputModule_handle dinputModule_init(dinputModule_t *dinputModule)
{
    // :TODO:  add pull up code

    dinputModule_handle handler = NULL;

    if (dinputModule != NULL && gu8_handlerItemsCounter < DINPUT_MODULE_DEVICE_NUM)
    {
        gx_dinputModule_internalHandler[gu8_handlerItemsCounter].button.type = dinputModule->type;
        if (dinputModule->type == DINPUT_GPIO_INPUT)
        {
            gx_dinputModule_internalHandler[gu8_handlerItemsCounter].button.gpio.port = dinputModule->gpio.port;
            gx_dinputModule_internalHandler[gu8_handlerItemsCounter].button.gpio.pin = dinputModule->gpio.pin;
            gx_dinputModule_internalHandler[gu8_handlerItemsCounter].button.pullUpEn = dinputModule->pullUpEn;

            gpio_pinIOState_set(gx_dinputModule_internalHandler[gu8_handlerItemsCounter].button.gpio.port, gx_dinputModule_internalHandler[gu8_handlerItemsCounter].button.gpio.pin, GPIO_INPUT);
        }
        else if (dinputModule->type == DINPUT_REG_INPUT)
        {
            gx_dinputModule_internalHandler[gu8_handlerItemsCounter].button.reg = dinputModule->reg;
        }
        else
        {
            // do nothing
        }

        gx_dinputModule_internalHandler[gu8_handlerItemsCounter].button.activeLevel = dinputModule->activeLevel;

        gx_dinputModule_internalHandler[gu8_handlerItemsCounter].activeStateCnt = 0;
        gx_dinputModule_internalHandler[gu8_handlerItemsCounter].activeState = DINPUT_MODULE_RELEASED;
        gx_dinputModule_internalHandler[gu8_handlerItemsCounter].edgeActiveState = DINPUT_MODULE_RELEASED;
        gx_dinputModule_internalHandler[gu8_handlerItemsCounter].onReleaseState = DINPUT_MODULE_RELEASED;

        if (dinputModule->shortPressMultiplier == 0UL)
        {
            dinputModule->shortPressMultiplier = 1;
        }
        if (dinputModule->longPressMultiplier < dinputModule->shortPressMultiplier)
        {
            dinputModule->longPressMultiplier = dinputModule->shortPressMultiplier;
        }
        gx_dinputModule_internalHandler[gu8_handlerItemsCounter].button.shortPressMultiplier = dinputModule->shortPressMultiplier;
        gx_dinputModule_internalHandler[gu8_handlerItemsCounter].button.longPressMultiplier = dinputModule->longPressMultiplier;

        handler = &gx_dinputModule_internalHandler[gu8_handlerItemsCounter];
        gu8_handlerItemsCounter++;
    }

    return handler;
}

void dinputModule_update(uint32_t period)
{
    for (uint32_t index = 0; index < gu8_handlerItemsCounter; index++)
    {
        switch (gx_dinputModule_internalHandler[index].activeState)
        {
            case DINPUT_MODULE_RELEASED:
                if (dinputModule_level_check(index) == gx_dinputModule_internalHandler[index].button.activeLevel)
                {
                    gx_dinputModule_internalHandler[index].activeStateCnt++;
                    if (gx_dinputModule_internalHandler[index].activeStateCnt >= gx_dinputModule_internalHandler[index].button.shortPressMultiplier)
                    {
                        gx_dinputModule_internalHandler[index].activeState = DINPUT_MODULE_PRESSED_SHORT_PRESS;
                    }
                }
                else
                {
                    gx_dinputModule_internalHandler[index].activeStateCnt = 0;
                }
                break;

            case DINPUT_MODULE_PRESSED_SHORT_PRESS:
                if (dinputModule_level_check(index) == gx_dinputModule_internalHandler[index].button.activeLevel)
                {
                    gx_dinputModule_internalHandler[index].activeStateCnt++;
                    if (gx_dinputModule_internalHandler[index].button.longPressMultiplier != UINT32_MAX && (gx_dinputModule_internalHandler[index].activeStateCnt >= gx_dinputModule_internalHandler[index].button.longPressMultiplier))
                    {
                        gx_dinputModule_internalHandler[index].activeState = DINPUT_MODULE_PRESSED_LONG_PRESS;
                        gx_dinputModule_internalHandler[index].edgeActiveState = DINPUT_MODULE_PRESSED_LONG_PRESS;
                        gx_dinputModule_internalHandler[index].activeStateCnt = 0;
                    }
                }
                else
                {
                    gx_dinputModule_internalHandler[index].activeState = DINPUT_MODULE_RELEASED;
                    gx_dinputModule_internalHandler[index].activeStateCnt = 0;
                    gx_dinputModule_internalHandler[index].onReleaseState = DINPUT_MODULE_PRESSED_SHORT_PRESS;
                    gx_dinputModule_internalHandler[index].edgeActiveState = DINPUT_MODULE_PRESSED_SHORT_PRESS;
                }
                break;

            case DINPUT_MODULE_PRESSED_LONG_PRESS:
                if (dinputModule_level_check(index) == gx_dinputModule_internalHandler[index].button.activeLevel)
                {
                    gx_dinputModule_internalHandler[index].activeState = DINPUT_MODULE_PRESSED_LONG_PRESS;
                }
                else
                {
                    gx_dinputModule_internalHandler[index].activeState = DINPUT_MODULE_RELEASED;
                    gx_dinputModule_internalHandler[index].activeStateCnt = 0;
                    gx_dinputModule_internalHandler[index].onReleaseState = DINPUT_MODULE_PRESSED_LONG_PRESS;
                }
                break;

            default:
                // do nothing
                break;
        }
    }
}

dinputModule_stateEnum_t dinputModule_state_get(dinputModule_handle handle)
{
    dinputModule_stateEnum_t ret = DINPUT_MODULE_FAULT;

    if (handle != NULL)
    {
        ret = handle->activeState;
    }

    return ret;
}

dinputModule_stateEnum_t dinputModule_edgeState_get(dinputModule_handle handle)
{
    dinputModule_stateEnum_t ret = DINPUT_MODULE_FAULT;

    if (handle != NULL)
    {
        ret = handle->edgeActiveState;
        handle->edgeActiveState = DINPUT_MODULE_RELEASED;
    }

    return ret;
}

dinputModule_stateEnum_t dinputModule_onReleaseState_get(dinputModule_handle handle)
{
    dinputModule_stateEnum_t ret = DINPUT_MODULE_FAULT;

    if (handle != NULL)
    {
        ret = handle->onReleaseState;
        handle->edgeActiveState = DINPUT_MODULE_RELEASED;
        handle->onReleaseState = DINPUT_MODULE_RELEASED;
    }

    return ret;
}

void dinputModule_shortPressMultiplier_set(dinputModule_handle handle, uint32_t newMultiplier)
{
    if (handle != NULL)
    {
        handle->button.shortPressMultiplier = newMultiplier;
    }
}

void dinputModule_longPressMultiplier_set(dinputModule_handle handle, uint32_t newMultiplier)
{
    if (handle != NULL)
    {
        handle->button.longPressMultiplier = newMultiplier;
    }
}

static uint8_t dinputModule_level_check(uint32_t keyNum)
{
    uint8_t ret = !gx_dinputModule_internalHandler[keyNum].button.activeLevel;
    uint32_t buff = 0;

    if (gx_dinputModule_internalHandler[keyNum].button.type == DINPUT_GPIO_INPUT)
    {
        ret = mcal_gpio_pinState_get(&(gx_dinputModule_internalHandler[keyNum].button.gpio));
    }
    else if (gx_dinputModule_internalHandler[keyNum].button.type == DINPUT_REG_INPUT)
    {
        buff = *(gx_dinputModule_internalHandler[keyNum].button.reg);
        buff &= (1 << keyNum);
        ret = (uint8_t)(buff >> keyNum);
    }
    else
    {
        // do nothing
    }

    return ret;
}
