# TODO: add coverage
# @ cd build/components/doutput_module;	 gcovr.exe -r .. --html-nested doutput_coverage.html

F_OSC = 10000000
CHIP_NAME = tmpm370

COM_PORT = /dev/ttyUSB0
MONITOR_SPEED = 115200

#----------------------------------------------------------------------------
# NOTE: source files (add sources under SRC)

TARGET_INITIALS = ..
TESTING_INITIALS = ../tests

TARGET_ROOT_DIR = $(TARGET_INITIALS)/source
TESTING_ROOT_DIR = $(TESTING_INITIALS)/source

# unity
UNITY_DIR = $(TESTING_INITIALS)/unity

VPATH += $(UNITY_DIR)

TESTING_INC += $(TESTING_INITIALS)
INC += $(UNITY_DIR)

TESTING_SRC += $(UNITY_DIR)/unity.c
TESTING_SRC += $(UNITY_DIR)/unity_fixture.c


# ---------------------------------

TARGET_ROOT_SRC_DIR = $(TARGET_ROOT_DIR)
TESTING_ROOT_SRC_DIR = $(TESTING_ROOT_DIR)
TARGET_ROOT_INC_DIR = $(TARGET_ROOT_DIR)
TESTING_ROOT_INC_DIR = $(TESTING_ROOT_DIR)

VPATH += $(TARGET_ROOT_SRC_DIR)
VPATH += $(TESTING_ROOT_SRC_DIR)

TESTING_INC += $(TESTING_ROOT_INC_DIR)
INC += $(TARGET_ROOT_INC_DIR)

# target sources
SRC +=

#testing source
TESTING_SRC +=
TESTING_SRC +=

# ---------------------------------

# app config
TARGET_APP_CONFIG_INC_DIR = $(TARGET_ROOT_DIR)/app/config
TESTING_APP_CONFIG_INC_DIR = $(TESTING_ROOT_DIR)/app/config

TESTING_INC += $(TESTING_APP_CONFIG_INC_DIR)
INC += $(TARGET_APP_CONFIG_INC_DIR)

# ---------------------------------

# app entry
TARGET_APP_ENTRY_SRC_DIR = $(TARGET_ROOT_DIR)/app/entry/src
TESTING_APP_ENTRY_SRC_DIR = $(TESTING_ROOT_DIR)/app/entry/src
TARGET_APP_ENTRY_INC_DIR = $(TARGET_ROOT_DIR)/app/entry/inc
TESTING_APP_ENTRY_INC_DIR = $(TESTING_ROOT_DIR)/app/entry/inc

VPATH += $(TARGET_APP_ENTRY_SRC_DIR)
VPATH += $(TESTING_APP_ENTRY_SRC_DIR)

TESTING_INC += $(TESTING_APP_ENTRY_INC_DIR)
INC += $(TARGET_APP_ENTRY_INC_DIR)

# target sources
SRC += $(TARGET_APP_ENTRY_SRC_DIR)/app_entry_startup.c
SRC += $(TARGET_APP_ENTRY_SRC_DIR)/app_entry.c

#testing source
TESTING_SRC +=


# ---------------------------------

# app interrupt
TARGET_APP_INTERRUPT_SRC_DIR = $(TARGET_ROOT_DIR)/app/interrupt/src
TESTING_APP_INTERRUPT_SRC_DIR = $(TESTING_ROOT_DIR)/app/interrupt/src
TARGET_APP_INTERRUPT_INC_DIR = $(TARGET_ROOT_DIR)/app/interrupt/inc
TESTING_APP_INTERRUPT_INC_DIR = $(TESTING_ROOT_DIR)/app/interrupt/inc

VPATH += $(TARGET_APP_INTERRUPT_SRC_DIR)
VPATH += $(TESTING_APP_INTERRUPT_SRC_DIR)

TESTING_INC += $(TESTING_APP_INTERRUPT_INC_DIR)
INC += $(TARGET_APP_INTERRUPT_INC_DIR)

# target sources
SRC += $(TARGET_APP_INTERRUPT_SRC_DIR)/app_interrupt.c

#testing source
TESTING_SRC +=


# ---------------------------------

# app exit
TARGET_APP_EXIT_SRC_DIR = $(TARGET_ROOT_DIR)/app/exit/src
TESTING_APP_EXIT_SRC_DIR = $(TESTING_ROOT_DIR)/app/exit/src
TARGET_APP_EXIT_INC_DIR = $(TARGET_ROOT_DIR)/app/exit/inc
TESTING_APP_EXIT_INC_DIR = $(TESTING_ROOT_DIR)/app/exit/inc

VPATH += $(TARGET_APP_EXIT_SRC_DIR)
VPATH += $(TESTING_APP_EXIT_SRC_DIR)

TESTING_INC += $(TESTING_APP_EXIT_INC_DIR)
INC += $(TARGET_APP_EXIT_INC_DIR)

# target sources
SRC += $(TARGET_APP_EXIT_SRC_DIR)/app_exit_shutdown.c

#testing source
TESTING_SRC += $(TESTING_APP_EXIT_SRC_DIR)/stub_app_shutdown.c

# ---------------------------------

# fault_mode
TARGET_APP_FAULT_SRC_DIR = $(TARGET_ROOT_DIR)/app/pm/5_fault_mode/src
TESTING_APP_FAULT_SRC_DIR = $(TESTING_ROOT_DIR)/app/pm/5_fault_mode/src
TARGET_APP_FAULT_INC_DIR = $(TARGET_ROOT_DIR)/app/pm/5_fault_mode/inc
TESTING_APP_FAULT_INC_DIR = $(TESTING_ROOT_DIR)/app/pm/5_fault_mode/inc

VPATH += $(TARGET_APP_FAULT_SRC_DIR)
VPATH += $(TESTING_APP_FAULT_SRC_DIR)

TESTING_INC += $(TESTING_APP_FAULT_INC_DIR)
INC += $(TARGET_APP_FAULT_INC_DIR)

# target sources
SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault.c
# SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_blower.c
# SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_condenser.c
# SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_cooling_fan.c
SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_drain.c
SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_fill.c
SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_heater.c
SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_lid.c
SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_memory.c
SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_motor.c
SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_user_interface.c
SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_display.c

#testing source
TESTING_SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault.c
# TESTING_SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_blower.c
# TESTING_SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_condenser.c
# TESTING_SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_cooling_fan.c
TESTING_SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_drain.c
TESTING_SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_fill.c
TESTING_SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_heater.c
TESTING_SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_lid.c
TESTING_SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_memory.c
TESTING_SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_motor.c
TESTING_SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_user_interface.c
TESTING_SRC += $(TARGET_APP_FAULT_SRC_DIR)/app_fault_display.c

# ---------------------------------

# pm
TARGET_APP_PROCESSOR_SRC_DIR = $(TARGET_ROOT_DIR)/app/pm/processor/src
TESTING_APP_PROCESSOR_SRC_DIR = $(TESTING_ROOT_DIR)/app/pm/processor/src
TARGET_APP_PROCESSOR_INC_DIR = $(TARGET_ROOT_DIR)/app/pm/processor/inc
TESTING_APP_PROCESSOR_INC_DIR = $(TESTING_ROOT_DIR)/app/pm/processor/inc

