#ifndef OS_DEVICE_H
#define OS_DEVICE_H

#include "def.h"
#include "mcal.h"

#define OS_POR_RESET_FLAG   (MCAL_POR_REST_FLAG)
#define OS_EXT_RESET_FLAG   (MCAL_EXT_RESET_FLAG)
#define OS_WDT_RESET_FLAG   (MCAL_WDT_RESET_FLAG)
#define OS_VLTD_RESET_FLAG  (MCAL_VLTD_RESET_FLAG)
#define OS_DEBUG_RESET_FLAG (MCAL_DEBUG_RESET_FLAG)
#define OS_OFD_RESET_FLAG   (MCAL_OFD_RESET_FLAG)

uint32_t os_device_internals_sp_get(void);
uint32_t os_device_internals_stackStartAddr_get(void);
uint32_t os_device_internals_stackEndAddr_get(void);
uint32_t os_device_internals_get_stack_size(void);

#endif
