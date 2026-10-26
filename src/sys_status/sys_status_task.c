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

#include "sys_status_task.h"

#include "sys_status.h"

#include "indication/indication_task.h"

#include <cmsis_os2.h>
#include <stdbool.h>

#define SYS_STATUS_QUEUE_SIZE (10U)

typedef struct
{
    SysStatusEventSource source;
    SysStatus            status;
} SysStatusMsg;

typedef void (*SysStatusCallback)(void);

typedef struct
{
    SysStatus         status;
    SysStatusCallback on_change_cb;
} SysStatusStruct;

static osMessageQueueId_t sys_status_q;

// Private prototypes
static void on_change_usbtmc_status(void);
static void on_change_regulator_status(void);
static void on_change_monitor_status(void);
static void on_change_pwr_in_status(void);


static SysStatusStruct sys_status[SYS_STATUS_SRC_NUM] = {
    [SYS_STATUS_SRC_USBCTRL] =
        {.status = SYS_STATUS_OFF, .on_change_cb = on_change_usbtmc_status   },
    [SYS_STATUS_SRC_REGULATOR] =
        {.status = SYS_STATUS_OFF, .on_change_cb = on_change_regulator_status},
    [SYS_STATUS_SRC_MONITOR] =
        {.status = SYS_STATUS_OK,  .on_change_cb = on_change_monitor_status  },
    [SYS_STATUS_SRC_PWR_IN] =
        {.status = SYS_STATUS_OFF, .on_change_cb = on_change_pwr_in_status   },
};

void sys_status_queue_init(void)
{
    sys_status_q =
        osMessageQueueNew(SYS_STATUS_QUEUE_SIZE, sizeof(SysStatusMsg), NULL);
}

void sys_status_app_task(void* argument)
{
    (void)argument;

    SysStatusMsg msg;

    while (true) {
        osStatus_t queue_read_status =
            osMessageQueueGet(sys_status_q, &msg, NULL, osWaitForever);

        // Update internal status info
        if (queue_read_status == osOK) {
            if (sys_status[msg.source].status != msg.status) {
                sys_status[msg.source].status = msg.status;
                sys_status[msg.source].on_change_cb();
            }
        }
    }
}

void sys_status_set(SysStatusEventSource src, SysStatus status)
{
    SysStatusMsg msg;
    msg.status = status;
    msg.source = src;
    osMessageQueuePut(sys_status_q, &msg, 0, 0);
}

SysStatus sys_status_get(SysStatusEventSource src)
{
    return sys_status[src].status;
}

static void on_change_usbtmc_status(void)
{
    // Proecess status for USB led
    switch (sys_status[SYS_STATUS_SRC_USBCTRL].status) {
        case SYS_STATUS_OFF: {
            led_app_set_off(INDICATION_USB);
            break;
        }
        case SYS_STATUS_BUSY: {
            led_app_set_constrain(INDICATION_USB);
            break;
        }
        case SYS_STATUS_OK: {
            led_app_set_norm(INDICATION_USB);
            break;
        }
        case SYS_STATUS_FAULT: {
            led_app_set_alarm(INDICATION_USB);
            break;
        }
        default: break;
    }
}

static void on_change_regulator_status(void)
{
    // Process status for OUT led
    // Check monitor current status
    if (sys_status[SYS_STATUS_SRC_MONITOR].status == SYS_STATUS_FAULT) {
        led_app_set_alarm(INDICATION_OUT);
    }
    else {
        switch (sys_status[SYS_STATUS_SRC_REGULATOR].status) {
            case SYS_STATUS_OFF: {
                led_app_set_off(INDICATION_OUT);
                break;
            }
            case SYS_STATUS_CC: {
                led_app_set_warning(INDICATION_OUT);
                break;
            }
            case SYS_STATUS_CV: {
                led_app_set_norm(INDICATION_OUT);
                break;
            }
            case SYS_STATUS_FAULT: {
                led_app_set_alarm(INDICATION_OUT);
                break;
            }
            default: break;
        }
    }
}

static void on_change_monitor_status(void)
{
    // Process status for OUT led
    if (sys_status[SYS_STATUS_SRC_MONITOR].status == SYS_STATUS_FAULT) {
        led_app_set_alarm(INDICATION_OUT);
    }
    else {
        // Update with current regulator state
        on_change_regulator_status();
    }
}

static void on_change_pwr_in_status(void)
{
    // Proecess status for IN led
    switch (sys_status[SYS_STATUS_SRC_PWR_IN].status) {
        case SYS_STATUS_OFF: {
            led_app_set_off(INDICATION_IN);
            break;
        }
        case SYS_STATUS_OK: {
            led_app_set_norm(INDICATION_IN);
            break;
        }
        case SYS_STATUS_FAULT: {
            led_app_set_alarm(INDICATION_IN);
            break;
        }
        default: break;
    }
}
