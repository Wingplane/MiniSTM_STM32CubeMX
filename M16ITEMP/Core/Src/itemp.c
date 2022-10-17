#include "adc.h"
#include "itemp.h"

uint16_t Get_Adc(void)   
{
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1,10);
	return (uint16_t)HAL_ADC_GetValue(&hadc1);
}

uint16_t Get_Adc_Average(uint32_t ch,uint8_t times)
{
	uint32_t temp_val[16]={0};
	uint8_t t;
	for(t=0;t<times;t++)
	{
		temp_val[0] += Get_Adc();
		temp_val[1] += Get_Adc();
		HAL_Delay(5);
	}
	return temp_val[ch]/times;
} 

short Get_Temprate(void)
{
	uint32_t adcx;
	short result;
 	double temperate;
	adcx=Get_Adc_Average(1,20);//��ȡ�ڲ��¶ȴ�����ͨ��,10��ȡƽ��
	temperate=(float)adcx*(3.3/4096);				//��ѹֵ 
	temperate=(1.43-temperate)/0.0043+25;			//ת��Ϊ�¶�ֵ 	 
	result=temperate*=100;							//����100��.
	return result;
}
