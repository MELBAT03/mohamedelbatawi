#ifndef OS_INT_H
#define OS_INT_H

#include "def.h"
#include "os_int_handlers.h"

void os_int_handler_install(os_int_table_t vctNum, void (*handler)(void));

#endif