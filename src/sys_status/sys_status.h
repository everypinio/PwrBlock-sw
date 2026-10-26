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

typedef enum
{
    SYS_STATUS_SRC_USBCTRL,
    SYS_STATUS_SRC_REGULATOR,
    SYS_STATUS_SRC_MONITOR,
    SYS_STATUS_SRC_PWR_IN,

    SYS_STATUS_SRC_NUM
} SysStatusEventSource;

typedef enum
{
    SYS_STATUS_OFF,
    SYS_STATUS_CV,
    SYS_STATUS_CC,
    SYS_STATUS_FAULT,
    SYS_STATUS_OK,
    SYS_STATUS_BUSY
} SysStatus;

/**
 * @brief Setter of system status value.
 *
 * @param src Event source module.
 * @param status Status value.
 */
void sys_status_set(SysStatusEventSource src, SysStatus status);

/**
 * @brief Getter of system status.
 *
 * @param src Target event source module.
 * @return SysStatus Current value of status.
 */
SysStatus sys_status_get(SysStatusEventSource src);
