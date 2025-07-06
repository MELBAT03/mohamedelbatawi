#include "def.h"
#include "config.h"

#include "app_memory.h"
#include "app_nm.h"
#ifdef DRYER_ENABLED
#include "app_nm_dry.h"
#include "app_nm_dry_user_interface.h"
#endif
#include "app_buzzer.h"

#include "app_user_interface.h"
#include "app_nm_user_interface.h"
#include "app_nm_init_user_interface.h"
#include "app_nm_tubclean_user_interface.h"
#include "app_nm_pause_user_interface.h"
#include "app_nm_finish_user_interface.h"

#include "app_nm_data_display.h"
#include "app_nm_endurance.h"

#include "handlers/input/inc/handler_keypad.h"

typedef struct app_nm_userInterface_courseSequence
{
    app_nm_userInterface_courseSelection_t sequence;
    uint8_t operationFlag;
} app_nm_userInterface_courseSequence_t;

static app_nm_userInterface_state_t gx_app_nm_userInterface_state = APP_NM_USER_INTERFACE_NORMAL;

#ifdef COURSE_SELECTION_SINGLE_BUTTON
static app_nm_userInterface_courseSequence_t gx_nm_userInterface_courseSequence[] = {
    {.sequence = APP_NORMAL_MODE_UI_DEFAULT_COURSE, .operationFlag = APP_NORMAL_MODE_UI_COURSE_OPERATION_ALL},
    {.sequence = APP_NORMAL_MODE_UI_COURSE_LIGHT, .operationFlag = APP_NORMAL_MODE_UI_COURSE_OPERATION_ALL},
    {.sequence = APP_NORMAL_MODE_UI_COURSE_DARK, .operationFlag = APP_NORMAL_MODE_UI_COURSE_OPERATION_ALL},
    {.sequence = APP_NORMAL_MODE_UI_COURSE_WHITE, .operationFlag = APP_NORMAL_MODE_UI_COURSE_OPERATION_ALL},
    {.sequence = APP_NORMAL_MODE_UI_COURSE_JEANS, .operationFlag = APP_NORMAL_MODE_UI_COURSE_OPERATION_ALL},
    {.sequence = APP_NORMAL_MODE_UI_COURSE_ECO, .operationFlag = APP_NORMAL_MODE_UI_COURSE_OPERATION_ALL},
    {.sequence = APP_NORMAL_MODE_UI_COURSE_BABYCARE, .operationFlag = APP_NORMAL_MODE_UI_COURSE_OPERATION_ALL},
    {.sequence = APP_NORMAL_MODE_UI_COURSE_BEDDING, .operationFlag = APP_NORMAL_MODE_UI_COURSE_OPERATION_ALL},
    {.sequence = APP_NORMAL_MODE_UI_COURSE_ALERGY, .operationFlag = APP_NORMAL_MODE_UI_COURSE_OPERATION_ALL},
    {.sequence = APP_NORMAL_MODE_UI_COURSE_SPORTS, .operationFlag = APP_NORMAL_MODE_UI_COURSE_OPERATION_ALL},
    {.sequence = APP_NORMAL_MODE_UI_COURSE_DELICATE, .operationFlag = APP_NORMAL_MODE_UI_COURSE_OPERATION_ALL},
    {.sequence = APP_NORMAL_MODE_UI_COURSE_MIX, .operationFlag = APP_NORMAL_MODE_UI_COURSE_OPERATION_ALL}};
#endif

