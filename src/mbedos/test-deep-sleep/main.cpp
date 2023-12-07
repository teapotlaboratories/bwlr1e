
#include <mbed.h>
#include "external/bme680/BME680.h"

mbed::DigitalOut led0(PA_15);
mbed::DigitalOut led1(PA_1);

int main()
{
    // disable on-board LED
    led0 = 0;
    led1 = 0;

    // I2C driver need to be free, when not used. BME680 object is in a scoped area
    // so that once it exits, the object is destroyed and I2C is freed.
    // this feature is not in upstream mbed-os sdk. see readme for more details.
    {
        BME680 bme688( I2C_SDA, I2C_SCL, 1);
        auto init_sensor = bme688.init();
        // printf("bme688 sensor successful? %d \n", init_sensor);

        // Select oversampling for T, P, H
        bme688.setOversamplingTemperature(2);
        bme688.setOversamplingPressure(5);
        bme688.setOversamplingHumidity(1);

        // Select IIR filter for pressure & temperature
        bme688. setIIRfilterCoefficient(0);

        // Select heater set-points to be used
        bme688.setHeaterProfile(1);

        // Define heater-on times
        // Convert durations to register codes
        // Set gas_wait_x<7:0> (time base unit is ms)
        bme688.setGasWaitTime(0,25,4);

        // Set it to sleep
        bme688.setMode(0);   
    }
    
    // set I2C pin as high-impedance
    mbed::DigitalIn sda(I2C_SDA);
    mbed::DigitalIn scl(I2C_SCL);
    bool read_dummy = sda;
    read_dummy = scl;

    while (1) {
        ThisThread::sleep_for(10000ms); 
    }
}