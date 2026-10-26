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

#include "led.h"

#include "bsp/drivers/ws2812b/ws2812b.h"

#define LED_BRIGHTNESS_RATIO (0.25F)

void led_init(void)
{
    ws2812b_init();
}

void led_set(LedName name, LedColor color)
{
    Ws2812Color ws_color;
    switch (color) {
        case LED_COLOR_OFF: {
            ws_color.red   = 0;
            ws_color.green = 0;
            ws_color.blue  = 0;
            break;
        }
        case LED_COLOR_RED: {
            ws_color.red   = (uint8_t)(0xFF * LED_BRIGHTNESS_RATIO);
            ws_color.green = 0;
            ws_color.blue  = 0;
            break;
        }
        case LED_COLOR_GREEN: {
            ws_color.red   = 0;
            ws_color.green = (uint8_t)(0xFF * LED_BRIGHTNESS_RATIO);
            ws_color.blue  = 0;
            break;
        }
        case LED_COLOR_BLUE: {
            ws_color.red   = 0;
            ws_color.green = 0;
            ws_color.blue  = (uint8_t)(0xFF * LED_BRIGHTNESS_RATIO);
            break;
        }
        case LED_COLOR_YELLOW: {
            ws_color.red   = (uint8_t)(0xFF * LED_BRIGHTNESS_RATIO);
            ws_color.green = (uint8_t)(0xFF * LED_BRIGHTNESS_RATIO);
            ws_color.blue  = 0;
            break;
        }
        case LED_COLOR_MAGENTA: {
            ws_color.red   = (uint8_t)(0xE1 * LED_BRIGHTNESS_RATIO);
            ws_color.green = (uint8_t)(0x60 * LED_BRIGHTNESS_RATIO);
            ws_color.blue  = (uint8_t)(0xF6 * LED_BRIGHTNESS_RATIO);
            break;
        }
    }

    ws2812b_set_led(name, &ws_color);
}
