#include "def.h"
#include "components/crc16/crc16.h"

#define CRC_RANGE_START_ADDR 0x00000   /* start addr */
#define CRC_RANGE_END_ADDR   0x2EFFE   // 0x1FFFE  /* end addr, this one will not be counted */
#define CRC_VALUE_ADDR       0x2EFFE   // 0x1FFFE   /* address of CRC (16bits) */

static uint8_t crcError_flag = 0;

void handler_crc_init(void)
{
    // unsigned short crc;

    // crc = crc16_ccitt((char *)CRC_RANGE_START_ADDR, CRC_RANGE_END_ADDR - CRC_RANGE_START_ADDR, CRC16_START_VAL);
    // if (crc != *((unsigned short *)CRC_VALUE_ADDR))
    // {
    //     crcError_flag = 1;
    // }
}

uint8_t handler_crc_error_get(void)
{
    return crcError_flag;
}
