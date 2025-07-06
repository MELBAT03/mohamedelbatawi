#include "def.h"
#include "app_nm_tubclean_table.h"

const uint32_t *app_nm_tubclean_timeTable_get(void)
{
    return gu32_app_nm_tubclean_timeMSec;
}

uint32_t app_nm_tubclean_fillWaterLevel_get(void)
{
    return TUBCLEAN_FILL_STATE_WATER_LEVEL_LITER;
}

const app_nm_tubclean_fillStatus_t *app_nm_tubclean_fillStirTable_get(uint8_t index)
{
    return &gxa_waterSupplyLevelValues_litter[index];
}
