#include "def.h"

#include "app_nm_dry_fill.h"
#include "app_fill.h"

void app_nm_dry_fill_process(uint32_t period)
{
    app_fill_stop();

    // TODO: add error checking using the water level sensor so if there is water in the tub. sound.
}
