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

#include <scpi/scpi.h>

/**
 * @brief Restore the power-on defaults of all pins.
 *
 * Wraps digital_gpio_reset() so that callers outside the ext_conn driver,
 * e.g. *RST, do not depend on it directly.
 *
 * @param context
 * @return scpi_result_t
 */
scpi_result_t SCPI_DigitalReset(scpi_t* context);

/**
 * [SOURce]:DIGital:COUNt?
 * @brief This query returns the number of GPIO pins.
 *
 * @param context
 * @return scpi_result_t
 */
scpi_result_t SCPI_DigitalCountQ(scpi_t* context);

/**
 * [SOURce]:DIGital:PIN<n>:FUNCtion?
 * @brief This query returns the current owner of the pin. IO1 - IO7 are always
 * GPIO, every bus is wired to its own dedicated pins, so the reply is always
 * GPIO. Kept for forward compatibility.
 *
 * @param context
 * @return scpi_result_t
 */
scpi_result_t SCPI_DigitalPinFunctionQ(scpi_t* context);

/**
 * [SOURce]:DIGital:PIN<n>:DIRection {INPut | OUTPut}
 * @brief Set up direction of the pin. Default is INPut.
 *
 * @param context
 * @return scpi_result_t
 */
scpi_result_t SCPI_DigitalPinDirection(scpi_t* context);

/**
 * [SOURce]:DIGital:PIN<n>:DIRection?
 * @brief This query returns direction of the pin.
 *
 * @param context
 * @return scpi_result_t
 */
scpi_result_t SCPI_DigitalPinDirectionQ(scpi_t* context);

/**
 * [SOURce]:DIGital:PIN<n>:MODE {PUSHpull | ODRain}
 * @brief Set up output driver type of the pin. Default is PUSHpull.
 *
 * @param context
 * @return scpi_result_t
 */
scpi_result_t SCPI_DigitalPinMode(scpi_t* context);

/**
 * [SOURce]:DIGital:PIN<n>:MODE?
 * @brief This query returns output driver type of the pin.
 *
 * @param context
 * @return scpi_result_t
 */
scpi_result_t SCPI_DigitalPinModeQ(scpi_t* context);

/**
 * [SOURce]:DIGital:PIN<n>:PULL {NONE | UP | DOWN}
 * @brief Set up internal pull resistor of the pin. Default is NONE.
 *
 * @param context
 * @return scpi_result_t
 */
scpi_result_t SCPI_DigitalPinPull(scpi_t* context);

/**
 * [SOURce]:DIGital:PIN<n>:PULL?
 * @brief This query returns internal pull resistor of the pin.
 *
 * @param context
 * @return scpi_result_t
 */
scpi_result_t SCPI_DigitalPinPullQ(scpi_t* context);

/**
 * [SOURce]:DIGital:PIN<n>[:LEVel] {OFF | ON | 0 | 1}
 * @brief Set up output level of the pin. For an input pin the value is stored
 * and applied when the pin is switched to output.
 *
 * @param context
 * @return scpi_result_t
 */
scpi_result_t SCPI_DigitalPinLevel(scpi_t* context);

/**
 * [SOURce]:DIGital:PIN<n>[:LEVel]?
 * @brief This query returns actual level on the pin, for both input and
 * output pins.
 *
 * @param context
 * @return scpi_result_t
 */
scpi_result_t SCPI_DigitalPinLevelQ(scpi_t* context);
