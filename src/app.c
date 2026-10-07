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

#include "bsp/drivers/ext_conn/digital_gpio.h"
#include "cmsis_os2.h"
#include "middleware/adc_mgr/adc_mgr.h"
#include "middleware/i2c_mgr/i2c_mgr.h"
#include "threads/threads.h"

int app(void)
{
    i2c_manager_init();
    adc_manager_init();
    digital_gpio_init();

    thread_usbpd_init();
    thread_usbtmc_init();
    thread_regulator_init();
    thread_indication_init();
    thread_button_init();
    thread_monitor_init();
    thread_sys_status_init();

    osKernelInitialize();
    osKernelStart();

    while (1) {}
}
