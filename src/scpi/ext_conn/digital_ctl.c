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

#include "digital_ctl.h"
#include "bsp/drivers/ext_conn/digital_gpio.h"

#include <string.h>

/// Owner of a pin, reported by [SOURce]:DIGital:PIN<n>:FUNCtion?. Buses are
/// wired to their own dedicated pins, so IO1 - IO7 are always GPIO.
typedef enum
{
    DIGITAL_FUNCTION_GPIO = 0,
} DigitalFunction;

// Parameter patterns, used to decode an enumerated value of a command
static const scpi_choice_def_t DIRECTION_CHOICES[] = {
    {"INPut",  DIGITAL_DIRECTION_INPUT },
    {"OUTPut", DIGITAL_DIRECTION_OUTPUT},
    SCPI_CHOICE_LIST_END
};

static const scpi_choice_def_t MODE_CHOICES[] = {
    {"PUSHpull", DIGITAL_MODE_PUSHPULL},
    {"ODRain",   DIGITAL_MODE_ODRAIN  },
    SCPI_CHOICE_LIST_END
};

static const scpi_choice_def_t PULL_CHOICES[] = {
    {"NONE", DIGITAL_PULL_NONE},
    {"UP",   DIGITAL_PULL_UP  },
    {"DOWN", DIGITAL_PULL_DOWN},
    SCPI_CHOICE_LIST_END
};

// Short forms of the enumerated values, used to answer a query
static const char* const FUNCTION_NAMES[]  = {"GPIO"};
static const char* const DIRECTION_NAMES[] = {"INP", "OUTP"};
static const char* const MODE_NAMES[]      = {"PUSH", "ODR"};
static const char* const PULL_NAMES[]      = {"NONE", "UP", "DOWN"};

// Private function prototypes
static void digital_name_result(
    scpi_t* context, const char* const* names, int32_t value
);
static scpi_bool_t digital_pin_decode(scpi_t* context, int32_t* pin);
static scpi_bool_t digital_pin_choice_decode(
    scpi_t*                  context,
    const scpi_choice_def_t* choices,
    int32_t*                 pin,
    int32_t*                 value
);

scpi_result_t SCPI_DigitalPinFunctionQ(scpi_t* context)
{
    int32_t pin = 0;
    if (!digital_pin_decode(context, &pin)) {
        return SCPI_RES_ERR;
    }

    digital_name_result(context, FUNCTION_NAMES, DIGITAL_FUNCTION_GPIO);
    return SCPI_RES_OK;
}

scpi_result_t SCPI_DigitalCountQ(scpi_t* context)
{
    SCPI_ResultUInt32(context, DIGITAL_PIN_COUNT);
    return SCPI_RES_OK;
}

scpi_result_t SCPI_DigitalPinDirection(scpi_t* context)
{
    int32_t pin   = 0;
    int32_t value = 0;
    if (!digital_pin_choice_decode(context, DIRECTION_CHOICES, &pin, &value)) {
        return SCPI_RES_ERR;
    }

    digital_gpio_direction_set((uint32_t)pin, (DigitalDirection)value);
    return SCPI_RES_OK;
}

scpi_result_t SCPI_DigitalPinDirectionQ(scpi_t* context)
{
    int32_t pin = 0;
    if (!digital_pin_decode(context, &pin)) {
        return SCPI_RES_ERR;
    }

    DigitalDirection direction = digital_gpio_direction_get((uint32_t)pin);

    digital_name_result(context, DIRECTION_NAMES, (int32_t)direction);
    return SCPI_RES_OK;
}

scpi_result_t SCPI_DigitalPinMode(scpi_t* context)
{
    int32_t pin   = 0;
    int32_t value = 0;
    if (!digital_pin_choice_decode(context, MODE_CHOICES, &pin, &value)) {
        return SCPI_RES_ERR;
    }

    digital_gpio_mode_set((uint32_t)pin, (DigitalMode)value);
    return SCPI_RES_OK;
}

