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

typedef enum
{
    USBCTL_PORT_PC = 0,
    USBCTL_PORT_IDC,
    USBCTL_PORT_PWR,

    USBCTL_PORT_COUNT,
} UsbctlPort;

/**
 * @brief Select initial USB host port. Must be called before tusb_init().
 */
void usbctl_init(void);

/**
 * @brief Reset USB host port selection to initial state.
 */
void usbctl_reset(void);

/**
 * @brief Switch USB DATA lines between USB ports.
 *
 * The selection process occurs once after the component is initialized.
 * When the selected port is disabled,
 * no search for another active port is performed.
 */
void usbctl_task_iter(void);

/**
 * @brief Get currently selected USB host port.
 *
 * @return UsbctlPort Selected port.
 */
UsbctlPort usbctl_port_get(void);

/**
 * @brief Get status of USB port selection.
 *
 * @return bool - True if port selected, False otherwise.
 */
bool usbctl_is_selected(void);
