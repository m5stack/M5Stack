/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit RS485-ISO
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

String str = "";

void setup()
{
    M5.begin();
    M5.Lcd.drawString("ISO485", 20, 0, 2);
    Serial2.begin(115200, SERIAL_8N1, 16, 17);
    M5.Lcd.setCursor(0, 20);
}

void loop()
{
    if (M5.BtnA.wasPressed()) {
        Serial2.write("Hello World\r\n");
    }

    if (Serial2.available()) {
        char ch = Serial2.read();
        str += ch;
        if (str.endsWith("\r\n")) {
            Serial.print(str);
            M5.Lcd.print(str);
            str = "";
        }
    }
    M5.update();
}
