#include "components/com_display/com_display.h"
#include "board.h"
#include "handlers/output/inc/handler_display.h"
#include "components/ssd/ssd.h"

#include "app_user_interface.h"

static void lidLock_ledGroup_handle(uint32_t value);
static void wash_ledGroup_handle(uint32_t value);
static void washButton_ledGroup_handle(uint32_t value);
static void extraSpin_ledGroup_handle(uint32_t value);
static void soak_ledGroup_handle(uint32_t value);
static void waterLevel_ledGroup_handle(uint32_t value);
static void waterLevelButton_ledGroup_handle(uint32_t value);
static void delayStart_ledGroup_handle(uint32_t value);
static void delayStartButton_ledGroup_handle(uint32_t value);
static void course_ledGroup_handle(uint32_t value);
static void rinse_ledGroup_handle(uint32_t value);
static void rinseButton_ledGroup_handle(uint32_t value);
static void spin_ledGroup_handle(uint32_t value);
static void spinButton_ledGroup_handle(uint32_t value);
static void childLock_ledGroup_handle(uint32_t value);
static void waterTemp_ledGroup_handle(uint32_t value);
static void waterTempButton_ledGroup_handle(uint32_t value);
static void stainLevel_ledGroup_handle(uint32_t value);
static void stainLevelButton_ledGroup_handle(uint32_t value);
static void steamTech_ledGroup_handle(uint32_t value);
static void steamTechButton_ledGroup_handle(uint32_t value);
static void gelDetergent_ledGroup_handle(uint32_t value);
static void gelDetergentButton_ledGroup_handle(uint32_t value);
static void tubclean_ledGroup_handle(uint32_t value);
static void tubcleanButton_ledGroup_handle(uint32_t value);
static void heater_ledGroup_handle(uint32_t value);
static void powderDetergent_ledGroup_handle(uint32_t value);
static void softner_ledGroup_handle(uint32_t value);
static void min_ledGroup_handle(uint32_t value);
static void times_ledGroup_handle(uint32_t value);
static void start_ledGroup_handle(uint32_t value);
static void onOff_ledGroup_handle(uint32_t value);

static uint8_t gau8_app_displayBuffer_ledState[LED_MATRIX_LED_NUM] = {0};
static uint8_t gau8_app_displayBuffer_ssdState[SSD_DIGITS_NUM] = {SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_NULL_SYMBOL, SSD_NULL_SYMBOL};
static uint8_t gau8_app_displayBuffer_dpState = 0;
static uint8_t gu8_app_displayBuffer_dimEnable = 0;

void handler_display_init(void)
{
    com_display_config_t display;

    display.resetSignalPin.port = DISPLAY_RESET_SIGNAL_PORT;
    display.resetSignalPin.pin = DISPLAY_RESET_SIGNAL_PIN;
    display.resetSignalPin.ioState = MCAL_GPIO_OUTPUT;
    display.resetSignalPin.pinState = GPIO_LOW;

    display.commDisplayID = DISPLAY_PORT_ID;

    display.frameWaitTimeMSec = 20;
    display.receiveTimeoutMSec = 1000;

    display.ptr_dimGeter = &handler_display_dimState_get;
    display.ptr_ledGeter = &handler_display_ledState_get;
    display.ptr_ssdGeter = &handler_display_ssd_get;
    display.ptr_dpGeter = &handler_display_ssdDP_get;

    com_display_init(&display);

    handler_display_ssd_set(0, SSD_NULL_SYMBOL);
    handler_display_ssd_set(1, SSD_NULL_SYMBOL);
    handler_display_ssd_set(2, SSD_NULL_SYMBOL);
    handler_display_ssd_set(3, SSD_NULL_SYMBOL);
}

void handler_display_end(void)
{
    com_display_state_set(COM_DISPLAY_DONE);
    for (uint8_t i = 0; i < LED_MATRIX_LED_NUM; i++)
    {
        gau8_app_displayBuffer_ledState[i] = 0;
    }
}

uint8_t handler_display_version_get(void)
{
    return com_display_displayVersion_get();
}