VPATH += $(TARGET_APP_PROCESSOR_SRC_DIR)
VPATH += $(TESTING_APP_PROCESSOR_SRC_DIR)

TESTING_INC += $(TESTING_APP_PROCESSOR_INC_DIR)
INC += $(TARGET_APP_PROCESSOR_INC_DIR)

# target sources
# SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_blower.c
SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_buzzer.c
# SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_condenser.c
# SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_cooling_fan.c
SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_drain.c
SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_fill.c
SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_heater.c
SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_lid.c
SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_load.c
SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_memory.c
SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_motor.c
SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_processor.c
SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_display.c
SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_unbalance.c
SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_user_interface.c
SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_controller.c
SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_component_mocker.c

#testing source
# TESTING_SRC += $(TESTING_APP_PROCESSOR_SRC_DIR)/test_app_main.c
TESTING_SRC += $(TESTING_APP_PROCESSOR_SRC_DIR)/test_app_main_update.c

# TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_blower.c
TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_buzzer.c
# TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_condenser.c
# TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_cooling_fan.c
TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_drain.c
TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_fill.c
TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_heater.c
TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_lid.c
TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_load.c
TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_memory.c
TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_motor.c
TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_processor.c
TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_display.c
TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_unbalance.c
TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_user_interface.c
TESTING_SRC += $(TARGET_APP_PROCESSOR_SRC_DIR)/app_controller.c

# ---------------------------------

# startup_mode
TARGET_APP_STARTUP_SRC_DIR = $(TARGET_ROOT_DIR)/app/pm/0_startup_mode/src
TESTING_APP_STARTUP_SRC_DIR = $(TESTING_ROOT_DIR)/app/pm/0_startup_mode/src
TARGET_APP_STARTUP_INC_DIR = $(TARGET_ROOT_DIR)/app/pm/0_startup_mode/inc
TESTING_APP_STARTUP_INC_DIR = $(TESTING_ROOT_DIR)/app/pm/0_startup_mode/inc

VPATH += $(TARGET_APP_STARTUP_SRC_DIR)
VPATH += $(TESTING_APP_STARTUP_SRC_DIR)

TESTING_INC += $(TESTING_APP_STARTUP_INC_DIR)
INC += $(TARGET_APP_STARTUP_INC_DIR)

SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup.c
# SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_blower.c
# SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_condenser.c
# SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_cooling_fan.c
SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_drain.c
SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_fill.c
SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_heater.c
SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_lid.c
SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_memory.c
SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_motor.c
SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_user_interface.c
SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_display.c

TESTING_SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup.c
# TESTING_SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_blower.c
# TESTING_SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_condenser.c
# TESTING_SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_cooling_fan.c
TESTING_SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_drain.c
TESTING_SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_fill.c
TESTING_SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_heater.c
TESTING_SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_lid.c
TESTING_SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_memory.c
TESTING_SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_motor.c
TESTING_SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_user_interface.c
TESTING_SRC += $(TARGET_APP_STARTUP_SRC_DIR)/app_startup_display.c

# ---------------------------------

# setup_mode
TARGET_APP_SETUP_SRC_DIR = $(TARGET_ROOT_DIR)/app/pm/1_setup_mode/src
TESTING_APP_SETUP_SRC_DIR = $(TESTING_ROOT_DIR)/app/pm/1_setup_mode/src
TARGET_APP_SETUP_INC_DIR = $(TARGET_ROOT_DIR)/app/pm/1_setup_mode/inc
TESTING_APP_SETUP_INC_DIR = $(TESTING_ROOT_DIR)/app/pm/1_setup_mode/inc

VPATH += $(TARGET_APP_SETUP_SRC_DIR)
VPATH += $(TESTING_APP_SETUP_SRC_DIR)

TESTING_INC += $(TESTING_APP_SETUP_INC_DIR)
INC += $(TARGET_APP_SETUP_INC_DIR)

SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup.c
# SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_blower.c
# SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_condenser.c
# SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_cooling_fan.c
SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_drain.c
SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_fill.c
SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_heater.c
SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_lid.c
SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_memory.c
SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_motor.c
SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_user_interface.c
SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_display.c

TESTING_SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup.c
# TESTING_SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_blower.c
# TESTING_SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_condenser.c
# TESTING_SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_cooling_fan.c
TESTING_SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_drain.c
TESTING_SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_fill.c
TESTING_SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_heater.c
TESTING_SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_lid.c
TESTING_SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_memory.c
TESTING_SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_motor.c
TESTING_SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_user_interface.c
TESTING_SRC += $(TARGET_APP_SETUP_SRC_DIR)/app_setup_display.c

# ---------------------------------

# standby_mode
TARGET_APP_STANDBY_SRC_DIR = $(TARGET_ROOT_DIR)/app/pm/2_standby_mode/src
TESTING_APP_STANDBY_SRC_DIR = $(TESTING_ROOT_DIR)/app/pm/2_standby_mode/src
TARGET_APP_STANDBY_INC_DIR = $(TARGET_ROOT_DIR)/app/pm/2_standby_mode/inc
TESTING_APP_STANDBY_INC_DIR = $(TESTING_ROOT_DIR)/app/pm/2_standby_mode/inc

VPATH += $(TARGET_APP_STANDBY_SRC_DIR)
VPATH += $(TESTING_APP_STANDBY_SRC_DIR)

TESTING_INC += $(TESTING_APP_STANDBY_INC_DIR)
INC += $(TARGET_APP_STANDBY_INC_DIR)

SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby.c
# SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_blower.c
# SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_condenser.c
# SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_cooling_fan.c
SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_drain.c
SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_fill.c
SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_heater.c
SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_lid.c
SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_memory.c
SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_motor.c
SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_user_interface.c
SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_display.c

TESTING_SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby.c
# TESTING_SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_blower.c
# TESTING_SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_condenser.c
# TESTING_SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_cooling_fan.c
TESTING_SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_drain.c
TESTING_SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_fill.c
TESTING_SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_heater.c
TESTING_SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_lid.c
TESTING_SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_memory.c
TESTING_SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_motor.c
TESTING_SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_user_interface.c
TESTING_SRC += $(TARGET_APP_STANDBY_SRC_DIR)/app_standby_display.c

# ---------------------------------

# normal_mode - nm
TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR = $(TARGET_ROOT_DIR)/app/pm/3_normal_mode/nm/src
TESTING_APP_NORMAL_MODE_MAIN_SRC_DIR = $(TESTING_ROOT_DIR)/app/pm/3_normal_mode/nm/src
TARGET_APP_NORMAL_MODE_MAIN_INC_DIR = $(TARGET_ROOT_DIR)/app/pm/3_normal_mode/nm/inc
TESTING_APP_NORMAL_MODE_MAIN_INC_DIR = $(TESTING_ROOT_DIR)/app/pm/3_normal_mode/nm/inc

VPATH += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)
VPATH += $(TESTING_APP_NORMAL_MODE_MAIN_SRC_DIR)

TESTING_INC += $(TESTING_APP_NORMAL_MODE_MAIN_INC_DIR)
INC += $(TARGET_APP_NORMAL_MODE_MAIN_INC_DIR)

