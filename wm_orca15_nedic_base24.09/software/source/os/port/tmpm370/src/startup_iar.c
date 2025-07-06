/**************************************************
 *
 * This file contains an interrupt vector for Cortex-M written in C.
 * The actual interrupt functions must be provided by the application developer.
 *
 * Copyright 2007-2017 IAR Systems AB.
 *
 * $Revision$
 *
 **************************************************/

#pragma language = extended
#pragma segment = "CSTACK"

extern void __iar_program_start(void);
extern void SystemInit(void);

void dummy_isr(void);

extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);
extern void SVC_Handler(void);
extern void DebugMon_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);
void INTRX1_IRQHandler(void);
void INTRX2_IRQHandler(void);
void INTRX3_IRQHandler(void);
void INT1_IRQHandler(void);
void INTADAPDA_IRQHandler(void);
void INTTX0_IRQHandler(void);
void INTTX1_IRQHandler(void);
void INTTX2_IRQHandler(void);
void INTTX3_IRQHandler(void);
void INTRX0_IRQHandler(void);
void INTTB70_IRQHandler(void);
typedef void (*intfunc)(void);
typedef union
{
    intfunc __fun;
    void *__ptr;
} intvec_elem;

// The vector table is normally located at address 0.
// When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
// If you need to define interrupt service routines,
// make a copy of this file and include it in your project.
// The name "__vector_table" has special meaning for C-SPY, which
// is where to find the SP start value.
// If vector table is not located at address 0, the user has to initialize
// the  NVIC vector table register (VTOR) before using interrupts.

