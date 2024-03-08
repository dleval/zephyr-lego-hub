/**
 * @file hub.c
 * 
 */

#include <stdio.h>
#include <zephyr/kernel.h>

#include "hub.h"
#include "ble.h"

void hub_init(void)
{
    printf("\nBLE init ... ");
    if(init_ble() == 0) printf("OK\n");
    else return;
}