# target sources
# SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_blower.c
# SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_condenser.c
# SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_cooling_fan.c
SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_drain.c
SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_fill.c
SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_heater.c
SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_lid.c
SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_log.c
SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_memory.c
SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_motor.c
SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_user_interface.c
SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_data_display.c
SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_endurance.c
SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm.c
SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_display.c


#testing source
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_blower.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_condenser.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_cooling_fan.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_drain.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_fill.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_heater.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_lid.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_log.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_memory.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_motor.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_user_interface.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_data_display.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_endurance.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_MAIN_SRC_DIR)/app_nm_display.c

# ---------------------------------

# normal_mode - init
TARGET_APP_NORMAL_MODE_INIT_SRC_DIR = $(TARGET_ROOT_DIR)/app/pm/3_normal_mode/0_nm_init/src
TESTING_APP_NORMAL_MODE_INIT_SRC_DIR = $(TESTING_ROOT_DIR)/app/pm/3_normal_mode/0_nm_init/src
TARGET_APP_NORMAL_MODE_INIT_INC_DIR = $(TARGET_ROOT_DIR)/app/pm/3_normal_mode/0_nm_init/inc
TESTING_APP_NORMAL_MODE_INIT_INC_DIR = $(TESTING_ROOT_DIR)/app/pm/3_normal_mode/0_nm_init/inc

VPATH += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)
VPATH += $(TESTING_APP_NORMAL_MODE_INIT_SRC_DIR)

TESTING_INC += $(TESTING_APP_NORMAL_MODE_INIT_INC_DIR)
INC += $(TARGET_APP_NORMAL_MODE_INIT_INC_DIR)

# target sources
# SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_blower.c
# SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_condenser.c
# SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_cooling_fan.c
SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_drain.c
SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_fill.c
SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_heater.c
SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_lid.c
SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_motor.c
SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_user_interface.c
SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_display.c
SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init.c

# testing sources
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_blower.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_condenser.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_cooling_fan.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_drain.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_fill.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_heater.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_lid.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_motor.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_user_interface.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init_display.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_INIT_SRC_DIR)/app_nm_init.c

# ---------------------------------

# normal_mode - wash
TARGET_APP_WASH_SRC_DIR = $(TARGET_ROOT_DIR)/app/pm/3_normal_mode/1_nm_wash/src
TESTING_APP_WASH_SRC_DIR = $(TESTING_ROOT_DIR)/app/pm/3_normal_mode/1_nm_wash/src
TARGET_APP_WASH_INC_DIR = $(TARGET_ROOT_DIR)/app/pm/3_normal_mode/1_nm_wash/inc
TESTING_APP_WASH_INC_DIR = $(TESTING_ROOT_DIR)/app/pm/3_normal_mode/1_nm_wash/inc

VPATH += $(TARGET_APP_WASH_SRC_DIR)
VPATH += $(TESTING_APP_WASH_SRC_DIR)

TESTING_INC += $(TESTING_APP_WASH_INC_DIR)
INC += $(TARGET_APP_WASH_INC_DIR)

# target sources
# SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_blower.c
# SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_condenser.c
# SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_cooling_fan.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_drain.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_fill.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_heater.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_heater_table.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_lid.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_motor_table.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_motor.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_rinse_fill_table.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_rinse_table.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_rinse.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_setup.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_soak.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_spin_table.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_spin.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_user_interface.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_display.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_wash_table.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_wash.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_steam.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_steam_table.c
SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash.c

#testing source
# ADD HERE
TESTING_SRC += $(TESTING_APP_WASH_SRC_DIR)/test_app_wash_nosoak.c
TESTING_SRC += $(TESTING_APP_WASH_SRC_DIR)/test_app_wash_soak.c
TESTING_SRC += $(TESTING_APP_WASH_SRC_DIR)/test_app_rinse.c
TESTING_SRC += $(TESTING_APP_WASH_SRC_DIR)/test_app_spin.c
TESTING_SRC += $(TESTING_APP_WASH_SRC_DIR)/test_app_steam.c

# TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_blower.c
# TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_condenser.c
# TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_cooling_fan.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_drain.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_fill.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_heater.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_heater_table.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_lid.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_motor_table.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_motor.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_rinse_fill_table.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_rinse_table.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_rinse.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_setup.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_soak.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_spin_table.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_spin.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_user_interface.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_display.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_wash_table.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_wash.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_steam.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash_steam_table.c
TESTING_SRC += $(TARGET_APP_WASH_SRC_DIR)/app_nm_wash.c

# ---------------------------------

# normal_mode - tubclean
TARGET_APP_TUBCLEAN_SRC_DIR = $(TARGET_ROOT_DIR)/app/pm/3_normal_mode/3_nm_tubclean/src
TESTING_APP_TUBCLEAN_SRC_DIR = $(TESTING_ROOT_DIR)/app/pm/3_normal_mode/3_nm_tubclean/src
TARGET_APP_TUBCLEAN_INC_DIR = $(TARGET_ROOT_DIR)/app/pm/3_normal_mode/3_nm_tubclean/inc
TESTING_APP_TUBCLEAN_INC_DIR = $(TESTING_ROOT_DIR)/app/pm/3_normal_mode/3_nm_tubclean/inc

VPATH += $(TARGET_APP_TUBCLEAN_SRC_DIR)
VPATH += $(TESTING_APP_TUBCLEAN_SRC_DIR)

TESTING_INC += $(TESTING_APP_TUBCLEAN_INC_DIR)
INC += $(TARGET_APP_TUBCLEAN_INC_DIR)

# target sources
SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean.c
SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_table.c
SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_user_interface.c
SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_motor.c
SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_motor_table.c
# SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_blower.c
# SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_condenser.c
# SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_cooling_fan.c
SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_drain.c
SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_fill.c
SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_lid.c
SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_heater.c
SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_heater_table.c
SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_display.c

#testing source
# ADD HERE
# TESTING_SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/test_app_steam.c

TESTING_SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean.c
TESTING_SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_table.c
TESTING_SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_user_interface.c
TESTING_SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_motor.c
TESTING_SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_motor_table.c
# TESTING_SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_blower.c
# TESTING_SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_condenser.c
# TESTING_SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_cooling_fan.c
TESTING_SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_drain.c
TESTING_SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_fill.c
TESTING_SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_lid.c
TESTING_SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_heater.c
TESTING_SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_heater_table.c
TESTING_SRC += $(TARGET_APP_TUBCLEAN_SRC_DIR)/app_nm_tubclean_display.c

# ---------------------------------

# normal_mode - dry
TARGET_APP_NORMAL_MODE_DRY_SRC_DIR = $(TARGET_ROOT_DIR)/app/pm/3_normal_mode/2_nm_dry/src
TESTING_APP_NORMAL_MODE_DRY_SRC_DIR = $(TESTING_ROOT_DIR)/app/pm/3_normal_mode/2_nm_dry/src
TARGET_APP_NORMAL_MODE_DRY_INC_DIR = $(TARGET_ROOT_DIR)/app/pm/3_normal_mode/2_nm_dry/inc
TESTING_APP_NORMAL_MODE_DRY_INC_DIR = $(TESTING_ROOT_DIR)/app/pm/3_normal_mode/2_nm_dry/inc

