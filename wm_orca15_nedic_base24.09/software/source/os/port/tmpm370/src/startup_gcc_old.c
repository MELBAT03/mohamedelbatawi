// Stack size (in Words)
#define STACK_SIZE 0x00000300
__attribute__((section(".stack"))) unsigned long _stackBuffer[STACK_SIZE];

void __attribute__((weak)) Reset_Handler(void);
void __attribute__((weak)) NMI_Handler(void);
void __attribute__((weak)) HardFault_Handler(void);
void __attribute__((weak)) MemManage_Handler(void);
void __attribute__((weak)) BusFault_Handler(void);
void __attribute__((weak)) UsageFault_Handler(void);
void __attribute__((weak)) SVC_Handler(void);
void __attribute__((weak)) DebugMon_Handler(void);
void __attribute__((weak)) PendSV_Handler(void);
void __attribute__((weak)) SysTick_Handler(void);

void __attribute__((weak)) INT0_IRQHandler(void);        // 0:  Interrupt Pin (PH0/AINA0/96pin or 98pin)
void __attribute__((weak)) INT1_IRQHandler(void);        // 1:  Interrupt Pin (PH1/AINA1/95pin or 97pin)
void __attribute__((weak)) INT2_IRQHandler(void);        // 2:  Interrupt Pin (PH2/AINA2/94pin or 96pin)
void __attribute__((weak)) INT3_IRQHandler(void);        // 3:  Interrupt Pin (PA0/TB0IN/2pin or 4pin)
void __attribute__((weak)) INT4_IRQHandler(void);        // 4:  Interrupt Pin (PA2/TB1IN/4pin or 6pin)
void __attribute__((weak)) INT5_IRQHandler(void);        // 5:  Interrupt Pin (PE4/TB2IN//15pin or 17pin)
void __attribute__((weak)) INTRX0_IRQHandler(void);      // 6:  Serial reception (channel.0)
void __attribute__((weak)) INTTX0_IRQHandler(void);      // 7:  Serial transmit (channel.0)
void __attribute__((weak)) INTRX1_IRQHandler(void);      // 8:  Serial reception (channel.1)
void __attribute__((weak)) INTTX1_IRQHandler(void);      // 9:  Serial transmit (channel.1)
void __attribute__((weak)) INTVCNA_IRQHandler(void);     // 10: Vector Engine interrupt A
void __attribute__((weak)) INTVCNB_IRQHandler(void);     // 11: Vector Engine interrupt B
void __attribute__((weak)) INTEMG0_IRQHandler(void);     // 12: PMD0 EMG interrupt
void __attribute__((weak)) INTEMG1_IRQHandler(void);     // 13: PMD1 EMG interrupt
void __attribute__((weak)) INTOVV0_IRQHandler(void);     // 14: PMD0 OVV interrupt
void __attribute__((weak)) INTOVV1_IRQHandler(void);     // 15: PMD1 OVV interrupt
void __attribute__((weak)) INTADAPDA_IRQHandler(void);   // 16: ADCA conversion triggered by PMD0 is finished
void __attribute__((weak)) INTADBPDA_IRQHandler(void);   // 17: ADCB conversion triggered by PMD0 is finished
void __attribute__((weak)) INTADAPDB_IRQHandler(void);   // 18: ADCA conversion triggered by PMD1 is finished
void __attribute__((weak)) INTADBPDB_IRQHandler(void);   // 19: ADCB conversion triggered by PMD1 is finished
void __attribute__((weak)) INTTB00_IRQHandler(void);     // 20: 16bit TMRB0 compare match detection 0/ Over flow
void __attribute__((weak)) INTTB01_IRQHandler(void);     // 21: 16bit TMRB0 compare match detection 1
void __attribute__((weak)) INTTB10_IRQHandler(void);     // 22: 16bit TMRB1 compare match detection 0/ Over flow
void __attribute__((weak)) INTTB11_IRQHandler(void);     // 23: 16bit TMRB1 compare match detection 1
void __attribute__((weak)) INTTB40_IRQHandler(void);     // 24: 16bit TMRB4 compare match detection 0/ Over flow
void __attribute__((weak)) INTTB41_IRQHandler(void);     // 25: 16bit TMRB4 compare match detection 1
void __attribute__((weak)) INTTB50_IRQHandler(void);     // 26: 16bit TMRB5 compare match detection 0/ Over flow
void __attribute__((weak)) INTTB51_IRQHandler(void);     // 27: 16bit TMRB5 compare match detection 1
void __attribute__((weak)) INTPMD0_IRQHandler(void);     // 28: PMD0 PWM interrupt
void __attribute__((weak)) INTPMD1_IRQHandler(void);     // 29: PMD1 PWM interrupt
void __attribute__((weak)) INTCAP00_IRQHandler(void);    // 30: 16bit TMRB0 input capture 0
void __attribute__((weak)) INTCAP01_IRQHandler(void);    // 31: 16bit TMRB0 input capture 1
void __attribute__((weak)) INTCAP10_IRQHandler(void);    // 32: 16bit TMRB1 input capture 0
void __attribute__((weak)) INTCAP11_IRQHandler(void);    // 33: 16bit TMRB1 input capture 1
void __attribute__((weak)) INTCAP40_IRQHandler(void);    // 34: 16bit TMRB4 input capture 0
void __attribute__((weak)) INTCAP41_IRQHandler(void);    // 35: 16bit TMRB4 input capture 1
void __attribute__((weak)) INTCAP50_IRQHandler(void);    // 36: 16bit TMRB5 input capture 0
void __attribute__((weak)) INTCAP51_IRQHandler(void);    // 37: 16bit TMRB5 input capture 1
void __attribute__((weak)) INT6_IRQHandler(void);        // 38: Interrupt Pin (PE6/TB3IN//17pin or 19pin)
void __attribute__((weak)) INT7_IRQHandler(void);        // 39: Interrupt Pin (PE7/TB3OUT/18pin or 20pin)
void __attribute__((weak)) INTRX2_IRQHandler(void);      // 40: Serial reception (channel.2)
void __attribute__((weak)) INTTX2_IRQHandler(void);      // 41: Serial transmit (channel.2)
void __attribute__((weak)) INTADACPA_IRQHandler(void);   // 42: ADCA conversion monitoring function interrupt A
void __attribute__((weak)) INTADBCPA_IRQHandler(void);   // 43: ADCB conversion monitoring function interrupt A
void __attribute__((weak)) INTADACPB_IRQHandler(void);   // 44: ADCA conversion monitoring function interrupt B
void __attribute__((weak)) INTADBCPB_IRQHandler(void);   // 45: ADCB conversion monitoring function interrupt B
void __attribute__((weak)) INTTB20_IRQHandler(void);     // 46: 16bit TMRB2 compare match detection 0/ Over flow
void __attribute__((weak)) INTTB21_IRQHandler(void);     // 47: 16bit TMRB2 compare match detection 1
void __attribute__((weak)) INTTB30_IRQHandler(void);     // 48: 16bit TMRB3 compare match detection 0/ Over flow
void __attribute__((weak)) INTTB31_IRQHandler(void);     // 49: 16bit TMRB3 compare match detection 1
void __attribute__((weak)) INTCAP20_IRQHandler(void);    // 50: 16bit TMRB2 input capture 0
void __attribute__((weak)) INTCAP21_IRQHandler(void);    // 51: 16bit TMRB2 input capture 1
void __attribute__((weak)) INTCAP30_IRQHandler(void);    // 52: 16bit TMRB3 input capture 0
void __attribute__((weak)) INTCAP31_IRQHandler(void);    // 53: 16bit TMRB3 input capture 1
void __attribute__((weak)) INTADASFT_IRQHandler(void);   // 54: ADCA conversion started by software is finished
void __attribute__((weak)) INTADBSFT_IRQHandler(void);   // 55: ADCB conversion started by software is finished
void __attribute__((weak)) INTADATMR_IRQHandler(void);   // 56: ADCA conversion triggered by timer is finished
void __attribute__((weak)) INTADBTMR_IRQHandler(void);   // 57: ADCB conversion triggered by timer is finished
void __attribute__((weak)) INT8_IRQHandler(void);        // 58: Interrupt Pin (PA7/TB4IN/9pin or 11pin)
void __attribute__((weak)) INT9_IRQHandler(void);        // 59: Interrupt Pin (PD3/33pin or 35pin)
void __attribute__((weak)) INTA_IRQHandler(void);        // 60: Interrupt Pin (PL1/21pin or 23pin)
void __attribute__((weak)) INTB_IRQHandler(void);        // 61: Interrupt Pin (PL0/20pin or 22pin)
void __attribute__((weak)) INTENC0_IRQHandler(void);     // 62: Ender input0 interrupt
void __attribute__((weak)) INTENC1_IRQHandler(void);     // 63: Ender input1 interrupt
void __attribute__((weak)) INTRX3_IRQHandler(void);      // 64: Serial reception (channel.3)
void __attribute__((weak)) INTTX3_IRQHandler(void);      // 65: Serial transmit (channel.3)
void __attribute__((weak)) INTTB60_IRQHandler(void);     // 66: 16bit TMRB6 compare match detection 0 / Over flow
void __attribute__((weak)) INTTB61_IRQHandler(void);     // 67: 16bit TMRB6 compare match detection 1
void __attribute__((weak)) INTTB70_IRQHandler(void);     // 68: 16bit TMRB7 compare match detection 0 / Over flow
void __attribute__((weak)) INTTB71_IRQHandler(void);     // 69: 16bit TMRB7 compare match detection 1
void __attribute__((weak)) INTCAP60_IRQHandler(void);    // 70: 16bit TMRB6 input capture 0
void __attribute__((weak)) INTCAP61_IRQHandler(void);    // 71: 16bit TMRB6 input capture 1
void __attribute__((weak)) INTCAP70_IRQHandler(void);    // 72: 16bit TMRB7 input capture 0
void __attribute__((weak)) INTCAP71_IRQHandler(void);    // 73: 16bit TMRB7 input capture 1
void __attribute__((weak)) INTC_IRQHandler(void);        // 74: Interrupt Pin (PJ6/AINB9/74pin or 76 pin)
void __attribute__((weak)) INTD_IRQHandler(void);        // 75: Interrupt Pin (PJ7/AINB10/73pin or 75pin)
void __attribute__((weak)) INTE_IRQHandler(void);        // 76: Interrupt Pin (PK0/AINB11/72pin or 74pin)
void __attribute__((weak)) INTF_IRQHandler(void);        // 77: Interrupt Pin (PK1/AINB12/71pin or 73pin)

