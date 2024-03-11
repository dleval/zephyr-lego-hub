#include <zephyr/ztest.h>
#include <string.h>
#include "lwp.h"
#include "lwp_types.h"
#include "hub.h"

ZTEST_SUITE(tests_lwp, NULL, NULL, NULL, NULL, NULL);

/**
 * @brief Test property get advertising name
 */
ZTEST(tests_lwp, test_property_get_advertising_name)
{
    //GIVEN a property get advertising name request
    uint8_t in_data[] = {0x05, 0x00, 0x01, 0x01, 0x05};
    uint8_t rsp_data[20];
    uint16_t rsp_size = 0;
    //WHEN lwp_message_income is called
    e_lwp_status_t status = lwp_message_income(in_data, sizeof(in_data), rsp_data, &rsp_size);
    //THEN lwp_message_income returns LWP_STATUS_RESP with the advertising name
    zassert_equal(status, LWP_STATUS_RESP, "lwp_message_income failed");
    uint16_t expected_rsp_size = strlen(LWP_HUB_NAME) + 5;
    zassert_equal(rsp_size, expected_rsp_size, "wrong response size");
    zassert_equal(rsp_data[0], expected_rsp_size, "wrong lwp header size");
    zassert_equal(rsp_data[1], 0, "wrong hub id");
    zassert_equal(rsp_data[2], HUB_PROPERTIES, "wrong message type");
    zassert_equal(rsp_data[3], ADVERTISING_NAME, "wrong property reference");
    zassert_equal(rsp_data[4], UPDATE_UPSTREAM, "wrong property operation");
    zassert_mem_equal(&rsp_data[5], LWP_HUB_NAME, strlen(LWP_HUB_NAME), "wrong advertising name");
}

/**
 * @brief Test read port mode information
 */
void tests_lwp_led_output_callback(e_hub_port_t port, uint8_t data)
{
    //dummy
}
ZTEST(tests_lwp, test_read_port_mode_info)
{
    //GIVEN a LED port attached to the hub
    hub_init();
    e_hub_status_t hub_status = hub_attach_device(LED, RGB_LED, &tests_lwp_led_output_callback);
    zassert_equal(hub_status, HUB_NO_ERROR, "hub_attach_device failed");
    //WHEN a read port mode information request is received
    uint8_t in_data[] = {0x05, 0x00, PORT_MODE_INFORMATION_REQUEST, LED, 0x00, INFO_TYPE_NAME};
    uint8_t rsp_data[20];
    uint16_t rsp_size = 0;
    e_lwp_status_t lwp_status = lwp_message_income(in_data, sizeof(in_data), rsp_data, &rsp_size);
    //THEN lwp_message_income returns LWP_STATUS_RESP with the port mode information
    zassert_equal(lwp_status, LWP_STATUS_RESP, "lwp_message_income failed");
}