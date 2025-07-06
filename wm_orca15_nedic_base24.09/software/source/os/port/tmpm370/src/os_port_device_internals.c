#include "def.h"
// #include "core_cm3.h"
// #include "core_cmFunc.h"

#include "os_config.h"
#include "mcal.h"
#include "os_int.h"

#ifdef __IAR_SYSTEMS_ICC__
#pragma language = extended
#pragma segment = "CSTACK"
#endif

extern uint32_t _stackstart;
extern const uint32_t STACK_SIZE;

uint32_t os_device_internals_sp_get(void)
{
    return __get_MSP();
}

uint32_t os_device_internals_stackStartAddr_get(void)
{
#if __GNUC__
    uint32_t stackStart = (uint32_t)&_stackstart;
    // TODO:
    return (stackStart + (STACK_SIZE * sizeof(long)));
#else
    return (uint32_t)(__sfe("CSTACK"));
#endif
}

uint32_t os_device_internals_stackEndAddr_get(void)
{
#if __GNUC__
    return _stackstart;
#else
    return ((uint32_t)(__sfe("CSTACK")) - ((uint32_t)&STACK_SIZE));
#endif
}

uint32_t os_device_internals_get_stack_size(void)
{
#if __GNUC__
    return STACK_SIZE;
#else
    return (uint32_t)&STACK_SIZE;
#endif
}
