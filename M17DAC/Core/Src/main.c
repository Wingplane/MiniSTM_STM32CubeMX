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
#include "adc.h"
#include "dac.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "uart.h"
#include "usmart.h"
#include "ad.h"
#include "da.h"
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
	uint16_t adcx;
	float temp;
	uint8_t t = 0;
	uint16_t dacval = 0;
	uint8_t key;
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
	MX_ADC1_Init();
	MX_DAC_Init();
	/* USER CODE BEGIN 2 */
	LCD_Init();

	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	HAL_NVIC_SetPriority(TIM4_IRQn, 9, 0);
	HAL_TIM_Base_Start_IT(&htim4);

	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_SetPriority(USART1_IRQn, 9, 0);
	HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE);

	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);

	POINT_COLOR = RED;
	LCD_ShowString(30, 50, 200, 16, 16, "Mini STM32");
	LCD_ShowString(30, 70, 200, 16, 16, "DAC TEST");
	LCD_ShowString(30, 90, 200, 16, 16, "ATOM@ALIENTEK");
	LCD_ShowString(30, 110, 200, 16, 16, "2019/11/15");
	LCD_ShowString(30, 130, 200, 16, 16, "WK_UP:+  KEY1:-");
	POINT_COLOR = BLUE;
	LCD_ShowString(30, 150, 200, 16, 16, "DAC VAL:");
	LCD_ShowString(30, 170, 200, 16, 16, "DAC VOL:0.000V");
	LCD_ShowString(30, 190, 200, 16, 16, "ADC VOL:0.000V");

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		t++;
		key = KEY_Scan(0);
		if (key == WKUP_PRES)
		{
			if (dacval < 4000)
				dacval += 200;
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacval); //����DACֵ
		}
		else if (key == 2)
		{
			if (dacval > 200)
				dacval -= 200;
			else
				dacval = 0;
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacval); //����DACֵ
		}
		if (t == 10 || key == KEY1_PRES || key == WKUP_PRES) // WKUP/KEY1������,���߶�ʱʱ�䵽��
		{
			adcx = HAL_DAC_GetValue(&hdac, DAC_CHANNEL_1); //��ȡǰ������DAC��ֵ
			LCD_ShowxNum(94, 150, adcx, 4, 16, 0);		   //��ʾDAC�Ĵ���ֵ
			temp = (float)adcx * (3.3 / 4096);			   //�õ�DAC��ѹֵ
			adcx = temp;
			LCD_ShowxNum(94, 170, temp, 1, 16, 0); //��ʾ��ѹֵ��������
			temp -= adcx;
			temp *= 1000;
			LCD_ShowxNum(110, 170, temp, 3, 16, 0X80); //��ʾ��ѹֵ��С������
			adcx = Get_Adc_Average(ADC_CHANNEL_1, 10); //�õ�ADCת��ֵ
			temp = (float)adcx * (3.3 / 4096);		   //�õ�ADC��ѹֵ
			adcx = temp;
			LCD_ShowxNum(94, 190, temp, 1, 16, 0); //��ʾ��ѹֵ��������
			temp -= adcx;
			temp *= 1000;
			LCD_ShowxNum(110, 190, temp, 3, 16, 0X80); //��ʾ��ѹֵ��С������
			LED0 = !LED0;
			t = 0;
		}
		HAL_Delay(10);
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
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
