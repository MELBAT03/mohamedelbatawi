#include "def.h"
#include "board.h"

#include "components/heartbeat/heartbeat.h"
#include "components/gpio/gpio.h"

void handler_heartbeat_init(void)
{
#ifdef HEARTBEAT_LED_ENABLE
    gpio_t x_heartbeatGpio;

    x_heartbeatGpio.port = HEARTBEAT_LED_PORT;
    x_heartbeatGpio.pin = HEARTBEAT_LED_PIN;
    x_heartbeatGpio.state = HEARTBEAT_LED_INIT_STATE;
    heartbeat_init(&x_heartbeatGpio);
#endif
}
