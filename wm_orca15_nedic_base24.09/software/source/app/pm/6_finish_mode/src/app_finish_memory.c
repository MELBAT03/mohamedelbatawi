#include "def.h"

#include "app_memory.h"

#include "app_finish_memory.h"

void app_finish_memory_process(uint32_t period)
{
    app_memory_data_store(MEMORY_STORE_TYPE_PERIODIC);
    // app_memory_data_store(MEMORY_STORE_TYPE_CONTINUOUS);
}
