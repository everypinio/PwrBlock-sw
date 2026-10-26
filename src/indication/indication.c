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

#include "indication.h"

#include "bsp/drivers/led/led.h"

#include <stdbool.h>

typedef enum
{
    INDICATION_EFFECT_OFF,
    INDICATION_EFFECT_STATIC,
    INDICATION_EFFECT_BLINK,
} IndicationEffect;

typedef struct
{
    IndicationEffect effect;
    LedColor         color;
    uint32_t         tick_count;
    uint16_t         period;
    bool             is_on;
} IndicationLed;

static IndicationLed leds[LED_NUM];

void indication_init(void)
{
    led_init();

    for (uint32_t i = 0; i < LED_NUM; i++) {
        indication_set_state(i, INDICATION_STATE_OFF);
    }
}

void indication_set_state(LedName name, IndicationState state)
{
    switch (state) {
        case INDICATION_STATE_OFF: {
            leds[name].color      = LED_COLOR_OFF;
            leds[name].effect     = INDICATION_EFFECT_OFF;
            leds[name].period     = 0;
            leds[name].tick_count = 0;
            break;
        }
        case INDICATION_STATE_NORM: {
            leds[name].color      = LED_COLOR_MAGENTA;
            leds[name].effect     = INDICATION_EFFECT_STATIC;
            leds[name].period     = 0;
            leds[name].tick_count = 0;
            break;
        }
        case INDICATION_STATE_ACT: {
            leds[name].color      = LED_COLOR_MAGENTA;
            leds[name].effect     = INDICATION_EFFECT_BLINK;
            leds[name].period     = 1000;
            leds[name].tick_count = 0;
            break;
        }
        case INDICATION_STATE_CONSTRAIN: {
            leds[name].color      = LED_COLOR_YELLOW;
            leds[name].effect     = INDICATION_EFFECT_STATIC;
            leds[name].period     = 0;
            leds[name].tick_count = 0;
            break;
        }
        case INDICATION_STATE_WARNING: {
            leds[name].color      = LED_COLOR_YELLOW;
            leds[name].effect     = INDICATION_EFFECT_BLINK;
            leds[name].period     = 2000;
            leds[name].tick_count = 0;
            break;
        }
        case INDICATION_STATE_ALARM: {
            leds[name].color      = LED_COLOR_RED;
            leds[name].effect     = INDICATION_EFFECT_BLINK;
            leds[name].period     = 500;
            leds[name].tick_count = 0;
            break;
        }
        default: {
            break;
        }
    }
}

void indication_update(uint32_t ticks)
{
    for (uint32_t i = 0; i < LED_NUM; i++) {
        IndicationLed* led_i = &leds[i];

        switch (led_i->effect) {
            case INDICATION_EFFECT_OFF:
            case INDICATION_EFFECT_STATIC: {
                led_set(i, led_i->color);
                break;
            }
            case INDICATION_EFFECT_BLINK: {
                led_i->tick_count += ticks;

                if (led_i->tick_count >= (led_i->period / 2)) {
                    led_i->is_on      = !led_i->is_on;
                    led_i->tick_count = 0;
                }
                led_set(i, led_i->is_on ? led_i->color : LED_COLOR_OFF);
                break;
            }
        }
    }
}
