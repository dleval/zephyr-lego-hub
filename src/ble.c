#include <stdio.h>
#include <zephyr/kernel.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "ble.h"
#include "lwp.h"

/* Button value. */
static uint16_t but_val;

/* Prototype */
static ssize_t gatt_write_callback(struct bt_conn *conn,
		    const struct bt_gatt_attr *attr, const void *buf,
		    uint16_t len, uint16_t offset, uint8_t flags);

static ssize_t gatt_read_callback(struct bt_conn *conn,
		    const struct bt_gatt_attr *attr, void *buf,
		    uint16_t len, uint16_t offset);

/* Hub Custom Service  */
static const struct bt_uuid_128 hub_service_uuid = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x00001623, 0x1212, 0xefde, 0x1623, 0x785feabcd123));

/* Hub characteristic service */
static const struct bt_uuid_128 hub_char_uuid = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x00001624, 0x1212, 0xefde, 0x1623, 0x785feabcd123));

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
#define ADV_LEN 8

/* Advertising data */
static uint8_t manuf_data[ADV_LEN] = {
	0x97, 0x03, 0x00, 0x41, 0x07, 0x00, 0x63, 0x00
};

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
	BT_DATA(BT_DATA_MANUFACTURER_DATA, manuf_data, ADV_LEN)
};

/* BLE connection */
struct bt_conn *ble_conn;
/* Notification state */
volatile bool notify_enable;
volatile bool ble_connected;

static void mpu_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	ARG_UNUSED(attr);
	notify_enable = (value == BT_GATT_CCC_NOTIFY);
	printf("Notification %s\n", notify_enable ? "enabled" : "disabled");
    // static uint8_t resp_data[0x0F] = {0x0F, 0x00, 0x04, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10};
    // if(notify_enable) bt_gatt_notify(NULL, attr, resp_data, 0x0F);
}

/* The embedded board is acting as GATT server.
 * The ST BLE Android app is the BLE GATT client.
 */

/* ST BLE Sensor GATT services and characteristic */

BT_GATT_SERVICE_DEFINE(
    stsensor_svc,
    BT_GATT_PRIMARY_SERVICE(&hub_service_uuid),
    BT_GATT_CHARACTERISTIC( &hub_char_uuid.uuid,
		                    BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP | BT_GATT_CHRC_NOTIFY,
		                    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, 
                            gatt_read_callback, 
                            gatt_write_callback, 
                            &but_val),
    BT_GATT_CCC(mpu_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

static ssize_t gatt_write_callback(struct bt_conn *conn,
		    const struct bt_gatt_attr *attr, const void *buf,
		    uint16_t len, uint16_t offset, uint8_t flags)
{
    static uint8_t resp_data[LWP_MSG_DATA_SIZE_MAX];
    static uint16_t resp_size;
    printf("gatt_write_callback, len: %d, offset %d\n", len, offset);
    for(uint16_t i = offset; i<len; i++) {
        printf("%02X ", ((uint8_t*)buf)[i]);
    }
    printf("\n");
    if(lwp_message_income((uint8_t*)buf, len, resp_data, &resp_size) == LWP_STATUS_RESP) {
        printf("Response (size=%d): ", resp_size);
        for(uint16_t i = 0; i<resp_size; i++) {
            printf("%02X ", resp_data[i]);
        }
        printf("\n");
        if(notify_enable) bt_gatt_notify(conn, attr, resp_data, resp_size);
    }
	return 0;
}

int ble_char_notify(uint8_t* data, uint16_t size)
{
    if(notify_enable) {
        int ret = bt_gatt_notify(NULL, &stsensor_svc.attrs[1], data, size);
        printf("bt_gatt_notify: size %d ret %d data: ", size, ret);
        for(uint16_t i = 0; i<size; i++) {
            printf("%02X ", data[i]);
        }
        printf("\n");
        return ret;
    }
    return -1;
}

static ssize_t gatt_read_callback(struct bt_conn *conn,
		    const struct bt_gatt_attr *attr, void *buf,
		    uint16_t len, uint16_t offset)
{
    printf("gatt_read_callback, len: %d, offset %d\n", len, offset);
    return 0;
}

static void bt_ready(int err)
{
	if (err) {
		printf("Bluetooth init failed (err %d)\n", err);
		return;
	}
	printf("Bluetooth initialized\n");
	/* Start advertising */
	err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printf("Advertising failed to start (err %d)\n", err);
		return;
	}

	printf("Configuration mode: waiting connections...\n");
}

static void connected(struct bt_conn *connected, uint8_t err)
{
	if (err) {
		printf("Connection failed (err %u)\n", err);
	} else {
		printf("Connected\n");
		if (!ble_conn) {
			ble_conn = bt_conn_ref(connected);
		}
	}
}

static void disconnected(struct bt_conn *disconn, uint8_t reason)
{
	if (ble_conn) {
		bt_conn_unref(ble_conn);
		ble_conn = NULL;
	}

	printf("Disconnected (reason %u)\n", reason);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

int init_ble() {
    int err;

    err = bt_enable(bt_ready);
    if (err) {
        printf("Bluetooth init failed (err %d)\n", err);
    }
    return err;
}

bool ble_is_connected(void) {
    return ble_connected;
}

bool ble_is_notify_enable(void) {
    return notify_enable;
}