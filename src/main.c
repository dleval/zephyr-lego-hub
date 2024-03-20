/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#include "hub.h"
#include "lwp_types.h"
#include "ble.h"
#include "0x0017_rgb_led.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000


/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(blinking_led), gpios);

e_port_feedback_message_t led_output_callback(e_hub_port_t port, e_port_output_subcommand_t subcmd, uint8_t* data, uint8_t size);

int main(void)
{
	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

    k_msleep(SLEEP_TIME_MS);
    printf("\nESP32 Lego Hub\n");

    hub_init();
    while((ble_is_connected() == false) && ble_is_notify_enable() == false)
    {
        ret = gpio_pin_toggle_dt(&led);
        k_msleep(100);
    }
    k_msleep(SLEEP_TIME_MS);
    hub_attach_device(LED, RGB_LED, &led_output_callback);

	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return 0;
		}

		led_state = !led_state;
		// printf("LED state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

e_port_feedback_message_t led_output_callback(e_hub_port_t port, e_port_output_subcommand_t subcmd, uint8_t* data, uint8_t size)
{
    printf("LED port: %d, subcmd: 0x%X\n", port, subcmd);

    printf("Data:");
    for(uint8_t i = 0; i < size; i++)
    {
        printf(" 0x%X", data[i]);
    }
    printf("\n");

    e_rgb_led_color_t led_color = led_rgb_decode_output_cmd(subcmd, data, size);
    printf("LED color: %d\n", led_color);

    if(hub_notification_enabled(port) == true)
    {
        printf("Notification color\n");
        led_rgb_send_value(port, led_color);
    }

    return BUFFER_EMPTY_AND_COMMAND_COMPLETED | IDLE;
}