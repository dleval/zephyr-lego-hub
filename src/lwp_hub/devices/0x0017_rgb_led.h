/**
 * @file 0x0017_rgb_led.h
 * @author David Leval
 * @brief RGB Led device parameters
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _0X0017_RGB_LED_H_
#define _0X0017_RGB_LED_H_

#include "devices_types.h"
#include "lwp_types.h"

typedef enum
{
    LED_COLOR_BLACK = 0,
    LED_COLOR_PINK = 1,
    LED_COLOR_PURPLE = 2,
    LED_COLOR_BLUE = 3,
    LED_COLOR_LIGHTBLUE = 4,
    LED_COLOR_CYAN = 5,
    LED_COLOR_GREEN = 6,
    LED_COLOR_YELLOW = 7,
    LED_COLOR_ORANGE = 8,
    LED_COLOR_RED = 9,
    LED_COLOR_WHITE = 10,
    LED_COLOR_NONE = 255
} e_rgb_led_color_t;

extern const s_devices_types_t led_rgb_params;

e_rgb_led_color_t led_rgb_decode_output_cmd(e_port_output_subcommand_t subcmd, uint8_t* data, uint8_t size);
void led_rgb_send_value(e_hub_port_t port, e_rgb_led_color_t color);

#endif // _0X0017_RGB_LED_H_
