#include "wdg.h"

void IWDG_Feed(void)
{
	HAL_IWDG_Refresh(&hiwdg);
}