VPATH += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)
VPATH += $(TESTING_APP_NORMAL_MODE_DRY_SRC_DIR)

TESTING_INC += $(TESTING_APP_NORMAL_MODE_DRY_INC_DIR)
INC += $(TARGET_APP_NORMAL_MODE_DRY_INC_DIR)

# target sources
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_blower.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_condenser.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_cooling_fan.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_drain.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_fill.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_heater_table.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_heater.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_lid.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_motor_table.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_motor.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_operation_table.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_operation.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_preSetup_table.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_preSetup.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_setup.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_user_interface.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_display.c
# SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry.c

#testing source
# TESTING_SRC += $(TESTING_APP_NORMAL_MODE_DRY_SRC_DIR)/test_app_dry.c
# TESTING_SRC += $(TESTING_APP_NORMAL_MODE_DRY_SRC_DIR)/test_app_dry_heater.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_blower.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_condenser.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_cooling_fan.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_drain.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_fill.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_heater_table.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_heater.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_lid.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_motor_table.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_motor.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_operation_table.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_operation.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_preSetup_table.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_preSetup.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_setup.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_user_interface.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry_display.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_DRY_SRC_DIR)/app_nm_dry.c

# ---------------------------------

# normal mode - pause
TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR = $(TARGET_ROOT_DIR)/app/pm/3_normal_mode/4_nm_pause/src
TESTING_APP_NORMAL_MODE_PAUSE_SRC_DIR = $(TESTING_ROOT_DIR)/app/pm/3_normal_mode/4_nm_pause/src
TARGET_APP_NORMAL_MODE_PAUSE_INC_DIR = $(TARGET_ROOT_DIR)/app/pm/3_normal_mode/4_nm_pause/inc
TESTING_APP_NORMAL_MODE_PAUSE_INC_DIR = $(TESTING_ROOT_DIR)/app/pm/3_normal_mode/4_nm_pause/inc

VPATH += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)
VPATH += $(TESTING_APP_NORMAL_MODE_PAUSE_SRC_DIR)

TESTING_INC += $(TESTING_APP_NORMAL_MODE_PAUSE_INC_DIR)
INC += $(TARGET_APP_NORMAL_MODE_PAUSE_INC_DIR)

# target sources
# SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_blower.c
# SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_condenser.c
# SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_cooling_fan.c
SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_drain.c
SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_fill.c
SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_heater.c
SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_lid.c
SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_motor.c
SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_user_interface.c
SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_display.c
SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause.c


#testing source
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_blower.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_condenser.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_cooling_fan.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_drain.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_fill.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_heater.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_lid.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_motor.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_user_interface.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause_display.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_PAUSE_SRC_DIR)/app_nm_pause.c

# ---------------------------------

# normal mode - finish
TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR = $(TARGET_ROOT_DIR)/app/pm/3_normal_mode/5_nm_finish/src
TESTING_APP_NORMAL_MODE_FINISH_SRC_DIR = $(TESTING_ROOT_DIR)/app/pm/3_normal_mode/5_nm_finish/src
TARGET_APP_NORMAL_MODE_FINISH_INC_DIR = $(TARGET_ROOT_DIR)/app/pm/3_normal_mode/5_nm_finish/inc
TESTING_APP_NORMAL_MODE_FINISH_INC_DIR = $(TESTING_ROOT_DIR)/app/pm/3_normal_mode/5_nm_finish/inc

VPATH += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)
VPATH += $(TESTING_APP_NORMAL_MODE_FINISH_SRC_DIR)

TESTING_INC += $(TESTING_APP_NORMAL_MODE_FINISH_INC_DIR)
INC += $(TARGET_APP_NORMAL_MODE_FINISH_INC_DIR)

# target sources
# SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_blower.c
# SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_condenser.c
# SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_cooling_fan.c
SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_drain.c
SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_fill.c
SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_heater.c
SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_lid.c
SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_motor.c
SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_user_interface.c
SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_display.c
SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish.c


#testing source
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_blower.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_condenser.c
# TESTING_SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_cooling_fan.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_drain.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_fill.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_heater.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_lid.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_motor.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_user_interface.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish_display.c
TESTING_SRC += $(TARGET_APP_NORMAL_MODE_FINISH_SRC_DIR)/app_nm_finish.c

# ---------------------------------

# finish_mode
TARGET_APP_FINISH_SRC_DIR = $(TARGET_ROOT_DIR)/app/pm/6_finish_mode/src
TESTING_APP_FINISH_SRC_DIR = $(TESTING_ROOT_DIR)/app/pm/6_finish_mode/src
TARGET_APP_FINISH_INC_DIR = $(TARGET_ROOT_DIR)/app/pm/6_finish_mode/inc
TESTING_APP_FINISH_INC_DIR = $(TESTING_ROOT_DIR)/app/pm/6_finish_mode/inc

VPATH += $(TARGET_APP_FINISH_SRC_DIR)
VPATH += $(TESTING_APP_FINISH_SRC_DIR)

TESTING_INC += $(TESTING_APP_FINISH_INC_DIR)
INC += $(TARGET_APP_FINISH_INC_DIR)

SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish.c
# SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_blower.c
# SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_condenser.c
# SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_cooling_fan.c
SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_drain.c
SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_fill.c
SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_heater.c
SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_lid.c
SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_memory.c
SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_motor.c
SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_user_interface.c
SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_display.c

TESTING_SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish.c
# TESTING_SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_blower.c
# TESTING_SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_condenser.c
# TESTING_SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_cooling_fan.c
TESTING_SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_drain.c
TESTING_SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_fill.c
TESTING_SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_heater.c
TESTING_SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_lid.c
TESTING_SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_memory.c
TESTING_SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_motor.c
TESTING_SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_user_interface.c
TESTING_SRC += $(TARGET_APP_FINISH_SRC_DIR)/app_finish_display.c

# ---------------------------------

# test modes
TARGET_APP_TEST_MODE_SRC_DIR = $(TARGET_ROOT_DIR)/app/pm/4_test_mode/src
TESTING_APP_TEST_MODE_SRC_DIR = $(TESTING_ROOT_DIR)/app/pm/4_test_mode/src
TARGET_APP_TEST_MODE_INC_DIR = $(TARGET_ROOT_DIR)/app/pm/4_test_mode/inc
TESTING_APP_TEST_MODE_INC_DIR = $(TESTING_ROOT_DIR)/app/pm/4_test_mode/inc

VPATH += $(TARGET_APP_TEST_MODE_SRC_DIR)
VPATH += $(TESTING_APP_TEST_MODE_SRC_DIR)

TESTING_INC += $(TESTING_APP_TEST_MODE_INC_DIR)
INC += $(TARGET_APP_TEST_MODE_INC_DIR)

# target sources
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_test_mode.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_user_interface.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_water_level_calibration.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_weight_calibration.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_electrical_parts.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_lv_calibration.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_pcb.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_unbalance_spin.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_water_level_freq.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_middle_inspection.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_final_inspection.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_error_count_display.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_cycle_count_display.c
# SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_blower.c
# SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_condenser.c
# SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_cooling_fan.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_drain.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_fill.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_heater.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_lid.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_memory.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_motor.c
SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_display.c

