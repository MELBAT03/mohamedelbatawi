#include "mcal.h"
#include "def.h"
#include "board.h"
#include "os.h"

#include "handlers/input/inc/handler_power_detection.h"
#include "components/zcd/zcd.h"

#define HANDLER_POWER_DETECTION_MIN_CYCLES_WAIT_COUNT (4UL)

void (*handler_powerDetection_isr)(void);

#ifndef ZCD_SYNC_ENABLE
static handler_powerDetection_sate_t gx_handler_powerDetectionTest_state = HANDLER_POWER_DETECTION_OK;
#endif

void handler_powerDetection_init(void)
{
    zcd_config_t zcd;

    zcd.pin.port = ZCD_PORT;
    zcd.pin.pin = ZCD_PIN;
    zcd.pin.ioState = MCAL_GPIO_INPUT;
    zcd.pin.pinState = GPIO_LOW;
    zcd.irqPinNumber = ZCD_IRQ;
    zcd.irqReq = ZCD_IRQ;
    zcd.edge = MCAL_EXTINT_RISING_EDGE;

    zcd.minDetectionCycleCnt = HANDLER_POWER_DETECTION_MIN_CYCLES_WAIT_COUNT;

    handler_powerDetection_isr = zcd_init(&zcd);
}

void handler_powerDetection_edge_set(mcal_exti_detection_type_t edge)
{
    zcd_config_t zcd;

    zcd.pin.port = ZCD_PORT;
    zcd.pin.pin = ZCD_PIN;
    zcd.pin.ioState = MCAL_GPIO_INPUT;
    zcd.pin.pinState = GPIO_LOW;
    zcd.irqPinNumber = ZCD_IRQ;
    zcd.irqReq = ZCD_IRQ;
    zcd.edge = edge;

    zcd.minDetectionCycleCnt = HANDLER_POWER_DETECTION_MIN_CYCLES_WAIT_COUNT;

    handler_powerDetection_isr = zcd_init(&zcd);
}

handler_powerDetection_sate_t handler_powerDetection_state_get(void)
{
#ifdef ZCD_SYNC_ENABLE
    return zcd_state_get();
#else
    (void)zcd_state_get();
    return gx_handler_powerDetectionTest_state;
#endif
}

void handler_powerDetection_state_set(handler_powerDetection_sate_t value)
{
#ifndef ZCD_SYNC_ENABLE 
    gx_handler_powerDetectionTest_state = value;
#else
    // do nothing
#endif
}

uint8_t handler_powerDetection_freq_get(void)
{
#ifdef ZCD_SYNC_ENABLE
    return zcd_freq_get();
#else
    (void)zcd_freq_get();
    return 50;
#endif
}
