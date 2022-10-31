/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    user_diskio.c
 * @brief   This file includes a diskio driver skeleton to be completed by the user.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/*
 * Warning: the user section 0 is no more in use (starting from CubeMx version 4.16.0)
 * To be suppressed in the future.
 * Kept to ensure backward compatibility with previous CubeMx versions when
 * migrating projects.
 * User code previously added there should be copied in the new user sections before
 * the section contents can be deleted.
 */
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
#endif

/* USER CODE BEGIN DECL */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"
#include "mmc_sd.h"
#include "w25qxx.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SD_CARD 0  // SD卡,卷标为0
#define EX_FLASH 1 // 外部spi flash,卷标为1
#define FLASH_SECTOR_SIZE 512
#define FLASH_BLOCK_SIZE 8 // 每个BLOCK有8个扇区
/* Private variables ---------------------------------------------------------*/
uint16_t FLASH_SECTOR_COUNT = 9832; // 4.8M字节,默认为W25Q64
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/
DSTATUS USER_initialize(BYTE pdrv);
DSTATUS USER_status(BYTE pdrv);
DRESULT USER_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
DRESULT USER_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
DRESULT USER_ioctl(BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef USER_Driver =
	{
		USER_initialize,
		USER_status,
		USER_read,
#if _USE_WRITE
		USER_write,
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
		USER_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Initializes a Drive
 * @param  pdrv: Physical drive number (0..)
 * @retval DSTATUS: Operation status
 */
DSTATUS USER_initialize(
	BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
	/* USER CODE BEGIN INIT */
	uint8_t res = 0;
	switch (pdrv)
	{
	case SD_CARD:		 // SD卡
		res = SD_Init(); // SD卡初始化
		break;
	case EX_FLASH:	   // 外部flash
		W25QXX_Init(); // W25QXX初始化
		break;
	default:
		res = 1;
	}
	if (res)
		return STA_NOINIT;
	else
		return RES_OK; // 初始化成功
	/* USER CODE END INIT */
}

/**
 * @brief  Gets Disk Status
 * @param  pdrv: Physical drive number (0..)
 * @retval DSTATUS: Operation status
 */
DSTATUS USER_status(
	BYTE pdrv /* Physical drive number to identify the drive */
)
{
	/* USER CODE BEGIN STATUS */
	Stat = RES_OK;
	return Stat;
	/* USER CODE END STATUS */
}

/**
 * @brief  Reads Sector(s)
 * @param  pdrv: Physical drive number (0..)
 * @param  *buff: Data buffer to store read data
 * @param  sector: Sector address (LBA)
 * @param  count: Number of sectors to read (1..128)
 * @retval DRESULT: Operation result
 */
DRESULT USER_read(
	BYTE pdrv,	  /* Physical drive nmuber to identify the drive */
	BYTE *buff,	  /* Data buffer to store read data */
	DWORD sector, /* Sector address in LBA */
	UINT count	  /* Number of sectors to read */
)
{
	/* USER CODE BEGIN READ */
	uint8_t res = 0;
	if (!count)
		return RES_PARERR; // count不能等于0，否则返回参数错误
	switch (pdrv)
	{
	case SD_CARD: // SD卡
		res = SD_ReadDisk(buff, sector, count);
		while (res) // 读出错
		{
			SD_Init(); // 重新初始化SD卡
			res = SD_ReadDisk(buff, sector, count);
			// printf("sd rd error:%d\r\n",res);
		}
		break;
	case EX_FLASH: // 外部flash
		for (; count > 0; count--)
		{
			W25QXX_Read(buff, sector * FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE);
			sector++;
			buff += FLASH_SECTOR_SIZE;
		}
		res = 0;
		break;
	default:
		res = 1;
	}
	// 处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
	if (res == 0x00)
		return RES_OK;
	else
		return RES_ERROR;
	/* USER CODE END READ */
}

/**
 * @brief  Writes Sector(s)
 * @param  pdrv: Physical drive number (0..)
 * @param  *buff: Data to be written
 * @param  sector: Sector address (LBA)
 * @param  count: Number of sectors to write (1..128)
 * @retval DRESULT: Operation result
 */
#if _USE_WRITE == 1
DRESULT USER_write(
	BYTE pdrv,		  /* Physical drive nmuber to identify the drive */
	const BYTE *buff, /* Data to be written */
	DWORD sector,	  /* Sector address in LBA */
	UINT count		  /* Number of sectors to write */
)
{
	/* USER CODE BEGIN WRITE */
	/* USER CODE HERE */
	uint8_t res = 0;
	if (!count)
		return RES_PARERR; // count不能等于0，否则返回参数错误
	switch (pdrv)
	{
	case SD_CARD: // SD卡
		res = SD_WriteDisk((uint8_t *)buff, sector, count);
		while (res) // 写出错
		{
			SD_Init(); // 重新初始化SD卡
			res = SD_WriteDisk((uint8_t *)buff, sector, count);
		}
		break;
	case EX_FLASH: // 外部flash
		for (; count > 0; count--)
		{
			W25QXX_Write((uint8_t *)buff, sector * FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE);
			sector++;
			buff += FLASH_SECTOR_SIZE;
		}
		res = 0;
		break;
	default:
		res = 1;
	}
	// 处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
	if (res == 0x00)
		return RES_OK;
	else
		return RES_ERROR;
	/* USER CODE END WRITE */
}
#endif /* _USE_WRITE == 1 */

/**
 * @brief  I/O control operation
 * @param  pdrv: Physical drive number (0..)
 * @param  cmd: Control code
 * @param  *buff: Buffer to send/receive control data
 * @retval DRESULT: Operation result
 */
#if _USE_IOCTL == 1
DRESULT USER_ioctl(
	BYTE pdrv, /* Physical drive nmuber (0..) */
	BYTE cmd,  /* Control code */
	void *buff /* Buffer to send/receive control data */
)
{
	/* USER CODE BEGIN IOCTL */
	DRESULT res;
	if (pdrv == SD_CARD) // SD卡
	{
		switch (cmd)
		{
		case CTRL_SYNC:
			res = RES_OK;
			break;
		case GET_SECTOR_SIZE:
			*(DWORD *)buff = 512;
			res = RES_OK;
			break;
		case GET_BLOCK_SIZE:
			*(WORD *)buff = 8;
			res = RES_OK;
			break;
		case GET_SECTOR_COUNT:
			*(DWORD *)buff = SD_GetSectorCount();
			res = RES_OK;
			break;
		default:
			res = RES_PARERR;
			break;
		}
	}
	else if (pdrv == EX_FLASH) // 外部FLASH
	{
		switch (cmd)
		{
		case CTRL_SYNC:
			res = RES_OK;
			break;
		case GET_SECTOR_SIZE:
			*(WORD *)buff = FLASH_SECTOR_SIZE;
			res = RES_OK;
			break;
		case GET_BLOCK_SIZE:
			*(WORD *)buff = FLASH_BLOCK_SIZE;
			res = RES_OK;
			break;
		case GET_SECTOR_COUNT:
			*(DWORD *)buff = FLASH_SECTOR_COUNT;
			res = RES_OK;
			break;
		default:
			res = RES_PARERR;
			break;
		}
	}
	else
		res = RES_ERROR; // 其他的不支持
	return res;
	/* USER CODE END IOCTL */
}
#endif /* _USE_IOCTL == 1 */
