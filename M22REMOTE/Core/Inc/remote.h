#ifndef __REMOTE_H
#define __REMOTE_H

#include "stm32f1xx_hal.h"

#define RDATA_R HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)
#define REMOTE_ID 0

extern uint8_t RmtCnt;
uint8_t Remote_Scan(void);

#endif
