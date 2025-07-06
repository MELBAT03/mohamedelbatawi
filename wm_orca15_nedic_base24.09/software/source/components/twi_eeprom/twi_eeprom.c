#include "components/twi_eeprom/twi_eeprom.h"
#include "component_config.h"
#include "def.h"
#include "components/stwi/stwi.h"
#include "os.h"

static int8_t twieeprom_slave_ack_check(uint32_t deviceAddr);

void twieeprom_init(hwTimer_t *tm, twi_pinConfig_t *pins)
{
#ifdef STWI_ENABLE
    twi_pinConfig_t pinConfig;

    pinConfig.dataPort = pins->dataPort;
    pinConfig.dataPin = pins->dataPin;
    pinConfig.clockPort = pins->clockPort;
    pinConfig.clockPin = pins->clockPin;

    twi_init(tm, &pinConfig, TWI_EEPROM_SPEED);
#else
    twi_init();
#endif
}

void twieeprom_data_set(uint32_t deviceAddr, uint8_t *pBuffer, uint16_t writeAddr, uint16_t numByteToWrite)
{
    int8_t ret = 0;

    if (numByteToWrite > TWI_EEPROM_PAGE_SIZE)
    {
        return;
    }

    // :NOTE: this loops until the slave sends ack
    ret = twieeprom_slave_ack_check(deviceAddr);

    if (ret != -1)
    {
        if ((uint16_t)writeAddr > 0xff)
        {
            while (twi_status() == 1)
                ;
            twi_write((writeAddr & 0xff00) >> 8);
            twi_write(writeAddr & 0x00ff);
        }
        else
        {
            while (twi_status() == 1)
                ;
            twi_write(writeAddr);
        }

        while (numByteToWrite--)
        {
            while (twi_status() == 1)
                ;
            twi_write(*pBuffer);
            pBuffer++;
        }

        while (twi_status() == 1)
            ;
        twi_stop();
    }
}

int8_t twieeprom_data_get(uint32_t deviceAddr, uint8_t *pBuffer, uint16_t readAddr, uint16_t numByteToRead)
{
    int8_t ret = 0;

    ret = twieeprom_slave_ack_check(deviceAddr);

    if (ret != -1)
    {
        if ((uint16_t)readAddr > 0xff)
        {
            while (twi_status() == 1)
                ;
            twi_write((readAddr & 0xff00) >> 8);
            twi_write(readAddr & 0x00ff);
        }
        else
        {
            while (twi_status() == 1)
                ;
            twi_write(readAddr);
        }

        twi_start();
        twi_write(TWI_READ_SET(deviceAddr));

        while (twi_status() == 1)
            ;

        while (numByteToRead)
        {
            while (twi_status() == 1)
                ;

            if (numByteToRead > 1)
            {
                *pBuffer = twi_read(TWI_ACK_SET);
            }
            else
            {
                *pBuffer = twi_read(TWI_ACK_CLR);
            }

            pBuffer++;
            numByteToRead--;
        }

        twi_stop();
    }
    else
    {
        while (numByteToRead)
        {
            if (numByteToRead > 1)
            {
                *pBuffer = 0;
            }
            else
            {
                *pBuffer = 0;
            }

            pBuffer++;
            numByteToRead--;
        }
    }

    while (numByteToRead)
    {
        if (numByteToRead > 1)
        {
            *pBuffer = 0;
        }
        else
        {
            *pBuffer = 0;
        }

        pBuffer++;
        numByteToRead--;
    }

    return ret;
}

static int8_t twieeprom_slave_ack_check(uint32_t deviceAddr)
{
    uint32_t ack;
    static uint8_t slaveAckCnt = 0;

    while (slaveAckCnt < 100)
    {
        twi_start();
        ack = twi_write(TWI_WRITE_SET(deviceAddr));

        while (1)
        {
            if (ack == 1)
            {
                slaveAckCnt++;
                break;
            }
            else
            {
                slaveAckCnt = 0;
                return 0;
            }
        }
    }
    return -1;
}
