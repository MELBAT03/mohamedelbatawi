#include "stwi.h"
#include "util.h"
#include "drv10983.h"

#define DRV10983_TWI_ADDR              0xA4
#define DRV10983_VCC_REGISTER_VALUE(x) ((((uint16_t)x) * 256) / 30)
#define DRV10983_VCC_CHECK_VALUE       (0xBB)   // corresponds to 22v

#define DRV10983_SPEED_REG_LOW_ADDR     0X00
#define DRV10983_SPEED_REG_HIGH_ADDR    0X01
#define DRV10983_EECTRL_REG_ADDR        0x03
#define DRV10983_SUPPLYVOLTAGE_REG_ADDR 0x1A
#define DRV10983_DEVCTRL_REG_ADDR       0x02

#define DRV10983_EECTRL_SLEEP_BIT_SET     (1 << 7)
#define DRV10983_EECTRL_SIDATA_BIT_SET    (1 << 6)
#define DRV10983_EECTRL_EEWRITE_BIT_SET   (1 << 4)
#define DRV10983_EECTRL_EEREFRESH_BIT_SET (1 << 5)
#define DRV10983_PROG_KEY_ENABLE_VALUE    0xB6

#define DRV10983_SLEEP_DISABLE() (DRV10983_EECTRL_SLEEP_BIT_SET)

#define DRV10983_MOTOR_PARAMETER_REG_NUM (12)
#define DRV10983_MOTOR_READINGS_REG_NUM  (12)

#define DRV10983_MOTOR_PARAMETER_BASE_REG_ADDR    (0x20)
#define DRV10983_MOTOR_PARAMETER_REG_MP1_ADDR     (DRV10983_MOTOR_PARAMETER_BASE_REG_ADDR)
#define DRV10983_MOTOR_PARAMETER_REG_MP2_ADDR     (DRV10983_MOTOR_PARAMETER_BASE_REG_ADDR + 1)
#define DRV10983_MOTOR_PARAMETER_REG_MP3_ADDR     (DRV10983_MOTOR_PARAMETER_BASE_REG_ADDR + 2)
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT1_ADDR (DRV10983_MOTOR_PARAMETER_BASE_REG_ADDR + 3)
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT2_ADDR (DRV10983_MOTOR_PARAMETER_BASE_REG_ADDR + 4)
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT3_ADDR (DRV10983_MOTOR_PARAMETER_BASE_REG_ADDR + 5)
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT4_ADDR (DRV10983_MOTOR_PARAMETER_BASE_REG_ADDR + 6)
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT5_ADDR (DRV10983_MOTOR_PARAMETER_BASE_REG_ADDR + 7)
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT6_ADDR (DRV10983_MOTOR_PARAMETER_BASE_REG_ADDR + 8)
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT7_ADDR (DRV10983_MOTOR_PARAMETER_BASE_REG_ADDR + 9)
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT8_ADDR (DRV10983_MOTOR_PARAMETER_BASE_REG_ADDR + 10)
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT9_ADDR (DRV10983_MOTOR_PARAMETER_BASE_REG_ADDR + 11)

#define DRV10983_MOTOR_PARAMETER_REG_MP1_VALUE     0x4f
#define DRV10983_MOTOR_PARAMETER_REG_MP2_VALUE     0x69
#define DRV10983_MOTOR_PARAMETER_REG_MP3_VALUE     0x00
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT1_VALUE 0x00
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT2_VALUE 0x00
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT3_VALUE 0xFD
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT4_VALUE 0x88
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT5_VALUE 0x00
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT6_VALUE 0x06
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT7_VALUE 0x09
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT8_VALUE 0x08
#define DRV10983_MOTOR_PARAMETER_REG_SYSOPT9_VALUE 0x01

#define DRV10983_MOTOR_READINGS_BASE_REG_ADDR        (0x10)
#define DRV10983_MOTOR_READINGS_STATUS_ADDR          (DRV10983_MOTOR_READINGS_BASE_REG_ADDR)
#define DRV10983_MOTOR_READINGS_MOTORSPEED_1_ADDR    (DRV10983_MOTOR_READINGS_BASE_REG_ADDR + 1)
#define DRV10983_MOTOR_READINGS_MOTORSPEED_2_ADDR    (DRV10983_MOTOR_READINGS_BASE_REG_ADDR + 2)
#define DRV10983_MOTOR_READINGS_MOTORPERIOD_1_ADDR   (DRV10983_MOTOR_READINGS_BASE_REG_ADDR + 3)
#define DRV10983_MOTOR_READINGS_MOTORPERIOD_2_ADDR   (DRV10983_MOTOR_READINGS_BASE_REG_ADDR + 4)
#define DRV10983_MOTOR_READINGS_MOTORKT_1_ADDR       (DRV10983_MOTOR_READINGS_BASE_REG_ADDR + 5)
#define DRV10983_MOTOR_READINGS_MOTORKT_2_ADDR       (DRV10983_MOTOR_READINGS_BASE_REG_ADDR + 6)
#define DRV10983_MOTOR_READINGS_IPDPOSITION_ADDR     (DRV10983_MOTOR_READINGS_BASE_REG_ADDR + 9)
#define DRV10983_MOTOR_READINGS_SUPPLYVOLTAGE_ADDR   (DRV10983_MOTOR_READINGS_BASE_REG_ADDR + 10)
#define DRV10983_MOTOR_READINGS_SPEEDCMD_ADDR        (DRV10983_MOTOR_READINGS_BASE_REG_ADDR + 11)
#define DRV10983_MOTOR_READINGS_SPEEDCMD_BUFFER_ADDR (DRV10983_MOTOR_READINGS_BASE_REG_ADDR + 12)
#define DRV10983_MOTOR_READINGS_FAULT_CODE_ADDR      (DRV10983_MOTOR_READINGS_BASE_REG_ADDR + 13)

