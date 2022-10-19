#ifndef __USMART_STR_H
#define __USMART_STR_H	 
#include "stm32f1xx_hal.h"
uint8_t usmart_get_parmpos(uint8_t num);						//得到某个参数在参数列里面的起始位置
uint8_t usmart_strcmp(uint8_t*str1,uint8_t *str2);					//对比两个字符串是否相等
uint32_t usmart_pow(uint8_t m,uint8_t n);							//M^N次方
uint8_t usmart_str2num(uint8_t*str,uint32_t *res);					//字符串转为数字
uint8_t usmart_get_cmdname(uint8_t*str,uint8_t*cmdname,uint8_t *nlen,uint8_t maxlen);//从str中得到指令名,并返回指令长度
uint8_t usmart_get_fname(uint8_t*str,uint8_t*fname,uint8_t *pnum,uint8_t *rval);		//从str中得到函数名
uint8_t usmart_get_aparm(uint8_t *str,uint8_t *fparm,uint8_t *ptype); 	//从str中得到一个函数参数
uint8_t usmart_get_fparam(uint8_t*str,uint8_t *parn);  			//得到str中所有的函数参数.
#endif