void app_nm_userInterface_process(uint32_t period)
{
    static void (*const handlers[])(uint32_t period) =
        {
            app_nm_init_userInterface_process,
            app_nm_wash_userInterface_process,
#ifdef DRYER_ENABLED
            app_nm_dry_userInterface_process,
#else
            NULL,
#endif
            app_nm_tubclean_userInterface_process,
            app_nm_pause_userInterface_process,
            app_nm_finish_userInterface_process,
            NULL};

    switch (gx_app_nm_userInterface_state)
    {
        case APP_NM_USER_INTERFACE_NORMAL:
        {
            if (handlers[app_nm_state_get()] != NULL)
            {
                handlers[app_nm_state_get()](period);
            }

            if (app_userInterface_data_get()->normalMode.common.childLockSelection == APP_NORMAL_MODE_UI_CHILD_LOCK_OFF)
            {
                gx_app_nm_userInterface_state = app_nm_userInterface_dataDisplay_entry_check(period);
                app_nm_endurance_entry_check(period);
            }
        }
        break;

        case APP_NM_USER_INTERFACE_DATA_DISPLAY:
        {
            app_nm_userInterface_dataDisplay_handle();
            gx_app_nm_userInterface_state = app_nm_userInterface_dataDisplay_exit_check();
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}

app_nm_userInterface_settingAvaiableState_t app_nm_userInterface_setting_check(void)
{
    app_nm_userInterface_settingAvaiableState_t ret = APP_NORMAL_MODE_UI_SETTING_NOK;

    if ((app_userInterface_data_get()->normalMode.wash.washSelection != APP_NORMAL_MODE_UI_WASH_TIME_0_MIN) ||
        (app_userInterface_data_get()->normalMode.wash.rinseSelection != APP_NORMAL_MODE_UI_RINSE_0_TIMES) ||
        (app_userInterface_data_get()->normalMode.wash.spinSelection != APP_NORMAL_MODE_UI_SPIN_TIME_0_MIN) ||
#ifdef DRYER_ENABLED
        (app_userInterface_data_get()->normalMode.dry.dryTimeSelection != APP_NORMAL_MODE_UI_DRY_TIME_SELECTION_0_MIN) ||
#endif
        (app_userInterface_data_get()->normalMode.common.tubcleanSelection != APP_NORMAL_MODE_UI_TUBCLEAN_OFF))
    {
        ret = APP_NORMAL_MODE_UI_SETTING_OK;
    }

    return ret;
}

app_nm_userInterface_settingAvaiableState_t app_nm_userInterface_startPause_check(void)
{
    app_nm_userInterface_settingAvaiableState_t ret = APP_NORMAL_MODE_UI_SETTING_NOK;

    if ((app_userInterface_keyEdgeState_get()->startPauseKeyEdgeState == HANDLER_KEY_PRESSED) && (app_userInterface_data_get()->normalMode.common.childLockSelection == APP_NORMAL_MODE_UI_CHILD_LOCK_OFF))
    {
        ret = APP_NORMAL_MODE_UI_SETTING_OK;
    }

    return ret;
}

app_nm_userInterface_courseSelection_t app_nm_userInterface_nextCourseInOperation_get(app_nm_userInterface_courseSelection_t course)
{
    app_nm_userInterface_courseSelection_t ret = APP_NORMAL_MODE_UI_COURSE_NONE;

#ifdef COURSE_SELECTION_SINGLE_BUTTON
    uint32_t search = 0;
    uint8_t i = 0;
    uint8_t j = 0;

    // search for the current course in the courseSequence array
    for (i = 0; i < APP_NORMAL_MODE_UI_COURSE_NUM; i++)
    {
        if (gx_nm_userInterface_courseSequence[i].sequence == course)
        {
            i++;   // move to next in order
            break;
        }
    }

    for (j = i; j < APP_NORMAL_MODE_UI_COURSE_NUM; j++)
    {
        search = gx_nm_userInterface_courseSequence[j].operationFlag;

        if (0U != (search & (((uint32_t)app_userInterface_data_get()->normalMode.common.operationSelection) << 4U)))
        {
            break;
        }
    }

    if (j == APP_NORMAL_MODE_UI_COURSE_NUM)
    {
        j = APP_NORMAL_MODE_UI_DEFAULT_COURSE;
    }

    ret = gx_nm_userInterface_courseSequence[j].sequence;

#elif defined(COURSE_SELECTION_MULTIPLE_BUTTONS)
    if (app_userInterface_keyEdgeState_get()->courseCottonKeyEdgeState == HANDLER_KEY_PRESSED)
    {
        ret = APP_NORMAL_MODE_UI_COURSE_COTTON;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_OFF;
    }
    else if (app_userInterface_keyEdgeState_get()->courseLightKeyEdgeState == HANDLER_KEY_PRESSED)
    {
        ret = APP_NORMAL_MODE_UI_COURSE_LIGHT;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_OFF;
    }
    else if (app_userInterface_keyEdgeState_get()->courseDarksKeyEdgeState == HANDLER_KEY_PRESSED)
    {
        ret = APP_NORMAL_MODE_UI_COURSE_DARK;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_OFF;
    }
    else if (app_userInterface_keyEdgeState_get()->courseWhiteKeyEdgeState == HANDLER_KEY_PRESSED)
    {
        ret = APP_NORMAL_MODE_UI_COURSE_WHITE;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_OFF;
    }
    else if (app_userInterface_keyEdgeState_get()->courseJeansKeyEdgeState == HANDLER_KEY_PRESSED)
    {
        ret = APP_NORMAL_MODE_UI_COURSE_JEANS;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_OFF;
    }
#ifdef AUTO_DETERGENT_HEATER_MODEL
    else if (app_userInterface_keyEdgeState_get()->courseECOKeyEdgeState == HANDLER_KEY_PRESSED)
    {
        ret = APP_NORMAL_MODE_UI_COURSE_ECO;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_OFF;
    }
    else if (app_userInterface_keyEdgeState_get()->courseAlergyKeyEdgeState == HANDLER_KEY_PRESSED)
    {
        ret = APP_NORMAL_MODE_UI_COURSE_ALERGY;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_OFF;
    }
#else
    else if (app_userInterface_keyEdgeState_get()->courseWoolKeyEdgeState == HANDLER_KEY_PRESSED)
    {
        ret = APP_NORMAL_MODE_UI_COURSE_WOOL;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_OFF;
    }
    else if (app_userInterface_keyEdgeState_get()->courseRapidWashKeyEdgeState == HANDLER_KEY_PRESSED)
    {
        ret = APP_NORMAL_MODE_UI_COURSE_RAPIDWASH;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_OFF;
    }
#endif
    else if (app_userInterface_keyEdgeState_get()->courseBabyCareKeyEdgeState == HANDLER_KEY_PRESSED)
    {
        ret = APP_NORMAL_MODE_UI_COURSE_BABYCARE;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_OFF;
    }
    else if (app_userInterface_keyEdgeState_get()->courseBeddingKeyEdgeState == HANDLER_KEY_PRESSED)
    {
        ret = APP_NORMAL_MODE_UI_COURSE_BEDDING;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_OFF;
    }
    else if (app_userInterface_keyEdgeState_get()->courseSportsKeyEdgeState == HANDLER_KEY_PRESSED)
    {
        ret = APP_NORMAL_MODE_UI_COURSE_SPORTS;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_OFF;
    }
    else if (app_userInterface_keyEdgeState_get()->courseDelicateKeyEdgeState == HANDLER_KEY_PRESSED)
    {
        ret = APP_NORMAL_MODE_UI_COURSE_DELICATE;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_OFF;
    }
    else if (app_userInterface_keyEdgeState_get()->courseMixKeyEdgeState == HANDLER_KEY_PRESSED)
    {
        ret = APP_NORMAL_MODE_UI_COURSE_MIX;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_OFF;
    }
    else if (app_userInterface_keyEdgeState_get()->tubCleanKeyEdgeState == HANDLER_KEY_PRESSED)
    {
        ret = APP_NORMAL_MODE_UI_COURSE_NONE;
        app_userInterface_data_set()->normalMode.common.tubcleanSelection = APP_NORMAL_MODE_UI_TUBCLEAN_ON;
    }
    else
    {
        // do nothing
    }
#endif

    return ret;
}

app_nm_userInterface_state_t app_nm_userInterface_state_get(void)
{
    return gx_app_nm_userInterface_state;
}
