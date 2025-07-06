#include "component_config.h"
#include "gpio/gpio.h"
#include "keypad/keypad.h"
#include "keypad/keypad_def.h"
#include "os.h"

static uint8_t keypad_register_check(uint8_t id);

typedef struct keypad_internalKeypadHandler
{
    uint32_t activeStateCnt;
    keypad_stateEnum_t activeState;
    keypad_stateEnum_t edgeActiveState;
    keypad_stateEnum_t onReleaseState;
} keypad_internalKeypadHandler_t;

static keypad_t gx_keypad_handler;
static keypad_internalKeypadHandler_t gx_keypad_internalHandler[KEYPAD_ROW_NUMBER][KEYPAD_COL_NUMBER];
static uint32_t gu32_keypad_pressedKeyCount = 0;
static uint32_t gu32_keypad_edgePressedKeyCount = 0;

void keypad_init(keypad_t *px_keypadHandler)
{
    uint8_t i = 0, j = 0;

    gx_keypad_handler.type = px_keypadHandler->type;
    gx_keypad_handler.activeLevel = px_keypadHandler->activeLevel;

    if (gx_keypad_handler.type == KEYPAD_GPIO_INPUT)
    {
        for (i = 0; i < KEYPAD_COL_NUMBER; i++)
        {
            gpio_pinIOState_set(px_keypadHandler->colPort[i], px_keypadHandler->colPin[i], GPIO_OUTPUT);
            gx_keypad_handler.colPort[i] = px_keypadHandler->colPort[i];
            gx_keypad_handler.colPin[i] = px_keypadHandler->colPin[i];
        }

        for (i = 0; i < KEYPAD_ROW_NUMBER; i++)
        {
            gpio_pinIOState_set(px_keypadHandler->rowPort[i], px_keypadHandler->rowPin[i], GPIO_INPUT);
            gx_keypad_handler.rowPort[i] = px_keypadHandler->rowPort[i];
            gx_keypad_handler.rowPin[i] = px_keypadHandler->rowPin[i];
        }
    }
    else
    {
        gx_keypad_handler.reg = px_keypadHandler->reg;
    }

    for (i = 0; i < KEYPAD_ROW_NUMBER; i++)
    {
        for (j = 0; j < KEYPAD_COL_NUMBER; j++)
        {
            gx_keypad_internalHandler[i][j].activeStateCnt = 0;
            gx_keypad_internalHandler[i][j].activeState = KEYPAD_KEY_RELEASED;
            gx_keypad_internalHandler[i][j].edgeActiveState = KEYPAD_KEY_RELEASED;
            gx_keypad_internalHandler[i][j].onReleaseState = KEYPAD_KEY_RELEASED;
            gx_keypad_handler.shortPressMultiplier = px_keypadHandler->shortPressMultiplier;
            if (px_keypadHandler->shortPressMultiplier <= 0)
            {
                px_keypadHandler->shortPressMultiplier = 1;
            }
            if (px_keypadHandler->longPressMultiplier < px_keypadHandler->shortPressMultiplier)
            {
                gx_keypad_handler.longPressMultiplier = px_keypadHandler->shortPressMultiplier;
            }
            else
            {
                gx_keypad_handler.longPressMultiplier = px_keypadHandler->longPressMultiplier;
            }
        }
    }
}

void keypad_update(uint32_t period)
{
    static uint8_t col = 0;
    uint8_t i = 0;

    if (gx_keypad_handler.type != KEYPAD_INPUT_NONE)
    {
        if (gx_keypad_handler.type == KEYPAD_GPIO_INPUT)
        {
            for (i = 0; i < KEYPAD_COL_NUMBER; i++)
            {
                if (col == i)
                {
                    gpio_pinState_set(gx_keypad_handler.colPort[i], gx_keypad_handler.colPin[i], gx_keypad_handler.activeLevel);
                }
                else
                {
                    gpio_pinState_set(gx_keypad_handler.colPort[i], gx_keypad_handler.colPin[i], !gx_keypad_handler.activeLevel);
                }
            }
        }

        keypad_scan(col);

        col++;
        if (col >= KEYPAD_COL_NUMBER)
        {
            col = 0;
        }
    }
}

uint32_t keypad_pressedKeyCount_get(void)
{
    return gu32_keypad_pressedKeyCount;
}

uint32_t keypad_edgePressedKeyCount_get(void)
{
    return gu32_keypad_edgePressedKeyCount;
}

