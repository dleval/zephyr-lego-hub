/**
 * @file lwp.c
 * 
 */

#include "lwp.h"
#include "lwp_types.h"
#include <stddef.h>
#include <string.h>
#include "hub.h"
#include "devices.h"

#include <stdio.h>
#include <zephyr/kernel.h>


static s_common_header_t in_header_msg;

uint16_t get_lenght(uint8_t* in_data);
uint8_t get_hub_id(uint8_t* in_data, uint16_t lenght);
e_msg_type_t get_msg_type(uint8_t* in_data, uint16_t lenght);

void property_response(uint8_t* rsp_data, uint16_t* rsp_size, e_property_reference_t property_ref, uint8_t* data, uint16_t data_size);

e_lwp_status_t operation_hub_property(uint8_t* in_data, uint16_t in_size, uint8_t* rsp_data, uint16_t* rsp_size);
e_lwp_status_t operation_hub_alert(uint8_t* in_data, uint16_t in_size, uint8_t* rsp_data, uint16_t* rsp_size);
e_lwp_status_t operation_port_mode_inf(uint8_t* in_data, uint16_t in_size, uint8_t* rsp_data, uint16_t* rsp_size);

e_lwp_status_t lwp_message_income(uint8_t* in_data, uint16_t in_size, uint8_t* rsp_data, uint16_t* rsp_size)
{
    if((in_data == NULL) || (rsp_data == NULL) || (rsp_size == NULL) || (in_size < LWP_HEADER_SIZE))
    {
        return LWP_ERROR_DATA;
    }

    //Decode Header
    in_header_msg.lenght = get_lenght(in_data);
    in_header_msg.hub_id = get_hub_id(in_data, in_header_msg.lenght);
    in_header_msg.type = get_msg_type(in_data, in_header_msg.lenght);

    //    printf("LWP\nLenght: %d, Hub ID: %d, Type: %d\n", in_header_msg.lenght, in_header_msg.hub_id, in_header_msg.type);

    if(in_header_msg.lenght > LWP_MSG_DATA_SIZE_MAX)
    {
        return LWP_ERROR_MSG_LENGHT;
    }

    if(in_header_msg.type == HUB_PROPERTIES) return operation_hub_property(in_data, in_size, rsp_data, rsp_size);
    if(in_header_msg.type == HUB_ALERTS) return operation_hub_alert(in_data, in_size, rsp_data, rsp_size);
    if(in_header_msg.type == PORT_MODE_INFORMATION_REQUEST) return operation_port_mode_inf(in_data, in_size, rsp_data, rsp_size);

    printf("Unknown message type\n");
    return LWP_UNKNOWN_TYPE;
}

uint16_t get_lenght(uint8_t* in_data)
{
   if(in_data[0] & 0b1000000) return (uint16_t)((in_data[0] + in_data[1]) - 1);
   else return (uint16_t)in_data[0];
}

uint8_t get_hub_id(uint8_t* in_data, uint16_t lenght)
{
   if(lenght > 127) return in_data[2];
   else return in_data[1];
}

e_msg_type_t get_msg_type(uint8_t* in_data, uint16_t lenght)
{
   if(lenght > 127) return (e_msg_type_t)in_data[3];
   else return (e_msg_type_t)in_data[2];
}

void property_response(uint8_t* rsp_data, uint16_t* rsp_size, e_property_reference_t property_ref, uint8_t* data, uint16_t data_size)
{
    // printf("property_response\n");
   *rsp_size = (uint16_t)(data_size + 5);
   rsp_data[0] = (uint8_t)(data_size + 5);
   rsp_data[1] = (uint8_t)0;
   rsp_data[2] = (uint8_t)HUB_PROPERTIES;
   rsp_data[3] = (uint8_t)property_ref;
   rsp_data[4] = (uint8_t)UPDATE_UPSTREAM;
   for(uint8_t i = 0; i < data_size; i++)
   {
      rsp_data[i+5] = data[i];
   }
}

