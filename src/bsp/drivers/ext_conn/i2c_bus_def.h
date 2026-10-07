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

/*
 * Fixed parameters and limits of the I2C bus: timeout, clock timing, transfer
 * length and address ranges.
 *
 * Internal to the I2C component: include it from its .c files only
 * (i2c_bus.c and i2c_ctl.c), the public interface of the backend is
 * i2c_bus.h.
 */

/// Timeout of a transaction in milliseconds, including a clock stretched by a
/// slave. The commands are served from the USB device task, so it also bounds
/// how long an unresponsive slave can delay the answer to any other command.
#define I2C_BUS_TIMEOUT_MS 100

/**
 * TIMINGR value of the peripheral, the clock of the bus is fixed at 100 kHz.
 * I2C2 is clocked from PCLK1 at 64 MHz, so a prescaled tick is
 * t_PRESC = (PRESC + 1) / 64 MHz and the period of the clock is
 * (SCLL + 1 + SCLH + 1) * t_PRESC plus the rise and fall times.
 *
 * The value is the one CubeMX generated for MX_I2C2_Init(): PRESC = 1,
 * SCLDEL = 0xB, SDADEL = 1, SCLH = 125, SCLL = 181, that is 126 * 31.25 ns high
 * and 182 * 31.25 ns low.
 */
#define I2C_BUS_TIMING_100_KHZ 0x10B17DB5

/// Maximum number of data bytes of a single transfer. The reply of a read is
/// built in the output buffer of the SCPI parser, and a byte costs up to five
/// characters of it in the hexadecimal response format.
#define I2C_BUS_XFER_MAX_LEN 32

/// Max unshifted address of a slave, per width of the address
#define I2C_BUS_ADDR_MAX_7BIT  0x7F
#define I2C_BUS_ADDR_MAX_10BIT 0x3FF
