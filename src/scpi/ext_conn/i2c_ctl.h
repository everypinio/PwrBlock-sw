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
 * @brief Restore the I2C bus defaults, used by *RST.
 */
scpi_result_t SCPI_I2cReset(scpi_t* context);

/**
 * BUS:I2C:STATe {OFF | ON | 0 | 1}
 * @brief Enable or disable the bus. Default is OFF.
 */
scpi_result_t SCPI_I2cState(scpi_t* context);

/**
 * BUS:I2C:STATe?
 * @brief Query state of the bus.
 */
scpi_result_t SCPI_I2cStateQ(scpi_t* context);

/**
 * BUS:I2C:ADDRess:WIDTh {7 | 10}
 * @brief Set width of the slave address in bits. Default is 7.
 */
scpi_result_t SCPI_I2cAddressWidth(scpi_t* context);

/**
 * BUS:I2C:ADDRess:WIDTh?
 * @brief Query width of the slave address in bits.
 */
scpi_result_t SCPI_I2cAddressWidthQ(scpi_t* context);

/**
 * BUS:I2C:WRITe {<Address>},{<Data>}
 * @brief Write up to 32 bytes to the slave.
 */
scpi_result_t SCPI_I2cWrite(scpi_t* context);

/**
 * BUS:I2C:READ? {<Address>},{<Count>}
 * @brief Read 1 to 32 bytes from the slave, reply follows FORMat[:DATA].
 */
scpi_result_t SCPI_I2cReadQ(scpi_t* context);
