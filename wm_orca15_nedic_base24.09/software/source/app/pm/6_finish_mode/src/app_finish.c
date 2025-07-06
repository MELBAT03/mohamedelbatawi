#include "def.h"

#include "app_finish.h"
#include "app_nm.h"
#include "app_processor.h"
#include "app_lid.h"
#include "app_motor.h"

#include "app_memory.h"
#include "app_buzzer.h"

#include "app_user_interface.h"
#include "app_nm_user_interface.h"

#include "handlers/input/inc/handler_off_sw.h"
#include "handlers/input/inc/handler_keypad.h"

app_processor_state_t app_finish_process(uint32_t period)
{
    app_processor_state_t ret = APP_PROCESSOR_FINISH_MODE;
    handler_offSW_state_t offSW = handler_offSW_state_get();

    if (app_processor_shutdownState_get() == MEMORY_SHUTDOWN_NONE)
    {
        app_processor_shutdownState_set(MEMORY_SHUTDOWN_NORMAL);
    }

    app_memory_command_set(app_processor_shutdownState_get());
    app_memory_preferedCourse_set(app_userInterface_data_get()->normalMode.common.courseSelection);

    if (offSW == HANDLER_OFF_SW_LONG_PRESSED)
    {
        app_processor_shutdownState_set(MEMORY_SHUTDOWN_SKIP);
        if (MEMORY_STORING_DONE == app_memory_storingState_get())
        {
            ret = APP_PROCESSOR_DONE_MODE;
        }
    }
    else if ((((MEMORY_STORING_DONE == app_memory_storingState_get())) && (app_lidLock_state_get() == APP_LID_LOCK_OFF) && (APP_MOTOR_HALT_OK == app_motor_haltState_get()) && (APP_MOTOR_CLUTCH_WITHOUT_BASKET == app_motor_data_get()->clutchState)) || APP_MOTOR_OPERATION_STATE_FAULT == app_motor_operationState_get())
    {
        ret = APP_PROCESSOR_DONE_MODE;
    }
    else
    {
        // do nothing
    }

    return ret;
}
