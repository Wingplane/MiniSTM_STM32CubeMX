#include "myiic.h"

void IIC_Start(void)
{
	SDA_OUT;
	IIC_SDA_H;
	IIC_SCL_H;
	IIC_SDA_L;
	IIC_SCL_L;
}

void IIC_Stop(void)
{
	SDA_OUT;
	IIC_SCL_L;
	IIC_SDA_L;
	IIC_SCL_H;
	IIC_SDA_H;
}

uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime = 0;
	SDA_IN;
	IIC_SDA_H;
	IIC_SCL_H;
	while (READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_L;
	return 0;
}

void IIC_Ack(void)
{
	IIC_SCL_L;
	SDA_OUT;
	IIC_SDA_L;
	IIC_SCL_H;
	IIC_SCL_L;
}

void IIC_NAck(void)
{
	IIC_SCL_L;
	SDA_OUT;
	IIC_SDA_H;
	IIC_SCL_H;
	IIC_SCL_L;
}

void IIC_Send_Byte(uint8_t txd)
{
	uint8_t t;
	SDA_OUT;
	IIC_SCL_L;
	for (t = 0; t < 8; t++)
	{
		if ((txd & 0x80) >> 7)
			IIC_SDA_H;
		else
			IIC_SDA_L;
		txd <<= 1;
		IIC_SCL_H;
		IIC_SCL_L;
	}
}

uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA_IN;
	for (i = 0; i < 8; i++)
	{
		IIC_SCL_L;
		IIC_SCL_H;
		receive <<= 1;
		if (READ_SDA)
			receive++;
		HAL_Delay(1);
	}
	if (!ack)
		IIC_NAck();
	else
		IIC_Ack();
	return receive;
}
