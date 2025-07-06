#include "def.h"

#include "app_startup.h"
#include "app_nm.h"
#include "app_memory.h"
#include "app_buzzer.h"

#include "app_processor.h"
#include "app_nm_log.h"

#include "handlers/input/inc/handler_crc.h"
#include "handlers/output/inc/handler_relay.h"

#define STARTUP_TIMEOUT_MSEC          (800U)
#define STARTUP_ABNORMAL_TIMEOUT_MSEC (3000U)

app_processor_state_t app_startup_process(uint32_t period)
{
    app_processor_state_t ret = APP_PROCESSOR_STARTUP_MODE;
    static uint32_t timeoutMSec = 0U;

    handler_mainRelay_state_set(RELAY_ACTIVE);
    handler_DDMRelay_state_set(RELAY_ACTIVE);

    timeoutMSec += period;
    if (((MEMORY_INIT_DONE == app_memory_initState_get()) && (timeoutMSec >= STARTUP_TIMEOUT_MSEC)) || (timeoutMSec >= STARTUP_ABNORMAL_TIMEOUT_MSEC))
    {
        timeoutMSec = 0;
        ret = APP_PROCESSOR_SETUP_MODE;
        app_buzzer_tone_set(APP_BUZZER_TONE_STARTUP);
    }

    if (0 != handler_crc_error_get())
    {
        app_nm_log_error_set(APP_NORMAL_MODE_LOG_ERROR_CRC_MISMATCH_FAILURE);
    }

    return ret;
}
