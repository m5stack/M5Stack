/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit Hall
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

#define HALL (36)

void setup()
{
    M5.begin();             // Init M5Stack.  初始化M5Stack
    M5.Power.begin();       // Init power  初始化电源模块
    M5.lcd.setTextSize(2);  // Set the text size to 2.  设置文字大小为2
    M5.Lcd.print("     HALL Sensor");
    pinMode(HALL, INPUT);  // Set the pins to which the Hall sensor is connected to
                           // the input mode.  将霍尔传感器所连接的引脚设置为输入模式
}

void loop()
{
    bool status = digitalRead(HALL);
    M5.Lcd.setCursor(20, 80);
    M5.Lcd.printf("Hall status : %d", status);
}
