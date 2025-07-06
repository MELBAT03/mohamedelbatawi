#include "def.h"

#include "app_nm_finish_lid.h"

#include "app_nm.h"
#include "app_lid.h"

#include "app_motor.h"

void app_nm_finish_lid_process(uint32_t period)
{
    app_lidLock_state_set(APP_LID_LOCK_ON);
}
