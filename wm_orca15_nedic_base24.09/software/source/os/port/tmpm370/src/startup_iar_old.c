void SysTick_Handler(void);
void INTRX1_IRQHandler(void);
void INTRX2_IRQHandler(void);
void INTRX3_IRQHandler(void);
void INT1_IRQHandler(void);

unsigned int unFlagISR;
void NMI_ISR(void)
{
    unFlagISR |= (1U << 0);
}

void ISR_DUMMY(void)
{
    unFlagISR |= (1U << 2);
}

void ISR_HFI(void)
{
    unFlagISR |= (1U << 3);
}
void ISR_MMI(void)
{
    unFlagISR |= (1U << 4);
}
void ISR_BFI(void)
{
    unFlagISR |= (1U << 5);
}
void ISR_UFI(void)
{
    unFlagISR |= (1U << 6);
}
void ISR_SVCI(void)
{
    unFlagISR |= (1U << 7);
}
void ISR_PSI(void)
{
    unFlagISR |= (1U << 8);
}
void ISR_STI(void)
{
    unFlagISR |= (1U << 9);
}

typedef void (*intfunc)(void);
typedef union
{
    intfunc __fun;
    void *__ptr;
} intvec_elem;

#pragma language = extended
#pragma segment = "CSTACK"

void __iar_program_start(void);

