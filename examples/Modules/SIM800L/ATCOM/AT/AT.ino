/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Module SIM800L
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>
#define RX_PIN    16
#define TX_PIN    17
#define RESET_PIN 5

// Description: SIM800L serial transparent transmission debugging program

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
    header("SIM800L AT command", TFT_BLACK);
    M5.Lcd.setTextFont(2);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawString("Please use serial port to Test AT command.", 0, 35, 2);
    // Host serial communication

    // SIM800L serial communication
    Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
    pinMode(RESET_PIN, OUTPUT);
}

void loop()
{
    // AT instruction write
    if (Serial.available()) {
        Serial2.write(Serial.read());
    }

    // AT instruction result
    if (Serial2.available()) {
        Serial.write(Serial2.read());
    }

    delay(10);
}
