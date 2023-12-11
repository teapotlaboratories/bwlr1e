/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"

Thread thread;
FlashIAP flash;

mbed::DigitalOut led0(PA_1);
DigitalOut led1(PA_15);

// Spawns a thread to run blink for 5 seconds
int main()
{
    led0 = 0;
    led1 = 0;

    uint32_t flash_start = flash.get_flash_start();
    uint32_t flash_size = flash.get_flash_size();
    uint32_t page_size = flash.get_page_size();

    printf("Flash start: %ld bytes\r\n", flash_start);
    printf("Flash size: %ld bytes\r\n", flash_size);
    printf("Page size: %ld bytes\r\n", page_size);

    while ( 1 )
    {
        ThisThread::sleep_for(5000ms);
    }
}


