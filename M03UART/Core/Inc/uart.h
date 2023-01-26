#ifndef __UART_H
#define __UART_H
#include "stm32f1xx_hal.h"
#include "stdio.h"

#define USART_REC_LEN 200
#define RXBUFFERSIZE 1

extern uint8_t aRxBuffer[RXBUFFERSIZE];
extern uint8_t USART_RX_BUF[USART_REC_LEN];
extern uint16_t USART_RX_STA;

#endif // !__UART_H
