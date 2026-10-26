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

#include "usbctl.h"

#include <cmsis_os2.h>
#include <stdbool.h>
#include <tusb.h>

#include "main.h"
#include "sys_status/sys_status.h"

#define USBCTL_PORT_SWITCH_PERIOD_MS 1000

typedef struct
{
    UsbctlPort port;
    bool       selected;
    uint32_t   select_tick;
} UsbctlHandle;

static UsbctlHandle usbctl;

// Private function prototypes
static void mux_set(UsbctlPort port);

void usbctl_init(void)
{
    usbctl_reset();
}

void usbctl_reset(void)
{
    usbctl.selected    = false;
    usbctl.select_tick = osKernelGetTickCount();
    usbctl.port        = USBCTL_PORT_PC;
    mux_set(usbctl.port);
}

void usbctl_task_iter(void)
{
    if (usbctl.selected) {
        return;
    }

    if (tud_connected()) {
        usbctl.selected = true;
        sys_status_set(SYS_STATUS_SRC_USBCTRL, SYS_STATUS_BUSY);
        return;
    }

    uint32_t now = osKernelGetTickCount();
    if ((now - usbctl.select_tick) < USBCTL_PORT_SWITCH_PERIOD_MS) {
        return;
    }

    usbctl.port        = (usbctl.port + 1) % USBCTL_PORT_COUNT;
    usbctl.select_tick = now;
    mux_set(usbctl.port);
}

UsbctlPort usbctl_port_get(void)
{
    return usbctl.port;
}

static void mux_set(UsbctlPort port)
{
    // Mux code SW1,SW2: 01 - PLD, 11 - PC, 00, 10 - PWR
    GPIO_PinState sw1 =
        (port == USBCTL_PORT_PC) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState sw2 =
        (port != USBCTL_PORT_PWR) ? GPIO_PIN_SET : GPIO_PIN_RESET;

    HAL_GPIO_WritePin(USB_SW1_GPIO_Port, USB_SW1_Pin, sw1);
    HAL_GPIO_WritePin(USB_SW2_GPIO_Port, USB_SW2_Pin, sw2);
}
