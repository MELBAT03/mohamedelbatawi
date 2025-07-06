#include "doutput_module/doutput_module.h"
#include "def.h"
#include "board.h"

#include "handlers/output/inc/handler_water_valve.h"

static doutputModule_handle gx_coldValveHandle;
static doutputModule_handle gx_hotValveHandle;
static doutputModule_handle gx_softenerValveHandle;
static doutputModule_handle gx_showerValveHandle;

void handler_waterValve_init(void)
{
    doutputModule_t coldValve;
    doutputModule_t hotValve;
    doutputModule_t softenerValve;
    doutputModule_t showerValve;

    coldValve.gpio.port = COLD_WATER_VALVE_PORT;
    coldValve.gpio.pin = COLD_WATER_VALVE_PIN;
    coldValve.gpio.state = WATER_VALVE_INACTIVE;
    gx_coldValveHandle = doutputModule_init(&coldValve);

    hotValve.gpio.port = HOT_WATER_VALVE_PORT;
    hotValve.gpio.pin = HOT_WATER_VALVE_PIN;
    hotValve.gpio.state = WATER_VALVE_INACTIVE;
    gx_hotValveHandle = doutputModule_init(&hotValve);

    softenerValve.gpio.port = SOFTENER_WATER_VALVE_PORT;
    softenerValve.gpio.pin = SOFTENER_WATER_VALVE_PIN;
    softenerValve.gpio.state = WATER_VALVE_INACTIVE;
    gx_softenerValveHandle = doutputModule_init(&softenerValve);

    showerValve.gpio.port = SHOWER_WATER_VALVE_PORT;
    showerValve.gpio.pin = SHOWER_WATER_VALVE_PIN;
    showerValve.gpio.state = WATER_VALVE_INACTIVE;
    gx_showerValveHandle = doutputModule_init(&showerValve);
}

void handler_coldWaterValve_state_set(handler_waterValve_state_t state)
{
    (void)doutputModule_state_set(gx_coldValveHandle, state);
}

void handler_hotWaterValve_state_set(handler_waterValve_state_t state)
{
    (void)doutputModule_state_set(gx_hotValveHandle, state);
}

void handler_softenerWaterValve_state_set(handler_waterValve_state_t state)
{
    (void)doutputModule_state_set(gx_softenerValveHandle, state);
}

void handler_showerWaterValve_state_set(handler_waterValve_state_t state)
{
    (void)doutputModule_state_set(gx_showerValveHandle, state);
}

void handler_condenserWaterValve_state_set(handler_waterValve_state_t state)
{
    // do nothing
}

handler_waterValve_state_t handler_coldWaterValve_state_get(void)
{
    return (handler_waterValve_state_t)doutputModule_state_get(gx_coldValveHandle);
}

handler_waterValve_state_t handler_hotWaterValve_state_get(void)
{
    return (handler_waterValve_state_t)doutputModule_state_get(gx_hotValveHandle);
}

handler_waterValve_state_t handler_softenerWaterValve_state_get(void)
{
    return (handler_waterValve_state_t)doutputModule_state_get(gx_softenerValveHandle);
}

handler_waterValve_state_t handler_showerWaterValve_state_get(void)
{
    return (handler_waterValve_state_t)doutputModule_state_get(gx_showerValveHandle);
}

handler_waterValve_state_t handler_condenserWaterValve_state_get(void)
{
    return (handler_waterValve_state_t)0;
}
