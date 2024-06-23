/**
 * @file : uart.h
 * @brief : UART driver
 * @author : zhang yue email:zhangyue@petalmail.com
 * @version : 1.0
 * @date : 2024-06-23
 */
#ifndef __UART_H
#define __UART_H
#include "stm32f1xx_hal.h"
#include "stdio.h"

#define USART_REC_LEN 200 // Maximum received data length
#define RXBUFFERSIZE 1    // Maximum received data length

extern uint8_t aRxBuffer[RXBUFFERSIZE];     // Received data buffer, maximum RXBUFFERSIZE bytes
extern uint8_t USART_RX_BUF[USART_REC_LEN]; // Received data buffer, maximum USART_REC_LEN bytes, last byte is '\0'
extern uint16_t USART_RX_STA;               // Received data status

#endif // !__UART_H