static uint8_t motorReadingRegAddr[DRV10983_MOTOR_READINGS_REG_NUM] = {DRV10983_MOTOR_READINGS_STATUS_ADDR, DRV10983_MOTOR_READINGS_MOTORSPEED_1_ADDR,
                                                                       DRV10983_MOTOR_READINGS_MOTORSPEED_2_ADDR, DRV10983_MOTOR_READINGS_MOTORPERIOD_1_ADDR,
                                                                       DRV10983_MOTOR_READINGS_MOTORPERIOD_2_ADDR, DRV10983_MOTOR_READINGS_MOTORKT_1_ADDR,
                                                                       DRV10983_MOTOR_READINGS_MOTORKT_2_ADDR, DRV10983_MOTOR_READINGS_IPDPOSITION_ADDR,
                                                                       DRV10983_MOTOR_READINGS_SUPPLYVOLTAGE_ADDR, DRV10983_MOTOR_READINGS_SPEEDCMD_ADDR,
                                                                       DRV10983_MOTOR_READINGS_SPEEDCMD_BUFFER_ADDR,
                                                                       DRV10983_MOTOR_READINGS_FAULT_CODE_ADDR};

static uint8_t
drv10983_powerSupply_check(void);
static void drv10983_siDataBit_set(void);
static void drv10983_eepromProgStart_set(void);
static void drv10983_eepromProgKey_set(void);
static void drv10983_eeprom_refresh(void);
static void verybadDelay(uint32_t delayTime);

uint8_t defaultMotorParameterValues[DRV10983_MOTOR_PARAMETER_REG_NUM] = {DRV10983_MOTOR_PARAMETER_REG_MP1_VALUE, DRV10983_MOTOR_PARAMETER_REG_MP2_VALUE, DRV10983_MOTOR_PARAMETER_REG_MP3_VALUE, DRV10983_MOTOR_PARAMETER_REG_SYSOPT1_VALUE, DRV10983_MOTOR_PARAMETER_REG_SYSOPT2_VALUE, DRV10983_MOTOR_PARAMETER_REG_SYSOPT3_VALUE, DRV10983_MOTOR_PARAMETER_REG_SYSOPT4_VALUE, DRV10983_MOTOR_PARAMETER_REG_SYSOPT5_VALUE, DRV10983_MOTOR_PARAMETER_REG_SYSOPT6_VALUE, DRV10983_MOTOR_PARAMETER_REG_SYSOPT7_VALUE, DRV10983_MOTOR_PARAMETER_REG_SYSOPT8_VALUE, DRV10983_MOTOR_PARAMETER_REG_SYSOPT9_VALUE};

static void _delay_ms(void)
{
    volatile uint16_t count = 1000;
    while (count > 0)
        count--;
}

void drv10983_init(uint8_t *motorParameters)
{
    uint8_t supplyVoltage = 0;

    twi_init();

    // :NOTE: it halts here till the supply reaches the appropriate voltage level ~ 22 v
    while (supplyVoltage < DRV10983_VCC_CHECK_VALUE)
    {
        supplyVoltage = drv10983_powerSupply_check();
        _delay_ms();
    }

    drv10983_siDataBit_set();

    if (motorParameters == NULL)
    {
        drv10983_motorParameters_set(defaultMotorParameterValues);
    }
    else
    {
        drv10983_motorParameters_set(motorParameters);
    }

    // drv10983_speed_set();

    drv10983_eepromProgKey_set();
    drv10983_eepromProgStart_set();

    // wait for eeprom to finish ~24 msec
    verybadDelay(1000000);

    drv10983_eeprom_refresh();
}

