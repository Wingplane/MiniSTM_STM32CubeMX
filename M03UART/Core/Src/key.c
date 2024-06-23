/**
 * @file : key.c
 * @brief : key driver
 * @author : zhang yue email:zhangyue@petalmail.com
 * @version : 1.0
 * @date : 2024-06-23
 */
#include "key.h"

/**
 * @brief : KEY_Init
 * @param  mode: 0:GPIO input
 * @param  mode: 1:EXTI input
 * @retval KEY0: key0
 * @retval KEY1: key1
 * @retval WK_UP: wakeup
 * @retval 0: no key pressed
 */
uint8_t KEY_Scan(uint8_t mode)
{
    static uint8_t key_up = 1;
    if (mode == 1)
    {
        key_up = 1;
    }

    if (key_up && (KEY0 == 0 || KEY1 == 0 || WK_UP == 1))
    {
        HAL_Delay(10);
        key_up = 0;
        if (KEY0 == 0)
        {
            return KEY0_PRES;
        }
        else if (KEY1 == 0)
        {
            return KEY1_PRES;
        }
        else if (WK_UP == 1)
        {
            return WKUP_PRES;
        }
    }
    else if (KEY0 == 1 && KEY1 == 1 && WK_UP == 0)
    {
        key_up = 1;
    }
    return 0;
}
