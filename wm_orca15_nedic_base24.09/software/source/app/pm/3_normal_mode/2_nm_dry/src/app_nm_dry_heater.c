#include "def.h"
#include "app_nm_dry.h"
#include "app_nm_dry_heater.h"
#include "app_nm.h"
#include "app_heater.h"

#include "app_user_interface.h"

#include "os.h"

#define DRY_HEATER_AUTO_PATTERN_UPPER_LIMIT_MSEC (7200000UL)
#define DRY_HEATER_AUTO_PATTERN_LOWER_LIMIT_MSEC (3600000UL)

#define HEATER_AUTO_TERMINATION_MSEC                           (30000U)
#define LINT_FILTER_CLOGGING_INTERNVAL_MSEC                    (120000U)
#define APP_NORMAL_MODE_DRY_HEATER_LINT_FILTER_CLOGGED_CURRENT (2.7F)
#define HEATER_AUTO_TERMINATION_DROP_PERCENTAGE                (0.1F)

static app_nm_dry_heater_autoTerminationState_t dry_heater_autoTermination_check(uint32_t period);
static void dry_heater_lintFilterClogging_check(uint32_t period);

static void dry_heater_pattern_handle(uint32_t period);

static void heater_operation_handle(uint32_t period);
static void heater_initPattern_handle(void);
static void heater_mainPattern_handle(void);
static void heater_patternSelection_update(void);

static app_nm_dry_heater_patternEnum_t patternState;
static app_nm_dry_heater_initialPatternEnum_t initPatternState;
static app_nm_dry_heater_mainPatternEnum_t mainPatternState;
static app_nm_dry_heater_patternSelection_t patternSelection;

static uint32_t dry_heater_patternPeriod;

static app_nm_dry_heater_autoTerminationState_t gx_dry_heater_autoTerminationState = APP_NORMAL_MODE_DRY_HEATER_AUTO_TERMINATION_NOK;
static app_nm_dry_heater_lintFilterCloggingState_t gx_dry_heater_lintFilterCloggingState = APP_NORMAL_MODE_DRY_HEATER_LINT_FILTER_NOT_CLOGGED;

static double gf_dry_heater_currentFeedbackInitialValue;

void app_nm_dry_heater_process(uint32_t period)
{
    if (APP_NORMAL_MODE_DRY == app_nm_state_get())
    {
        gx_dry_heater_autoTerminationState = dry_heater_autoTermination_check(period);
        dry_heater_lintFilterClogging_check(period);

        switch (app_nm_dry_state_get())
        {
            case APP_NORMAL_MODE_DRY_INIT:
            case APP_NORMAL_MODE_DRY_PRE_SETUP:
            case APP_NORMAL_MODE_DRY_SETUP:
            case APP_NORMAL_MODE_DRY_FINISH:
            {
                app_heater_plateState_set(APP_HEATER_OFF);
            }
            break;

            case APP_NORMAL_MODE_DRY_OPERATION:
            {
                heater_operation_handle(period);
            }
            break;

            case APP_NORMAL_MODE_DRY_DONE:
            default:
            {
                // do nothing
            }
            break;
        }
    }

    LOG('H', 'v', "heaterPattern[period:%d  state:%d  initState:%d  mainState:%d  selection:%d]  autoTermination:%d  clogging:%d  current:%u  temp:%u", dry_heater_patternPeriod, patternState, initPatternState, mainPatternState, patternSelection, gx_dry_heater_autoTerminationState, gx_dry_heater_lintFilterCloggingState, (uint32_t)app_heater_currentFeedback_get(), app_heater_temp_get());
}

uint32_t app_nm_dry_heater_patternPeriod_get(void)
{
    return dry_heater_patternPeriod;
}

void app_nm_dry_heater_patternPeriod_set(uint32_t value)
{
    dry_heater_patternPeriod = value;
}

app_nm_dry_heater_patternEnum_t app_nm_dry_heater_patternState_get(void)
{
    return patternState;
}
app_nm_dry_heater_initialPatternEnum_t app_nm_dry_heater_initialPatternState_get(void)
{
    return initPatternState;
}
app_nm_dry_heater_mainPatternEnum_t app_nm_dry_heater_mainPatternState_get(void)
{
    return mainPatternState;
}
app_nm_dry_heater_patternSelection_t app_nm_dry_heater_patternSelection_get(void)
{
    return patternSelection;
}

app_nm_dry_heater_autoTerminationState_t app_nm_dry_heater_autoTerminationState_get(void)
{
    return gx_dry_heater_autoTerminationState;
}

app_nm_dry_heater_lintFilterCloggingState_t app_nm_dry_heater_lintFilterCloggingState_get(void)
{
    return gx_dry_heater_lintFilterCloggingState;
}

void app_nm_dry_heater_lintFilterCloggingState_set(app_nm_dry_heater_lintFilterCloggingState_t value)
{
    gx_dry_heater_lintFilterCloggingState = value;
}

