/*
 * Copyright (c) 2026 Everypin
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
 *
 * --- NOTICE OF MODIFICATION ---
 * This file has been modified by Everypin in 2026.
 * Original code is licensed under the MIT License (see below).
 * Modifications are licensed under the Apache License 2.0.
 *
 * --- ORIGINAL MIT LICENSE NOTE ---
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <cmsis_os2.h>
#include <tusb.h>

#include "scpi/scpi_def.h"
#include "usbctl.h"

#ifdef USB_MODE_TMC
#include "usbtmc/usbtmc_app.h"
#elif USB_MODE_CDC
#include "usbcdc/usbcdc_app.h"
#endif

#define USBCTL_POLL_MS (100U)

void tud_app_task(void* argument)
{
    (void)argument;

    // init device stack on configured roothub port
    tusb_rhport_init_t dev_init = {
        .role = TUSB_ROLE_DEVICE, .speed = TUSB_SPEED_AUTO
    };

    usbctl_init();
    tusb_init(BOARD_TUD_RHPORT, &dev_init);

    while (true) {
        tud_task_ext(USBCTL_POLL_MS, false);
        usbctl_task_iter();
    }
}

void scpi_parser_app_task(void* argument)
{
    (void)argument;

    SCPI_Init(
        &scpi_context,
        scpi_commands,
        &scpi_interface,
        scpi_units_def,
        SCPI_IDN1,
        SCPI_IDN2,
        SCPI_IDN3,
        SCPI_IDN4,
        scpi_input_buffer,
        SCPI_INPUT_BUFFER_LENGTH,
        scpi_error_queue_data,
        SCPI_ERROR_QUEUE_SIZE
    );

    while (true) {
#ifdef USB_MODE_TMC
        usbtmc_app_task_iter();
#elif USB_MODE_CDC
        usbcdc_app_task_iter();
#endif
        osDelay(100 / portTICK_PERIOD_MS);
    }
}
