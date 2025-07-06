#include "def.h"

#include "app_drain.h"
#include "app_fill.h"
#include "app_motor.h"

#include "app_nm_wash_spin.h"
#include "app_nm_wash_rinse.h"
#include "app_nm_wash_rinse_table.h"

#include "app_nm_wash.h"
#include "app_user_interface.h"

static const app_nm_wash_spin_unbalance_stateTime_t courseX_spinUnbalanceTimeTable = {
    .drainTimeMSec = APP_DRAIN_WATER_ESTIMATED_INTERVAL, .fillTimeMSec = APP_FILL_TIME_MSEC, .stirTimeMSec = 240000U, .superSpinStirTimeMSec = 10000U};

static const app_nm_wash_spin_stateTime_t courseX_spinStateTimeTable = {
    {0, APP_DRAIN_WATER_ESTIMATED_INTERVAL, 540000U, 0, APP_MOTOR_SPIN_BRAKE_TIME_MSEC, 0}};

const app_nm_wash_spin_stateTime_t *app_nm_wash_spin_stateTime_get(app_nm_userInterface_courseSelection_t course)
{
    static app_nm_wash_spin_stateTime_t internalData;
    const app_nm_wash_spin_stateTime_t *ret = NULL;

    if ((course > APP_NORMAL_MODE_UI_COURSE_NONE) && (course < APP_NORMAL_MODE_UI_COURSE_NUM))
    {
        internalData = courseX_spinStateTimeTable;

        if (app_drain_shortDrainFlag_get() == 1U)
        {
            internalData.state[APP_NORMAL_MODE_WASH_SPIN_DRAIN] = APP_DRAIN_SHORT_WATER_ESTIMATED_INTERVAL;
        }

        internalData.state[APP_NORMAL_MODE_WASH_SPIN_SPIN] = app_userInterface_data_get()->normalMode.wash.spinSelection * 60 * 1000;
        internalData.state[APP_NORMAL_MODE_WASH_SPIN_SUPER_SPIN] = app_userInterface_data_get()->normalMode.wash.superSpinSelection * 60 * 1000;
        internalData.state[APP_NORMAL_MODE_WASH_SPIN_ANTI_WRINKLE] = app_userInterface_data_get()->normalMode.wash.antiWrinkleSelection * 60 * 1000;

        ret = &internalData;
    }

    return ret;
}

const app_nm_wash_spin_unbalance_stateTime_t *app_nm_wash_spin_unabalanceStateTime_get(app_nm_userInterface_courseSelection_t course, app_nm_wash_userInterface_waterLevelSelection_t waterLevel)
{
    const app_nm_wash_spin_unbalance_stateTime_t *ret = NULL;

#ifdef REMAINING_TIME_ALWAYS_IN_STANDBY
    if (waterLevel == APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0)
    {
        waterLevel = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_DURATION_DEFAULT;
    }
#endif

    if (((course > APP_NORMAL_MODE_UI_COURSE_NONE) && (course < APP_NORMAL_MODE_UI_COURSE_NUM)) && ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1) && (waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT)))
    {
        ret = &(courseX_spinUnbalanceTimeTable);
    }

    return ret;
}

uint32_t app_nm_wash_spin_unbalanceStateWaterLevel_get(app_nm_wash_userInterface_waterLevelSelection_t waterLevel)
{
    uint32_t ret = 0;

#ifdef REMAINING_TIME_ALWAYS_IN_STANDBY
    if (waterLevel == APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0)
    {
        waterLevel = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_DURATION_DEFAULT;
    }
#endif

    if ((waterLevel >= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1) && (waterLevel <= APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_LIMIT))
    {
        ret = rinse_waterLevelTable[waterLevel - 1].fillWaterLevelLiter;
    }

    return ret;
}
