/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "lcd.h"
#include "uart.h"
#include "usmart.h"
#include "remote.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */
	uint8_t key;
	uint8_t t = 0;
	uint8_t *str = 0;
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_TIM4_Init();
	MX_TIM5_Init();
	/* USER CODE BEGIN 2 */
	LCD_Init();

	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	HAL_NVIC_SetPriority(TIM4_IRQn, 9, 0);
	HAL_TIM_Base_Start_IT(&htim4);

	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_SetPriority(USART1_IRQn, 9, 0);
	HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE);

	HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_2);
	__HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE);

	POINT_COLOR = RED;
	LCD_ShowString(30, 50, 200, 16, 16, "Mini STM32");
	LCD_ShowString(30, 70, 200, 16, 16, "REMOTE TEST");
	LCD_ShowString(30, 90, 200, 16, 16, "ATOM@ALIENTEK");
	LCD_ShowString(30, 110, 200, 16, 16, "2019/11/15");
	LCD_ShowString(30, 130, 200, 16, 16, "KEYVAL:");
	LCD_ShowString(30, 150, 200, 16, 16, "KEYCNT:");
	LCD_ShowString(30, 170, 200, 16, 16, "SYMBOL:");
	POINT_COLOR = BLUE;
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		key = Remote_Scan();
		if (key)
		{
			LCD_ShowNum(86, 130, key, 3, 16);
			LCD_ShowNum(86, 150, RmtCnt, 3, 16);
			switch (key)
			{
			case 0:
				str = "ERROR";
				break;
			case 162:
				str = "POWER";
				break;
			case 98:
				str = "UP";
				break;
			case 2:
				str = "PLAY";
				break;
			case 226:
				str = "ALIENTEK";
				break;
			case 194:
				str = "RIGHT";
				break;
			case 34:
				str = "LEFT";
				break;
			case 224:
				str = "VOL-";
				break;
			case 168:
				str = "DOWN";
				break;
			case 144:
				str = "VOL+";
				break;
			case 104:
				str = "1";
				break;
			case 152:
				str = "2";
				break;
			case 176:
				str = "3";
				break;
			case 48:
				str = "4";
				break;
			case 24:
				str = "5";
				break;
			case 122:
				str = "6";
				break;
			case 16:
				str = "7";
				break;
			case 56:
				str = "8";
				break;
			case 90:
				str = "9";
				break;
			case 66:
				str = "0";
				break;
			case 82:
				str = "DELETE";
				break;
			}
			LCD_Fill(86, 170, 116 + 8 * 8, 170 + 16, WHITE);
			LCD_ShowString(86, 170, 200, 16, 16, str);
		}
		else
			HAL_Delay(10);
		t++;
		if (t == 20)
		{
			t = 0;
			LED0 = !LED0;
		}
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
