#ifndef APP_NM_TUBCLEAN_MOTOR_H
#define APP_NM_TUBCLEAN_MOTOR_H

#include "def.h"

#include "app_motor.h"

typedef app_motor_motionPattern_t app_nm_tubclean_motor_motionPattern_t;

typedef enum app_nm_tubclean_steamMH_motor_stirState
{
    TUBCLEAN_STEAM_TECH_MH,
    TUBCLEAN_STEAM_TECH_MH_STOP,
    TUBCLEAN_STEAM_TECH_MH_STATE_NUM
} app_nm_tubclean_steamMH_motor_stirState_t;

typedef enum app_nm_tubclean_steamMD_motor_stirState
{
    TUBCLEAN_STEAM_TECH_MD,
    TUBCLEAN_STEAM_TECH_MD_STOP,
    TUBCLEAN_STEAM_TECH_MD_STATE_NUM
} app_nm_tubclean_steamMD_motor_stirState_t;

typedef enum app_nm_tubclean_soak_motor_stirState
{
    TUBCLEAN_SOAK_OPERATION,
    TUBCLEAN_SOAK_STOP
} app_nm_tubclean_soak_motor_stirState_t;

typedef struct app_nm_tubclean_motor_MHStateTime
{
    uint32_t stirState[TUBCLEAN_STEAM_TECH_MH_STATE_NUM];
} app_nm_tubclean_motor_MHStateTime_t;

typedef struct app_nm_tubclean_motor_MDStateTime
{
    uint32_t stirState[TUBCLEAN_STEAM_TECH_MD_STATE_NUM];
} app_nm_tubclean_motor_MDStateTime_t;

typedef struct app_nm_tubclean_motor_statePattern
{
    app_nm_tubclean_motor_motionPattern_t mhPattern;
    app_nm_tubclean_motor_motionPattern_t mdPattern;
    app_nm_tubclean_motor_motionPattern_t fillPattern;
    app_nm_tubclean_motor_motionPattern_t soakPattern;
    app_nm_tubclean_motor_motionPattern_t washPattern;
} app_nm_tubclean_motor_statePattern_t;

typedef struct app_nm_tubclean_motor_drainSpinPattern
{
    uint32_t drainSpinTimeoutMSec;
    uint32_t drainSpinRPM;
    uint32_t drainSpinRateMSec;
} app_nm_tubclean_motor_drainSpinPattern_t;

void app_nm_tubclean_motor_process(uint32_t period);

const app_nm_tubclean_motor_statePattern_t *app_nm_tubclean_motorMotionPattern_get(void);
uint32_t app_nm_tubclean_motorMHStateMotion_get(app_nm_tubclean_steamMH_motor_stirState_t state);
uint32_t app_nm_tubclean_motorMDStateMotion_get(app_nm_tubclean_steamMD_motor_stirState_t state);

uint32_t app_nm_tubclean_soak_motorStateTable_get(app_nm_tubclean_soak_motor_stirState_t state);

uint32_t app_nm_tubclean_motor_drainSpinStateData_get(void);
const app_motor_spinProfileSectionData_t *app_nm_tubclean_drainSpin_motorMotion_get(void);

const app_motor_spinProfileSectionData_t *app_nm_tubclean_spin_motorMotion_get(void);

#endif
