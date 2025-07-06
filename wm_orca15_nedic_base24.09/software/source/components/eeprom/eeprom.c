#include "components/eeprom/eeprom.h"
#include "components/twi_eeprom/twi_eeprom.h"
#include "def.h"
#include "components/stwi/stwi.h"
#include "component_config.h"

typedef struct
{
    uint16_t id0;
    uint16_t idx;
    uint8_t devAddr;
    uint8_t addrBuffer[EEPROM_BUFFER_SIZE];
    uint8_t dataBuffer[EEPROM_BUFFER_SIZE];
} eeprom_handler_t;

static eeprom_handler_t gx_eeprom_internalHandler;

void eeprom_init(hwTimer_t *tm, twi_pinConfig_t *pins)
{
#ifdef EXTERNAL_EEPROM_ENABLE
    twieeprom_init(tm, pins);
#endif

    for (uint8_t i = 0; i < EEPROM_BUFFER_SIZE; i++)
    {
        gx_eeprom_internalHandler.addrBuffer[i] = 0;
        gx_eeprom_internalHandler.dataBuffer[i] = 0;
    }
    gx_eeprom_internalHandler.id0 = 0;
    gx_eeprom_internalHandler.idx = 0;
}

void eeprom_data_set(uint16_t devAddr, uint8_t *pBuffer, uint16_t startAddr, uint16_t numByte)
{
    if (gx_eeprom_internalHandler.idx + numByte < EEPROM_BUFFER_SIZE)
    {
        gx_eeprom_internalHandler.devAddr = devAddr;
        for (uint16_t i = 0; i < numByte; i++)
        {
            gx_eeprom_internalHandler.addrBuffer[gx_eeprom_internalHandler.idx + i] = startAddr + i;
            gx_eeprom_internalHandler.dataBuffer[gx_eeprom_internalHandler.idx + i] = pBuffer[i];
        }
        gx_eeprom_internalHandler.idx += numByte;
    }
}

int8_t eeprom_data_get(uint16_t devAddr, uint8_t *pBuffer, uint16_t startAddr, uint16_t numByte)
{
    return twieeprom_data_get(devAddr, pBuffer, startAddr, numByte);
}

void eeprom_update(uint32_t period)
{
    uint32_t byte2Wr = 1;

    if (gx_eeprom_internalHandler.idx > gx_eeprom_internalHandler.id0)
    {
        // :NOTE: right now, it writes data byte by byte
        // :TODO: write page by page, but requires handling the sequential data in the buffer (how to differentiate between sequential data and not seq data in the buffer)
        twieeprom_data_set((uint32_t)gx_eeprom_internalHandler.devAddr, &gx_eeprom_internalHandler.dataBuffer[gx_eeprom_internalHandler.id0], gx_eeprom_internalHandler.addrBuffer[gx_eeprom_internalHandler.id0], byte2Wr);

        gx_eeprom_internalHandler.id0 += byte2Wr;
    }
    else
    {
        gx_eeprom_internalHandler.id0 = 0;
        gx_eeprom_internalHandler.idx = 0;
    }

    if (gx_eeprom_internalHandler.id0 == gx_eeprom_internalHandler.idx)
    {
        gx_eeprom_internalHandler.id0 = 0;
        gx_eeprom_internalHandler.idx = 0;
    }
}
