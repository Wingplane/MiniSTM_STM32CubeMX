#ifndef __WKUP_H
#define __WKUP_H
#include "stm32f1xx_hal.h"

#define WKUP_KD HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)

uint8_t Check_WKUP(void);
void WKUP_Init(void);
void Sys_Enter_Standby(void);

#endif // !__WKUP_H
