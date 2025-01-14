/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit Limit
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

#define KEY_PIN 36  // Define Limit Pin.  定义Limit连接引脚

void setup()
{
    M5.begin();  // Init M5Stack  初始化M5Stack
    M5.Lcd.setTextSize(4);
    M5.Lcd.print(("\n  UNIT-LIMIT\n  Example"));
    pinMode(KEY_PIN, INPUT_PULLUP);  // Init Limit pin.  初始化Limit引脚.
}

void loop()
{
    if (!digitalRead(KEY_PIN)) {  // If Limit was hit.  如果触碰了Limit.
        M5.Lcd.setCursor(0, 130);
        M5.Lcd.print(("  Hit limit!"));
    } else {
        M5.Lcd.setCursor(0, 130);
        M5.Lcd.println(("            "));
    }
    delay(100);
}
