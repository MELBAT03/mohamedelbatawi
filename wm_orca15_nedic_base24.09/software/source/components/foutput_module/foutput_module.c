#include "mcal.h"
#include "def.h"
#include "component_config.h"
#include "foutput_module/foutput_module.h"

typedef struct
{
    foutputModuleInterface_t pwmInterface;
    mcal_gpio_portEnum_t port;
    mcal_gpio_pinEnum_t pin;
    uint32_t freq;
    uint32_t duty;
    uint32_t period;
    uint32_t internalCounter;
    uint8_t type;
    uint8_t foutputID;
    uint8_t enable;
    uint8_t changeRequest;
    uint8_t periodDone;
} foutputModule_internalHandler_t;

static mcal_pwmConfig_t gx_pwmConfig;
static foutputModule_internalHandler_t gax_foutputModule_internalHandler[FOUTPUT_MODULE_DEVCIES_NUM];
static uint8_t gu8_outputModule_internalCounter = 0;

void foutputModule_init(foutputModule_t *x_foutputModule)
{
    gax_foutputModule_internalHandler[gu8_outputModule_internalCounter].port = x_foutputModule->port;
    gax_foutputModule_internalHandler[gu8_outputModule_internalCounter].pin = x_foutputModule->pin;
    gax_foutputModule_internalHandler[gu8_outputModule_internalCounter].foutputID = x_foutputModule->foutputID;
    gax_foutputModule_internalHandler[gu8_outputModule_internalCounter].period = 0;
    gax_foutputModule_internalHandler[gu8_outputModule_internalCounter].internalCounter = 0;
    gax_foutputModule_internalHandler[gu8_outputModule_internalCounter].changeRequest = 1;
    gax_foutputModule_internalHandler[gu8_outputModule_internalCounter].enable = x_foutputModule->enable;
    gax_foutputModule_internalHandler[gu8_outputModule_internalCounter].freq = x_foutputModule->freq;
    gax_foutputModule_internalHandler[gu8_outputModule_internalCounter].duty = x_foutputModule->duty;
    gax_foutputModule_internalHandler[gu8_outputModule_internalCounter].pwmInterface = x_foutputModule->pwmInterface;
    gax_foutputModule_internalHandler[gu8_outputModule_internalCounter].periodDone = 0;

    // mcal_pwm_channel_set(x_foutputModule->pwmInterface, &gx_pwmConfig);
    // mcal_pwm_channel_enable(x_foutputModule->pwmInterface);

    gu8_outputModule_internalCounter++;
}

void foutputModule_freq_set(uint8_t foutputID, uint32_t freq, uint32_t duty, uint32_t period)
{
    uint8_t i;

    for (i = 0; i < gu8_outputModule_internalCounter; i++)
    {
        if (gax_foutputModule_internalHandler[i].foutputID == foutputID)
        {
            if (gax_foutputModule_internalHandler[i].freq != freq)
            {
                gax_foutputModule_internalHandler[i].freq = freq;
                gax_foutputModule_internalHandler[i].changeRequest = 1;
            }

            if (gax_foutputModule_internalHandler[i].duty != duty)
            {
                gax_foutputModule_internalHandler[i].duty = duty;
                gax_foutputModule_internalHandler[i].changeRequest = 1;
            }

            if (gax_foutputModule_internalHandler[i].period != period)
            {
                gax_foutputModule_internalHandler[i].period = period;
                gax_foutputModule_internalHandler[i].changeRequest = 1;
            }
        }
    }
}

void foutputModule_state_set(uint8_t foutputID, uint8_t state)
{
    uint8_t i;

    for (i = 0; i < gu8_outputModule_internalCounter; i++)
    {
        if (gax_foutputModule_internalHandler[i].foutputID == foutputID)
        {
            gax_foutputModule_internalHandler[i].enable = state;
        }
    }
}

uint8_t foutputModule_state_get(uint8_t foutputID)
{
    uint8_t i;

    for (i = 0; i < gu8_outputModule_internalCounter; i++)
    {
        if (gax_foutputModule_internalHandler[i].foutputID == foutputID)
        {
            return gax_foutputModule_internalHandler[i].enable;
        }
    }
}

void foutputModule_update(uint32_t period)
{
    uint8_t i;

    for (i = 0; i < gu8_outputModule_internalCounter; i++)
    {
        if (gax_foutputModule_internalHandler[i].enable == 1)
        {
            mcal_pwm_channel_enable(gax_foutputModule_internalHandler[i].pwmInterface);
            // gax_foutputModule_internalHandler[i].internalCounter += period;
            // if (gax_foutputModule_internalHandler[i].internalCounter >= gax_foutputModule_internalHandler[i].period)
            // {
            //     gax_foutputModule_internalHandler[i].internalCounter = 0;
            //     // gax_foutputModule_internalHandler[i].enable = 0;
            //     gax_foutputModule_internalHandler[i].periodDone = 1;
            //     // mcal_pwm_channel_disable(gax_foutputModule_internalHandler[i].pwmInterface);
            // }
        }
        else
        {
            mcal_pwm_channel_disable(gax_foutputModule_internalHandler[i].pwmInterface);
        }

        if (gax_foutputModule_internalHandler[i].changeRequest == 1 && gax_foutputModule_internalHandler[i].enable == 1)
        {
            gx_pwmConfig.duty = gax_foutputModule_internalHandler[i].duty;
            gx_pwmConfig.freq = gax_foutputModule_internalHandler[i].freq;
            gx_pwmConfig.pin = gax_foutputModule_internalHandler[i].pin;
            gx_pwmConfig.port = gax_foutputModule_internalHandler[i].port;

            mcal_pwm_channel_set(gax_foutputModule_internalHandler[i].pwmInterface, &gx_pwmConfig);
            gax_foutputModule_internalHandler[i].changeRequest = 0;
        }
    }
}
