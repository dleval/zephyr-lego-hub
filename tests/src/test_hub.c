#include <zephyr/ztest.h>

#include "hub.h"


extern const e_hub_port_t hub_port_list[9];

#define HUB_PORT_NBR    (sizeof(hub_port_list) / sizeof(hub_port_list[0]))

extern s_device_handle_t device_handle[HUB_PORT_NBR];

ZTEST_SUITE(tests_hub, NULL, NULL, NULL, NULL, NULL);


/**
 * @brief Test hub init
 */
ZTEST(tests_hub, test_init_hub)
{
	//GIVEN hub not initialized with ble status OK
    //WHEN hub_init is called
    e_hub_status_t status = hub_init();
    //THEN hub_init returns HUB_NO_ERROR, all ports are in default state
    zassert_equal(status, HUB_NO_ERROR, "hub_init failed");
    for(uint8_t i = 0; i < HUB_PORT_NBR; i++)
    {
        zassert_equal(device_handle[i].port, hub_port_list[i], "port not initialized");
        zassert_equal(device_handle[i].type, UNKNOWNDEVICE, "port not initialized");
        zassert_equal(device_handle[i].p_port_output_callback, NULL, "port not initialized");
        zassert_equal(device_handle[i].initialized, false, "port not initialized");
    }

    //GIVEN hub not initialized with ble status ERROR
    //WHEN hub_init is called
    //THEN hub_init returns HUB_ERROR_INIT
}

/**
 * @brief Test hub attach device
 */
void led_output_callback(e_hub_port_t port, uint8_t data)
{
    //dummy
}
ZTEST(tests_hub, test_attach_device)
{
    //GIVEN hub initialized
    hub_init();
    //WHEN hub_attach_device is called with valid port and type
    e_hub_status_t status = hub_attach_device(LED, RGB_LED, &led_output_callback);
    //THEN hub_attach_device returns HUB_NO_ERROR
    zassert_equal(status, HUB_NO_ERROR, "hub_attach_device failed");
    //AND device is attached to the port
    zassert_equal(device_handle[5].port, LED, "wrong port");
    zassert_equal(device_handle[5].type, RGB_LED, "wrong device type");
    zassert_equal_ptr(device_handle[5].p_port_output_callback, &led_output_callback, "callback not set");
    zassert_true(device_handle[5].initialized, "port not initialized");

    //GIVEN hub initialized
    hub_init();
    //WHEN hub_attach_device is called with invalid port
    status = hub_attach_device(0xFE, RGB_LED, &led_output_callback);
    //THEN hub_attach_device returns HUB_UNKNOWN_PORT
    zassert_equal(status, HUB_UNKNOWN_PORT, "hub_attach_device failed");

    //GIVEN hub initialized
    hub_init();
    //WHEN hub_attach_device is called with NULL callback
    status = hub_attach_device(LED, RGB_LED, NULL);
    //THEN hub_attach_device returns HUB_CALLBACK_NULL
    zassert_equal(status, HUB_CALLBACK_NULL, "hub_attach_device failed");
    zassert_false(device_handle[5].initialized, "port initialized");
}
