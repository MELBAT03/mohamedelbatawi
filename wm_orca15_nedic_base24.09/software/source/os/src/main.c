#include "board.h"
#include "def.h"
#include "config.h"
#include "mcal.h"
#include "os.h"
#include "os/inc/os_log.h"
#include "os/inc/os_runtime.h"
#include "os/inc/os_scheduler.h"
#include "os/inc/os_tick.h"
#include "os/inc/os_timer.h"
#include "os/inc/os_timeout.h"
#include "os/inc/os_int.h"
#include "os/inc/os_init.h"
#include "os_int_handlers.h"
#include "os_stack_health.h"
#include "app_entry.h"
#include "components/wdt/wdt.h"
#include "components/gpio/gpio.h"
#include "components/hw_timer/hw_timer.h"
#include "app_entry_startup.h"
#include "app_exit_shutdown.h"
#include "app_interrupt.h"
#include "version.h"

int main(void)
{
    app_entry_fastStartup_init();

    OS_INT_DISABLE;
    wdt_disable();

    wdt_init();
    gpio_init();
    hwTimer_init();

    os_init();
    app_interrupt_init();

    os_runTime_init();
    os_scheduler_init(OS_TICK_PERIOD_MS);
    os_scheduler_onExit_run(app_exit_shutdown);

    os_scheduler_task_add(&wdt_update, "wdt", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 0, OS_TICK_PERIOD_MS);

    app_entry_startup_init();
    app_init();

    OS_INT_ENABLE;

    while (0U == os_scheduler_startUpSync_check(&app_entry_startup_condition_check))
    {
        // NOTE: intended to trigger wdt reset
    }

    wdt_feed();

    LOG('0', 'i', "Developed By        : mnana3a");
    LOG('0', 'i', "App Version         : %d.%d", WM_SW_VERSION_MAJOR, WM_SW_VERSION_MINOR);
    LOG('0', 'i', "Compile Time & Date : %s   %s", __TIME__, __DATE__);

    os_scheduler_process();

    while (1)
    {
        // control should never get to here
    }
}
