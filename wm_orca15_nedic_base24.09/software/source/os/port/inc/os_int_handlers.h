#ifndef OS_INT_HANDLERS_H
#define OS_INT_HANDLERS_H

#include "mcal/mcal.h"
#include "os_port_int_handlers.h"

#define OS_INT_DISABLE MCAL_GLOBAL_INTERRUPTS_DISABLE()
#define OS_INT_ENABLE  MCAL_GLOBAL_INTERRUPTS_ENABLE()

#endif
