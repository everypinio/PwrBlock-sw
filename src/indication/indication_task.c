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

#include "indication_task.h"

#include <cmsis_os2.h>
#include <stdbool.h>

typedef struct
{
    LedName led_name;
    IndicationState   state;
} IndicationQueueMessage;

static osMessageQueueId_t indication_q;

void led_app_task(void* argument)
{
    (void)argument;

    indication_init();

    // Init task queue
    indication_q = osMessageQueueNew(5, sizeof(IndicationQueueMessage), NULL);

    while (true) {
        IndicationQueueMessage cmd;
        while (osMessageQueueGet(indication_q, &cmd, NULL, 0) == osOK) {
            indication_set_state(cmd.led_name, cmd.state);
        }

        indication_update(20);

        osDelay(20);
    }
}

void led_app_set_off(LedName led_name)
{
    IndicationQueueMessage msg = {
        .led_name = led_name, .state = INDICATION_STATE_OFF
    };
    osMessageQueuePut(indication_q, &msg, 0U, 0U);
}

void led_app_set_norm(LedName led_name)
{
    IndicationQueueMessage msg = {
        .led_name = led_name, .state = INDICATION_STATE_NORM
    };
    osMessageQueuePut(indication_q, &msg, 0U, 0U);
}

void led_app_set_act(LedName led_name)
{
    IndicationQueueMessage msg = {
        .led_name = led_name, .state = INDICATION_STATE_ACT
    };
    osMessageQueuePut(indication_q, &msg, 0U, 0U);
}

void led_app_set_constrain(LedName led_name)
{
    IndicationQueueMessage msg = {
        .led_name = led_name, .state = INDICATION_STATE_CONSTRAIN
    };
    osMessageQueuePut(indication_q, &msg, 0U, 0U);
}

void led_app_set_warning(LedName led_name)
{
    IndicationQueueMessage msg = {
        .led_name = led_name, .state = INDICATION_STATE_WARNING
    };
    osMessageQueuePut(indication_q, &msg, 0U, 0U);
}

void led_app_set_alarm(LedName led_name)
{
    IndicationQueueMessage msg = {
        .led_name = led_name, .state = INDICATION_STATE_ALARM
    };
    osMessageQueuePut(indication_q, &msg, 0U, 0U);
}
