#include "def.h"
#include "os.h"
#include "app_component_mocker.h"

#include "app_nm_dry.h"
#include "app_user_interface.h"
#include "app_nm_dry_motor.h"
#include "app_nm_dry_heater.h"
#include "app_nm_dry_drain.h"
#include "app_user_interface.h"
#include "app_nm.h"

#include "app_nm_wash.h"

#include "assert.h"
#include "app_nm_dry_drain.h"
#include "app_nm_init.h"
#include "app_fault.h"
#include "app_motor.h"
#ifdef CONDENSER_ENABLED
#include "app_condenser.h"
#include "app_nm_dry_condenser.h"
#endif

#ifdef BLOWER_ENABLED
#include "app_blower.h"
#endif
#include "app_drain.h"
#include "app_heater.h"

#include "app_processor.h"
#include "app_fault.h"
#include "app_heater.h"
#include "app_motor.h"
#include "app_drain.h"
#include "app_lid.h"
#include "app_fill.h"
#ifdef COOLING_FAN_ENABLED
#include "app_cooling_fan.h"
#endif
#include "app_controller.h"

#include "handlers/output/inc/handler_motor.h"
#include "handlers/output/inc/handler_lock_motor.h"
#include "handlers/input/inc/handler_lid_sensor.h"
#include "handlers/input/inc/handler_lid_sensor.h"

#include "handlers/input/inc/handler_water_level.h"

#define FILL_LEVEL (22000)

void app_cMocker_init(void)
{
}

