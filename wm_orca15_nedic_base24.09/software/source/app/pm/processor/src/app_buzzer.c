#include "def.h"

#include "app_buzzer.h"

#include "handlers/output/inc/handler_buzzer.h"

void app_buzzer_tone_set(app_buzzer_tone_t tone)
{
    handler_buzzer_tone_set(tone);
}
