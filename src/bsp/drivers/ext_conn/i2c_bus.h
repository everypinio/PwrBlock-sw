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

/// Result of an operation of the bus
typedef enum
{
    I2C_BUS_OK = 0,
    I2C_BUS_ERR_NACK,      // No acknowledge from the slave
    I2C_BUS_ERR_TIMEOUT,   // Transaction timeout, or a stretched clock
    I2C_BUS_ERR_BUS,       // Arbitration lost, bus error or overrun
    I2C_BUS_ERR_DISABLED,  // The bus is disabled
    I2C_BUS_ERR_PARAM,     // Address or length out of range
} I2cBusStatus;

/// Width of the slave address
typedef enum
{
    I2C_BUS_WIDTH_7BIT  = 7,
    I2C_BUS_WIDTH_10BIT = 10,
} I2cBusAddrWidth;

/**
 * @brief Init the I2C bus of the external connector.
 *
 * Master at a fixed 100 kHz, external pull-ups required. The bus starts
 * disabled.
 */
void i2c_bus_init(void);

/**
 * @brief Restore defaults: bus disabled, 7 bit address width.
 */
void i2c_bus_reset(void);

/**
 * @brief Enable or disable the bus.
 *
 * @param enabled True - enable, false - disable.
 * @return I2cBusStatus I2C_BUS_ERR_BUS if the peripheral setup failed.
 */
I2cBusStatus i2c_bus_state_set(bool enabled);

/**
 * @brief Get state of the bus.
 *
 * @return bool True if the bus is enabled.
 */
bool i2c_bus_state_get(void);

/**
 * @brief Set width of the slave address, applied at once on an enabled bus.
 *
 * @param width Width to apply.
 * @return I2cBusStatus I2C_BUS_ERR_BUS if the peripheral setup failed.
 */
I2cBusStatus i2c_bus_addr_width_set(I2cBusAddrWidth width);

/**
 * @brief Get width of the slave address.
 *
 * @return I2cBusAddrWidth Current width.
 */
I2cBusAddrWidth i2c_bus_addr_width_get(void);

/**
 * @brief Write data to a slave: START, address + W, data, STOP.
 *
 * @param addr Unshifted address of the slave.
 * @param data Bytes to write.
 * @param len Number of bytes, 1 to I2C_BUS_XFER_MAX_LEN.
 * @return I2cBusStatus Result of the transfer.
 */
I2cBusStatus i2c_bus_write(uint32_t addr, const uint8_t* data, uint32_t len);

/**
 * @brief Read data from a slave: START, address + R, data, STOP.
 *
 * @param addr Unshifted address of the slave.
 * @param dst Destination of the read bytes.
 * @param count Number of bytes, 1 to I2C_BUS_XFER_MAX_LEN.
 * @return I2cBusStatus Result of the transfer.
 */
I2cBusStatus i2c_bus_read(uint32_t addr, uint8_t* dst, uint32_t count);
