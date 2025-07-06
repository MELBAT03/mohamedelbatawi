#include "buzzer.h"
#include "board.h"
#include "os.h"

#define BUZZER_DUTY_VALUE (50)

static const buzzer_tone_t *buzzerTones;
static const buzzer_tone_t *toneData;

static uint32_t u32_internalCounter = 0;
static buzzer_state_t gx_buzzer_currentState = BUZZER_INIT;
static uint8_t gu8_buzzer_currentToneLen, gu8_buzzer_currentToneRepeat;
static uint8_t gx_buzzer_currentTone;
static mcal_pwm_t channel;
mcal_pwmConfig_t freqPin;

void buzzer_init(buzzer_config_t *buzzer, const buzzer_tone_t *pBuzzerTones)
{
    gpio_t powerPin;

    freqPin.port = buzzer->freqPort;
    freqPin.pin = buzzer->freqPin;
    freqPin.duty = BUZZER_DUTY_VALUE;
    freqPin.freq = 0;
    channel = buzzer->channel;

    powerPin.ioState = buzzer->powerPin.ioState;
    powerPin.port = buzzer->powerPin.port;
    powerPin.pin = buzzer->powerPin.pin;
    powerPin.state = buzzer->powerPin.state;

    gu8_buzzer_currentToneLen = 0;
    gu8_buzzer_currentToneRepeat = 0;

    buzzer_state_set(BUZZER_INACTIVE);

    gpio_pin_init(&powerPin);
    gpio_pinState_set(powerPin.port, powerPin.pin, powerPin.state);

    buzzerTones = pBuzzerTones;

    gx_buzzer_currentState = BUZZER_IDLE;
}

void buzzer_update(uint32_t period)
{
    switch (gx_buzzer_currentState)
    {
        case BUZZER_INIT:
        {
            // do nothing
            // this is to confirm the init has been called
        }
        break;

        case BUZZER_IDLE:
        {
            // do nothing
        }
        break;

        case BUZZER_SETUP:
        {
            gu8_buzzer_currentToneLen = 0;
            freqPin.freq = toneData->data[gu8_buzzer_currentToneLen].freq;
            mcal_pwm_channel_set(channel, &freqPin);
            gx_buzzer_currentState = BUZZER_ON_INTERVAL;
            gpio_pinState_set(BUZZER_ENABLE_PORT, BUZZER_ENABLE_PIN, GPIO_HIGH);
            buzzer_state_set(BUZZER_ACTIVE);
        }
        break;

        case BUZZER_ON_INTERVAL:
        {
            u32_internalCounter += period;
            if (u32_internalCounter >= toneData->data[gu8_buzzer_currentToneLen].onInterval)
            {
                u32_internalCounter = 0;
                gx_buzzer_currentState = BUZZER_OFF_INTERVAL;
                gpio_pinState_set(BUZZER_ENABLE_PORT, BUZZER_ENABLE_PIN, GPIO_LOW);
            }
        }
        break;

        case BUZZER_OFF_INTERVAL:
        {
            u32_internalCounter += period;
            if (u32_internalCounter >= toneData->data[gu8_buzzer_currentToneLen].offInterval)
            {
                u32_internalCounter = 0;
                gu8_buzzer_currentToneLen++;

                if (gu8_buzzer_currentToneLen < buzzerTones[gx_buzzer_currentTone].len)
                {
                    freqPin.freq = toneData->data[gu8_buzzer_currentToneLen].freq;
                    mcal_pwm_channel_set(channel, &freqPin);
                    gx_buzzer_currentState = BUZZER_ON_INTERVAL;
                    gpio_pinState_set(BUZZER_ENABLE_PORT, BUZZER_ENABLE_PIN, GPIO_HIGH);
                    buzzer_state_set(BUZZER_ACTIVE);
                }
                else
                {
                    gu8_buzzer_currentToneLen = 0;
                    if (gu8_buzzer_currentToneRepeat >= toneData->repeat)
                    {
                        buzzer_state_set(BUZZER_INACTIVE);
                        gx_buzzer_currentState = BUZZER_IDLE;
                    }
                    else
                    {
                        gu8_buzzer_currentToneRepeat++;
                        gx_buzzer_currentState = BUZZER_SETUP;
                    }
                }
            }
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }

    LOG('b', 'v', "state %d  Tone %d curLen %d  Frq %d Off %d On %d Len %d Rep %d  curRep %d", gx_buzzer_currentState, gx_buzzer_currentTone, gu8_buzzer_currentToneLen, toneData->data[gu8_buzzer_currentToneLen].freq, toneData->data[gu8_buzzer_currentToneLen].offInterval, toneData->data[gu8_buzzer_currentToneLen].onInterval, toneData->len, toneData->repeat, gu8_buzzer_currentToneRepeat);
}

void buzzer_tone_set(uint8_t tone)
{
    gx_buzzer_currentTone = tone;
    gx_buzzer_currentState = BUZZER_SETUP;
    u32_internalCounter = 0;
    gu8_buzzer_currentToneRepeat = 0;
    toneData = &buzzerTones[tone];
}

void buzzer_state_set(uint8_t state)
{
    if (BUZZER_INACTIVE == state)
    {
        mcal_pwm_channel_disable(channel);
        gx_buzzer_currentState = BUZZER_IDLE;
    }
    else
    {
        mcal_pwm_channel_enable(channel);
    }
}
