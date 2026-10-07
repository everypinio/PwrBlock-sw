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

#include "bus_format.h"

#include <string.h>

#define BUS_FORMAT_DEFAULT BUS_FORMAT_INT

// Parameter patterns, used to decode an enumerated value of a command
static const scpi_choice_def_t FORMAT_CHOICES[] = {
    {"ASCii",       BUS_FORMAT_ASCII},
    {"HEXadecimal", BUS_FORMAT_HEX  },
    {"INTeger",     BUS_FORMAT_INT  },
    SCPI_CHOICE_LIST_END
};

// Short forms of the enumerated values, used to answer a query
static const char* const FORMAT_NAMES[] = {"ASC", "HEX", "INT"};

static BusFormat format = BUS_FORMAT_DEFAULT;

// Private function prototypes
static void bus_byte_result_hex(scpi_t* context, uint8_t value);

BusFormat bus_format_get(void)
{
    return format;
}

void bus_format_reset(void)
{
    format = BUS_FORMAT_DEFAULT;
}

scpi_bool_t bus_data_param(
    scpi_t* context, uint8_t* dst, uint32_t max, uint32_t* len
)
{
    scpi_parameter_t param;

    // The first parameter decides the form of the whole value
    if (!SCPI_Parameter(context, &param, TRUE)) {
        return FALSE;
    }

    if (param.type == SCPI_TOKEN_ARBITRARY_BLOCK_PROGRAM_DATA) {
        if (param.len < 0 || (uint32_t)param.len > max) {
            SCPI_ErrorPush(context, SCPI_ERROR_TOO_MUCH_DATA);
            return FALSE;
        }

        memcpy(dst, param.ptr, (size_t)param.len);
        *len = (uint32_t)param.len;
        return TRUE;
    }

    uint32_t value = 0;
    if (!SCPI_ParamToUInt32(context, &param, &value)) {
        return FALSE;
    }

    uint32_t count = 0;

    // Every following read consumes the comma of the list on its own, and
    // reports the end of the list by failing on a missing optional parameter
    do {
        if (value > UINT8_MAX) {
            SCPI_ErrorPush(context, SCPI_ERROR_DATA_OUT_OF_RANGE);
            return FALSE;
        }

        if (count >= max) {
            SCPI_ErrorPush(context, SCPI_ERROR_TOO_MUCH_DATA);
            return FALSE;
        }

        dst[count] = (uint8_t)value;
        count++;
    } while (SCPI_ParamUInt32(context, &value, FALSE));

    // Tell the end of the list from a malformed byte of it
    if (SCPI_ParamErrorOccurred(context)) {
        return FALSE;
    }

    *len = count;
    return TRUE;
}

void bus_data_result(scpi_t* context, const uint8_t* data, uint32_t len)
{
    switch (format) {
        case BUS_FORMAT_ASCII: {
            SCPI_ResultArrayUInt8(context, data, len, SCPI_FORMAT_ASCII);
            break;
        }
        case BUS_FORMAT_INT: {
            // Bytes have no byte order, so the block is the same in any of
            // the binary formats of the parser
            SCPI_ResultArrayUInt8(context, data, len, SCPI_FORMAT_NORMAL);
            break;
        }
        case BUS_FORMAT_HEX:
        default: {
            // The parser has no hexadecimal array, the bytes are written one
            // by one and the separating commas are emitted by the parser
            for (uint32_t index = 0; index < len; index++) {
                bus_byte_result_hex(context, data[index]);
            }
            break;
        }
    }
}

scpi_result_t SCPI_BusFormatReset(scpi_t* context)
{
    (void)context;

    bus_format_reset();
    return SCPI_RES_OK;
}

scpi_result_t SCPI_BusFormat(scpi_t* context)
{
    int32_t value = 0;

    // Read first parameter if present
    if (!SCPI_ParamChoice(context, FORMAT_CHOICES, &value, TRUE)) {
        return SCPI_RES_ERR;
    }

    format = (BusFormat)value;
    return SCPI_RES_OK;
}

scpi_result_t SCPI_BusFormatQ(scpi_t* context)
{
    const char* name = FORMAT_NAMES[format];

    SCPI_ResultCharacters(context, name, strlen(name));
    return SCPI_RES_OK;
}

/**
 * @brief Append a byte to a reply as a pair of hexadecimal digits, "#H01".
 *
 * @param context
 * @param value Byte to reply.
 */
static void bus_byte_result_hex(scpi_t* context, uint8_t value)
{
    static const char DIGITS[] = "0123456789ABCDEF";

    // The parser drops the leading zero of a hexadecimal result, while the
    // interface reports a byte as a pair of digits
    const char text[] = {'#', 'H', DIGITS[value >> 4], DIGITS[value & 0x0F]};

    SCPI_ResultCharacters(context, text, sizeof(text));
}
