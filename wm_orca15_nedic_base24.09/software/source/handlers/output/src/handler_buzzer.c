#include "handlers/output/inc/handler_buzzer.h"
#include "components/buzzer/buzzer.h"
#include "components/foutput_module/foutput_module.h"
#include "board.h"

static const buzzer_toneData_t buzzerValidKeyPress[] =
    {
        {2803, 40, 120}};

static const buzzer_toneData_t buzzerInvalidKeyPress[] =
    {
        // {2935, 63, 28},
        {2935, 65, 30},
        // {2933, 69, 30},
        {2933, 70, 30},
        // {2935, 70, 1539}};
        {2935, 70, 250}};

static const buzzer_toneData_t buzzerShutdown[] =
    {
        {2817, 33, 292}};

static const buzzer_toneData_t buzzerStartup[] =
    {
        {1956, 40, 300},
        // {2072, 400, 1},
        {2072, 350, 5},
        // {2322, 201, 1},
        {2322, 150, 5},
        // {3100, 598, 1200}};
        {3100, 400, 400}};

static const buzzer_toneData_t buzzerChildLockActive[] =
    {
        {2790, 35, 60},
        {2800, 40, 60},
};

static const buzzer_toneData_t buzzerChildLockInActive[] =
    {
        {2790, 35, 60},
        {2800, 40, 60}};

static const buzzer_toneData_t buzzerProgramStart[] =
    // {
    //     {1240, 395, 1},
    //     {1470, 200, 1},
    //     {1650, 200, 1},
    //     {1860, 200, 1},
    //     {2200, 200, 1},
    //     {2190, 200, 1},
    //     {3100, 600, 1200}};
    {
        {1240, 380, 5},
        {1470, 190, 5},
        {1650, 190, 5},
        {1860, 190, 5},
        {2200, 190, 5},
        {2190, 190, 5},
        {3100, 550, 700}};

static const buzzer_toneData_t buzzerProgramPause[] =
    {
        // {2803, 38, 148}};
        {2803, 40, 150}};

static const buzzer_toneData_t buzzerProgramFinish[] =
    {
        // {4119, 330, 1},
        // {3895, 200, 1},
        // {3463, 200, 1},
        // {3096, 200, 1},
        // {2610, 200, 1},
        // {2320, 600, 6600}};
        {4119, 300, 5},
        {3895, 180, 5},
        {3463, 180, 5},
        {3096, 180, 5},
        {2610, 180, 5},
        {2320, 500, 6000}};

static const buzzer_toneData_t buzzerError[] =
    {
        {2000, 80, 220},
        {1667, 80, 1220}};

static const buzzer_tone_t buzzerTones[] =
    {
        {buzzerStartup, 4, 0},
        {buzzerShutdown, 1, 0},
        {buzzerValidKeyPress, 1, 0},
        {buzzerInvalidKeyPress, 3, 0},
        {buzzerChildLockActive, 2, 0},
        {buzzerChildLockInActive, 2, 0},
        {buzzerProgramStart, 7, 0},
        {buzzerProgramPause, 1, 0},
        {buzzerProgramFinish, 6, 0},
        {buzzerError, 2, 4},
        {buzzerProgramPause, 1, 3}};

void handler_buzzer_init(void)
{
    buzzer_config_t buzzer;

    buzzer.powerPin.ioState = GPIO_OUTPUT;
    buzzer.powerPin.port = BUZZER_ENABLE_PORT;
    buzzer.powerPin.pin = BUZZER_ENABLE_PIN;
    buzzer.powerPin.state = GPIO_LOW;

    buzzer.freqPort = BUZZER_FREQ_PORT;
    buzzer.freqPin = BUZZER_FREQ_PIN;
    buzzer.channel = BUZZER_CHANNEL;

    buzzer_init(&buzzer, buzzerTones);
}

void handler_buzzer_tone_set(handler_buzzer_toneSelection_t tone)
{
    buzzer_tone_set((uint8_t)tone);
}

void handler_buzzer_state_set(handler_buzzer_state_t state)
{
    buzzer_state_set((uint8_t)state);
}
