/****************************************************************************
 * Title                 :   Blower Motor
 * Filename              :   blower_motor.h
 * Author                :   Iman Khairy
 * Origin Date           :   Dec 4, 2023
 * Version               :   1.0.0
 *
 *****************************************************************************/
/*************** INTERFACE NOTES **************************************
 * !important to read before use this module.
 * NOTES: 1- accordong to the lower layer, the interface uses the finput module
 *           to use counter timer for frequency input catch and use pwm to generate
 *           signals with different duty.
 *        2- you have to call the update function of the finput module to get
 *           the updated frequency.
 *        3- accordong to the upper layer, this interface is used by app_blower_motor
 *           in handler output layer and the app layer.
 *
 *****************************************************************************/
/** @file blower_motor.h
 *  @brief The interface definition for the blower motor.
 *
 *  This is the header file for the definition of the interface for a blower motor.
 */

#ifndef SOURCE_COMPONENTS_BLOWER_MOTOR_BLOWER_MOTOR_H_
#define SOURCE_COMPONENTS_BLOWER_MOTOR_BLOWER_MOTOR_H_

/******************************************************************************
 * Includes
 *******************************************************************************/
#include "mcal.h"                          // used for generating the required pwm signal.
                                           // contain blower IDs needed
#include "component_config.h"              // contain blower configurations needed
#include "finput_module/finput_module.h"   // used for the speed of the input

/******************************************************************************
 * Typedefs
 *******************************************************************************/
typedef struct
{
    gpio_port_t freqPort;
    gpio_pin_t freqPin;
    mcal_pwm_t channel;
    uint32_t freq;
    uint8_t duty;
    finputModule_t inputConfig;
} blower_motor_config_t;

void blower_motor_init(blower_motor_config_t *blower_ptrConfig);
void blower_motor_power_set(uint8_t duty_value);
uint32_t blower_motor_speed_get(void);

/* These APIs for testing */
uint8_t blower_motor_power_get(void);
void blower_motor_speed_set(uint32_t speed_value);

#endif /* SOURCE_COMPONENTS_BLOWER_MOTOR_BLOWER_MOTOR_H_ */
