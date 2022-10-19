#ifndef __USMART_STR_H
#define __USMART_STR_H	 
#include "stm32f1xx_hal.h"
uint8_t usmart_get_parmpos(uint8_t num);						//�õ�ĳ�������ڲ������������ʼλ��
uint8_t usmart_strcmp(uint8_t*str1,uint8_t *str2);					//�Ա������ַ����Ƿ����
uint32_t usmart_pow(uint8_t m,uint8_t n);							//M^N�η�
uint8_t usmart_str2num(uint8_t*str,uint32_t *res);					//�ַ���תΪ����
uint8_t usmart_get_cmdname(uint8_t*str,uint8_t*cmdname,uint8_t *nlen,uint8_t maxlen);//��str�еõ�ָ����,������ָ���
uint8_t usmart_get_fname(uint8_t*str,uint8_t*fname,uint8_t *pnum,uint8_t *rval);		//��str�еõ�������
uint8_t usmart_get_aparm(uint8_t *str,uint8_t *fparm,uint8_t *ptype); 	//��str�еõ�һ����������
uint8_t usmart_get_fparam(uint8_t*str,uint8_t *parn);  			//�õ�str�����еĺ�������.
#endif