#pragma location = ".intvec"
const intvec_elem __vector_table[] =
    {
        {.__ptr = __sfe("CSTACK")},
        __iar_program_start,

        NMI_Handler,
        HardFault_Handler,
        MemManage_Handler,
        BusFault_Handler,
        UsageFault_Handler,
        0,
        0,
        0,
        0,
        SVC_Handler,
        DebugMon_Handler,
        0,
        PendSV_Handler,
        SysTick_Handler,
        dummy_isr,
        INT1_IRQHandler,
        dummy_isr,              // INT2:Int Pin (PH2/AINA2/94pin or 96pin)
        dummy_isr,              // INT3:Int Pin (PA0/TB0IN/2pin or 4pin)
        dummy_isr,              // INT4:Int Pin (PA2/TB1IN/4pin or 6pin)
        dummy_isr,              // INT5:Int Pin (PE4/TB2IN//15pin or 17pin)
        INTRX0_IRQHandler,      // INTRX0      ( 6 + EII) : Serial reception (channel.0)
        INTTX0_IRQHandler,      // INTTX0      ( 7 + EII) : Serial transmit (channel.0)
        INTRX1_IRQHandler,      // INTRX1      ( 8 + EII) : Serial reception (channel.1)
        INTTX1_IRQHandler,      // INTTX1      ( 9 + EII) : Serial transmit (channel.1)
        dummy_isr,              // INTVCNA     (10 + EII) : Vector Engine interrupt A
        dummy_isr,              // INTVCNB     (11 + EII) : Vector Engine interrupt B
        dummy_isr,              // INTEMG0     (12 + EII) : PMD0 EMG interrupt
        dummy_isr,              // INTEMG1     (13 + EII) : PMD1 EMG interrupt
        dummy_isr,              // INTOVV0     (14 + EII) : PMD0 OVV interrupt
        dummy_isr,              // INTOVV1     (15 + EII) : PMD1 OVV interrupt
        INTADAPDA_IRQHandler,   // INTAD0PDA:ADC0 triggered by PMD0 is finished
        dummy_isr,              // INTAD1PDA:ADC1 triggered by PMD0 is finished
        dummy_isr,              // INTAD0PDB   (18 + EII) : ADC0 conversion triggered by PMD1 is finished
        dummy_isr,              // INTAD1PDB   (19 + EII) : ADC1 conversion triggered by PMD1 is finished
        dummy_isr,              // INTTB00     (20 + EII) : 16bit TMRB0 compare match detection 0/ Over flow
        dummy_isr,              // INTTB01     (21 + EII) : 16bit TMRB0 compare match detection 1
        dummy_isr,              // INTTB10     (22 + EII) : 16bit TMRB1 compare match detection 0/ Over flow
        dummy_isr,              // INTTB11     (23 + EII) : 16bit TMRB1 compare match detection 1
        dummy_isr,              // INTTB40     (24 + EII) : 16bit TMRB4 compare match detection 0/ Over flow
        dummy_isr,              // INTTB41     (25 + EII) : 16bit TMRB4 compare match detection 1
        dummy_isr,              // INTTB50     (26 + EII) : 16bit TMRB5 compare match detection 0/ Over flow
        dummy_isr,              // INTTB51     (27 + EII) : 16bit TMRB5 compare match detection 1
        dummy_isr,              // INTPMD0     (28 + EII) : PMD0 PWM interrupt
        dummy_isr,              // INTPMD1     (29 + EII) : PMD1 PWM interrupt
        dummy_isr,              // INTCAP00    (30 + EII) : 16bit TMRB0 input capture 0
        dummy_isr,              // INTCAP01    (31 + EII) : 16bit TMRB0 input capture 1
        dummy_isr,              // INTCAP10    (32 + EII) : 16bit TMRB1 input capture 0
        dummy_isr,              // INTCAP11    (33 + EII) : 16bit TMRB1 input capture 1
        dummy_isr,              // INTCAP40    (34 + EII) : 16bit TMRB4 input capture 0
        dummy_isr,              // INTCAP41    (35 + EII) : 16bit TMRB4 input capture 1
        dummy_isr,              // INTCAP50    (36 + EII) : 16bit TMRB5 input capture 0
        dummy_isr,              // INTCAP51    (37 + EII) : 16bit TMRB5 input capture 1
        dummy_isr,              // INT6        (38 + EII) : Interrupt Pin (PE6/TB3IN//17pin or 19pin)
        dummy_isr,              // INT7        (39 + EII) : Interrupt Pin (PE7/TB3OUT/18pin or 20pin)
        INTRX2_IRQHandler,      // INTRX2      (40 + EII) : Serial reception (channel.2)
        INTTX2_IRQHandler,      // INTTX2      (41 + EII) : Serial transmit (channel.2)
        dummy_isr,              // INTAD0CPA   (42 + EII) : AD0 conversion monitoring function interrupt A
        dummy_isr,              // INTAD1CPA   (43 + EII) : AD1 conversion monitoring function interrupt A
        dummy_isr,              // INTAD0CPB   (44 + EII) : AD0 conversion monitoring function interrupt B
        dummy_isr,              // INTAD1CPB   (45 + EII) : AD1 conversion monitoring function interrupt B
        dummy_isr,              // INTTB20     (46 + EII) : 16bit TMRB2 compare match detection 0/ Over flow
        dummy_isr,              // INTTB21     (47 + EII) : 16bit TMRB2 compare match detection 1
        dummy_isr,              // INTTB30     (48 + EII) : 16bit TMRB3 compare match detection 0/ Over flow
        dummy_isr,              // INTTB31     (49 + EII) : 16bit TMRB3 compare match detection 1
        dummy_isr,              // INTCAP20    (50 + EII) : 16bit TMRB2 input capture 0
        dummy_isr,              // INTCAP21    (51 + EII) : 16bit TMRB2 input capture 1
        dummy_isr,              // INTCAP30    (52 + EII) : 16bit TMRB3 input capture 0
        dummy_isr,              // INTCAP31    (53 + EII) : 16bit TMRB3 input capture 1
        dummy_isr,              // INTAD0SFT   (54 + EII) : ADC0 conversion started by software is finished
        dummy_isr,              // INTAD1SFT   (55 + EII) : ADC1 conversion started by software is finished
        dummy_isr,              // INTAD0TMR   (56 + EII) : ADC0 conversion triggered by timer is finished
        dummy_isr,              // INTAD1TMR   (57 + EII) : ADC1 conversion triggered by timer is finished
        dummy_isr,              // INT8        (58 + EII) : Interrupt Pin (PA7/TB4IN/9pin or 11pin)
        dummy_isr,              // INT9        (59 + EII) : Interrupt Pin (PD3/33pin or 35pin)
        dummy_isr,              // INTA        (60 + EII) : Interrupt Pin (FTEST2/PL1/21pin or 23pin)
        dummy_isr,              // INTB        (61 + EII) : Interrupt Pin (FTEST3/PL0/20pin or 22pin)
        dummy_isr,              // INTENC0     (62 + EII) : Ender input0 interrupt
        dummy_isr,              // INTENC1     (63 + EII) : Ender input1 interrupt
        INTRX3_IRQHandler,      // INTRX3      (64 + EII) : Serial reception (channel.3)
        INTTX3_IRQHandler,      // INTTX3      (65 + EII) : Serial transmit (channel.3)
        dummy_isr,              // INTTB60     (66 + EII) : 16bit TMRB6 compare match detection 0 / Over flow
        dummy_isr,              // INTTB61     (67 + EII) : 16bit TMRB6 compare match detection 1
        INTTB70_IRQHandler,     // INTTB70     (68 + EII) : 16bit TMRB7 compare match detection 0 / Over flow
        dummy_isr,              // INTTB71     (69 + EII) : 16bit TMRB7 compare match detection 1
        dummy_isr,              // INTCAP60    (70 + EII) : 16bit TMRB6 input capture 0
        dummy_isr,              // INTCAP61    (71 + EII) : 16bit TMRB6 input capture 1
        dummy_isr,              // INTCAP70    (72 + EII) : 16bit TMRB7 input capture 0
        dummy_isr,              // INTCAP71    (73 + EII) : 16bit TMRB7 input capture 1
        dummy_isr,              // INTC        (74 + EII) : Interrupt Pin (PJ6/AINB9/74pin or 76 pin)
        dummy_isr,              // INTD        (75 + EII) : Interrupt Pin (PJ7/AINB10/73pin or 75pin)
        dummy_isr,              // INTE        (76 + EII) : Interrupt Pin (PK0/AINB11/72pin or 74pin)
        dummy_isr,              // INTF        (77 + EII) : Interrupt Pin (PK1/AINB12/71pin or 73pin)
};

