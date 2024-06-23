/**
 * @file : wdg.c
 * @brief : watchdog driver
 * @author : zhang yue email:zhangyue@petalmail.com
 * @version : 1.0
 * @date : 2024-06-23
 */
#include "wdg.h"

/**
 * @brief : feed the watchdog
 */
void IWDG_Feed(void)
{
	HAL_IWDG_Refresh(&hiwdg); // feed the watchdog
}
