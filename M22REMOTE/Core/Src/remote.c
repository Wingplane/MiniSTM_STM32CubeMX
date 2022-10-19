#include "remote.h"
#include "tim.h"

uint8_t RmtSta = 0;
uint16_t Dval;
uint32_t RmtRec = 0;
uint8_t RmtCnt = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM5)
	{
		if (RmtSta & 0x80)
		{
			RmtSta &= ~0X10;
			if ((RmtSta & 0X0F) == 0X00)
				RmtSta |= 1 << 6;
			if ((RmtSta & 0X0F) < 14)
				RmtSta++;
			else
			{
				RmtSta &= ~(1 << 7);
				RmtSta &= 0XF0;
			}
		}
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM5)
	{
		if (RDATA_R)
		{
			TIM_RESET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_2);
			TIM_SET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_2, TIM_ICPOLARITY_FALLING);
			__HAL_TIM_SET_COUNTER(&htim5, 0);
			RmtSta |= 0X10;
		}
		else
		{
			Dval = HAL_TIM_ReadCapturedValue(&htim5, TIM_CHANNEL_2);
			TIM_RESET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_2);
			TIM_SET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_2, TIM_ICPOLARITY_RISING);
			if (RmtSta & 0X10)
			{
				if (RmtSta & 0X80)
				{

					if (Dval > 300 && Dval < 800)
					{
						RmtRec <<= 1;
						RmtRec |= 0;
					}
					else if (Dval > 1400 && Dval < 1800)
					{
						RmtRec <<= 1;
						RmtRec |= 1;
					}
					else if (Dval > 2200 && Dval < 2600)
					{
						RmtCnt++;
						RmtSta &= 0XF0;
					}
				}
				else if (Dval > 4200 && Dval < 4700)
				{
					RmtSta |= 1 << 7;
					RmtCnt = 0;
				}
			}
			RmtSta &= ~(1 << 4);
		}
	}
}

uint8_t Remote_Scan(void)
{
	uint8_t sta = 0;
	uint8_t t1, t2;
	if (RmtSta & (1 << 6))
	{
		t1 = RmtRec >> 24;
		t2 = (RmtRec >> 16) & 0xff;
		if ((t1 == (uint8_t)~t2) && t1 == REMOTE_ID)
		{
			t1 = RmtRec >> 8;
			t2 = RmtRec;
			if (t1 == (uint8_t)~t2)
				sta = t1;
		}
		if ((sta == 0) || ((RmtSta & 0X80) == 0))
		{
			RmtSta &= ~(1 << 6);
			RmtCnt = 0;
		}
	}
	return sta;
}
