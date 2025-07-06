/*
 * LedDriver.c
 *
 *  Created on: Aug 29, 2021
 *      Author: MMORGA03
 */

#include "LedDriver.h"
#include "mcal.h"
#include "component_config.h"

ROW rows[NUM_OF_ROWS];
Data_Setting_t Data_Setting;
Display_Mode_Setting_t Display_Mode_Setting;
Address_Setting_t Address_Setting;
/**
 * @brief Set the display mode object
 *
 * @param disp_mode
 */
void set_display_mode(DISP_MODE disp_mode)
{
    if (disp_mode == DISP_12GRID_10SEG)
    {
        Display_Mode_Setting.BIT.disp_mode_bits = 3;
    }
    else if (disp_mode == DISP_11GRID_11SEG)
    {
        Display_Mode_Setting.BIT.disp_mode_bits = 4;
    }
    else if (disp_mode == DISP_10GRID_12SEG)
    {
        Display_Mode_Setting.BIT.disp_mode_bits = 5;
    }
    else if (disp_mode == DISP_9GRID_13SEG)
    {
        Display_Mode_Setting.BIT.disp_mode_bits = 6;
    }
    else if (disp_mode == DISP_8GRID_14SEG)
    {
        Display_Mode_Setting.BIT.disp_mode_bits = 7;
    }
    else
    {
        /*error hanlde*/
    }
}
/**
 * @brief Set the disp on object
 *
 * @param disp_setting
 */
void set_disp_on(DISP_SETTING disp_setting)
{
    if (disp_setting == DISP_ON)
    {
        Display_Mode_Setting.BIT.disp_setting = 1;
    }
    else if (disp_setting == DISP_OFF)
    {
        Display_Mode_Setting.BIT.disp_setting = 0;
    }
    else
    {
        /* error handle*/
    }
}
/**
 * @brief Set the read write setting object
 *
 * @param write_read_setting
 */
void set_read_write_setting(WRITE_READ_SETTING write_read_setting)
{
    if (write_read_setting == WRITE_DATA_TO_DISPLAY)
    {
        Data_Setting.BIT.data_write_read_setting = 0;
    }
    else if (write_read_setting == READ_DATA_FROM_DISPALY)
    {
        Data_Setting.BIT.data_write_read_setting = 1;
    }
    else if (write_read_setting == READ_KEY_DATA)
    {
        Data_Setting.BIT.data_write_read_setting = 2;
    }
    else if (write_read_setting == READ_DATA_FROM_COMMAND_REGISTER)
    {
        Data_Setting.BIT.data_write_read_setting = 3;
    }
    else
    {
        /*error handle*/
    }
}
/**
 * @brief Set the address mode object
 *
 * @param address_mode
 */
void set_address_mode(ADDRESS_MODE_SETTING address_mode)
{
    if (address_mode == INCREMENT_ADDRESS)
    {
        Data_Setting.BIT.address_increment = 0;
    }
    else if (address_mode == FIXED_ADDRESS)
    {
        Data_Setting.BIT.address_increment = 1;
    }
    else
    {
        /*error handle*/
    }
}
void set_OP_mode(MODE_SETTING mode_setting)
{
    if (mode_setting == NOMRAL_OP_MODE)
    {
        Data_Setting.BIT.mode_setting_bits = 0;
    }
    else if (mode_setting == TEST_MODE)
    {
        Data_Setting.BIT.mode_setting_bits = 1;
    }
    else
    {
        /*error handle*/
    }
}
/**
 * @brief Set the address object
 *
 * @param address
 */
void set_address(uint8_t address)
{
    Address_Setting.BIT.address = address;
}
/**
 * @brief
 *
 * @param config
 */
void init_LEDDriver(IK2108_CONFIG_t *config)
{

    /*setGPIO PINS*/
    core_gpio_pinDirection_set(MCAL_GPIO_PORTA, 0, MCAL_GPIO_OUTPUT); /*DIO*/
    core_gpio_pinDirection_set(MCAL_GPIO_PORTA, 1, MCAL_GPIO_OUTPUT); /*CLK*/
    core_gpio_pinDirection_set(MCAL_GPIO_PORTA, 3, MCAL_GPIO_OUTPUT); /*STB*/
    /*initial VAlues for Commands*/
    Display_Mode_Setting.Byte = 0x00;
    Data_Setting.Byte = 0x40;
    Address_Setting.Byte = 0xC0;

    /*set Configurations Values*/
    set_display_mode(config->disp_mode);
    set_disp_on(config->disp_setting);
    set_read_write_setting(config->write_read_setting);
    set_address_mode(config->address_mode);
    set_OP_mode(config->mode_setting);
    set_address(config->address);

    /*Send Configuration to Driver*/
    STB_LOW;
    IK2108_Write_8bit(Display_Mode_Setting.Byte);
    IK2108_Write_8bit(Data_Setting.Byte);
    // IK2108_Write_8bit(Address_Setting.Byte);
    STB_HIGH;
}
/**
 * @brief
 *
 * @param command
 */
void send_command(COMMAND_t command)
{
    switch (command)
    {
        case COMMAND_1:
            IK2108_Write_8bit(Display_Mode_Setting.Byte);
            break;
        case COMMAND_2:
            IK2108_Write_8bit(Data_Setting.Byte);
            break;
        case COMMAND_3:
            IK2108_Write_8bit(Address_Setting.Byte);
            break;
        default:
            /*error handle*/
            break;
    }
}
/**
 * @brief
 *
 * @param data
 */
void IK2108_Write_8bit(uint8_t data)
{
    // _asm("nop");
    for (int i = 0; i < 8; i++)
    {
        CLK_LOW;
        if (data & 0x1)
        {
            DIO_HIGH;
        }
        else
        {
            DIO_LOW;
        }
        data = data >> 1;
        // __asm("nop");
        CLK_HIGH;
        // __asm("nop");
    }
}
/**
 * @brief
 *
 */

/*
LED Num = COL | ROW
COL == Segment && ROW == GRID


*/
void LedDriver_LedState_set(ledDriver_ledEnum_t lednum, uint8_t state)
{
    uint8_t row, segment;
    row = lednum % 10;
    segment = lednum / 10;
    rows[row].Byte &= ~(1 << segment);
    rows[row].Byte |= (!state << segment);   // LED ON with ZERO
}

void LedDriver_row_set(uint8_t rownum, uint8_t value)
{
    rows[rownum].Byte = value;
}

void LedDriver_row_clear(uint8_t rownum)
{
    rows[rownum].Byte = 0x00;
}

void LedDriver_clear(void)
{
    uint8_t i;
    for (i = 0; i < NUM_OF_ROWS; i++)
    {
        LedDriver_row_clear(i);
    }
}
void LedDriver_dp_set(uint8_t rownum, uint8_t state)
{
    rows[rownum].BIT.b7 = state;
}
void ledDriver_update(uint32_t period)
{
    uint8_t i;
    STB_LOW;
    IK2108_Write_8bit(0xC0);
    for (i = 0; i < NUM_OF_ROWS; i++)
    {

        // IK2108_Write_8bit(0x02); // dimming 3/16
        // IK2108_Write_8bit(0x03); // dimming 5/16
        // IK2108_Write_8bit(0x04); // dimming 7/16
        // IK2108_Write_8bit(0x05); // dimming 10/16
        // IK2108_Write_8bit(0x06); // dimming 14/16
        IK2108_Write_8bit(IK2108_DIMMING);

        IK2108_Write_8bit(rows[i].Byte);
        IK2108_Write_8bit(0x00);
        IK2108_Write_8bit(0x00);
    }
    STB_HIGH;
}
