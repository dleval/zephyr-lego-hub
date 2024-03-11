/**
 * @file hub.c
 * 
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <stddef.h>
#include "hub.h"
#include "ble.h"

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
    }
}

e_hub_status_t hub_attach_device(e_hub_port_t port, e_device_type_t type, p_port_output_callback_t p_port_output_callback)
{
    if(p_port_output_callback == NULL) return HUB_CALLBACK_NULL;

    for(uint8_t i = 0; i < HUB_PORT_NBR; i++)
    {
        if(device_handle[i].port == port)
        {
            device_handle[i].type = type;
            device_handle[i].p_port_output_callback = p_port_output_callback;
            device_handle[i].initialized = true;
            return HUB_NO_ERROR;
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