void keypad_scan(uint8_t col)
{
    uint8_t i = 0;

    for (i = 0; i < KEYPAD_ROW_NUMBER; i++)
    {
        switch (gx_keypad_internalHandler[i][col].activeState)
        {
            case KEYPAD_KEY_RELEASED:
                if (keypad_register_check(i) == gx_keypad_handler.activeLevel)
                {
                    gx_keypad_internalHandler[i][col].activeStateCnt++;
                }
                else
                {
                    gx_keypad_internalHandler[i][col].activeStateCnt = 0;
                }

                if (gx_keypad_internalHandler[i][col].activeStateCnt >= gx_keypad_handler.shortPressMultiplier)
                {
                    gx_keypad_internalHandler[i][col].activeState = KEYPAD_KEY_PRESSED_SHORT_PRESS;
                    gx_keypad_internalHandler[i][col].edgeActiveState = KEYPAD_KEY_PRESSED_SHORT_PRESS;
                    gu32_keypad_edgePressedKeyCount++;
                    gu32_keypad_pressedKeyCount++;
                }
                break;

            case KEYPAD_KEY_PRESSED_SHORT_PRESS:
                if (keypad_register_check(i) == gx_keypad_handler.activeLevel)
                {
                    gx_keypad_internalHandler[i][col].activeStateCnt++;
                }
                else
                {
                    gx_keypad_internalHandler[i][col].activeState = KEYPAD_KEY_RELEASED;
                    gu32_keypad_pressedKeyCount--;
                    gx_keypad_internalHandler[i][col].activeStateCnt = 0;
                    gx_keypad_internalHandler[i][col].onReleaseState = KEYPAD_KEY_PRESSED_SHORT_PRESS;
                    // gx_keypad_internalHandler[i][col].edgeActiveState = KEYPAD_KEY_PRESSED_SHORT_PRESS;
                }

                if (gx_keypad_internalHandler[i][col].activeStateCnt >= gx_keypad_handler.longPressMultiplier)
                {
                    gx_keypad_internalHandler[i][col].activeState = KEYPAD_KEY_PRESSED_LONG_PRESS;
                    gx_keypad_internalHandler[i][col].edgeActiveState = KEYPAD_KEY_PRESSED_LONG_PRESS;
                    gx_keypad_internalHandler[i][col].activeStateCnt = 0;
                }
                break;

            case KEYPAD_KEY_PRESSED_LONG_PRESS:
                if (keypad_register_check(i) == gx_keypad_handler.activeLevel)
                {
                    gx_keypad_internalHandler[i][col].activeState = KEYPAD_KEY_PRESSED_LONG_PRESS;
                }
                else
                {
                    gx_keypad_internalHandler[i][col].activeState = KEYPAD_KEY_RELEASED;
                    gu32_keypad_pressedKeyCount--;
                    gx_keypad_internalHandler[i][col].activeStateCnt = 0;
                    gx_keypad_internalHandler[i][col].onReleaseState = KEYPAD_KEY_PRESSED_LONG_PRESS;
                }
                break;

            default:
                // do nothing
                break;
        }
    }
}

keypad_stateEnum_t keypad_state_get(keypad_keyEnum_t key)
{
    uint8_t keyRow = 0;
    uint8_t keyCol = 0;
    keypad_stateEnum_t keyState;

    if (gx_keypad_handler.type == KEYPAD_GPIO_INPUT)
    {
        keyCol = key % 10;
        keyRow = key / 10;
    }
    else
    {
        keyCol = KEYPAD_COL_NUMBER - 1;
        keyRow = key;
    }

    if (keyCol >= KEYPAD_COL_NUMBER || keyRow >= KEYPAD_ROW_NUMBER)
    {
        return KEYPAD_KEY_FAULT;
    }

    keyState = gx_keypad_internalHandler[keyRow][keyCol].activeState;

    return keyState;
}
void keypad_state_set(keypad_keyEnum_t key, keypad_stateEnum_t value)
{
    uint8_t keyRow = 0;
    uint8_t keyCol = 0;

    if (gx_keypad_handler.type == KEYPAD_GPIO_INPUT)
    {
        keyCol = key % 10;
        keyRow = key / 10;
    }
    else
    {
        keyCol = KEYPAD_COL_NUMBER - 1;
        keyRow = key;
    }

    gx_keypad_internalHandler[keyRow][keyCol].activeState = value;
}

