/*
  LoRa register dump

  This examples shows how to inspect and output the LoRa radio's
  registers on the Serial interface
*/
#include <M5Stack.h>              // include libraries
#include <M5LoRa.h>

void setup() {
  M5.begin();
  while (!Serial);

  Serial.println("LoRa Dump Registers");

  // override the default CS, reset, and IRQ pins (optional)
  // LoRa.setPins(7, 6, 1); // set CS, reset, IRQ pin

  if (!LoRa.begin(433E6)) {         // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                   // if failed, do nothing
  }

  LoRa.dumpRegisters(Serial);
}


void loop() {
}

