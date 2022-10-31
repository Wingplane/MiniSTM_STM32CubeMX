#include "string.h"
#include "exfuns.h"
#include "fattester.h"
#include "malloc.h"
#include "usart.h"
#include "uart.h"
#include "ff.h"
#include "ff_gen_drv.h"
#define FILE_MAX_TYPE_NUM 7 
#define FILE_MAX_SUBT_NUM 4 

uint8_t *const FILE_TYPE_TBL[FILE_MAX_TYPE_NUM][FILE_MAX_SUBT_NUM] =
	{
		{"BIN"},					   
		{"LRC"},					   
		{"NES", "SMS"},				   
		{"TXT", "C", "H"},			   
		{"WAV", "MP3", "APE", "FLAC"},
		{"BMP", "JPG", "JPEG", "GIF"}, 
		{"AVI"},					  
};

FATFS *fs[_VOLUMES]; 
FIL *file;			 
FIL *ftemp;			 
UINT br, bw;		 
FILINFO fileinfo;	 
DIR dir;			 

uint8_t *fatbuf; 
uint8_t exfuns_init(void)
{
	uint8_t i;
	for (i = 0; i < _VOLUMES; i++)
	{
		fs[i] = (FATFS *)mymalloc(sizeof(FATFS)); 
		if (!fs[i])
			break;
	}
	file = (FIL *)mymalloc(sizeof(FIL));  
	ftemp = (FIL *)mymalloc(sizeof(FIL)); 
	fatbuf = (uint8_t *)mymalloc(512);	  
	if (i == _VOLUMES && file && ftemp && fatbuf)
		return 0; 
	else
		return 1;
}

uint8_t char_upper(uint8_t c)
{
	if (c < 'A')
		return c; 
	if (c >= 'a')
		return c - 0x20;
	else
		return c; 
}

uint8_t f_typetell(uint8_t *fname)
{
	uint8_t tbuf[5];
	uint8_t *attr = '\0'; 
	uint8_t i = 0, j;
	while (i < 250)
	{
		i++;
		if (*fname == '\0')
			break; 
		fname++;
	}
	if (i == 250)
		return 0XFF;		
	for (i = 0; i < 5; i++) 
	{
		fname--;
		if (*fname == '.')
		{
			fname++;
			attr = fname;
			break;
		}
	}
	strcpy((char *)tbuf, (const char *)attr); // copy
	for (i = 0; i < 4; i++)
		tbuf[i] = char_upper(tbuf[i]);		
	for (i = 0; i < FILE_MAX_TYPE_NUM; i++) 
	{
		for (j = 0; j < FILE_MAX_SUBT_NUM; j++) 
		{
			if (*FILE_TYPE_TBL[i][j] == 0)
				break;																
			if (strcmp((const char *)FILE_TYPE_TBL[i][j], (const char *)tbuf) == 0) 
			{
				return (i << 4) | j;
			}
		}
	}
	return 0XFF;
}

uint8_t exf_getfree(uint8_t *drv, uint32_t *total, uint32_t *free)
{
	FATFS *fs1;
	uint8_t res;
	uint32_t fre_clust = 0, fre_sect = 0, tot_sect = 0;
	res = (uint32_t)f_getfree((const TCHAR *)drv, (DWORD *)&fre_clust, &fs1);
	if (res == 0)
	{
		tot_sect = (fs1->n_fatent - 2) * fs1->csize; 
		fre_sect = fre_clust * fs1->csize;			
#if _MAX_SS != 512									 
		tot_sect *= fs1->ssize / 512;
		fre_sect *= fs1->ssize / 512;
#endif
		*total = tot_sect >> 1; 
		*free = fre_sect >> 1;	
	}
	return res;
}

