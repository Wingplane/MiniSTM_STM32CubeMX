#ifndef __AD_H
#define __AD_H
#include "stm32f1xx_hal.h"

uint16_t  Get_Adc(uint32_t ch);
uint16_t Get_Adc_Average(uint32_t ch,uint8_t times);

#endif // !__AD_H
