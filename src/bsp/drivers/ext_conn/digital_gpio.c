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

#include "digital_gpio.h"

#include <stm32g0xx_hal.h>

/// Port and pin of a GPIO of the external connector
typedef struct
{
    GPIO_TypeDef* port;
    uint16_t      mask;
} DigitalPinHw;

typedef struct
{
    DigitalDirection direction;
    DigitalMode      mode;
    DigitalPull      pull;
    bool             level;
} DigitalPinCfg;

// Pins of the external connector, indexed by pin number - 1.
static const DigitalPinHw PIN_HW[DIGITAL_PIN_COUNT] = {
    {GPIOD, GPIO_PIN_0 }, // IO1
    {GPIOD, GPIO_PIN_1 }, // IO2
    {GPIOD, GPIO_PIN_2 }, // IO3
    {GPIOD, GPIO_PIN_3 }, // IO4
    {GPIOC, GPIO_PIN_6 }, // IO5
    {GPIOC, GPIO_PIN_7 }, // IO6
    {GPIOC, GPIO_PIN_13}, // IO7
};

static DigitalPinCfg pin_cfg[DIGITAL_PIN_COUNT];

// Private function prototypes
static bool digital_pin_index(uint32_t pin, uint32_t* index);
static void digital_pin_drive(uint32_t index, bool level);
static void digital_pin_apply(uint32_t index);

void digital_gpio_init(void)
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    digital_gpio_reset();
}

void digital_gpio_reset(void)
{
    for (uint32_t index = 0; index < DIGITAL_PIN_COUNT; index++) {
        pin_cfg[index].direction = DIGITAL_DIRECTION_INPUT;
        pin_cfg[index].mode      = DIGITAL_MODE_PUSHPULL;
        pin_cfg[index].pull      = DIGITAL_PULL_NONE;
        pin_cfg[index].level     = false;

        digital_pin_apply(index);
    }
}

void digital_gpio_direction_set(uint32_t pin, DigitalDirection direction)
{
    uint32_t index = 0;
    if (!digital_pin_index(pin, &index)) {
        return;
    }

    pin_cfg[index].direction = direction;
    digital_pin_apply(index);
}

DigitalDirection digital_gpio_direction_get(uint32_t pin)
{
    uint32_t index = 0;
    if (!digital_pin_index(pin, &index)) {
        return DIGITAL_DIRECTION_INPUT;
    }

    return pin_cfg[index].direction;
}

void digital_gpio_mode_set(uint32_t pin, DigitalMode mode)
{
    uint32_t index = 0;
    if (!digital_pin_index(pin, &index)) {
        return;
    }

    pin_cfg[index].mode = mode;
    digital_pin_apply(index);
}

DigitalMode digital_gpio_mode_get(uint32_t pin)
{
    uint32_t index = 0;
    if (!digital_pin_index(pin, &index)) {
        return DIGITAL_MODE_PUSHPULL;
    }

    return pin_cfg[index].mode;
}

void digital_gpio_pull_set(uint32_t pin, DigitalPull pull)
{
    uint32_t index = 0;
    if (!digital_pin_index(pin, &index)) {
        return;
    }

    pin_cfg[index].pull = pull;
    digital_pin_apply(index);
}

DigitalPull digital_gpio_pull_get(uint32_t pin)
{
    uint32_t index = 0;
    if (!digital_pin_index(pin, &index)) {
        return DIGITAL_PULL_NONE;
    }

    return pin_cfg[index].pull;
}

void digital_gpio_level_set(uint32_t pin, bool level)
{
    uint32_t index = 0;
    if (!digital_pin_index(pin, &index)) {
        return;
    }

    pin_cfg[index].level = level;
    digital_pin_drive(index, level);
}

bool digital_gpio_level_get(uint32_t pin)
{
    uint32_t index = 0;
    if (!digital_pin_index(pin, &index)) {
        return false;
    }

    return HAL_GPIO_ReadPin(PIN_HW[index].port, PIN_HW[index].mask) ==
           GPIO_PIN_SET;
}

/**
 * @brief Check a pin number and convert it into an index of the pin tables.
 *
 * @param pin Pin number, 1 to DIGITAL_PIN_COUNT.
 * @param index Index of the pin, pin number - 1.
 * @return bool false if the pin number is out of range.
 */
static bool digital_pin_index(uint32_t pin, uint32_t* index)
{
    if (pin < 1 || pin > DIGITAL_PIN_COUNT) {
        return false;
    }

    *index = pin - 1;
    return true;
}

/**
 * @brief Drive the level of a pin.
 *
 * Has no effect on the pin while it is an input, the output register keeps
 * the level until the pin is switched to output.
 *
 * @param index Index of the pin.
 * @param level Level to drive.
 */
static void digital_pin_drive(uint32_t index, bool level)
{
    HAL_GPIO_WritePin(
        PIN_HW[index].port,
        PIN_HW[index].mask,
        level ? GPIO_PIN_SET : GPIO_PIN_RESET
    );
}

/**
 * @brief Apply the stored configuration of a pin to the port registers.
 *
 * @param index Index of the pin.
 */
static void digital_pin_apply(uint32_t index)
{
    const DigitalPinCfg* cfg  = &pin_cfg[index];
    GPIO_InitTypeDef     init = {0};

    init.Pin   = PIN_HW[index].mask;
    init.Speed = GPIO_SPEED_FREQ_LOW;

    switch (cfg->pull) {
        case DIGITAL_PULL_UP: {
            init.Pull = GPIO_PULLUP;
            break;
        }
        case DIGITAL_PULL_DOWN: {
            init.Pull = GPIO_PULLDOWN;
            break;
        }
        case DIGITAL_PULL_NONE:
        default: {
            init.Pull = GPIO_NOPULL;
            break;
        }
    }

    if (cfg->direction == DIGITAL_DIRECTION_OUTPUT) {
        init.Mode = (cfg->mode == DIGITAL_MODE_ODRAIN) ? GPIO_MODE_OUTPUT_OD :
                                                         GPIO_MODE_OUTPUT_PP;
    }
    else {
        init.Mode = GPIO_MODE_INPUT;
    }

    // Load the output register before the mode is applied, so switching a pin
    // to output cannot emit a pulse of the level left by a previous setup
    digital_pin_drive(index, cfg->level);

    HAL_GPIO_Init(PIN_HW[index].port, &init);
}
