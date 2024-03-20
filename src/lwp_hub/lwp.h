/**
 * @file lwp.h
 * @author David Leval
 * @brief Lego Wireless Protocol
 * @version 1.0
 * @date 2024-03-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdint.h>
#include <stdbool.h>

#define LWP_HEADER_SIZE          3
#define LWP_MAX_ADV_NAME_SIZE    14

#define LWP_MSG_DATA_SIZE_MAX    64

#define LWP_HUB_NAME            "TrainHub"

#define LWP_HUB_SERVICE_UUID     "00001623-1212-efde-1623-785feabcd123"
#define LWP_HUB_CHAR_UUID        "00001624-1212-efde-1623-785feabcd123"

typedef enum {
   LWP_STATUS_NO_RESP = 0,
   LWP_STATUS_RESP = 1,
   LWP_UNKNOWN_TYPE = -1,
   LWP_ERROR_DATA = -2,
   LWP_ERROR_MSG_LENGHT = -3,
   LWP_ERROR_PROPERTY = -4
} e_lwp_status_t;

/**
 * @brief Lego Wireless Protocol message reception
 * 
 * @param in_data 
 * @param in_size 
 * @param rsp_data 
 * @param rsp_size 
 * @return e_lwp_status_t Indicate if a response is available
 */
e_lwp_status_t lwp_message_income(uint8_t* in_data, uint16_t in_size, uint8_t* rsp_data, uint16_t* rsp_size);
e_lwp_status_t lwp_attach_device_notify(uint8_t port, uint16_t device_type, uint8_t* rsp_data, uint16_t* rsp_size);
e_lwp_status_t lwp_port_value_uint8(uint8_t port, uint8_t value, uint8_t* rsp_data, uint16_t* rsp_size);
e_lwp_status_t lwp_port_value_uint16(uint8_t port, uint16_t value, uint8_t* rsp_data, uint16_t* rsp_size);
e_lwp_status_t lwp_port_value_uint32(uint8_t port, uint32_t value, uint8_t* rsp_data, uint16_t* rsp_size);
e_lwp_status_t lwp_port_value_float(uint8_t port, float value, uint8_t* rsp_data, uint16_t* rsp_size);