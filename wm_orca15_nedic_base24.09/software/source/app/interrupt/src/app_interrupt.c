#include "def.h"
#include "os.h"
#include "os_config.h"

#include "components/uart/uart.h"
#include "components/acin/acin.h"

#include "handlers/input/inc/handler_power_detection.h"

#if DDM_MODEL == DDM_INTERNAL
void ISR_MCPWM_ADC(void);
void ISR_INT_TX0(void);
void ISR_INT_RX0(void);
#endif

// NOTE: don't forget to add the handler in `os_port_int_handlers.c` if you add a new one

static void systick_handler(void)
{
    os_tick_update(&os_scheduler_time_update, OS_TICK_PERIOD_MS);
}

static void intrx1_handler(void)
{
    volatile UART_Err err;

    err = UART_GetErrState(UART1);
    if (UART_NO_ERR == err)
    {
        // RxBuffer1[RxCounter1++] = (uint8_t)UART_GetRxData(UART1);
        //  uart_recv_int(UART_1);
    }
    // uart_byte_put(UART_3, (uint8_t)UART_GetRxData(UART1) + 48);
    uart_recv_int((uart_t)MOTOR_COM_PORT_ID);
}

static void intrx2_handler(void)
{
    volatile UART_Err err;

    err = UART_GetErrState(UART2);
    if (UART_NO_ERR == err)
    {
        // RxBuffer1[RxCounter1++] = (uint8_t)UART_GetRxData(UART2);
        //  uart_recv_int(UART_2);
    }
    // uart_byte_put(UART_3, (uint8_t)UART_GetRxData(UART2) + 48);
    uart_recv_int((uart_t)DISPLAY_PORT_ID);
}

static void intrx3_handler(void)
{
    volatile UART_Err err;

    err = UART_GetErrState(UART3);
    if (UART_NO_ERR == err)
    {
        // RxBuffer1[RxCounter1++] = (uint8_t)UART_GetRxData(UART2);
        //  uart_recv_int(UART_2);
    }
    // uart_byte_put(UART_3, (uint8_t)UART_GetRxData(UART3) + 48);
    uart_recv_int((uart_t)LOG_CHANNEL_ID);
}

extern void (*handler_powerDetection_isr)(void);

static void extint1_handler(void)
{
    static uint8_t flag = 0;

    if (flag == 0U)
    {
        flag = 1;
        acin_calculation_start();
        handler_powerDetection_edge_set(MCAL_EXTINT_FALLING_EDGE);
    }
    else if (flag == 1U)
    {
        flag = 0;
        acin_calculation_record();
        handler_powerDetection_edge_set(MCAL_EXTINT_RISING_EDGE);
    }
    else
    {
        // do nothing
    }

    os_scheduler_event_update();

    handler_powerDetection_isr();
}

void app_interrupt_init(void)
{
    os_int_handler_install(SYSTICK_HANDLER, systick_handler);
    os_int_handler_install(INTRX1_HANDLER, intrx1_handler);
    os_int_handler_install(INTRX2_HANDLER, intrx2_handler);
    os_int_handler_install(INTRX3_HANDLER, intrx3_handler);
    os_int_handler_install(EXTINT1_HANDLER, extint1_handler);
}

#if DDM_MODEL == DDM_INTERNAL
void INTRX0_IRQHandler(void)
{
    ISR_INT_RX0();
}

void INTTX0_IRQHandler(void)
{
    ISR_INT_TX0();
}
//
//
// void INTRX1_IRQHandler(void)
//{
//  /*mabrouk edit uart1 */
////  Rx = UART_GetRxData(UART1);  //recieve the sent value at the heartbeat function
//  HAL_UART_RxCpltCallback();
//  ISR_INT_RX1();
//}
//
void INTTX1_IRQHandler(void)
{
    // ISR_INT_TX1();
}
void INTTX2_IRQHandler(void)
{
}
void INTTX3_IRQHandler(void)
{
}

//
//
/* ADC interrupt after conversion */
void INTADAPDA_IRQHandler(void)
{
    ISR_MCPWM_ADC();
}
#else
void INTRX0_IRQHandler(void)
{
    // do nothing
}

void INTTX0_IRQHandler(void)
{
    // do nothing
}
void INTTX1_IRQHandler(void)
{
    // do nothing
}
void INTTX2_IRQHandler(void)
{
}
void INTTX3_IRQHandler(void)
{
}
/* ADC interrupt after conversion */
void INTADAPDA_IRQHandler(void)
{
    // do nothing
}
#endif
