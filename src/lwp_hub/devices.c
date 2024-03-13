/**
 * @file devices.c
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stddef.h>
#include "devices.h"

#include "0x0002_train_motor.h"
#include "0x0008_light.h"
#include "0x0017_rgb_led.h"

const s_devices_types_t unknown_device_params = {
    .name = "UNKNOWN",
    .raw = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    .pct = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    .si = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    .symbol = "",
    .mapping = 0x0000,
    .motor_bias = 0x00,
    .capability_bits = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    .value_format = {0x00, 0x00, 0x00, 0x00}
};

const s_devices_types_t* get_device_params(e_device_type_t device_id)
{
    switch(device_id)
    {
        case TRAIN_MOTOR: return &train_motor_params;
        case LIGHT: return &light_params;
        case RGB_LED: return &led_rgb_params;
        default: return &unknown_device_params;
    }
}

const char* get_device_name(const e_device_type_t device_id)
{
    const s_devices_types_t* p_devices_params = get_device_params(device_id);
    return p_devices_params->name;
}

const uint8_t* get_device_raw(const e_device_type_t device_id)
{
    const s_devices_types_t* p_devices_params = get_device_params(device_id);
    return p_devices_params->raw;
}

const uint8_t* get_device_pct(const e_device_type_t device_id)
{
    const s_devices_types_t* p_devices_params = get_device_params(device_id);
    return p_devices_params->pct;
}
const uint8_t* get_device_si(const e_device_type_t device_id)
{
    const s_devices_types_t* p_devices_params = get_device_params(device_id);
    return p_devices_params->si;
}
const char* get_device_symbol(const e_device_type_t device_id)
{
    const s_devices_types_t* p_devices_params = get_device_params(device_id);
    return p_devices_params->symbol;
}

const uint16_t* get_device_mapping(const e_device_type_t device_id)
{
    const s_devices_types_t* p_devices_params = get_device_params(device_id);
    return &p_devices_params->mapping;
}
const uint8_t* get_device_motor_bias(const e_device_type_t device_id)
{
    const s_devices_types_t* p_devices_params = get_device_params(device_id);
    return &p_devices_params->motor_bias;
}
const uint8_t* get_device_capability_bits(const e_device_type_t device_id)
{
    const s_devices_types_t* p_devices_params = get_device_params(device_id);
    return p_devices_params->capability_bits;
}
const uint8_t* get_device_value_format(const e_device_type_t device_id)
{
    const s_devices_types_t* p_devices_params = get_device_params(device_id);
    return p_devices_params->value_format;
}