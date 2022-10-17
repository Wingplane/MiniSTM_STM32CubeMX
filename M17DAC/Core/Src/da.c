#include "dac.h"
#include "da.h"

void DAC1_Set_Vol(uint16_t vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
  HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,temp);//12位右对齐数据格式设置DAC值
}
