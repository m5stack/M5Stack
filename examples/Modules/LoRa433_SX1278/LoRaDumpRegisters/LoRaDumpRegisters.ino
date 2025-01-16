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

// LoRa register dump
// This examples shows how to inspect and output the LoRa radio's
// registers on the Serial interface
void header(const char *string, uint16_t color)
{
    M5.Lcd.fillScreen(color);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_MAGENTA, TFT_BLUE);
    M5.Lcd.fillRect(0, 0, 320, 30, TFT_BLUE);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString(string, 160, 3, 4);
}

void setup()
{
    M5.begin();
    M5.Power.begin();
    while (!Serial);

    header("LoRa Dump Registers", TFT_BLACK);

    M5.Lcd.setTextFont(2);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawString("Please use serial port to view data.", 0, 80, 2);

    Serial.println("LoRa Dump Registers\n");

    // override the default CS, reset, and IRQ pins (optional)
    // LoRa.setPins(7, 6, 1); // set CS, reset, IRQ pin

    if (!LoRa.begin(433E6)) {  // initialize ratio at 915 MHz
        Serial.println("LoRa init failed. Check your connections.");
        while (true);  // if failed, do nothing
    }

    LoRa.dumpRegisters(Serial);
}

void loop()
{
}
