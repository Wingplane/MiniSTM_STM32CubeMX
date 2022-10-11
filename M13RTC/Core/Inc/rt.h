
#ifndef __RT_H
#define __RT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


extern RTC_DateTypeDef GetDate;  //��ȡ���ڽṹ��
extern RTC_TimeTypeDef GetTime;   //��ȡʱ��ṹ��

void RTC_Alarm_Set(uint8_t hour,uint8_t min,uint8_t sec);	//��������	
void RTC_Time_Set(uint8_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec);
	
#ifdef __cplusplus
}
#endif

#endif //__RT_H

