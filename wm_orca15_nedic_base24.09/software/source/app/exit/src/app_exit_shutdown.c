#include "def.h"
#include "os.h"
#include "handlers/output/inc/handler_buzzer.h"
#include "handlers/output/inc/handler_relay.h"

#include "app_exit_shutdown.h"

NO_RETURN void app_exit_shutdown(void)
{
    handler_buzzer_state_set(HANDLER_BUZZER_INACTIVE);
    handler_mainRelay_state_set(RELAY_INACTIVE);
    handler_DDMRelay_state_set(RELAY_INACTIVE);

    while (1)
    {
        // aimed to trigger wdt reset
    }
}

void app_exit_shutdown_request(void (*condition)(void))
{
    os_scheduler_shutdown_request(condition);
}
