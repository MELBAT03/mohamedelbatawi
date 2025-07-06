#ifndef APP_NM_USER_INTERFACE_H
#define APP_NM_USER_INTERFACE_H

#include "def.h"

#include "app_memory.h"

#include "app_nm_dry_user_interface.h"
#include "app_nm_wash_user_interface.h"

typedef enum app_nm_userInterface_settingAvaiableState
{
    APP_NORMAL_MODE_UI_SETTING_NOK = 0,
    APP_NORMAL_MODE_UI_SETTING_OK = 1
} app_nm_userInterface_settingAvaiableState_t;

typedef enum app_nm_userInterface_operationSelection
{
    APP_NORMAL_MODE_UI_OPERATION_NONE = 0,
    APP_NORMAL_MODE_UI_OPERATION_WASH_ONLY = 1,
    APP_NORMAL_MODE_UI_OPERATION_WASH_DRY = 2,
    APP_NORMAL_MODE_UI_OPERATION_DRY_ONLY = 4,

    APP_NORMAL_MODE_UI_OPERATION_WASH_DRY_OPTOIN_NUM
} app_nm_userInterface_operationSelection_t;

#define APP_NORMAL_MODE_UI_COURSE_OPERATION_ALL               (0x70U)
#define APP_NORMAL_MODE_UI_COURSE_OPERATION_WASH_ONLY         (0x10U)
#define APP_NORMAL_MODE_UI_COURSE_OPERATION_WASH_WASH_DRY     (0x30U)
#define APP_NORMAL_MODE_UI_COURSE_OPERATION_WASH_DRY_DRY_ONLY (0x60U)
#define APP_NORMAL_MODE_UI_COURSE_OPERATION_DRY_ONLY          (0x40U)

// NOTE: changing the sequence of the courses needs a change in the order of the tables as this is used as table index
typedef enum app_nm_userInterface_courseSelection
{
    APP_NORMAL_MODE_UI_COURSE_NONE = 0x00,
    APP_NORMAL_MODE_UI_COURSE_WHITE = 0x01,
    APP_NORMAL_MODE_UI_COURSE_LIGHT = 0x02,
    APP_NORMAL_MODE_UI_COURSE_DARK = 0x03,
    APP_NORMAL_MODE_UI_COURSE_ECO = 0x04,
    APP_NORMAL_MODE_UI_COURSE_WOOL = APP_NORMAL_MODE_UI_COURSE_ECO,
    APP_NORMAL_MODE_UI_COURSE_COTTON = 0x05,
    APP_NORMAL_MODE_UI_COURSE_JEANS = 0x06,
    APP_NORMAL_MODE_UI_COURSE_DELICATE = 0x07,
    APP_NORMAL_MODE_UI_COURSE_SPORTS = 0x08,
    APP_NORMAL_MODE_UI_COURSE_MIX = 0x09,
    APP_NORMAL_MODE_UI_COURSE_ALERGY = 0x0A,
    APP_NORMAL_MODE_UI_COURSE_RAPIDWASH = APP_NORMAL_MODE_UI_COURSE_ALERGY,
    APP_NORMAL_MODE_UI_COURSE_BEDDING = 0x0B,
    APP_NORMAL_MODE_UI_COURSE_BABYCARE = 0x0C,

    APP_NORMAL_MODE_UI_COURSE_NUM
} app_nm_userInterface_courseSelection_t;

typedef enum app_nm_userInterface_delayStartSelection
{
    APP_NORMAL_MODE_UI_DELAY_START_TIME_0_HOUR = 0U,
    APP_NORMAL_MODE_UI_DELAY_START_TIME_1_HOUR = 1U,
    APP_NORMAL_MODE_UI_DELAY_START_TIME_12_HOUR = 12U,
    APP_NORMAL_MODE_UI_DELAY_START_TIME_END = 12U
} app_nm_userInterface_delayStartSelection_t;

typedef enum app_nm_userInterface_childLockSelection
{
    APP_NORMAL_MODE_UI_CHILD_LOCK_ON = 1,
    APP_NORMAL_MODE_UI_CHILD_LOCK_OFF = 0
} app_nm_userInterface_childLockSelection_t;

typedef enum app_nm_userInterface_cycleTypeSelection
{
    APP_NORMAL_MODE_UI_SINGLE_CYCLE,
    APP_NORMAL_MODE_UI_CONTINUOUS_CYCLE
} app_nm_userInterface_cycleTypeSelection_t;

typedef enum app_nm_userInterface_tubcleanSelection
{
    APP_NORMAL_MODE_UI_TUBCLEAN_OFF,
    APP_NORMAL_MODE_UI_TUBCLEAN_ON
} app_nm_userInterface_tubcleanSelection_t;

typedef struct app_nm_common_userInterface_data
{
    app_nm_userInterface_courseSelection_t courseSelection;
    app_nm_userInterface_operationSelection_t operationSelection;
    app_nm_userInterface_delayStartSelection_t delayStartHourSelection;
    app_nm_userInterface_childLockSelection_t childLockSelection;
    app_nm_userInterface_cycleTypeSelection_t cycleTypeSelection;
    app_nm_userInterface_tubcleanSelection_t tubcleanSelection;
} app_nm_common_userInterface_data_t;

typedef struct app_nm_userInterface_data
{
    app_nm_common_userInterface_data_t common;

    // NOTE: wash user selections
    app_nm_wash_userInterface_data_t wash;

    // NOTE: dry user selections
    app_nm_dry_userInterface_data_t dry;

} app_nm_userInterface_data_t;

typedef enum app_nm_userInterface_state
{
    APP_NM_USER_INTERFACE_NORMAL,
    APP_NM_USER_INTERFACE_DATA_DISPLAY
} app_nm_userInterface_state_t;

void app_nm_userInterface_process(uint32_t period);

app_nm_userInterface_settingAvaiableState_t app_nm_userInterface_setting_check(void);
app_nm_userInterface_settingAvaiableState_t app_nm_userInterface_startPause_check(void);

app_nm_userInterface_courseSelection_t app_nm_userInterface_nextCourseInOperation_get(app_nm_userInterface_courseSelection_t course);

app_nm_userInterface_state_t app_nm_userInterface_state_get(void);

#endif
