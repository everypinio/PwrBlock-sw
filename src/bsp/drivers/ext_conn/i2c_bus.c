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

#include "i2c_bus.h"
#include "i2c_bus_def.h"
#include "i2c_bus_it.h"

#include <cmsis_os2.h>
#include <stm32g0xx_hal.h>

typedef struct
{
    I2C_HandleTypeDef handler;
    osMutexId_t       mutex;
    osSemaphoreId_t   ready_sem;

    I2cBusAddrWidth   width;
    bool              enabled;
    volatile bool     pending;
    volatile uint32_t errors;
} I2CBus;

static I2CBus bus;

extern I2C_HandleTypeDef hi2c2;

// Private function prototypes
static I2cBusStatus i2c_bus_status(uint32_t errors);

static I2cBusStatus i2c_bus_refresh(void);
static I2cBusStatus i2c_bus_apply(void);
static void         i2c_bus_xfer_prepare(void);
static I2cBusStatus i2c_bus_wait(HAL_StatusTypeDef started);

static uint16_t i2c_bus_dev_addr(uint32_t addr);
static uint32_t i2c_bus_addr_max(void);

void i2c_bus_init(void)
{
    bus.ready_sem = osSemaphoreNew(1, 0, NULL);
    bus.mutex     = osMutexNew(NULL);

    // The de-init of a handle touches its peripheral, so the handle of the bus
    // needs it before the first one
    bus.handler = hi2c2;

    // The peripheral is brought up by the setup of the board on its own handle,
    // the bus takes it over with the one above
    HAL_I2C_DeInit(&hi2c2);

    i2c_bus_reset();
}

void i2c_bus_reset(void)
{
    bus.enabled = false;
    bus.width   = I2C_BUS_WIDTH_7BIT;

    HAL_I2C_DeInit(&bus.handler);
}

bool i2c_bus_state_get(void)
{
    return bus.enabled;
}

I2cBusAddrWidth i2c_bus_addr_width_get(void)
{
    return bus.width;
}

I2cBusStatus i2c_bus_state_set(bool enabled)
{
    if (enabled == bus.enabled) {
        return I2C_BUS_OK;
    }

    if (!enabled) {
        bus.enabled = false;
        HAL_I2C_DeInit(&bus.handler);
        return I2C_BUS_OK;
    }

    I2cBusStatus status = i2c_bus_apply();
    if (status != I2C_BUS_OK) {
        HAL_I2C_DeInit(&bus.handler);
        return status;
    }

    bus.enabled = true;
    return I2C_BUS_OK;
}

I2cBusStatus i2c_bus_addr_width_set(I2cBusAddrWidth width)
{
    bus.width = width;

    return i2c_bus_refresh();
}

I2cBusStatus i2c_bus_write(uint32_t addr, const uint8_t* data, uint32_t len)
{
    if (!bus.enabled) {
        return I2C_BUS_ERR_DISABLED;
    }

    if (addr > i2c_bus_addr_max()) {
        return I2C_BUS_ERR_PARAM;
    }

    if (len == 0 || len > I2C_BUS_XFER_MAX_LEN) {
        return I2C_BUS_ERR_PARAM;
    }

    i2c_bus_xfer_prepare();

    return i2c_bus_wait(HAL_I2C_Master_Transmit_IT(
        &bus.handler, i2c_bus_dev_addr(addr), (uint8_t*)data, (uint16_t)len
    ));
}

I2cBusStatus i2c_bus_read(uint32_t addr, uint8_t* dst, uint32_t count)
{
    if (!bus.enabled) {
        return I2C_BUS_ERR_DISABLED;
    }

    if (addr > i2c_bus_addr_max()) {
        return I2C_BUS_ERR_PARAM;
    }

    if (count == 0 || count > I2C_BUS_XFER_MAX_LEN) {
        return I2C_BUS_ERR_PARAM;
    }

    i2c_bus_xfer_prepare();

    return i2c_bus_wait(HAL_I2C_Master_Receive_IT(
        &bus.handler, i2c_bus_dev_addr(addr), dst, (uint16_t)count
    ));
}

