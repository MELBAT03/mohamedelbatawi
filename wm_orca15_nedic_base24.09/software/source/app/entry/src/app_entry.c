#include "app_entry.h"
#include "board.h"
#include "config.h"
#include "def.h"
#include "os.h"
#include "utils.h"
#include "version.h"

#include "app_processor.h"
#include "app_display.h"
#include "app_nm.h"
#include "app_nm_dry.h"
#include "app_heater.h"
#include "app_nm_dry_heater.h"
#include "app_drain.h"
#ifdef CONDENSER_ENABLED
#include "app_condenser.h"
#endif
#ifdef COOLING_FAN_ENABLED
#include "app_cooling_fan.h"
#endif
#include "app_lid.h"
#ifdef BLOWER_ENABLED
#include "app_blower.h"
#endif
#include "app_fill.h"
#include "app_controller.h"
#include "app_component_mocker.h"

#include "handlers/input/inc/handler_acin.h"
#include "handlers/input/inc/handler_keypad.h"
#include "handlers/input/inc/handler_lid_sensor.h"
#include "handlers/input/inc/handler_mems.h"
#include "handlers/input/inc/handler_temp_sensor.h"
#include "handlers/input/inc/handler_water_level.h"
#include "handlers/input/inc/handler_power_detection.h"

#include "handlers/output/inc/handler_buzzer.h"
#include "handlers/output/inc/handler_drain_pump.h"
#include "handlers/output/inc/handler_drain_valve.h"
#include "handlers/output/inc/handler_lock_motor.h"
#include "handlers/output/inc/handler_motor.h"
#include "handlers/output/inc/handler_relay.h"
#include "handlers/output/inc/handler_water_valve.h"
#include "handlers/output/inc/handler_clutch.h"
#include "handlers/output/inc/handler_display.h"
#include "handlers/output/inc/handler_detergent_pump.h"
#include "handlers/output/inc/handler_softener_pump.h"

#include "components/ainput_module/ainput_module.h"
#include "components/buzzer/buzzer.h"
#include "components/dinput_module/dinput_module.h"
#include "components/doutput_module/doutput_module.h"
#include "components/finput_module/finput_module.h"
#include "components/gpio/gpio.h"
#include "components/heartbeat/heartbeat.h"
#include "components/kx022/kx022.h"
#include "components/s15d/s15d.h"
#include "components/stwi/stwi.h"
#include "components/uart/uart.h"
#include "components/wl90at/wl90at.h"
#include "components/wdt/wdt.h"
#include "components/hm15n/hm15n.h"
#include "components/ptc_heater/ptc_heater.h"
#include "components/ntc_temp_sensor/ntc_temp_sensor.h"
#include "components/acin/acin.h"
#include "components/zcd/zcd.h"
#include "components/com_display/com_display.h"
#include "components/ssd/ssd.h"

#include "app_processor.h"
#include "app_nm.h"
#include "app_motor.h"
#include "app_user_interface.h"
#include "app_fault.h"

#if DDM_MODEL == DDM_INTERNAL
#include "McMain.h"
#endif

#ifndef APP_TICK_MULTIPLIER
#define APP_TICK_MULTIPLIER (OS_SCH_MULTIPLIER_NONE)
#endif

#define DDM_MOTOR_STABILIZATION_TIMEOUT_MSEC (2000U)
#define MEMS_STABILIZATION_TIMEOUT_MSEC      (1000U)

void system_test_update(uint32_t period);

const int appVersion = (10U * WM_SW_VERSION_MAJOR) + WM_SW_VERSION_MINOR;

