#include "board.h"
#include "def.h"
#include "os.h"
#include "utils.h"

#include "handlers/input/inc/handler_water_level.h"

#include "components/finput_module/finput_module.h"
#include "components/wl90at/wl90at.h"

#define WATER_LEVEL_LITTER_MAP_LENGTH           (13U)
#define AINPUT_WATER_LEVEL_SENSOR_UPDATE_PERIOD (500U)

static uint8_t gu8_waterLevel_calibrationFlag;
static uint32_t samplesPerSec = 0;
static wl90at_data_t gx_app_waterLevel_litter2Freq_map[] = {
    {0U, HANDLER_WATER_LEVEL_RESET_VALUE},
    {10U, 26100U},
    {20U, 25550U},
    {30U, 25000U},
    {40U, 24550U},
    {50U, 24180U},
    {60U, 23750U},
    {70U, 23320U},
    {80U, 22970U},
    {90U, 22600U},
    {100U, 22300U},
    {110U, 22050U},
    {120U, 21750U}};

static uint32_t gu32_app_waterLevel_upperDrainFilterFreqRange = 1000;
static uint32_t gu32_app_waterLevel_lowerDrainFilterFreqRange = 2300;

void handler_waterLevel_init(void)
{
    wl90at_config_t wlsensor;

    wlsensor.channel.timerModule = WATER_LEVEL_CHANNEL;
    wlsensor.channel.gpio.port = WATER_LEVEL_PORT;
    wlsensor.channel.gpio.pin = WATER_LEVEL_PIN;
    wlsensor.channel.edge = WATER_LEVEL_DETECTION_EDGE;
    wlsensor.channel.updatePeriod = AINPUT_WATER_LEVEL_SENSOR_UPDATE_PERIOD;

    wlsensor.wl90atBuffer = gx_app_waterLevel_litter2Freq_map;
    wlsensor.size = WATER_LEVEL_LITTER_MAP_LENGTH;

    wlsensor.upperThreshold = HANDLER_WATER_LEVEL_DEFAULT_UPPER_THRESHOLD;
    wlsensor.lowerThreshold = HANDLER_WATER_LEVEL_DEFAULT_LOWER_THRESHOLD;
    samplesPerSec = 1000 / AINPUT_WATER_LEVEL_SENSOR_UPDATE_PERIOD;
    wlsensor.samplesPerSec = samplesPerSec;

    wl90at_init(&wlsensor);
}

uint32_t handler_waterLevel_resetLevel_get(void)
{
    return gx_app_waterLevel_litter2Freq_map[0].waterLevel_freq;
}

void handler_waterLevel_resetLevel_set(uint32_t value)
{
    gx_app_waterLevel_litter2Freq_map[0].waterLevel_freq = value;
}

void handler_waterLevel_calibrationFlag_set(void)
{
    gu8_waterLevel_calibrationFlag = 1;
}

uint8_t handler_waterLevel_calibrationFlag_get(void)
{
    return gu8_waterLevel_calibrationFlag;
}

uint8_t handler_waterLevel_error_get(void)
{
    return wl90at_error_get();
}

uint32_t handler_waterLevel_samplesPerSec_get(void)
{
    return samplesPerSec;
}

uint32_t handler_waterLevel_currentFreq_get(void)
{
    return wl90at_freq_get();
}

uint32_t handler_waterLevel_freqValue_get(uint32_t litter)
{
    return wl90at_freqForLitter_get(litter);
}

uint8_t handler_waterLevel_resetLevel_check(handler_waterLevel_resetCheckType_t type)
{
    uint32_t acceptedDiff = 0;

    if (type == WATER_LEVEL_RESET_NORMAL_CHECK)
    {
        acceptedDiff = HANDLER_WATER_LEVEL_ZERO_LEVEL_ACCEPTED_DIFF;
    }
    else if (type == WATER_LEVEL_RESET_ERROR_CHECK)
    {
        acceptedDiff = HANDLER_WATER_LEVEL_ZERO_LEVEL_ERROR_ACCEPTED_DIFF;
    }
    else if (type == WATER_LEVEL_RESET_SPIN_ISSUE_CHECK)
    {
        acceptedDiff = HANDLER_WATER_LEVEL_ZERO_LEVEL_SPIN_ERROR_ACCEPTED_DIFF;
    }
    else if (type == WATER_LEVEL_RESET_TEST_MODE_CHECK)
    {
        acceptedDiff = HANDLER_WATER_LEVEL_ZERO_LEVEL_TEST_MODE_ERROR_ACCEPTED_DIFF;
    }
    else if (type == WATER_LEVEL_RESET_CALIBRATION_CHECK)
    {
        acceptedDiff = HANDLER_WATER_LEVEL_ZERO_CALIBRATION_ACCEPTED_DIFF;
    }
    else if (type == WATER_LEVEL_LEAKAGE_CHECK)
    {
        acceptedDiff = HANDLER_WATER_LEVEL_ZERO_LEVEL_LEAKAGE_ERROR_ACCEPTED_DIFF;
    }

    return wl90at_resetLevel_check(acceptedDiff);
}

uint8_t handler_waterLevel_compare(uint32_t current, uint32_t userSetting, uint32_t diff)
{
    if (current <= userSetting + diff)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void handler_waterLevel_currentLevelFreq_set(uint32_t value)
{
    wl90at_freq_set(value);
}

void handler_waterLevel_resetLevel_mark(void)
{
    wl90at_resetLevel_set();
}

uint32_t handler_waterLevel_upperDrainFilterThreshold_get(void)
{
    return gu32_app_waterLevel_upperDrainFilterFreqRange;
}

uint32_t handler_waterLevel_lowerDrainFilterThreshold_get(void)
{
    return gu32_app_waterLevel_lowerDrainFilterFreqRange;
}

void handler_waterLevel_currentFreq_set(uint32_t value)
{
}
