#include "mcal.h"
#include "def.h"
#include "component_config.h"
#include "components/dmem/dmem.h"

#define DMEM_ACTUAL_RECORD_NUM (DMEM_RECORD_NUM + 1)

#define DMEM_RECORD_INDEX_ADDR (0)

#define DATAFLASH_BUFFER_SIZE (MEMORY_SIZE)
#define RECORD_FIRST_LOC      0
#define RECORD_LAST_LOC       (8)
#define INTEPROM_BLOCK_NUM    0
#define INTEPROM_ONE_BYTE     1

static uint8_t dmem_recordData[DMEM_ACTUAL_RECORD_NUM];
static uint16_t dmem_recordIndex = RECORD_LAST_LOC - 1;
volatile uint8_t flashWrite_status = 0;
static uint8_t dmem_reset_flag = 0;

void dmem_init(void)
{
    uint16_t searchIndex = 0, searchBuff[RECORD_LAST_LOC] = 0;
    uint8_t index = 0;
    mcal_dataFlash_t dmemConfig = {INTEPROM_BLOCK_NUM, 0, INTEPROM_ONE_BYTE, NULL};

    for (searchIndex = 0; searchIndex < RECORD_LAST_LOC; searchIndex++)
    {
        dmemConfig.dataFlash_blockAddress = searchIndex;
        searchBuff[searchIndex] = mcal_flashData_read(&dmemConfig);

        if (searchBuff[searchIndex] == 0xff)
        {
            if (searchIndex == 0)
            {
                dmem_recordIndex = 0;
            }
            else
            {
                dmem_recordIndex = searchBuff[searchIndex - 1];
            }

            break;
        }
    }

    if (dmem_recordIndex > RECORD_LAST_LOC)
    {
        mcal_flashData_erase(&dmemConfig);
        dmem_recordIndex = 0;
    }
    else
    {
        if (searchIndex != 0)
        {
            for (index = 0; index < DMEM_ACTUAL_RECORD_NUM; index++)
            {
                dmemConfig.dataFlash_blockAddress = dmem_recordIndex + (RECORD_LAST_LOC * index);
                dmem_recordData[index] = mcal_flashData_read(&dmemConfig);
            }

            if (searchIndex == RECORD_LAST_LOC)
            {
                // now all locations have been filled tst
                mcal_flashData_erase(&dmemConfig);
                dmem_recordIndex = 0;
            }
            else
            {
                dmem_recordIndex++;
            }
        }
        else
        {
            for (index = 0; index < DMEM_ACTUAL_RECORD_NUM; index++)
            {
                dmem_recordData[index] = 0;
            }

            dmem_recordIndex = 0;
        }
    }
}

uint8_t dmem_data_get(uint16_t recordNum)
{
    return dmem_recordData[recordNum + 1];
}

uint8_t dmem_data_set(uint16_t recordNum, uint8_t data)
{
    static uint8_t memIndexWrite_flag = 0;
    mcal_dataFlash_t dataflash_config = {INTEPROM_BLOCK_NUM, 0, INTEPROM_ONE_BYTE, NULL};

    if (memIndexWrite_flag == 0)
    {
        memIndexWrite_flag = 1;
        dataflash_config.dataFlash_blockAddress = dmem_recordIndex;
        dataflash_config.dataFlash_dataBuffer = &dmem_recordIndex;
        flashWrite_status = mcal_flashData_write(&dataflash_config);
    }

    dmem_recordData[recordNum + 1] = data;

    dataflash_config.dataFlash_blockAddress = dmem_recordIndex + (RECORD_LAST_LOC * (recordNum + 1));
    dataflash_config.dataFlash_dataBuffer = &data;

    flashWrite_status = mcal_flashData_write(&dataflash_config);

    return flashWrite_status;
}

uint8_t dmem_resetFlag_get(void)
{
    return dmem_reset_flag;
}

uint8_t dmem_reset(void)
{
    mcal_dataFlash_t dmemConfig = {INTEPROM_BLOCK_NUM, 0, INTEPROM_ONE_BYTE, NULL};

    for (uint16_t index = 0; index < DMEM_ACTUAL_RECORD_NUM; index++)
    {
        dmem_recordData[index] = 0xff;
    }

    dmem_recordIndex = 0xff;

    dmem_reset_flag = 1;

    return mcal_flashData_erase(&dmemConfig);
}
