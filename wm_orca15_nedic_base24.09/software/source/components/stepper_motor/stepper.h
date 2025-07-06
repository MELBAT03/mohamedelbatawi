/********************************************************************************************************
* Title                 :   Stepper Motor
* Filename              :   Stepper.h
* Author                :   AbdelAziz Moustafa
* Origin Date           :   11/07/2020
* Version               :   1.0.0
* Brief                 :   This module contains definitions and function prototypes for stepper motor
* 
********************************************************************************************************/

#ifndef STEPPER_H_
#define STEPPER_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include "mcal.h"
#include "gpio/gpio.h"

/******************************************************************************
*								Constants
*******************************************************************************/
typedef uint8_t stepper_directionEnum_t;
#define STEPPER_DIRECTION_CW (0U)
#define STEPPER_DIRECTION_CCW (1U)

/******************************************************************************
*								Typedefs
*******************************************************************************/
typedef struct
{
    gpio_t stepper_gpioPins[4];               /* GPIO pins connected to the H-bridge */
    stepper_directionEnum_t stepperDirection; /* Direction of rotation: CW or CCW */
    uint8_t stepperDeviceID;
} stepperMotor_t;

/******************************************************************************
*						 Public Function Prototypes
*******************************************************************************/
void stepperMotor_init(stepperMotor_t *stepper);
void stepperMotor_setState(stepper_directionEnum_t direction, uint16_t steps);
void stepperMotor_setAngle_dir(stepper_directionEnum_t direction, uint8_t angle);
void stepperMotor_setAngle(uint8_t angle);
void stepperMotor_defaultParameters_set(uint8_t angle);
void stepperMotor_setSpeed(uint16_t speed);
uint32_t stepperMotor_steps_get(void);
void stepperMotor_update(uint32_t period);

#endif /* STEPPER_H_ */