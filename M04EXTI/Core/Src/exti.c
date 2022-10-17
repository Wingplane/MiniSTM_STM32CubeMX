#include "exti.h"
#include "key.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//	HAL_Delay(100); //消抖
	switch (GPIO_Pin)
	{
	case GPIO_PIN_0:
		if (WK_UP == 1)
		{
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2);
		}
		break;
	case GPIO_PIN_5:
		if (KEY0 == 0)
		{
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2);
		}
		break;
	case GPIO_PIN_15:
		if (KEY1 == 0)
		{
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
		}
		break;
	}
}
