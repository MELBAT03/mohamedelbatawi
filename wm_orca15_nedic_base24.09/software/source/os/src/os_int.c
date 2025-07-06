#include "os_int.h"
#include "def.h"

void (*gpv_os_int_handlers[INT_NUM])(void);

void os_int_handler_install(os_int_table_t intNum, void (*handler)(void))
{
    gpv_os_int_handlers[intNum] = handler;
}