//*****************************************************************************
// Symbols defined in linker script

// Start address for the initialization values of the .data section.
extern unsigned long _sidata;

// Start address for the .data section
extern unsigned long _sdata;

// End address for the .data section
extern unsigned long _edata;

// Start address for the .bss section
extern unsigned long _sbss;

// End address for the .bss section
extern unsigned long _ebss;

// End address for ram
extern void _eram;

//*****************************************************************************

extern int main(void);
extern void SystemInit(void);
__attribute__((used)) void Reset_Handler(void);
static void Default_Handler(void);

//*****************************************************************************

// The minimal vector table for a Cortex M3.  Note that the proper constructs
// must be placed on this to ensure that it ends up at physical address
// 0x00000000.

__attribute__((used, section(".isr_vector"))) void (*const __Vectors[])(void) =
    {
        (void *)&_stackBuffer[STACK_SIZE],   // The initial stack pointer
        Reset_Handler,                       // The reset handler
        NMI_Handler,                         // The NMI handler
        HardFault_Handler,                   // The hard fault handler
        MemManage_Handler,                   // The MPU fault handler
        BusFault_Handler,                    // The bus fault handler
        UsageFault_Handler,                  // The usage fault handler
        0,                                   // Reserved
        0,                                   // Reserved
        0,                                   // Reserved
        0,                                   // Reserved
        SVC_Handler,                         // SVCall handler
        DebugMon_Handler,                    // Debug monitor handler
        0,                                   // Reserved
        PendSV_Handler,                      // The PendSV handler
        SysTick_Handler,                     // The SysTick handler

        INT0_IRQHandler,        // 0:  Interrupt Pin (PH0/AINA0/96pin or 98pin)
        INT1_IRQHandler,        // 1:  Interrupt Pin (PH1/AINA1/95pin or 97pin)
        INT2_IRQHandler,        // 2:  Interrupt Pin (PH2/AINA2/94pin or 96pin)
        INT3_IRQHandler,        // 3:  Interrupt Pin (PA0/TB0IN/2pin or 4pin)
        INT4_IRQHandler,        // 4:  Interrupt Pin (PA2/TB1IN/4pin or 6pin)
        INT5_IRQHandler,        // 5:  Interrupt Pin (PE4/TB2IN//15pin or 17pin)
        INTRX0_IRQHandler,      // 6:  Serial reception (channel.0)
        INTTX0_IRQHandler,      // 7:  Serial transmit (channel.0)
        INTRX1_IRQHandler,      // 8:  Serial reception (channel.1)
        INTTX1_IRQHandler,      // 9:  Serial transmit (channel.1)
        INTVCNA_IRQHandler,     // 10: Vector Engine interrupt A
        INTVCNB_IRQHandler,     // 11: Vector Engine interrupt B
        INTEMG0_IRQHandler,     // 12: PMD0 EMG interrupt
        INTEMG1_IRQHandler,     // 13: PMD1 EMG interrupt
        INTOVV0_IRQHandler,     // 14: PMD0 OVV interrupt
        INTOVV1_IRQHandler,     // 15: PMD1 OVV interrupt
        INTADAPDA_IRQHandler,   // 16: ADCA conversion triggered by PMD0 is finished
        INTADBPDA_IRQHandler,   // 17: ADCB conversion triggered by PMD0 is finished
        INTADAPDB_IRQHandler,   // 18: ADCA conversion triggered by PMD1 is finished
        INTADBPDB_IRQHandler,   // 19: ADCB conversion triggered by PMD1 is finished
        INTTB00_IRQHandler,     // 20: 16bit TMRB0 compare match detection 0/ Over flow
        INTTB01_IRQHandler,     // 21: 16bit TMRB0 compare match detection 1
        INTTB10_IRQHandler,     // 22: 16bit TMRB1 compare match detection 0/ Over flow
        INTTB11_IRQHandler,     // 23: 16bit TMRB1 compare match detection 1
        INTTB40_IRQHandler,     // 24: 16bit TMRB4 compare match detection 0/ Over flow
        INTTB41_IRQHandler,     // 25: 16bit TMRB4 compare match detection 1
        INTTB50_IRQHandler,     // 26: 16bit TMRB5 compare match detection 0/ Over flow
        INTTB51_IRQHandler,     // 27: 16bit TMRB5 compare match detection 1
        INTPMD0_IRQHandler,     // 28: PMD0 PWM interrupt
        INTPMD1_IRQHandler,     // 29: PMD1 PWM interrupt
        INTCAP00_IRQHandler,    // 30: 16bit TMRB0 input capture 0
        INTCAP01_IRQHandler,    // 31: 16bit TMRB0 input capture 1
        INTCAP10_IRQHandler,    // 32: 16bit TMRB1 input capture 0
        INTCAP11_IRQHandler,    // 33: 16bit TMRB1 input capture 1
        INTCAP40_IRQHandler,    // 34: 16bit TMRB4 input capture 0
        INTCAP41_IRQHandler,    // 35: 16bit TMRB4 input capture 1
        INTCAP50_IRQHandler,    // 36: 16bit TMRB5 input capture 0
        INTCAP51_IRQHandler,    // 37: 16bit TMRB5 input capture 1
        INT6_IRQHandler,        // 38: Interrupt Pin (PE6/TB3IN//17pin or 19pin)
        INT7_IRQHandler,        // 39: Interrupt Pin (PE7/TB3OUT/18pin or 20pin)
        INTRX2_IRQHandler,      // 40: Serial reception (channel.2)
        INTTX2_IRQHandler,      // 41: Serial transmit (channel.2)
        INTADACPA_IRQHandler,   // 42: ADCA conversion monitoring function interrupt A
        INTADBCPA_IRQHandler,   // 43: ADCB conversion monitoring function interrupt A
        INTADACPB_IRQHandler,   // 44: ADCA conversion monitoring function interrupt B
        INTADBCPB_IRQHandler,   // 45: ADCB conversion monitoring function interrupt B
        INTTB20_IRQHandler,     // 46: 16bit TMRB2 compare match detection 0/ Over flow
        INTTB21_IRQHandler,     // 47: 16bit TMRB2 compare match detection 1
        INTTB30_IRQHandler,     // 48: 16bit TMRB3 compare match detection 0/ Over flow
        INTTB31_IRQHandler,     // 49: 16bit TMRB3 compare match detection 1
        INTCAP20_IRQHandler,    // 50: 16bit TMRB2 input capture 0
        INTCAP21_IRQHandler,    // 51: 16bit TMRB2 input capture 1
        INTCAP30_IRQHandler,    // 52: 16bit TMRB3 input capture 0
        INTCAP31_IRQHandler,    // 53: 16bit TMRB3 input capture 1
        INTADASFT_IRQHandler,   // 54: ADCA conversion started by software is finished
        INTADBSFT_IRQHandler,   // 55: ADCB conversion started by software is finished
        INTADATMR_IRQHandler,   // 56: ADCA conversion triggered by timer is finished
        INTADBTMR_IRQHandler,   // 57: ADCB conversion triggered by timer is finished
        INT8_IRQHandler,        // 58: Interrupt Pin (PA7/TB4IN/9pin or 11pin)
        INT9_IRQHandler,        // 59: Interrupt Pin (PD3/33pin or 35pin)
        INTA_IRQHandler,        // 60: Interrupt Pin (PL1/21pin or 23pin)
        INTB_IRQHandler,        // 61: Interrupt Pin (PL0/20pin or 22pin)
        INTENC0_IRQHandler,     // 62: Ender input0 interrupt
        INTENC1_IRQHandler,     // 63: Ender input1 interrupt
        INTRX3_IRQHandler,      // 64: Serial reception (channel.3)
        INTTX3_IRQHandler,      // 65: Serial transmit (channel.3)
        INTTB60_IRQHandler,     // 66: 16bit TMRB6 compare match detection 0 / Over flow
        INTTB61_IRQHandler,     // 67: 16bit TMRB6 compare match detection 1
        INTTB70_IRQHandler,     // 68: 16bit TMRB7 compare match detection 0 / Over flow
        INTTB71_IRQHandler,     // 69: 16bit TMRB7 compare match detection 1
        INTCAP60_IRQHandler,    // 70: 16bit TMRB6 input capture 0
        INTCAP61_IRQHandler,    // 71: 16bit TMRB6 input capture 1
        INTCAP70_IRQHandler,    // 72: 16bit TMRB7 input capture 0
        INTCAP71_IRQHandler,    // 73: 16bit TMRB7 input capture 1
        INTC_IRQHandler,        // 74: Interrupt Pin (PJ6/AINB9/74pin or 76 pin)
        INTD_IRQHandler,        // 75: Interrupt Pin (PJ7/AINB10/73pin or 75pin)
        INTE_IRQHandler,        // 76: Interrupt Pin (PK0/AINB11/72pin or 74pin)
        INTF_IRQHandler,        // 77: Interrupt Pin (PK1/AINB12/71pin or 73pin)

};