#testing source
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_test_mode.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_user_interface.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_water_level_calibration.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_lv_calibration.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_weight_calibration.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_electrical_parts.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_pcb.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_unbalance_spin.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_water_level_freq.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_middle_inspection.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_final_inspection.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_error_count_display.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_cycle_count_display.c
# TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_blower.c
# TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_condenser.c
# TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_cooling_fan.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_drain.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_fill.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_heater.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_lid.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_memory.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_motor.c
TESTING_SRC += $(TARGET_APP_TEST_MODE_SRC_DIR)/app_tm_display.c

# ---------------------------------

# components
TARGET_COMPONENTS_DIR = $(TARGET_ROOT_DIR)/components
TESTING_COMPONENTS_DIR = $(TESTING_ROOT_DIR)/components

COMPONENTS := ainput_module
COMPONENTS += acin
# COMPONENTS += blower_motor
COMPONENTS += doutput_module
COMPONENTS += dinput_module
COMPONENTS += finput_module
COMPONENTS += heartbeat
COMPONENTS += keypad
COMPONENTS += ddm_motor
COMPONENTS += twi_eeprom
COMPONENTS += gpio
COMPONENTS += uart
COMPONENTS += stwi
COMPONENTS += hw_timer
COMPONENTS += timer_sandwich
COMPONENTS += wdt
COMPONENTS += sspi
COMPONENTS += kx022
COMPONENTS += buzzer
COMPONENTS += hm15n
COMPONENTS += wl90at
COMPONENTS += s15d
COMPONENTS += crc16
COMPONENTS += ptc_heater
COMPONENTS += ntc_temp_sensor
COMPONENTS += zcd
COMPONENTS += com_display
# COMPONENTS += itw_lock
COMPONENTS += dls35s

TESTING_COMPONENTS += uart
TESTING_COMPONENTS += kx022

VPATH += $(foreach I, $(TESTING_COMPONENTS), $(TESTING_COMPONENTS_DIR)/$(I))
VPATH += $(foreach I, $(COMPONENTS), $(TARGET_COMPONENTS_DIR)/$(I))


TESTING_INC += $(TESTING_COMPONENTS_DIR)
TESTING_INC += $(foreach I, $(TESTING_COMPONENTS), $(TESTING_COMPONENTS_DIR)/$(I))
INC += $(TARGET_COMPONENTS_DIR)
INC += $(foreach I, $(COMPONENTS), $(TARGET_COMPONENTS_DIR)/$(I))

# target sources
SRC += $(foreach D,$(COMPONENTS),$(TARGET_COMPONENTS_DIR)/$(D)/$(D).c)

# testing sources
TESTING_SRC +=  $(foreach K,$(TESTING_COMPONENTS),$(TESTING_COMPONENTS_DIR)/$(K)/stub_$(K).c)

# ---------------------------------

# core
TARGET_CORE_SRC_DIR = $(TARGET_ROOT_DIR)/core/$(CHIP_NAME)/src
TESTING_CORE_SRC_DIR = $(TESTING_ROOT_DIR)/core/$(CHIP_NAME)/src
TARGET_CORE_INC_DIR = $(TARGET_ROOT_DIR)/core/$(CHIP_NAME)/inc
TESTING_CORE_INC_DIR = $(TESTING_ROOT_DIR)/core/$(CHIP_NAME)/inc

VPATH += $(TARGET_CORE_SRC_DIR)
VPATH += $(TESTING_CORE_SRC_DIR)

TESTING_INC += $(TESTING_CORE_INC_DIR)
INC += $(TARGET_CORE_INC_DIR)

# target sources
SRC += $(TARGET_CORE_SRC_DIR)/tmpm370_adc.c
SRC += $(TARGET_CORE_SRC_DIR)/tmpm370_cg.c
SRC += $(TARGET_CORE_SRC_DIR)/tmpm370_gpio.c
SRC += $(TARGET_CORE_SRC_DIR)/tmpm370_tmrb.c
SRC += $(TARGET_CORE_SRC_DIR)/tmpm370_uart.c
SRC += $(TARGET_CORE_SRC_DIR)/tmpm370_wdt.c

#testing source
TESTING_SRC +=

# ---------------------------------

# handlers input
TARGET_HANDLERS_INPUT_SRC_DIR = $(TARGET_ROOT_DIR)/handlers/input/src
TESTING_HANDLERS_INPUT_SRC_DIR = $(TESTING_ROOT_DIR)/handlers/input/src
TARGET_HANDLERS_INPUT_INC_DIR = $(TARGET_ROOT_DIR)/handlers/input/inc
TESTING_HANDLERS_INPUT_INC_DIR = $(TESTING_ROOT_DIR)/handlers/input/inc

VPATH += $(TARGET_HANDLERS_INPUT_SRC_DIR)
VPATH += $(TESTING_HANDLERS_INPUT_SRC_DIR)

TESTING_INC += $(TESTING_HANDLERS_INPUT_INC_DIR)
INC += $(TARGET_HANDLERS_INPUT_INC_DIR)

# target sources
SRC += $(TARGET_HANDLERS_INPUT_SRC_DIR)/handler_off_sw.c
SRC += $(TARGET_HANDLERS_INPUT_SRC_DIR)/handler_acin.c
SRC += $(TARGET_HANDLERS_INPUT_SRC_DIR)/handler_crc.c
SRC += $(TARGET_HANDLERS_INPUT_SRC_DIR)/handler_keypad.c
SRC += $(TARGET_HANDLERS_INPUT_SRC_DIR)/handler_lid_sensor.c
# SRC += $(TARGET_HANDLERS_INPUT_SRC_DIR)/handler_lint_filter_sensor.c
SRC += $(TARGET_HANDLERS_INPUT_SRC_DIR)/handler_load_sense.c
SRC += $(TARGET_HANDLERS_INPUT_SRC_DIR)/handler_memory.c
SRC += $(TARGET_HANDLERS_INPUT_SRC_DIR)/handler_mems.c
SRC += $(TARGET_HANDLERS_INPUT_SRC_DIR)/handler_power_detection.c
SRC += $(TARGET_HANDLERS_INPUT_SRC_DIR)/handler_temp_sensor.c
SRC += $(TARGET_HANDLERS_INPUT_SRC_DIR)/handler_water_level.c
SRC += $(TARGET_HANDLERS_INPUT_SRC_DIR)/handler_detergent_sensor.c
SRC += $(TARGET_HANDLERS_INPUT_SRC_DIR)/handler_softener_sensor.c

