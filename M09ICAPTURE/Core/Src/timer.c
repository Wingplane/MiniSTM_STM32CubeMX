/**
 * @file : timer.c
 * @brief : timer driver
 * @author : zhang yue email:zhangyue@petalmail.com
 * @version : 1.1
 * @date : 2024-06-24
 */
#include "timer.h"
#include "tim.h"
#include "led.h"

uint8_t TIM5CH1_CAPTURE_STA = 0; // capture status
uint16_t TIM5CH1_CAPTURE_VAL;    // capture value

/**
 * @brief - timer3 init
 * @param  htim: timer handler
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if ((TIM5CH1_CAPTURE_STA & 0X80) == 0) // still not capture the rising edge
  {
    if (TIM5CH1_CAPTURE_STA & 0X40) // capture the falling edge
    {
      if ((TIM5CH1_CAPTURE_STA & 0X3F) == 0X3F) // capture the falling edge
      {
        TIM5CH1_CAPTURE_STA |= 0X80;  // capture the rising edge
        TIM5CH1_CAPTURE_VAL = 0XFFFF; // set the value to the max
      }
      else
        TIM5CH1_CAPTURE_STA++; // increment the capture state
    }
  }
}

/**
 * @brief - timer5 channel1 capture callback
 * @param  htim: timer handler
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if ((TIM5CH1_CAPTURE_STA & 0X80) == 0) // still not capture the rising edge
  {
    if (TIM5CH1_CAPTURE_STA & 0X40) // capture the falling edge
    {
      TIM5CH1_CAPTURE_STA |= 0X80;                                            // capture the rising edge
      TIM5CH1_CAPTURE_VAL = HAL_TIM_ReadCapturedValue(&htim5, TIM_CHANNEL_1); // read the captured value
      TIM_RESET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_1);                       // reset the capture polarity
      TIM_SET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);  // set the capture polarity
    }
    else // still not capture the falling edge
    {
      TIM5CH1_CAPTURE_STA = 0;                                                // reset the capture state
      TIM5CH1_CAPTURE_VAL = 0;                                                // reset the captured value
      TIM5CH1_CAPTURE_STA |= 0X40;                                            // capture the falling edge
      __HAL_TIM_DISABLE(&htim5);                                              // disable the timer
      __HAL_TIM_SET_COUNTER(&htim5, 0);                                       // reset the counter
      TIM_RESET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_1);                       // reset the capture polarity
      TIM_SET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING); // set the capture polarity
      __HAL_TIM_ENABLE(&htim5);                                               // enable the timer
    }
  }
}
