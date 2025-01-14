/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit Earth
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

void setup()
{
    M5.begin();             // Init M5Stack.  初始化M5Stack
    M5.Power.begin();       // Init power  初始化电源模块
    M5.lcd.setTextSize(2);  // Set the text size to 2.  设置文字大小为2
    M5.Lcd.printf("UNIT_EARTH EXAMPLE\n");
    pinMode(26, INPUT);  // Set pin 26 to input mode.  将引脚26设置为输入模式
    dacWrite(25, 0);     // disable the speak noise.  禁用喇叭
}

void loop()
{
    M5.Lcd.setCursor(0, 80);  // Set the cursor at (0,80).  将光标设置在(0,80)
    M5.Lcd.printf("AnalogRead:%d\n", analogRead(36));
    M5.Lcd.printf("DigitalRead:%d\n", digitalRead(26));
    delay(1000);
}
