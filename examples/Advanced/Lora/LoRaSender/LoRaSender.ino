//nedd to download LoRa library: https://github.com/sandeepmistry/arduino-LoRa.git

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

  Serial.println("LoRa Sender");
  // M5.Lcd.println("LoRa Receiver Callback");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    M5.Lcd.println("Starting LoRa failed!");
    while (1);
  }

}

void loop() {
  // try to parse packet
  static uint32_t counter;

  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(1000);
}