uint8_t handler_display_ledState_get(uint8_t index)
{
    return gau8_app_displayBuffer_ledState[index];
}

uint8_t handler_display_dimState_get(void)
{
    return gu8_app_displayBuffer_dimEnable;
}

void handler_display_dimState_set(uint8_t state)
{
    state &= 0x01;
    gu8_app_displayBuffer_dimEnable = state;
}

void handler_display_ledState_set(uint8_t value, uint8_t index)
{
    gau8_app_displayBuffer_ledState[index] = value;
}

void handler_display_ssd_set(uint8_t digitID, uint8_t symbol)
{
    gau8_app_displayBuffer_ssdState[digitID] = symbol;
}

uint8_t handler_display_ssd_get(uint8_t digitID)
{
    return gau8_app_displayBuffer_ssdState[digitID];
}

void handler_display_ssdDP_set(uint8_t value)
{
    if (value == SSD_DP_ON)
    {
        gau8_app_displayBuffer_dpState = 3;
    }
    else
    {
        gau8_app_displayBuffer_dpState = 0;
    }
}

uint8_t handler_display_ssdDP_get(void)
{
    return gau8_app_displayBuffer_dpState;
}

void handler_display_ssd_clear(void)
{
    for (uint8_t i = 0; i < SSD_DIGITS_NUM; i++)
    {
        gau8_app_displayBuffer_ssdState[i] = SSD_NULL_SYMBOL;
    }
    gau8_app_displayBuffer_dpState = 0;
}

void handler_display_led_clear(void)
{
    for (uint8_t i = 0; i < LED_MATRIX_LED_NUM; i++)
    {
        gau8_app_displayBuffer_ledState[i] = 0;
    }
}

void handler_display_clear(void)
{
    for (uint8_t i = 0; i < LED_MATRIX_LED_NUM; i++)
    {
        gau8_app_displayBuffer_ledState[i] = 0;
    }

    gau8_app_displayBuffer_dpState = 0;

    for (uint8_t i = 0; i < SSD_DIGITS_NUM; i++)
    {
        gau8_app_displayBuffer_ssdState[i] = SSD_NULL_SYMBOL;
    }
}

void handler_display_fire(void)
{
    gau8_app_displayBuffer_ssdState[SSD_DIGIT_0_ID] = SSD_8_SYMBOL;
    gau8_app_displayBuffer_ssdState[SSD_DIGIT_1_ID] = SSD_8_SYMBOL;
    gau8_app_displayBuffer_ssdState[SSD_DIGIT_2_ID] = SSD_8_SYMBOL;
    gau8_app_displayBuffer_ssdState[SSD_DIGIT_3_ID] = SSD_8_SYMBOL;

    gau8_app_displayBuffer_dpState = 3;

    for (uint8_t i = 0; i < LED_MATRIX_LED_NUM; i++)
    {
        gau8_app_displayBuffer_ledState[i] = 1;
    }
}

uint8_t handler_display_error_check(void)
{
    uint8_t ret = 0;

    if (com_display_state_get() == COM_DISPLAY_ERROR)
    {
        ret = 1;
    }

    return ret;
}

