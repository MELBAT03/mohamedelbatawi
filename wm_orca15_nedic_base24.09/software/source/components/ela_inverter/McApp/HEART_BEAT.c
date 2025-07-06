
#include "../core/tmpm370/inc/tmpm370_gpio.h"
#include "HEART_BEAT.h"


void HEARTBEAT_Init(void)
{
  
  GPIO_InitTypeDef Hart_breat;
  Hart_breat.IOMode = GPIO_OUTPUT_MODE;
  Hart_breat.OpenDrain =GPIO_OPEN_DRAIN_DISABLE;
  GPIO_Init(GPIO_PG , GPIO_BIT_1,&Hart_breat);
}
//
void HEARTBEAT_Update(uint32_t period)
{
  GPIO_ToggleDataBit(GPIO_PG , GPIO_BIT_1);
}