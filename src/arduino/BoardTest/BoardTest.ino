#define LED0          PA0
#define LED1          PA1
#define POWER_STATUS  PB12
#define BATT_MEASURE  PA15

void setup() {
  Serial.begin(115200);
  pinMode(POWER_STATUS, INPUT);
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  api.system.restoreDefault();
}

void loop() {
  digitalWrite(LED0, LOW);  
  digitalWrite(LED1, HIGH);
  delay(500);
  digitalWrite(LED0, HIGH);
  digitalWrite(LED1, LOW);
  delay(500);

  int batt_measure_adc = analogRead(BATT_MEASURE);
  Serial.printf("Power Status: %d, Battery Voltage: %d\n", digitalRead(POWER_STATUS), analogRead(BATT_MEASURE));
  delay(100);
}
