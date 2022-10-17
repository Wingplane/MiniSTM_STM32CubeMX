#ifndef __ITEMP_H
#define __ITEMP_H
#include "stm32f1xx_hal.h"

uint16_t  Get_Adc(void);
uint16_t Get_Adc_Average(uint32_t ch,uint8_t times);
short Get_Temprate(void);

#endif // !__ITEMP_H
