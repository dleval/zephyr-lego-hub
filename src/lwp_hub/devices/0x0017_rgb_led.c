/**
 * @file 0x0017_rgb_led.c
 * @author David Leval
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdbool.h>
#include "0x0017_rgb_led.h"
#include "lwp_types.h"
#include "hub.h"

const s_devices_types_t led_rgb_params = {
    .name = "LED-RGB",
    .raw = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x41},
    .pct = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC8, 0x42},
    .si = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x41},
    .symbol = "",
    .mapping = 0x0044,
    .motor_bias = 0x00,
    .capability_bits = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    .value_format = {0x01, 0x00, 0x01, 0x00},
    .hw_version = {0x00, 0x00, 0x00, 0x10},
    .fw_version = {0x00, 0x00, 0x00, 0x10}
};

e_rgb_led_color_t led_rgb_decode_output_cmd(e_port_output_subcommand_t subcmd, uint8_t* data, uint8_t size)
{
    e_rgb_led_color_t led_color = LED_COLOR_NONE;

    if (subcmd == SUBCMD_WRITE_DIRECT_MODE_DATA)
    {
        led_color = (e_rgb_led_color_t)data[1];
    }

    return led_color;
}

void led_rgb_send_value(e_hub_port_t port, e_rgb_led_color_t color)
{
    hub_send_port_value_uint16(port, (uint16_t)color);
}
