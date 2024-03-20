/**
 * @file devices_types.h
 * @author david Leval
 * @brief 
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _DEVICES_TYPES_H_
#define _DEVICES_TYPES_H_

#include <stdint.h>

typedef struct
{
    char* name;
    uint8_t raw[8];
    uint8_t pct[8];
    uint8_t si[8];
    char* symbol;
    uint16_t mapping;
    uint8_t motor_bias;
    uint8_t capability_bits[6];
    uint8_t value_format[4];
    uint8_t hw_version[4];
    uint8_t fw_version[4];
    
} s_devices_types_t;

#endif // _DEVICES_TYPES_H_
