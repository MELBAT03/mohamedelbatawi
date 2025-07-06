#ifndef COMPONENT_BUZZER_H
#define COMPONENT_BUZZER_H 1

#include "mcal.h"
#include "def.h"
#include "gpio/gpio.h"

#define BUZZER_ACTIVE   (1)
#define BUZZER_INACTIVE (0)

typedef enum
{
    BUZZER_INIT,
    BUZZER_IDLE,
    BUZZER_SETUP,
    BUZZER_ON_INTERVAL,
    BUZZER_OFF_INTERVAL
} buzzer_state_t;

typedef struct
{
    uint32_t freq;
    uint32_t onInterval;
    uint32_t offInterval;
} buzzer_toneData_t;

typedef struct
{
    const buzzer_toneData_t *data;
    uint8_t len;
    uint8_t repeat;
} buzzer_tone_t;

typedef struct
{
    gpio_t powerPin;
    gpio_port_t freqPort;
    gpio_pin_t freqPin;
    mcal_pwm_t channel;
} buzzer_config_t;

void buzzer_init(buzzer_config_t *buzzer, const buzzer_tone_t *pBuzzerTones);

void buzzer_update(uint32_t period);

void buzzer_tone_set(uint8_t tone);
void buzzer_freq_set(uint32_t freq, uint32_t period);
void buzzer_state_set(uint8_t state);

#endif
