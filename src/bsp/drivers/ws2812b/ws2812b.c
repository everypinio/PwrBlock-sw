/*
 * Copyright 2026 Everypin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ws2812b.h"
#include "ws2812b_conf.h"

#include <stm32g0xx_hal.h>

extern TIM_HandleTypeDef WS2812B_TIMER;

static uint8_t ws_buffer[WS2812B_BUFF_SIZE] = {0};

void ws2812b_init(void)
{
    Ws2812Color led_off = {
        .red   = 0,
        .green = 0,
        .blue  = 0,
    };

    for (uint8_t i = 0; i < WS2812B_NUM; i++) {
        ws2812b_set_led(i, &led_off);
    }

    HAL_TIM_PWM_Start_DMA(
        &WS2812B_TIMER,
        WS2812B_TIMER_CH,
        (uint32_t*)ws_buffer,
        WS2812B_BUFF_SIZE
    );
}

void ws2812b_set_led(uint32_t pos, Ws2812Color* color)
{

    uint32_t offset = WS2812B_RESET_SLOTS + pos * 24;

    if (color != NULL) {
        for (uint8_t i = 0; i < 8; i++) {
            ws_buffer[offset + i]      = ((color->green << i) & 0x80) ?
                                             WS2812B_LED_BIT_SET :
                                             WS2812B_LED_BIT_RESET;
            ws_buffer[offset + 8 + i]  = ((color->red << i) & 0x80) ?
                                             WS2812B_LED_BIT_SET :
                                             WS2812B_LED_BIT_RESET;
            ws_buffer[offset + 16 + i] = ((color->blue << i) & 0x80) ?
                                             WS2812B_LED_BIT_SET :
                                             WS2812B_LED_BIT_RESET;
        }
    }
}