e_lwp_status_t operation_hub_property(uint8_t* in_data, uint16_t in_size, uint8_t* rsp_data, uint16_t* rsp_size)
{
   if(in_size < 5) return LWP_ERROR_MSG_LENGHT;

   e_lwp_status_t status = LWP_ERROR_PROPERTY;

   e_property_reference_t property_ref = (e_property_reference_t)in_data[3];
   e_property_operation_t property_op = (e_property_operation_t)in_data[4];

   switch(property_ref)
   {
        case ADVERTISING_NAME:
            if(property_op == REQUEST_UPDATE_DOWNSTREAM)
            {
                property_response(rsp_data, rsp_size, ADVERTISING_NAME, (uint8_t*)LWP_HUB_NAME, strlen(LWP_HUB_NAME));
                printf("Send hub name : %s\n", LWP_HUB_NAME);
                status = LWP_STATUS_RESP;
            }
            if(property_op == SET_DOWNSTREAM){}
            break;
        case HW_VERSION:
            if(property_op == REQUEST_UPDATE_DOWNSTREAM)
            {
                uint8_t hw_version[4] = {0x00, 0x00, 0x00, 0x01};
                property_response(rsp_data, rsp_size, HW_VERSION, hw_version, sizeof(hw_version));
                printf("Send hub hw version\n");
                status = LWP_STATUS_RESP;
            }
            break;
        case FW_VERSION:
            if(property_op == REQUEST_UPDATE_DOWNSTREAM)
            {
                uint8_t fw_version[4] = {0x00, 0x00, 0x02, 0x11};
                property_response(rsp_data, rsp_size, FW_VERSION, fw_version, sizeof(fw_version));
                printf("Send hub fw version\n");
                status = LWP_STATUS_RESP;
            }
            break;
        case RADIO_FIRMWARE_VERSION:
            if(property_op == REQUEST_UPDATE_DOWNSTREAM)
            {
                char* radio_fw_version = "2_02_01";
                property_response(rsp_data, rsp_size, RADIO_FIRMWARE_VERSION, (uint8_t*)radio_fw_version, strlen(radio_fw_version));
                printf("Send hub radio fw version\n");
                status = LWP_STATUS_RESP;
            }
            break;
        case MANUFACTURER_NAME:
            if(property_op == REQUEST_UPDATE_DOWNSTREAM)
            {
                char* manufacturer_name = "LEGO System A/S";
                property_response(rsp_data, rsp_size, MANUFACTURER_NAME, (uint8_t*)manufacturer_name, strlen(manufacturer_name));
                printf("Send hub manufacturer name\n");
                status = LWP_STATUS_RESP;
            }
            break;
        case BATTERY_TYPE:
            if(property_op == REQUEST_UPDATE_DOWNSTREAM)
            {
                uint8_t battery_type = 0x00;
                property_response(rsp_data, rsp_size, BATTERY_TYPE, &battery_type, sizeof(battery_type));
                printf("Send hub battery type\n");
                status = LWP_STATUS_RESP;
            }
            break;
        case LEGO_WIRELESS_PROTOCOL_VERSION:
            if(property_op == REQUEST_UPDATE_DOWNSTREAM)
            {
                uint8_t lwp_version[2] = {0x00, 0x03};
                property_response(rsp_data, rsp_size, LEGO_WIRELESS_PROTOCOL_VERSION, lwp_version, sizeof(lwp_version));
                printf("Send hub lwp version\n");
                status = LWP_STATUS_RESP;
            }
            break;
        case PRIMARY_MAC_ADDRESS:
            if(property_op == REQUEST_UPDATE_DOWNSTREAM)
            {
                uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
                property_response(rsp_data, rsp_size, PRIMARY_MAC_ADDRESS, mac, sizeof(mac));
                printf("Send hub pri. mac\n");
                status = LWP_STATUS_RESP;
            }
            break;
        case BUTTON:
            if(property_op == REQUEST_UPDATE_DOWNSTREAM)
            {
                uint8_t button = 0x00;
                property_response(rsp_data, rsp_size, BUTTON, &button, sizeof(button));
                printf("Button enable\n");
                status = LWP_STATUS_RESP;
            }
            break;
        case RSSI:
            if(property_op == REQUEST_UPDATE_DOWNSTREAM)
            {
                uint8_t rssi = 0xC8;
                property_response(rsp_data, rsp_size, RSSI, &rssi, sizeof(rssi));
                printf("RSSI enable\n");
                status = LWP_STATUS_RESP;
            }
            break;
        case BATTERY_VOLTAGE:
            if(property_op == REQUEST_UPDATE_DOWNSTREAM)
            {
                uint8_t battery_voltage = 0x47; //71%
                property_response(rsp_data, rsp_size, BATTERY_VOLTAGE, &battery_voltage, sizeof(battery_voltage));
                printf("Battery voltage\n");
                status = LWP_STATUS_RESP;
            }
            break;
        default:
            break;
   }
   return status;
}

