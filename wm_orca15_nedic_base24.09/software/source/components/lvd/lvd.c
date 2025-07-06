#include <mcal_lvd.h>
#include "utils.h"
#include "components/lvd/lvd.h"
#include "mcal/mcal.h"

void lvd_init(void)
{
    mcal_onChip_vdet1Config_t mcal_onChip_vdet1Config;

    mcal_onChip_vdet1Config.vdet1_state = MCAL_ON_CHIP_VDET1_ENABLED;
    mcal_onChip_vdet1Config.vdet1_intType = MCAL_ON_CHIP_VDET1_MASKABLE_INT;
    mcal_onChip_vdet1Config.vdet1_refVolt = MCAL_ON_CHIP_VDET1_3_84V_REF;

    mcal_onChip_vdet1_init(&mcal_onChip_vdet1Config);
}
