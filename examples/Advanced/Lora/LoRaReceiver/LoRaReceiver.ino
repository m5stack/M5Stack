//nedd to download library https://github.com/sandeepmistry/arduino-LoRa.git

#include <M5Stack.h>
#include <SPI.h>
#include <LoRa.h>

#define LORA_CS_PIN   5
#define LORA_RST_PIN  26
#define LORA_IRQ_PIN  36

void setup() {
  
  M5.begin();

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(LORA_CS_PIN, LORA_RST_PIN, LORA_IRQ_PIN); // set CS, reset, IRQ pin

  Serial.println("LoRa Receiver");
  M5.Lcd.println("LoRa Receiver");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    M5.Lcd.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet: \"");
    M5.Lcd.print("Received packet: \"");

    // read packet
    while (LoRa.available()) {
      char ch = (char)LoRa.read();
      Serial.print(ch);
      M5.Lcd.print(ch);
    }

    // print RSSI of packet
    Serial.print("\" with RSSI ");
    Serial.println(LoRa.packetRssi());
    M5.Lcd.print("\" with RSSI ");
    M5.Lcd.println(LoRa.packetRssi());
  }
}
