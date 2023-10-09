/***
 *  This example shows powersave function.
***/
#define LED0          PA15
#define LED1          PA1

void WakeupCallback()
{
    Serial.printf("This is Wakeup Callback\r\n");
}

void setup()
{
    Serial.begin(115200);
    pinMode(LED0, INPUT);
    pinMode(LED1, INPUT);
    delay(2000);
  
    Serial.println("RAKwireless System Powersave Example");
    Serial.println("------------------------------------------------------");
  
#if defined(WISBLOCK_BASE_5005) || defined(WISBLOCK_BASE_5005_O)
    api.system.sleep.setup(RUI_WAKEUP_FALLING_EDGE, WB_IO1);	// Set a pin to be wake up pin
#else
#warning Please set the right pin refer to the documentation
    api.system.sleep.setup(RUI_WAKEUP_FALLING_EDGE, 0xFF);
#endif

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
    api.system.sleep.all(900000);
    Serial.print("The timestamp after sleeping: ");
    Serial.print(millis());
    Serial.println(" ms");
}