uint8_t exf_copy(uint8_t (*fcpymsg)(uint8_t *pname, uint8_t pct, uint8_t mode), uint8_t *psrc, uint8_t *pdst, uint32_t totsize, uint32_t cpdsize, uint8_t fwmode)
{
	uint8_t res;
	uint16_t br = 0;
	uint16_t bw = 0;
	FIL *fsrc = 0;
	FIL *fdst = 0;
	uint8_t *fbuf = 0;
	uint8_t curpct = 0;
	unsigned long long lcpdsize = cpdsize;
	fsrc = (FIL *)mymalloc(sizeof(FIL)); // ÉêÇëÄÚ´æ
	fdst = (FIL *)mymalloc(sizeof(FIL));
	fbuf = (uint8_t *)mymalloc(8192);
	if (fsrc == NULL || fdst == NULL || fbuf == NULL)
		res = 100; 
	else
	{
		if (fwmode == 0)
			fwmode = FA_CREATE_NEW; 
		else
			fwmode = FA_CREATE_ALWAYS;

		res = f_open(fsrc, (const TCHAR *)psrc, FA_READ | FA_OPEN_EXISTING); 
		if (res == 0)
			res = f_open(fdst, (const TCHAR *)pdst, FA_WRITE | fwmode); 
		if (res == 0)													
		{
			if (totsize == 0)
			{
				totsize = fsrc->fsize;
				lcpdsize = 0;
				curpct = 0;
			}
			else
				curpct = (lcpdsize * 100) / totsize; 
			fcpymsg(psrc, curpct, 0X02);			 
			while (res == 0)						 
			{
				res = f_read(fsrc, fbuf, 8192, (UINT *)&br); 
				if (res || br == 0)
					break;
				res = f_write(fdst, fbuf, (UINT)br, (UINT *)&bw); 
				lcpdsize += bw;
				if (curpct != (lcpdsize * 100) / totsize) 
				{
					curpct = (lcpdsize * 100) / totsize;
					if (fcpymsg(psrc, curpct, 0X02)) 
					{
						res = 0XFF;
						break;
					}
				}
				if (res || bw < br)
					break;
			}
			f_close(fsrc);
			f_close(fdst);
		}
	}
	myfree(fsrc); 
	myfree(fdst);
	myfree(fbuf);
	return res;
}

uint8_t *exf_get_src_dname(uint8_t *dpfn)
{
	uint16_t temp = 0;
	while (*dpfn != 0)
	{
		dpfn++;
		temp++;
	}
	if (temp < 4)
		return 0;
	while ((*dpfn != 0x5c) && (*dpfn != 0x2f))
		dpfn--; 
	return ++dpfn;
}

uint32_t exf_fdsize(uint8_t *fdname)
{
#define MAX_PATHNAME_DEPTH 512 + 1 
	uint8_t res = 0;
	DIR *fddir = 0;		  
	FILINFO *finfo = 0;	  
	uint8_t *pathname = 0;
	uint16_t pathlen = 0;  
	uint32_t fdsize = 0;

	fddir = (DIR *)mymalloc(sizeof(DIR));
	finfo = (FILINFO *)mymalloc(sizeof(FILINFO));
	if (fddir == NULL || finfo == NULL)
		res = 100;
	if (res == 0)
	{
		pathname = mymalloc(MAX_PATHNAME_DEPTH);
		if (pathname == NULL)
			res = 101;
		if (res == 0)
		{
			pathname[0] = 0;
			strcat((char *)pathname, (const char *)fdname);
			res = f_opendir(fddir, (const TCHAR *)fdname);	
			if (res == 0)									
			{
				while (res == 0) 
				{
					res = f_readdir(fddir, finfo); 
					if (res != FR_OK || finfo->fname[0] == 0)
						break; 
					if (finfo->fname[0] == '.')
						continue;			  
					if (finfo->fattrib & 0X10) 
					{
						pathlen = strlen((const char *)pathname);			
						strcat((char *)pathname, (const char *)"/");		  
						strcat((char *)pathname, (const char *)finfo->fname); 
						// printf("\r\nsub folder:%s\r\n",pathname);	
						fdsize += exf_fdsize(pathname); 
						pathname[pathlen] = 0;			
					}
					else
						fdsize += finfo->fsize;
				}
			}
			myfree(pathname);
		}
	}
	myfree(fddir);
	myfree(finfo);
	if (res)
		return 0;
	else
		return fdsize;
}