static app_nm_dry_heater_autoTerminationState_t dry_heater_autoTermination_check(uint32_t period)
{
    static uint32_t timeoutMSec = 0U;
    app_nm_dry_heater_autoTerminationState_t ret = APP_NORMAL_MODE_DRY_HEATER_AUTO_TERMINATION_NOK;

    if ((patternState == APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN) && (mainPatternState == APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_0_2_PLATES) && (gf_dry_heater_currentFeedbackInitialValue * HEATER_AUTO_TERMINATION_DROP_PERCENTAGE) <= (gf_dry_heater_currentFeedbackInitialValue - app_heater_currentFeedback_get()))
    {
        timeoutMSec += period;
        if (timeoutMSec >= HEATER_AUTO_TERMINATION_MSEC)
        {
            timeoutMSec = 0;
            ret = APP_NORMAL_MODE_DRY_HEATER_AUTO_TERMINATION_OK;
        }
    }
    else
    {
        timeoutMSec = 0;
    }

    return ret;
}

static void dry_heater_lintFilterClogging_check(uint32_t period)
{
    static uint32_t timeoutMSec = 0;

    if ((app_heater_plateState_get() == APP_HEATER_TWO_PLATES) && (APP_NORMAL_MODE_DRY_HEATER_LINT_FILTER_CLOGGED_CURRENT > app_heater_currentFeedback_get()))
    {
        timeoutMSec += period;
        if (timeoutMSec >= LINT_FILTER_CLOGGING_INTERNVAL_MSEC)
        {
            timeoutMSec = 0;
#if DRY_CF_DETECTION_ENABLE == 1
            gx_dry_heater_lintFilterCloggingState = APP_NORMAL_MODE_DRY_HEATER_LINT_FILTER_CLOGGED;
#endif
        }
    }
    else
    {
        timeoutMSec = 0;
    }
}

static void heater_operation_handle(uint32_t period)
{
    switch (app_nm_dry_operation_state_get())
    {
        case APP_NORMAL_MODE_DRY_OPERATION_INIT:
        {
            app_heater_plateState_set(APP_HEATER_OFF);

            patternSelection = APP_NORMAL_MODE_DRY_HEATER_DEFAULT_PATTERN;
            patternState = APP_NORMAL_MODE_DRY_HEATER_INIT_PATTERN;
            initPatternState = APP_NORMAL_MODE_DRY_HEATER_INIT_PATTERN_STEP_0_0_PLATES;
            mainPatternState = APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_0_2_PLATES;

            dry_heater_patternPeriod = 0;
        }
        break;

        case APP_NORMAL_MODE_DRY_OPERATION_DRAIN:
        case APP_NORMAL_MODE_DRY_OPERATION_FINAL_SPIN:
        case APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED:
        case APP_NORMAL_MODE_DRY_OPERATION_LINT_FILTER_CLOGGED_RESET:
        case APP_NORMAL_MODE_DRY_OPERATION_COOLING_WATER:
        case APP_NORMAL_MODE_DRY_OPERATION_COOLING_AIR:
        case APP_NORMAL_MODE_DRY_OPERATION_FINISH:
        {
            app_heater_plateState_set(APP_HEATER_OFF);
        }
        break;

        case APP_NORMAL_MODE_DRY_OPERATION_DRYING_SPIN:
        case APP_NORMAL_MODE_DRY_OPERATION_BRAKE:
        case APP_NORMAL_MODE_DRY_OPERATION_UNTANGLE:
        case APP_NORMAL_MODE_DRY_OPERATION_HEATING:
        case APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_1:
        case APP_NORMAL_MODE_DRY_OPERATION_CONDENSE_2:
        {
            dry_heater_pattern_handle(period);
        }
        break;

        case APP_NORMAL_MODE_DRY_OPERATION_EXTRA_TIME:
        {
            app_heater_plateState_set(APP_HEATER_ONE_PLATE);
        }
        break;

        case APP_NORMAL_MODE_DRY_OPERATION_DONE:
        default:
        {
            // do nothing
        }
        break;
    }
}