void i2c_bus_irq_handler(void)
{
    I2C_HandleTypeDef* const hi2c = &bus.handler;

    // The HAL returns the handle to ready at the end of a transfer, either
    // completed or aborted by an error, a NACK included
    if (bus.pending && HAL_I2C_GetState(hi2c) == HAL_I2C_STATE_READY) {
        bus.pending = false;
        bus.errors  = HAL_I2C_GetError(hi2c);
        osSemaphoreRelease(bus.ready_sem);
    }
}

/**
 * @brief Get max unshifted slave address for the current width.
 */
static uint32_t i2c_bus_addr_max(void)
{
    if (bus.width == I2C_BUS_WIDTH_10BIT) {
        return I2C_BUS_ADDR_MAX_10BIT;
    }

    return I2C_BUS_ADDR_MAX_7BIT;
}

/**
 * @brief Convert an unshifted address to the HAL form, 7 bit ones shifted left.
 */
static uint16_t i2c_bus_dev_addr(uint32_t addr)
{
    if (bus.width == I2C_BUS_WIDTH_10BIT) {
        return (uint16_t)addr;
    }

    return (uint16_t)(addr << 1);
}

/**
 * @brief Convert a HAL_I2C_ERROR_* code into a status of the bus.
 */
static I2cBusStatus i2c_bus_status(uint32_t errors)
{
    if (errors == HAL_I2C_ERROR_NONE) {
        return I2C_BUS_OK;
    }

    if ((errors & HAL_I2C_ERROR_AF) != 0) {
        return I2C_BUS_ERR_NACK;
    }

    return I2C_BUS_ERR_BUS;
}

/**
 * @brief Re-initialize the peripheral with the stored configuration.
 */
static I2cBusStatus i2c_bus_apply(void)
{
    HAL_I2C_DeInit(&bus.handler);

    bus.handler.Instance              = I2C2;
    bus.handler.Init.Timing           = I2C_BUS_TIMING_100_KHZ;
    bus.handler.Init.OwnAddress1      = 0;
    bus.handler.Init.AddressingMode   = (bus.width == I2C_BUS_WIDTH_10BIT) ?
                                            I2C_ADDRESSINGMODE_10BIT :
                                            I2C_ADDRESSINGMODE_7BIT;
    bus.handler.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    bus.handler.Init.OwnAddress2      = 0;
    bus.handler.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    bus.handler.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    bus.handler.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(&bus.handler) != HAL_OK) {
        return I2C_BUS_ERR_BUS;
    }

    if (HAL_I2CEx_ConfigAnalogFilter(&bus.handler, I2C_ANALOGFILTER_ENABLE) !=
        HAL_OK) {
        return I2C_BUS_ERR_BUS;
    }

    if (HAL_I2CEx_ConfigDigitalFilter(&bus.handler, 0) != HAL_OK) {
        return I2C_BUS_ERR_BUS;
    }

    return I2C_BUS_OK;
}

/**
 * @brief Re-apply the configuration if the bus is enabled.
 */
static I2cBusStatus i2c_bus_refresh(void)
{
    if (!bus.enabled) {
        return I2C_BUS_OK;
    }

    return i2c_bus_apply();
}

/**
 * @brief Arm the completion semaphore, call right before starting a transfer.
 */
static void i2c_bus_xfer_prepare(void)
{
    // Drop a release left by a transfer that has just timed out
    osSemaphoreAcquire(bus.ready_sem, 0);

    bus.errors  = HAL_I2C_ERROR_NONE;
    bus.pending = true;
}

/**
 * @brief Wait for the end of a transfer, re-init the peripheral on timeout.
 *
 * @param started Result of the HAL call that started the transfer.
 */
static I2cBusStatus i2c_bus_wait(HAL_StatusTypeDef started)
{
    if (started != HAL_OK) {
        bus.pending = false;
        return I2C_BUS_ERR_BUS;
    }

    if (osSemaphoreAcquire(bus.ready_sem, I2C_BUS_TIMEOUT_MS) != osOK) {
        bus.pending = false;
        i2c_bus_apply();
        return I2C_BUS_ERR_TIMEOUT;
    }

    return i2c_bus_status(bus.errors);
}
