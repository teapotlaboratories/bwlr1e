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