static void dry_heater_pattern_handle(uint32_t period)
{
    dry_heater_patternPeriod += period;

    switch (patternState)
    {
        case APP_NORMAL_MODE_DRY_HEATER_INIT_PATTERN:
        {
            heater_initPattern_handle();
        }
        break;

        case APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN:
        {
            heater_mainPattern_handle();
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}

static void heater_initPattern_handle(void)
{
    switch (initPatternState)
    {
        case APP_NORMAL_MODE_DRY_HEATER_INIT_PATTERN_STEP_0_0_PLATES:
        {
            if (dry_heater_patternPeriod >= app_nm_dry_heater_table_get(patternSelection)->initialPatternIntervalMSec[initPatternState])
            {
                initPatternState = APP_NORMAL_MODE_DRY_HEATER_INIT_PATTERN_STEP_1_2_PLATES;
                dry_heater_patternPeriod = 0;
            }
            else
            {
                app_heater_plateState_set(APP_HEATER_OFF);
            }
        }
        break;

        case APP_NORMAL_MODE_DRY_HEATER_INIT_PATTERN_STEP_1_2_PLATES:
        {
            if ((app_heater_temp_get() >= DRY_HEATER_TARGET_TEMP) && (0U != app_nm_dry_heater_table_get(patternSelection)->initialPatternIntervalMSec[initPatternState]))
            {
                heater_patternSelection_update();

                initPatternState = APP_NORMAL_MODE_DRY_HEATER_INIT_PATTERN_STEP_2_1_PLATE;
                dry_heater_patternPeriod = 0;

                gf_dry_heater_currentFeedbackInitialValue = app_heater_currentFeedback_get();
            }
            else
            {
                app_heater_plateState_set(app_nm_dry_heater_allowedMaxPlatesPerCourse_get(app_userInterface_data_get()->normalMode.common.courseSelection));
            }
        }
        break;

        case APP_NORMAL_MODE_DRY_HEATER_INIT_PATTERN_STEP_2_1_PLATE:
        {
            if (dry_heater_patternPeriod >= app_nm_dry_heater_table_get(patternSelection)->initialPatternIntervalMSec[initPatternState])
            {
                initPatternState = APP_NORMAL_MODE_DRY_HEATER_INIT_PATTERN_STEP_0_0_PLATES;
                dry_heater_patternPeriod = 0;

                patternState = APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN;
                mainPatternState = APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_0_2_PLATES;
            }
            else
            {
                app_heater_plateState_set(APP_HEATER_ONE_PLATE);
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

static void heater_mainPattern_handle(void)
{
    switch (mainPatternState)
    {
        case APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_0_2_PLATES:
        {
            if ((app_heater_temp_get() >= DRY_HEATER_TARGET_TEMP) && app_nm_dry_heater_table_get(patternSelection)->mainPatternIntervalMSec[mainPatternState])
            {
                mainPatternState = APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_1_1_PLATE;
                dry_heater_patternPeriod = 0;
            }
            else
            {
                app_heater_plateState_set(app_nm_dry_heater_allowedMaxPlatesPerCourse_get(app_userInterface_data_get()->normalMode.common.courseSelection));
            }
        }
        break;

        case APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_1_1_PLATE:
        {
            if (dry_heater_patternPeriod >= app_nm_dry_heater_table_get(patternSelection)->mainPatternIntervalMSec[mainPatternState])
            {
                mainPatternState = APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_2_0_PLATE;
                dry_heater_patternPeriod = 0;
            }
            else
            {
                app_heater_plateState_set(APP_HEATER_ONE_PLATE);
            }
        }
        break;

        case APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_2_0_PLATE:
        {
            if (dry_heater_patternPeriod >= app_nm_dry_heater_table_get(patternSelection)->mainPatternIntervalMSec[mainPatternState])
            {
                mainPatternState = APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_3_1_PLATE;
                dry_heater_patternPeriod = 0;
            }
            else
            {
                app_heater_plateState_set(APP_HEATER_OFF);
            }
        }
        break;

        case APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_3_1_PLATE:
        {
            if (dry_heater_patternPeriod >= app_nm_dry_heater_table_get(patternSelection)->mainPatternIntervalMSec[mainPatternState])
            {
                mainPatternState = APP_NORMAL_MODE_DRY_HEATER_MAIN_PATTERN_STEP_0_2_PLATES;
                dry_heater_patternPeriod = 0;
            }
            else
            {
                app_heater_plateState_set(APP_HEATER_ONE_PLATE);
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

static void heater_patternSelection_update(void)
{
    if ((app_userInterface_data_get()->normalMode.dry.dryTimeSelection == APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_30_MIN) || (app_userInterface_data_get()->normalMode.dry.dryTimeSelection == APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_60_MIN) || (app_userInterface_data_get()->normalMode.dry.dryTimeSelection == APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_90_MIN))
    {
        patternSelection = APP_NORMAL_MODE_DRY_HEATER_MANUAL_PATTERN;
    }
    else if (app_userInterface_data_get()->normalMode.dry.dryTimeSelection == APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_AUTO)
    {
        if (dry_heater_patternPeriod <= DRY_HEATER_AUTO_PATTERN_LOWER_LIMIT_MSEC)
        {
            patternSelection = APP_NORMAL_MODE_DRY_HEATER_AUTO_1_PATTERN;
        }
        else if ((dry_heater_patternPeriod > DRY_HEATER_AUTO_PATTERN_LOWER_LIMIT_MSEC) && (dry_heater_patternPeriod <= DRY_HEATER_AUTO_PATTERN_UPPER_LIMIT_MSEC))
        {
            patternSelection = APP_NORMAL_MODE_DRY_HEATER_AUTO_2_PATTERN;
        }
        else if (dry_heater_patternPeriod > DRY_HEATER_AUTO_PATTERN_UPPER_LIMIT_MSEC)
        {
            patternSelection = APP_NORMAL_MODE_DRY_HEATER_AUTO_3_PATTERN;
        }
        else
        {
            // do nothing
        }
    }
    else
    {
        // do nothing
    }
}
