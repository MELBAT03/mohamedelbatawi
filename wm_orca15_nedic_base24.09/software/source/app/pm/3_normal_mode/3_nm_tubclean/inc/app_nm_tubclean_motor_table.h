#ifndef APP_NM_TUBCLEAN_MOTOR_TABLE_H
#define APP_NM_TUBCLEAN_MOTOR_TABLE_H

#include "def.h"
#include "app_nm_tubclean_motor.h"

#define TUBCLEAN_DRAIN_SPIN_TIMEOUT_MSEC (30000U)

static const app_nm_tubclean_motor_MHStateTime_t app_nm_tubclean_motor_steamTech_MHInterStateTable = {{60000U, 120000U}};

static const app_nm_tubclean_motor_MDStateTime_t app_nm_tubclean_motor_steamTech_MDInterStateTable = {{180000U, 120000U}};

static const uint32_t tubclean_motor_soak_stateTable[] = {60000U, 240000U};

static const app_nm_tubclean_motor_statePattern_t motor_tubclean_patternTable =
    {.mhPattern = {1300, 1300, 1300, 1300, 500, 500, 60}, .mdPattern = {0, 0, 180000, 0, 500, 500, 40}, .fillPattern = {3400, 4600, 3400, 4600, 600, 600, 110}, .soakPattern = {1500, 1000, 1500, 1000, 500, 500, 105}, .washPattern = {1500, 1000, 1500, 1000, 500, 500, 105}};

static const app_motor_spinProfileSectionData_t motor_tubclean_spinDrainSection = {70U, 0, 0, 7000U};

static const app_motor_spinProfileSectionData_t motor_tubclean_spinSection = {700U, 0, 0, 90000U};

#endif
