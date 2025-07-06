#include "handlers/output/inc/handler_motor.h"
#include "def.h"
#include "os.h"

void handler_loadSense_init(void)
{
    // do nothing
}

uint32_t handler_loadSense_weightG_get(void)
{
    uint32_t weightG = 0;
    uint32_t weightFeedback = 0;

    weightFeedback = handler_motor_weightFeedback_get();

    if (weightFeedback)
    {
        weightG = weightFeedback;

        if (handler_motor_weightRawFeedback_get() == 0)
        {
            weightG = 10000;
        }
        else if (weightG == 0)
        {
            weightG = 1000;
        }
        else if (weightG >= 10000)
        {
            weightG = 10000;
        }
        else
        {
            // do nothing
        }
    }

    return weightG;
}

void handler_loadSense_weightG_set(uint32_t value)
{
    handler_motor_weightFeedback_set(value);
}

uint32_t handler_loadSense_weightRaw_get(void)
{
    return handler_motor_weightRawFeedback_get();
}

ret_t handler_loadSense_resetLoad_check(uint32_t reference)
{
    ret_t ret = RET_OK;

    // NOTE: 1000 grams
    if (reference > 1000)
    {
        ret = RET_NOK;
    }

    return ret;
}
