#include "myspi.h"
#include "spi.h"

void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	__HAL_SPI_DISABLE(&hspi1);
	hspi1.Instance->CR1 &= 0XFFC7;
	hspi1.Instance->CR1 |= SPI_BaudRatePrescaler;
	__HAL_SPI_ENABLE(&hspi1);
}

uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{
	uint8_t Rxdata;
	HAL_SPI_TransmitReceive(&hspi1, &TxData, &Rxdata, 1, 1000);
	return Rxdata;
}
