#include "usmart.h"
#include "usmart_str.h"
#include "lcd.h"
#include "fattester.h" 
								 
extern void led_set(uint8_t sta);
extern void test_fun(void(*ledset)(uint8_t),uint8_t sta);										  

struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 
	(void*)read_addr,	"uint32_t read_addr(uint32_t addr)",
	(void*)write_addr,	"void write_addr(uint32_t addr,uint32_t val)",	 
#endif		   
 	(void*)HAL_Delay,	"void HAL_Delay(uint32_t Delay)",	 
	(void*)mf_mount,	"uint8_t mf_mount(uint8_t* path,uint8_t mt)", 
	(void*)mf_open,		"uint8_t mf_open(uint8_t*path,uint8_t mode)", 
	(void*)mf_close,	"uint8_t mf_close(void)", 
	(void*)mf_read,		"uint8_t mf_read(uint16_t len)", 
	(void*)mf_write,	"uint8_t mf_write(uint8_t*dat,uint16_t len)", 
	(void*)mf_opendir,	"uint8_t mf_opendir(uint8_t* path)", 
	(void*)mf_closedir,	"uint8_t mf_closedir(void)", 
	(void*)mf_readdir,	"uint8_t mf_readdir(void)", 
	(void*)mf_scan_files,"uint8_t mf_scan_files(uint8_t * path)", 
	(void*)mf_showfree,	"uint32_t mf_showfree(uint8_t *drv)", 
	(void*)mf_lseek,	"uint8_t mf_lseek(uint32_t offset)", 
	(void*)mf_tell,		"uint32_t mf_tell(void)", 
	(void*)mf_size,		"uint32_t mf_size(void)", 
	(void*)mf_mkdir,	"uint8_t mf_mkdir(uint8_t*pname)", 
	(void*)mf_fmkfs,	"uint8_t mf_fmkfs(uint8_t* path,uint8_t mode,uint16_t au)", 
	(void*)mf_unlink,	"uint8_t mf_unlink(uint8_t *pname)", 
	(void*)mf_rename,	"uint8_t mf_rename(uint8_t *oldname,uint8_t* newname)", 
	(void*)mf_getlabel,	"void mf_getlabel(uint8_t *path)", 
	(void*)mf_setlabel,	"void mf_setlabel(uint8_t *path)", 
	(void*)mf_gets,		"void mf_gets(uint16_t size)", 
	(void*)mf_putc,		"uint8_t mf_putc(uint8_t c)", 
	(void*)mf_puts,		"uint8_t mf_puts(uint8_t*c)", 
};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),
	0,	  	
	0,	 	
	1,		
	0,		 
	0,	  
	0,		
};   