void drv10983_motorParameters_set(uint8_t *array)
{
    uint8_t i = 0;

    for (i = 0; i < DRV10983_MOTOR_PARAMETER_REG_NUM; i++)
    {
        twi_start();
        twi_write(TWI_WRITE_SET(DRV10983_TWI_ADDR));
        twi_write(DRV10983_MOTOR_PARAMETER_BASE_REG_ADDR + i);
        twi_write(array[i]);
        twi_stop();
    }
}

void drv10983_motorParameters_get(uint8_t *readValues)
{
    uint8_t i = 0;

    for (i = 0; i < DRV10983_MOTOR_PARAMETER_REG_NUM; i++)
    {
        twi_start();
        twi_write(TWI_WRITE_SET(DRV10983_TWI_ADDR));
        twi_write(DRV10983_MOTOR_PARAMETER_BASE_REG_ADDR + i);
        twi_stop();

        twi_start();
        twi_write(TWI_READ_SET(DRV10983_TWI_ADDR));
        readValues[i] = twi_read(0);
        twi_stop();
    }
}

void drv10983_motorReadings_get(uint8_t *readValues)
{
    uint8_t i = 0;

    for (i = 0; i < DRV10983_MOTOR_READINGS_REG_NUM; i++)
    {
        twi_start();
        twi_write(TWI_WRITE_SET(DRV10983_TWI_ADDR));
        twi_write(motorReadingRegAddr[i]);
        twi_stop();

        twi_start();
        twi_write(TWI_READ_SET(DRV10983_TWI_ADDR));
        readValues[i] = twi_read(0);
        twi_stop();
    }
}

static uint8_t drv10983_powerSupply_check(void)
{
    uint8_t supplyVoltage = 0;

    twi_start();
    twi_write(TWI_WRITE_SET(DRV10983_TWI_ADDR));
    twi_write(DRV10983_SUPPLYVOLTAGE_REG_ADDR);
    twi_stop();

    twi_start();
    twi_write(TWI_READ_SET(DRV10983_TWI_ADDR));
    supplyVoltage = twi_read(0);
    twi_stop();

    return supplyVoltage;
}

static void drv10983_siDataBit_set(void)
{
    twi_start();
    twi_write(TWI_WRITE_SET(DRV10983_TWI_ADDR));
    twi_write(DRV10983_EECTRL_REG_ADDR);
    twi_write(DRV10983_EECTRL_SIDATA_BIT_SET | DRV10983_SLEEP_DISABLE());
    twi_stop();
}

static void drv10983_eepromProgStart_set(void)
{
    twi_start();
    twi_write(TWI_WRITE_SET(DRV10983_TWI_ADDR));
    twi_write(DRV10983_EECTRL_REG_ADDR);
    twi_write(DRV10983_EECTRL_SIDATA_BIT_SET | DRV10983_EECTRL_EEWRITE_BIT_SET | DRV10983_SLEEP_DISABLE());
    twi_stop();
}

static void drv10983_eepromProgKey_set(void)
{
    twi_start();
    twi_write(TWI_WRITE_SET(DRV10983_TWI_ADDR));
    twi_write(DRV10983_DEVCTRL_REG_ADDR);
    twi_write(DRV10983_PROG_KEY_ENABLE_VALUE);
    twi_stop();
}

static void drv10983_eeprom_refresh(void)
{
    twi_start();
    twi_write(TWI_WRITE_SET(DRV10983_TWI_ADDR));
    twi_write(DRV10983_EECTRL_REG_ADDR);
    twi_write(DRV10983_EECTRL_EEREFRESH_BIT_SET);
    twi_stop();
}

void drv10983_speed_set(void)
{
    drv10983_siDataBit_set();

    twi_start();
    twi_write(TWI_WRITE_SET(DRV10983_TWI_ADDR));
    twi_write(DRV10983_SPEED_REG_HIGH_ADDR);
    twi_write(0x81);
    twi_stop();

    twi_start();
    twi_write(TWI_WRITE_SET(DRV10983_TWI_ADDR));
    twi_write(DRV10983_SPEED_REG_LOW_ADDR);
    twi_write(0x8b);
    twi_stop();

    drv10983_eepromProgKey_set();
    drv10983_eepromProgStart_set();

    // wait for eeprom to finish ~24 msec
    verybadDelay(1000000);

    drv10983_eeprom_refresh();
}

uint8_t drv10983_speedCmdFB_get(void)
{
    uint8_t speedCmdFB = 0;

    twi_start();
    twi_write(TWI_WRITE_SET(DRV10983_TWI_ADDR));
    twi_write(0x1b);
    twi_stop();

    twi_start();
    twi_write(TWI_READ_SET(DRV10983_TWI_ADDR));
    speedCmdFB = twi_read(0);
    twi_stop();

    return speedCmdFB;
}

static void verybadDelay(uint32_t delayTime)
{
    volatile uint32_t count = 10000;
    while (count > 0)
        count--;
}
