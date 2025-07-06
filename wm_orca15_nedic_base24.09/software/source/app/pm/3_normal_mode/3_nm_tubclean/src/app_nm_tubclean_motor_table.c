#include "def.h"

#include "app_user_interface.h"
#include "app_nm_tubclean_motor.h"
#include "app_nm_tubclean_motor_table.h"

const app_nm_tubclean_motor_statePattern_t *app_nm_tubclean_motorMotionPattern_get(void)
{
    const app_nm_tubclean_motor_statePattern_t *ret = NULL;

    ret = &(motor_tubclean_patternTable);

    return ret;
}

uint32_t app_nm_tubclean_motorMHStateMotion_get(app_nm_tubclean_steamMH_motor_stirState_t state)
{
    uint32_t ret = 0;

    if (state <= TUBCLEAN_STEAM_TECH_MH_STOP)
    {
        ret = app_nm_tubclean_motor_steamTech_MHInterStateTable.stirState[state];
    }

    return ret;
}

uint32_t app_nm_tubclean_motorMDStateMotion_get(app_nm_tubclean_steamMD_motor_stirState_t state)
{
    uint32_t ret = 0;

    if (state <= TUBCLEAN_STEAM_TECH_MD_STOP)
    {
        ret = app_nm_tubclean_motor_steamTech_MDInterStateTable.stirState[state];
    }

    return ret;
}

uint32_t app_nm_tubclean_soak_motorStateTable_get(app_nm_tubclean_soak_motor_stirState_t state)
{
    uint32_t ret = 0;

    if (state <= TUBCLEAN_SOAK_STOP)
    {
        ret = tubclean_motor_soak_stateTable[state];
    }

    return ret;
}

uint32_t app_nm_tubclean_motor_drainSpinStateData_get(void)
{
    return TUBCLEAN_DRAIN_SPIN_TIMEOUT_MSEC;
}

const app_motor_spinProfileSectionData_t *app_nm_tubclean_drainSpin_motorMotion_get(void)
{
    return &motor_tubclean_spinDrainSection;
}

const app_motor_spinProfileSectionData_t *app_nm_tubclean_spin_motorMotion_get(void)
{
    return &motor_tubclean_spinSection;
}
