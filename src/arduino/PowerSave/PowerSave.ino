/***
 *  This example shows powersave function.
***/

// select which SDK to use:
//   RUI3       - RAKWireless RUI3 SDK
//   STM32DUINO - STM32duino SDK  

#define RUI3
// #define STM32DUINO

#define LED0          PA15
#define LED1          PA1
#define I2C_SDA       PA11
#define I2C_SCL       PA12

#ifdef STM32DUINO
#include "STM32LowPower.h"
#endif

#ifdef RUI3
#include "Zanshin_BME680.h"
#endif

void setup()
{

#ifdef STM32DUINO  
    LowPower.begin();
#endif 
#ifdef RUI3
    // NOTE: need to check. 
    //       For some reason, RUI3 SDK need to disable bme688
    DisableBME688();
#endif

    // disable LED
    pinMode(LED0, OUTPUT);
    pinMode(LED1, OUTPUT);
    digitalWrite( LED0, LOW );
    digitalWrite( LED1, LOW );

    Serial.begin(115200);

    delay(2000);
}

void loop()
{
    Serial.print("The timestamp before sleeping: ");
    Serial.print(millis());
    Serial.println(" ms");
    Serial.println("(Wait 10 seconds or Press any key to wakeup)");

#ifdef RUI3  
    api.system.sleep.all(10000);
#endif
#ifdef STM32DUINO
  LowPower.shutdown(10000);
#endif

    Serial.print("The timestamp after sleeping: ");
    Serial.print(millis());
    Serial.println(" ms");
}

void DisableBME688(){

  Wire.begin();
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