keypad_stateEnum_t keypad_edgeState_get(keypad_keyEnum_t key)
{
    uint8_t keyRow = 0;
    uint8_t keyCol = 0;
    keypad_stateEnum_t keyState;

    if (gx_keypad_handler.type == KEYPAD_GPIO_INPUT)
    {
        keyCol = key % 10;
        keyRow = key / 10;
    }
    else
    {
        keyCol = KEYPAD_COL_NUMBER - 1;
        keyRow = key;
    }

    if (keyCol >= KEYPAD_COL_NUMBER || keyRow >= KEYPAD_ROW_NUMBER)
    {
        return KEYPAD_KEY_FAULT;
    }

    keyState = gx_keypad_internalHandler[keyRow][keyCol].edgeActiveState;
    if (keyState == KEYPAD_KEY_PRESSED_SHORT_PRESS && gu32_keypad_edgePressedKeyCount > 0)
    {
        gu32_keypad_edgePressedKeyCount--;
    }
    gx_keypad_internalHandler[keyRow][keyCol].edgeActiveState = KEYPAD_KEY_RELEASED;

    return keyState;
}

void keypad_edgeState_set(keypad_keyEnum_t key, keypad_stateEnum_t value)
{
    uint8_t keyRow = 0;
    uint8_t keyCol = 0;

    if (gx_keypad_handler.type == KEYPAD_GPIO_INPUT)
    {
        keyCol = key % 10;
        keyRow = key / 10;
    }
    else
    {
        keyCol = KEYPAD_COL_NUMBER - 1;
        keyRow = key;
    }

    gx_keypad_internalHandler[keyRow][keyCol].edgeActiveState = value;
}

keypad_stateEnum_t keypad_onReleaseState_get(keypad_keyEnum_t key)
{
    uint8_t keyRow = 0;
    uint8_t keyCol = 0;
    keypad_stateEnum_t keyState;

    if (gx_keypad_handler.type == KEYPAD_GPIO_INPUT)
    {
        keyCol = key % 10;
        keyRow = key / 10;
    }
    else
    {
        keyCol = KEYPAD_COL_NUMBER - 1;
        keyRow = key;
    }

    if (keyCol >= KEYPAD_COL_NUMBER || keyRow >= KEYPAD_ROW_NUMBER)
    {
        return KEYPAD_KEY_FAULT;
    }

    keyState = gx_keypad_internalHandler[keyRow][keyCol].onReleaseState;
    // gx_keypad_internalHandler[keyRow][keyCol].edgeActiveState = KEYPAD_KEY_RELEASED;
    gx_keypad_internalHandler[keyRow][keyCol].onReleaseState = KEYPAD_KEY_RELEASED;

    return keyState;
}

void keypad_keyMirror_get(uint8_t (*arr)[4])
{
    uint8_t i = 0, j = 0;

    for (i = 0; i < KEYPAD_ROW_NUMBER; i++)
    {
        for (j = 0; j < KEYPAD_COL_NUMBER; j++)
        {
            arr[i][j] = gx_keypad_internalHandler[i][j].activeState;
        }
    }
}

void keypad_keyEdgeMirror_get(uint8_t (*arr)[4])
{
    uint8_t i = 0, j = 0;

    for (i = 0; i < KEYPAD_ROW_NUMBER; i++)
    {
        for (j = 0; j < KEYPAD_COL_NUMBER; j++)
        {
            arr[i][j] = gx_keypad_internalHandler[i][j].edgeActiveState;
        }
    }
}

void keypad_keyEdge_clear(void)
{
    uint8_t i = 0, j = 0;

    for (i = 0; i < KEYPAD_ROW_NUMBER; i++)
    {
        for (j = 0; j < KEYPAD_COL_NUMBER; j++)
        {
            gx_keypad_internalHandler[i][j].edgeActiveState = (keypad_stateEnum_t)0;
        }
    }
}

static uint8_t keypad_register_check(uint8_t id)
{
    uint8_t keypadret = !gx_keypad_handler.activeLevel;
    uint32_t buff = 0;

    if (gx_keypad_handler.type == KEYPAD_GPIO_INPUT)
    {
        keypadret = gpio_pinState_get(gx_keypad_handler.rowPort[id], gx_keypad_handler.rowPin[id]);
    }
    else if (gx_keypad_handler.type == KEYPAD_REG_INPUT)
    {
        buff = *(gx_keypad_handler.reg);
        buff &= (1 << id);
        keypadret = (buff >> id);
    }

    return keypadret;
}