#pragma location = ".intvec"
/* TMPM330 Vector Table entries */
const intvec_elem __vector_table[] =
    {
        {.__ptr = __sfe("CSTACK")},   // MAIN_STACK 0 : Main Stack
        __iar_program_start,          // RESETI 1 : Reset
        NMI_ISR,                      // NMII 2  : Non-maskable Interrupt
        ISR_HFI,                      // HFI  3  : Hard Fault
        ISR_MMI,                      // MMI  4  : Memory Management
        ISR_BFI,                      // BFI  5  : Bus Fault
        ISR_UFI,                      // UFI  6  : Usage Fault
        0,
        0,
        0,
        0,                 // Reserved     7,8,9,10
        ISR_SVCI,          // SVCI  11 : SVCall
        ISR_DUMMY,         // DMI   12 : Debug Monitor
        ISR_DUMMY,         // Reserved  13
        ISR_PSI,           // PSI  14  : PendSV
        SysTick_Handler,   // STI  15  : SysTick
        // EII 16 : External Interrupt
        ISR_DUMMY,         // INT0:Int Pin(PH0/AINA0/96pin or 98pin)
        INT1_IRQHandler,   // INT1:Int Pin (PH1/AINA1/95pin or 97pin)
        ISR_DUMMY,         // INT2:Int Pin (PH2/AINA2/94pin or 96pin)
        ISR_DUMMY,         // INT3:Int Pin (PA0/TB0IN/2pin or 4pin)
        ISR_DUMMY,         // INT4:Int Pin (PA2/TB1IN/4pin or 6pin)
        ISR_DUMMY,         // INT5:Int Pin (PE4/TB2IN//15pin or 17pin)
        ISR_DUMMY,         // INTRX0      ( 6 + EII) : Serial reception (channel.0)
        ISR_DUMMY,         // INTTX0      ( 7 + EII) : Serial transmit (channel.0)
        INTRX1_IRQHandler, // INTRX1      ( 8 + EII) : Serial reception (channel.1)
        ISR_DUMMY,         // INTTX1      ( 9 + EII) : Serial transmit (channel.1)
        ISR_DUMMY,         // INTVCNA     (10 + EII) : Vector Engine interrupt A
        ISR_DUMMY,         // INTVCNB     (11 + EII) : Vector Engine interrupt B
        ISR_DUMMY,         // INTEMG0     (12 + EII) : PMD0 EMG interrupt
        ISR_DUMMY,         // INTEMG1     (13 + EII) : PMD1 EMG interrupt
        ISR_DUMMY,         // INTOVV0     (14 + EII) : PMD0 OVV interrupt
        ISR_DUMMY,         // INTOVV1     (15 + EII) : PMD1 OVV interrupt
        ISR_DUMMY,         // INTAD0PDA:ADC0 triggered by PMD0 is finished
        ISR_DUMMY,         // INTAD1PDA:ADC1 triggered by PMD0 is finished
        ISR_DUMMY,           // INTAD0PDB   (18 + EII) : ADC0 conversion triggered by PMD1 is finished
        ISR_DUMMY,           // INTAD1PDB   (19 + EII) : ADC1 conversion triggered by PMD1 is finished
        ISR_DUMMY,           // INTTB00     (20 + EII) : 16bit TMRB0 compare match detection 0/ Over flow
        ISR_DUMMY,           // INTTB01     (21 + EII) : 16bit TMRB0 compare match detection 1
        ISR_DUMMY,           // INTTB10     (22 + EII) : 16bit TMRB1 compare match detection 0/ Over flow
        ISR_DUMMY,           // INTTB11     (23 + EII) : 16bit TMRB1 compare match detection 1
        ISR_DUMMY,           // INTTB40     (24 + EII) : 16bit TMRB4 compare match detection 0/ Over flow
        ISR_DUMMY,           // INTTB41     (25 + EII) : 16bit TMRB4 compare match detection 1
        ISR_DUMMY,           // INTTB50     (26 + EII) : 16bit TMRB5 compare match detection 0/ Over flow
        ISR_DUMMY,           // INTTB51     (27 + EII) : 16bit TMRB5 compare match detection 1
        ISR_DUMMY,           // INTPMD0     (28 + EII) : PMD0 PWM interrupt
        ISR_DUMMY,           // INTPMD1     (29 + EII) : PMD1 PWM interrupt
        ISR_DUMMY,           // INTCAP00    (30 + EII) : 16bit TMRB0 input capture 0
        ISR_DUMMY,           // INTCAP01    (31 + EII) : 16bit TMRB0 input capture 1
        ISR_DUMMY,           // INTCAP10    (32 + EII) : 16bit TMRB1 input capture 0
        ISR_DUMMY,           // INTCAP11    (33 + EII) : 16bit TMRB1 input capture 1
        ISR_DUMMY,           // INTCAP40    (34 + EII) : 16bit TMRB4 input capture 0
        ISR_DUMMY,           // INTCAP41    (35 + EII) : 16bit TMRB4 input capture 1
        ISR_DUMMY,           // INTCAP50    (36 + EII) : 16bit TMRB5 input capture 0
        ISR_DUMMY,           // INTCAP51    (37 + EII) : 16bit TMRB5 input capture 1
        ISR_DUMMY,           // INT6        (38 + EII) : Interrupt Pin (PE6/TB3IN//17pin or 19pin)
        ISR_DUMMY,           // INT7        (39 + EII) : Interrupt Pin (PE7/TB3OUT/18pin or 20pin)
        INTRX2_IRQHandler,   // INTRX2      (40 + EII) : Serial reception (channel.2)
        ISR_DUMMY,           // INTTX2      (41 + EII) : Serial transmit (channel.2)
        ISR_DUMMY,           // INTAD0CPA   (42 + EII) : AD0 conversion monitoring function interrupt A
        ISR_DUMMY,           // INTAD1CPA   (43 + EII) : AD1 conversion monitoring function interrupt A
        ISR_DUMMY,           // INTAD0CPB   (44 + EII) : AD0 conversion monitoring function interrupt B
        ISR_DUMMY,           // INTAD1CPB   (45 + EII) : AD1 conversion monitoring function interrupt B
        ISR_DUMMY,           // INTTB20     (46 + EII) : 16bit TMRB2 compare match detection 0/ Over flow
        ISR_DUMMY,           // INTTB21     (47 + EII) : 16bit TMRB2 compare match detection 1
        ISR_DUMMY,           // INTTB30     (48 + EII) : 16bit TMRB3 compare match detection 0/ Over flow
        ISR_DUMMY,           // INTTB31     (49 + EII) : 16bit TMRB3 compare match detection 1
        ISR_DUMMY,           // INTCAP20    (50 + EII) : 16bit TMRB2 input capture 0
        ISR_DUMMY,           // INTCAP21    (51 + EII) : 16bit TMRB2 input capture 1
        ISR_DUMMY,           // INTCAP30    (52 + EII) : 16bit TMRB3 input capture 0
        ISR_DUMMY,           // INTCAP31    (53 + EII) : 16bit TMRB3 input capture 1
        ISR_DUMMY,           // INTAD0SFT   (54 + EII) : ADC0 conversion started by software is finished
        ISR_DUMMY,           // INTAD1SFT   (55 + EII) : ADC1 conversion started by software is finished
        ISR_DUMMY,           // INTAD0TMR   (56 + EII) : ADC0 conversion triggered by timer is finished
        ISR_DUMMY,           // INTAD1TMR   (57 + EII) : ADC1 conversion triggered by timer is finished
        ISR_DUMMY,           // INT8        (58 + EII) : Interrupt Pin (PA7/TB4IN/9pin or 11pin)
        ISR_DUMMY,           // INT9        (59 + EII) : Interrupt Pin (PD3/33pin or 35pin)
        ISR_DUMMY,           // INTA        (60 + EII) : Interrupt Pin (FTEST2/PL1/21pin or 23pin)
        ISR_DUMMY,           // INTB        (61 + EII) : Interrupt Pin (FTEST3/PL0/20pin or 22pin)
        ISR_DUMMY,           // INTENC0     (62 + EII) : Ender input0 interrupt
        ISR_DUMMY,           // INTENC1     (63 + EII) : Ender input1 interrupt
        INTRX3_IRQHandler,   // INTRX3      (64 + EII) : Serial reception (channel.3)
        ISR_DUMMY,           // INTTX3      (65 + EII) : Serial transmit (channel.3)
        ISR_DUMMY,           // INTTB60     (66 + EII) : 16bit TMRB6 compare match detection 0 / Over flow
        ISR_DUMMY,           // INTTB61     (67 + EII) : 16bit TMRB6 compare match detection 1
        ISR_DUMMY,           // INTTB70     (68 + EII) : 16bit TMRB7 compare match detection 0 / Over flow
        ISR_DUMMY,           // INTTB71     (69 + EII) : 16bit TMRB7 compare match detection 1
        ISR_DUMMY,           // INTCAP60    (70 + EII) : 16bit TMRB6 input capture 0
        ISR_DUMMY,           // INTCAP61    (71 + EII) : 16bit TMRB6 input capture 1
        ISR_DUMMY,           // INTCAP70    (72 + EII) : 16bit TMRB7 input capture 0
        ISR_DUMMY,           // INTCAP71    (73 + EII) : 16bit TMRB7 input capture 1
        ISR_DUMMY,           // INTC        (74 + EII) : Interrupt Pin (PJ6/AINB9/74pin or 76 pin)
        ISR_DUMMY,           // INTD        (75 + EII) : Interrupt Pin (PJ7/AINB10/73pin or 75pin)
        ISR_DUMMY,           // INTE        (76 + EII) : Interrupt Pin (PK0/AINB11/72pin or 74pin)
        ISR_DUMMY,           // INTF        (77 + EII) : Interrupt Pin (PK1/AINB12/71pin or 73pin)
};
