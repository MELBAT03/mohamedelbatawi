#include "board.h"
#include "def.h"
#include "os.h"
#include "utils.h"

#include "components/wl90at/wl90at.h"
#include "components/finput_module/finput_module.h"

static wl90at_data_t *wlBuffer;
static uint8_t bufferSize;
static uint32_t upperThreshold, lowerThreshold;
static uint32_t gu32_wl90at_value;
static uint32_t gu32_wl90at_samplesPerSec = 0;
static uint8_t gu8_wl90at_error_flag = 0;
static uint8_t gu8_wl90at_initDone_flag = 0;

static finputModule_handle gx_finput;

void wl90at_init(wl90at_config_t *wl90at)
{
    static finputModule_t wl;

    wl.timerModule = wl90at->channel.timerModule;
    wl.gpio.port = wl90at->channel.gpio.port;
    wl.gpio.pin = wl90at->channel.gpio.pin;
    wl.edge = wl90at->channel.edge;
    wl.updatePeriod = wl90at->channel.updatePeriod;

    gx_finput = finputModule_init(&wl);

    wlBuffer = wl90at->wl90atBuffer;
    bufferSize = wl90at->size;

    upperThreshold = wl90at->upperThreshold;
    lowerThreshold = wl90at->lowerThreshold;

    gu32_wl90at_samplesPerSec = wl90at->samplesPerSec;

    gu32_wl90at_value = wl90at->wl90atBuffer[0].waterLevel_freq;

    gu8_wl90at_initDone_flag = 1;
}

void wl90at_update(uint32_t period)
{
    static uint8_t index = 0, avgIndex = 0;
    uint32_t tmp;
    static uint8_t u8_abnormalSensorReadingCounter = 0;
    static uint32_t avg = 0;
    static uint32_t valueBuffer[6] = {0};
    uint32_t res = 0, count = 0, maxCount = 0;

    if (0 != gu8_wl90at_initDone_flag)
    {
        tmp = finputModule_value_get(gx_finput);
        tmp *= gu32_wl90at_samplesPerSec;

        if (utils_thresholdValidity_check(tmp, upperThreshold, lowerThreshold) == 1)
        {
            u8_abnormalSensorReadingCounter = 0;
            avg += tmp;
            if (avgIndex == 0)
            {
                avgIndex++;
            }
            else
            {
                avg /= 2;
            }

            if (index < 6)
            {
                valueBuffer[index] = avg;
                index++;
            }
            else
            {
                index = 0;

                for (uint8_t i = 0; i < 6; i++)
                {
                    for (uint8_t j = 0; j < 6; j++)
                    {
                        if (valueBuffer[i] == valueBuffer[j])
                        {
                            count++;
                        }

                        if (count > maxCount)
                        {
                            maxCount = count;
                            res = valueBuffer[i];
                        }
                    }
                }
                gu32_wl90at_value = res;
            }

            // printf("water level sensor : %d    %d\n\r", gu32_wl90at_value, avg);

            // gu32_wl90at_value = avg;
        }
        else
        {
            u8_abnormalSensorReadingCounter++;
            if (u8_abnormalSensorReadingCounter >= 10)
            {
                u8_abnormalSensorReadingCounter = 0;
                gu8_wl90at_error_flag = 1;
            }
        }
    }
}

uint8_t wl90at_error_get(void)
{
    return gu8_wl90at_error_flag;
}

uint32_t wl90at_freq_get(void)
{
    return gu32_wl90at_value;
}

uint32_t wl90at_freqForLitter_get(uint32_t litter)
{
    float tmp;

    if ((litter > (wlBuffer[bufferSize - 1].waterLevel_litter)) || (litter < (wlBuffer[0].waterLevel_litter)))
    {
        return 0;
    }

    for (uint8_t i = 0; i < bufferSize; i++)
    {
        if (litter == (wlBuffer[i].waterLevel_litter))
        {
            return wlBuffer[i].waterLevel_freq;
        }
        else if (i < bufferSize - 1 && (litter > (wlBuffer[i].waterLevel_litter) && litter < (wlBuffer[i + 1].waterLevel_litter)))
        {
            tmp = (float)((float)(wlBuffer[i].waterLevel_freq) - (wlBuffer[i + 1].waterLevel_freq));
            tmp /= (float)((float)(wlBuffer[i + 1].waterLevel_litter) - (wlBuffer[i].waterLevel_litter));
            tmp *= (litter - (wlBuffer[i].waterLevel_litter));
            tmp = (wlBuffer[i].waterLevel_freq) - tmp;
            return ((int32_t)tmp);
        }
    }
    return -1;
}

uint8_t wl90at_resetLevel_check(uint32_t acceptedDiff)
{
    uint8_t ret = 0;

    if (gu32_wl90at_value >= wlBuffer[0].waterLevel_freq - acceptedDiff)
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }

    return ret;
}

void wl90at_freq_set(uint32_t value)
{
    gu32_wl90at_value = value;
}

void wl90at_resetLevel_set(void)
{
    gu32_wl90at_value = wlBuffer[0].waterLevel_freq;
}
