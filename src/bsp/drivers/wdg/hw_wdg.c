#include "hw_wdg.h"

#include <stm32g0xx_hal.h>


extern IWDG_HandleTypeDef hiwdg;

void hw_wdg_feed(void)
{
    HAL_IWDG_Refresh(&hiwdg);
}
