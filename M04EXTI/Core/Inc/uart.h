#ifndef __UART_H
#define __UART_H
#include "stm32f1xx_hal.h"
#include "stdio.h"

#define USART_REC_LEN 200 //定义最大接收字节数 200
#define RXBUFFERSIZE 1	  //缓存大小

extern uint8_t aRxBuffer[RXBUFFERSIZE];
extern uint8_t USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern uint16_t USART_RX_STA;				//接收状态标记

#endif // !__UART_H
