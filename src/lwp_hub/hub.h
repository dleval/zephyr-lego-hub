/**
 * @file hub.h
 * @author David Leval
 * @brief Hub
 * @version 1.0
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef HUB_H
#define HUB_H

#include <stdint.h>
#include "lwp_types.h"

typedef enum {
   HUB_NO_ERROR = 0,
   HUB_ERROR_INIT = -1,
   HUB_UNKNOWN_PORT = -2,
   HUB_CALLBACK_NULL = -3
} e_hub_status_t;

typedef void (*p_port_output_callback_t)(e_hub_port_t port, uint8_t data);

typedef struct
{
    e_hub_port_t port;
    e_device_type_t type;
    p_port_output_callback_t p_port_output_callback;
    bool initialized;
} s_device_handle_t;


e_hub_status_t hub_init(void);
e_hub_status_t hub_attach_device(e_hub_port_t port, e_device_type_t type, p_port_output_callback_t p_port_output_callback);
e_hub_status_t hub_detach_device(e_hub_port_t port);

e_device_type_t hub_get_attached_device_type(e_hub_port_t port);

#endif // HUB_H
