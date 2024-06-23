/**
 * @file : led.h
 * @brief : LED driver
 * @author : zhang yue email:zhangyue@petalmail,com
 * @version : 1.0
 * @date : 2024-06-23
 */
#ifndef __LED_H
#define __LED_H
#include "stm32f1xx_hal.h"

#define LED0 *((volatile unsigned long *)((((GPIOA_BASE + 12) & 0xF0000000) + 0x2000000 + (((GPIOA_BASE + 12) & 0xFFFFF) << 5) + (8 << 2)))) // PA8
#define LED1 *((volatile unsigned long *)((((GPIOD_BASE + 12) & 0xF0000000) + 0x2000000 + (((GPIOD_BASE + 12) & 0xFFFFF) << 5) + (2 << 2)))) // PD2

#endif // !__LED_H
