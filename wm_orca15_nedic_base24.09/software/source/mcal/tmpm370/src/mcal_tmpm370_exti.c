#include "mcal.h"

void mcal_exti_init(mcal_exti_t *config)
{
    CG_ClearINTReq((CG_INTSrc)config->intNumber);
    GPIO_EnableFuncReg(config->port, GPIO_FUNC_REG_1, config->pin);
    GPIO_SetInputEnableReg(config->port, config->pin, ENABLE);
    CG_SetSTBYReleaseINTSrc((CG_INTSrc)config->intNumber, (CG_INTActiveState)config->detectionType, ENABLE);

    switch (config->intNumber)
    {
        case MCAL_EXTI_IRQ0:
        {
            NVIC_ClearPendingIRQ(INT0_IRQn);
            NVIC_EnableIRQ(INT0_IRQn);
        }
        break;

        case MCAL_EXTI_IRQ1:
        {
            NVIC_ClearPendingIRQ(INT1_IRQn);
            NVIC_EnableIRQ(INT1_IRQn);
        }
        break;

        case MCAL_EXTI_IRQ2:
        {
            NVIC_ClearPendingIRQ(INT2_IRQn);
            NVIC_EnableIRQ(INT2_IRQn);
        }
        break;

        case MCAL_EXTI_IRQ3:
        {
            NVIC_ClearPendingIRQ(INT3_IRQn);
            NVIC_EnableIRQ(INT3_IRQn);
        }
        break;

        case MCAL_EXTI_IRQ4:
        {
            NVIC_ClearPendingIRQ(INT4_IRQn);
            NVIC_EnableIRQ(INT4_IRQn);
        }
        break;

        case MCAL_EXTI_IRQ5:
        {
            NVIC_ClearPendingIRQ(INT5_IRQn);
            NVIC_EnableIRQ(INT5_IRQn);
        }
        break;

        case MCAL_EXTI_IRQ6:
        {
            NVIC_ClearPendingIRQ(INT6_IRQn);
            NVIC_EnableIRQ(INT6_IRQn);
        }
        break;

        case MCAL_EXTI_IRQ7:
        {
            NVIC_ClearPendingIRQ(INT7_IRQn);
            NVIC_EnableIRQ(INT7_IRQn);
        }
        break;

        case MCAL_EXTI_IRQ8:
        {
            NVIC_ClearPendingIRQ(INT8_IRQn);
            NVIC_EnableIRQ(INT8_IRQn);
        }
        break;

        case MCAL_EXTI_IRQ9:
        {
            NVIC_ClearPendingIRQ(INT9_IRQn);
            NVIC_EnableIRQ(INT9_IRQn);
        }
        break;

        case MCAL_EXTI_IRQA:
        {
            NVIC_ClearPendingIRQ(INTA_IRQn);
            NVIC_EnableIRQ(INTA_IRQn);
        }
        break;

        case MCAL_EXTI_IRQB:
        {
            NVIC_ClearPendingIRQ(INTB_IRQn);
            NVIC_EnableIRQ(INTB_IRQn);
        }
        break;

        case MCAL_EXTI_IRQC:
        {
            NVIC_ClearPendingIRQ(INTC_IRQn);
            NVIC_EnableIRQ(INTC_IRQn);
        }
        break;

        case MCAL_EXTI_IRQD:
        {
            NVIC_ClearPendingIRQ(INTD_IRQn);
            NVIC_EnableIRQ(INTD_IRQn);
        }
        break;

        case MCAL_EXTI_IRQE:
        {
            NVIC_ClearPendingIRQ(INTE_IRQn);
            NVIC_EnableIRQ(INTE_IRQn);
        }
        break;

        case MCAL_EXTI_IRQF:
        {
            NVIC_ClearPendingIRQ(INTF_IRQn);
            NVIC_EnableIRQ(INTF_IRQn);
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}

void mcal_exti_InterruptState_set(mcal_exti_irqNumber_t irq, mcal_exti_state_t intState)
{
    // do nothing
}

mcal_exti_flagStatus_t mcal_exti_requestFlag_query(mcal_exti_irqNumber_t irq)
{
    return MCAL_EXTI_NO_INT_REQUEST_OCCURED;
}

void mcal_exti_requestFlag_clear(mcal_exti_irqNumber_t irq)
{
    CG_ClearINTReq((CG_INTSrc)irq);
}