#testing source
TESTING_SRC += $(TESTING_HANDLERS_INPUT_SRC_DIR)/stub_handler_off_sw.c
TESTING_SRC += $(TESTING_HANDLERS_INPUT_SRC_DIR)/stub_handler_water_level.c
TESTING_SRC += $(TESTING_HANDLERS_INPUT_SRC_DIR)/stub_handler_mems.c
TESTING_SRC += $(TESTING_HANDLERS_INPUT_SRC_DIR)/stub_handler_keypad.c
TESTING_SRC += $(TESTING_HANDLERS_INPUT_SRC_DIR)/stub_handler_lid_sensor.c
TESTING_SRC += $(TESTING_HANDLERS_INPUT_SRC_DIR)/stub_handler_memory.c
TESTING_SRC += $(TESTING_HANDLERS_INPUT_SRC_DIR)/stub_handler_temp_sensor.c
TESTING_SRC += $(TESTING_HANDLERS_INPUT_SRC_DIR)/stub_handler_load_sense.c
TESTING_SRC += $(TESTING_HANDLERS_INPUT_SRC_DIR)/stub_handler_power_detection.c
TESTING_SRC += $(TESTING_HANDLERS_INPUT_SRC_DIR)/stub_handler_acin.c
TESTING_SRC += $(TESTING_HANDLERS_INPUT_SRC_DIR)/stub_handler_crc.c
TESTING_SRC += $(TESTING_HANDLERS_INPUT_SRC_DIR)/stub_handler_detergent_sensor.c
TESTING_SRC += $(TESTING_HANDLERS_INPUT_SRC_DIR)/stub_handler_softener_sensor.c
# TESTING_SRC += $(TESTING_HANDLERS_INPUT_SRC_DIR)/stub_handler_lint_filter_sensor.c

# ---------------------------------

# handlers output
TARGET_HANDLERS_OUTPUT_SRC_DIR = $(TARGET_ROOT_DIR)/handlers/output/src
TESTING_HANDLERS_OUTPUT_SRC_DIR = $(TESTING_ROOT_DIR)/handlers/output/src
TARGET_HANDLERS_OUTPUT_INC_DIR = $(TARGET_ROOT_DIR)/handlers/output/inc
TESTING_HANDLERS_OUTPUT_INC_DIR = $(TESTING_ROOT_DIR)/handlers/output/inc

VPATH += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)
VPATH += $(TESTING_HANDLERS_OUTPUT_SRC_DIR)

TESTING_INC += $(TESTING_HANDLERS_OUTPUT_INC_DIR)
INC += $(TARGET_HANDLERS_OUTPUT_INC_DIR)

# target sources
# SRC += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)/handler_blower.c
SRC += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)/handler_buzzer.c
SRC += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)/handler_clutch_hm15n.c
SRC += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)/handler_com.c
# SRC += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)/handler_cooling_fan.c
SRC += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)/handler_display.c
SRC += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)/handler_drain_pump.c
SRC += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)/handler_softener_pump.c
SRC += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)/handler_detergent_pump.c
SRC += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)/handler_drain_valve_hm15n.c
SRC += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)/handler_heater.c
SRC += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)/handler_lock_motor.c
SRC += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)/handler_motor.c
SRC += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)/handler_relay.c
SRC += $(TARGET_HANDLERS_OUTPUT_SRC_DIR)/handler_water_valve.c

#testing source
TESTING_SRC += $(TESTING_HANDLERS_OUTPUT_SRC_DIR)/stub_handler_clutch.c
TESTING_SRC += $(TESTING_HANDLERS_OUTPUT_SRC_DIR)/stub_handler_buzzer.c
TESTING_SRC += $(TESTING_HANDLERS_OUTPUT_SRC_DIR)/stub_handler_motor.c
TESTING_SRC += $(TESTING_HANDLERS_OUTPUT_SRC_DIR)/stub_handler_display.c
TESTING_SRC += $(TESTING_HANDLERS_OUTPUT_SRC_DIR)/stub_handler_lock_motor.c
TESTING_SRC += $(TESTING_HANDLERS_OUTPUT_SRC_DIR)/stub_handler_drain_pump.c
TESTING_SRC += $(TESTING_HANDLERS_OUTPUT_SRC_DIR)/stub_handler_softener_pump.c
TESTING_SRC += $(TESTING_HANDLERS_OUTPUT_SRC_DIR)/stub_handler_detergent_pump.c
TESTING_SRC += $(TESTING_HANDLERS_OUTPUT_SRC_DIR)/stub_handler_drain_valve.c
TESTING_SRC += $(TESTING_HANDLERS_OUTPUT_SRC_DIR)/stub_handler_water_valve.c
# TESTING_SRC += $(TESTING_HANDLERS_OUTPUT_SRC_DIR)/stub_handler_blower.c
# TESTING_SRC += $(TESTING_HANDLERS_OUTPUT_SRC_DIR)/stub_handler_cooling_fan.c
TESTING_SRC += $(TESTING_HANDLERS_OUTPUT_SRC_DIR)/stub_handler_heater.c
TESTING_SRC += $(TESTING_HANDLERS_OUTPUT_SRC_DIR)/stub_handler_relay.c

# ---------------------------------

# mcal
TARGET_MCAL_SRC_DIR = $(TARGET_ROOT_DIR)/mcal/$(CHIP_NAME)/src
TESTING_MCAL_SRC_DIR = $(TESTING_ROOT_DIR)/mcal/$(CHIP_NAME)/src
TARGET_MCAL_INC_DIR = $(TARGET_ROOT_DIR)/mcal/$(CHIP_NAME)/inc
TESTING_MCAL_INC_DIR = $(TESTING_ROOT_DIR)/mcal/$(CHIP_NAME)/inc

VPATH += $(TARGET_MCAL_SRC_DIR)
VPATH += $(TESTING_MCAL_SRC_DIR)

TESTING_INC += $(TESTING_ROOT_DIR)/mcal
INC += $(TARGET_ROOT_DIR)/mcal
TESTING_INC += $(TESTING_MCAL_INC_DIR)
INC += $(TARGET_MCAL_INC_DIR)

# target source
SRC += $(TARGET_MCAL_SRC_DIR)/mcal_tmpm370_adc.c
SRC += $(TARGET_MCAL_SRC_DIR)/mcal_tmpm370_clock.c
SRC += $(TARGET_MCAL_SRC_DIR)/mcal_tmpm370_gpio.c
SRC += $(TARGET_MCAL_SRC_DIR)/mcal_tmpm370_pwm.c
SRC += $(TARGET_MCAL_SRC_DIR)/mcal_tmpm370_systick.c
SRC += $(TARGET_MCAL_SRC_DIR)/mcal_tmpm370_timer.c
SRC += $(TARGET_MCAL_SRC_DIR)/mcal_tmpm370_uart.c
SRC += $(TARGET_MCAL_SRC_DIR)/mcal_tmpm370_wdt.c
SRC += $(TARGET_MCAL_SRC_DIR)/mcal_tmpm370_exti.c

#testing source
TESTING_SRC +=

# ---------------------------------

