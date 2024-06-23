/**
 * @file : uart.c
 * @brief : UART driver
 * @author : zhang yue email:zhangyue@petalmail.com
 * @version : 1.0
 * @date : 2024-06-23
 */
#include "uart.h"
#include "usart.h"

#pragma import(__use_no_semihosting)

struct __FILE
{
	int handle;
};

FILE __stdout;

void _sys_exit(int x)
{
	x = x;
}

int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0)
		;
	USART1->DR = (uint8_t)ch;
	return ch;
}

uint8_t aRxBuffer[RXBUFFERSIZE];
uint8_t USART_RX_BUF[USART_REC_LEN];
uint16_t USART_RX_STA = 0;

/**
 * @brief : USART1 init function
 */
void USART1_IRQHandler(void)
{
	uint8_t Res;

	if ((__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET)) // Receive not empty
	{
		Res = USART1->DR;
		if ((USART_RX_STA & 0x8000) == 0) // Receive not completed
		{
			if (USART_RX_STA & 0x4000) // Received 0X0D
			{
				if (Res != 0x0a)
					USART_RX_STA = 0; // Received error, restart receiving
				else
					USART_RX_STA |= 0x8000; // Received 0X0A, receive completed
			}
			else // Not received 0X0D
			{
				if (Res == 0x0d)
					USART_RX_STA |= 0x4000; // Received 0X0D
				else
				{
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res; // Save received data
					USART_RX_STA++; // Increase received data counter
					if (USART_RX_STA > (USART_REC_LEN - 1))
						USART_RX_STA = 0; // Received error, restart receiving
				}
			}
		}
	}
	HAL_UART_IRQHandler(&huart1); // Call HAL library function
}
