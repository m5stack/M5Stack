/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit OP90
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

void setup()
{
    M5.begin();                // Init M5Stack.  初始化M5Stack
    M5.Power.begin();          // Init power  初始化电源模块
    M5.lcd.setTextSize(2);     // Set the text size to 2.  设置文字大小为2
    M5.Lcd.setCursor(80, 10);  // Set the cursor at (80,10).  将光标设置在(80,10)处
    M5.Lcd.println("90/180 OPTICAL");
    pinMode(36, INPUT_PULLUP);  // Set pin 36 to input pull-up mode.
                                // 设置36号引脚为输入上拉模式
}

void loop()
{
    M5.Lcd.setCursor(80, 120);
    M5.Lcd.printf("IR Receive: %d", digitalRead(36));  // Output the value of pin 36.  输出36号引脚的值
}
