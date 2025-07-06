
#include "power_relay.h"
#include "AppWashingMachine.h"
static U08 FG_POWER_detect=0;

static void power_on_set_flag(void);
void Power_relay_init(void)
{
  
  GPIO_SetOutputEnableReg(GPIO_PH,GPIO_BIT_3,ENABLE); 
  GPIO_WriteDataBit(GPIO_PH,GPIO_BIT_3,GPIO_BIT_VALUE_1);
  GPIO_SetInputEnableReg(GPIO_PH,GPIO_BIT_1,ENABLE);
  GPIO_SetPullUp(GPIO_PH,GPIO_BIT_1,ENABLE);
  
  GPIO_EnableFuncReg(GPIO_PH,GPIO_FUNC_REG_1,GPIO_BIT_1);
  power_on_set_flag();
}
void Power_relay_Update(uint32_t period)
{
  if(FG_POWER_detect)
  {
    
    //GPIO_WriteDataBit(GPIO_PH,GPIO_BIT_3,(~GPIO_ReadDataBit(GPIO_PH,GPIO_BIT_3)));
    GPIO_ToggleDataBit(GPIO_PH,GPIO_BIT_3);
    //PORT_RELAY_POWER = ~PORT_RELAY_POWER;
  }
  else
  {
    PORT_RELAY_POWER_write(1);
    //PORT_RELAY_POWER = 1;
  }
}



/*--------------------------------------------------------*/
void power_on_set_flag(void)
{
  FG_POWER_detect = 1;
}

