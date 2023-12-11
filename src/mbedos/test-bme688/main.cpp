#include "mbed.h"
#include <stdio.h>

#include "bme688/BME688.h"

static EventQueue mainThread(5 *EVENTS_EVENT_SIZE);

mbed::DigitalOut led0(PA_15);
mbed::DigitalOut led1(PA_1);
BME688 iaq_sensor;

void checkDataAndPost();

int main()
{
    // disable on-board led
    led0 = 0;
    led1 = 0;

    iaq_sensor.initialise();
    iaq_sensor.doMeasurements();
    mainThread.call_every(15s, checkDataAndPost );

    mainThread.dispatch_forever();
}

void checkDataAndPost()
{
    printf("checking if data is available\n\r");
    if(iaq_sensor.isNewDataAvailable())
    {
        printf("data available!\n\r"); 
        iaq_sensor.dumpData();
    }
}

// /*
//  * Copyright (c) 2020 Arm Limited and affiliates.
//  * SPDX-License-Identifier: Apache-2.0
//  */
// #include "mbed.h"

// Thread thread;

// mbed::DigitalOut led0(PA_1);
// DigitalOut led1(PA_15);
// volatile bool running = true;

// // Blink function toggles the led in a long running loop
// void blink(DigitalOut *led)
// {
//     while (running) {
//         *led = !*led;
//         ThisThread::sleep_for(1000);
//     }
// }

// // Spawns a thread to run blink for 5 seconds
// int main()
// {
//     led0 = 0;
//     thread.start(callback(blink, &led1));
//     while ( 1 )
//     {
//         ThisThread::sleep_for(5000ms);
//     }
//     // running = false;
//     thread.join();
// }


