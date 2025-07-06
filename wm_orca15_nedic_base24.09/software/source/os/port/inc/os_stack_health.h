#ifndef OS_STACK_HEALTH_H
#define OS_STACK_HEALTH_H

#include "mcal.h"

typedef enum
{
    STACK_OK,
    STACK_NOK
} stackHealth_t;

/**
 * @brief Initializes stack health monitoring.
 *
 * @param tm Pointer to a timer used for stack health monitoring.
 * @param timeout The timeout value for the timer in microseconds.
 *
 * @details This function sets up a canary value at the end of the stack to detect stack overflows,
 * initializes the timer for periodic stack health checks, installs the interrupt handler for
 * stack checking, and sets the maximum stack pointer value.
 */
void os_stackHealth_init(mcal_timer_t *tm, uint32_t timeout);

uint32_t os_stackHealth_spMaxValue_get(void);

/**
 * @brief Checks the stack health by comparing the stack pointer to the stack's maximum used value
 * and checking a canary value at the end of the stack.
 *
 * @return STACK_OK if the stack is healthy, STACK_NOK if the stack is not healthy.
 *
 * @details The function checks whether the stack pointer is within the size range specified by
 * STACK_SIZE and whether the canary value at the end of the stack is still intact.
 * If both conditions are true, the function returns STACK_OK, otherwise it returns STACK_NOK.
 *
 * The first condition is a comparison between the difference of the current stack
 * pointer and the maximum allowed value for the stack pointer
 * (gu32_os_stackHealth_spMaxValue) and the product of the size of the stack
 * (STACK_SIZE) and the size of a long data type. If this condition is true, it means
 * that the current stack usage is within acceptable limits.
 *
 * The second condition checks if the value at the end of the stack (endAddr) is equal
 * to a predefined constant (STACK_PROTECTION_CHECK_VALUE). This serves as a canary
 * value to detect stack overflows. If both conditions are satisfied, it indicates that
 * the stack is healthy and the function returns STACK_OK.
 *
 * If both conditions are true, the code inside the if statement will be executed. This
 * sets a return value (ret) to STACK_OK, which presumably indicates that the stack is
 * healthy and in use as expected.
 */
void os_stackHealth_check(void);

#endif
