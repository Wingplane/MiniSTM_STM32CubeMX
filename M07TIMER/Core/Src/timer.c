/**
 * @file : timer.c
 * @brief : timer driver
 * @author : zhang yue email:zhangyue@petalmail.com
 * @version : 1.0
 * @date : 2024-06-23
 */
#include "timer.h"
#include "led.h"

/**
 * @brief : timer3 init
 * @param  htim: timer handler
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim3))
    {
        LED1 = !LED1;
    }
}
