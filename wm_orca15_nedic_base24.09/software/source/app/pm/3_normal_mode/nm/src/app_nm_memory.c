#include "def.h"

#include "app_memory.h"
#include "app_nm_memory.h"

#include "app_nm.h"
#include "app_nm_wash.h"
#ifdef DRYER_ENABLED
#include "app_nm_dry.h"
#endif
#include "app_nm_tubclean.h"
#include "app_nm_init.h"

static void nm_memory_logging_handle(void);

void app_nm_memory_process(uint32_t period)
{
    nm_memory_logging_handle();
}

static void nm_memory_logging_handle(void)
{
    app_memory_data_set(MEMORY_LAYOUT_COURSE_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.common.courseSelection);
    app_memory_data_set(MEMORY_LAYOUT_OPERATION_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.common.operationSelection);
    app_memory_data_set(MEMORY_LAYOUT_DLEAY_START_LOCATION, (uint8_t)(app_nm_data_get()->delayStart / 60));
    app_memory_data_set(MEMORY_LAYOUT_CHILD_LOCK_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.common.childLockSelection);
    app_memory_data_set(MEMORY_LAYOUT_CYCLE_TYPE_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.common.cycleTypeSelection);
    app_memory_data_set(MEMORY_LAYOUT_TUBCLEAN_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.common.tubcleanSelection);

    app_memory_data_set(MEMORY_LAYOUT_WATER_TEMP_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.wash.waterTempSelection);
    app_memory_data_set(MEMORY_LAYOUT_WATER_LEVEL_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
    app_memory_data_set(MEMORY_LAYOUT_SOAK_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.wash.soakSelection);
    app_memory_data_set(MEMORY_LAYOUT_WASH_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.wash.washSelection);
    app_memory_data_set(MEMORY_LAYOUT_STEAM_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.wash.steamSelection);
    app_memory_data_set(MEMORY_LAYOUT_RINSE_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.wash.rinseSelection);
    app_memory_data_set(MEMORY_LAYOUT_ERINSE_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.wash.extraRinseSelection);
    app_memory_data_set(MEMORY_LAYOUT_SPIN_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.wash.spinSelection);
    app_memory_data_set(MEMORY_LAYOUT_SOIL_LEVEL_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.wash.soilLevelSelection);
    app_memory_data_set(MEMORY_LAYOUT_SUPER_SPIN_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.wash.superSpinSelection);
    app_memory_data_set(MEMORY_LAYOUT_ANTIWRINKLE_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.wash.antiWrinkleSelection);
    app_memory_data_set(MEMORY_LAYOUT_WATER_HEATING_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.wash.waterHeatingSelection);
    app_memory_data_set(MEMORY_LAYOUT_STEAM_TECH_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.wash.steamTechSelection);
    app_memory_data_set(MEMORY_LAYOUT_GEL_DETERGENT_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.wash.gelDetergentSelection);

#ifdef DRYER_ENABLED
    app_memory_data_set(MEMORY_LAYOUT_DRY_TIME_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.dry.dryTimeSelection);
    app_memory_data_set(MEMORY_LAYTOUT_DRY_INTENSITY_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.dry.dryIntensitySelection);
    app_memory_data_set(MEMORY_LAYTOUT_DRY_WATER_LEVEL_LOCATION, (uint8_t)app_userInterface_data_get()->normalMode.dry.dryWaterLevel);
#endif

    app_memory_data_set(MEMORY_LAYOUT_NM_STATE_LOCATION, (uint8_t)APP_NORMAL_MODE_INIT);
    app_memory_data_set(MEMORY_LAYOUT_NM_INIT_STATE_LOCATION, (uint8_t)APP_NORMAL_MODE_INIT_INIT);
    app_memory_data_set(MEMORY_LAYOUT_NM_WASH_STATE_LOCATION, (uint8_t)app_nm_wash_state_get());
    app_memory_data_set(MEMORY_LAYOUT_WASH_SETUP_STATE_LOCATION, (uint8_t)app_nm_wash_setup_state_get());
    app_memory_data_set(MEMORY_LAYOUT_WASH_SOAK_STATE_LOCATION, (uint8_t)app_nm_wash_soak_state_get());
    app_memory_data_set(MEMORY_LAYOUT_WASH_WASH_STATE_LOCATION, (uint8_t)app_nm_wash_wash_state_get());
    app_memory_data_set(MEMORY_LAYOUT_WASH_STEAM_STATE_LOCATION, (uint8_t)app_nm_wash_steam_state_get());
    app_memory_data_set(MEMORY_LAYOUT_WASH_RINSE_STATE_LOCATION, (uint8_t)app_nm_wash_rinse_state_get());
    app_memory_data_set(MEMORY_LAYOUT_WASH_SPIN_STATE_LOCATION, (uint8_t)app_nm_wash_spin_state_get());
#ifdef DRYER_ENABLED
    app_memory_data_set(MEMORY_LAYOUT_NM_DRY_STATE_LOCATION, (uint8_t)app_nm_dry_state_get());
    app_memory_data_set(MEMORY_LAYOUT_DRY_PRESETUP_STATE_LOCATION, (uint8_t)app_nm_dry_preSetup_state_get());
    app_memory_data_set(MEMORY_LAYOUT_DRY_SETUP_STATE_LOCATION, (uint8_t)app_nm_dry_setup_state_get());
    app_memory_data_set(MEMORY_LAYOUT_DRY_OPERATION_STATE_LOCATION, (uint8_t)app_nm_dry_operation_state_get());
#endif

    app_memory_data_set(MEMORY_LAYOUT_TUBCLEAN_STATE_LOCATION, (uint8_t)app_nm_tubclean_state_get());
    app_memory_data_set(MEMORY_LAYOUT_TUBCLEAN_NUM_LOCATION, (uint8_t)app_nm_data_get()->wash.tubcleanNum);

    app_memory_data_set(MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_HIGH_LOCATION, (uint8_t)(app_nm_remainingTimeMin_get() >> 8U));
    app_memory_data_set(MEMORY_LAYOUT_CURRENT_STATE_REMAINING_TIME_LOW_LOCATION, (uint8_t)(app_nm_remainingTimeMin_get() & 0x00ffU));

    app_memory_data_set(MEMORY_LAYOUT_END_MARKER_HIGH_LOCATION, 0xBE);
    app_memory_data_set(MEMORY_LAYOUT_END_MARKER_LOW_LOCATION, 0xEF);
}