void app_init(void)
{
    os_scheduler_task_add(buzzer_update, "buz", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 5U, BUZZER_UPDATE_PERIOD);
    os_scheduler_task_add(uart_update, "urt", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 15U, UART_UPDATE_PERIOD_MS);

#ifdef ZCD_SYNC_ENABLE
    // NOTE: moved to zcd isr to sync the trigger of triac with the zcd
    os_scheduler_task_add(doutputModule_update, "dot", EVENT_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 0U, DOUTPUT_MODULE_UPDATE_PERIOD_MS);
#else
    os_scheduler_task_add(doutputModule_update, "dot", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 0U, DOUTPUT_MODULE_UPDATE_PERIOD_MS);
#endif

    os_scheduler_task_add(finputModule_update, "fin", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 0U, FINPUT_MODULE_UPDATE_PERIOD_MS);
    os_scheduler_task_add(keypad_update, "pad", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 0U, 10);
    os_scheduler_task_add(acin_update, "ac", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 0U, ACIN_UPDATE_PERIOD_MS);

    os_scheduler_task_add(ainputModule_update, "ain", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 5U, AINPUT_MODULE_UPDATE_PERIOD_MS);
    os_scheduler_task_add(zcd_update, "zcd", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 5U, ZCD_UPDATE_PERIOD_MS);
    os_scheduler_task_add(com_display_update, "dco", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 5U, 10);

    os_scheduler_task_add(dinputModule_update, "din", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 10U, DINPUT_MODULE_UPDATE_PERIOD_MS);
    os_scheduler_task_add(hm15n_update, "hm", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 10U, HM15N_UPDATE_PERIOD_MS);

    os_scheduler_task_add(dls35s_update, "dls", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 15U, DLS_LOCK_UPDATE_PERIOD_MS);

    os_scheduler_task_add(kx022_update, "kx", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, MEMS_STABILIZATION_TIMEOUT_MSEC, MEMS_UPDATE_PERIOD_MS);
    os_scheduler_task_add(DDM_update, "ndc", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 15U + DDM_MOTOR_STABILIZATION_TIMEOUT_MSEC, DDM_MOTOR_UPDATE_PERIOD_MS);

    os_scheduler_task_add(ptcHeater_update, "ptc", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 10U, PTC_HEATER_UPDATE_PERIOD_MS);
    os_scheduler_task_add(ntc_update, "ntc", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 15U, NTC_UPDATE_PERIOS_MS);

    os_scheduler_task_add(wl90at_update, "wl", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 0U, WATER_LEVEL_SENSOR_UPDATE_MS);

#ifdef COMP_MOCKER_ENABLE
    app_cMocker_init();
    os_scheduler_task_add(app_cMocker_update, "cmk", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 0U, APP_UPDATE_PERIOD_MS);
#endif

    os_scheduler_task_add(app_controller_update, "cnt", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 20U, APP_UPDATE_PERIOD_MS);

    os_scheduler_task_add(app_userInterface_update, "ui", TIME_BASED_TASK, APP_TICK_MULTIPLIER, 25U, 40U);
    os_scheduler_task_add(app_motor_update, "mot", TIME_BASED_TASK, APP_TICK_MULTIPLIER, 30U, APP_UPDATE_PERIOD_MS);
    os_scheduler_task_add(app_heater_update, "het", TIME_BASED_TASK, APP_TICK_MULTIPLIER, 35U, APP_UPDATE_PERIOD_MS);
    // os_scheduler_task_add(app_blower_update, "blw", TIME_BASED_TASK, APP_TICK_MULTIPLIER, 40U, APP_UPDATE_PERIOD_MS);
    // os_scheduler_task_add(app_condenser_update, "cnd", TIME_BASED_TASK, APP_TICK_MULTIPLIER, 45U, APP_UPDATE_PERIOD_MS);
    // os_scheduler_task_add(app_coolingFan_update, "cfn", TIME_BASED_TASK, APP_TICK_MULTIPLIER, 50U, APP_UPDATE_PERIOD_MS);
    os_scheduler_task_add(app_drain_update, "dra", TIME_BASED_TASK, APP_TICK_MULTIPLIER, 40U, APP_UPDATE_PERIOD_MS);
    os_scheduler_task_add(app_fill_update, "fil", TIME_BASED_TASK, APP_TICK_MULTIPLIER, 45U, APP_UPDATE_PERIOD_MS);
    os_scheduler_task_add(app_lid_update, "lid", TIME_BASED_TASK, APP_TICK_MULTIPLIER, 55U, APP_UPDATE_PERIOD_MS);
    os_scheduler_task_add(app_memory_update, "mem", TIME_BASED_TASK, APP_TICK_MULTIPLIER, 60U, APP_MEMORY_UPDATE_PERIOD_MS);
    os_scheduler_task_add(app_processor_update, "prc", TIME_BASED_TASK, APP_TICK_MULTIPLIER, 65U, APP_UPDATE_PERIOD_MS);
    os_scheduler_task_add(app_display_update, "dsp", TIME_BASED_TASK, APP_TICK_MULTIPLIER, 70U, 20U);

    // #ifdef SYSTEM_TEST
    // os_scheduler_task_add(system_test_update, "sts", TIME_BASED_TASK, APP_TICK_MULTIPLIER, 2000U, 2000U);
    // #endif

    // os_scheduler_task_add(heartbeat_update, "hb", TIME_BASED_TASK, OS_SCH_MULTIPLIER_NONE, 500U, HEARTBEAT_UPDATE_PERIOD_MS);

#if DDM_MODEL == DDM_INTERNAL
    mc_app_init();
#endif
}

