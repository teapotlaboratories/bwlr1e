#define LED0          PA15
#define LED1          PA1
#define POWER_STATUS  PB12
#define BATT_MEASURE  PB3

#define ADC_TO_BATTERY  0.00118042553

void setup() {
  Serial.begin(115200);
  
  pinMode( BATT_MEASURE, INPUT );
  pinMode( POWER_STATUS, INPUT );
  pinMode( LED0, INPUT );
  pinMode( LED1, INPUT );
  analogReadResolution( 12 );
}

void loop() {
  digitalWrite(LED0, LOW);  
  digitalWrite(LED1, HIGH);
  delay(500);
  digitalWrite(LED0, HIGH);
  digitalWrite(LED1, LOW);
  delay(500);

  int   batt_measure_adc  = analogRead( BATT_MEASURE );
  float batt_measure_volt = batt_measure_adc * ADC_TO_BATTERY;
  Serial.printf("Battery Voltage ADC: %d, Battery Voltage: %f\n", batt_measure_adc, batt_measure_volt);
  delay(100);
}