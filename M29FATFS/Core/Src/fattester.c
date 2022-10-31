#include "fattester.h"
#include "MMC_SD.h"
#include "usart.h"
#include "uart.h"
#include "exfuns.h"
#include "malloc.h"
#include "ff.h"
#include "string.h"


uint8_t mf_mount(uint8_t *path, uint8_t mt)
{
	return f_mount(fs[1], (const TCHAR *)path, mt);
}

uint8_t mf_open(uint8_t *path, uint8_t mode)
{
	uint8_t res;
	printf("mode=%#x\r\n", mode);
	res = f_open(file, (const TCHAR *)path, mode); // 打开文件夹
	return res;
}

uint8_t mf_close(void)
{
	f_close(file);
	return 0;
}

uint8_t mf_read(uint16_t len)
{
	uint16_t i, t;
	uint8_t res = 0;
	uint16_t tlen = 0;
	printf("\r\nRead file data is:\r\n");
	for (i = 0; i < len / 512; i++)
	{
		res = f_read(file, fatbuf, 512, &br);
		if (res)
		{
			printf("Read Error:%d\r\n", res);
			break;
		}
		else
		{
			tlen += br;
			for (t = 0; t < br; t++)
				printf("%c", fatbuf[t]);
		}
	}
	if (len % 512)
	{
		res = f_read(file, fatbuf, len % 512, &br);
		if (res) 
		{
			printf("\r\nRead Error:%d\r\n", res);
		}
		else
		{
			tlen += br;
			for (t = 0; t < br; t++)
				printf("%c", fatbuf[t]);
		}
	}
	if (tlen)
		printf("\r\nReaded data len:%d\r\n", tlen); 
	printf("Read data over\r\n");
	return res;
}

uint8_t mf_write(uint8_t *dat, uint16_t len)
{
	uint8_t res;

	printf("\r\nBegin Write file...\r\n");
	printf("Write data len:%d\r\n", len);
	res = f_write(file, dat, len, &bw);
	if (res)
	{
		printf("Write Error:%d\r\n", res);
	}
	else
		printf("Writed data len:%d\r\n", bw);
	printf("Write data over.\r\n");
	return res;
}

uint8_t mf_opendir(uint8_t *path)
{
	return f_opendir(&dir, (const TCHAR *)path);
}

uint8_t mf_closedir(void)
{
	return f_closedir(&dir);
}

uint8_t mf_readdir(void)
{
	uint8_t res;
	res = f_readdir(&dir, &fileinfo); 
	if (res != FR_OK)
		return res; 
	printf("\r\n DIR info:\r\n");

	return 0;
}

uint8_t mf_scan_files(uint8_t *path)
{
	FRESULT res;
	res = f_opendir(&dir, (const TCHAR *)path); 
	if (res == FR_OK)
	{
		printf("\r\n");
		while (1)
		{
			res = f_readdir(&dir, &fileinfo); 
			if (res != FR_OK || fileinfo.fname[0] == 0)
				break; 
			// if (fileinfo.fname[0] == '.') continue;             
			printf("%s/", path);			 
			printf("%s\r\n", fileinfo.fname); 
		}
	}
	return res;
}

uint32_t mf_showfree(uint8_t *drv)
{
	FATFS *fs1;
	uint8_t res;
	uint32_t fre_clust = 0, fre_sect = 0, tot_sect = 0;
	res = f_getfree((const TCHAR *)drv, (DWORD *)&fre_clust, &fs1);
	if (res == 0)
	{
		tot_sect = (fs1->n_fatent - 2) * fs1->csize; 
		fre_sect = fre_clust * fs1->csize;			
#if _MAX_SS != 512
		tot_sect *= fs1->ssize / 512;
		fre_sect *= fs1->ssize / 512;
#endif
		if (tot_sect < 20480) 
		{
			/* Print free space in unit of KB (assuming 512 bytes/sector) */
			printf("\r\n磁盘总容量:%d KB\r\n"
				   "可用空间:%d KB\r\n",
				   tot_sect >> 1, fre_sect >> 1);
		}
		else
		{
			/* Print free space in unit of KB (assuming 512 bytes/sector) */
			printf("\r\n磁盘总容量:%d MB\r\n"
				   "可用空间:%d MB\r\n",
				   tot_sect >> 11, fre_sect >> 11);
		}
	}
	return fre_sect;
}

uint8_t mf_lseek(uint32_t offset)
{
	return f_lseek(file, offset);
}

uint32_t mf_tell(void)
{
	return f_tell(file);
}

uint32_t mf_size(void)
{
	return f_size(file);
}

uint8_t mf_mkdir(uint8_t *pname)
{
	return f_mkdir((const TCHAR *)pname);
}

uint8_t mf_fmkfs(uint8_t *path, uint8_t mode, uint16_t au)
{
	return f_mkfs((const TCHAR *)path, mode, au); 
}

uint8_t mf_unlink(uint8_t *pname)
{
	return f_unlink((const TCHAR *)pname);
}

uint8_t mf_rename(uint8_t *oldname, uint8_t *newname)
{
	return f_rename((const TCHAR *)oldname, (const TCHAR *)newname);
}

void mf_getlabel(uint8_t *path)
{
	uint8_t buf[20];
	uint32_t sn = 0;
	uint8_t res;
	res = f_getlabel((const TCHAR *)path, (TCHAR *)buf, (DWORD *)&sn);
	if (res == FR_OK)
	{
		printf("\r\n磁盘%s 的盘符为:%s\r\n", path, buf);
		printf("磁盘%s 的序列号:%X\r\n\r\n", path, sn);
	}
	else
		printf("\r\n获取失败，错误码:%X\r\n", res);
}

void mf_setlabel(uint8_t *path)
{
	uint8_t res;
	res = f_setlabel((const TCHAR *)path);
	if (res == FR_OK)
	{
		printf("\r\n磁盘盘符设置成功:%s\r\n", path);
	}
	else
		printf("\r\n磁盘盘符设置失败，错误码:%X\r\n", res);
}

void mf_gets(uint16_t size)
{
	TCHAR *rbuf;
	rbuf = f_gets((TCHAR *)fatbuf, size, file);
	if (*rbuf == 0)
		return; 
	else
	{
		printf("\r\nThe String Readed Is:%s\r\n", rbuf);
	}
}

uint8_t mf_putc(uint8_t c)
{
	return f_putc((TCHAR)c, file);
}

uint8_t mf_puts(uint8_t *c)
{
	return f_puts((TCHAR *)c, file);
}
