/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Module LoRa433
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 * arduino-LoRa：https://github.com/sandeepmistry/arduino-LoRa
 */

#include <LoRa.h>
#include <M5Stack.h>

void setup()
{
    M5.begin();
    M5.Power.begin();
    // override the default CS, reset, and IRQ pins (optional)
    LoRa.setPins();  // default set CS, reset, IRQ pin
    Serial.println("LoRa Sender");
    M5.Lcd.println("LoRa Sender");

    // frequency in Hz (433E6, 866E6, 915E6)
    if (!LoRa.begin(433E6)) {
        Serial.println("Starting LoRa failed!");
        M5.Lcd.println("Starting LoRa failed!");
        while (1);
    }

    // LoRa.setSyncWord(0x69);
    Serial.println("LoRa init succeeded.");
    M5.Lcd.println("LoRa init succeeded.");
}

void loop()
{
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