//*****************************************************************************

void DefaultReset_Handler(void)
{
    // Initialize data and bss
    unsigned long *pulSrc, *pulDest;

    // Copy the data segment initializers from flash to SRAM
    pulSrc = &_sidata;

    for (pulDest = &_sdata; pulDest < &_edata;)
    {
        *(pulDest++) = *(pulSrc++);
    }

    // Zero fill the bss segment.
    for (pulDest = &_sbss; pulDest < &_ebss;)
    {
        *(pulDest++) = 0;
    }

#if 0
    /* Zero fill the bss segment.  This is done with inline assembly since this
     will clear the value of pulDest if it is not kept in a register. */
    __asm("  ldr     r0, =_sbss\n"
          "  ldr     r1, =_ebss\n"
          "  mov     r2, #0\n"
          "  .thumb_func\n"
          "zero_loop:\n"
          "    cmp     r0, r1\n"
          "    it      lt\n"
          "    strlt   r2, [r0], #4\n"
          "    blt     zero_loop");
#endif

    // Call the application's entry point.
    SystemInit();
    main();
}

//*****************************************************************************
//
// Provide weak aliases for each Exception handler to the DefaultIntHandler.
// As they are weak aliases, any function with the same name will override
// this definition.
//
//*****************************************************************************

#pragma weak Reset_Handler = DefaultReset_Handler
#pragma weak NMI_Handler = Default_Handler
#pragma weak HardFault_Handler = Default_Handler
#pragma weak MemManage_Handler = Default_Handler
#pragma weak BusFault_Handler = Default_Handler
#pragma weak UsageFault_Handler = Default_Handler
#pragma weak SVC_Handler = Default_Handler
#pragma weak DebugMon_Handler = Default_Handler
#pragma weak PendSV_Handler = Default_Handler
#pragma weak SysTick_Handler = Default_Handler

