/***
 *  This example shows powersave function.
***/

#define LED0          PA15
#define LED1          PA1
#define I2C_SDA       PA11
#define I2C_SCL       PA12

#include "Zanshin_BME680.h"  // Include the BME680 Sensor library

void DisableBME688(){
  BME680_Class BME680;

  // Initialize BME688
  BME680.setOversampling(TemperatureSensor, Oversample16);  // Use enumerated type values
  BME680.setOversampling(HumiditySensor, Oversample16);     // Use enumerated type values
  BME680.setOversampling(PressureSensor, Oversample16);     // Use enumerated type values
  BME680.setIIRFilter(IIR4);  // Use enumerated type values
  BME680.setGas(320, 150);  // 320�c for 150 milliseconds

  // NOTE: Need to be verified.
  //       Based on power observation, after sensor initialization the power consumption goes down. Thus, we assume that BME688 goes to sleep/standby.
  //       At the moment, the total power consumption is close to the expected power consumption, which is good enough for power save testing.
  //       Later, we need to set BME688 to properly sleep.
  
  // disable I2C
  pinMode(I2C_SDA, INPUT);
  pinMode(I2C_SCL, INPUT);
}

void WakeupCallback()
{
    Serial.printf("This is Wakeup Callback\r\n");
}

void setup()
{
    DisableBME688();
 
    Serial.begin(115200);
    // disable LED
    pinMode(LED0, OUTPUT);
    pinMode(LED1, OUTPUT);

    delay(2000);
  
    Serial.println("RAKwireless System Powersave Example");
    Serial.println("------------------------------------------------------");
    if ( api.system.sleep.registerWakeupCallback(WakeupCallback) == false )
    {
        Serial.println("Create Wakeup Callback failed.");
    }
}

void loop()
{
    Serial.print("The timestamp before sleeping: ");
    Serial.print(millis());
    Serial.println(" ms");
    Serial.println("(Wait 10 seconds or Press any key to wakeup)");
    api.system.sleep.all(10000);
    Serial.print("The timestamp after sleeping: ");
    Serial.print(millis());
    Serial.println(" ms");
}
