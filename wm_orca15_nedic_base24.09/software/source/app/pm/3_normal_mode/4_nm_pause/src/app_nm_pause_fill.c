#include "def.h"

#include "app_nm_pause_fill.h"
#include "app_fill.h"

void app_nm_pause_fill_process(uint32_t period)
{
    app_fill_stop();
}
