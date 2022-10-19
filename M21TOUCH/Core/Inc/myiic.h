#ifndef _MYIIC_H
#define _MYIIC_H
#include "stm32f1xx_hal.h"

#define SDA_IN                           \
	{                                    \
		GPIOC->CRH &= 0XFFFF0FFF;        \
		GPIOC->CRH |= (uint32_t)8 << 12; \
	}
#define SDA_OUT                          \
	{                                    \
		GPIOC->CRH &= 0XFFFF0FFF;        \
		GPIOC->CRH |= (uint32_t)3 << 12; \
	}

#define IIC_SCL_H HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET)
#define IIC_SCL_L HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET)
#define IIC_SDA_H HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET)
#define IIC_SDA_L HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET)
#define READ_SDA HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11)

void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Read_Byte(unsigned char ack);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);

void IIC_Write_One_Byte(uint8_t daddr, uint8_t addr, uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr, uint8_t addr);
#endif
