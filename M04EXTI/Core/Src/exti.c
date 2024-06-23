/**
 * @file : exti.c
 * @brief : exti driver
 * @author : zhang yue email:zhangyue@petalmail.com
 * @version : 1.0
 * @date : 2024-06-23
 */
#include "exti.h"
#include "key.h"

/**
 * @brief : EXTI_Init
 * @param  GPIO_Pin: GPIO_PIN_0, GPIO_PIN_5, GPIO_PIN_15
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin) // Check which pin has triggered the interrupt
	{
	case GPIO_PIN_0:	// Check if the interrupt is from GPIO_PIN_0
		if (WK_UP == 1) // Check if the key is pressed
		{
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2); // Toggle the LED
		}
		break;
	case GPIO_PIN_5:   // Check if the interrupt is from GPIO_PIN_5
		if (KEY0 == 0) // Check if the key is pressed
		{
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2); // Toggle the LED
		}
		break;
	case GPIO_PIN_15:  // Check if the interrupt is from GPIO_PIN_15
		if (KEY1 == 0) // Check if the key is pressed
		{
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8); // Toggle the LED
		}
		break;
	}
}
