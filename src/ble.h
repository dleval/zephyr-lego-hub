#ifndef _BLE_H
#define _BLE_H

#include <stdint.h>

int init_ble(void);
int ble_char_notify(uint8_t* data, uint16_t size);
bool ble_is_connected(void);
bool ble_is_notify_enable(void);

#endif // _BLE_H
