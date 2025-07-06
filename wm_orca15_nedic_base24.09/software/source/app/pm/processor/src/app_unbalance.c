#include "def.h"

#include "app_unbalance.h"

#include "handlers/input/inc/handler_mems.h"

static uint8_t mems_detectionEnable_flag = 1;

app_unbalance_state_t app_unbalance_state_get(void)
{
    app_unbalance_state_t ret = APP_UNBALANCE_NOT_DETECTED;

    if (mems_detectionEnable_flag == 1)
    {
        ret = handler_mems_unbalanceDetectionFlag_get();
    }

    return ret;
}

void app_unbalance_reset(void)
{
    handler_mems_unbalance_reset();
}

void app_unbalance_detection_set(uint8_t value)
{
    mems_detectionEnable_flag = value;
}

uint8_t app_unbalance_detection_get(void)
{
    return mems_detectionEnable_flag;
}