void app_cMocker_update(uint32_t period)
{
    if (app_processor_entryFlag_get() == 0)
    {
        return;
    }

    switch (app_processor_state_get())
    {
        case APP_PROCESSOR_STARTUP_MODE:
        {
            app_memory_shutdownState_set(MEMORY_SHUTDOWN_NORMAL);
            app_memory_initState_set(MEMORY_INIT_DONE);
        }
        break;

        case APP_PROCESSOR_SETUP_MODE:
        {
            // TEST_ASSERT_EQUAL(APP_NORMAL_MODE_INIT, app_nm_state_get());
            // TEST_ASSERT_EQUAL(APP_NORMAL_MODE_UI_DEFAULT_COURSE, app_userInterface_data_get()->normalMode.common.courseSelection);
            // TEST_ASSERT_EQUAL(APP_NORMAL_MODE_UI_OPERATION_WASH_ONLY, app_userInterface_data_get()->normalMode.common.operationSelection);
            // TEST_ASSERT_EQUAL(APP_NORMAL_MODE_UI_DELAY_START_TIME_0_HOUR, app_userInterface_data_get()->normalMode.common.delayStartHourSelection);
            // TEST_ASSERT_EQUAL(APP_NORMAL_MODE_UI_CHILD_LOCK_OFF, app_userInterface_data_get()->normalMode.common.childLockSelection);
            // TEST_ASSERT_EQUAL(APP_NORMAL_MODE_UI_WATER_LEVEL_0, app_userInterface_data_get()->normalMode.wash.waterLevelSelection);
            // TEST_ASSERT_EQUAL(APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO, app_userInterface_data_get()->normalMode.dry.dryTimeSelection);
            // TEST_ASSERT_EQUAL(APP_NORMAL_MODE_UI_DRYNESS_INTENSITY_NORMAL, app_userInterface_data_get()->normalMode.dry.dryIntensitySelection);

            // TEST_ASSERT_EQUAL(0, app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_DRAIN]);
            // TEST_ASSERT_EQUAL(0, app_nm_data_get()->dry.preSetupTimeMSec[APP_NORMAL_MODE_DRY_PRE_SETUP_DRAIN]);
            // TEST_ASSERT_EQUAL(0, app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR]);
            // TEST_ASSERT_EQUAL(0, app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME]);
            // TEST_ASSERT_EQUAL(0, app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_COOLING_WATER]);
            // TEST_ASSERT_EQUAL(0, app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_2]);
            // TEST_ASSERT_EQUAL(0, app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_1]);
            // TEST_ASSERT_EQUAL(0, app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_HEATING]);
            // TEST_ASSERT_EQUAL(0, app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_UNTANGLE]);
            // TEST_ASSERT_EQUAL(0, app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_DRYING_SPIN]);
            // TEST_ASSERT_EQUAL(0, app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_BRAKE]);
            // TEST_ASSERT_EQUAL(0, app_nm_data_get()->dry.operationTimeMSec[APP_NORMAL_MODE_DRY_OPERATION_FINAL_SPIN]);
            // TEST_ASSERT_EQUAL(0, app_nm_data_get()->dry.preSetupTimeMSec[APP_NORMAL_MODE_DRY_PRE_SETUP_SPIN]);
            // TEST_ASSERT_EQUAL(0, app_nm_data_get()->dry.preSetupTimeMSec[APP_NORMAL_MODE_DRY_PRE_SETUP_UNTANGLE]);
        }
        break;

        case APP_PROCESSOR_STANDBY_MODE:
        {
            // do nothing
        }
        break;

        case APP_PROCESSOR_NORMAL_MODE:
        {
            switch (app_nm_state_get())
            {
                case APP_NORMAL_MODE_INIT:
                {
                    switch (app_nm_init_state_get())
                    {
                        case APP_NORMAL_MODE_INIT_INIT:
                        {
                            handler_motor_state_set(HANDLER_MOTOR_OPERATION_STATE);
                            handler_motor_speed_set(0);
                            handler_waterLevel_resetLevel_mark();
                        }
                        break;

                        case APP_NORMAL_MODE_INIT_MOTOR_CLUTCH_RESET:
                        {
                            handler_motor_speed_set(0);
                        }
                        break;

                        default:
                        {
                            // do nothing
                        }
                        break;
                    }
                }
                break;

                case APP_NORMAL_MODE_WASH:
                {
                    handler_motor_speed_set(0);

                    switch (app_nm_wash_state_get())
                    {
                        case APP_NORMAL_MODE_WASH_WASH:
                        {
                            handler_waterLevel_currentLevelFreq_set(21000);
                        }
                        break;

                        case APP_NORMAL_MODE_WASH_RINSE:
                        {
                            switch (app_nm_wash_rinse_state_get())
                            {
                                case APP_NORMAL_MODE_WASH_RINSE_DRAIN:
                                {
                                    handler_waterLevel_currentLevelFreq_set(27000);
                                    handler_waterLevel_resetLevel_mark();
                                }
                                break;

                                case APP_NORMAL_MODE_WASH_RINSE_FILL:
                                {
                                    handler_waterLevel_currentLevelFreq_set(21000);
                                }
                                break;

                                default:
                                {
                                    // do nothing
                                }
                                break;
                            }
                        }
                        break;

                        default:
                        {
                            // do nothing
                        }
                        break;
                    }
                }
                break;

#ifdef DRYER_ENABLED
                case APP_NORMAL_MODE_DRY:
                {
                    switch (app_nm_dry_state_get())
                    {
                        case APP_NORMAL_MODE_DRY_INIT:
                        case APP_NORMAL_MODE_DRY_SETUP:
                        case APP_NORMAL_MODE_DRY_OPERATION:
                        case APP_NORMAL_MODE_DRY_FINISH:
                        case APP_NORMAL_MODE_DRY_DONE:
                        {
                            handler_motor_speed_set(0);
                        }
                        break;

                        case APP_NORMAL_MODE_DRY_PRE_SETUP:
                        {
                            switch (app_nm_dry_preSetup_state_get())
                            {
                                case APP_NORMAL_MODE_DRY_PRE_SETUP_INIT:
                                case APP_NORMAL_MODE_DRY_PRE_SETUP_UNTANGLE:
                                case APP_NORMAL_MODE_DRY_PRE_SETUP_FINISH:
                                case APP_NORMAL_MODE_DRY_PRE_SETUP_DONE:
                                {
                                    // do nothing
                                }
                                break;

                                case APP_NORMAL_MODE_DRY_PRE_SETUP_DRAIN:
                                case APP_NORMAL_MODE_DRY_PRE_SETUP_SPIN:
                                {
                                    handler_waterLevel_resetLevel_mark();
                                    handler_motor_speed_set(0);
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
                break;
#endif

                case APP_NORMAL_MODE_PAUSE:
                {
                    // do nothing
                }
                break;

                case APP_NORMAL_MODE_FINISH:
                {
                    handler_lockMotor_state_set((handler_lockMotor_state_t)0);
                    app_motor_haltState_set(APP_MOTOR_HALT_OK);
                }
                break;

                default:
                {
                    // do nothing
                }
                break;
            }
        }
        break;

        case APP_PROCESSOR_FAULT_MODE:
        {
            // do nothing
        }
        break;

        case APP_PROCESSOR_FINISH_MODE:
        {
            app_memory_storingState_set(MEMORY_STORING_DONE);
        }
        break;

        case APP_PROCESSOR_DONE_MODE:
        {
        }
        break;

        default:
        {
            // do nothing
        }
        break;
            // }
    }
}
