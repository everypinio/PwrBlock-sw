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

#include "i2c_ctl.h"

#include "bsp/drivers/ext_conn/i2c_bus.h"
#include "bsp/drivers/ext_conn/i2c_bus_def.h"
#include "scpi/bus_format.h"

// Buffers of the transfers. The commands are served from the USB device task,
// whose stack is too small to carry them, and only that task touches them
static uint8_t tx_buffer[I2C_BUS_XFER_MAX_LEN];
static uint8_t rx_buffer[I2C_BUS_XFER_MAX_LEN];

/**
 * @brief Report a result of the bus through the error queue.
 *
 * @param context
 * @param status Result of the call of the backend.
 * @return scpi_result_t SCPI_RES_ERR if the status is an error.
 */
static scpi_result_t i2c_status_result(scpi_t* context, I2cBusStatus status)
{
    int16_t error = 0;

    switch (status) {
        case I2C_BUS_OK: {
            return SCPI_RES_OK;
        }
        case I2C_BUS_ERR_NACK: {
            error = SCPI_ERROR_HARDWARE_MISSING;
            break;
        }
        case I2C_BUS_ERR_TIMEOUT: {
            error = SCPI_ERROR_TIME_OUT;
            break;
        }
        case I2C_BUS_ERR_DISABLED: {
            error = SCPI_ERROR_SETTINGS_CONFLICT;
            break;
        }
        case I2C_BUS_ERR_PARAM: {
            error = SCPI_ERROR_DATA_OUT_OF_RANGE;
            break;
        }
        case I2C_BUS_ERR_BUS:
        default: {
            error = SCPI_ERROR_HARDWARE_ERROR;
            break;
        }
    }

    SCPI_ErrorPush(context, error);
    return SCPI_RES_ERR;
}

/**
 * @brief Check that the bus is enabled before a transfer.
 *
 * @param context
 * @return scpi_bool_t FALSE if the bus is disabled.
 */
static scpi_bool_t i2c_enabled_check(scpi_t* context)
{
    if (!i2c_bus_state_get()) {
        SCPI_ErrorPush(context, SCPI_ERROR_SETTINGS_CONFLICT);
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief Read a <Count> parameter of a read query.
 *
 * @param context
 * @param count Decoded number of bytes.
 * @return scpi_bool_t FALSE if the parameter is missing or out of range.
 */
static scpi_bool_t i2c_count_param(scpi_t* context, uint32_t* count)
{
    uint32_t value = 0;

    // Read first parameter if present
    if (!SCPI_ParamUInt32(context, &value, TRUE)) {
        return FALSE;
    }

    if (value == 0) {
        SCPI_ErrorPush(context, SCPI_ERROR_DATA_OUT_OF_RANGE);
        return FALSE;
    }

    if (value > I2C_BUS_XFER_MAX_LEN) {
        SCPI_ErrorPush(context, SCPI_ERROR_TOO_MUCH_DATA);
        return FALSE;
    }

    *count = value;
    return TRUE;
}

scpi_result_t SCPI_I2cReset(scpi_t* context)
{
    (void)context;

    i2c_bus_reset();
    return SCPI_RES_OK;
}

scpi_result_t SCPI_I2cState(scpi_t* context)
{
    bool state = false;

    // Read first parameter if present
    if (!SCPI_ParamBool(context, &state, TRUE)) {
        return SCPI_RES_ERR;
    }

    return i2c_status_result(context, i2c_bus_state_set(state));
}

scpi_result_t SCPI_I2cStateQ(scpi_t* context)
{
    SCPI_ResultBool(context, i2c_bus_state_get());
    return SCPI_RES_OK;
}

scpi_result_t SCPI_I2cAddressWidth(scpi_t* context)
{
    uint32_t value = 0;

    // Read first parameter if present, the widths are numbers rather than
    // mnemonics, so they are not read as a choice
    if (!SCPI_ParamUInt32(context, &value, TRUE)) {
        return SCPI_RES_ERR;
    }

    // The value of the enum is the width in bits, so it is the only place that
    // decides which widths are accepted
    if (value != I2C_BUS_WIDTH_7BIT && value != I2C_BUS_WIDTH_10BIT) {
        SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
        return SCPI_RES_ERR;
    }

    const I2cBusAddrWidth width = (value == I2C_BUS_WIDTH_10BIT) ?
                                      I2C_BUS_WIDTH_10BIT :
                                      I2C_BUS_WIDTH_7BIT;

    return i2c_status_result(context, i2c_bus_addr_width_set(width));
}

scpi_result_t SCPI_I2cAddressWidthQ(scpi_t* context)
{
    const uint32_t width = (i2c_bus_addr_width_get() == I2C_BUS_WIDTH_10BIT) ?
                               I2C_BUS_WIDTH_10BIT :
                               I2C_BUS_WIDTH_7BIT;

    SCPI_ResultUInt32(context, width);
    return SCPI_RES_OK;
}

scpi_result_t SCPI_I2cWrite(scpi_t* context)
{
    if (!i2c_enabled_check(context)) {
        return SCPI_RES_ERR;
    }

    // Read first parameter if present
    uint32_t addr = 0;
    if (!SCPI_ParamUInt32(context, &addr, TRUE)) {
        return SCPI_RES_ERR;
    }

    uint32_t len = 0;
    if (!bus_data_param(context, tx_buffer, I2C_BUS_XFER_MAX_LEN, &len)) {
        return SCPI_RES_ERR;
    }

    return i2c_status_result(context, i2c_bus_write(addr, tx_buffer, len));
}

scpi_result_t SCPI_I2cReadQ(scpi_t* context)
{
    if (!i2c_enabled_check(context)) {
        return SCPI_RES_ERR;
    }

    // Read first parameter if present
    uint32_t addr = 0;
    if (!SCPI_ParamUInt32(context, &addr, TRUE)) {
        return SCPI_RES_ERR;
    }

    uint32_t count = 0;
    if (!i2c_count_param(context, &count)) {
        return SCPI_RES_ERR;
    }

    const I2cBusStatus status = i2c_bus_read(addr, rx_buffer, count);
    if (status != I2C_BUS_OK) {
        return i2c_status_result(context, status);
    }

    bus_data_result(context, rx_buffer, count);
    return SCPI_RES_OK;
}
