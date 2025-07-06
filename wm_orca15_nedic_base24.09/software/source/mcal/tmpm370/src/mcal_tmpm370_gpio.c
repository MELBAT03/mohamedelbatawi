#include "mcal.h"

void mcal_gpio_init(void)
{
    // do nothing
}

void mcal_gpio_pin_init(mcal_gpio_t *px_gpio)
{
    switch (px_gpio->ioState)
    {
        case MCAL_GPIO_INPUT:
        {
            GPIO_SetInput(px_gpio->port, px_gpio->pin);
            GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_1, px_gpio->pin);
            GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_2, px_gpio->pin);
        }
        break;

        case MCAL_GPIO_OUTPUT:
        {
            GPIO_SetOutput(px_gpio->port, px_gpio->pin);
            GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_1, px_gpio->pin);
            GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_2, px_gpio->pin);
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}

void mcal_gpio_pinState_set(mcal_gpio_t *px_gpio, mcal_gpio_stateEnum_t x_state)
{
    GPIO_WriteDataBit(px_gpio->port, px_gpio->pin, x_state);
}

void mcal_gpio_portState_set(mcal_gpio_t *px_gpio, mcal_gpio_stateEnum_t x_state)
{
    GPIO_WriteData(px_gpio->port, x_state);
}

uint8_t mcal_gpio_pinState_get(mcal_gpio_t *px_gpio)
{
    return GPIO_ReadDataBit(px_gpio->port, px_gpio->pin);
}

uint8_t mcal_gpio_portState_get(mcal_gpio_t *px_gpio)
{
    return GPIO_ReadData(px_gpio->port);
}

void mcal_gpio_pinIOState_set(mcal_gpio_t *px_gpio, mcal_gpio_ioStateEnum_t x_ioState)
{
    switch (x_ioState)
    {
        case MCAL_GPIO_INPUT:
            GPIO_SetInput(px_gpio->port, px_gpio->pin);
            break;

        case MCAL_GPIO_OUTPUT:
            GPIO_SetOutput(px_gpio->port, px_gpio->pin);
            break;

        default:
            // do nothing
            break;
    }
    GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_1, px_gpio->pin);
    GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_2, px_gpio->pin);
}

void mcal_gpio_pinState_toggle(mcal_gpio_t *px_gpio)
{
    static uint8_t p = 0;
    p = GPIO_ReadDataBit(px_gpio->port, px_gpio->pin);
    p ^= 1;
    if (p)
    {
        GPIO_WriteDataBit(px_gpio->port, px_gpio->pin, GPIO_BIT_VALUE_1);
    }
    else
    {
        GPIO_WriteDataBit(px_gpio->port, px_gpio->pin, GPIO_BIT_VALUE_0);
    }
}

void mcal_gpio_altFunction_set(mcal_gpio_t *px_gpio, mcal_gpio_alternateFnEnum_t x_func)
{
    switch (x_func)
    {
        case MCAL_GPIO_NORMAL:
            GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_1, px_gpio->pin);
            GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_2, px_gpio->pin);
            GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_3, px_gpio->pin);
            break;

        case MCAL_GPIO_ALT_FUNC_1:
            GPIO_EnableFuncReg(px_gpio->port, GPIO_FUNC_REG_1, px_gpio->pin);
            GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_2, px_gpio->pin);
            GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_3, px_gpio->pin);
            break;

        case MCAL_GPIO_ALT_FUNC_2:
            GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_1, px_gpio->pin);
            GPIO_EnableFuncReg(px_gpio->port, GPIO_FUNC_REG_2, px_gpio->pin);
            GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_3, px_gpio->pin);
            break;

        case MCAL_GPIO_ALT_FUNC_3:
            GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_1, px_gpio->pin);
            GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_2, px_gpio->pin);
            GPIO_EnableFuncReg(px_gpio->port, GPIO_FUNC_REG_3, px_gpio->pin);
            break;

        default:
            // do nothing
            break;
    }
}