void system_test_update(uint32_t period)
{
#if 0
    static uint32_t cnt = 0;

    handler_mainRelay_state_set(RELAY_ACTIVE);

    cnt += period;

    if (cnt >= 20000)
    {
        handler_coldWaterValve_state_set(WATER_VALVE_ACTIVE);
        handler_hotWaterValve_state_set(WATER_VALVE_ACTIVE);
        handler_softenerWaterValve_state_set(WATER_VALVE_ACTIVE);
        handler_showerWaterValve_state_set(WATER_VALVE_ACTIVE);

        handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_ACTIVE);

        handler_softenerPump_state_set(0);
        handler_drainPump_state_set(1);
        handler_detergentPump_state_set(1);

        handler_heater_plate_set(2);

        handler_clutch_switch(HM15N_ACTIVE);

        if (cnt >= 40000)
        {
            cnt = 0;
        }
    }
    else
    {
        handler_coldWaterValve_state_set(WATER_VALVE_INACTIVE);
        handler_hotWaterValve_state_set(WATER_VALVE_INACTIVE);
        handler_softenerWaterValve_state_set(WATER_VALVE_INACTIVE);
        handler_showerWaterValve_state_set(WATER_VALVE_INACTIVE);

        handler_lockMotor_state_set(HANDLER_LOCK_MOTOR_INACTIVE);

        handler_softenerPump_state_set(0);
        handler_drainPump_state_set(0);
        handler_detergentPump_state_set(1);

        handler_heater_plate_set(0);

        handler_clutch_switch(HM15N_INACTIVE);
    }
#endif

    handler_mainRelay_state_set(RELAY_ACTIVE);

    typedef enum heaterState
    {
        HEATER_ON,
        HEATER_OFF
    } heaterState_t;

    static heaterState_t heater = HEATER_ON;

    switch (heater)
    {
        case HEATER_ON:
        {
            handler_heater_plate_set(HANDLER_HEATER_PLATE_2);
            if (60U < app_heater_temp_get())
            {
                heater = HEATER_OFF;
            }
        }
        break;

        case HEATER_OFF:
        {
            handler_heater_plate_set(HANDLER_HEATER_OFF);
            if (30U > app_heater_temp_get())
            {
                heater = HEATER_ON;
            }
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }

    printf("\n\rheater State %d    heater Temp %d", heater, app_heater_temp_get());

#if 0
    static uint8_t highLow_flag[2] = {0};
    static uint32_t timeout[2] = {0};
    static uint8_t port[2], pin[2];
    static uint8_t index = 0;

    typedef enum
    {
        ON_TIME,
        OFF_TIME,
        WAIT_TIME
    } onOffState_t;
    static onOffState_t onOffState[2] = {ON_TIME, ON_TIME};
    static onOffState_t retState[2] = {ON_TIME, ON_TIME};

    port[0] = DRAIN_VALVE_PORT;
    port[1] = LOCK_MOTOR_PORT;
    pin[0] = DRAIN_VALVE_PIN;
    pin[1] = LOCK_MOTOR_PIN;

    switch (onOffState[index])
    {
        case ON_TIME:
        {
            if (highLow_flag[index] == 0)
            {
                gpio_pinState_set(port[index], pin[index], 1);
                highLow_flag[index] = 1;
            }
            else
            {
                gpio_pinState_set(port[index], pin[index], 0);
                highLow_flag[index] = 0;
                onOffState[index] = WAIT_TIME;
                retState[index] = OFF_TIME;
                timeout[0] = 3600000;
                timeout[1] = 9000;
            }
        }
        break;

        case OFF_TIME:
        {
            if (highLow_flag[index] == 0)
            {
                gpio_pinState_set(port[index], pin[index], 1);
                highLow_flag[index] = 1;
            }
            else
            {
                gpio_pinState_set(port[index], pin[index], 0);
                highLow_flag[index] = 0;
                onOffState[index] = WAIT_TIME;
                retState[index] = ON_TIME;
                timeout[0] = 120000;
                timeout[1] = 9000;
            }
        }
        break;

        case WAIT_TIME:
        {
            timeout[index] -= period;
            if (timeout[index] == 0)
            {
                onOffState[index] = retState[index];
            }
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }

    index++;
    if (index == 2)
    {
        index = 0;
    }
#endif
}
