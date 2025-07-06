#include "os_config.h"
#include "os_int_handlers.h"

extern void (*gpv_os_int_handlers[INT_NUM])(void);

void SysTick_Handler(void)
{
    if (gpv_os_int_handlers[SYSTICK_HANDLER] != NULL)
    {
        gpv_os_int_handlers[SYSTICK_HANDLER]();
    }
}

void INTRX1_IRQHandler(void)
{
    if (gpv_os_int_handlers[INTRX1_HANDLER] != NULL)
    {
        gpv_os_int_handlers[INTRX1_HANDLER]();
    }
}

void INTRX2_IRQHandler(void)
{
    if (gpv_os_int_handlers[INTRX2_HANDLER] != NULL)
    {
        gpv_os_int_handlers[INTRX2_HANDLER]();
    }
}

void INTRX3_IRQHandler(void)
{
    if (gpv_os_int_handlers[INTRX3_HANDLER] != NULL)
    {
        gpv_os_int_handlers[INTRX3_HANDLER]();
    }
}

void INT1_IRQHandler(void)
{
    if (gpv_os_int_handlers[EXTINT1_HANDLER] != NULL)
    {
        gpv_os_int_handlers[EXTINT1_HANDLER]();
    }
}

void INTTB70_IRQHandler(void)
{
    if (gpv_os_int_handlers[TM7_OVFHANDLER] != NULL)
    {
        gpv_os_int_handlers[TM7_OVFHANDLER]();
    }
}
