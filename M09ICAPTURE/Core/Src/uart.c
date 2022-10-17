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
    while ((USART1->SR & 0X40) == 0);
    USART1->DR = (uint8_t)ch;
    return ch;
}

uint8_t aRxBuffer[RXBUFFERSIZE];
uint8_t USART_RX_BUF[USART_REC_LEN];
uint16_t USART_RX_STA = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		if((USART_RX_STA&0x8000) == 0)
		{
			if(USART_RX_STA&0x4000)
			{
				if(aRxBuffer[0]!=0x0a)
					USART_RX_STA = 0;
				else
					USART_RX_STA |= 0x8000;
			}
			else
			{
				if(aRxBuffer[0]!=0x0a)
					USART_RX_STA |= 0x4000;
				else
				{
						USART_RX_BUF[USART_RX_STA&0x3fff] = aRxBuffer[0];
						USART_RX_STA++;
						if(USART_RX_STA>(USART_REC_LEN - 1))
							USART_RX_STA = 0;
				}
			}
		}
	}
}

//串口1中断服务程序
void USART1_IRQHandler(void)                	
{ 
	uint32_t timeout=0;
	HAL_UART_IRQHandler(&huart1);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
	timeout=0;
	while(HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>HAL_MAX_DELAY) break;	
	}
} 
