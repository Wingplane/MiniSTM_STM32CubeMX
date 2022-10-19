#ifndef __MYSPI_H
#define __MYSPI_H
#include "stm32f1xx_hal.h"

void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler);
uint8_t SPI1_ReadWriteByte(uint8_t TxData);

#endif // !__MYSPI_H
