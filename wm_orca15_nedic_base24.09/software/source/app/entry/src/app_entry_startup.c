#include "def.h"

#include "handlers/input/inc/handler_off_sw.h"
#include "handlers/input/inc/handler_acin.h"
#include "handlers/input/inc/handler_crc.h"
#include "handlers/input/inc/handler_memory.h"
#include "handlers/input/inc/handler_keypad.h"
#include "handlers/input/inc/handler_lid_sensor.h"
#include "handlers/input/inc/handler_mems.h"
#include "handlers/input/inc/handler_temp_sensor.h"
#include "handlers/input/inc/handler_water_level.h"
#include "handlers/input/inc/handler_power_detection.h"
#include "handlers/input/inc/handler_load_sense.h"
#include "handlers/input/inc/handler_detergent_sensor.h"
#include "handlers/input/inc/handler_softener_sensor.h"

#include "handlers/output/inc/handler_buzzer.h"
#include "handlers/output/inc/handler_display.h"
#include "handlers/output/inc/handler_clutch.h"
#include "handlers/output/inc/handler_drain_pump.h"
#include "handlers/output/inc/handler_softener_pump.h"
#include "handlers/output/inc/handler_detergent_pump.h"
#include "handlers/output/inc/handler_drain_valve.h"
#include "handlers/output/inc/handler_lock_motor.h"
#include "handlers/output/inc/handler_motor.h"
#include "handlers/output/inc/handler_relay.h"
#include "handlers/output/inc/handler_water_valve.h"
#include "handlers/output/inc/handler_com.h"
#include "handlers/output/inc/handler_heater.h"

#if DDM_MODEL == DDM_INTERNAL
#include "McMain.h"
#endif

void app_entry_fastStartup_init(void)
{
    // do nothing
}

void app_entry_startup_init(void)
{
    // NOTE: inputs
    handler_offSW_init();
    handler_acin_init();
    handler_crc_init();
    handler_keypad_init();
    handler_lidSensor_init();
    handler_loadSense_init();
    handler_memory_init();
    handler_mems_init();
    handler_powerDetection_init();
    handler_tempSensor_init();
    handler_waterLevel_init();
    handler_detergentSensor_init();
    handler_softenerSensor_init();

    // NOTE: outputs
    handler_buzzer_init();
    handler_clutch_init();
    handler_com_init();
    handler_display_init();
    handler_drainPump_init();
    handler_softenerPump_init();
    handler_detergentPump_init();
    handler_heater_init();
    handler_lockMotor_init();
    handler_motor_init();
    handler_relay_init();
    handler_waterValve_init();

#if DDM_MODEL == DDM_INTERNAL
    mc_app_entry_startup_init();
#endif

    // os_stackHealth_init(TSB_TB7, 1000);
}

uint32_t app_entry_startup_condition_check(void)
{
    return (uint32_t)handler_powerDetection_state_get();
}
