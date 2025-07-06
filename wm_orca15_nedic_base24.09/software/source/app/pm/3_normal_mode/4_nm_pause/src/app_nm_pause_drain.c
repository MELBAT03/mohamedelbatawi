#include "def.h"
#include "app_nm_pause_drain.h"

#include "app_drain.h"

void app_nm_pause_drain_process(uint32_t period)
{
    app_drain_switch(APP_DRAIN_CLOSE);
}
