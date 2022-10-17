#include "led.h"
#include "wkup.h"

void Sys_Enter_Standby(void)
{
    __HAL_RCC_APB2_FORCE_RESET();
   	__HAL_RCC_PWR_CLK_ENABLE();
			  	
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
    HAL_PWR_EnterSTANDBYMode();
}

uint8_t Check_WKUP(void) 
{
	uint8_t t=0;
	LED0=0;
	while(1)
	{
		if(WKUP_KD)
		{
			t++;
			HAL_Delay(30);
			if(t>=100)
			{
				LED0=0;
				return 1;
			}
		}else 
		{ 
			LED0=1;
			return 0;
		}
	}
}  

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin==GPIO_PIN_0)
    {
        if(Check_WKUP())
        {
            Sys_Enter_Standby();
        }
    }    
}

void WKUP_Init(void)
{
    if(Check_WKUP()==0)
    {
        Sys_Enter_Standby();
    }
}
