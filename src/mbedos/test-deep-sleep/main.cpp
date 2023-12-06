
#include "mbed.h"
#include "mbed_stats.h"

int main() {
    
    while (1) {
        ThisThread::sleep_for(2000ms);

        mbed_stats_cpu_t stats;
        mbed_stats_cpu_get(&stats);
        printf("Uptime: %llu ", stats.uptime / 1000);
        printf("Sleep time: %llu ", stats.sleep_time / 1000);
        printf("Deep Sleep: %llu\n", stats.deep_sleep_time / 1000);
    }
}

// /*
//  * Copyright (c) 2020 Arm Limited and affiliates.
//  * SPDX-License-Identifier: Apache-2.0
//  */

// #include "mbed.h"

// // Create a BufferedSerial object with a default baud rate.
// static BufferedSerial serial_port(PA_2, PA_3);
// char buffer[1024];

// int main(void)
// {
//     // Set desired properties (9600-8-N-1).
//     serial_port.set_baud(115200);
//     serial_port.set_format(
//         /* bits */ 8,
//         /* parity */ BufferedSerial::None,
//         /* stop bit */ 1
//     );
    
//     while (1) {
//         ThisThread::sleep_for(2000ms);

//         mbed_stats_cpu_t stats;
//         mbed_stats_cpu_get(&stats);
//         snprintf( buffer, sizeof(buffer), "Uptime: %llu Sleep time: %llu Deep Sleep: %llu\r\n\0", stats.uptime / 1000, stats.sleep_time / 1000, stats.deep_sleep_time / 1000 );
//         serial_port.write(buffer, strlen(buffer) );
//     }
// }


// #include "mbed.h"
// #include "bme680.h"

// extern uint8_t deep_sleep_mode;

// BME680 bme688( I2C_SDA, I2C_SCL, 1);
// mbed::DigitalOut led0(PA_15);
// mbed::DigitalOut led1(PA_1);
// int main()
// {
//     led0 = 0;
//     led1 = 0;

//     deep_sleep_mode = 1;

    
//     auto init_sensor = bme688.init();
//     printf(" bme688 sensor successful? %d \n", init_sensor);
//     //Select oversampling for T, P, H
//     bme688.setOversamplingTemperature(2);
//     bme688.setOversamplingPressure(5);
//     bme688.setOversamplingHumidity(1);

//     //Select IIR filter for pressure & temperature
//     bme688. setIIRfilterCoefficient(0);

//     //Select heater set-points to be used
//     bme688.setHeaterProfile(1);

//     //Define heater-on times
//     //Convert durations to register codes
//     //Set gas_wait_x<7:0> (time base unit is ms)
//     bme688.setGasWaitTime(0,25,4);
//     bme688.setMode(0);
    
//     ThisThread::sleep_for(5s); 
//     auto can_deep_sleep = sleep_manager_can_deep_sleep();
//     printf("can deep-sleep? %d\r\n", can_deep_sleep);

//     while (1) {
//         printf("Going to sleep\n\r");
//         hal_deepsleep();
//         ThisThread::sleep_for(2000ms); 
//     }
// }