e_lwp_status_t operation_hub_alert(uint8_t* in_data, uint16_t in_size, uint8_t* rsp_data, uint16_t* rsp_size)
{
    uint8_t alert_type = in_data[3];
    uint8_t alert_op = in_data[4];

    if(alert_op == 0x03)
    {
        *rsp_size = 6;
        rsp_data[0] = 6;
        rsp_data[1] = 0;
        rsp_data[2] = (uint8_t)HUB_ALERTS;
        rsp_data[3] = alert_type;
        rsp_data[4] = 0x04; // Alert Operation, Update (Upstream)
        rsp_data[5] = 0; //Status OK
        return LWP_STATUS_RESP;
    }
    return LWP_STATUS_NO_RESP;
}

e_lwp_status_t operation_port_mode_inf(uint8_t* in_data, uint16_t in_size, uint8_t* rsp_data, uint16_t* rsp_size)
{
    uint8_t port_id = in_data[3];
    e_device_type_t device_type = hub_get_attached_device_type(port_id);
    uint8_t mode = in_data[4];
    e_port_mode_info_type_t info_type = (e_port_mode_info_type_t)in_data[5];
    uint16_t payload_lenght = 0;
    const uint8_t* payload_data = NULL;
    e_lwp_status_t status = LWP_STATUS_NO_RESP;

    printf("Port mode info request: port_id %d device type %d info type %d\n", port_id, device_type, info_type);

    switch (info_type)
    {
        case INFO_TYPE_NAME:
            const char* device_name = get_device_name(device_type);
            payload_lenght = strlen(device_name);
            payload_data = (const uint8_t*)device_name;
            status = LWP_STATUS_RESP;
            break;
        case INFO_TYPE_RAW:
            payload_lenght = sizeof(((s_devices_types_t*)0)->raw);
            payload_data = get_device_raw(device_type);
            status = LWP_STATUS_RESP;
            break;
        case INFO_TYPE_VALUE_FORMAT:
            payload_lenght = sizeof(((s_devices_types_t*)0)->value_format);
            payload_data = get_device_value_format(device_type);
            status = LWP_STATUS_RESP;
            break;
        case INFO_TYPE_PCT:
            payload_lenght = sizeof(((s_devices_types_t*)0)->pct);
            payload_data = get_device_pct(device_type);
            status = LWP_STATUS_RESP;
            break;
        case INFO_TYPE_SI:
            payload_lenght = sizeof(((s_devices_types_t*)0)->si);
            payload_data = get_device_si(device_type);
            status = LWP_STATUS_RESP;
            break;
        case INFO_TYPE_SYMBOL:
            const char* device_symbol = get_device_symbol(device_type);
            payload_lenght = strlen(device_symbol);
            payload_data = (const uint8_t*)device_symbol;
            status = LWP_STATUS_RESP;
            break;
        case INFO_TYPE_MAPPING:
            payload_lenght = sizeof(((s_devices_types_t*)0)->mapping);
            payload_data = (const uint8_t*)get_device_mapping(device_type);
            status = LWP_STATUS_RESP;
            break;
        case INFO_TYPE_CAPABILITY_BIT:
            payload_lenght = sizeof(((s_devices_types_t*)0)->capability_bits);
            payload_data = get_device_capability_bits(device_type);
            status = LWP_STATUS_RESP;
            break;
        case INFO_TYPE_MOTOR_BIAS:
            payload_lenght = sizeof(((s_devices_types_t*)0)->motor_bias);
            payload_data = get_device_motor_bias(device_type);
            status = LWP_STATUS_RESP;
            break;
        default:
            break;
    }

    if(status == LWP_STATUS_RESP)
    {
        *rsp_size = 6 + payload_lenght;
        rsp_data[0] = *rsp_size; // Define size depending on the response
        rsp_data[1] = 0;
        rsp_data[2] = (uint8_t)PORT_MODE_INFORMATION;
        rsp_data[3] = port_id;
        rsp_data[4] = mode;
        rsp_data[5] = info_type;
        if(payload_data != NULL)
        {
            for(uint8_t i = 0; i < payload_lenght; i++)
            {
                rsp_data[i+6] = payload_data[i];
            }
        }
    }

    return status;
}