scpi_result_t SCPI_DigitalPinModeQ(scpi_t* context)
{
    int32_t pin = 0;
    if (!digital_pin_decode(context, &pin)) {
        return SCPI_RES_ERR;
    }

    DigitalMode mode = digital_gpio_mode_get((uint32_t)pin);

    digital_name_result(context, MODE_NAMES, (int32_t)mode);
    return SCPI_RES_OK;
}

scpi_result_t SCPI_DigitalPinPull(scpi_t* context)
{
    int32_t pin   = 0;
    int32_t value = 0;
    if (!digital_pin_choice_decode(context, PULL_CHOICES, &pin, &value)) {
        return SCPI_RES_ERR;
    }

    digital_gpio_pull_set((uint32_t)pin, (DigitalPull)value);
    return SCPI_RES_OK;
}

scpi_result_t SCPI_DigitalPinPullQ(scpi_t* context)
{
    int32_t pin = 0;
    if (!digital_pin_decode(context, &pin)) {
        return SCPI_RES_ERR;
    }

    DigitalPull pull = digital_gpio_pull_get((uint32_t)pin);

    digital_name_result(context, PULL_NAMES, (int32_t)pull);
    return SCPI_RES_OK;
}

scpi_result_t SCPI_DigitalPinLevel(scpi_t* context)
{
    int32_t pin = 0;
    if (!digital_pin_decode(context, &pin)) {
        return SCPI_RES_ERR;
    }

    bool state = false;

    // Read first parameter if present
    if (!SCPI_ParamBool(context, &state, TRUE)) {
        return SCPI_RES_ERR;
    }

    digital_gpio_level_set((uint32_t)pin, state);
    return SCPI_RES_OK;
}

scpi_result_t SCPI_DigitalPinLevelQ(scpi_t* context)
{
    int32_t pin = 0;
    if (!digital_pin_decode(context, &pin)) {
        return SCPI_RES_ERR;
    }

    SCPI_ResultBool(context, digital_gpio_level_get((uint32_t)pin));
    return SCPI_RES_OK;
}

scpi_result_t SCPI_DigitalReset(scpi_t* context)
{
    (void)context;

    digital_gpio_reset();
    return SCPI_RES_OK;
}

/**
 * @brief Decode <n> suffix of the PIN<n> header into a pin number.
 *
 * @param context
 * @param pin Decoded pin number, 1 to DIGITAL_PIN_COUNT.
 * @return scpi_bool_t FALSE if the suffix is out of range.
 */
static scpi_bool_t digital_pin_decode(scpi_t* context, int32_t* pin)
{
    int32_t numbers[1] = {0};

    // Pin 1 is used when the suffix is omitted
    SCPI_CommandNumbers(context, numbers, 1, 1);

    if (numbers[0] < 1 || numbers[0] > DIGITAL_PIN_COUNT) {
        SCPI_ErrorPush(context, SCPI_ERROR_DATA_OUT_OF_RANGE);
        return FALSE;
    }

    *pin = numbers[0];
    return TRUE;
}

/**
 * @brief Read an enumerated parameter of a PIN<n> command.
 *
 * @param context
 * @param choices Accepted values of the parameter.
 * @param pin Decoded pin number, 1 to DIGITAL_PIN_COUNT.
 * @param value Decoded value of the parameter.
 * @return scpi_bool_t FALSE if the suffix or the parameter is invalid.
 */
static scpi_bool_t digital_pin_choice_decode(
    scpi_t*                  context,
    const scpi_choice_def_t* choices,
    int32_t*                 pin,
    int32_t*                 value
)
{
    if (!digital_pin_decode(context, pin)) {
        return FALSE;
    }

    // Read first parameter if present
    if (!SCPI_ParamChoice(context, choices, value, TRUE)) {
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief Answer a query with a short form of an enumerated value.
 *
 * @param context
 * @param names Short forms of the enumerated values.
 * @param value Value to reply.
 */
static void digital_name_result(
    scpi_t* context, const char* const* names, int32_t value
)
{
    const char* name = names[value];
    SCPI_ResultCharacters(context, name, strlen(name));
}
