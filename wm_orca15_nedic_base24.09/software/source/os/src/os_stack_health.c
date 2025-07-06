#include "os_stack_health.h"
#include "def.h"
#include "mcal.h"
#include "os_config.h"
#include "os_device_internals.h"
#include "os_int.h"

#define STACK_PROTECTION_CHECK_VALUE (0XDEADBEEF)   // Canary value to detect stack overflow

static void os_stackHealth_updateMaxSP(void);
static void os_stackHealth_update_current_sp(void);
static void os_stackHealth_calculate_usage_ratio(const volatile uint32_t sp_addr,
                                                 volatile uint16_t *used_space,
                                                 volatile uint8_t *usage_ratio);

typedef struct
{
    //   uint32_t dummy[2];
    uint32_t max_sp_address;
    uint32_t current_sp_address;
    uint16_t stack_size;
    uint16_t max_used_space;
    uint16_t current_used_space;
    uint8_t max_usage_ratio;
    uint8_t current_usage_ratio;
    uint8_t protection_value_currupted;
} stack_info_t;

volatile stack_info_t stack_info = {0};

void os_stackHealth_init(mcal_timer_t *tm, uint32_t timeout)
{
    uint32_t *endAddr = (uint32_t *)(os_device_internals_stackEndAddr_get() + 4UL);
    *endAddr = STACK_PROTECTION_CHECK_VALUE;

    stack_info.stack_size = os_device_internals_get_stack_size();

    // Init Timer 7
    mcal_timer_timerModeUS_init(tm, timeout);

    // Set the interrupt handler for Timer 7
    os_int_handler_install(TM7_OVFHANDLER, os_stackHealth_check);

    // NVIC Enable Timer 7 OVF_ISR (Number 68)
    mcal_timer_timerInterruptState_set(tm, MCAL_TIMER_INT_ENABLE);

    // Run Timer 7
    mcal_timer_timerState_set(tm, MCAL_TIMER_START);

    // Update the max MSP Address
    stack_info.max_sp_address = os_device_internals_sp_get();
}

static void os_stackHealth_updateMaxSP(void)
{
    if (stack_info.max_sp_address > os_device_internals_sp_get())
    {
        stack_info.max_sp_address = os_device_internals_sp_get();
        if (stack_info.max_sp_address == 0)
        {
            asm("NOP");
        }
    }
}

static void os_stackHealth_update_current_sp(void)
{
    stack_info.current_sp_address = os_device_internals_sp_get();
}

static void os_stackHealth_calculate_usage_ratio(const volatile uint32_t sp_addr, volatile uint16_t *used_space, volatile uint8_t *usage_ratio)
{
    uint16_t space;
    uint8_t ratio;
    
    space = (uint16_t)(os_device_internals_stackStartAddr_get() - sp_addr);
    ratio = ((uint32_t)space * 100UL) / stack_info.stack_size;
    
    *used_space = space;
    *usage_ratio = ratio;
}

void os_stackHealth_check(void)   // Periodic call
{
    // OS_INT_DISABLE;
    uint32_t *endAddr = (uint32_t *)(os_device_internals_stackEndAddr_get() + (uint32_t)4);
    uint16_t maxUsedSpace = 0;
    uint16_t stackSize = 0;

    os_stackHealth_updateMaxSP();   // out of boundary from nedic_ddm
    if (stack_info.max_sp_address == 0)
    {
        asm("NOP");
    }
    os_stackHealth_update_current_sp();

    os_stackHealth_calculate_usage_ratio(stack_info.max_sp_address, &stack_info.max_used_space, &stack_info.max_usage_ratio);
    os_stackHealth_calculate_usage_ratio(stack_info.current_sp_address, &stack_info.current_used_space, &stack_info.current_usage_ratio);
    
    maxUsedSpace = stack_info.max_used_space;
    stackSize = stack_info.stack_size;

    // Check the usage size, and the protection value
    if ((maxUsedSpace >= stackSize) || (*(endAddr) != STACK_PROTECTION_CHECK_VALUE))
    {
        stack_info.protection_value_currupted = 1;   // Report Stack Overflow
    }
    // OS_INT_ENABLE;
}
