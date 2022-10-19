#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "stm32f1xx_hal.h"

#define STM32_FLASH_SIZE 256
#define STM32_FLASH_WREN 1
#define FLASH_WAITETIME 50000
#define STM32_FLASH_BASE 0x08000000

uint8_t STMFLASH_GetStatus(void);
uint8_t STMFLASH_WaitDone(uint16_t time);
uint8_t STMFLASH_ErasePage(uint32_t paddr);
uint8_t STMFLASH_WriteHalfWord(uint32_t faddr, uint16_t dat);
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);
void STMFLASH_WriteLenByte(uint32_t WriteAddr, uint32_t DataToWrite, uint16_t Len);
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr, uint16_t Len);
void STMFLASH_Write(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);
void STMFLASH_Read(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead);
void Test_Write(uint32_t WriteAddr, uint16_t WriteData);

#endif
