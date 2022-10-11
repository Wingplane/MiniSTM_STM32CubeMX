#include "rtc.h"
#include "rt.h"
#include "usart.h"
#include "uart.h"
#include "led.h"


RTC_DateTypeDef GetDate;  //获取日期结构体
RTC_TimeTypeDef GetTime;   //获取时间结构体
void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc)
{
	HAL_RTC_GetTime(hrtc,&GetTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(hrtc,&GetDate,RTC_FORMAT_BIN);
	LED1=!LED1;
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	HAL_RTC_GetTime(hrtc,&GetTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(hrtc,&GetDate,RTC_FORMAT_BIN);
	printf("ALARM A!\r\n");
}


void RTC_Alarm_Set(uint8_t hour,uint8_t min,uint8_t sec)
{
	RTC_AlarmTypeDef RTC_AlarmSturuct;
    
    RTC_AlarmSturuct.AlarmTime.Hours=hour;  			//小时
    RTC_AlarmSturuct.AlarmTime.Minutes=min;				//分钟
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; 			//秒
    RTC_AlarmSturuct.Alarm=RTC_ALARM_A;     			//闹钟A
    HAL_RTC_SetAlarm_IT(&hrtc,&RTC_AlarmSturuct,RTC_FORMAT_BIN);
    
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,0x01,0x02); 	//抢占优先级1,子优先级2
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

void RTC_Time_Set(uint8_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_TimeStructure.Hours=hour;
	RTC_TimeStructure.Minutes=min;
	RTC_TimeStructure.Seconds=sec;
	
	RTC_DateTypeDef RTC_DateStructure;
	RTC_DateStructure.Date=day;
	RTC_DateStructure.Month=month;
	RTC_DateStructure.Year=year;
	
	HAL_RTC_SetTime(&hrtc,&RTC_TimeStructure,RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc,&RTC_DateStructure,RTC_FORMAT_BIN);
}
