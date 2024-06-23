/**
 * @file : wdg.c
 * @brief : watchdog driver
 * @author : zhang yue email:zhangyue@petalmail.com
 * @version : 1.0
 * @date : 2024-06-23
 */
#include "wwdg.h"
#include "led.h"

/**
 * @brief : watchdog initialization
 * @param  hwwdg: watchdog handler
 */
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
    HAL_WWDG_Refresh(hwwdg);
    LED1 = !LED1;
}
