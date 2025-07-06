#include "def.h"
#include "component_config.h"

#include "components/uart/uart.h"

void handler_com_init(void)
{
    uart_config_t uartConf;

    //  NOTE: for some weird reason, if you move this to the 3rd location, some error happens with other uart modules. unless this channel speed is set to the same speed as the others
    uartConf.baudRate = 115200;
    uartConf.mode = UART_MODE_TXRX;
    uartConf.dataBits = UART_DATA_BITS_8;
    uartConf.parity = UART_PARITY_NONE;
    uartConf.stopBits = UART_STOP_BITS_1;
    uartConf.rxInterruptEN = 1;
    uartConf.chainInterval = 0;
    uart_init((uart_t)LOG_CHANNEL_ID, &uartConf);

    uartConf.baudRate = 4800;
    uartConf.mode = UART_MODE_TXRX;
    uartConf.dataBits = UART_DATA_BITS_8;
    uartConf.parity = UART_PARITY_ODD;
    uartConf.stopBits = UART_STOP_BITS_1;
    uartConf.rxInterruptEN = 1;
    uartConf.chainInterval = 0;
    uart_init((uart_t)MOTOR_COM_PORT_ID, &uartConf);

    uartConf.baudRate = 9600;
    uartConf.mode = UART_MODE_TXRX;
    uartConf.dataBits = UART_DATA_BITS_8;
    uartConf.parity = UART_PARITY_NONE;
    uartConf.stopBits = UART_STOP_BITS_1;
    uartConf.rxInterruptEN = 1;
    uartConf.chainInterval = 0;
    uart_init((uart_t)DISPLAY_PORT_ID, &uartConf);
}
