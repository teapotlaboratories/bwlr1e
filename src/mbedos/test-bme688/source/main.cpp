/*
 * Copyright (c) 2006-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"

int main()
{
    // wait a while
    ThisThread::sleep_for(11s);

    auto now_ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(Kernel::Clock::now()).time_since_epoch().count(); // Convert time_point to one in microsecond accuracy
    printf("epoch printf %lld\n", now_ns);
}


// #include "mbed.h"
// #include "bme688/BME688.h"

// #define ARRAY_LEN(array)				(sizeof(array)/sizeof(array[0]))

// #define FEATHER_RAK3172
// // #define BWLR1E

// // define on-board led to use
// #ifdef FEATHER_RAK3172
// mbed::DigitalOut led0(PA_8);
// #endif
// #ifdef BWLR1E
// mbed::DigitalOut led0(PA_15);
// mbed::DigitalOut led1(PA_1);
// #endif

// // Spawns a thread to measurement every 500ms
// Thread measure_thread;

// // measure environment 
// static void measure(BME688* bme688);

// int main()
// {

// // turn-off led for now
//     led0 = 0;
// #ifdef BWLR1E
//     led01 = 0;
// #endif

//     // initialise bmr688
//     BME688 iaq_sensor( I2C_SDA, I2C_SCL, 0x77 );

//     /* Desired subscription list of BSEC2 outputs */
//     bsec_virtual_sensor_t sensor_list[] = {
//             BSEC_OUTPUT_IAQ,
//             BSEC_OUTPUT_RAW_TEMPERATURE,
//             BSEC_OUTPUT_RAW_PRESSURE,
//             BSEC_OUTPUT_RAW_HUMIDITY,
//             BSEC_OUTPUT_RAW_GAS,
//             BSEC_OUTPUT_STABILIZATION_STATUS,
//             BSEC_OUTPUT_RUN_IN_STATUS
//     };
//     BME688::ReturnCode result = iaq_sensor.Initialise( sensor_list, ARRAY_LEN(sensor_list), BSEC_SAMPLE_RATE_LP );
//     if( result != BME688::ReturnCode::kOk )
//     {
//         while( true )
//         {
//             // blink led infinity to indicate bme688
//             // fail to initialise
//             ThisThread::sleep_for(500ms);
//             led0 = !led0;
//         }
//     }

//     // start measurement thread
//     measure_thread.start(callback(measure, &iaq_sensor));
//     while ( true )
//     {
//         ThisThread::sleep_for(15s);        
//         printf("checking if data is available\n\r");
//         // if(iaq_sensor.IsNewDataAvailable())
//         // {
//         //     // toggle pin if new data found
//         //     led0 = !led0;
//         //     printf("data available!\n\r"); 
            
//         //     // enable platform.stdio-buffered-serial in mbed_app.json 
//         //     // to see the dumped data
//         //     // iaq_sensor.DumpData();
//         // }
//     }
//     measure_thread.join();
// }

// // measure environment 
// void measure(BME688* bme688)
// {
//     while ( true ) {
//         // bme688->DoMeasurements();
//         ThisThread::sleep_for(500ms);
//     }
// }
