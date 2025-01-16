/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Module GPS
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

HardwareSerial GPSRaw(2);

void setup()
{
    M5.begin();
    M5.Power.begin();
    GPSRaw.begin(9600);
    Serial.println("hello");
    M5.Lcd.setTextFont(4);
    M5.Lcd.println(("GPS Raw Example"));
}

void loop()
{
    // put your main code here, to run repeatedly:
    if (Serial.available()) {
        int ch = Serial.read();
        GPSRaw.write(ch);
    }

    if (GPSRaw.available()) {
        int ch = GPSRaw.read();
        Serial.write(ch);
    }
}
