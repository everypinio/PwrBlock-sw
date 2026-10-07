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

#include "log_uart.h"

#include <stm32g0xx_hal.h>

extern UART_HandleTypeDef  huart3;
static UART_HandleTypeDef* uart_log = &huart3;

int log_uart_write(const uint8_t* buf, uint16_t len)
{
    HAL_StatusTypeDef ret = HAL_UART_Transmit(uart_log, buf, len, 0xffff);

    if (ret != HAL_OK) {
        return -1;
    }
    return 0;
}