uint8_t exf_fdcopy(uint8_t (*fcpymsg)(uint8_t *pname, uint8_t pct, uint8_t mode), uint8_t *psrc, uint8_t *pdst, uint32_t *totsize, uint32_t *cpdsize, uint8_t fwmode)
{
#define MAX_PATHNAME_DEPTH 512 + 1 
	uint8_t res = 0;
	DIR *srcdir = 0;	
	DIR *dstdir = 0;	
	FILINFO *finfo = 0; 
	uint8_t *fn = 0;	

	uint8_t *dstpathname = 0; 
	uint8_t *srcpathname = 0; 

	uint16_t dstpathlen = 0; 
	uint16_t srcpathlen = 0; 

	srcdir = (DIR *)mymalloc(sizeof(DIR)); 
	dstdir = (DIR *)mymalloc(sizeof(DIR));
	finfo = (FILINFO *)mymalloc(sizeof(FILINFO));

	if (srcdir == NULL || dstdir == NULL || finfo == NULL)
		res = 100;
	if (res == 0)
	{
		dstpathname = mymalloc(MAX_PATHNAME_DEPTH);
		srcpathname = mymalloc(MAX_PATHNAME_DEPTH);
		if (dstpathname == NULL || srcpathname == NULL)
			res = 101;
		if (res == 0)
		{
			dstpathname[0] = 0;
			srcpathname[0] = 0;
			strcat((char *)srcpathname, (const char *)psrc); 
			strcat((char *)dstpathname, (const char *)pdst); 
			res = f_opendir(srcdir, (const TCHAR *)psrc);	 
			if (res == 0)									
			{
				strcat((char *)dstpathname, (const char *)"/"); 
				fn = exf_get_src_dname(psrc);
				if (fn == 0) 
				{
					dstpathlen = strlen((const char *)dstpathname);
					dstpathname[dstpathlen] = psrc[0]; 
					dstpathname[dstpathlen + 1] = 0;  
				}
				else
					strcat((char *)dstpathname, (const char *)fn);
				fcpymsg(fn, 0, 0X04);							   
				res = f_mkdir((const TCHAR *)dstpathname);		  
				if (res == FR_EXIST)
					res = 0;
				while (res == 0) 
				{
					res = f_readdir(srcdir, finfo); 
					if (res != FR_OK || finfo->fname[0] == 0)
						break; 
					if (finfo->fname[0] == '.')
						continue;								
					fn = (uint8_t *)finfo->fname;					
					dstpathlen = strlen((const char *)dstpathname); 
					srcpathlen = strlen((const char *)srcpathname); 

					strcat((char *)srcpathname, (const char *)"/"); 
					if (finfo->fattrib & 0X10)					
					{
						strcat((char *)srcpathname, (const char *)fn);								  
						res = exf_fdcopy(fcpymsg, srcpathname, dstpathname, totsize, cpdsize, fwmode); 
					}
					else 
					{
						strcat((char *)dstpathname, (const char *)"/");								 
						strcat((char *)dstpathname, (const char *)fn);								  
						strcat((char *)srcpathname, (const char *)fn);								  
						fcpymsg(fn, 0, 0X01);														   
						res = exf_copy(fcpymsg, srcpathname, dstpathname, *totsize, *cpdsize, fwmode); 
						*cpdsize += finfo->fsize;													  
					}
					srcpathname[srcpathlen] = 0;
					dstpathname[dstpathlen] = 0; 
				}
			}
			myfree(dstpathname);
			myfree(srcpathname);
		}
	}
	myfree(srcdir);
	myfree(dstdir);
	myfree(finfo);
	return res;
}
