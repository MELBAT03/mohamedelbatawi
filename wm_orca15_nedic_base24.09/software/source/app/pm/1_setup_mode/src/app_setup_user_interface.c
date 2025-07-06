#include "def.h"

#include "app_user_interface.h"
#include "app_setup_user_interface.h"

#include "app_memory.h"
#include "app_nm_user_interface.h"
#include "app_user_interface.h"

static void userInterface_setup_handle(void);
static void common_userInterface_setup_handle(app_memory_shutdownState_t state);
static void wash_userInterface_setup_handle(app_memory_shutdownState_t state);
#ifdef DRYER_ENABLED
static void dry_userInterface_setup_handle(app_memory_shutdownState_t state);
#endif

void app_setup_userInterface_process(uint32_t period)
{
    userInterface_setup_handle();
}

static void userInterface_setup_handle(void)
{
    app_memory_shutdownState_t memState = app_memory_shutdownState_get();

    common_userInterface_setup_handle(memState);
    wash_userInterface_setup_handle(memState);
#ifdef DRYER_ENABLED
    dry_userInterface_setup_handle(memState);
#endif
}

static void common_userInterface_setup_handle(app_memory_shutdownState_t state)
{
    if (MEMORY_SHUTDOWN_NORMAL == state)
    {
        if ((app_memory_preferedCourse_get() > APP_NORMAL_MODE_UI_COURSE_NONE) && (app_memory_preferedCourse_get() < APP_NORMAL_MODE_UI_COURSE_NUM))
        {
            app_userInterface_data_set()->normalMode.common.courseSelection = (app_nm_userInterface_courseSelection_t)app_memory_preferedCourse_get();
        }
        else
        {
            app_userInterface_data_set()->normalMode.common.courseSelection = APP_NORMAL_MODE_UI_DEFAULT_COURSE;
        }
        app_userInterface_data_set()->normalMode.common.operationSelection = APP_NORMAL_MODE_UI_DEFAULT_OPERATION;
        app_userInterface_data_set()->normalMode.common.delayStartHourSelection = APP_NORMAL_MODE_UI_DEFAUL_DELAY_START;
        app_userInterface_data_set()->normalMode.common.childLockSelection = APP_NORMAL_MODE_UI_DEFAUL_CHILD_LOCK;
        app_userInterface_data_set()->normalMode.common.cycleTypeSelection = APP_NORMAL_MODE_UI_DEFAULT_CYCLE_TYPE;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_DEFAULT_TUBCLEAN;
    }
    else if (MEMORY_SHUTDOWN_ABNORMAL == state)
    {
        app_userInterface_data_set()->normalMode.common.courseSelection = (app_nm_userInterface_courseSelection_t)app_memory_data_get(MEMORY_LAYOUT_COURSE_LOCATION);
        app_userInterface_data_set()->normalMode.common.operationSelection = (app_nm_userInterface_operationSelection_t)app_memory_data_get(MEMORY_LAYOUT_OPERATION_LOCATION);
        app_userInterface_data_set()->normalMode.common.delayStartHourSelection = (app_nm_userInterface_delayStartSelection_t)(60 * app_memory_data_get(MEMORY_LAYOUT_DLEAY_START_LOCATION));
        app_userInterface_data_set()->normalMode.common.childLockSelection = (app_nm_userInterface_childLockSelection_t)app_memory_data_get(MEMORY_LAYOUT_CHILD_LOCK_LOCATION);
        app_userInterface_data_set()->normalMode.common.cycleTypeSelection = (app_nm_userInterface_cycleTypeSelection_t)app_memory_data_get(MEMORY_LAYOUT_CYCLE_TYPE_LOCATION);
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = (app_nm_userInterface_tubcleanSelection_t)app_memory_data_get(MEMORY_LAYOUT_TUBCLEAN_LOCATION);
    }
    else
    {
        // do nothing
    }
}

