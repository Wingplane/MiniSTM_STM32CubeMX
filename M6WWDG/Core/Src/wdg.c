#include "wwdg.h"
#include "led.h"
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg)
{
    HAL_WWDG_Refresh(hwwdg);
    LED1=!LED1; 
}
