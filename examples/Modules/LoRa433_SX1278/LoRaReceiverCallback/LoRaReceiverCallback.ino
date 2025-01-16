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
    // initialize serial
    header("LoRa Receiver Callback", TFT_BLACK);
    M5.Lcd.setTextFont(2);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawString("Please use serial port to view data.", 0, 35, 2);
    Serial.println("LoRa Receiver Callback");

    // frequency in Hz (433E6, 866E6, 915E6)
    if (!LoRa.begin(433E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }

    // register the receive callback
    LoRa.onReceive(onReceive);

    // put the radio into receive mode
    LoRa.receive();
}

void loop()
{
    // do nothing
    LoRa.receive();
}

void onReceive(int packetSize)
{
    // received a packet
    Serial.print("Received packet '");

    // read packet
    for (int i = 0; i < packetSize; i++) {
        Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
}