static void wash_userInterface_setup_handle(app_memory_shutdownState_t state)
{
    if (MEMORY_SHUTDOWN_NORMAL == state)
    {
        app_userInterface_data_set()->normalMode.wash.waterTempSelection = app_nm_wash_userInterface_defaults_data_get()->defaultWaterTemp;
        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = app_nm_wash_userInterface_defaults_data_get()->defaultWaterLevel;
        app_userInterface_data_set()->normalMode.wash.soakSelection = app_nm_wash_userInterface_defaults_data_get()->defaultSoakTime;
        app_userInterface_data_set()->normalMode.wash.washSelection = app_nm_wash_userInterface_defaults_data_get()->defaultWashTime;
        app_userInterface_data_set()->normalMode.wash.steamSelection = app_nm_wash_userInterface_defaults_data_get()->defaultSteam;
        app_userInterface_data_set()->normalMode.wash.rinseSelection = app_nm_wash_userInterface_defaults_data_get()->defaultRinseTime;
        app_userInterface_data_set()->normalMode.wash.extraRinseSelection = app_nm_wash_userInterface_defaults_data_get()->defaultExtraRinse;
        app_userInterface_data_set()->normalMode.wash.spinSelection = app_nm_wash_userInterface_defaults_data_get()->defaultSpinTime;
        app_userInterface_data_set()->normalMode.wash.soilLevelSelection = app_nm_wash_userInterface_defaults_data_get()->defaultSoilLevel;
        app_userInterface_data_set()->normalMode.wash.superSpinSelection = app_nm_wash_userInterface_defaults_data_get()->defaultSuperSpinTime;
        app_userInterface_data_set()->normalMode.wash.antiWrinkleSelection = app_nm_wash_userInterface_defaults_data_get()->defaultAntiWrinkle;
        app_userInterface_data_set()->normalMode.wash.steamTechSelection = app_nm_wash_userInterface_defaults_data_get()->defaultSteamTech;
        app_userInterface_data_set()->normalMode.wash.waterHeatingSelection = app_nm_wash_userInterface_defaults_data_get()->defaultWaterHeating;
        app_userInterface_data_set()->normalMode.wash.gelDetergentSelection = app_nm_wash_userInterface_defaults_data_get()->defaultGelDetergent;
    }
    else if (MEMORY_SHUTDOWN_ABNORMAL == state)
    {
        app_userInterface_data_set()->normalMode.wash.waterTempSelection = (app_nm_wash_userInterface_waterTempSelection_t)app_memory_data_get(MEMORY_LAYOUT_WATER_TEMP_LOCATION);
        app_userInterface_data_set()->normalMode.wash.waterLevelSelection = (app_nm_wash_userInterface_waterLevelSelection_t)app_memory_data_get(MEMORY_LAYOUT_WATER_LEVEL_LOCATION);
        app_userInterface_data_set()->normalMode.wash.soakSelection = (app_nm_wash_userInterface_soakTimeSelection_t)app_memory_data_get(MEMORY_LAYOUT_SOAK_LOCATION);
        app_userInterface_data_set()->normalMode.wash.washSelection = (app_nm_wash_userInterface_washTimeSelection_t)app_memory_data_get(MEMORY_LAYOUT_WASH_LOCATION);
        app_userInterface_data_set()->normalMode.wash.steamSelection = (app_nm_wash_userInterface_steamSelection_t)app_memory_data_get(MEMORY_LAYOUT_STEAM_LOCATION);
        app_userInterface_data_set()->normalMode.wash.rinseSelection = (app_nm_wash_userInterface_rinseTimesSelection_t)app_memory_data_get(MEMORY_LAYOUT_RINSE_LOCATION);
        app_userInterface_data_set()->normalMode.wash.extraRinseSelection = (app_nm_wash_userInterface_extraRinseSelection_t)app_memory_data_get(MEMORY_LAYOUT_ERINSE_LOCATION);
        app_userInterface_data_set()->normalMode.wash.spinSelection = (app_nm_wash_userInterface_spinTimeSelection_t)app_memory_data_get(MEMORY_LAYOUT_SPIN_LOCATION);
        app_userInterface_data_set()->normalMode.wash.soilLevelSelection = (app_nm_wash_userInterface_soilLevelSelection_t)app_memory_data_get(MEMORY_LAYOUT_SOIL_LEVEL_LOCATION);
        app_userInterface_data_set()->normalMode.wash.superSpinSelection = (app_nm_wash_userInterface_superSpinSelection_t)app_memory_data_get(MEMORY_LAYOUT_SUPER_SPIN_LOCATION);
        app_userInterface_data_set()->normalMode.wash.antiWrinkleSelection = (app_nm_wash_userInterface_antiWrinkleSelection_t)app_memory_data_get(MEMORY_LAYOUT_ANTIWRINKLE_LOCATION);
        app_userInterface_data_set()->normalMode.wash.waterHeatingSelection = (app_nm_wash_userInterface_waterHeatingSelection_t)app_memory_data_get(MEMORY_LAYOUT_WATER_HEATING_LOCATION);
        app_userInterface_data_set()->normalMode.wash.steamTechSelection = (app_nm_wash_userInterface_steamTechSelection_t)app_memory_data_get(MEMORY_LAYOUT_STEAM_TECH_LOCATION);
        app_userInterface_data_set()->normalMode.wash.gelDetergentSelection = (app_nm_wash_userInterface_gelDetergentSelection_t)app_memory_data_get(MEMORY_LAYOUT_GEL_DETERGENT_LOCATION);
    }
    else
    {
        // do nothing
    }
}

#ifdef DRYER_ENABLED
static void dry_userInterface_setup_handle(app_memory_shutdownState_t state)
{
    if (MEMORY_SHUTDOWN_NORMAL == state)
    {
        app_userInterface_data_set()->normalMode.dry.dryTimeSelection = app_nm_dry_userInterface_defaults_data_get()->defaultDryTime;
        app_userInterface_data_set()->normalMode.dry.dryIntensitySelection = app_nm_dry_userInterface_defaults_data_get()->defaultDryIntensity;
        app_userInterface_data_set()->normalMode.dry.dryWaterLevel = app_nm_dry_userInterface_defaults_data_get()->defaultDryWaterLevel;
    }
    else if (MEMORY_SHUTDOWN_ABNORMAL == state)
    {
        app_userInterface_data_set()->normalMode.dry.dryTimeSelection = app_memory_data_get(MEMORY_LAYOUT_DRY_TIME_LOCATION);
        app_userInterface_data_set()->normalMode.dry.dryIntensitySelection = app_memory_data_get(MEMORY_LAYTOUT_DRY_INTENSITY_LOCATION);
        app_userInterface_data_set()->normalMode.dry.dryWaterLevel = app_memory_data_get(MEMORY_LAYTOUT_DRY_WATER_LEVEL_LOCATION);
    }
    else
    {
        // do nothing
    }
}
#endif
