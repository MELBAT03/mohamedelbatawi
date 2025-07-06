#include "def.h"
#include "config.h"

#include "app_nm_user_interface.h"

#include "app_nm_wash.h"
#include "app_nm_wash_user_interface.h"
#include "app_motor.h"
#include "app_load.h"
#include "app_lid.h"

#include "app_nm_log.h"

#define WASH_DETERGENT_ADD_TIMEOUT_MSEC (20000U)

static void init_handle(uint32_t period);
static void checkWaterLevel_handle(uint32_t period);
static void weightDetection_handle(uint32_t period);
static void detergentAdd_handle(uint32_t period);
static void finish_handle(uint32_t period);

static uint8_t wash_setup_userAction_monitor(void);

static app_nm_wash_setup_state_t gx_app_wash_setup_state = APP_NORMAL_MODE_WASH_SETUP_INIT;

static uint32_t weightData;
static uint8_t overLoad_flag = 0;

void app_nm_wash_setup_process(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) = {
        init_handle,
        checkWaterLevel_handle,
        weightDetection_handle,
        detergentAdd_handle,
        finish_handle,
        NULL};

    if (APP_NORMAL_MODE_WASH_SETUP == app_nm_wash_state_get())
    {
        if (handlers[gx_app_wash_setup_state] != NULL)
        {
            handlers[gx_app_wash_setup_state](period);
        }
    }
}

app_nm_wash_setup_state_t app_nm_wash_setup_state_get(void)
{
    return gx_app_wash_setup_state;
}

void app_nm_wash_setup_state_set(app_nm_wash_setup_state_t value)
{
    gx_app_wash_setup_state = value;
}

uint32_t app_nm_wash_setup_weightData_get(void)
{
    return weightData;
}

void app_nm_wash_setup_reset(void)
{
    gx_app_wash_setup_state = APP_NORMAL_MODE_WASH_SETUP_INIT;
}

uint8_t app_nm_wash_setup_overloadFlag_get(void)
{
    return overLoad_flag;
}
void app_nm_wash_setup_overloadFlag_set(uint8_t value)
{
    overLoad_flag = value;
}

static void init_handle(uint32_t period)
{
    if ((APP_MOTOR_HALT_OK == app_motor_haltState_get()) && (APP_CLUTCH_RESET_OK == app_motor_clutchResetState_get()))
    {
        gx_app_wash_setup_state = APP_NORMAL_MODE_WASH_SETUP_CHECK_WATER_LEVEL;
    }
}
static void checkWaterLevel_handle(uint32_t period)
{
    if (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0 == app_userInterface_data_get()->normalMode.wash.waterLevelSelection)
    {
        gx_app_wash_setup_state = APP_NORMAL_MODE_WASH_SETUP_WEIGHT_DETECTION;
    }
    else
    {
        gx_app_wash_setup_state = APP_NORMAL_MODE_WASH_SETUP_DETERGENT_ADD;
    }
}
static void weightDetection_handle(uint32_t period)
{
    weightData = app_load_weightG_get();

    if (0 != weightData)
    {
        weightData = weightData / 1000;   // get value in kg
        if (weightData < 1)
        {
            weightData = 1;
        }

        if ((app_nm_wash_userInterface_overloadingFactor_get() != 0) && (weightData > app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterLevel))
        {
            weightData = app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterLevel;
            overLoad_flag = 1;
        }
        else if ((app_nm_wash_userInterface_overloadingFactor_get() == 0) && (weightData > app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterLevel))
        {
            weightData = app_nm_wash_userInterface_highLimitDefaults_data_get()->defaultWaterLevel;
        }
        else
        {
            // do nothing
        }

        if (APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0 != app_userInterface_data_get()->normalMode.wash.waterLevelSelection)
        {
            gx_app_wash_setup_state = APP_NORMAL_MODE_WASH_SETUP_DETERGENT_ADD;
        }
    }
}
static void detergentAdd_handle(uint32_t period)
{
    static uint32_t detergentAddTimeoutMSec = 0;

    detergentAddTimeoutMSec += period;
    if ((detergentAddTimeoutMSec >= WASH_DETERGENT_ADD_TIMEOUT_MSEC) || (0 != wash_setup_userAction_monitor()))
    {
        detergentAddTimeoutMSec = 0;
        gx_app_wash_setup_state = APP_NORMAL_MODE_WASH_SETUP_FINISH;
    }
}
static void finish_handle(uint32_t period)
{
    gx_app_wash_setup_state = APP_NORMAL_MODE_WASH_SETUP_DONE;
}

static uint8_t wash_setup_userAction_monitor(void)
{
#ifdef USER_DETERGENT_SKIP_ENABLE
    uint8_t ret = 0;
    static uint8_t moveOn_flag = 0;
    app_lid_sensorState_t lidState = app_lidSensor_state_get();

    if (APP_LID_SENSOR_OPENED == lidState)
    {
        moveOn_flag = 1;
    }

    if ((APP_LID_SENSOR_CLOSED == lidState) && (0 != moveOn_flag))
    {
        moveOn_flag = 0;
        ret = 1;
    }

    return ret;
#else
    return 0;
#endif
}
