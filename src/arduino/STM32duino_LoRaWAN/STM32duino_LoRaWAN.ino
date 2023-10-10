/**
 * This is a very basic example that demonstrates how to configure the
 * library, join the network, send regular packets and print any
 * downlink packets received.
 *
 * Revised BSD License - https://spdx.org/licenses/BSD-3-Clause.html
 */
#include <STM32LoRaWAN.h>

STM32LoRaWAN modem;

static const unsigned long TX_INTERVAL = 60000; /* ms */
unsigned long last_tx = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Start");
  modem.begin(US915);

  // Configure join method by (un)commenting the right method
  // call, and fill in credentials in that method call.
  bool connected = modem.joinOTAA(/* AppEui */ "0E0D0D010E01020E", /* AppKey */ "b512fb72fc87202ab390c1be8e4699f6", /* DevEui */ "AC1F09FFFE0E3A30");
  //bool connected = modem.joinABP(/* DevAddr */ "00000000", /* NwkSKey */ "00000000000000000000000000000000", /* AppSKey */ "00000000000000000000000000000000");

  if (connected) {
    Serial.println("Joined");
  } else {
    Serial.println("Join failed");
    while (true) /* infinite loop */;
  }
}

void send_packet()
{
  uint8_t payload[] = {0xde, 0xad, 0xbe, 0xef};
  modem.setPort(10);
  modem.beginPacket();
  modem.write(payload, sizeof(payload));
  if (modem.endPacket() == sizeof(payload)) {
    Serial.println("Sent packet");
  } else {
    Serial.println("Failed to send packet");
  }

  if (modem.available()) {
    Serial.print("Received packet on port ");
    Serial.print(modem.getDownlinkPort());
    Serial.print(":");
    while (modem.available()) {
      uint8_t b = modem.read();
      Serial.print(" ");
      Serial.print(b >> 4, HEX);
      Serial.print(b & 0xF, HEX);
    }
    Serial.println();
  }
}

void loop()
{
  if (!last_tx || millis() - last_tx > TX_INTERVAL) {
    send_packet();
    last_tx = millis();
  }
}
