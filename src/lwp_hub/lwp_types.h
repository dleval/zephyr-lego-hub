/**
 * @file lwp_types.h
 * @author David Leval
 * @brief Lego Wireless Protocol types
 * @version 1.0
 * @date 2024-03-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef LWP_TYPES_H
#define LWP_TYPES_H

#include <stdint.h>

typedef enum
{
  HUB_PROPERTIES = 0x01,
  HUB_ACTIONS = 0x02,
  HUB_ALERTS = 0x03,
  HUB_ATTACHED_IO = 0x04,
  GENERIC_ERROR_MESSAGES = 0x05,
  HW_NETWORK_COMMANDS = 0x08,
  FW_UPDATE_GO_INTO_BOOT_MODE = 0x10,
  FW_UPDATE_LOCK_MEMORY = 0x11,
  FW_UPDATE_LOCK_STATUS_REQUEST = 0x12,
  FW_LOCK_STATUS = 0x13,
  PORT_INFORMATION_REQUEST = 0x21,
  PORT_MODE_INFORMATION_REQUEST = 0x22,
  PORT_INPUT_FORMAT_SETUP_SINGLE = 0x41,
  PORT_INPUT_FORMAT_SETUP_COMBINEDMODE = 0x42,
  PORT_INFORMATION = 0x43,
  PORT_MODE_INFORMATION = 0x44,
  PORT_VALUE_SINGLE = 0x45,
  PORT_VALUE_COMBINEDMODE = 0x46,
  PORT_INPUT_FORMAT_SINGLE = 0x47,
  PORT_INPUT_FORMAT_COMBINEDMODE = 0x48,
  VIRTUAL_PORT_SETUP = 0x61,
  PORT_OUTPUT_COMMAND = 0x81,
  PORT_OUTPUT_COMMAND_FEEDBACK = 0x82,
} e_msg_type_t;

typedef enum 
{
  ADVERTISING_NAME = 0x01,
  BUTTON = 0x02,
  FW_VERSION = 0x03,
  HW_VERSION = 0x04,
  RSSI = 0x05,
  BATTERY_VOLTAGE = 0x06,
  BATTERY_TYPE = 0x07,
  MANUFACTURER_NAME = 0x08,
  RADIO_FIRMWARE_VERSION = 0x09,
  LEGO_WIRELESS_PROTOCOL_VERSION = 0x0A,
  SYSTEM_TYPE_ID = 0x0B,
  HW_NETWORK_ID = 0x0C,
  PRIMARY_MAC_ADDRESS = 0x0D,
  SECONDARY_MAC_ADDRESS = 0x0E,
  HARDWARE_NETWORK_FAMILY = 0x0F,
} e_property_reference_t;

typedef enum
{
  SET_DOWNSTREAM = 0x01,
  ENABLE_UPDATES_DOWNSTREAM = 0x02,
  DISABLE_UPDATES_DOWNSTREAM = 0x03,
  RESET_DOWNSTREAM = 0x04,
  REQUEST_UPDATE_DOWNSTREAM = 0x05,
  UPDATE_UPSTREAM = 0x06,
} e_property_operation_t;

typedef struct {
   uint16_t lenght;
   uint8_t hub_id;
   e_msg_type_t type;
} s_common_header_t;

// typedef struct {
//    s_common_header_t header;
//    uint8_t p_msg_data[LWP_MSG_DATA_SIZE_MAX];
//    uint16_t msg_size;
// } s_lwp_msg_t;

#endif // LWP_TYPES_H