#pragma call_graph_root = "interrupt"
__weak void NMI_Handler(void)
{
    while (1)
    {
    }
}
#pragma call_graph_root = "interrupt"
__weak void HardFault_Handler(void)
{
    while (1)
    {
    }
}
#pragma call_graph_root = "interrupt"
__weak void MemManage_Handler(void)
{
    while (1)
    {
    }
}
#pragma call_graph_root = "interrupt"
__weak void BusFault_Handler(void)
{
    while (1)
    {
    }
}
#pragma call_graph_root = "interrupt"
__weak void UsageFault_Handler(void)
{
    while (1)
    {
    }
}
#pragma call_graph_root = "interrupt"
__weak void SVC_Handler(void)
{
    while (1)
    {
    }
}
#pragma call_graph_root = "interrupt"
__weak void DebugMon_Handler(void)
{
    while (1)
    {
    }
}
#pragma call_graph_root = "interrupt"
__weak void PendSV_Handler(void)
{
    while (1)
    {
    }
}
#pragma call_graph_root = "interrupt"
__weak void SysTick_Handler(void)
{
    while (1)
    {
    }
}

void dummy_isr(void)
{
    while (1)
    {
        // do nothing
    }
}

void __cmain(void);
__weak void __iar_init_core(void);
__weak void __iar_init_vfp(void);

#pragma required = __vector_table
void __iar_program_start(void)
{
    SystemInit();

#if __ARM_ISA_THUMB >= 2
    asm("mov32   r0, #0xfef5eda5\n"
#else
#ifndef __NO_LITERAL_POOL__
    asm("ldr     r0, =0xfef5eda5\n"
#else
    asm("movs    r0,     #0xfe\n"
        "lsls    r0, r0, #8\n"
        "adds    r0, r0, #0xf5\n"
        "lsls    r0, r0, #8\n"
        "adds    r0, r0, #0xed\n"
        "lsls    r0, r0, #8\n"
        "adds    r0, r0, #0xa5\n"
#endif
#endif
        "mov     r1, r0\n"
        "push    {r0, r1}\n"
        "mov     r0, sp\n"
        "msr     PSP, r0\n");
    __iar_init_core();
    __iar_init_vfp();
    __cmain();
}
