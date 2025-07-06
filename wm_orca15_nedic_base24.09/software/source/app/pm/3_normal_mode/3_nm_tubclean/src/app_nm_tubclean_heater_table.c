#include "def.h"

#include "app_nm_user_interface.h"

#include "app_nm_tubclean_heater.h"
#include "app_nm_tubclean_heater_table.h"

static const uint32_t *const steamTechONTempPtr = &tubclean_steamTechONTempTable;
static const uint32_t *const steamTechOFFTempPtr = &tubclean_steamTechOFFTempTable;

uint32_t app_nm_tubclean_heater_steamTechTargetONTemp_get(void)
{
    return *steamTechONTempPtr;
}

uint32_t app_nm_tubclean_heater_steamTechTargetOFFTemp_get(void)
{
    return *steamTechOFFTempPtr;
}
