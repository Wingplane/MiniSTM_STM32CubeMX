#include "w25qxx.h"
#include "spi.h"
#include "myspi.h"
#include "usart.h"
#include "uart.h"
#include "stm32f1xx_hal_gpio.h"

uint16_t W25QXX_TYPE = W25Q64;

void W25QXX_Init(void)
{
	uint8_t temp;
	W25QXX_CS_H;
//	SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_2);
	W25QXX_TYPE = W25QXX_ReadID();
	if (W25QXX_TYPE == W25Q256)
	{
		temp = W25QXX_ReadSR(3);
		if ((temp & 0X01) == 0)
		{
			W25QXX_CS_L;
			SPI1_ReadWriteByte(W25X_Enable4ByteAddr);
			W25QXX_CS_H;
		}
	}
}

uint8_t W25QXX_ReadSR(uint8_t regno)
{
	uint8_t byte = 0, command = 0;
	switch (regno)
	{
	case 1:
		command = W25X_ReadStatusReg1;
		break;
	case 2:
		command = W25X_ReadStatusReg2;
		break;
	case 3:
		command = W25X_ReadStatusReg3;
		break;
	default:
		command = W25X_ReadStatusReg1;
		break;
	}
	W25QXX_CS_L;
	SPI1_ReadWriteByte(command);
	byte = SPI1_ReadWriteByte(0Xff);
	W25QXX_CS_H;
	return byte;
}

void W25QXX_Write_SR(uint8_t regno, uint8_t sr)
{
	uint8_t command = 0;
	switch (regno)
	{
	case 1:
		command = W25X_WriteStatusReg1;
		break;
	case 2:
		command = W25X_WriteStatusReg2;
		break;
	case 3:
		command = W25X_WriteStatusReg3;
		break;
	default:
		command = W25X_WriteStatusReg1;
		break;
	}
	W25QXX_CS_L;
	SPI1_ReadWriteByte(command);
	SPI1_ReadWriteByte(sr);
	W25QXX_CS_H;
}

void W25QXX_Write_Enable(void)
{
	W25QXX_CS_L;
	SPI1_ReadWriteByte(W25X_WriteEnable);
	W25QXX_CS_H;
}

void W25QXX_Write_Disable(void)
{
	W25QXX_CS_L;
	SPI1_ReadWriteByte(W25X_WriteDisable);
	W25QXX_CS_H;
}

uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;
	W25QXX_CS_L;
	SPI1_ReadWriteByte(0x90);
	SPI1_ReadWriteByte(0x00);
	SPI1_ReadWriteByte(0x00);
	SPI1_ReadWriteByte(0x00);
	Temp |= SPI1_ReadWriteByte(0xFF) << 8;
	Temp |= SPI1_ReadWriteByte(0xFF);
	W25QXX_CS_H;
	return Temp;
}

void W25QXX_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	uint16_t i;
	W25QXX_CS_L;
	SPI1_ReadWriteByte(W25X_ReadData);
	if (W25QXX_TYPE == W25Q256)
	{
		SPI1_ReadWriteByte((uint8_t)((ReadAddr) >> 24));
	}
	SPI1_ReadWriteByte((uint8_t)((ReadAddr) >> 16));
	SPI1_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
	SPI1_ReadWriteByte((uint8_t)ReadAddr);
	for (i = 0; i < NumByteToRead; i++)
	{
		pBuffer[i] = SPI1_ReadWriteByte(0XFF);
	}
	W25QXX_CS_H;
}

void W25QXX_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint16_t i;
	W25QXX_Write_Enable();
	W25QXX_CS_L;
	SPI1_ReadWriteByte(W25X_PageProgram);
	if (W25QXX_TYPE == W25Q256)
	{
		SPI1_ReadWriteByte((uint8_t)((WriteAddr) >> 24));
	}
	SPI1_ReadWriteByte((uint8_t)((WriteAddr) >> 16));
	SPI1_ReadWriteByte((uint8_t)((WriteAddr) >> 8));
	SPI1_ReadWriteByte((uint8_t)WriteAddr);
	for (i = 0; i < NumByteToWrite; i++)
		SPI1_ReadWriteByte(pBuffer[i]);
	W25QXX_CS_H;
	W25QXX_Wait_Busy();
}

void W25QXX_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint16_t pageremain;
	pageremain = 256 - WriteAddr % 256;
	if (NumByteToWrite <= pageremain)
		pageremain = NumByteToWrite;
	while (1)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
		if (NumByteToWrite == pageremain)
			break;
		else
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;

			NumByteToWrite -= pageremain;
			if (NumByteToWrite > 256)
				pageremain = 256;
			else
				pageremain = NumByteToWrite;
		}
	};
}

uint8_t W25QXX_BUFFER[4096];
void W25QXX_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;
	uint16_t i;
	uint8_t *W25QXX_BUF;
	W25QXX_BUF = W25QXX_BUFFER;
	secpos = WriteAddr / 4096;
	secoff = WriteAddr % 4096;
	secremain = 4096 - secoff;
	// printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);
	if (NumByteToWrite <= secremain)
		secremain = NumByteToWrite;
	while (1)
	{
		W25QXX_Read(W25QXX_BUF, secpos * 4096, 4096);
		for (i = 0; i < secremain; i++)
		{
			if (W25QXX_BUF[secoff + i] != 0XFF)
				break;
		}
		if (i < secremain)
		{
			W25QXX_Erase_Sector(secpos);
			for (i = 0; i < secremain; i++)
			{
				W25QXX_BUF[i + secoff] = pBuffer[i];
			}
			W25QXX_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096);
		}
		else
			W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain);
		if (NumByteToWrite == secremain)
			break;
		else
		{
			secpos++;
			secoff = 0;

			pBuffer += secremain;
			WriteAddr += secremain;
			NumByteToWrite -= secremain;
			if (NumByteToWrite > 4096)
				secremain = 4096;
			else
				secremain = NumByteToWrite;
		}
	};
}

void W25QXX_Erase_Chip(void)
{
	W25QXX_Write_Enable();
	W25QXX_Wait_Busy();
	W25QXX_CS_L;
	SPI1_ReadWriteByte(W25X_ChipErase);
	W25QXX_CS_H;
	W25QXX_Wait_Busy();
}

void W25QXX_Erase_Sector(uint32_t Dst_Addr)
{
	// printf("fe:%x\r\n",Dst_Addr);
	Dst_Addr *= 4096;
	W25QXX_Write_Enable();
	W25QXX_Wait_Busy();
	W25QXX_CS_L;
	SPI1_ReadWriteByte(W25X_SectorErase);
	if (W25QXX_TYPE == W25Q256)
	{
		SPI1_ReadWriteByte((uint8_t)((Dst_Addr) >> 24));
	}
	SPI1_ReadWriteByte((uint8_t)((Dst_Addr) >> 16));
	SPI1_ReadWriteByte((uint8_t)((Dst_Addr) >> 8));
	SPI1_ReadWriteByte((uint8_t)Dst_Addr);
	W25QXX_CS_H;
	W25QXX_Wait_Busy();
}

void W25QXX_Wait_Busy(void)
{
	while ((W25QXX_ReadSR(1) & 0x01) == 0x01)
		;
}

void W25QXX_PowerDown(void)
{
	W25QXX_CS_L;
	SPI1_ReadWriteByte(W25X_PowerDown);
	W25QXX_CS_H;
	HAL_Delay(3000);
}

void W25QXX_WAKEUP(void)
{
	W25QXX_CS_L;
	SPI1_ReadWriteByte(W25X_ReleasePowerDown);
	W25QXX_CS_H;
	HAL_Delay(3000);
}