# os
TARGET_OS_SRC_DIR = $(TARGET_ROOT_DIR)/os/src
TARGET_OS_PORT_SRC_DIR = $(TARGET_ROOT_DIR)/os/port/tmpm370/src
TESTING_OS_SRC_DIR = $(TESTING_ROOT_DIR)/os/src
TESTING_OS_PORT_SRC_DIR = $(TESTING_ROOT_DIR)/os/port/tmpm370/src
TARGET_OS_INC_DIR = $(TARGET_ROOT_DIR)/os/inc
TARGET_OS_INC_DIR += $(TARGET_ROOT_DIR)/os/port/inc
TARGET_OS_INC_DIR += $(TARGET_ROOT_DIR)/os/port/tmpm370/inc
TESTING_OS_INC_DIR = $(TESTING_ROOT_DIR)/os/inc
TESTING_OS_INC_DIR += $(TESTING_ROOT_DIR)/os/port/inc
TESTING_OS_INC_DIR += $(TESTING_ROOT_DIR)/os/port/tmpm370/inc
TARGET_UTILS_SRC_DIR = $(TARGET_ROOT_DIR)/os/utils/src
TESTING_UTILS_SRC_DIR = $(TESTING_ROOT_DIR)/os/utils/src
TARGET_UTILS_INC_DIR = $(TARGET_ROOT_DIR)/os/utils/inc
TESTING_UTILS_INC_DIR = $(TESTING_ROOT_DIR)/os/utils/inc

VPATH += $(TARGET_OS_SRC_DIR)
VPATH += $(TARGET_OS_PORT_SRC_DIR)
VPATH += $(TESTING_OS_SRC_DIR)
VPATH += $(TESTING_OS_PORT_SRC_DIR)
VPATH += $(TARGET_UTILS_SRC_DIR)
VPATH += $(TESTING_UTILS_SRC_DIR)

TESTING_INC += $(TESTING_OS_INC_DIR)
TESTING_INC += $(TESTING_UTILS_INC_DIR)
INC += $(TARGET_OS_INC_DIR)
INC += $(TARGET_UTILS_INC_DIR)

# target source
SRC += $(TARGET_OS_SRC_DIR)/main.c
SRC += $(TARGET_OS_SRC_DIR)/os_log.c
SRC += $(TARGET_OS_SRC_DIR)/os_runtime.c
SRC += $(TARGET_OS_SRC_DIR)/os_scheduler.c
SRC += $(TARGET_OS_SRC_DIR)/os_stack_health.c
SRC += $(TARGET_OS_SRC_DIR)/os_tick.c
SRC += $(TARGET_OS_SRC_DIR)/os_timeout.c
SRC += $(TARGET_OS_SRC_DIR)/os_timer.c
SRC += $(TARGET_OS_SRC_DIR)/os_init.c
SRC += $(TARGET_OS_SRC_DIR)/os_int.c
SRC += $(TARGET_OS_PORT_SRC_DIR)/system_TMPM370.c
SRC += $(TARGET_OS_PORT_SRC_DIR)/startup_gcc.c
SRC += $(TARGET_OS_PORT_SRC_DIR)/os_port_tick_handler.c
SRC += $(TARGET_OS_PORT_SRC_DIR)/os_port_assert_failed.c
SRC += $(TARGET_OS_PORT_SRC_DIR)/os_port_int_handlers.c
SRC += $(TARGET_OS_PORT_SRC_DIR)/os_port_device_internals.c
SRC += $(TARGET_UTILS_SRC_DIR)/utils.c

#testing source
TESTING_SRC += $(TESTING_OS_SRC_DIR)/os.c

# ---------------------------------

# TESTING:
# TODO: add needed
VPATH +=

TESTING_INC +=

SRC +=
TESTING_SRC +=

#----------------------------------------------------------------------------

CORE = cortex-m3
GCCPREFIX = arm-none-eabi-
LINKER_SCRIPT = ../source/os/port/tmpm370/linker.ld

# Target file name (without extension).
TARGET = $(lastword $(subst /, ,$(CURDIR)))

# Output format.
FORMAT = ihex

OBJDIR = obj
EXECDIR = .
DEPDIR = .dep

TESTING_OBJDIR = ../tests/build/obj
TESTING_EXECDIR = ../tests/build/exe
TESTING_DEPDIR = ../tests/build/.dep

SOURCE := $(sort $(patsubst %.c, %.c,$(notdir $(SRC))))
TESTING_SOURCE := $(sort $(patsubst %.c, %.c,$(notdir $(TESTING_SRC))))

LISTS=$(SOURCE:.c=.lst)
EXTRAINCDIRS = $(INC)
OBJS :=  $(sort $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SOURCE))))
DEPFILES := $(SOURCE:%.c=$(DEPDIR)/%.d)

TESTINGINCDIRS = $(TESTING_INC)
TESTING_OBJS :=  $(sort $(patsubst %.c,$(TESTING_OBJDIR)/%.o,$(notdir $(TESTING_SOURCE))))
TESTING_DEPFILES := $(TESTING_SOURCE:%.c=$(TESTING_DEPDIR)/%.d)

# List Assembler source files here.
#     Make them always end in a capital .S.  Files ending in a lowercase .s
#     will not be considered source files but generated files (assembler
#     output from the compiler), and will be deleted upon "make clean"!
#     Even though the DOS/Win* filesystem matches both .s and .S the same,
#     it will preserve the spelling of the filenames, and gcc itself does
#     care about how the name is spelled on its command-line.
ASRC =


# Optimization level, can be [0, 1, 2, 3, s].
#     0 = turn off optimization. s = optimize for size.
OPT = s


# Debugging format.
DEBUG = dwarf-2


# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99


# Place -D or -U options here for C sources
CDEFS = -DF_CPU=$(F_OSC)


# Place -D or -U options here for ASM sources
ADEFS =

UDEFS =

#---------------- Compiler Options C ----------------
CFLAGS = -g$(DEBUG)
CFLAGS += $(CDEFS)
CFLAGS += $(UDEFS)
CFLAGS += -O$(OPT)
#CFLAGS += -fgnu89-inline
#CFLAGS += -funsigned-char
#CFLAGS += -funsigned-bitfields
#CFLAGS += -fpack-struct
# CFLAGS += -fshort-enums
# CFLAGS += -w # disables all warnings
CFLAGS += -fanalyzer
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpedantic
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-type-limits
CFLAGS += -Wno-deprecated
# CFLAGS += -Wundef
CFLAGS += -Wunreachable-code
CFLAGS += -Wsign-compare
# CFLAGS += -mshort-calls
#CFLAGS += -fno-unit-at-a-time
CFLAGS += -Wa,-adhlns=$(<:%.c=.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)
#CFLAGS += -finline-functions
# CFLAGS += -fmem-report
#CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d
CFLAGS += -MD
CFLAGS += -mapcs-frame
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -nostdlib
CFLAGS += -fdiagnostics-color=always

ARCHFLAGS = -mcpu=$(CORE) -mthumb -mthumb-interwork

## Assembly specific flags
ASMFLAGS =


EXTRALIBDIRS =
#---------------- Linker Options ----------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -Wl,-Map=$(EXECDIR)/$(TARGET).map,--cref
LDFLAGS += -Wl,--gc-sections --specs=nano.specs
LDFLAGS += -T $(LINKER_SCRIPT)
LDFLAGS += $(patsubst %,-L%,$(EXTRALIBDIRS))
LDFLAGS += -Wl,--print-memory-usage


