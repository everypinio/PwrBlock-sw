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

#pragma once

#include <stdbool.h>
#include <stdint.h>

/// Number of GPIO pins IO1 - IO7 exposed on the external connector. Pin <n>
/// of the DIGital subsystem corresponds to the IO<n> label on the connector.
#define DIGITAL_PIN_COUNT 7

typedef enum
{
    DIGITAL_DIRECTION_INPUT = 0,
    DIGITAL_DIRECTION_OUTPUT,
} DigitalDirection;

typedef enum
{
    DIGITAL_MODE_PUSHPULL = 0,
    DIGITAL_MODE_ODRAIN,
} DigitalMode;

typedef enum
{
    DIGITAL_PULL_NONE = 0,
    DIGITAL_PULL_UP,
    DIGITAL_PULL_DOWN,
} DigitalPull;

/**
 * @brief Init the GPIO subsystem of the external connector.
 *
 * Enables the port clocks of IO1 - IO7 and applies the power-on defaults.
 * Must be called before the SCPI parser starts to serve DIGital commands.
 *
 * The configuration calls of this module do a read-modify-write of the port
 * registers.
 */
void digital_gpio_init(void);

/**
 * @brief Restore the power-on defaults of all pins, used by *RST.
 *
 * Every pin becomes an input without a pull resistor, with a push-pull
 * driver and a stored output level of 0.
 */
void digital_gpio_reset(void);

/**
 * @brief Set up direction of a pin.
 *
 * Switching a pin to output drives the level stored by
 * digital_gpio_level_set().
 *
 * @param pin Pin number, 1 to DIGITAL_PIN_COUNT.
 * @param direction Direction to apply.
 */
void digital_gpio_direction_set(uint32_t pin, DigitalDirection direction);

/**
 * @brief Get direction of a pin.
 *
 * @param pin Pin number, 1 to DIGITAL_PIN_COUNT.
 * @return DigitalDirection Current direction of the pin.
 */
DigitalDirection digital_gpio_direction_get(uint32_t pin);

/**
 * @brief Set up output driver type of a pin.
 *
 * @param pin Pin number, 1 to DIGITAL_PIN_COUNT.
 * @param mode Output driver type to apply.
 */
void digital_gpio_mode_set(uint32_t pin, DigitalMode mode);

/**
 * @brief Get output driver type of a pin.
 *
 * @param pin Pin number, 1 to DIGITAL_PIN_COUNT.
 * @return DigitalMode Current output driver type of the pin.
 */
DigitalMode digital_gpio_mode_get(uint32_t pin);

/**
 * @brief Set up internal pull resistor of a pin.
 *
 * @param pin Pin number, 1 to DIGITAL_PIN_COUNT.
 * @param pull Pull resistor to apply.
 */
void digital_gpio_pull_set(uint32_t pin, DigitalPull pull);

/**
 * @brief Get internal pull resistor of a pin.
 *
 * @param pin Pin number, 1 to DIGITAL_PIN_COUNT.
 * @return DigitalPull Current pull resistor of the pin.
 */
DigitalPull digital_gpio_pull_get(uint32_t pin);

/**
 * @brief Set up output level of a pin.
 *
 * For an input pin the level is only stored and is applied when the pin is
 * switched to output.
 *
 * @param pin Pin number, 1 to DIGITAL_PIN_COUNT.
 * @param level Level to apply.
 */
void digital_gpio_level_set(uint32_t pin, bool level);

/**
 * @brief Get the actual level on a pin, for both input and output pins.
 *
 * @param pin Pin number, 1 to DIGITAL_PIN_COUNT.
 * @return bool Level read from the pin.
 */
bool digital_gpio_level_get(uint32_t pin);
