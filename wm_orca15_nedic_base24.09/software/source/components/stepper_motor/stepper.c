/*****************************************************************************************************
*                                       Hardware Connections
*                                    **************************			          		      
*                                      P1_4  ---------->  IN4
*                                      P1_5  ---------->  IN3
*							           P1_6  ---------->  IN2
*                                      P1_7  ---------->  IN1
*****************************************************************************************************/

#include "stepper.h"

#define STEPS_PER_REV (6000U)
#define STEPS_PER_ONE_DEGREE (17U)
#define STEPPER_MOTOR_UPDATE_PERIOD_MS (5U)
#define OS_TICK_MS (1U)

typedef struct
{
    stepper_directionEnum_t stepperMotor_dir;
    uint32_t stepperMotor_steps;
    uint16_t stepperMotor_reqAngle;
    uint16_t stepperMotor_actAngle;
    uint16_t stepperMoter_speed;
} gx_stepperMotor_t;

static int16_t steps_index = 0;

uint8_t cwRotation[4] = {0x50, 0x60, 0xA0, 0x90};
uint8_t ccwRotation[4] = {0x90, 0xA0, 0x60, 0x50};

gx_stepperMotor_t gx_stepperMotor;
uint8_t gx_stepperMotor_time = 0;

static void stepperTurn_CW(void);
static void stepperTurn_CCW(void);

void stepperMotor_init(stepperMotor_t *stepper)
{
    uint8_t i;
    gx_stepperMotor.stepperMotor_dir = STEPPER_DIRECTION_CW;
    gx_stepperMotor.stepperMotor_steps = 0;
    gx_stepperMotor.stepperMotor_actAngle = 0;
    gx_stepperMotor.stepperMotor_reqAngle = 0;
    gx_stepperMotor.stepperMoter_speed = 5;
    gx_stepperMotor_time = 0;
    for (i = 0; i < 4; i++)
    {
        // Note : Set the pins as output directly
        gpio_pin_init(&(stepper->stepper_gpioPins[i]));
    }
}

void stepperMotor_update(uint32_t period)
{
    static uint8_t i = 0;
    static int32_t angle = 0;

    gx_stepperMotor_time += OS_TICK_MS;
    if (gx_stepperMotor_time != gx_stepperMotor.stepperMoter_speed)
    {
        return;
    }
    gx_stepperMotor_time = 0;

    if (gx_stepperMotor.stepperMotor_actAngle < gx_stepperMotor.stepperMotor_reqAngle)
    {
        gx_stepperMotor.stepperMotor_dir = STEPPER_DIRECTION_CW;
        GPIO_WriteData(GPIO_PB, cwRotation[i]);
        i++;

        gx_stepperMotor.stepperMotor_steps--;
        if (gx_stepperMotor.stepperMotor_steps % STEPS_PER_ONE_DEGREE == 0)
        {
            gx_stepperMotor.stepperMotor_actAngle++;
        }

        if (i >= 4)
        {
            i = 0;
        }
    }
    else if (gx_stepperMotor.stepperMotor_actAngle > gx_stepperMotor.stepperMotor_reqAngle)
    {
        gx_stepperMotor.stepperMotor_dir = STEPPER_DIRECTION_CCW;
        GPIO_WriteData(GPIO_PB, ccwRotation[i]);
        i++;

        gx_stepperMotor.stepperMotor_steps--;
        if (gx_stepperMotor.stepperMotor_steps % STEPS_PER_ONE_DEGREE == 0)
        {
            gx_stepperMotor.stepperMotor_actAngle--;
        }

        if (i >= 4)
        {
            i = 0;
        }
    }
    else
    {
        GPIO_WriteData(GPIO_PB, 0x00);
        i = 0;
        gx_stepperMotor.stepperMotor_actAngle = gx_stepperMotor.stepperMotor_reqAngle;
    }
}

void stepperMotor_setAngle(uint8_t angle)
{
    if (angle != gx_stepperMotor.stepperMotor_reqAngle)
    {
        gx_stepperMotor.stepperMotor_reqAngle = angle;
        if (gx_stepperMotor.stepperMotor_actAngle < gx_stepperMotor.stepperMotor_reqAngle)
        {
            gx_stepperMotor.stepperMotor_steps = (gx_stepperMotor.stepperMotor_reqAngle - gx_stepperMotor.stepperMotor_actAngle) * STEPS_PER_ONE_DEGREE;
        }
        else if (gx_stepperMotor.stepperMotor_actAngle > gx_stepperMotor.stepperMotor_reqAngle)
        {
            gx_stepperMotor.stepperMotor_steps = (gx_stepperMotor.stepperMotor_actAngle - gx_stepperMotor.stepperMotor_reqAngle) * STEPS_PER_ONE_DEGREE;
        }
    }
}

void stepperMotor_defaultParameters_set(uint8_t angle)
{
    gx_stepperMotor.stepperMotor_actAngle = angle;
}

uint32_t stepperMotor_steps_get(void)
{
    return gx_stepperMotor.stepperMotor_steps;
}

void stepperMotor_setSpeed(uint16_t speed)
{
    gx_stepperMotor.stepperMoter_speed = speed;
}

void stepperMotor_setState(stepper_directionEnum_t direction, uint16_t steps)
{
    gx_stepperMotor.stepperMotor_dir = direction;
    gx_stepperMotor.stepperMotor_steps = steps;
}
