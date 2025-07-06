/*
 * blower_motor.c
 *
 *  Created on: Dec 4, 2023
 *      Author: EKHAIR01
 */

/****************************************************************************
 * Title                 :   Blower Motor
 * Filename              :   blower_motor.h
 * Author                :   Iman Khairy
 * Origin Date           :   Dec 4, 2023
 * Version               :   1.0.0
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *******************************************************************************/
#include "blower_motor.h" /* For this modules interfaces */

/******************************************************************************
 * Macros
 *******************************************************************************/

#define BLOWER_MOTOR_SECOND_MSEC (1000UL)

#define BLOWER_SPEED_FEEDBACK_CONVERSION_FACTOR (5U)

/******************************************************************************
 * Typedefs
 *******************************************************************************/
typedef struct blower_internalHandler
{
    /* to store the configuration data that is needed */
    mcal_pwm_t channel;
    mcal_pwmConfig_t freqPin;
    uint32_t blowerFeedback_updatePeriod;
    /* for internal handling */
    uint32_t blowerMotor_speed;
} blower_internalHandler_t;

/******************************************************************************
 * Module Variable Declarations
 *******************************************************************************/
/**
 *  Declare internal handler to store the blower motor configuration and other needed internal data.
 */
static blower_internalHandler_t gx_blower_internalHandler;

/******************************************************************************
 * Module Variable Definitions
 *******************************************************************************/

/******************************************************************************
 * Functions prototype
 *******************************************************************************/

/******************************************************************************
 * Functions definition
 *******************************************************************************/
/******************************************************************************
 * Function : blower_motor_init()
 */
/**
 * \b Description:
 *
 * This function is used to initialize the blower based on the blower configuration
 * structure which is passed to the function.
 *
 * PRE-CONDITION: The MCU clocks must be configured and enabled.
 * PRE-CONDITION: The pointers of the input parameters not equal NULL.
 * PRE-CONDITION: the counter ID doesn't exceed the number that is defined.
 *
 * POST-CONDITION: The blower component is setup with the configuration settings.
 *
 * @param  		blower_ptrConfig is a pointer to the configuration structure that contains
 *				the initialization for the peripheral.
 *
 * @return 		void
 *******************************************************************************/

static finputModule_handle gx_finput;

void blower_motor_init(blower_motor_config_t *blower_ptrConfig)
{
    gx_blower_internalHandler.freqPin.port = blower_ptrConfig->freqPort;
    gx_blower_internalHandler.freqPin.pin = blower_ptrConfig->freqPin;
    gx_blower_internalHandler.freqPin.duty = blower_ptrConfig->duty;
    gx_blower_internalHandler.freqPin.freq = blower_ptrConfig->freq;
    gx_blower_internalHandler.channel = blower_ptrConfig->channel;

    mcal_pwm_channel_disable(gx_blower_internalHandler.channel);

    gx_blower_internalHandler.blowerMotor_speed = 0;

    gx_blower_internalHandler.blowerFeedback_updatePeriod = blower_ptrConfig->inputConfig.updatePeriod;
    gx_finput = finputModule_init(&blower_ptrConfig->inputConfig);
}

void blower_motor_power_set(uint8_t duty_value)
{
    if (gx_blower_internalHandler.freqPin.duty != duty_value)
    {
        gx_blower_internalHandler.freqPin.duty = duty_value;
        mcal_pwm_channel_disable(gx_blower_internalHandler.channel);
        mcal_pwm_channel_set(gx_blower_internalHandler.channel, &gx_blower_internalHandler.freqPin);

        if (0 < duty_value)
        {
            mcal_pwm_channel_enable(gx_blower_internalHandler.channel);
        }
        else
        {
            mcal_pwm_channel_disable(gx_blower_internalHandler.channel);
        }
    }
}

uint32_t blower_motor_speed_get(void)
{
    uint16_t samples_perSec;
    samples_perSec = BLOWER_MOTOR_SECOND_MSEC / gx_blower_internalHandler.blowerFeedback_updatePeriod;
    return (BLOWER_SPEED_FEEDBACK_CONVERSION_FACTOR * samples_perSec * finputModule_value_get(gx_finput));
}

/* These APIs for testing */
uint8_t blower_motor_power_get(void)
{
    return gx_blower_internalHandler.freqPin.duty;
}

void blower_motor_speed_set(uint32_t speed_value)
{
    gx_blower_internalHandler.blowerMotor_speed = speed_value;
}
