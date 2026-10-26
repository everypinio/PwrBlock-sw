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

#include "bsp/drivers/led/led_def.h"

#include <stdint.h>

typedef enum
{
    INDICATION_STATE_OFF,
    INDICATION_STATE_NORM,
    INDICATION_STATE_ACT,
    INDICATION_STATE_CONSTRAIN,
    INDICATION_STATE_WARNING,
    INDICATION_STATE_ALARM,
} IndicationState;

/**
 * @brief Init indication system.
 *
 */
void indication_init(void);

/**
 * @brief Set state of led in indication.
 *
 * @param name Naming of led in system.
 * @param state State to be set.
 */
void indication_set_state(LedName name, IndicationState state);

/**
 * @brief Update indication system (render led states).
 *
 * @param ticks Ticks passed from previous update (scale of milli seconds).
 */
void indication_update(uint32_t ticks);
