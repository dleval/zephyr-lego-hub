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
   HUB_CALLBACK_NULL = -3,
   HUB_ERROR_DEVICE = -4
} e_hub_status_t;

typedef e_port_feedback_message_t (*p_port_output_callback_t)(e_hub_port_t port, e_port_output_subcommand_t subcmd, uint8_t* data, uint8_t size);

typedef struct
{
    e_hub_port_t port;
    e_device_type_t type;
    p_port_output_callback_t p_port_output_callback;
    bool initialized;
    uint32_t delta_interval;
    bool notification_enabled;
} s_device_handle_t;


e_hub_status_t hub_init(void);
e_hub_status_t hub_attach_device(e_hub_port_t port, e_device_type_t type, p_port_output_callback_t p_port_output_callback);
e_hub_status_t hub_detach_device(e_hub_port_t port);

e_device_type_t hub_get_attached_device_type(e_hub_port_t port);
e_port_feedback_message_t hub_device_callback(e_hub_port_t port, e_port_output_subcommand_t subcmd, uint8_t* data, uint8_t size);

void hub_set_notification(e_hub_port_t port, bool enable);
void hub_set_delta_interval(e_hub_port_t port, uint32_t delta_interval);

bool hub_notification_enabled(e_hub_port_t port);

e_hub_status_t hub_send_port_value_uint8(e_hub_port_t port, uint8_t value);
e_hub_status_t hub_send_port_value_uint16(e_hub_port_t port, uint16_t value);
e_hub_status_t hub_send_port_value_uint32(e_hub_port_t port, uint32_t value);
e_hub_status_t hub_send_port_value_float(e_hub_port_t port, float value);

#endif // HUB_H
