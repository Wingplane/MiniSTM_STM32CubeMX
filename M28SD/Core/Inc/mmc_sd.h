#ifndef _MMC_SD_H_
#define _MMC_SD_H_
#include "stm32f1xx_hal.h"

#define SD_TYPE_ERR 0X00
#define SD_TYPE_MMC 0X01
#define SD_TYPE_V1 0X02
#define SD_TYPE_V2 0X04
#define SD_TYPE_V2HC 0X06

#define CMD0 0
#define CMD1 1
#define CMD8 8
#define CMD9 9
#define CMD10 10
#define CMD12 12
#define CMD16 16
#define CMD17 17
#define CMD18 18
#define CMD23 23
#define CMD24 24
#define CMD25 25
#define CMD41 41
#define CMD55 55
#define CMD58 58
#define CMD59 59

#define MSD_DATA_OK 0x05
#define MSD_DATA_CRC_ERROR 0x0B
#define MSD_DATA_WRITE_ERROR 0x0D
#define MSD_DATA_OTHER_ERROR 0xFF

#define MSD_RESPONSE_NO_ERROR 0x00
#define MSD_IN_IDLE_STATE 0x01
#define MSD_ERASE_RESET 0x02
#define MSD_ILLEGAL_COMMAND 0x04
#define MSD_COM_CRC_ERROR 0x08
#define MSD_ERASE_SEQUENCE_ERROR 0x10
#define MSD_ADDRESS_ERROR 0x20
#define MSD_PARAMETER_ERROR 0x40
#define MSD_RESPONSE_FAILURE 0xFF

#define SD_CS_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET)
#define SD_CS_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET)

	extern uint8_t SD_Type;

uint8_t SD_SPI_ReadWriteByte(uint8_t data);
void SD_SPI_SpeedLow(void);
void SD_SPI_SpeedHigh(void);
uint8_t SD_WaitReady(void);
uint8_t SD_GetResponse(uint8_t Response);
uint8_t SD_Init(void);
uint8_t SD_ReadDisk(uint8_t *buf, uint32_t sector, uint8_t cnt);
uint8_t SD_WriteDisk(uint8_t *buf, uint32_t sector, uint8_t cnt);
uint32_t SD_GetSectorCount(void);
uint8_t SD_GetCID(uint8_t *cid_data);
uint8_t SD_GetCSD(uint8_t *csd_data);

#endif
