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
