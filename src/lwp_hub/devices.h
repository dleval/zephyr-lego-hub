/**
 * @file devices.h
 * @author David Leval
 * @brief Devices parameters
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _DEVICES_H_
#define _DEVICES_H_

#include "devices_types.h"
#include "lwp_types.h"

const char* get_device_name(const e_device_type_t device_id);
const uint8_t* get_device_raw(const e_device_type_t device_id);
const uint8_t* get_device_pct(const e_device_type_t device_id);
const uint8_t* get_device_si(const e_device_type_t device_id);
const char* get_device_symbol(const e_device_type_t device_id);
const uint16_t* get_device_mapping(const e_device_type_t device_id);
const uint8_t* get_device_motor_bias(const e_device_type_t device_id);
const uint8_t* get_device_capability_bits(const e_device_type_t device_id);
const uint8_t* get_device_value_format(const e_device_type_t device_id);
const uint8_t* get_device_hw_version(const e_device_type_t device_id);
const uint8_t* get_device_fw_version(const e_device_type_t device_id);

#endif // _DEVICES_H_
