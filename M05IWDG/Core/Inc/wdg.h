/**
 * @file : wdg.h
 * @brief : watchdog driver
 * @author : zhang yue email:zhangyue@petalmail.com
 * @version : 1.0
 * @date : 2024-06-23
 */
#ifndef __WDG_H
#define __WDG_H
#include "stm32f1xx_hal.h"
#include "iwdg.h"
void IWDG_Feed(void);
#endif // !__WDG_H
