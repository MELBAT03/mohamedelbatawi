#ifndef APP_FAULT_H
#define APP_FAULT_H

#include "def.h"

#include "app_processor.h"

typedef enum app_fault_childLockProcedure
{
    CHILD_LOCK_PROCEDURE_INACTIVE = 0,
    CHILD_LOCK_PROCEDURE_ACTIVE = 1,
    CHILD_LOCK_PROCEDURE_DONE = 2
} app_fault_childLockProcedure_t;

app_processor_state_t app_fault_process(uint32_t period);

app_fault_childLockProcedure_t app_fault_childLockState_get(void);

#endif
