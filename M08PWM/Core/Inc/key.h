/**
 * @file : key.h
 * @brief : key driver
 * @author : zhang yue email:zhangyue@petalmail.com
 * @version : 1.0
 * @date : 2024-06-23
 */
#ifndef __KEY_H
#define __KEY_H
#include "stm32f1xx_hal.h"

#define KEY0 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)
#define KEY1 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)
#define WK_UP HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)

#define KEY0_PRES 1
#define KEY1_PRES 2
#define WKUP_PRES 3

uint8_t KEY_Scan(uint8_t mode);
#endif // !__KEY_H
