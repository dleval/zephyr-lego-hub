/**
 * @file hub.c
 * 
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <stddef.h>
#include "hub.h"
#include "ble.h"
#include "lwp.h"

const e_hub_port_t hub_port_list[] = {A, B, AB, C, D, LED, TILT, CURRENT, VOLTAGE};

#define HUB_PORT_NBR    (sizeof(hub_port_list) / sizeof(hub_port_list[0]))

s_device_handle_t device_handle[HUB_PORT_NBR];

void init_port(void);

e_hub_status_t hub_init(void)
{
    printf("\nBLE init ... ");
    if(init_ble() == 0) printf("OK\n");
    else return HUB_ERROR_INIT;

    init_port();

    return HUB_NO_ERROR;
}

void init_port(void)
{
    for(uint8_t i = 0; i < HUB_PORT_NBR; i++)
    {
        device_handle[i].port = hub_port_list[i];
        device_handle[i].type = UNKNOWNDEVICE;
        device_handle[i].p_port_output_callback = NULL;
        device_handle[i].initialized = false;
        device_handle[i].delta_interval = 0;
        device_handle[i].notification_enabled = false;
    }
}

e_hub_status_t hub_attach_device(e_hub_port_t port, e_device_type_t type, p_port_output_callback_t p_port_output_callback)
{
    static uint8_t payload_data[16];
    static uint16_t payload_size = 0;

    if(p_port_output_callback == NULL) return HUB_CALLBACK_NULL;

    for(uint8_t i = 0; i < HUB_PORT_NBR; i++)
    {
        if(device_handle[i].port == port)
        {
            device_handle[i].type = type;
            device_handle[i].p_port_output_callback = p_port_output_callback;
            device_handle[i].initialized = true;
            if(lwp_attach_device_notify(port, type, payload_data, &payload_size) == LWP_STATUS_RESP)
            {
                printf("Attach device : type %d port %d\n", type, port);
                ble_char_notify(payload_data, payload_size);
                return HUB_NO_ERROR;
            }
            return HUB_ERROR_DEVICE;
        }
    }
    return HUB_UNKNOWN_PORT;
}

e_hub_status_t hub_detach_device(e_hub_port_t port)
{
    for(uint8_t i = 0; i < HUB_PORT_NBR; i++)
    {
        if(device_handle[i].port == port)
        {
            device_handle[i].type = UNKNOWNDEVICE;
            device_handle[i].p_port_output_callback = NULL;
            device_handle[i].initialized = false;
            device_handle[i].delta_interval = 0;
            device_handle[i].notification_enabled = false;
            return HUB_NO_ERROR;
        }
    }
    return HUB_UNKNOWN_PORT;
}

e_device_type_t hub_get_attached_device_type(e_hub_port_t port)
{
    for(uint8_t i = 0; i < HUB_PORT_NBR; i++)
    {
        if(device_handle[i].port == port)
        {
            return device_handle[i].type;
        }
    }
    return UNKNOWNDEVICE;
}

e_port_feedback_message_t hub_device_callback(e_hub_port_t port, e_port_output_subcommand_t subcmd, uint8_t* data, uint8_t size)
{
    for(uint8_t i = 0; i < HUB_PORT_NBR; i++)
    {
        if(device_handle[i].port == port)
        {
            if(device_handle[i].p_port_output_callback != NULL)
            {
                return device_handle[i].p_port_output_callback(port, subcmd, data, size);
            }
        }
    }
    return COMMANDS_DISCARDED;
}

void hub_set_notification(e_hub_port_t port, bool enable)
{
    for(uint8_t i = 0; i < HUB_PORT_NBR; i++)
    {
        if(device_handle[i].port == port)
        {
            device_handle[i].notification_enabled = enable;
        }
    }
}

void hub_set_delta_interval(e_hub_port_t port, uint32_t delta_interval)
{
    for(uint8_t i = 0; i < HUB_PORT_NBR; i++)
    {
        if(device_handle[i].port == port)
        {
            device_handle[i].delta_interval = delta_interval;
        }
    }
}

bool hub_notification_enabled(e_hub_port_t port)
{
    for(uint8_t i = 0; i < HUB_PORT_NBR; i++)
    {
        if(device_handle[i].port == port)
        {
            return device_handle[i].notification_enabled;
        }
    }
    return false;
}

e_hub_status_t hub_send_port_value_uint8(e_hub_port_t port, uint8_t value)
{
    static uint8_t payload_data[5];
    static uint16_t payload_size = 0;

    for(uint8_t i = 0; i < HUB_PORT_NBR; i++)
    {
        if(device_handle[i].port == port)
        {
            if(lwp_port_value_uint8(port, value, payload_data, &payload_size) == LWP_STATUS_RESP)
            {
                ble_char_notify(payload_data, payload_size);
                return HUB_NO_ERROR;
            }
        }
    }
    return HUB_UNKNOWN_PORT;
}

e_hub_status_t hub_send_port_value_uint16(e_hub_port_t port, uint16_t value)
{
    static uint8_t payload_data[6];
    static uint16_t payload_size = 0;

    for(uint8_t i = 0; i < HUB_PORT_NBR; i++)
    {
        if(device_handle[i].port == port)
        {
            if(lwp_port_value_uint16(port, value, payload_data, &payload_size) == LWP_STATUS_RESP)
            {
                ble_char_notify(payload_data, payload_size);
                return HUB_NO_ERROR;
            }
        }
    }
    return HUB_UNKNOWN_PORT;
}

e_hub_status_t hub_send_port_value_uint32(e_hub_port_t port, uint32_t value)
{
    static uint8_t payload_data[8];
    static uint16_t payload_size = 0;

    for(uint8_t i = 0; i < HUB_PORT_NBR; i++)
    {
        if(device_handle[i].port == port)
        {
            if(lwp_port_value_uint32(port, value, payload_data, &payload_size) == LWP_STATUS_RESP)
            {
                ble_char_notify(payload_data, payload_size);
                return HUB_NO_ERROR;
            }
        }
    }
    return HUB_UNKNOWN_PORT;
}

e_hub_status_t hub_send_port_value_float(e_hub_port_t port, float value)
{
    static uint8_t payload_data[8];
    static uint16_t payload_size = 0;

    for(uint8_t i = 0; i < HUB_PORT_NBR; i++)
    {
        if(device_handle[i].port == port)
        {
            if(lwp_port_value_float(port, value, payload_data, &payload_size) == LWP_STATUS_RESP)
            {
                ble_char_notify(payload_data, payload_size);
                return HUB_NO_ERROR;
            }
        }
    }
    return HUB_UNKNOWN_PORT;
}
