#include "def.h"

#include "app_standby_drain.h"

#include "app_drain.h"

void app_standby_drain_process(uint32_t period)
{
    app_drain_switch(APP_DRAIN_CLOSE);
}
