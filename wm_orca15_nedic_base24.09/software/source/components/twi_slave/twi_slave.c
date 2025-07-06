#include "UTILS.h"

#include "MAIN_HOLTEK_HT66F489.h"
#include "PORT_HOLTEK_HT66F489_FINAL.h"

typedef struct
{
    uint8_t buff[10];
    uint8_t id0;
    uint8_t idx;
} twiSlave_t;

static volatile uint8_t gu8_twiSlaveData_flag;
static volatile twiSlave_t gx_twiSlave_receiver, gx_twiSlave_transmitter;

void twi_slave_init(uint8_t slaveAddr)
{
    gx_twiSlave_receiver.id0 = 0u;
    gx_twiSlave_transmitter.id0 = 0u;
    gx_twiSlave_receiver.idx = 0u;
    gx_twiSlave_transmitter.idx = 0u;
    gu8_twiSlaveData_flag = 0u;

    TWI_SLAVE_REG_INIT();
    TWI_SLAVE_SEND_ACK_SET();
    TWI_SLAVE_ADDR_SET(slaveAddr);
    TWI_SLAVE_INTERRUPT_ENABLE();
    TWI_SLAVE_ENABLE();
}

uint8_t twi_slave_dataReadyFlag_get(void)
{
    return gu8_twiSlaveData_flag;
}

void twi_slave_data_set(uint8_t data)
{
    if (gx_twiSlave_transmitter.idx < 10u)
    {
        gx_twiSlave_transmitter.buff[gx_twiSlave_transmitter.idx] = data;
        gx_twiSlave_transmitter.idx++;
    }
}

uint8_t twi_slave_data_get(void)
{
    volatile uint8_t ret;

    ret = gx_twiSlave_receiver.buff[gx_twiSlave_receiver.id0];
    gx_twiSlave_receiver.id0++;
    if (gx_twiSlave_receiver.id0 == gx_twiSlave_receiver.idx)
    {
        gx_twiSlave_receiver.id0 = 0u;
        gx_twiSlave_receiver.idx = 0u;
        gu8_twiSlaveData_flag = 0u;
    }
    return ret;
}

twi_slave_isr()
{
    uint8_t temp = 0;
    if (TWI_SLAVE_ADDRESS_MATCHING_FLAG_GET() == TWI_SLAVE_ADDR_MATCH)
    {
        if (TWI_SLAVE_READ_WRITE_MODE_GET() == TWI_SLAVE_WRITE)
        {
            TWI_SLAVE_TRANSMITTER_SET();
            // :TODO: write data to simd
            TWI_SLAVE_DATA_SET(gx_twiSlave_transmitter.buff[gx_twiSlave_transmitter.id0]);
            gx_twiSlave_transmitter.id0++;
        }
        else if (TWI_SLAVE_READ_WRITE_MODE_GET() == TWI_SLAVE_READ)
        {
            TWI_SLAVE_RECEIVER_SET();
            TWI_SLAVE_SEND_ACK_SET();
            // :TODO: read from simd
            // gu8_twiSlaveData_flag = 1u;
            // gx_twiSlave_receiver.buff[gx_twiSlave_receiver.idx++] = TWI_SLAVE_DATA_GET();
            temp = TWI_SLAVE_DATA_GET();
        }
    }
    else if (TWI_SLAVE_DATA_READY_FLAG_GET() == TWI_SLAVE_DATA_RDY)
    {
        if (TWI_SLAVE_MODE_GET() == TWI_SLAVE_TRANSIMTTER_MODE)
        {
            if (TWI_SLAVE_ACK_FROM_MASTER_GET() == TWI_SLAVE_MASTER_NACK)
            {
                TWI_SLAVE_RECEIVER_SET();
                TWI_SLAVE_SEND_ACK_SET();
                // :TODO: read from simd
                gu8_twiSlaveData_flag = 1;
                gx_twiSlave_receiver.buff[gx_twiSlave_receiver.idx++] = TWI_SLAVE_DATA_GET();
            }
            else if (TWI_SLAVE_ACK_FROM_MASTER_GET() == TWI_SLAVE_MASTER_ACK)
            {
                // :TODO: write to simd
                TWI_SLAVE_DATA_SET(gx_twiSlave_transmitter.buff[gx_twiSlave_transmitter.id0]);
                gx_twiSlave_transmitter.id0++;
            }
        }
        else if (TWI_SLAVE_MODE_GET() == TWI_SLAVE_RECEIVER_MODE)
        {
            // :TODO: read from simd
            gu8_twiSlaveData_flag = 1u;
            gx_twiSlave_receiver.buff[gx_twiSlave_receiver.idx++] = TWI_SLAVE_DATA_GET();
        }
    }

    if (gx_twiSlave_transmitter.id0 == gx_twiSlave_transmitter.idx)
    {
        gx_twiSlave_transmitter.id0 = 0u;
        gx_twiSlave_transmitter.idx = 0u;
    }
}