void handler_display_courseLedSequence_traverse(uint8_t id)
{
    gau8_app_displayBuffer_ledState[LED_COURSE_MIX_ID] = LED_MATRIX_OFF;
    gau8_app_displayBuffer_ledState[LED_COURSE_BEDDING_ID] = LED_MATRIX_OFF;
    gau8_app_displayBuffer_ledState[LED_COURSE_WHITE_ID] = LED_MATRIX_OFF;
    gau8_app_displayBuffer_ledState[LED_COURSE_DELICATE_ID] = LED_MATRIX_OFF;
    gau8_app_displayBuffer_ledState[LED_COURSE_BABYCARE_ID] = LED_MATRIX_OFF;
    gau8_app_displayBuffer_ledState[LED_COURSE_DARK_ID] = LED_MATRIX_OFF;
    gau8_app_displayBuffer_ledState[LED_COURSE_SPORTS_ID] = LED_MATRIX_OFF;
    gau8_app_displayBuffer_ledState[LED_COURSE_ECO_ID] = LED_MATRIX_OFF;
    gau8_app_displayBuffer_ledState[LED_COURSE_LIGHT_ID] = LED_MATRIX_OFF;
    gau8_app_displayBuffer_ledState[LED_COURSE_ALERGY_ID] = LED_MATRIX_OFF;
    gau8_app_displayBuffer_ledState[LED_COURSE_JEANS_ID] = LED_MATRIX_OFF;
    gau8_app_displayBuffer_ledState[LED_COURSE_COTTON_ID] = LED_MATRIX_OFF;

    for (uint8_t j = 0; j <= id; j++)
    {
        gau8_app_displayBuffer_ledState[j] = LED_MATRIX_ON;
    }
}
void handler_display_ledGroup_set(handler_display_ledGroup_t ledGroup, uint32_t value)
{
    switch (ledGroup)
    {
        case HANDLER_DISPLAY_LID_LOCK_LED_GROUP:
        {
            lidLock_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_WASH_TIME_LED_GROUP:
        {
            wash_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_WASH_TIME_BUTTON_LED_GROUP:
        {
            washButton_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_EXTRA_SPIN_LED_GROUP:
        {
            extraSpin_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_SOAK_LED_GROUP:
        {
            soak_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_WATER_LEVEL_LED_GROUP:
        {
            waterLevel_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_WATER_LEVEL_BUTTON_LED_GROUP:
        {
            waterLevelButton_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_DELAY_START_LED_GROUP:
        {
            delayStart_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_DELAY_START_BUTTON_LED_GROUP:
        {
            delayStartButton_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_RINSE_LED_GROUP:
        {
            rinse_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_RINSE_BUTTON_LED_GROUP:
        {
            rinseButton_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_SPIN_LED_GROUP:
        {
            spin_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_SPIN_BUTTON_LED_GROUP:
        {
            spinButton_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_CHILD_LOCK_LED_GROUP:
        {
            childLock_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_COURSE_LED_GROUP:
        {
            course_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_WATER_TEMP_LED_GROUP:
        {
            waterTemp_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_WATER_TEMP_BUTTON_LED_GROUP:
        {
            waterTempButton_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_STAIN_LEVEL_LED_GROUP:
        {
            stainLevel_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_STAIN_LEVEL_BUTTON_LED_GROUP:
        {
            stainLevelButton_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_STEAM_TECH_LED_GROUP:
        {
            steamTech_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_STEAM_TECH_BUTTON_LED_GROUP:
        {
            steamTechButton_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_GEL_DETERGENT_LED_GROUP:
        {
            gelDetergent_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_GEL_DETERGENT_BUTTON_LED_GROUP:
        {
            gelDetergentButton_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_TUBCLEAN_LED_GROUP:
        {
            tubclean_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_TUBCLEAN_BUTTON_LED_GROUP:
        {
            tubcleanButton_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_HEATER_LED_GROUP:
        {
            heater_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_POWDER_DETERGENT_LED_GROUP:
        {
            powderDetergent_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_SOFTNER_LED_GROUP:
        {
            softner_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_MIN_LED_GROUP:
        {
            min_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_TIMES_LED_GROUP:
        {
            times_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_START_LED_GROUP:
        {
            start_ledGroup_handle(value);
        }
        break;

        case HANDLER_DISPLAY_ON_OFF_LED_GROUP:
        {
            onOff_ledGroup_handle(value);
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
}

static void lidLock_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_LID_LOCK_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_LID_LOCK_ID] = LED_MATRIX_ON;
    }
}
static void wash_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_WASH_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_WASH_ID] = LED_MATRIX_ON;
    }
}
static void washButton_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_WASH_BUTTON_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_WASH_BUTTON_ID] = LED_MATRIX_ON;
    }
}
static void extraSpin_ledGroup_handle(uint32_t value)
{
    // if (value == 0)
    // {
    //     gau8_app_displayBuffer_ledState[LED_EXTRA_SPIN_ID] = LED_MATRIX_OFF;
    // }
    // else
    // {
    //     gau8_app_displayBuffer_ledState[LED_EXTRA_SPIN_ID] = LED_MATRIX_ON;
    // }
}
static void soak_ledGroup_handle(uint32_t value)
{
    // if (value == 0)
    // {
    //     gau8_app_displayBuffer_ledState[LED_SOAK_ID] = LED_MATRIX_OFF;
    // }
    // else
    // {
    //     gau8_app_displayBuffer_ledState[LED_SOAK_ID] = LED_MATRIX_ON;
    // }
}
static void waterLevel_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelDisplaySelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_0;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_1_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_2_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_3_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_4_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_5_ID] = LED_MATRIX_OFF;
    }
    else if (value == 1)
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelDisplaySelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_1;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_1_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_2_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_3_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_4_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_5_ID] = LED_MATRIX_OFF;
    }
    else if (value == 2 || value == 3)
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelDisplaySelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_2;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_1_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_2_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_3_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_4_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_5_ID] = LED_MATRIX_OFF;
    }
    else if (value == 4 || value == 5)
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelDisplaySelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_3;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_1_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_2_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_3_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_4_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_5_ID] = LED_MATRIX_OFF;
    }
    else if (value == 6 || value == 7)
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelDisplaySelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_4;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_1_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_2_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_3_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_4_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_5_ID] = LED_MATRIX_OFF;
    }
    else if (value >= 8)
    {
        app_userInterface_data_set()->normalMode.wash.waterLevelDisplaySelection = APP_NORMAL_MODE_WASH_UI_WATER_LEVEL_5;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_1_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_2_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_3_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_4_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_5_ID] = LED_MATRIX_ON;
    }
    else
    {
        // do nothing
    }
}
static void waterLevelButton_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_BUTTON_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_WATER_LEVEL_BUTTON_ID] = LED_MATRIX_ON;
    }
}
static void delayStart_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_DELAY_START_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_DELAY_START_ID] = LED_MATRIX_ON;
    }
}
static void delayStartButton_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_DELAY_START_BUTTON_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_DELAY_START_BUTTON_ID] = LED_MATRIX_ON;
    }
}
static void course_ledGroup_handle(uint32_t value)
{
    if (value == APP_NORMAL_MODE_UI_COURSE_NONE)
    {
        gau8_app_displayBuffer_ledState[LED_COURSE_MIX_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BEDDING_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_WHITE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DELICATE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BABYCARE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DARK_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_SPORTS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ECO_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ALERGY_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_JEANS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_COTTON_ID] = LED_MATRIX_OFF;
    }
    else if (value == APP_NORMAL_MODE_UI_COURSE_COTTON)
    {
        gau8_app_displayBuffer_ledState[LED_COURSE_MIX_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BEDDING_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_WHITE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DELICATE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BABYCARE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DARK_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_SPORTS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ECO_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ALERGY_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_JEANS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_COTTON_ID] = LED_MATRIX_ON;
    }
    else if (value == APP_NORMAL_MODE_UI_COURSE_LIGHT)
    {
        gau8_app_displayBuffer_ledState[LED_COURSE_MIX_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BEDDING_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_WHITE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DELICATE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BABYCARE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DARK_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_SPORTS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ECO_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_LIGHT_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_ALERGY_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_JEANS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_COTTON_ID] = LED_MATRIX_OFF;
    }
    else if (value == APP_NORMAL_MODE_UI_COURSE_DARK)
    {
        gau8_app_displayBuffer_ledState[LED_COURSE_MIX_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BEDDING_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_WHITE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DELICATE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BABYCARE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DARK_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_SPORTS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ECO_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ALERGY_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_JEANS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_COTTON_ID] = LED_MATRIX_OFF;
    }
    else if (value == APP_NORMAL_MODE_UI_COURSE_WHITE)
    {
        gau8_app_displayBuffer_ledState[LED_COURSE_MIX_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BEDDING_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_WHITE_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_DELICATE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BABYCARE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DARK_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_SPORTS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ECO_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ALERGY_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_JEANS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_COTTON_ID] = LED_MATRIX_OFF;
    }
    else if (value == APP_NORMAL_MODE_UI_COURSE_JEANS)
    {
        gau8_app_displayBuffer_ledState[LED_COURSE_MIX_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BEDDING_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_WHITE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DELICATE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BABYCARE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DARK_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_SPORTS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ECO_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ALERGY_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_JEANS_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_COTTON_ID] = LED_MATRIX_OFF;
    }
    else if (value == APP_NORMAL_MODE_UI_COURSE_ECO)
    {
        gau8_app_displayBuffer_ledState[LED_COURSE_MIX_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BEDDING_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_WHITE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DELICATE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BABYCARE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DARK_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_SPORTS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ECO_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ALERGY_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_JEANS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_COTTON_ID] = LED_MATRIX_OFF;
    }
    else if (value == APP_NORMAL_MODE_UI_COURSE_BABYCARE)
    {
        gau8_app_displayBuffer_ledState[LED_COURSE_MIX_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BEDDING_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_WHITE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DELICATE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BABYCARE_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_DARK_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_SPORTS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ECO_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ALERGY_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_JEANS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_COTTON_ID] = LED_MATRIX_OFF;
    }
    else if (value == APP_NORMAL_MODE_UI_COURSE_BEDDING)
    {
        gau8_app_displayBuffer_ledState[LED_COURSE_MIX_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BEDDING_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_WHITE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DELICATE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BABYCARE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DARK_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_SPORTS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ECO_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ALERGY_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_JEANS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_COTTON_ID] = LED_MATRIX_OFF;
    }
    else if (value == APP_NORMAL_MODE_UI_COURSE_ALERGY)
    {
        gau8_app_displayBuffer_ledState[LED_COURSE_MIX_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BEDDING_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_WHITE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DELICATE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BABYCARE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DARK_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_SPORTS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ECO_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ALERGY_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_JEANS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_COTTON_ID] = LED_MATRIX_OFF;
    }
    else if (value == APP_NORMAL_MODE_UI_COURSE_SPORTS)
    {
        gau8_app_displayBuffer_ledState[LED_COURSE_MIX_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BEDDING_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_WHITE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DELICATE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BABYCARE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DARK_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_SPORTS_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_ECO_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ALERGY_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_JEANS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_COTTON_ID] = LED_MATRIX_OFF;
    }
    else if (value == APP_NORMAL_MODE_UI_COURSE_DELICATE)
    {
        gau8_app_displayBuffer_ledState[LED_COURSE_MIX_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BEDDING_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_WHITE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DELICATE_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_BABYCARE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DARK_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_SPORTS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ECO_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ALERGY_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_JEANS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_COTTON_ID] = LED_MATRIX_OFF;
    }
    else if (value == APP_NORMAL_MODE_UI_COURSE_MIX)
    {
        gau8_app_displayBuffer_ledState[LED_COURSE_MIX_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_BEDDING_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_WHITE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DELICATE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_BABYCARE_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_DARK_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_SPORTS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ECO_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_ALERGY_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_JEANS_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_COURSE_COTTON_ID] = LED_MATRIX_OFF;
    }
    else if (value == 0xff)
    {
        gau8_app_displayBuffer_ledState[LED_COURSE_MIX_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_BEDDING_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_WHITE_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_DELICATE_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_BABYCARE_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_DARK_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_SPORTS_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_ECO_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_LIGHT_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_ALERGY_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_JEANS_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_COURSE_COTTON_ID] = LED_MATRIX_ON;
    }
    else
    {
        // do nothing
    }
}
static void rinse_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_RINSE_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_RINSE_ID] = LED_MATRIX_ON;
    }
}
static void rinseButton_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_RINSE_BUTTON_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_RINSE_BUTTON_ID] = LED_MATRIX_ON;
    }
}
static void spin_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_SPIN_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_SPIN_ID] = LED_MATRIX_ON;
    }
}
static void spinButton_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_SPIN_BUTTON_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_SPIN_BUTTON_ID] = LED_MATRIX_ON;
    }
}
static void childLock_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_CHILD_LOCK_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_CHILD_LOCK_ID] = LED_MATRIX_ON;
    }
}
static void waterTemp_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_TEMP_COLD_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_TEMP_WARM_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_TEMP_HOT_ID] = LED_MATRIX_OFF;
    }
    else if (value == 1)
    {
        gau8_app_displayBuffer_ledState[LED_TEMP_COLD_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_TEMP_WARM_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_TEMP_HOT_ID] = LED_MATRIX_OFF;
    }
    else if (value == 2)
    {
        gau8_app_displayBuffer_ledState[LED_TEMP_COLD_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_TEMP_WARM_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_TEMP_HOT_ID] = LED_MATRIX_OFF;
    }
    else if (value == 3)
    {
        gau8_app_displayBuffer_ledState[LED_TEMP_COLD_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_TEMP_WARM_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_TEMP_HOT_ID] = LED_MATRIX_ON;
    }
    else if (value == 0xffU)
    {
        gau8_app_displayBuffer_ledState[LED_TEMP_COLD_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_TEMP_WARM_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_TEMP_HOT_ID] = LED_MATRIX_ON;
    }
    else
    {
        // do nothing
    }
}
static void waterTempButton_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_TEMP_BUTTON_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_TEMP_BUTTON_ID] = LED_MATRIX_ON;
    }
}
static void stainLevel_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_NORMAL_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_HEAVY_ID] = LED_MATRIX_OFF;
    }
    else if (value == 1)
    {
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_LIGHT_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_NORMAL_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_HEAVY_ID] = LED_MATRIX_OFF;
    }
    else if (value == 2)
    {
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_NORMAL_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_HEAVY_ID] = LED_MATRIX_OFF;
    }
    else if (value == 3)
    {
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_NORMAL_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_HEAVY_ID] = LED_MATRIX_ON;
    }
    else if (value == 0xff)
    {
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_LIGHT_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_NORMAL_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_HEAVY_ID] = LED_MATRIX_OFF;
    }
    else
    {
        // do nothing
    }
}
static void stainLevelButton_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_BUTTON_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_STAIN_LEVEL_BUTTON_ID] = LED_MATRIX_ON;
    }
}
static void steamTech_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_STEAM_TECH_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_STEAM_TECH_ID] = LED_MATRIX_ON;
    }
}
static void steamTechButton_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_STEAM_TECH_BUTTON_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_STEAM_TECH_BUTTON_ID] = LED_MATRIX_ON;
    }
}
static void gelDetergent_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_GEL_DETERGENT_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_GEL_DETERGENT_ID] = LED_MATRIX_ON;
    }
}
static void gelDetergentButton_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_GEL_DETERGENT_BUTTON_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_GEL_DETERGENT_BUTTON_ID] = LED_MATRIX_ON;
    }
}
static void tubclean_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_TUBCLEAN_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_TUBCLEAN_ID] = LED_MATRIX_ON;
    }
}
static void tubcleanButton_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_TUBCLEAN_BUTTON_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_TUBCLEAN_BUTTON_ID] = LED_MATRIX_ON;
    }
}
static void heater_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_HEATER_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_HEATER_ID] = LED_MATRIX_ON;
    }
}
static void powderDetergent_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_POWDER_DETERGENT_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_POWDER_DETERGENT_ID] = LED_MATRIX_ON;
    }
}
static void softner_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_SOFTNER_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_SOFTNER_ID] = LED_MATRIX_ON;
    }
}
static void min_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_MIN_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_MIN_ID] = LED_MATRIX_ON;
    }
}
static void times_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_TIMES_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_TIMES_ID] = LED_MATRIX_ON;
    }
}
static void start_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_START_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_START_BUTTON_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_START_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_START_BUTTON_ID] = LED_MATRIX_ON;
    }
}
static void onOff_ledGroup_handle(uint32_t value)
{
    if (value == 0)
    {
        gau8_app_displayBuffer_ledState[LED_ON_OFF_ID] = LED_MATRIX_OFF;
        gau8_app_displayBuffer_ledState[LED_ON_OFF_BUTTON_ID] = LED_MATRIX_OFF;
    }
    else
    {
        gau8_app_displayBuffer_ledState[LED_ON_OFF_ID] = LED_MATRIX_ON;
        gau8_app_displayBuffer_ledState[LED_ON_OFF_BUTTON_ID] = LED_MATRIX_ON;
    }
}