# Define programs and commands.
SHELL = sh
CC = $(GCCPREFIX)gcc-10.3.1
OBJCOPY = $(GCCPREFIX)objcopy
OBJDUMP = $(GCCPREFIX)objdump
SIZE = $(GCCPREFIX)size
AR = $(GCCPREFIX)ar
NM = $(GCCPREFIX)nm
AS = $(GCCPREFIX)as
LD = $(GCCPREFIX)ld
SREC = srec_cat

REMOVE = rm -f
REMOVEDIR = rm -rf
COPY = cp
MOVE = mv

# Define Messages
MSG_SIZE = Size
MSG_FLASH = Creating Hex
MSG_LINKING = "\033[0;32m[L]\033[0m  "
MSG_COMPILING = "\033[0;33m[C]\033[0m  "
MSG_CLEANING = "\033[0;33m[Cleaning Project]\033[0m  "

# Define all listing files.
LST = $(LISTS)

# Compiler flags to generate dependency files.
DEPFLAGS = -MMD -MP -MF $(DEPDIR)/$*.d

ALL_CFLAGS = $(ARCHFLAGS) -I. $(CFLAGS) $(DEPFLAGS)
ALL_ASFLAGS = $(ARCHFLAGS)

HEXSIZE = $(SIZE) --target=$(FORMAT) $(EXECDIR)/$(TARGET).hex
BINSIZE = $(SIZE) --target=binary $(EXECDIR)/$(TARGET).bin
ELFSIZE = $(SIZE) $(EXECDIR)/$(TARGET).elf

print-%  : ; @echo $* = $($*)

all: build checksum
	@echo  "\033[0;33m[$(MSG_SIZE)]\033[0m  "
	$(ELFSIZE)
	@echo  "\033[0;33m[Saving Continuous Build File]\033[0m"
# @cp build_crc.srec cb/build_crc-$(shell date +'%d%m%y_%H%M').srec

staging:
	@echo  "\033[0;33m[$(MSG_SIZE)]\033[0m  "
	$(ELFSIZE)
	@echo  "\033[0;33m[Saving staging Build File]\033[0m"
	@cp build_crc.srec ../../releases/staging_system-$(shell date +'%d%m%y').srec

begin:
	@echo  "\033[0;33mGCC Version \033[0m  "
	@$(CC) --version

format:
	@clang-format -style=file:.clang-format -i $(SRC)

cppcheck:
	@cppcheck --enable=all --addon=misra_cli.json --inline-suppr --error-exitcode=1 -f --suppress=missingInclude --suppress=unmatchedSuppression --suppress=unusedFunction --template=gcc --std=c11 $(SRC)

# Change the build target to build a HEX file or a library.
build: begin elf hex bin srec lss sym
#build: lib

checksum: $(EXECDIR)/$(TARGET).srec
	@echo  "\033[0;33m[Generating Checksum]\033[0m"
	@$(SREC) @srec_dump.txt
	@$(SREC) @srec_file.txt
	@rm $(EXECDIR)/$(TARGET).srec


target_compile_commands:
	@echo  "\033[0;33m[Generating compile_commands.json]\033[0m"
	@$(REMOVE) compile_commands.json
	@compiledb -n make all

crc: all checksum

coflash:
	@coflash.exe program TMPM370FYFG build.bin --driver=./flash/TMPM37x_256.elf --adapter-name=CMSIS-DAP --port=SWD --adapter-clk=2000000 --erase=all --verify=true --postrun=true --reset=HW

jlink:
	@JLinkExe -CommandFile command_file.jlink

elf: $(EXECDIR)/$(TARGET).elf
hex: $(EXECDIR)/$(TARGET).hex
bin: $(EXECDIR)/$(TARGET).bin
srec: $(EXECDIR)/$(TARGET).srec
lss: $(EXECDIR)/$(TARGET).lss
sym: $(EXECDIR)/$(TARGET).sym
LIBNAME=lib$(TARGET).a
lib: $(LIBNAME)


monitor:
	screen $(COM_PORT) $(MONITOR_SPEED)


# Patterns

%.hex: %.elf
	@echo  "\033[0;33m[$(MSG_FLASH)]\033[0m" $@
	@$(OBJCOPY) -O $(FORMAT) $< $@

%.bin: %.elf
	@echo  "\033[0;33m[Creating Bin File]\033[0m" $@
	@$(OBJCOPY) -O binary $< $@

%.srec: %.elf
	@echo  "\033[0;33m[Creating S-Record File]\033[0m" $@
	@$(OBJCOPY) -O srec $< $@

%.lss: %.elf
	@echo  "\033[0;33m[Creating Extended List File]\033[0m" $@
	@$(OBJDUMP) -h -S -z $< > $@

%.sym: %.elf
	@echo  "\033[0;33m[Creating Symbol Table]\033[0m" $@
	@$(NM) -n $< > $@

.SECONDARY : $(EXECDIR)/$(TARGET).elf
.PRECIOUS : $(OBJS)
%.elf: $(OBJS)
	@echo  $(MSG_LINKING) $@
	@$(CC) $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS)

$(OBJDIR)/%.o : %.c
	@echo  $(MSG_COMPILING) $<
	@$(CC) $(ALL_CFLAGS) -c  -o $@ $<

$(DEPDIR): ; @mkdir -p $@

$(DEPFILES):

clean: clean_list
clean_list :
	@echo $(MSG_CLEANING)
	@$(REMOVE) $(TARGET).hex
	@$(REMOVE) $(TARGET).srec
	@$(REMOVE) $(TARGET).bin
	@$(REMOVE) $(TARGET).s19
	@$(REMOVE) $(TARGET).eep
	@$(REMOVE) $(TARGET).cof
	@$(REMOVE) $(TARGET).elf
	@$(REMOVE) $(TARGET).map
	@$(REMOVE) $(TARGET).sym
	@$(REMOVE) $(TARGET).lss
	@$(REMOVE) .lst
	@$(REMOVE) $(SRC:.c=.lst)
	@$(REMOVE) $(SRC:.c=.s)
	@$(REMOVE) $(SRC:.c=.d)
	@$(REMOVE) $(SRC:.c=.i)
	@$(REMOVE) .dep/*.d
	@$(REMOVE) $(TARGET).hex
	@$(REMOVE) $(TARGET).bin
	@$(REMOVE) $(TARGET).s19
	@$(REMOVE) $(TARGET).eep
	@$(REMOVE) $(TARGET).cof
	@$(REMOVE) $(TARGET).elf
	@$(REMOVE) $(TARGET).map
	@$(REMOVE) $(TARGET).sym
	@$(REMOVE) $(TARGET).lss
	@$(REMOVE) obj/*.o
	@$(REMOVE) obj/*.gcno
	@$(REMOVE) build_crc.srec


#-------------------------------------------------------------------------
#-------------------------------------------------------------------------
#-------------------------------------------------------------------------
#-------------------------------------------------------------------------

-include test.mk

# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)

#-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
include $(wildcard $(DEPFILES))

$(TESTING_OBJDIR)/app_processor.o: .FORCE
$(TESTING_OBJDIR)/test_main_update.o: .FORCE
.FORCE :

.PHONY : all begin finish size \
build elf hex bin srec eep lss sym coff extcoff \
clean clean_list copyobj copyhexelfeeplsssym .FORCE
