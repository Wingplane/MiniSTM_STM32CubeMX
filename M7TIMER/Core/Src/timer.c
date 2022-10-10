#include "timer.h"
#include "led.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&htim3))
    {
        LED1=!LED1;
    }
}
