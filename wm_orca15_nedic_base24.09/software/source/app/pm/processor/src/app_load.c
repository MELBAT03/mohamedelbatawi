#include "def.h"

#include "app_load.h"

#include "handlers/input/inc/handler_load_sense.h"

uint32_t app_load_weightG_get(void)
{
    return handler_loadSense_weightG_get();
}

uint32_t app_load_weightRaw_get(void)
{
    return handler_loadSense_weightRaw_get();
}
