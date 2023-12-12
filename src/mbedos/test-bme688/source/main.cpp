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

    iaq_sensor.Initialise();
    iaq_sensor.DoMeasurements();
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
//         // *led = !*led;
//         ThisThread::sleep_for(1000ms);
//     }
// }

// // Spawns a thread to run blink for 5 seconds
// int main()
// {
//     led0 = 0;
//     led1 = 0;
//     thread.start(callback(blink, &led1));
//     while ( 1 )
//     {
//         ThisThread::sleep_for(5000ms);
//     }
//     // running = false;
//     thread.join();
// }


// /* 
//  * Deep-sleep test 
//  * Compiled in `Develop` or Release` build. The following is the expected behavior: 
//  * a. RED LED ( PA1 ) is on for 5 seconds. Consumed around 1.5ma 
//  * b. RED LED ( PA1 ) is off for 10 seconds. Consumed around 10ua 
//  * c. Repeat to inifinity
//  *
//  */

// #include <mbed.h>
// // #include "bme680/BME680.h"

// mbed::DigitalOut led0(PA_15);
// mbed::DigitalOut led1(PA_1);

// uint32_t sram_data = 0;

// int main()
// {
//     // disable on-board LED
//     led0 = 0;
//     led1 = 0;

//     // I2C driver need to be free, when not used. BME680 object is in a scoped area
//     // so that once it exits, the object is destroyed and I2C is freed.
//     // this feature is not in upstream mbed-os sdk. see readme for more details.
//     {
//         I2C i2c(I2C_SDA, I2C_SCL);
//     }
    
//     // set I2C pin as high-impedance
//     mbed::DigitalIn sda(I2C_SDA);
//     mbed::DigitalIn scl(I2C_SCL);
//     bool read_dummy = sda;
//     read_dummy = scl;

//     sram_data = 0xfeedbeef;

//     while (1) {
//         // data should still be retained even after deep-sleep
//         if ( sram_data == 0xfeedbeef){
//             led0 = 1;
//             // go to deep-sleep STOP2
//             ThisThread::sleep_for(1000ms);
//             led0 = 0;
//         }
//         ThisThread::sleep_for(10000ms); 
//     }
// }
