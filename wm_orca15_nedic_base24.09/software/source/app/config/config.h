#ifndef CONFIG_H
#define CONFIG_H

#if !defined(WM_MODEL_11KG) && !defined(WM_MODEL_13KG)
#error "define wm model"
#endif

#define WM_DDM
// #define CLUTCH_DECLUTCH_ENABLE
// #define MECH_BRAKE
// #define DRYER_ENABLED
// #define STEAM_ENABLED
// #define COOLING_FAN_ENABLED
// #define BLOWER_ENABLED
// #define LINT_FILTER_ENABLED
// #define CONDENSER_ENABLED

// NOTE: defined in tasks
// #define AUTO_DETERGENT_HEATER_MODEL

// #define BYPASS_LINT_FILTER_CHECK

// #define USER_DETERGENT_SKIP_ENABLE
#define REMAINING_TIME_ALWAYS_IN_STANDBY
// #define STANDBY_FIRST_PRESS_ENABLE

#define WASH_STIR_TIME_CALCULATION_BY_SYSTEM_TABLES   // this must change the enum of wash selection to 1 2 3 instead of 3 9 12

// #define COURSE_SELECTION_SINGLE_BUTTON
#define COURSE_SELECTION_MULTIPLE_BUTTONS

// #define COURSE_CHANGE_DURING_CYCLE_ENABLE
// #define WATER_TEMP_CHANGE_DURING_CYCLE_ENABLE

// #define HEARTBEAT_LED_ENABLE

#endif
