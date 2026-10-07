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
#include <stdint.h>

/// Response format of the read queries of all buses, set by FORMat[:DATA].
typedef enum
{
    BUS_FORMAT_ASCII = 0,  // Decimal bytes, "80,1,255"
    BUS_FORMAT_HEX,        // Hexadecimal bytes, "#H50,#H01,#HFF"
    BUS_FORMAT_INT,        // Arbitrary block of raw bytes, "#13ABC"
} BusFormat;

/**
 * @brief Get the response format of the read queries.
 *
 * @return BusFormat Current format.
 */
BusFormat bus_format_get(void);

/**
 * @brief Restore the power-on default of the response format, used by *RST.
 */
void bus_format_reset(void);

/**
 * @brief Read a <Data> parameter of a bus write command.
 *
 * Both documented forms are accepted, regardless of the response format: a
 * comma-separated list of bytes, e.g. "#H50,#H01,255", and a definite-length
 * arbitrary block, e.g. "#13ABC".
 *
 * @param context
 * @param dst Destination of the decoded bytes.
 * @param max Capacity of dst in bytes.
 * @param len Number of decoded bytes.
 * @return scpi_bool_t FALSE if the parameter is missing or invalid.
 */
scpi_bool_t bus_data_param(
    scpi_t* context, uint8_t* dst, uint32_t max, uint32_t* len
);

/**
 * @brief Answer a bus read query in the current response format.
 *
 * @param context
 * @param data Bytes to reply.
 * @param len Number of bytes to reply.
 */
void bus_data_result(scpi_t* context, const uint8_t* data, uint32_t len);

/**
 * @brief Restore the power-on default of the response format.
 *
 * @param context
 * @return scpi_result_t
 */
scpi_result_t SCPI_BusFormatReset(scpi_t* context);

/**
 * FORMat[:DATA] {ASCii | HEXadecimal | INTeger}
 * @brief Set up the response format of the read queries of all buses. Default
 * is HEXadecimal.
 *
 * @param context
 * @return scpi_result_t
 */
scpi_result_t SCPI_BusFormat(scpi_t* context);

/**
 * FORMat[:DATA]?
 * @brief This query returns the response format of the read queries.
 *
 * @param context
 * @return scpi_result_t
 */
scpi_result_t SCPI_BusFormatQ(scpi_t* context);
