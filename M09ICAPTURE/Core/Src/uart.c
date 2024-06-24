/**
 * @file : uart.c
 * @brief : UART driver
 * @author : zhang yue email:zhangyue@petalmail.com
 * @version : 1.1
 * @date : 2024-06-24
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
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1) // Determine whether it is USART1 interrupt
  {
    if ((USART_RX_STA & 0x8000) == 0) // Determine whether the data is received
    {
      if (USART_RX_STA & 0x4000) // Determine whether the data is overflow
      {
        if (aRxBuffer[0] != 0x0a) // Determine whether the data is '\n'
          USART_RX_STA = 0;       // Reset the data length
        else
          USART_RX_STA |= 0x8000; // Set the received flag
      }
      else
      {
        if (aRxBuffer[0] != 0x0a) // Determine whether the data is '\n'
          USART_RX_STA |= 0x4000; // Set the overflow flag
        else
        {
          USART_RX_BUF[USART_RX_STA & 0x3fff] = aRxBuffer[0]; // Save the data
          USART_RX_STA++;                                     // Increment the data length
          if (USART_RX_STA > (USART_REC_LEN - 1))             // Determine whether the data length is overflow
            USART_RX_STA = 0;                                 // Reset the data length
        }
      }
    }
  }
}

/**
 * @brief : USART1 interrupt function
 */
void USART1_IRQHandler(void)
{
  uint32_t timeout = 0;         // Timeout processing
  HAL_UART_IRQHandler(&huart1); // Call HAL library function
  timeout = 0;
  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY) // Wait for the end of the transmission
  {
    timeout++; // Timeout processing
    if (timeout > HAL_MAX_DELAY)
      break;
  }
  timeout = 0;
  while (HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK) // Start receiving
  {
    timeout++;                   // Timeout processing
    if (timeout > HAL_MAX_DELAY) // Timeout processing
      break;
  }
}