#pragma weak INT0_IRQHandler = Default_Handler
#pragma weak INT1_IRQHandler = Default_Handler
#pragma weak INT2_IRQHandler = Default_Handler
#pragma weak INT3_IRQHandler = Default_Handler
#pragma weak INT4_IRQHandler = Default_Handler
#pragma weak INT5_IRQHandler = Default_Handler
#pragma weak INTRX0_IRQHandler = Default_Handler
#pragma weak INTTX0_IRQHandler = Default_Handler
#pragma weak INTRX1_IRQHandler = Default_Handler
#pragma weak INTTX1_IRQHandler = Default_Handler
#pragma weak INTVCNA_IRQHandler = Default_Handler
#pragma weak INTVCNB_IRQHandler = Default_Handler
#pragma weak INTEMG0_IRQHandler = Default_Handler
#pragma weak INTEMG1_IRQHandler = Default_Handler
#pragma weak INTOVV0_IRQHandler = Default_Handler
#pragma weak INTOVV1_IRQHandler = Default_Handler
#pragma weak INTADAPDA_IRQHandler = Default_Handler
#pragma weak INTADBPDA_IRQHandler = Default_Handler
#pragma weak INTADAPDB_IRQHandler = Default_Handler
#pragma weak INTADBPDB_IRQHandler = Default_Handler
#pragma weak INTTB00_IRQHandler = Default_Handler
#pragma weak INTTB01_IRQHandler = Default_Handler
#pragma weak INTTB10_IRQHandler = Default_Handler
#pragma weak INTTB11_IRQHandler = Default_Handler
#pragma weak INTTB40_IRQHandler = Default_Handler
#pragma weak INTTB41_IRQHandler = Default_Handler
#pragma weak INTTB50_IRQHandler = Default_Handler
#pragma weak INTTB51_IRQHandler = Default_Handler
#pragma weak INTPMD0_IRQHandler = Default_Handler
#pragma weak INTPMD1_IRQHandler = Default_Handler
#pragma weak INTCAP00_IRQHandler = Default_Handler
#pragma weak INTCAP01_IRQHandler = Default_Handler
#pragma weak INTCAP10_IRQHandler = Default_Handler
#pragma weak INTCAP11_IRQHandler = Default_Handler
#pragma weak INTCAP40_IRQHandler = Default_Handler
#pragma weak INTCAP41_IRQHandler = Default_Handler
#pragma weak INTCAP50_IRQHandler = Default_Handler
#pragma weak INTCAP51_IRQHandler = Default_Handler
#pragma weak INT6_IRQHandler = Default_Handler
#pragma weak INT7_IRQHandler = Default_Handler
#pragma weak INTRX2_IRQHandler = Default_Handler
#pragma weak INTTX2_IRQHandler = Default_Handler
#pragma weak INTADACPA_IRQHandler = Default_Handler
#pragma weak INTADBCPA_IRQHandler = Default_Handler
#pragma weak INTADACPB_IRQHandler = Default_Handler
#pragma weak INTADBCPB_IRQHandler = Default_Handler
#pragma weak INTTB20_IRQHandler = Default_Handler
#pragma weak INTTB21_IRQHandler = Default_Handler
#pragma weak INTTB30_IRQHandler = Default_Handler
#pragma weak INTTB31_IRQHandler = Default_Handler
#pragma weak INTCAP20_IRQHandler = Default_Handler
#pragma weak INTCAP21_IRQHandler = Default_Handler
#pragma weak INTCAP30_IRQHandler = Default_Handler
#pragma weak INTCAP31_IRQHandler = Default_Handler
#pragma weak INTADASFT_IRQHandler = Default_Handler
#pragma weak INTADBSFT_IRQHandler = Default_Handler
#pragma weak INTADATMR_IRQHandler = Default_Handler
#pragma weak INTADBTMR_IRQHandler = Default_Handler
#pragma weak INT8_IRQHandler = Default_Handler
#pragma weak INT9_IRQHandler = Default_Handler
#pragma weak INTA_IRQHandler = Default_Handler
#pragma weak INTB_IRQHandler = Default_Handler
#pragma weak INTENC0_IRQHandler = Default_Handler
#pragma weak INTENC1_IRQHandler = Default_Handler
#pragma weak INTRX3_IRQHandler = Default_Handler
#pragma weak INTTX3_IRQHandler = Default_Handler
#pragma weak INTTB60_IRQHandler = Default_Handler
#pragma weak INTTB61_IRQHandler = Default_Handler
#pragma weak INTTB70_IRQHandler = Default_Handler
#pragma weak INTTB71_IRQHandler = Default_Handler
#pragma weak INTCAP60_IRQHandler = Default_Handler
#pragma weak INTCAP61_IRQHandler = Default_Handler
#pragma weak INTCAP70_IRQHandler = Default_Handler
#pragma weak INTCAP71_IRQHandler = Default_Handler
#pragma weak INTC_IRQHandler = Default_Handler
#pragma weak INTD_IRQHandler = Default_Handler
#pragma weak INTE_IRQHandler = Default_Handler
#pragma weak INTF_IRQHandler = Default_Handler

//*****************************************************************************

static void Default_Handler(void)
{
    //
    // Go into an infinite loop.
    //
    while (1)
    {
    }
}
